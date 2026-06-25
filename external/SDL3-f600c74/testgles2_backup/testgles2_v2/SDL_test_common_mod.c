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

