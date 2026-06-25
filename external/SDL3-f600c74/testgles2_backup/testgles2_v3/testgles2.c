/*
  Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/

#include <SDL3/SDL_test_common.h>
#include <SDL3/SDL_main.h>

#ifdef SDL_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif

#include <stdlib.h>

#if defined(SDL_PLATFORM_IOS) || defined(SDL_PLATFORM_ANDROID) || defined(SDL_PLATFORM_EMSCRIPTEN) || defined(SDL_PLATFORM_WIN32) || defined(SDL_PLATFORM_LINUX) || defined(SDL_PLATFORM_HURD)
#define HAVE_OPENGLES2
#endif

#ifdef HAVE_OPENGLES2

#include <SDL3/SDL_opengles2.h>

typedef struct GLES2_Context
{
#define SDL_PROC(ret, func, params) ret (APIENTRY *func) params;
#include "../src/render/opengles2/SDL_gles2funcs.h"
#undef SDL_PROC
} GLES2_Context;

typedef struct shader_data
{
    GLuint shader_program, shader_frag, shader_vert;

    GLint attr_position;
    GLint attr_color, attr_mvp;

    int angle_x, angle_y, angle_z;

    GLuint position_buffer;
    GLuint color_buffer;
} shader_data;

typedef enum wait_state
{
    WAIT_STATE_GO = 0,
    WAIT_STATE_ENTER_SEM,
    WAIT_STATE_WAITING_ON_SEM,
} wait_state;

typedef struct thread_data
{
    SDL_Thread *thread;
    SDL_Semaphore *suspend_sem;
    SDL_AtomicInt suspended;
    int done;
    int index;
} thread_data;

static SDLTest_CommonState *g_state;
static SDL_GLContext *context = NULL;
static int depth = 16;
static bool suspend_when_occluded;
static GLES2_Context ctx;
static shader_data *datas;

static bool LoadContext(GLES2_Context *data)
{
#ifdef SDL_VIDEO_DRIVER_UIKIT
#define __SDL_NOGETPROCADDR__
#elif defined(SDL_VIDEO_DRIVER_ANDROID)
#define __SDL_NOGETPROCADDR__
#endif

#if defined __SDL_NOGETPROCADDR__
#define SDL_PROC(ret, func, params) data->func = func;
#else
#define SDL_PROC(ret, func, params)                                                            \
    do {                                                                                       \
        data->func = (ret (APIENTRY *) params)SDL_GL_GetProcAddress(#func);                    \
        if (!data->func) {                                                                     \
            return SDL_SetError("Couldn't load GLES2 function %s: %s", #func, SDL_GetError()); \
        }                                                                                      \
    } while (0);
#endif /* __SDL_NOGETPROCADDR__ */

#include "../src/render/opengles2/SDL_gles2funcs.h"
#undef SDL_PROC
    return true;
}

/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc)
{
    int i;

    SDL_free(datas);
    if (context) {
        for (i = 0; i < g_state->num_windows; i++) {
            if (context[i]) {
                SDL_GL_DestroyContext(context[i]);
            }
        }

        SDL_free(context);
    }

    SDLTest_CommonQuit(g_state);
    /* Let 'main()' return normally */
    if (rc != 0) {
        exit(rc);
    }
}

#define GL_CHECK(x)                                                                         \
    x;                                                                                      \
    {                                                                                       \
        GLenum glError = ctx.glGetError();                                                  \
        if (glError != GL_NO_ERROR) {                                                       \
            SDL_Log("glGetError() = %i (0x%.8x) at line %i", glError, glError, __LINE__); \
            quit(1);                                                                        \
        }                                                                                   \
    }

/**
 * Simulates desktop's glRotatef. The matrix is returned in column-major
 * order.
 */
static void
rotate_matrix(float angle, float x, float y, float z, float *r)
{
    float radians, c, s, c1, u[3], length;
    int i, j;

    radians = (angle * SDL_PI_F) / 180.0f;

    c = SDL_cosf(radians);
    s = SDL_sinf(radians);

    c1 = 1.0f - SDL_cosf(radians);

    length = (float)SDL_sqrt(x * x + y * y + z * z);

    u[0] = x / length;
    u[1] = y / length;
    u[2] = z / length;

    for (i = 0; i < 16; i++) {
        r[i] = 0.0;
    }

    r[15] = 1.0;

    for (i = 0; i < 3; i++) {
        r[i * 4 + (i + 1) % 3] = u[(i + 2) % 3] * s;
        r[i * 4 + (i + 2) % 3] = -u[(i + 1) % 3] * s;
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            r[i * 4 + j] += c1 * u[i] * u[j] + (i == j ? c : 0.0f);
        }
    }
}

/**
 * Simulates gluPerspectiveMatrix
 */
static void
perspective_matrix(float fovy, float aspect, float znear, float zfar, float *r)
{
    int i;
    float f;

    f = 1.0f / SDL_tanf((fovy / 180.0f) * SDL_PI_F * 0.5f);

    for (i = 0; i < 16; i++) {
        r[i] = 0.0;
    }

    r[0] = f / aspect;
    r[5] = f;
    r[10] = (znear + zfar) / (znear - zfar);
    r[11] = -1.0f;
    r[14] = (2.0f * znear * zfar) / (znear - zfar);
    r[15] = 0.0f;
}

