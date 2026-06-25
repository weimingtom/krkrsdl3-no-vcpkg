https://github.com/Snapchat/SnapRHI/blob/b1c9b8f4738ffaddb586bc163eafbccc3e32f460/examples/triangle-demo-app/src/OpenGL/GLWindow.cpp#L12


#if defined(ANDROID)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
#if TARGET_OS_IOS
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif
#else
    // Windows/Linux: request a reasonably modern core profile.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
#endif

---------------

https://github.com/OpenMinecraft-Dev/sdl-ohos-shell/blob/d48a2eedefe303910a84f74847594fe03f691dad/entry/src/main/cpp/napi_init.cpp#L10

    SDL_SetHint(SDL_HINT_EGL_LIBRARY, "libEGL.so");
    SDL_SetHint(SDL_HINT_OPENGL_LIBRARY, "libGLESv2.so");
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "libGLESv2.so");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);


https://github.com/jack-faller/SDL3-template/tree/2854868e6d12b7741d4e515cab146cd8d1aace61

https://github.com/jack-faller/SDL3-template/blob/2854868e6d12b7741d4e515cab146cd8d1aace61/code/main.cpp#L232



https://stackoverflow.com/questions/79448732/sdl3-fails-to-create-opengl-es-2-0-context


I am working on an OpenGL project using the SDL3 and OpenGL ES 2.0. For the moment, I have a basic code which creates a window with the drawing of a triangle. The issue here is that the glCreateShader(GL_VERTEX_SHADER) function always returns 0.

The reason of this is that the context is not created, or not being used. For example, if I run the following code:

#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengles2.h>

int main(int argc, char* argv[]) {
    // Initialize SDL2

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create an SDL window with OpenGL ES context
    SDL_Window* window = SDL_CreateWindow("OpenGL ES 2.0 Triangle", 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);

    if (!context) {
        printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    int isMadeCurrent = SDL_GL_MakeCurrent(window, context);

    printf("Current context? %d\n", isMadeCurrent);
    printf("Context error: %s\n", SDL_GetError());
    printf("Context version: %s\n", glGetString(GL_VERSION));

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Exiting...\n");

    return 0;
}
I get the following output:

Current context? 1
Context error:
Context version: (null)
Exiting...
glGetString(GL_VERSION) printing (null) means that it couldn't check the version of OpenGL since there is no current context, despite there are no errors shown. I know SDL_GL_MakeCurrent() is not a necessary line, but I have included to make sure the context is being used, which does not seem to be the case.

My directory structure is the following:

- src
  - include
    - GLES2
    - SDL3
  - lib
    - libGLESv2.lib
    - libSDL3.dll.a
- libGLESv2.dll
- main.c
- main.exe
- Makefile
- mozglue.dll
- SDL3.dll
Here is the content of my Makefile (I compile everything in Windows with mingw32-make:

all:
    gcc -Isrc/include -L src/lib -o main main.c -lGLESv2 -lSDL3
I have tried in a separate project using SDL2 instead of SDL3, or compiling and then running my code in another system. None of this fixed this issue. Also, I made sure the libraries included are compatible with the architecture of my computer.

copengl-essdlmingw-w64sdl-3
Share
Improve this question
Follow
edited Feb 18, 2025 at 15:44
asked Feb 18, 2025 at 15:09
Josué Pedrajas Pérez's user avatar
Josué Pedrajas Pérez
3311 silver badge66 bronze badges
Which OpenGL ES 2.0 implementation are you using? – 
genpfault
 CommentedFeb 18, 2025 at 15:23 
What is the SDL_HINT_OPENGL_ES_DRIVER hint / SDL_OPENGL_ES_DRIVER environment variable set to? – 
genpfault
 CommentedFeb 18, 2025 at 18:29
That looks like SDL2 code, not SDL3? – 
trojanfoe
 CommentedFeb 21, 2025 at 10:51
Add a comment
2 Answers
Sorted by:

Highest score (default)
3

SDL_GL_CreateContext - Remarks

Windows users new to OpenGL should note that, for historical reasons, GL functions added after OpenGL version 1.1 are not available by default. Those functions must be loaded at run-time, either with an OpenGL extension-handling library or with SDL_GL_GetProcAddress() and its related functions.

If that is not the issue, try to run the example on the same page (example), which uses legacy opengl, which should run without loading the library functions.

See also: OpenGL Wiki - Load OpenGL Functions

Share
Improve this answer
Follow
edited Feb 18, 2025 at 16:21
answered Feb 18, 2025 at 16:15
Erdal Küçük's user avatar
Erdal Küçük
6,43322 gold badges99 silver badges1515 bronze badges
Sign up to request clarification or add additional context in comments.

Comments

1

Works On My Machine™ with both Mesa's EGL+GLES implementation (llvmpipe software rasterizer) via pal1000's Windows binaries (SDL_OPENGL_ES_DRIVER=1 + GALLIUM_DRIVER=llvmpipe).

>set SDL_OPENGL_ES_DRIVER=1
>set GALLIUM_DRIVER=llvmpipe
>sdl-gles.exe
Current context? 1
GL_VERSION : OpenGL ES 3.2 Mesa 24.3.4 (git-1950a8b78c)
GL_VENDOR  : Mesa
GL_RENDERER: llvmpipe (LLVM 19.1.7, 256 bits)
...and AMD's hardware GLES implementation via the GLES-on-WGL code-path (SDL_OPENGL_ES_DRIVER=0):

>set SDL_OPENGL_ES_DRIVER=0
>sdl-gles.exe
Current context? 1
GL_VERSION : OpenGL ES 2.0.0 23.10.18.05.230719
GL_VENDOR  : ATI Technologies Inc.
GL_RENDERER: AMD Radeon Pro WX 3200 Series
Plus GLAD for runtime OpenGL loading.

All together:

#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/gles2.h>

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create an SDL window with OpenGL ES context
    SDL_Window* window = SDL_CreateWindow("OpenGL ES 2.0 Triangle", 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);

    if (!context) {
        printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    int isMadeCurrent = SDL_GL_MakeCurrent(window, context);

    gladLoadGLES2( SDL_GL_GetProcAddress );

    printf("Current context? %d\n", isMadeCurrent);
    printf("GL_VERSION : %s\n", glGetString(GL_VERSION));
    printf("GL_VENDOR  : %s\n", glGetString(GL_VENDOR));
    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
Tied together using CMake:

cmake_minimum_required( VERSION 3.7.0 )
project( sdl-gles )

set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED OFF CACHE BOOL "" FORCE )
set( SDL_TESTS OFF CACHE BOOL "" FORCE )
set( SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE )
add_subdirectory( external/sdl EXCLUDE_FROM_ALL )

include_directories( "include" )
add_executable( sdl-gles "src/main.c" "src/gles2.c" )
target_link_libraries( sdl-gles PRIVATE SDL3::SDL3-static)


