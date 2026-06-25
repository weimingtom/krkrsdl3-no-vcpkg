#include <SDL3/SDL_test.h>

#define SDL_MAIN_NOIMPL
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

// tests can quit after N ms
static Uint32 SDLCALL quit_after_ms_cb(void *userdata, SDL_TimerID timerID, Uint32 interval)
{
    SDL_Event event;
    event.type = SDL_EVENT_QUIT;
    event.common.timestamp = 0;
    SDL_PushEvent(&event);
    return 0;
}

static void SDL_snprintfcat(SDL_OUT_Z_CAP(maxlen) char *text, size_t maxlen, SDL_PRINTF_FORMAT_STRING const char *fmt, ...)
{
    size_t length = SDL_strlen(text);
    va_list ap;

    va_start(ap, fmt);
    text += length;
    maxlen -= length;
    (void)SDL_vsnprintf(text, maxlen, fmt, ap);
    va_end(ap);
}

static void SDLCALL SDLTest_CommonArgParserFinalize(void *data)
{
    SDLTest_CommonState *state = data;

    if (!(state->flags & SDL_INIT_VIDEO)) {
        state->video_argparser.usage = NULL;
    }
    if (!(state->flags & SDL_INIT_AUDIO)) {
        state->audio_argparser.usage = NULL;
    }
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

    //state->common_argparser.parse_arguments = SDLTest_CommonStateParseCommonArguments;
    state->common_argparser.finalize = SDLTest_CommonArgParserFinalize;
    //state->common_argparser.usage = common_usage;
    state->common_argparser.data = state;
    state->common_argparser.next = &state->video_argparser;

    //state->video_argparser.parse_arguments = SDLTest_CommonStateParseVideoArguments;
    state->video_argparser.finalize = NULL;
    //state->video_argparser.usage = video_usage;
    state->video_argparser.data = state;
    state->video_argparser.next = &state->audio_argparser;

    //state->audio_argparser.parse_arguments = SDLTest_CommonStateParseAudioArguments;
    state->audio_argparser.finalize = NULL;
    //state->audio_argparser.usage = audio_usage;
    state->audio_argparser.data = state;

    state->argparser = &state->common_argparser;

    return state;
}

static void SDLTest_PrintPixelFormat(char *text, size_t maxlen, Uint32 format)
{
    const char *name = SDL_GetPixelFormatName(format);
    if (name) {
        if (SDL_strncmp(name, "SDL_PIXELFORMAT_", 16) == 0) {
            name += 16;
        }
        SDL_snprintfcat(text, maxlen, name);
    } else {
        SDL_snprintfcat(text, maxlen, "0x%8.8x", format);
    }
}

static void SDLTest_PrintRenderer(SDL_Renderer *renderer)
{
    const char *name;
    int i;
    char text[1024];
    int max_texture_size;
    const SDL_PixelFormat *texture_formats;

    name = SDL_GetRendererName(renderer);

    SDL_Log("  Renderer %s:", name);
    if (SDL_strcmp(name, "gpu") == 0) {
        SDL_GPUDevice *device = SDL_GetPointerProperty(SDL_GetRendererProperties(renderer), SDL_PROP_RENDERER_GPU_DEVICE_POINTER, NULL);
        SDL_Log("    Driver: %s", SDL_GetGPUDeviceDriver(device));
    }
    SDL_Log("    VSync: %d", (int)SDL_GetNumberProperty(SDL_GetRendererProperties(renderer), SDL_PROP_RENDERER_VSYNC_NUMBER, 0));

    texture_formats = (const SDL_PixelFormat *)SDL_GetPointerProperty(SDL_GetRendererProperties(renderer), SDL_PROP_RENDERER_TEXTURE_FORMATS_POINTER, NULL);
    if (texture_formats) {
        (void)SDL_snprintf(text, sizeof(text), "    Texture formats: ");
        for (i = 0; texture_formats[i]; ++i) {
            if (i > 0) {
                SDL_snprintfcat(text, sizeof(text), ", ");
            }
            SDLTest_PrintPixelFormat(text, sizeof(text), texture_formats[i]);
        }
        SDL_Log("%s", text);
    }

    max_texture_size = (int)SDL_GetNumberProperty(SDL_GetRendererProperties(renderer), SDL_PROP_RENDERER_MAX_TEXTURE_SIZE_NUMBER, 0);
    if (max_texture_size) {
        SDL_Log("    Max Texture Size: %dx%d", max_texture_size, max_texture_size);
    }
}