/**
 * Multiplies lhs by rhs and writes out to r. All matrices are 4x4 and column
 * major. In-place multiplication is supported.
 */
static void
multiply_matrix(const float *lhs, const float *rhs, float *r)
{
    int i, j, k;
    float tmp[16];

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            tmp[j * 4 + i] = 0.0;

            for (k = 0; k < 4; k++) {
                tmp[j * 4 + i] += lhs[k * 4 + i] * rhs[j * 4 + k];
            }
        }
    }

    for (i = 0; i < 16; i++) {
        r[i] = tmp[i];
    }
}

/**
 * Create shader, load in source, compile, dump debug as necessary.
 *
 * shader: Pointer to return created shader ID.
 * source: Passed-in shader source code.
 * shader_type: Passed to GL, e.g. GL_VERTEX_SHADER.
 */
static void
process_shader(GLuint *shader, const char *source, GLint shader_type)
{
    GLint status = GL_FALSE;
    const char *shaders[1] = { NULL };
    char buffer[1024];
    GLsizei length = 0;

    /* Create shader and load into GL. */
    *shader = GL_CHECK(ctx.glCreateShader(shader_type));

    shaders[0] = source;

    GL_CHECK(ctx.glShaderSource(*shader, 1, shaders, NULL));

    /* Clean up shader source. */
    shaders[0] = NULL;

    /* Try compiling the shader. */
    GL_CHECK(ctx.glCompileShader(*shader));
    GL_CHECK(ctx.glGetShaderiv(*shader, GL_COMPILE_STATUS, &status));

    /* Dump debug info (source and log) if compilation failed. */
    if (status != GL_TRUE) {
        ctx.glGetShaderInfoLog(*shader, sizeof(buffer), &length, &buffer[0]);
        buffer[length] = '\0';
        SDL_Log("Shader compilation failed: %s", buffer);
        quit(-1);
    }
}

static void
link_program(struct shader_data *data)
{
    GLint status = GL_FALSE;
    char buffer[1024];
    GLsizei length = 0;

    GL_CHECK(ctx.glAttachShader(data->shader_program, data->shader_vert));
    GL_CHECK(ctx.glAttachShader(data->shader_program, data->shader_frag));
    GL_CHECK(ctx.glLinkProgram(data->shader_program));
    GL_CHECK(ctx.glGetProgramiv(data->shader_program, GL_LINK_STATUS, &status));

    if (status != GL_TRUE) {
        ctx.glGetProgramInfoLog(data->shader_program, sizeof(buffer), &length, &buffer[0]);
        buffer[length] = '\0';
        SDL_Log("Program linking failed: %s", buffer);
        quit(-1);
    }
}

/* 3D data. Vertex range -0.5..0.5 in all axes.
 * Z -0.5 is near, 0.5 is far. */
static const float g_vertices[] = {
    /* Front face. */
    /* Bottom left */
    -0.5,
    0.5,
    -0.5,
    0.5,
    -0.5,
    -0.5,
    -0.5,
    -0.5,
    -0.5,
    /* Top right */
    -0.5,
    0.5,
    -0.5,
    0.5,
    0.5,
    -0.5,
    0.5,
    -0.5,
    -0.5,
    /* Left face */
    /* Bottom left */
    -0.5,
    0.5,
    0.5,
    -0.5,
    -0.5,
    -0.5,
    -0.5,
    -0.5,
    0.5,
    /* Top right */
    -0.5,
    0.5,
    0.5,
    -0.5,
    0.5,
    -0.5,
    -0.5,
    -0.5,
    -0.5,
    /* Top face */
    /* Bottom left */
    -0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    -0.5,
    -0.5,
    0.5,
    -0.5,
    /* Top right */
    -0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    -0.5,
    /* Right face */
    /* Bottom left */
    0.5,
    0.5,
    -0.5,
    0.5,
    -0.5,
    0.5,
    0.5,
    -0.5,
    -0.5,
    /* Top right */
    0.5,
    0.5,
    -0.5,
    0.5,
    0.5,
    0.5,
    0.5,
    -0.5,
    0.5,
    /* Back face */
    /* Bottom left */
    0.5,
    0.5,
    0.5,
    -0.5,
    -0.5,
    0.5,
    0.5,
    -0.5,
    0.5,
    /* Top right */
    0.5,
    0.5,
    0.5,
    -0.5,
    0.5,
    0.5,
    -0.5,
    -0.5,
    0.5,
    /* Bottom face */
    /* Bottom left */
    -0.5,
    -0.5,
    -0.5,
    0.5,
    -0.5,
    0.5,
    -0.5,
    -0.5,
    0.5,
    /* Top right */
    -0.5,
    -0.5,
    -0.5,
    0.5,
    -0.5,
    -0.5,
    0.5,
    -0.5,
    0.5,
};

