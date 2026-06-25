#define MY_USE_FPS 24

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengles2.h>
#include <stdlib.h>

#define DEFAULT_WINDOW_WIDTH  640
#define DEFAULT_WINDOW_HEIGHT 480

typedef struct
{
    int logical_w;
    int logical_h;
    float scale;
    int depth;
    float refresh_rate;
    bool fill_usable_bounds;
    bool fullscreen_exclusive;
    SDL_DisplayMode fullscreen_mode;
    SDL_Window **windows;
    const char *gpudriver;

    /* Renderer info */
    const char *renderdriver;
    int render_vsync;
    bool skip_renderer;
    SDL_Renderer **renderers;
    SDL_Texture **targets;
} CommonState_my;

CommonState_my *CommonCreateState(/*char **argv, SDL_InitFlags flags*/)
{
    CommonState_my *state;

    state = (CommonState_my *)SDL_calloc(1, sizeof(*state));
    if (!state) {
        return NULL;
    }
    return state;
}

bool CommonInit(CommonState_my *state, const char *window_title, int depth)
{
SDL_WindowFlags window_flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
int window_x = SDL_WINDOWPOS_UNDEFINED;
int window_y = SDL_WINDOWPOS_UNDEFINED;
int window_w = DEFAULT_WINDOW_WIDTH;
int window_h = DEFAULT_WINDOW_HEIGHT;
SDL_DisplayID displayID;

    int w, h;

    int gl_red_size = 0;
    int gl_green_size = 0;
    int gl_blue_size = 0;
    int gl_alpha_size = 0;
    int gl_buffer_size = 0;
    int gl_depth_size = 0;
    int gl_stencil_size = 0;
    int gl_double_buffer = 0;
    int gl_accum_red_size = 0;
    int gl_accum_green_size = 0;
    int gl_accum_blue_size = 0;
    int gl_accum_alpha_size = 0;
    int gl_stereo = 0;
    int gl_release_behavior = 0;
    int gl_multisamplebuffers = 0;
    int gl_multisamplesamples = 0;
    int gl_retained_backing = 0;
    int gl_accelerated = 0;
    int gl_major_version = 0;
    int gl_minor_version = 0;
    int gl_debug = 0;
    int gl_profile_mask = 0;
    
gl_red_size = 8;
gl_green_size = 8;
gl_blue_size = 8;
gl_alpha_size = 8;
gl_buffer_size = 0;
gl_depth_size = 16;
gl_stencil_size = 0;
gl_double_buffer = 1;
gl_accum_red_size = 0;
gl_accum_green_size = 0;
gl_accum_blue_size = 0;
gl_accum_alpha_size = 0;
gl_stereo = 0;
gl_multisamplebuffers = 0;
gl_multisamplesamples = 0;
gl_retained_backing = 1;
gl_accelerated = -1;
gl_debug = 0;    

gl_red_size = 5;
gl_green_size = 5;
gl_blue_size = 5;
gl_depth_size = depth;
gl_major_version = 2;
gl_minor_version = 0;
gl_profile_mask = SDL_GL_CONTEXT_PROFILE_ES;


        if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
            SDL_Log("Couldn't initialize video driver: %s",
                    SDL_GetError());
            return false;
        }

        /* Upload GL settings */
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, gl_red_size);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, gl_green_size);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, gl_blue_size);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, gl_alpha_size);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, gl_double_buffer);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, gl_buffer_size);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, gl_depth_size);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, gl_stencil_size);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, gl_accum_red_size);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, gl_accum_green_size);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, gl_accum_blue_size);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, gl_accum_alpha_size);
        SDL_GL_SetAttribute(SDL_GL_STEREO, gl_stereo);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, gl_release_behavior);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, gl_multisamplebuffers);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, gl_multisamplesamples);
        if (gl_accelerated >= 0) {
            SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,
                                gl_accelerated);
        }
        SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, gl_retained_backing);
        if (gl_major_version) {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_version);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_version);
        }
        if (gl_debug) {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        }
        if (gl_profile_mask) {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_profile_mask);
        }

        displayID = SDL_GetPrimaryDisplay();

        {
            SDL_GetClosestFullscreenDisplayMode(displayID, window_w, window_h, state->refresh_rate, false, &state->fullscreen_mode);
        }

        state->windows =
            (SDL_Window **)SDL_calloc(1, sizeof(*state->windows));
        state->renderers =
            (SDL_Renderer **)SDL_calloc(1, sizeof(*state->renderers));
        state->targets =
            (SDL_Texture **)SDL_calloc(1, sizeof(*state->targets));
        if (!state->windows || !state->renderers) {
            SDL_Log("Out of memory!");
            return false;
        }
        {
            char title[1024];
            SDL_Rect r;
            SDL_PropertiesID props;

            if (state->fill_usable_bounds) {
                SDL_GetDisplayUsableBounds(displayID, &r);
            } else {
                r.x = window_x;
                r.y = window_y;
                r.w = window_w;
                r.h = window_h;
            }

            SDL_strlcpy(title, window_title, SDL_arraysize(title));
            props = SDL_CreateProperties();
            SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, title);
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, r.x);
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, r.y);
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, r.w);
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, r.h);
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER, window_flags);
            state->windows[0] = SDL_CreateWindowWithProperties(props);
            SDL_DestroyProperties(props);
            if (!state->windows[0]) {
                SDL_Log("Couldn't create window: %s",
                        SDL_GetError());
                return false;
            }
            SDL_GetWindowSize(state->windows[0], &w, &h);
            if (!(window_flags & SDL_WINDOW_RESIZABLE) && (w != r.w || h != r.h)) {
                SDL_Log("Window requested size %dx%d, got %dx%d", r.w, r.h, w, h);
                window_w = w;
                window_h = h;
            }
            if (window_flags & SDL_WINDOW_FULLSCREEN) {
                if (state->fullscreen_exclusive) {
                    SDL_SetWindowFullscreenMode(state->windows[0], &state->fullscreen_mode);
                }
                SDL_SetWindowFullscreen(state->windows[0], true);
            }

            if (!state->skip_renderer && (state->renderdriver || !(window_flags & (SDL_WINDOW_OPENGL | SDL_WINDOW_VULKAN | SDL_WINDOW_METAL)))) {
                state->renderers[0] = SDL_CreateRenderer(state->windows[0], state->renderdriver);
                if (!state->renderers[0]) {
                    SDL_Log("Couldn't create renderer: %s",
                            SDL_GetError());
                    return false;
                }
                if (state->logical_w == 0 || state->logical_h == 0) {
                    state->logical_w = window_w;
                    state->logical_h = window_h;
                }
                if (state->render_vsync) {
                    SDL_SetRenderVSync(state->renderers[0], state->render_vsync);
                }
                if (!SDL_SetRenderLogicalPresentation(state->renderers[0], state->logical_w, state->logical_h, SDL_LOGICAL_PRESENTATION_DISABLED)) {
                    SDL_Log("Couldn't set logical presentation: %s", SDL_GetError());
                    return false;
                }
                if (state->scale != 0.0f) {
                    SDL_SetRenderScale(state->renderers[0], state->scale, state->scale);
                }
            }

            SDL_ShowWindow(state->windows[0]);
        }

    return true;
}