static SDL_Surface *SDLTest_LoadIcon(const char *file)
{
    SDL_Surface *icon;

    /* Load the icon surface */
    icon = SDL_LoadSurface(file);
    if (!icon) {
        SDL_Log("Couldn't load %s: %s", file, SDL_GetError());
        return NULL;
    }

    if (icon->format == SDL_PIXELFORMAT_INDEX8) {
        /* Set the colorkey */
        SDL_SetSurfaceColorKey(icon, 1, *((Uint8 *)icon->pixels));
    }

    return icon;
}

static SDL_HitTestResult SDLCALL SDLTest_ExampleHitTestCallback(SDL_Window *win, const SDL_Point *area, void *data)
{
    int w, h;
    const int RESIZE_BORDER = 8;
    const int DRAGGABLE_TITLE = 32;

    /*SDL_Log("Hit test point %d,%d", area->x, area->y);*/

    SDL_GetWindowSize(win, &w, &h);

    if (area->x < RESIZE_BORDER) {
        if (area->y < RESIZE_BORDER) {
            SDL_Log("SDL_HITTEST_RESIZE_TOPLEFT");
            return SDL_HITTEST_RESIZE_TOPLEFT;
        } else if (area->y >= (h - RESIZE_BORDER)) {
            SDL_Log("SDL_HITTEST_RESIZE_BOTTOMLEFT");
            return SDL_HITTEST_RESIZE_BOTTOMLEFT;
        } else {
            SDL_Log("SDL_HITTEST_RESIZE_LEFT");
            return SDL_HITTEST_RESIZE_LEFT;
        }
    } else if (area->x >= (w - RESIZE_BORDER)) {
        if (area->y < RESIZE_BORDER) {
            SDL_Log("SDL_HITTEST_RESIZE_TOPRIGHT");
            return SDL_HITTEST_RESIZE_TOPRIGHT;
        } else if (area->y >= (h - RESIZE_BORDER)) {
            SDL_Log("SDL_HITTEST_RESIZE_BOTTOMRIGHT");
            return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
        } else {
            SDL_Log("SDL_HITTEST_RESIZE_RIGHT");
            return SDL_HITTEST_RESIZE_RIGHT;
        }
    } else if (area->y >= (h - RESIZE_BORDER)) {
        SDL_Log("SDL_HITTEST_RESIZE_BOTTOM");
        return SDL_HITTEST_RESIZE_BOTTOM;
    } else if (area->y < RESIZE_BORDER) {
        SDL_Log("SDL_HITTEST_RESIZE_TOP");
        return SDL_HITTEST_RESIZE_TOP;
    } else if (area->y < DRAGGABLE_TITLE) {
        SDL_Log("SDL_HITTEST_DRAGGABLE");
        return SDL_HITTEST_DRAGGABLE;
    }
    return SDL_HITTEST_NORMAL;
}