static const float g_colors[] = {
    /* Front face */
    /* Bottom left */
    1.0, 0.0, 0.0, /* red */
    0.0, 0.0, 1.0, /* blue */
    0.0, 1.0, 0.0, /* green */
    /* Top right */
    1.0, 0.0, 0.0, /* red */
    1.0, 1.0, 0.0, /* yellow */
    0.0, 0.0, 1.0, /* blue */
    /* Left face */
    /* Bottom left */
    1.0, 1.0, 1.0, /* white */
    0.0, 1.0, 0.0, /* green */
    0.0, 1.0, 1.0, /* cyan */
    /* Top right */
    1.0, 1.0, 1.0, /* white */
    1.0, 0.0, 0.0, /* red */
    0.0, 1.0, 0.0, /* green */
    /* Top face */
    /* Bottom left */
    1.0, 1.0, 1.0, /* white */
    1.0, 1.0, 0.0, /* yellow */
    1.0, 0.0, 0.0, /* red */
    /* Top right */
    1.0, 1.0, 1.0, /* white */
    0.0, 0.0, 0.0, /* black */
    1.0, 1.0, 0.0, /* yellow */
    /* Right face */
    /* Bottom left */
    1.0, 1.0, 0.0, /* yellow */
    1.0, 0.0, 1.0, /* magenta */
    0.0, 0.0, 1.0, /* blue */
    /* Top right */
    1.0, 1.0, 0.0, /* yellow */
    0.0, 0.0, 0.0, /* black */
    1.0, 0.0, 1.0, /* magenta */
    /* Back face */
    /* Bottom left */
    0.0, 0.0, 0.0, /* black */
    0.0, 1.0, 1.0, /* cyan */
    1.0, 0.0, 1.0, /* magenta */
    /* Top right */
    0.0, 0.0, 0.0, /* black */
    1.0, 1.0, 1.0, /* white */
    0.0, 1.0, 1.0, /* cyan */
    /* Bottom face */
    /* Bottom left */
    0.0, 1.0, 0.0, /* green */
    1.0, 0.0, 1.0, /* magenta */
    0.0, 1.0, 1.0, /* cyan */
    /* Top right */
    0.0, 1.0, 0.0, /* green */
    0.0, 0.0, 1.0, /* blue */
    1.0, 0.0, 1.0, /* magenta */
};

static const char *g_shader_vert_src =
    " attribute vec4 av4position; "
    " attribute vec3 av3color; "
    " uniform mat4 mvp; "
    " varying vec3 vv3color; "
    " void main() { "
    "    vv3color = av3color; "
    "    gl_Position = mvp * av4position; "
    " } ";

static const char *g_shader_frag_src =
    " precision lowp float; "
    " varying vec3 vv3color; "
    " void main() { "
    "    gl_FragColor = vec4(vv3color, 1.0); "
    " } ";

static void
Render(unsigned int width, unsigned int height, shader_data *data)
{
    float matrix_rotate[16], matrix_modelview[16], matrix_perspective[16], matrix_mvp[16];

    /*
     * Do some rotation with Euler angles. It is not a fixed axis as
     * quaterions would be, but the effect is cool.
     */
    rotate_matrix((float)data->angle_x, 1.0f, 0.0f, 0.0f, matrix_modelview);
    rotate_matrix((float)data->angle_y, 0.0f, 1.0f, 0.0f, matrix_rotate);

    multiply_matrix(matrix_rotate, matrix_modelview, matrix_modelview);

    rotate_matrix((float)data->angle_z, 0.0f, 1.0f, 0.0f, matrix_rotate);

    multiply_matrix(matrix_rotate, matrix_modelview, matrix_modelview);

    /* Pull the camera back from the cube */
    matrix_modelview[14] -= 2.5f;

    perspective_matrix(45.0f, (float)width / height, 0.01f, 100.0f, matrix_perspective);
    multiply_matrix(matrix_perspective, matrix_modelview, matrix_mvp);

    GL_CHECK(ctx.glUniformMatrix4fv(data->attr_mvp, 1, GL_FALSE, matrix_mvp));

    data->angle_x += 3;
    data->angle_y += 2;
    data->angle_z += 1;

    if (data->angle_x >= 360) {
        data->angle_x -= 360;
    }
    if (data->angle_x < 0) {
        data->angle_x += 360;
    }
    if (data->angle_y >= 360) {
        data->angle_y -= 360;
    }
    if (data->angle_y < 0) {
        data->angle_y += 360;
    }
    if (data->angle_z >= 360) {
        data->angle_z -= 360;
    }
    if (data->angle_z < 0) {
        data->angle_z += 360;
    }

    GL_CHECK(ctx.glViewport(0, 0, width, height));
    GL_CHECK(ctx.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    GL_CHECK(ctx.glDrawArrays(GL_TRIANGLES, 0, 36));
}

static int g_done;
static Uint32 frames;
#ifndef SDL_PLATFORM_EMSCRIPTEN
static thread_data *threads;
#endif

static void
render_window(int index)
{
    int w, h;

    if (!g_state->windows[index]) {
        return;
    }

    if (!SDL_GL_MakeCurrent(g_state->windows[index], context[index])) {
        SDL_Log("SDL_GL_MakeCurrent(): %s", SDL_GetError());
        return;
    }

    SDL_GetWindowSizeInPixels(g_state->windows[index], &w, &h);
    Render(w, h, &datas[index]);
    SDL_GL_SwapWindow(g_state->windows[index]);
    ++frames;
}

#ifndef SDL_PLATFORM_EMSCRIPTEN
static int SDLCALL
render_thread_fn(void *render_ctx)
{
    thread_data *thread = render_ctx;

    while (!g_done && !thread->done && g_state->windows[thread->index]) {
        if (SDL_CompareAndSwapAtomicInt(&thread->suspended, WAIT_STATE_ENTER_SEM, WAIT_STATE_WAITING_ON_SEM)) {
            SDL_WaitSemaphore(thread->suspend_sem);
        }
        render_window(thread->index);
    }

    SDL_GL_MakeCurrent(g_state->windows[thread->index], NULL);
    return 0;
}

static thread_data *GetThreadDataForWindow(SDL_WindowID id)
{
    int i;
    SDL_Window *window = SDL_GetWindowFromID(id);
    if (window) {
        for (i = 0; i < g_state->num_windows; ++i) {
            if (window == g_state->windows[i]) {
                return &threads[i];
            }
        }
    }
    return NULL;
}

static void
loop_threaded(void)
{
    SDL_Event event;
    thread_data *tdata;

    /* Wait for events */
    while (SDL_WaitEvent(&event) && !g_done) {
        if (suspend_when_occluded && event.type == SDL_EVENT_WINDOW_OCCLUDED) {
            tdata = GetThreadDataForWindow(event.window.windowID);
            if (tdata) {
                SDL_CompareAndSwapAtomicInt(&tdata->suspended, WAIT_STATE_GO, WAIT_STATE_ENTER_SEM);
            }
        } else if (suspend_when_occluded && event.type == SDL_EVENT_WINDOW_EXPOSED) {
            tdata = GetThreadDataForWindow(event.window.windowID);
            if (tdata) {
                if (SDL_SetAtomicInt(&tdata->suspended, WAIT_STATE_GO) == WAIT_STATE_WAITING_ON_SEM) {
                    SDL_SignalSemaphore(tdata->suspend_sem);
                }
            }
        } else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            tdata = GetThreadDataForWindow(event.window.windowID);
            if (tdata) {
                /* Stop the render thread when the window is closed */
                tdata->done = 1;
                if (tdata->thread) {
                    SDL_SetAtomicInt(&tdata->suspended, WAIT_STATE_GO);
                    SDL_SignalSemaphore(tdata->suspend_sem);
                    SDL_WaitThread(tdata->thread, NULL);
                    tdata->thread = NULL;
                    SDL_DestroySemaphore(tdata->suspend_sem);
                }
                break;
            }
        }
        SDLTest_CommonEvent(g_state, &event, &g_done);
    }
}
#endif