SDL_AppResult CommonEventMainCallbacks(CommonState_my *state, const SDL_Event *event)
{
    switch (event->type) {
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

void CommonEvent(CommonState_my *state, SDL_Event *event, int *done)
{
    if (CommonEventMainCallbacks(state, event) != SDL_APP_CONTINUE) {
        *done = 1;
    }
}

void CommonQuit(CommonState_my *state)
{
    if (state) {
        if (state->targets) {
            {
                if (state->targets[0]) {
                    SDL_DestroyTexture(state->targets[0]);
                }
            }
            SDL_free(state->targets);
        }
        if (state->renderers) {
            {
                if (state->renderers[0]) {
                    SDL_DestroyRenderer(state->renderers[0]);
                }
            }
            SDL_free(state->renderers);
        }
        if (state->windows) {
            {
                SDL_DestroyWindow(state->windows[0]);
            }
            SDL_free(state->windows);
        }
    }
    SDL_Quit();
    SDL_free(state);
}


















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

static CommonState_my *g_state;
static SDL_GLContext *context = NULL;
static int g_depth = 16;
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
    SDL_free(datas);
    if (context) {
        {
            if (context[0]) {
                SDL_GL_DestroyContext(context[0]);
            }
        }

        SDL_free(context);
    }

    CommonQuit(g_state);
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
    
    //SDL_Delay(1000 / MY_USE_FPS);
}

static int g_done;
static Uint32 frames;

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

static void
loop(void)
{
    SDL_Event event;
    int active_windows = 0;

    /* Check for events */
    while (SDL_PollEvent(&event) && !g_done) {
        CommonEvent(g_state, &event, &g_done);
    }
    if (!g_done) {
        {
            if (g_state->windows[0] == NULL ||
                (suspend_when_occluded && (SDL_GetWindowFlags(g_state->windows[0]) & SDL_WINDOW_OCCLUDED))) {
		//skip
            } else {
            	++active_windows;
            	render_window(0);
            }
        }
    }

    /* If all windows are occluded, throttle event polling to 15hz. */
    if (!g_done && !active_windows) {
        SDL_DelayNS(SDL_NS_PER_SECOND / 15);
    } else if (!g_done) {
    	SDL_Delay(1000 / MY_USE_FPS); //FIXME: added
    }
}

int main(int argc, char *argv[])
{
    int value;
    const SDL_DisplayMode *mode;
    Uint64 then, now;
    shader_data *data;

    /* Initialize test framework */
    g_state = CommonCreateState();
    if (!g_state) {
        return 1;
    }
      
    if (!CommonInit(g_state, argv[0], g_depth)) {
        quit(2);
        return 0;
    }

    context = (SDL_GLContext *)SDL_calloc(1, sizeof(*context));
    if (!context) {
        SDL_Log("Out of memory!");
        quit(2);
    }

    /* Create OpenGL ES contexts */
    {
        context[0] = SDL_GL_CreateContext(g_state->windows[0]);
        if (!context[0]) {
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
        SDL_Log("SDL_GL_DEPTH_SIZE: requested %d, got %d", g_depth, value);
    } else {
        SDL_Log("Failed to get SDL_GL_DEPTH_SIZE: %s",
                SDL_GetError());
    }
    
    datas = (shader_data *)SDL_calloc(1, sizeof(shader_data));

    /* Set rendering settings for each context */
    {
        int w, h;
        if (!SDL_GL_MakeCurrent(g_state->windows[0], context[0])) {
            SDL_Log("SDL_GL_MakeCurrent(): %s", SDL_GetError());
	    //skip
        } else {
		SDL_GetWindowSizeInPixels(g_state->windows[0], &w, &h);
		ctx.glViewport(0, 0, w, h);

		data = &datas[0];
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

		SDL_GL_MakeCurrent(g_state->windows[0], NULL);
	}
    }

    /* Main render loop */
    frames = 0;
    then = SDL_GetTicks();
    g_done = 0;

    while (!g_done) {
        loop();
    }

    /* Print out some timing information */
    now = SDL_GetTicks();
    if (now > then) {
        SDL_Log("%2.2f frames per second",
                ((double)frames * 1000) / (now - then));
    }
    quit(0);
    return 0;
}



