//keep
bool SDLTest_CommonInit(SDLTest_CommonState *state)
{
    int i, j, m, n, w, h;
    char text[1024];

    if (state->flags & SDL_INIT_VIDEO) {
        if (1) { //state->verbose & VERBOSE_VIDEO) {
            n = SDL_GetNumVideoDrivers();
            if (n == 0) {
                SDL_Log("No built-in video drivers");
            } else {
                (void)SDL_snprintf(text, sizeof(text), "Built-in video drivers:");
                for (i = 0; i < n; ++i) {
                    if (i > 0) {
                        SDL_snprintfcat(text, sizeof(text), ",");
                    }
                    SDL_snprintfcat(text, sizeof(text), " %s", SDL_GetVideoDriver(i));
                }
                SDL_Log("%s", text);
            }
        }
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
            SDL_Log("Couldn't initialize video driver: %s",
                    SDL_GetError());
            return false;
        }
        if (1) { //state->verbose & VERBOSE_VIDEO) {
            SDL_Log("Video driver: %s",
                    SDL_GetCurrentVideoDriver());
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

        if (1) { //state->verbose & VERBOSE_MODES) {
            SDL_DisplayID *displays;
            SDL_Rect bounds, usablebounds;
            SDL_DisplayMode **modes;
            const SDL_DisplayMode *mode;
            int bpp;
            Uint32 Rmask, Gmask, Bmask, Amask;
#ifdef SDL_VIDEO_DRIVER_WINDOWS
            int adapterIndex = 0;
            int outputIndex = 0;
#endif
            displays = SDL_GetDisplays(&n);
            SDL_Log("Number of displays: %d", n);
            for (i = 0; i < n; ++i) {
                SDL_DisplayID displayID = displays[i];
                SDL_Log("Display %" SDL_PRIu32 ": %s", displayID, SDL_GetDisplayName(displayID));

                SDL_zero(bounds);
                SDL_GetDisplayBounds(displayID, &bounds);

                SDL_zero(usablebounds);
                SDL_GetDisplayUsableBounds(displayID, &usablebounds);

                SDL_Log("Bounds: %dx%d at %d,%d", bounds.w, bounds.h, bounds.x, bounds.y);
                SDL_Log("Usable bounds: %dx%d at %d,%d", usablebounds.w, usablebounds.h, usablebounds.x, usablebounds.y);

                mode = SDL_GetDesktopDisplayMode(displayID);
                SDL_GetMasksForPixelFormat(mode->format, &bpp, &Rmask, &Gmask,
                                           &Bmask, &Amask);
                SDL_Log("  Desktop mode: %dx%d@%gx %gHz, %d bits-per-pixel (%s)",
                        mode->w, mode->h, mode->pixel_density, mode->refresh_rate, bpp,
                        SDL_GetPixelFormatName(mode->format));
                if (Rmask || Gmask || Bmask) {
                    SDL_Log("      Red Mask   = 0x%.8" SDL_PRIx32, Rmask);
                    SDL_Log("      Green Mask = 0x%.8" SDL_PRIx32, Gmask);
                    SDL_Log("      Blue Mask  = 0x%.8" SDL_PRIx32, Bmask);
                    if (Amask) {
                        SDL_Log("      Alpha Mask = 0x%.8" SDL_PRIx32, Amask);
                    }
                }

                /* Print available fullscreen video modes */
                modes = SDL_GetFullscreenDisplayModes(displayID, &m);
                if (m == 0) {
                    SDL_Log("No available fullscreen video modes");
                } else {
                    SDL_Log("  Fullscreen video modes:");
                    for (j = 0; j < m; ++j) {
                        mode = modes[j];
                        SDL_GetMasksForPixelFormat(mode->format, &bpp, &Rmask,
                                                   &Gmask, &Bmask, &Amask);
                        SDL_Log("    Mode %d: %dx%d@%gx %gHz, %d bits-per-pixel (%s)",
                                j, mode->w, mode->h, mode->pixel_density, mode->refresh_rate, bpp,
                                SDL_GetPixelFormatName(mode->format));
                        if (Rmask || Gmask || Bmask) {
                            SDL_Log("        Red Mask   = 0x%.8" SDL_PRIx32,
                                    Rmask);
                            SDL_Log("        Green Mask = 0x%.8" SDL_PRIx32,
                                    Gmask);
                            SDL_Log("        Blue Mask  = 0x%.8" SDL_PRIx32,
                                    Bmask);
                            if (Amask) {
                                SDL_Log("        Alpha Mask = 0x%.8" SDL_PRIx32, Amask);
                            }
                        }
                    }
                }
                SDL_free(modes);

#if defined(SDL_VIDEO_DRIVER_WINDOWS) && !defined(SDL_PLATFORM_XBOXONE) && !defined(SDL_PLATFORM_XBOXSERIES)
                /* Print the D3D9 adapter index */
                adapterIndex = SDL_GetDirect3D9AdapterIndex(displayID);
                SDL_Log("D3D9 Adapter Index: %d", adapterIndex);

                /* Print the DXGI adapter and output indices */
                SDL_GetDXGIOutputInfo(displayID, &adapterIndex, &outputIndex);
                SDL_Log("DXGI Adapter Index: %d  Output Index: %d", adapterIndex, outputIndex);
#endif
            }
            SDL_free(displays);
        }

        if (1) { //state->verbose & VERBOSE_RENDER) {
            n = SDL_GetNumRenderDrivers();
            if (n == 0) {
                SDL_Log("No built-in render drivers");
            } else {
                SDL_Log("Built-in render drivers:");
                for (i = 0; i < n; ++i) {
                    SDL_Log("  %s", SDL_GetRenderDriver(i));
                }
            }
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

            /* Add resize/drag areas for windows that are borderless and resizable */
            if ((state->window_flags & (SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS)) ==
                (SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS)) {
                SDL_SetWindowHitTest(state->windows[i], SDLTest_ExampleHitTestCallback, NULL);
            }

            if (state->window_icon) {
                SDL_Surface *icon = SDLTest_LoadIcon(state->window_icon);
                if (icon) {
                    SDL_SetWindowIcon(state->windows[i], icon);
                    SDL_DestroySurface(icon);
                }
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
                if (1) { //state->verbose & VERBOSE_RENDER) {
                    SDL_Log("Current renderer:");
                    SDLTest_PrintRenderer(state->renderers[i]);
                }
            }

            SDL_ShowWindow(state->windows[i]);
        }
        if (state->hide_cursor) {
            SDL_HideCursor();
        }
    }

    if (state->flags & SDL_INIT_AUDIO) {
        if (1) { //state->verbose & VERBOSE_AUDIO) {
            n = SDL_GetNumAudioDrivers();
            if (n == 0) {
                SDL_Log("No built-in audio drivers");
            } else {
                (void)SDL_snprintf(text, sizeof(text), "Built-in audio drivers:");
                for (i = 0; i < n; ++i) {
                    if (i > 0) {
                        SDL_snprintfcat(text, sizeof(text), ",");
                    }
                    SDL_snprintfcat(text, sizeof(text), " %s", SDL_GetAudioDriver(i));
                }
                SDL_Log("%s", text);
            }
        }
        if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
            SDL_Log("Couldn't initialize audio driver: %s",
                    SDL_GetError());
            return false;
        }
        if (1) { //state->verbose & VERBOSE_AUDIO) {
            SDL_Log("Audio driver: %s",
                    SDL_GetCurrentAudioDriver());
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


#define SCREENSHOT_FILE "screenshot.bmp"

typedef struct
{
    void *image;
    size_t size;
} SDLTest_ClipboardData;

static void SDLCALL SDLTest_ScreenShotClipboardCleanup(void *context)
{
    SDLTest_ClipboardData *data = (SDLTest_ClipboardData *)context;

    SDL_Log("Cleaning up screenshot image data");

    SDL_free(data->image);
    SDL_free(data);
}

static const void * SDLCALL SDLTest_ScreenShotClipboardProvider(void *context, const char *mime_type, size_t *size)
{
    SDLTest_ClipboardData *data = (SDLTest_ClipboardData *)context;

    if (SDL_strncmp(mime_type, "text", 4) == 0) {
        SDL_Log("Providing screenshot title to clipboard!");

        /* Return "Test screenshot" */
        *size = 15;
        return "Test screenshot (but this isn't part of it)";
    }

    SDL_Log("Providing screenshot image to clipboard!");

    if (!data->image) {
        SDL_IOStream *file;

        file = SDL_IOFromFile(SCREENSHOT_FILE, "r");
        if (file) {
            size_t length = (size_t)SDL_GetIOSize(file);
            void *image = SDL_malloc(length);
            if (image) {
                if (SDL_ReadIO(file, image, length) != length) {
                    SDL_Log("Couldn't read %s: %s", SCREENSHOT_FILE, SDL_GetError());
                    SDL_free(image);
                    image = NULL;
                }
            }
            SDL_CloseIO(file);

            if (image) {
                data->image = image;
                data->size = length;
            }
        } else {
            SDL_Log("Couldn't load %s: %s", SCREENSHOT_FILE, SDL_GetError());
        }
    }

    *size = data->size;
    return data->image;
}

static void SDLTest_CopyScreenShot(SDL_Renderer *renderer)
{
    SDL_Surface *surface;
    const char *image_formats[] = {
        "text/plain;charset=utf-8",
        "image/bmp"
    };
    SDLTest_ClipboardData *clipboard_data;

    if (!renderer) {
        return;
    }

    surface = SDL_RenderReadPixels(renderer, NULL);
    if (!surface) {
        SDL_Log("Couldn't read screen: %s", SDL_GetError());
        return;
    }

    if (!SDL_SaveBMP(surface, SCREENSHOT_FILE)) {
        SDL_Log("Couldn't save %s: %s", SCREENSHOT_FILE, SDL_GetError());
        SDL_DestroySurface(surface);
        return;
    }
    SDL_DestroySurface(surface);

    clipboard_data = (SDLTest_ClipboardData *)SDL_calloc(1, sizeof(*clipboard_data));
    if (!clipboard_data) {
        SDL_Log("Couldn't allocate clipboard data");
        return;
    }
    SDL_SetClipboardData(SDLTest_ScreenShotClipboardProvider, SDLTest_ScreenShotClipboardCleanup, clipboard_data, image_formats, SDL_arraysize(image_formats));
    SDL_Log("Saved screenshot to %s and clipboard", SCREENSHOT_FILE);
}

static void SDLTest_PasteScreenShot(void)
{
    const char *image_formats[] = {
        "image/bmp",
        "image/png",
        "image/tiff",
    };
    size_t i;

    for (i = 0; i < SDL_arraysize(image_formats); ++i) {
        size_t size;
        void *data = SDL_GetClipboardData(image_formats[i], &size);
        if (data) {
            char filename[16];
            SDL_IOStream *file;

            SDL_snprintf(filename, sizeof(filename), "clipboard.%s", image_formats[i] + 6);
            file = SDL_IOFromFile(filename, "w");
            if (file) {
                SDL_Log("Writing clipboard image to %s", filename);
                SDL_WriteIO(file, data, size);
                SDL_CloseIO(file);
            }
            SDL_free(data);
            return;
        }
    }
    SDL_Log("No supported screenshot data in the clipboard");
}

static void FullscreenTo(SDLTest_CommonState *state, int index, int windowId)
{
    int num_displays;
    SDL_DisplayID *displays;
    SDL_Window *window;
    SDL_WindowFlags flags;
    const SDL_DisplayMode *mode;
    struct SDL_Rect rect = { 0, 0, 0, 0 };

    displays = SDL_GetDisplays(&num_displays);
    if (displays && index < num_displays) {
        window = SDL_GetWindowFromID(windowId);
        if (window) {
            SDL_GetDisplayBounds(displays[index], &rect);

            flags = SDL_GetWindowFlags(window);
            if (flags & SDL_WINDOW_FULLSCREEN) {
                SDL_SetWindowFullscreen(window, false);
                SDL_Delay(15);
            }

            mode = SDL_GetWindowFullscreenMode(window);
            if (mode) {
                /* Try to set the existing mode on the new display */
                SDL_DisplayMode new_mode;

                SDL_memcpy(&new_mode, mode, sizeof(new_mode));
                new_mode.displayID = displays[index];
                if (!SDL_SetWindowFullscreenMode(window, &new_mode)) {
                    /* Try again with a default mode */
                    bool include_high_density_modes = false;
                    if (state->window_flags & SDL_WINDOW_HIGH_PIXEL_DENSITY) {
                        include_high_density_modes = true;
                    }
                    if (SDL_GetClosestFullscreenDisplayMode(displays[index], state->window_w, state->window_h, state->refresh_rate, include_high_density_modes, &new_mode)) {
                        SDL_SetWindowFullscreenMode(window, &new_mode);
                    }
                }
            }
            if (!mode) {
                SDL_SetWindowPosition(window, rect.x, rect.y);
            }
            SDL_SetWindowFullscreen(window, true);
        }
    }
    SDL_free(displays);
}

SDL_AppResult SDLTest_CommonEventMainCallbacks(SDLTest_CommonState *state, const SDL_Event *event)
{
    int i;

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
        bool withControl = !!(event->key.mod & SDL_KMOD_CTRL);
        bool withShift = !!(event->key.mod & SDL_KMOD_SHIFT);
        bool withAlt = !!(event->key.mod & SDL_KMOD_ALT);

        switch (event->key.key) {
            /* Add hotkeys here */
        case SDLK_PRINTSCREEN:
        {
            SDL_Window *window = SDL_GetWindowFromEvent(event);
            if (window) {
                for (i = 0; i < state->num_windows; ++i) {
                    if (window == state->windows[i]) {
                        SDLTest_CopyScreenShot(state->renderers[i]);
                    }
                }
            }
        } break;
        case SDLK_EQUALS:
            if (withControl) {
                /* Ctrl-+ double the size of the window */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    int w, h;
                    SDL_GetWindowSize(window, &w, &h);
                    SDL_SetWindowSize(window, w * 2, h * 2);
                }
            }
            break;
        case SDLK_MINUS:
            if (withControl) {
                /* Ctrl-- half the size of the window */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    int w, h;
                    SDL_GetWindowSize(window, &w, &h);
                    SDL_SetWindowSize(window, w / 2, h / 2);
                }
            }
            break;
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_LEFT:
        case SDLK_RIGHT:
            if (withAlt) {
                /* Alt-Up/Down/Left/Right switches between displays */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    int num_displays;
                    const SDL_DisplayID *displays = SDL_GetDisplays(&num_displays);
                    if (displays) {
                        SDL_DisplayID displayID = SDL_GetDisplayForWindow(window);
                        int current_index = -1;

                        for (i = 0; i < num_displays; ++i) {
                            if (displayID == displays[i]) {
                                current_index = i;
                                break;
                            }
                        }
                        if (current_index >= 0) {
                            SDL_DisplayID dest;
                            if (event->key.key == SDLK_UP || event->key.key == SDLK_LEFT) {
                                dest = displays[(current_index + num_displays - 1) % num_displays];
                            } else {
                                dest = displays[(current_index + num_displays + 1) % num_displays];
                            }
                            SDL_Log("Centering on display (%" SDL_PRIu32 ")", dest);
                            SDL_SetWindowPosition(window,
                                                  SDL_WINDOWPOS_CENTERED_DISPLAY(dest),
                                                  SDL_WINDOWPOS_CENTERED_DISPLAY(dest));
                        }
                    }
                }
            }
            if (withShift) {
                /* Shift-Up/Down/Left/Right shift the window by 100px */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    const int delta = 100;
                    int x, y;
                    SDL_GetWindowPosition(window, &x, &y);

                    if (event->key.key == SDLK_UP) {
                        y -= delta;
                    }
                    if (event->key.key == SDLK_DOWN) {
                        y += delta;
                    }
                    if (event->key.key == SDLK_LEFT) {
                        x -= delta;
                    }
                    if (event->key.key == SDLK_RIGHT) {
                        x += delta;
                    }

                    SDL_Log("Setting position to (%d, %d)", x, y);
                    SDL_SetWindowPosition(window, x, y);
                }
            }
            break;
        case SDLK_O:
            if (withControl) {
                /* Ctrl-O (or Ctrl-Shift-O) changes window opacity. */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    float opacity = SDL_GetWindowOpacity(window);
                    if (withShift) {
                        opacity += 0.20f;
                    } else {
                        opacity -= 0.20f;
                    }
                    SDL_SetWindowOpacity(window, opacity);
                }
            }
            break;
        case SDLK_H:
            if (withControl) {
                /* Ctrl-H changes cursor visibility. */
                if (SDL_CursorVisible()) {
                    SDL_HideCursor();
                } else {
                    SDL_ShowCursor();
                }
            }
            break;
        case SDLK_C:
            if (withAlt) {
                /* Alt-C copy awesome text to the primary selection! */
                SDL_SetPrimarySelectionText("SDL rocks!\nYou know it!");
                SDL_Log("Copied text to primary selection");

            } else if (withControl) {
                if (withShift) {
                    /* Ctrl-Shift-C copy screenshot! */
                    SDL_Window *window = SDL_GetWindowFromEvent(event);
                    if (window) {
                        for (i = 0; i < state->num_windows; ++i) {
                            if (window == state->windows[i]) {
                                SDLTest_CopyScreenShot(state->renderers[i]);
                            }
                        }
                    }
                } else {
                    /* Ctrl-C copy awesome text! */
                    SDL_SetClipboardText("SDL rocks!\nYou know it!");
                    SDL_Log("Copied text to clipboard");
                }
                break;
            }
            break;
        case SDLK_V:
            if (withAlt) {
                /* Alt-V paste awesome text from the primary selection! */
                char *text = SDL_GetPrimarySelectionText();
                if (*text) {
                    SDL_Log("Primary selection: %s", text);
                } else {
                    SDL_Log("Primary selection is empty");
                }
                SDL_free(text);

            } else if (withControl) {
                if (withShift) {
                    /* Ctrl-Shift-V paste screenshot! */
                    SDLTest_PasteScreenShot();
                } else {
                    /* Ctrl-V paste awesome text! */
                    char *text = SDL_GetClipboardText();
                    if (*text) {
                        SDL_Log("Clipboard: %s", text);
                    } else {
                        SDL_Log("Clipboard is empty");
                    }
                    SDL_free(text);
                }
            }
            break;
        case SDLK_F:
            if (withControl) {
                /* Ctrl-F flash the window */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    SDL_FlashWindow(window, SDL_FLASH_BRIEFLY);
                }
            }
            break;
        case SDLK_D:
            if (withControl) {
                /* Ctrl-D toggle fill-document */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    SDL_SetWindowFillDocument(window, !((SDL_GetWindowFlags(window) & SDL_WINDOW_FILL_DOCUMENT) != 0));
                }
            }
            break;
        case SDLK_P:
            if (withAlt) {
                /* Alt-P cycle through progress states */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    const char *name;
                    SDL_ProgressState progress_state = SDL_GetWindowProgressState(window);
                    progress_state += 1;
                    if (progress_state > SDL_PROGRESS_STATE_ERROR) {
                        progress_state = SDL_PROGRESS_STATE_NONE;
                    }
                    switch (progress_state) {
                    case SDL_PROGRESS_STATE_NONE:
                        name = "NONE";
                        break;
                    case SDL_PROGRESS_STATE_INDETERMINATE:
                        name = "INDETERMINATE";
                        break;
                    case SDL_PROGRESS_STATE_NORMAL:
                        name = "NORMAL";
                        break;
                    case SDL_PROGRESS_STATE_PAUSED:
                        name = "PAUSED";
                        break;
                    case SDL_PROGRESS_STATE_ERROR:
                        name = "ERROR";
                        break;
                    default:
                        name = "UNKNOWN";
                        break;
                    }
                    SDL_Log("Setting progress state to %s", name);
                    SDL_SetWindowProgressState(window, progress_state);
                }
            }
            else if (withControl)
            {
                /* Ctrl-P increase progress value */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    float progress_value = SDL_GetWindowProgressValue(window);
                    if (withShift) {
                        progress_value -= 0.1f;
                    } else {
                        progress_value += 0.1f;
                    }
                    SDL_Log("Setting progress value to %.1f", progress_value);
                    SDL_SetWindowProgressValue(window, progress_value);
                }
            }
            break;
        case SDLK_G:
            if (withControl) {
                /* Ctrl-G toggle mouse grab */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    SDL_SetWindowMouseGrab(window, !SDL_GetWindowMouseGrab(window));
                }
            }
            break;
        case SDLK_K:
            if (withControl) {
                /* Ctrl-K toggle keyboard grab */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    SDL_SetWindowKeyboardGrab(window, !SDL_GetWindowKeyboardGrab(window));
                }
            }
            break;
        case SDLK_M:
            if (withControl) {
                /* Ctrl-M maximize */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    SDL_WindowFlags flags = SDL_GetWindowFlags(window);
                    if (!(flags & SDL_WINDOW_RESIZABLE)) {
                        SDL_SetWindowResizable(window, true);
                    }
                    if (flags & SDL_WINDOW_MAXIMIZED) {
                        SDL_RestoreWindow(window);
                    } else {
                        SDL_MaximizeWindow(window);
                    }
                    if (!(flags & SDL_WINDOW_RESIZABLE)) {
                        SDL_SetWindowResizable(window, false);
                    }
                }
            }
            if (withShift) {
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    const bool shouldCapture = !(SDL_GetWindowFlags(window) & SDL_WINDOW_MOUSE_CAPTURE);
                    const bool rc = SDL_CaptureMouse(shouldCapture);
                    SDL_Log("%sapturing mouse %s!", shouldCapture ? "C" : "Unc", rc ? "succeeded" : "failed");
                }
            }
            break;
        case SDLK_R:
            if (withControl) {
                /* Ctrl-R toggle mouse relative mode */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    SDL_SetWindowRelativeMouseMode(window, !SDL_GetWindowRelativeMouseMode(window));
                }
            }
            break;
        case SDLK_T:
            if (withControl) {
                /* Ctrl-T toggle topmost mode */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    SDL_WindowFlags flags = SDL_GetWindowFlags(window);
                    if (flags & SDL_WINDOW_ALWAYS_ON_TOP) {
                        SDL_SetWindowAlwaysOnTop(window, false);
                    } else {
                        SDL_SetWindowAlwaysOnTop(window, true);
                    }
                }
            }
            break;
        case SDLK_Z:
            if (withControl) {
                /* Ctrl-Z minimize */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    SDL_MinimizeWindow(window);
                }
            }
            break;
        case SDLK_RETURN:
            if (withControl) {
                /* Ctrl-Enter toggle fullscreen */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    SDL_WindowFlags flags = SDL_GetWindowFlags(window);
                    if (!(flags & SDL_WINDOW_FULLSCREEN) ||
                        !SDL_GetWindowFullscreenMode(window)) {
                        SDL_SetWindowFullscreenMode(window, &state->fullscreen_mode);
                        SDL_SetWindowFullscreen(window, true);
                    } else {
                        SDL_SetWindowFullscreen(window, false);
                    }
                }
            } else if (withAlt) {
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
        case SDLK_B:
            if (withControl) {
                /* Ctrl-B toggle window border */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    const SDL_WindowFlags flags = SDL_GetWindowFlags(window);
                    const bool b = (flags & SDL_WINDOW_BORDERLESS) ? true : false;
                    SDL_SetWindowBordered(window, b);
                }
            }
            break;
        case SDLK_A:
            if (withControl) {
                /* Ctrl-A toggle aspect ratio */
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                if (window) {
                    float min_aspect = 0.0f, max_aspect = 0.0f;

                    SDL_GetWindowAspectRatio(window, &min_aspect, &max_aspect);
                    if (min_aspect > 0.0f || max_aspect > 0.0f) {
                        min_aspect = 0.0f;
                        max_aspect = 0.0f;
                    } else {
                        min_aspect = 1.0f;
                        max_aspect = 1.0f;
                    }
                    SDL_SetWindowAspectRatio(window, min_aspect, max_aspect);
                }
            }
            break;
        case SDLK_0:
            if (withControl) {
                SDL_Window *window = SDL_GetWindowFromEvent(event);
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Test Message", "You're awesome!", window);
            }
            break;
        case SDLK_1:
            if (withControl) {
                FullscreenTo(state, 0, event->key.windowID);
            }
            break;
        case SDLK_2:
            if (withControl) {
                FullscreenTo(state, 1, event->key.windowID);
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