static void
loop(void)
{
    SDL_Event event;
    int i;
    int active_windows = 0;

    /* Check for events */
    while (SDL_PollEvent(&event) && !g_done) {
        SDLTest_CommonEvent(g_state, &event, &g_done);
    }
    if (!g_done) {
        for (i = 0; i < g_state->num_windows; ++i) {
            if (g_state->windows[i] == NULL ||
                (suspend_when_occluded && (SDL_GetWindowFlags(g_state->windows[i]) & SDL_WINDOW_OCCLUDED))) {
                continue;
            }
            ++active_windows;
            render_window(i);
        }
    }
#ifdef SDL_PLATFORM_EMSCRIPTEN
    else {
        emscripten_cancel_main_loop();
    }
#endif

    /* If all windows are occluded, throttle event polling to 15hz. */
    if (!g_done && !active_windows) {
        SDL_DelayNS(SDL_NS_PER_SECOND / 15);
    }
}

int main(int argc, char *argv[])
{
    int fsaa, accel, threaded;
    int value;
    int i;
    const SDL_DisplayMode *mode;
    Uint64 then, now;
    shader_data *data;

    /* Initialize parameters */
    fsaa = 0;
    accel = 0;
    threaded = 0;

    /* Initialize test framework */
    g_state = SDLTest_CommonCreateState(argv, SDL_INIT_VIDEO);
    if (!g_state) {
        return 1;
    }
    
    /* Set OpenGL parameters */
    g_state->window_flags |= SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    g_state->gl_red_size = 5;
    g_state->gl_green_size = 5;
    g_state->gl_blue_size = 5;
    g_state->gl_depth_size = depth;
    g_state->gl_major_version = 2;
    g_state->gl_minor_version = 0;
    g_state->gl_profile_mask = SDL_GL_CONTEXT_PROFILE_ES;

    if (fsaa) {
        g_state->gl_multisamplebuffers = 1;
        g_state->gl_multisamplesamples = fsaa;
    }
    if (accel) {
        g_state->gl_accelerated = 1;
    }
    if (!SDLTest_CommonInit(g_state)) {
        quit(2);
        return 0;
    }

    context = (SDL_GLContext *)SDL_calloc(g_state->num_windows, sizeof(*context));
    if (!context) {
        SDL_Log("Out of memory!");
        quit(2);
    }

    /* Create OpenGL ES contexts */
    for (i = 0; i < g_state->num_windows; i++) {
        context[i] = SDL_GL_CreateContext(g_state->windows[i]);
        if (!context[i]) {
            SDL_Log("SDL_GL_CreateContext(): %s", SDL_GetError());
            quit(2);
        }
    }

    /* Important: call this *after* creating the context */
    if (!LoadContext(&ctx)) {
        SDL_Log("Could not load GLES2 functions");
        quit(2);
        return 0;
    }

    SDL_GL_SetSwapInterval(g_state->render_vsync);

    mode = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
    SDL_Log("Threaded  : %s", threaded ? "yes" : "no");
    if (mode) {
        SDL_Log("Screen bpp: %d", SDL_BITSPERPIXEL(mode->format));
        SDL_Log("%s", "");
    }
    SDL_Log("Vendor     : %s", ctx.glGetString(GL_VENDOR));
    SDL_Log("Renderer   : %s", ctx.glGetString(GL_RENDERER));
    SDL_Log("Version    : %s", ctx.glGetString(GL_VERSION));
    SDL_Log("Extensions : %s", ctx.glGetString(GL_EXTENSIONS));
    SDL_Log("%s", "");

    if (SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &value)) {
        SDL_Log("SDL_GL_RED_SIZE: requested %d, got %d", 5, value);
    } else {
        SDL_Log("Failed to get SDL_GL_RED_SIZE: %s",
                SDL_GetError());
    }
    if (SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &value)) {
        SDL_Log("SDL_GL_GREEN_SIZE: requested %d, got %d", 5, value);
    } else {
        SDL_Log("Failed to get SDL_GL_GREEN_SIZE: %s",
                SDL_GetError());
    }
    if (SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &value)) {
        SDL_Log("SDL_GL_BLUE_SIZE: requested %d, got %d", 5, value);
    } else {
        SDL_Log("Failed to get SDL_GL_BLUE_SIZE: %s",
                SDL_GetError());
    }
    if (SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value)) {
        SDL_Log("SDL_GL_DEPTH_SIZE: requested %d, got %d", depth, value);
    } else {
        SDL_Log("Failed to get SDL_GL_DEPTH_SIZE: %s",
                SDL_GetError());
    }
    if (fsaa) {
        if (SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &value)) {
            SDL_Log("SDL_GL_MULTISAMPLEBUFFERS: requested 1, got %d", value);
        } else {
            SDL_Log("Failed to get SDL_GL_MULTISAMPLEBUFFERS: %s",
                    SDL_GetError());
        }
        if (SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &value)) {
            SDL_Log("SDL_GL_MULTISAMPLESAMPLES: requested %d, got %d", fsaa,
                    value);
        } else {
            SDL_Log("Failed to get SDL_GL_MULTISAMPLESAMPLES: %s",
                    SDL_GetError());
        }
    }
    if (accel) {
        if (SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &value)) {
            SDL_Log("SDL_GL_ACCELERATED_VISUAL: requested 1, got %d", value);
        } else {
            SDL_Log("Failed to get SDL_GL_ACCELERATED_VISUAL: %s",
                    SDL_GetError());
        }
    }

    datas = (shader_data *)SDL_calloc(g_state->num_windows, sizeof(shader_data));

    /* Set rendering settings for each context */
    for (i = 0; i < g_state->num_windows; ++i) {

        int w, h;
        if (!SDL_GL_MakeCurrent(g_state->windows[i], context[i])) {
            SDL_Log("SDL_GL_MakeCurrent(): %s", SDL_GetError());

            /* Continue for next window */
            continue;
        }
        SDL_GetWindowSizeInPixels(g_state->windows[i], &w, &h);
        ctx.glViewport(0, 0, w, h);

        data = &datas[i];
        data->angle_x = 0;
        data->angle_y = 0;
        data->angle_z = 0;

        /* Shader Initialization */
        process_shader(&data->shader_vert, g_shader_vert_src, GL_VERTEX_SHADER);
        process_shader(&data->shader_frag, g_shader_frag_src, GL_FRAGMENT_SHADER);

        /* Create shader_program (ready to attach shaders) */
        data->shader_program = GL_CHECK(ctx.glCreateProgram());

        /* Attach shaders and link shader_program */
        link_program(data);

        /* Get attribute locations of non-fixed attributes like color and texture coordinates. */
        data->attr_position = GL_CHECK(ctx.glGetAttribLocation(data->shader_program, "av4position"));
        data->attr_color = GL_CHECK(ctx.glGetAttribLocation(data->shader_program, "av3color"));

        /* Get uniform locations */
        data->attr_mvp = GL_CHECK(ctx.glGetUniformLocation(data->shader_program, "mvp"));

        GL_CHECK(ctx.glUseProgram(data->shader_program));

        /* Enable attributes for position, color and texture coordinates etc. */
        GL_CHECK(ctx.glEnableVertexAttribArray(data->attr_position));
        GL_CHECK(ctx.glEnableVertexAttribArray(data->attr_color));

        /* Populate attributes for position, color and texture coordinates etc. */

        GL_CHECK(ctx.glGenBuffers(1, &data->position_buffer));
        GL_CHECK(ctx.glBindBuffer(GL_ARRAY_BUFFER, data->position_buffer));
        GL_CHECK(ctx.glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW));
        GL_CHECK(ctx.glVertexAttribPointer(data->attr_position, 3, GL_FLOAT, GL_FALSE, 0, NULL));
        GL_CHECK(ctx.glBindBuffer(GL_ARRAY_BUFFER, 0));

        GL_CHECK(ctx.glGenBuffers(1, &data->color_buffer));
        GL_CHECK(ctx.glBindBuffer(GL_ARRAY_BUFFER, data->color_buffer));
        GL_CHECK(ctx.glBufferData(GL_ARRAY_BUFFER, sizeof(g_colors), g_colors, GL_STATIC_DRAW));
        GL_CHECK(ctx.glVertexAttribPointer(data->attr_color, 3, GL_FLOAT, GL_FALSE, 0, NULL));
        GL_CHECK(ctx.glBindBuffer(GL_ARRAY_BUFFER, 0));

        GL_CHECK(ctx.glEnable(GL_CULL_FACE));
        GL_CHECK(ctx.glEnable(GL_DEPTH_TEST));

        SDL_GL_MakeCurrent(g_state->windows[i], NULL);
    }

    /* Main render loop */
    frames = 0;
    then = SDL_GetTicks();
    g_done = 0;

#ifdef SDL_PLATFORM_EMSCRIPTEN
    emscripten_set_main_loop(loop, 0, 1);
#else
    if (threaded) {
        threads = (thread_data *)SDL_calloc(g_state->num_windows, sizeof(thread_data));

        /* Start a render thread for each window */
        for (i = 0; i < g_state->num_windows; ++i) {
            threads[i].index = i;
            SDL_SetAtomicInt(&threads[i].suspended, 0);
            threads[i].suspend_sem = SDL_CreateSemaphore(0);
            threads[i].thread = SDL_CreateThread(render_thread_fn, "RenderThread", &threads[i]);
        }

        while (!g_done) {
            loop_threaded();
        }

        /* Join the remaining render threads (if any) */
        for (i = 0; i < g_state->num_windows; ++i) {
            threads[i].done = 1;
            if (threads[i].thread) {
                SDL_WaitThread(threads[i].thread, NULL);
            }
        }
        SDL_free(threads);
    } else {
        while (!g_done) {
            loop();
        }
    }
#endif

    /* Print out some timing information */
    now = SDL_GetTicks();
    if (now > then) {
        SDL_Log("%2.2f frames per second",
                ((double)frames * 1000) / (now - then));
    }
#ifndef SDL_PLATFORM_ANDROID
    quit(0);
#endif
    return 0;
}


















// tests can quit after N ms
static Uint32 SDLCALL quit_after_ms_cb(void *userdata, SDL_TimerID timerID, Uint32 interval)
{
    SDL_Event event;
    event.type = SDL_EVENT_QUIT;
    event.common.timestamp = 0;
    SDL_PushEvent(&event);
    return 0;
}

//keep
SDLTest_CommonState *SDLTest_CommonCreateState(char **argv, SDL_InitFlags flags)
{
    int i;
    SDLTest_CommonState *state;

    /* Do this first so we catch all allocations */
    for (i = 1; argv[i]; ++i) {
        if (SDL_strcasecmp(argv[i], "--trackmem") == 0) {
            //SDLTest_TrackAllocations();
        } else if (SDL_strcasecmp(argv[i], "--randmem") == 0) {
            //SDLTest_RandFillAllocations();
        }
    }

    state = (SDLTest_CommonState *)SDL_calloc(1, sizeof(*state));
    if (!state) {
        return NULL;
    }

    /* Initialize some defaults */
    state->argv = argv;
    state->flags = flags;
    state->window_title = argv[0];
    state->window_flags = SDL_WINDOW_HIDDEN;
    state->window_x = SDL_WINDOWPOS_UNDEFINED;
    state->window_y = SDL_WINDOWPOS_UNDEFINED;
    state->window_w = DEFAULT_WINDOW_WIDTH;
    state->window_h = DEFAULT_WINDOW_HEIGHT;
    state->logical_presentation = SDL_LOGICAL_PRESENTATION_DISABLED;
    state->num_windows = 1;
    state->audio_freq = 22050;
    state->audio_format = SDL_AUDIO_S16;
    state->audio_channels = 2;

    /* Set some very sane GL defaults */
    state->gl_red_size = 8;
    state->gl_green_size = 8;
    state->gl_blue_size = 8;
    state->gl_alpha_size = 8;
    state->gl_buffer_size = 0;
    state->gl_depth_size = 16;
    state->gl_stencil_size = 0;
    state->gl_double_buffer = 1;
    state->gl_accum_red_size = 0;
    state->gl_accum_green_size = 0;
    state->gl_accum_blue_size = 0;
    state->gl_accum_alpha_size = 0;
    state->gl_stereo = 0;
    state->gl_multisamplebuffers = 0;
    state->gl_multisamplesamples = 0;
    state->gl_retained_backing = 1;
    state->gl_accelerated = -1;
    state->gl_debug = 0;

    return state;
}

//keep
bool SDLTest_CommonInit(SDLTest_CommonState *state)
{
    int i, n, w, h;

    if (state->flags & SDL_INIT_VIDEO) {
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
            SDL_Log("Couldn't initialize video driver: %s",
                    SDL_GetError());
            return false;
        }

        /* Upload GL settings */
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, state->gl_red_size);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, state->gl_green_size);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, state->gl_blue_size);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, state->gl_alpha_size);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, state->gl_double_buffer);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, state->gl_buffer_size);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, state->gl_depth_size);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, state->gl_stencil_size);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, state->gl_accum_red_size);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, state->gl_accum_green_size);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, state->gl_accum_blue_size);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, state->gl_accum_alpha_size);
        SDL_GL_SetAttribute(SDL_GL_STEREO, state->gl_stereo);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, state->gl_release_behavior);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, state->gl_multisamplebuffers);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, state->gl_multisamplesamples);
        if (state->gl_accelerated >= 0) {
            SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,
                                state->gl_accelerated);
        }
        SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, state->gl_retained_backing);
        if (state->gl_major_version) {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, state->gl_major_version);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, state->gl_minor_version);
        }
        if (state->gl_debug) {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        }
        if (state->gl_profile_mask) {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, state->gl_profile_mask);
        }

        state->displayID = SDL_GetPrimaryDisplay();
        if (state->display_index > 0) {
            SDL_DisplayID *displays = SDL_GetDisplays(&n);
            if (state->display_index < n) {
                state->displayID = displays[state->display_index];
            }
            SDL_free(displays);

            if (SDL_WINDOWPOS_ISUNDEFINED(state->window_x)) {
                state->window_x = SDL_WINDOWPOS_UNDEFINED_DISPLAY(state->displayID);
                state->window_y = SDL_WINDOWPOS_UNDEFINED_DISPLAY(state->displayID);
            } else if (SDL_WINDOWPOS_ISCENTERED(state->window_x)) {
                state->window_x = SDL_WINDOWPOS_CENTERED_DISPLAY(state->displayID);
                state->window_y = SDL_WINDOWPOS_CENTERED_DISPLAY(state->displayID);
            }
        }

        {
            bool include_high_density_modes = false;
            if (state->window_flags & SDL_WINDOW_HIGH_PIXEL_DENSITY) {
                include_high_density_modes = true;
            }
            SDL_GetClosestFullscreenDisplayMode(state->displayID, state->window_w, state->window_h, state->refresh_rate, include_high_density_modes, &state->fullscreen_mode);
        }

        state->windows =
            (SDL_Window **)SDL_calloc(state->num_windows,
                                      sizeof(*state->windows));
        state->renderers =
            (SDL_Renderer **)SDL_calloc(state->num_windows,
                                        sizeof(*state->renderers));
        state->targets =
            (SDL_Texture **)SDL_calloc(state->num_windows,
                                       sizeof(*state->targets));
        if (!state->windows || !state->renderers) {
            SDL_Log("Out of memory!");
            return false;
        }
        for (i = 0; i < state->num_windows; ++i) {
            char title[1024];
            SDL_Rect r;
            SDL_PropertiesID props;

            if (state->fill_usable_bounds) {
                SDL_GetDisplayUsableBounds(state->displayID, &r);
            } else {
                r.x = state->window_x;
                r.y = state->window_y;
                r.w = state->window_w;
                r.h = state->window_h;
                if (state->auto_scale_content) {
                    float scale = SDL_GetDisplayContentScale(state->displayID);
                    r.w = (int)SDL_ceilf(r.w * scale);
                    r.h = (int)SDL_ceilf(r.h * scale);
                }
            }

            if (state->num_windows > 1) {
                (void)SDL_snprintf(title, SDL_arraysize(title), "%s %d",
                                   state->window_title, i + 1);
            } else {
                SDL_strlcpy(title, state->window_title, SDL_arraysize(title));
            }
            props = SDL_CreateProperties();
            SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, title);
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, r.x);
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, r.y);
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, r.w);
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, r.h);
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER, state->window_flags);
            state->windows[i] = SDL_CreateWindowWithProperties(props);
            SDL_DestroyProperties(props);
            if (!state->windows[i]) {
                SDL_Log("Couldn't create window: %s",
                        SDL_GetError());
                return false;
            }
            if (state->window_minW || state->window_minH) {
                SDL_SetWindowMinimumSize(state->windows[i], state->window_minW, state->window_minH);
            }
            if (state->window_maxW || state->window_maxH) {
                SDL_SetWindowMaximumSize(state->windows[i], state->window_maxW, state->window_maxH);
            }
            if (state->window_min_aspect != 0.f || state->window_max_aspect != 0.f) {
                SDL_SetWindowAspectRatio(state->windows[i], state->window_min_aspect, state->window_max_aspect);
            }
            SDL_GetWindowSize(state->windows[i], &w, &h);
            if (!(state->window_flags & SDL_WINDOW_RESIZABLE) && (w != r.w || h != r.h)) {
                SDL_Log("Window requested size %dx%d, got %dx%d", r.w, r.h, w, h);
                state->window_w = w;
                state->window_h = h;
            }
            if (state->window_flags & SDL_WINDOW_FULLSCREEN) {
                if (state->fullscreen_exclusive) {
                    SDL_SetWindowFullscreenMode(state->windows[i], &state->fullscreen_mode);
                }
                SDL_SetWindowFullscreen(state->windows[i], true);
            }

            if (!SDL_RectEmpty(&state->confine)) {
                SDL_SetWindowMouseRect(state->windows[i], &state->confine);
            }

            if (!state->skip_renderer && (state->renderdriver || !(state->window_flags & (SDL_WINDOW_OPENGL | SDL_WINDOW_VULKAN | SDL_WINDOW_METAL)))) {
                state->renderers[i] = SDL_CreateRenderer(state->windows[i], state->renderdriver);
                if (!state->renderers[i]) {
                    SDL_Log("Couldn't create renderer: %s",
                            SDL_GetError());
                    return false;
                }
                if (state->logical_w == 0 || state->logical_h == 0) {
                    state->logical_w = state->window_w;
                    state->logical_h = state->window_h;
                }
                if (state->render_vsync) {
                    SDL_SetRenderVSync(state->renderers[i], state->render_vsync);
                }
                if (!SDL_SetRenderLogicalPresentation(state->renderers[i], state->logical_w, state->logical_h, state->logical_presentation)) {
                    SDL_Log("Couldn't set logical presentation: %s", SDL_GetError());
                    return false;
                }
                if (state->scale != 0.0f) {
                    SDL_SetRenderScale(state->renderers[i], state->scale, state->scale);
                }
            }

            SDL_ShowWindow(state->windows[i]);
        }
        if (state->hide_cursor) {
            SDL_HideCursor();
        }
    }

    if (state->flags & SDL_INIT_AUDIO) {
        if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
            SDL_Log("Couldn't initialize audio driver: %s",
                    SDL_GetError());
            return false;
        }

        const SDL_AudioSpec spec = { state->audio_format, state->audio_channels, state->audio_freq };
        state->audio_id = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
        if (!state->audio_id) {
            SDL_Log("Couldn't open audio: %s", SDL_GetError());
            return false;
        }
    }

    SDL_InitSubSystem(state->flags);


    if (state->quit_after_ms_interval) {
        state->quit_after_ms_timer = SDL_AddTimer(state->quit_after_ms_interval, quit_after_ms_cb, NULL);
    }

    return true;
}

SDL_AppResult SDLTest_CommonEventMainCallbacks(SDLTest_CommonState *state, const SDL_Event *event)
{
    switch (event->type) {
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
        if (state->auto_scale_content) {
            SDL_Window *window = SDL_GetWindowFromEvent(event);
            if (window) {
                float scale = SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(window));
                int w = state->window_w;
                int h = state->window_h;

                w = (int)SDL_ceilf(w * scale);
                h = (int)SDL_ceilf(h * scale);
                SDL_SetWindowSize(window, w, h);
            }
        }
        break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        if (state->flash_on_focus_loss) {
            SDL_Window *window = SDL_GetWindowFromEvent(event);
            if (window) {
                SDL_FlashWindow(window, SDL_FLASH_UNTIL_FOCUSED);
            }
        }
        break;
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    {
        SDL_Window *window = SDL_GetWindowFromEvent(event);
        if (window) {
            SDL_HideWindow(window);
        }
        break;
    }
    case SDL_EVENT_KEY_DOWN:
    {
        //bool withControl = !!(event->key.mod & SDL_KMOD_CTRL);
        //bool withShift = !!(event->key.mod & SDL_KMOD_SHIFT);
        bool withAlt = !!(event->key.mod & SDL_KMOD_ALT);

        switch (event->key.key) {
            /* Add hotkeys here */
        case SDLK_RETURN:
            if (withAlt) {
                /* Alt-Enter toggle fullscreen desktop */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    SDL_WindowFlags flags = SDL_GetWindowFlags(window);
                    if (!(flags & SDL_WINDOW_FULLSCREEN) ||
                        SDL_GetWindowFullscreenMode(window)) {
                        SDL_SetWindowFullscreenMode(window, NULL);
                        SDL_SetWindowFullscreen(window, true);
                    } else {
                        SDL_SetWindowFullscreen(window, false);
                    }
                }
            }
            break;
        case SDLK_ESCAPE:
            return SDL_APP_SUCCESS;
        default:
            break;
        }
        break;
    }
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

//keep
void SDLTest_CommonEvent(SDLTest_CommonState *state, SDL_Event *event, int *done)
{
    if (SDLTest_CommonEventMainCallbacks(state, event) != SDL_APP_CONTINUE) {
        *done = 1;
    }
}

//keep
void SDLTest_CommonQuit(SDLTest_CommonState *state)
{
    if (state) {
        int i;

        if (state->targets) {
            for (i = 0; i < state->num_windows; ++i) {
                if (state->targets[i]) {
                    SDL_DestroyTexture(state->targets[i]);
                }
            }
            SDL_free(state->targets);
        }
        if (state->renderers) {
            for (i = 0; i < state->num_windows; ++i) {
                if (state->renderers[i]) {
                    SDL_DestroyRenderer(state->renderers[i]);
                }
            }
            SDL_free(state->renderers);
        }
        if (state->windows) {
            for (i = 0; i < state->num_windows; i++) {
                SDL_DestroyWindow(state->windows[i]);
            }
            SDL_free(state->windows);
        }

        if (state->quit_after_ms_timer) {
            SDL_RemoveTimer(state->quit_after_ms_timer);
        }
    }
    SDL_Quit();
    //SDLTest_CommonDestroyState(state);
    SDL_free(state);
    //SDLTest_LogAllocations();
}





#else /* HAVE_OPENGLES2 */

int main(int argc, char *argv[])
{
    SDL_Log("No OpenGL ES support on this system");
    return 1;
}

#endif /* HAVE_OPENGLES2 */
