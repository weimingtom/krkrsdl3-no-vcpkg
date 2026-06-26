#sudo apt install gcc g++ gedit git lftp make
#sudo apt install libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxkbcommon-dev
#sudo apt install libegl-dev libegl1-mesa-dev libgles2-mesa-dev libgl1-mesa-dev libdrm-dev libgbm-dev 
#sudo apt install libasound2-dev 
#(for xubuntu 20.04 64bit virtualbox)
#sudo apt install libegl-mesa0 libegl1 libegl-dev libegl1-mesa-dev libgles2-mesa-dev libgl1-mesa-dev libdrm-dev libgbm-dev 
#
#sudo apt install libfreetype-dev libturbojpeg-dev libopusfile-dev libvorbis-dev libopencv-core-dev libopencv-imgproc-dev libglm-dev libonig-dev

CC  := gcc
CPP := g++
AR  := ar cru
RANLIB := ranlib

RM := rm -rf

CPPFLAGS2:= 

CPPFLAGS := 
CPPFLAGS += -I.   
#CPPFLAGS += -g -O2
#CPPFLAGS += -g3 -O0

#pc
CPPFLAGS += -g3 -O0

#libSDL3
CPPFLAGS += -DDLL_EXPORT 
CPPFLAGS += -DSDL_BUILD_MAJOR_VERSION=3 
CPPFLAGS += -DSDL_BUILD_MICRO_VERSION=0 
CPPFLAGS += -DSDL_BUILD_MINOR_VERSION=5 
CPPFLAGS += -DUSING_GENERATED_CONFIG_H 
#libSDL3
CPPFLAGS += -D_REENTRANT 
#libSDL3
CPPFLAGS += -Iexternal/SDL3-f600c74/include-config-relwithdebinfo/build_config 
CPPFLAGS += -Iexternal/SDL3-f600c74/include 
CPPFLAGS += -Iexternal/SDL3-f600c74/src 
CPPFLAGS += -idirafter./external/SDL3-f600c74/src/video/khronos 
CPPFLAGS += -isystem /usr/include/libdrm
#libSDL3
CPPFLAGS += -DNDEBUG 
#libSDL3
CPPFLAGS += -fPIC 
CPPFLAGS += -fvisibility=hidden 
CPPFLAGS += -fno-strict-aliasing 
CPPFLAGS += -fdiagnostics-color=always 
#libSDL3
CPPFLAGS += -Wall 
CPPFLAGS += -Wundef 
CPPFLAGS += -Wfloat-conversion 
CPPFLAGS += -Wshadow 
CPPFLAGS += -Wno-unused-local-typedefs 
CPPFLAGS += -Wimplicit-fallthrough 
CPPFLAGS += -Winvalid-pch 

#krkrsdl3
CPPFLAGS += -DMY_USE_MINLIB=1
CPPFLAGS += -DONIG_STATIC 
CPPFLAGS += -DPLUTOVG_BUILD_STATIC 
CPPFLAGS += -D_KRKRSDL3_CMD=1 
CPPFLAGS += -D_KRKRSDL3_GL=1 
CPPFLAGS += -D_KRKRSDL3_LINUX=1 
CPPFLAGS += -DCMAKE_INTDIR=\"Debug\" 

####FIXME:Added, need to add this macro, or the linux version will crash at ifdef _KRKRSDL3_LIB 
###CPPFLAGS += -D_KRKRSDL3_LIB=1

CPPFLAGS += -Icpp/core/archive 
CPPFLAGS += -Icpp/core/main 
CPPFLAGS += -Icpp/core/media/font 
CPPFLAGS += -Icpp/core/media/image 
CPPFLAGS += -Icpp/core/media/movie 
CPPFLAGS += -Icpp/core/media/sound 
CPPFLAGS += -Icpp/core/msg 
CPPFLAGS += -Icpp/core/render/public 
CPPFLAGS += -Icpp/core/render 
CPPFLAGS += -Icpp/core/script 
CPPFLAGS += -Icpp/core/tjs2 
CPPFLAGS += -Icpp/core/utils 
CPPFLAGS += -Icpp/core/utils/math 
CPPFLAGS += -Icpp/core/utils/simd 
CPPFLAGS += -Icpp/environ 
CPPFLAGS += -Icpp/plugins 

#CPPFLAGS += -isystem out/linux/vcpkg_installed/x64-linux-release/include 
#CPPFLAGS += -isystem out/linux/vcpkg_installed/x64-linux-release/include/webp 
#CPPFLAGS += -isystem out/linux/vcpkg_installed/x64-linux-release/include/opencv4 
#CPPFLAGS += -isystem out/linux/vcpkg_installed/x64-linux-release/include/opus 
#CPPFLAGS += -isystem out/linux/vcpkg_installed/x64-linux-release/include/plutovg 
#CPPFLAGS += -g 
CPPFLAGS += -I/usr/include/freetype2
CPPFLAGS += -I/usr/include/opus
CPPFLAGS += -I/usr/include/opencv4
#libswscale
#CPPFLAGS += -I/usr/include/x86_64-linux-gnu/
CPPFLAGS += -I./external/glad/include
CPPFLAGS += -I./external/SDL_ttf-release-3.2.2/include

CPPFLAGS2 += -std=gnu++17 
CPPFLAGS += -fPIE 
#-fexceptions

#kirikiroid2
#################


LDFLAGS := 

#LDFLAGS += -L/home/wmt/krkrsdl3/out/linux/vcpkg_installed/x64-linux-release/lib 
#LDFLAGS += -Wl,-rpath,/home/wmt/krkrsdl3/out/linux/vcpkg_installed/x64-linux-release/lib  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libavformat.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libavcodec.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libswresample.a  
#LDFLAGS += -lswscale #vcpkg_installed/x64-linux-release/lib/libswscale.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libavutil.a  
LDFLAGS += -pthread  
LDFLAGS += -lm  
LDFLAGS += -latomic  
LDFLAGS += -lpng #vcpkg_installed/x64-linux-release/lib/libpng16.a  
#LDFLAGS += -lglad #vcpkg_installed/x64-linux-release/lib/libglad.a  
#LDFLAGS += -lglm #vcpkg_installed/x64-linux-release/lib/libglm.a  
LDFLAGS += -ljpeg #vcpkg_installed/x64-linux-release/lib/libjpeg.a  
LDFLAGS += -lturbojpeg #vcpkg_installed/x64-linux-release/lib/libturbojpeg.a  
LDFLAGS += -lvorbis #vcpkg_installed/x64-linux-release/lib/libvorbis.a  
LDFLAGS += -lvorbisfile #vcpkg_installed/x64-linux-release/lib/libvorbisfile.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libvorbisenc.a  
#LDFLAGS += -lwebp #vcpkg_installed/x64-linux-release/lib/libwebp.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libwebpdecoder.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libwebpdemux.a  
LDFLAGS += -lonig #vcpkg_installed/x64-linux-release/lib/libonig.a  
LDFLAGS += -lopencv_imgproc #vcpkg_installed/x64-linux-release/lib/libopencv_imgproc4.a  
LDFLAGS += -lopencv_core #vcpkg_installed/x64-linux-release/lib/libopencv_core4.a  
LDFLAGS += -lopus #vcpkg_installed/x64-linux-release/lib/libopus.a  
LDFLAGS += -lopusfile #vcpkg_installed/x64-linux-release/lib/libopusfile.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libSDL3.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libSDL3_ttf.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libplutovg.a  
LDFLAGS += -lGL  
LDFLAGS += -lvorbis #vcpkg_installed/x64-linux-release/lib/libvorbis.a  
#LDFLAGS += -lwebp #vcpkg_installed/x64-linux-release/lib/libwebp.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libsharpyuv.a  
LDFLAGS += -ldl  
LDFLAGS += -lpthread  
LDFLAGS += -lrt  
LDFLAGS += -lopus #vcpkg_installed/x64-linux-release/lib/libopus.a  
LDFLAGS += -logg #vcpkg_installed/x64-linux-release/lib/libogg.a  
LDFLAGS += -lpng #vcpkg_installed/x64-linux-release/lib/libpng16.a  
LDFLAGS += -lfreetype #vcpkg_installed/x64-linux-release/lib/libfreetype.a  
LDFLAGS += -lz #vcpkg_installed/x64-linux-release/lib/libz.a  
LDFLAGS += -lbz2 #vcpkg_installed/x64-linux-release/lib/libbz2.a  
LDFLAGS += -lpng #vcpkg_installed/x64-linux-release/lib/libpng16.a  
LDFLAGS += -lz #vcpkg_installed/x64-linux-release/lib/libz.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libbrotlidec.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libbrotlicommon.a  
LDFLAGS += -lbz2 #vcpkg_installed/x64-linux-release/lib/libbz2.a  
#LDFLAGS += vcpkg_installed/x64-linux-release/lib/libSDL3.a  
LDFLAGS += -lm

OBJS := 

OBJS += external/SDL3-f600c74/src/SDL.o 
OBJS += external/SDL3-f600c74/src/SDL_assert.o 
OBJS += external/SDL3-f600c74/src/SDL_error.o 
OBJS += external/SDL3-f600c74/src/SDL_guid.o 
OBJS += external/SDL3-f600c74/src/SDL_hashtable.o 
OBJS += external/SDL3-f600c74/src/SDL_hints.o 
OBJS += external/SDL3-f600c74/src/SDL_list.o 
OBJS += external/SDL3-f600c74/src/SDL_log.o 
OBJS += external/SDL3-f600c74/src/SDL_properties.o 
OBJS += external/SDL3-f600c74/src/SDL_utils.o 

OBJS += external/SDL3-f600c74/src/atomic/SDL_atomic.o 
OBJS += external/SDL3-f600c74/src/atomic/SDL_spinlock.o 

OBJS += external/SDL3-f600c74/src/audio/SDL_audio.o 
OBJS += external/SDL3-f600c74/src/audio/SDL_audiocvt.o
OBJS += external/SDL3-f600c74/src/audio/SDL_audiodev.o 
OBJS += external/SDL3-f600c74/src/audio/SDL_audioqueue.o 
OBJS += external/SDL3-f600c74/src/audio/SDL_audioresample.o 
OBJS += external/SDL3-f600c74/src/audio/SDL_audiotypecvt.o 
OBJS += external/SDL3-f600c74/src/audio/SDL_mixer.o 
OBJS += external/SDL3-f600c74/src/audio/SDL_wave.o 

OBJS += external/SDL3-f600c74/src/camera/SDL_camera.o 

OBJS += external/SDL3-f600c74/src/core/SDL_core_unsupported.o 

OBJS += external/SDL3-f600c74/src/cpuinfo/SDL_cpuinfo.o 

OBJS += external/SDL3-f600c74/src/dynapi/SDL_dynapi.o 

OBJS += external/SDL3-f600c74/src/events/SDL_categories.o 
OBJS += external/SDL3-f600c74/src/events/SDL_clipboardevents.o 
OBJS += external/SDL3-f600c74/src/events/SDL_displayevents.o 
OBJS += external/SDL3-f600c74/src/events/SDL_dropevents.o 
OBJS += external/SDL3-f600c74/src/events/SDL_events.o 
OBJS += external/SDL3-f600c74/src/events/SDL_eventwatch.o 
OBJS += external/SDL3-f600c74/src/events/SDL_keyboard.o 
OBJS += external/SDL3-f600c74/src/events/SDL_keymap.o 
OBJS += external/SDL3-f600c74/src/events/SDL_keysym_to_keycode.o 
OBJS += external/SDL3-f600c74/src/events/SDL_keysym_to_scancode.o 
OBJS += external/SDL3-f600c74/src/events/SDL_mouse.o 
OBJS += external/SDL3-f600c74/src/events/SDL_pen.o 
OBJS += external/SDL3-f600c74/src/events/SDL_quit.o 
OBJS += external/SDL3-f600c74/src/events/SDL_scancode_tables.o 
OBJS += external/SDL3-f600c74/src/events/SDL_touch.o 
OBJS += external/SDL3-f600c74/src/events/SDL_windowevents.o 
OBJS += external/SDL3-f600c74/src/events/imKStoUCS.o 

OBJS += external/SDL3-f600c74/src/filesystem/SDL_filesystem.o 

OBJS += external/SDL3-f600c74/src/gpu/SDL_gpu.o 
OBJS += external/SDL3-f600c74/src/gpu/xr/SDL_gpu_openxr.o 
OBJS += external/SDL3-f600c74/src/gpu/xr/SDL_openxrdyn.o 

OBJS += external/SDL3-f600c74/src/haptic/SDL_haptic.o 

OBJS += external/SDL3-f600c74/src/hidapi/SDL_hidapi.o 

OBJS += external/SDL3-f600c74/src/io/SDL_asyncio.o 
OBJS += external/SDL3-f600c74/src/io/SDL_iostream.o 
OBJS += external/SDL3-f600c74/src/io/generic/SDL_asyncio_generic.o 

OBJS += external/SDL3-f600c74/src/joystick/SDL_gamepad.o 
OBJS += external/SDL3-f600c74/src/joystick/SDL_joystick.o 
OBJS += external/SDL3-f600c74/src/joystick/SDL_steam_virtual_gamepad.o 
OBJS += external/SDL3-f600c74/src/joystick/controller_type.o 

OBJS += external/SDL3-f600c74/src/locale/SDL_locale.o 

OBJS += external/SDL3-f600c74/src/main/SDL_main_callbacks.o 
OBJS += external/SDL3-f600c74/src/main/SDL_runapp.o 

OBJS += external/SDL3-f600c74/src/misc/SDL_libusb.o 
OBJS += external/SDL3-f600c74/src/misc/SDL_url.o 

OBJS += external/SDL3-f600c74/src/power/SDL_power.o 

OBJS += external/SDL3-f600c74/src/render/SDL_render.o 
OBJS += external/SDL3-f600c74/src/render/SDL_render_unsupported.o 
OBJS += external/SDL3-f600c74/src/render/SDL_yuv_sw.o 

OBJS += external/SDL3-f600c74/src/render/direct3d/SDL_render_d3d.o 
OBJS += external/SDL3-f600c74/src/render/direct3d/SDL_shaders_d3d.o 

OBJS += external/SDL3-f600c74/src/render/direct3d11/SDL_render_d3d11.o 
OBJS += external/SDL3-f600c74/src/render/direct3d11/SDL_shaders_d3d11.o 
OBJS += external/SDL3-f600c74/src/render/direct3d12/SDL_render_d3d12.o 
OBJS += external/SDL3-f600c74/src/render/direct3d12/SDL_shaders_d3d12.o 

OBJS += external/SDL3-f600c74/src/render/gpu/SDL_pipeline_gpu.o 
OBJS += external/SDL3-f600c74/src/render/gpu/SDL_render_gpu.o 
OBJS += external/SDL3-f600c74/src/render/gpu/SDL_shaders_gpu.o 

OBJS += external/SDL3-f600c74/src/render/ngage/SDL_render_ngage.o 

OBJS += external/SDL3-f600c74/src/render/opengl/SDL_render_gl.o 
OBJS += external/SDL3-f600c74/src/render/opengl/SDL_shaders_gl.o 

OBJS += external/SDL3-f600c74/src/render/opengles2/SDL_render_gles2.o 
OBJS += external/SDL3-f600c74/src/render/opengles2/SDL_shaders_gles2.o 

OBJS += external/SDL3-f600c74/src/render/ps2/SDL_render_ps2.o 
OBJS += external/SDL3-f600c74/src/render/psp/SDL_render_psp.o 

OBJS += external/SDL3-f600c74/src/render/software/SDL_blendfillrect.o 
OBJS += external/SDL3-f600c74/src/render/software/SDL_blendline.o 
OBJS += external/SDL3-f600c74/src/render/software/SDL_blendpoint.o 
OBJS += external/SDL3-f600c74/src/render/software/SDL_drawline.o 
OBJS += external/SDL3-f600c74/src/render/software/SDL_drawpoint.o 
OBJS += external/SDL3-f600c74/src/render/software/SDL_render_sw.o 
OBJS += external/SDL3-f600c74/src/render/software/SDL_triangle.o 

OBJS += external/SDL3-f600c74/src/render/vitagxm/SDL_render_vita_gxm.o 
OBJS += external/SDL3-f600c74/src/render/vitagxm/SDL_render_vita_gxm_memory.o 
OBJS += external/SDL3-f600c74/src/render/vitagxm/SDL_render_vita_gxm_tools.o 

OBJS += external/SDL3-f600c74/src/render/vulkan/SDL_render_vulkan.o 
OBJS += external/SDL3-f600c74/src/render/vulkan/SDL_shaders_vulkan.o 

OBJS += external/SDL3-f600c74/src/sensor/SDL_sensor.o 

OBJS += external/SDL3-f600c74/src/stdlib/SDL_crc16.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_crc32.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_getenv.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_iconv.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_malloc.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_memcpy.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_memmove.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_memset.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_mslibc.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_murmur3.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_qsort.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_random.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_stdlib.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_string.o 
OBJS += external/SDL3-f600c74/src/stdlib/SDL_strtokr.o 

OBJS += external/SDL3-f600c74/src/storage/SDL_storage.o 

OBJS += external/SDL3-f600c74/src/thread/SDL_thread.o 

OBJS += external/SDL3-f600c74/src/time/SDL_time.o 

OBJS += external/SDL3-f600c74/src/timer/SDL_timer.o 

OBJS += external/SDL3-f600c74/src/video/SDL_RLEaccel.o 
OBJS += external/SDL3-f600c74/src/video/SDL_blit.o 
OBJS += external/SDL3-f600c74/src/video/SDL_blit_0.o 
OBJS += external/SDL3-f600c74/src/video/SDL_blit_1.o 
OBJS += external/SDL3-f600c74/src/video/SDL_blit_A.o 
OBJS += external/SDL3-f600c74/src/video/SDL_blit_N.o 
OBJS += external/SDL3-f600c74/src/video/SDL_blit_auto.o 
OBJS += external/SDL3-f600c74/src/video/SDL_blit_copy.o 
OBJS += external/SDL3-f600c74/src/video/SDL_blit_slow.o 
OBJS += external/SDL3-f600c74/src/video/SDL_bmp.o 
OBJS += external/SDL3-f600c74/src/video/SDL_clipboard.o 
OBJS += external/SDL3-f600c74/src/video/SDL_egl.o 
OBJS += external/SDL3-f600c74/src/video/SDL_fillrect.o 
OBJS += external/SDL3-f600c74/src/video/SDL_pixels.o 
OBJS += external/SDL3-f600c74/src/video/SDL_rect.o 
OBJS += external/SDL3-f600c74/src/video/SDL_rotate.o 
OBJS += external/SDL3-f600c74/src/video/SDL_stb.o 
OBJS += external/SDL3-f600c74/src/video/SDL_stretch.o 
OBJS += external/SDL3-f600c74/src/video/SDL_surface.o 
OBJS += external/SDL3-f600c74/src/video/SDL_video.o 
OBJS += external/SDL3-f600c74/src/video/SDL_video_unsupported.o 
OBJS += external/SDL3-f600c74/src/video/SDL_vulkan_utils.o
OBJS += external/SDL3-f600c74/src/video/SDL_yuv.o 

OBJS += external/SDL3-f600c74/src/video/yuv2rgb/yuv_rgb_lsx.o 
OBJS += external/SDL3-f600c74/src/video/yuv2rgb/yuv_rgb_sse.o 
OBJS += external/SDL3-f600c74/src/video/yuv2rgb/yuv_rgb_std.o 

OBJS += external/SDL3-f600c74/src/audio/dummy/SDL_dummyaudio.o 

OBJS += external/SDL3-f600c74/src/audio/disk/SDL_diskaudio.o 

OBJS += external/SDL3-f600c74/src/camera/dummy/SDL_camera_dummy.o 

OBJS += external/SDL3-f600c74/src/loadso/dlopen/SDL_sysloadso.o 

OBJS += external/SDL3-f600c74/src/joystick/virtual/SDL_virtualjoystick.o 

OBJS += external/SDL3-f600c74/src/video/dummy/SDL_nullevents.o 
OBJS += external/SDL3-f600c74/src/video/dummy/SDL_nullframebuffer.o 
OBJS += external/SDL3-f600c74/src/video/dummy/SDL_nullvideo.o 

OBJS += external/SDL3-f600c74/src/audio/alsa/SDL_alsa_audio.o 

OBJS += external/SDL3-f600c74/src/audio/pulseaudio/SDL_pulseaudio.o 

OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11clipboard.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11dyn.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11events.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11framebuffer.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11keyboard.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11messagebox.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11modes.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11mouse.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11opengl.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11opengles.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11pen.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11settings.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11shape.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11toolkit.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11touch.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11video.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11vulkan.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11window.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11xfixes.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11xinput2.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11xsync.o 
OBJS += external/SDL3-f600c74/src/video/x11/SDL_x11xtest.o 
OBJS += external/SDL3-f600c74/src/video/x11/edid-parse.o 
OBJS += external/SDL3-f600c74/src/video/x11/xsettings-client.o 

##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylandclipboard.o 
##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylandcolor.o 
##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylanddatamanager.o 
##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylanddyn.o 
##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylandevents.o 
##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylandkeyboard.o 
##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylandmessagebox.o 
##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylandmouse.o 
##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylandopengles.o 
##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylandshmbuffer.o 
##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylandvideo.o 
##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylandvulkan.o 
##OBJS += external/SDL3-f600c74/src/video/wayland/SDL_waylandwindow.o 

##OBJS += external/SDL3-f600c74/wayland-generated-protocols/alpha-modifier-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/color-management-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/cursor-shape-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/fractional-scale-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/frog-color-management-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/idle-inhibit-unstable-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/input-timestamps-unstable-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/keyboard-shortcuts-inhibit-unstable-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/pointer-constraints-unstable-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/pointer-gestures-unstable-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/pointer-warp-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/primary-selection-unstable-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/relative-pointer-unstable-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/tablet-v2-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/text-input-unstable-v3-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/viewporter-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/wayland-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/xdg-activation-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/xdg-decoration-unstable-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/xdg-dialog-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/xdg-foreign-unstable-v2-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/xdg-output-unstable-v1-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/xdg-shell-protocol.o 
##OBJS += external/SDL3-f600c74/wayland-generated-protocols/xdg-toplevel-icon-v1-protocol.o 

OBJS += external/SDL3-f600c74/src/video/kmsdrm/SDL_kmsdrmdyn.o 
OBJS += external/SDL3-f600c74/src/video/kmsdrm/SDL_kmsdrmevents.o 
OBJS += external/SDL3-f600c74/src/video/kmsdrm/SDL_kmsdrmmouse.o 
OBJS += external/SDL3-f600c74/src/video/kmsdrm/SDL_kmsdrmopengles.o 
OBJS += external/SDL3-f600c74/src/video/kmsdrm/SDL_kmsdrmvideo.o 
OBJS += external/SDL3-f600c74/src/video/kmsdrm/SDL_kmsdrmvulkan.o 

####################

OBJS += external/SDL3-f600c74/src/tray/unix/SDL_tray.o 
OBJS += external/SDL3-f600c74/src/core/unix/SDL_appid.o 
OBJS += external/SDL3-f600c74/src/core/unix/SDL_fribidi.o 
OBJS += external/SDL3-f600c74/src/core/unix/SDL_gtk.o 
OBJS += external/SDL3-f600c74/src/core/unix/SDL_libthai.o 
OBJS += external/SDL3-f600c74/src/core/unix/SDL_poll.o 
OBJS += external/SDL3-f600c74/src/camera/v4l2/SDL_camera_v4l2.o 
OBJS += external/SDL3-f600c74/src/haptic/linux/SDL_syshaptic.o 

#IME
###OBJS += external/SDL3-f600c74/src/core/linux/SDL_dbus.o 
###OBJS += external/SDL3-f600c74/src/core/linux/SDL_system_theme.o 
###OBJS += external/SDL3-f600c74/src/core/linux/SDL_progressbar.o 
###OBJS += external/SDL3-f600c74/src/core/linux/SDL_ime.o 
###OBJS += external/SDL3-f600c74/src/core/linux/SDL_ibus.o 
###OBJS += external/SDL3-f600c74/src/core/linux/SDL_fcitx.o 
###OBJS += external/SDL3-f600c74/src/core/linux/SDL_udev.o 

OBJS += external/SDL3-f600c74/src/core/linux/SDL_evdev.o 
OBJS += external/SDL3-f600c74/src/core/linux/SDL_evdev_kbd.o 
OBJS += external/SDL3-f600c74/src/core/linux/SDL_evdev_capabilities.o 
OBJS += external/SDL3-f600c74/src/core/linux/SDL_threadprio.o 

OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_8bitdo.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_combined.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_flydigi.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_gamecube.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_gamesir.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_gip.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_lg4ff.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_luna.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_ps3.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_ps4.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_ps5.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_rumble.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_shield.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_sinput.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_stadia.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_steam.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_steam_hori.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_steam_triton.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_steamdeck.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_switch.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_switch2.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_wii.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_xbox360.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_xbox360w.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_xboxone.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapi_zuiki.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_hidapijoystick.o 
OBJS += external/SDL3-f600c74/src/joystick/hidapi/SDL_report_descriptor.o 

OBJS += external/SDL3-f600c74/src/haptic/hidapi/SDL_hidapihaptic.o 
OBJS += external/SDL3-f600c74/src/haptic/hidapi/SDL_hidapihaptic_lg4ff.o 

OBJS += external/SDL3-f600c74/src/joystick/linux/SDL_sysjoystick.o 

OBJS += external/SDL3-f600c74/src/thread/pthread/SDL_systhread.o 
OBJS += external/SDL3-f600c74/src/thread/pthread/SDL_sysmutex.o 
OBJS += external/SDL3-f600c74/src/thread/pthread/SDL_syscond.o 
OBJS += external/SDL3-f600c74/src/thread/pthread/SDL_sysrwlock.o 
OBJS += external/SDL3-f600c74/src/thread/pthread/SDL_systls.o 
OBJS += external/SDL3-f600c74/src/thread/pthread/SDL_syssem.o 

OBJS += external/SDL3-f600c74/src/misc/unix/SDL_sysurl.o 

OBJS += external/SDL3-f600c74/src/power/linux/SDL_syspower.o 

OBJS += external/SDL3-f600c74/src/locale/unix/SDL_syslocale.o 

OBJS += external/SDL3-f600c74/src/filesystem/unix/SDL_sysfilesystem.o 

OBJS += external/SDL3-f600c74/src/storage/generic/SDL_genericstorage.o 
OBJS += external/SDL3-f600c74/src/storage/steam/SDL_steamstorage.o 

OBJS += external/SDL3-f600c74/src/filesystem/posix/SDL_sysfsops.o 

OBJS += external/SDL3-f600c74/src/time/unix/SDL_systime.o 

OBJS += external/SDL3-f600c74/src/timer/unix/SDL_systimer.o 

OBJS += external/SDL3-f600c74/src/dialog/SDL_dialog.o 
OBJS += external/SDL3-f600c74/src/dialog/SDL_dialog_utils.o 
OBJS += external/SDL3-f600c74/src/dialog/unix/SDL_unixdialog.o 
OBJS += external/SDL3-f600c74/src/dialog/unix/SDL_portaldialog.o 
OBJS += external/SDL3-f600c74/src/dialog/unix/SDL_zenitydialog.o 
OBJS += external/SDL3-f600c74/src/dialog/unix/SDL_zenitymessagebox.o
 
OBJS += external/SDL3-f600c74/src/process/SDL_process.o 
OBJS += external/SDL3-f600c74/src/process/posix/SDL_posixprocess.o 

OBJS += external/SDL3-f600c74/src/video/offscreen/SDL_offscreenevents.o 
OBJS += external/SDL3-f600c74/src/video/offscreen/SDL_offscreenframebuffer.o 
OBJS += external/SDL3-f600c74/src/video/offscreen/SDL_offscreenopengles.o 
OBJS += external/SDL3-f600c74/src/video/offscreen/SDL_offscreenvideo.o 
OBJS += external/SDL3-f600c74/src/video/offscreen/SDL_offscreenvulkan.o 
OBJS += external/SDL3-f600c74/src/video/offscreen/SDL_offscreenwindow.o 

OBJS += external/SDL3-f600c74/src/tray/SDL_tray_utils.o 

OBJS += external/SDL3-f600c74/src/gpu/vulkan/SDL_gpu_vulkan.o 

OBJS += external/SDL3-f600c74/src/sensor/dummy/SDL_dummysensor.o 

OBJS += external/SDL3-f600c74/src/main/generic/SDL_sysmain_callbacks.o

#################
#kirikiroid2
KIRIKIROID2_OBJS :=

KIRIKIROID2_OBJS += cpp/core/archive/TARArchive.o 
KIRIKIROID2_OBJS += cpp/core/archive/TextStream.o 
KIRIKIROID2_OBJS += cpp/core/archive/TVPArchive.o 
KIRIKIROID2_OBJS += cpp/core/archive/TVPStorage.o 
KIRIKIROID2_OBJS += cpp/core/archive/UtilStreams.o 
KIRIKIROID2_OBJS += cpp/core/archive/XP3Archive.o 
KIRIKIROID2_OBJS += cpp/core/archive/ZIPArchive.o 
KIRIKIROID2_OBJS += cpp/core/archive/encoding/gbk2unicode.o 
KIRIKIROID2_OBJS += cpp/core/archive/encoding/jis2unicode.o 
KIRIKIROID2_OBJS += cpp/core/archive/minizip/unzip.o 
KIRIKIROID2_OBJS += cpp/core/archive/minizip/ioapi.o 

KIRIKIROID2_OBJS += cpp/core/main/NativeEventQueue.o 
KIRIKIROID2_OBJS += cpp/core/main/SystemControl.o 
KIRIKIROID2_OBJS += cpp/core/main/TVPApplication.o 
KIRIKIROID2_OBJS += cpp/core/main/TVPEvent.o 
KIRIKIROID2_OBJS += cpp/core/main/TVPSystem.o 
KIRIKIROID2_OBJS += cpp/core/main/TVPThread.o 
KIRIKIROID2_OBJS += cpp/core/main/WindowIntf.o 

KIRIKIROID2_OBJS += cpp/core/media/font/CharacterData.o 
KIRIKIROID2_OBJS += cpp/core/media/font/FreeTypeFontRasterizer.o 
KIRIKIROID2_OBJS += cpp/core/media/font/FreeTypeImp.o 
KIRIKIROID2_OBJS += cpp/core/media/font/PrerenderedFont.o 
KIRIKIROID2_OBJS += cpp/core/media/font/TVPFont.o 

KIRIKIROID2_OBJS += cpp/core/media/image/BitmapInfomation.o 
KIRIKIROID2_OBJS += cpp/core/media/image/GraphicsLoadThread.o 
KIRIKIROID2_OBJS += cpp/core/media/image/LoadJPEG.o 
KIRIKIROID2_OBJS += cpp/core/media/image/LoadPNG.o 
KIRIKIROID2_OBJS += cpp/core/media/image/LoadTLG.o 
KIRIKIROID2_OBJS += cpp/core/media/image/LoadWEBP.o 
KIRIKIROID2_OBJS += cpp/core/media/image/SaveTLG5.o 
KIRIKIROID2_OBJS += cpp/core/media/image/SaveTLG6.o 
KIRIKIROID2_OBJS += cpp/core/media/image/TVPGraphicsLoader.o 

#KIRIKIROID2_OBJS += cpp/core/media/movie/CodecAudioFFmpeg.o 
#KIRIKIROID2_OBJS += cpp/core/media/movie/CodecDemuxFFmpeg.o 
#KIRIKIROID2_OBJS += cpp/core/media/movie/CodecVideoFFmpeg.o 
KIRIKIROID2_OBJS += cpp/core/media/movie/InputStream.o 
#KIRIKIROID2_OBJS += cpp/core/media/movie/krffmpeg.o 
#KIRIKIROID2_OBJS += cpp/core/media/movie/KRMovieLayer.o 
#KIRIKIROID2_OBJS += cpp/core/media/movie/KRMovieOverlay.o 
#KIRIKIROID2_OBJS += cpp/core/media/movie/KRStreamInfo.o 
#KIRIKIROID2_OBJS += cpp/core/media/movie/Message.o 
#KIRIKIROID2_OBJS += cpp/core/media/movie/MessageQueue.o 
#KIRIKIROID2_OBJS += cpp/core/media/movie/VideoPlayer.o 
#KIRIKIROID2_OBJS += cpp/core/media/movie/VideoPlayerAudio.o 
#KIRIKIROID2_OBJS += cpp/core/media/movie/VideoPlayerVideo.o 
KIRIKIROID2_OBJS += cpp/core/media/movie/VideoReferenceClock.o 

#KIRIKIROID2_OBJS += cpp/core/media/sound/FFWaveDecoder.o 
KIRIKIROID2_OBJS += cpp/core/media/sound/OpusWaveDecoder.o 
KIRIKIROID2_OBJS += cpp/core/media/sound/VorbisWaveDecoder.o 
KIRIKIROID2_OBJS += cpp/core/media/sound/WaveIntf.o 
KIRIKIROID2_OBJS += cpp/core/media/sound/WaveLoopManager.o 
KIRIKIROID2_OBJS += cpp/core/media/sound/WaveSegmentQueue.o 

KIRIKIROID2_OBJS += cpp/core/msg/Log.o 
KIRIKIROID2_OBJS += cpp/core/msg/TVPDebug.o 
KIRIKIROID2_OBJS += cpp/core/msg/TVPMsg.o 

KIRIKIROID2_OBJS += cpp/core/render/gl/blend_function.o 
KIRIKIROID2_OBJS += cpp/core/render/gl/tvpgl.o 

KIRIKIROID2_OBJS += cpp/core/render/ComplexRect.o 
KIRIKIROID2_OBJS += cpp/core/render/DrawDevice.o 
KIRIKIROID2_OBJS += cpp/core/render/LayerBitmap.o 
KIRIKIROID2_OBJS += cpp/core/render/LayerManager.o 
KIRIKIROID2_OBJS += cpp/core/render/LayerTreeOwner.o 
KIRIKIROID2_OBJS += cpp/core/render/RenderManager.o 
KIRIKIROID2_OBJS += cpp/core/render/RenderManagerGL.o 
KIRIKIROID2_OBJS += cpp/core/render/TransIntf.o 
KIRIKIROID2_OBJS += cpp/core/render/TVPColor.o 

KIRIKIROID2_OBJS += cpp/core/script/tjsNativeAsyncTrigger.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeBasicDrawDevice.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeBitmap.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeBitmapLayerTreeOwner.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeCDDASoundBuffer.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeClipboard.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeDebug.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeFont.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeImageFunction.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeKAGParser.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeLayer.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeMenuItem.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeMIDISoundBuffer.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativePad.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativePhaseVocoder.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativePlugins.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeRect.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeScript.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeStorages.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeSystem.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeTimer.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeVideoOverlay.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeWaveSoundBuffer.o 
KIRIKIROID2_OBJS += cpp/core/script/tjsNativeWindow.o 
KIRIKIROID2_OBJS += cpp/core/script/TVPScript.o 

KIRIKIROID2_OBJS += cpp/core/tjs2/tjs.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjs.tab.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsArray.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsBinarySerializer.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsByteCodeLoader.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsCompileControl.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsConfig.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsConstArrayData.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsDate.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsdate.tab.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsDateParser.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsDebug.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsDictionary.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsDisassemble.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsError.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsException.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsGlobalStringMap.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsInterCodeExec.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsInterCodeGen.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsInterface.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsLex.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsMath.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsMessage.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsMT19937ar-cok.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsNamespace.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsNative.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsObject.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsObjectExtendable.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsOctPack.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjspp.tab.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsRandomGenerator.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsRegExp.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsScriptBlock.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsScriptCache.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsString.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsUtils.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsVariant.o 
KIRIKIROID2_OBJS += cpp/core/tjs2/tjsVariantString.o 

KIRIKIROID2_OBJS += cpp/core/utils/PhaseVocoderDSP.o 
KIRIKIROID2_OBJS += cpp/core/utils/Random.o 
KIRIKIROID2_OBJS += cpp/core/utils/RealFFT.o 
KIRIKIROID2_OBJS += cpp/core/utils/ResampleImage.o 
KIRIKIROID2_OBJS += cpp/core/utils/TickCount.o 
KIRIKIROID2_OBJS += cpp/core/utils/TVPTimer.o 
KIRIKIROID2_OBJS += cpp/core/utils/VelocityTracker.o 
KIRIKIROID2_OBJS += cpp/core/utils/WeightFunctor.o
 
KIRIKIROID2_OBJS += cpp/core/utils/math/md5.o 
KIRIKIROID2_OBJS += cpp/core/utils/math/xxhash.o 
KIRIKIROID2_OBJS += cpp/core/utils/math/xxtea.o 

KIRIKIROID2_OBJS += cpp/core/utils/simd/simd_image.o 

KIRIKIROID2_OBJS += cpp/environ/DetectCPU.o 
KIRIKIROID2_OBJS += cpp/environ/KeyCodeConv.o 
KIRIKIROID2_OBJS += cpp/environ/MainWindowLayer.o 
KIRIKIROID2_OBJS += cpp/environ/Other.o 
KIRIKIROID2_OBJS += cpp/environ/TVPConfig.o 
KIRIKIROID2_OBJS += cpp/environ/WaveMixer.o 
KIRIKIROID2_OBJS += cpp/environ/linux/Platform.o 

KIRIKIROID2_OBJS += cpp/plugins/ncbind/ncbind.o
 
KIRIKIROID2_OBJS += cpp/plugins/addFont.o 
KIRIKIROID2_OBJS += cpp/plugins/AlphaMovie.o 
KIRIKIROID2_OBJS += cpp/plugins/csvParser.o 
KIRIKIROID2_OBJS += cpp/plugins/dirlist.o 
KIRIKIROID2_OBJS += cpp/plugins/DrawDeviceD3D.o 
KIRIKIROID2_OBJS += cpp/plugins/expat.o 
KIRIKIROID2_OBJS += cpp/plugins/Extension.o 
KIRIKIROID2_OBJS += cpp/plugins/fftgraph.o 
KIRIKIROID2_OBJS += cpp/plugins/fstat.o 
KIRIKIROID2_OBJS += cpp/plugins/FuncStubs.o 
KIRIKIROID2_OBJS += cpp/plugins/getabout.o 
KIRIKIROID2_OBJS += cpp/plugins/getSample.o 
KIRIKIROID2_OBJS += cpp/plugins/gfxEffect.o 
KIRIKIROID2_OBJS += cpp/plugins/gfxFire.o 
KIRIKIROID2_OBJS += cpp/plugins/json.o 
KIRIKIROID2_OBJS += cpp/plugins/kirikiroid2.o 
KIRIKIROID2_OBJS += cpp/plugins/LayerExAreaAverage.o 
KIRIKIROID2_OBJS += cpp/plugins/LayerExBase.o 
KIRIKIROID2_OBJS += cpp/plugins/LayerExBTOA.o 
KIRIKIROID2_OBJS += cpp/plugins/LayerExImage.o 
#KIRIKIROID2_OBJS += cpp/plugins/LayerExMovie.o 
KIRIKIROID2_OBJS += cpp/plugins/LayerExPerspective.o 
KIRIKIROID2_OBJS += cpp/plugins/LayerExRaster.o 
KIRIKIROID2_OBJS += cpp/plugins/LayerExSave.o 
KIRIKIROID2_OBJS += cpp/plugins/motionPlayer.o 
KIRIKIROID2_OBJS += cpp/plugins/PackinOne.o 
KIRIKIROID2_OBJS += cpp/plugins/saveStruct.o 
KIRIKIROID2_OBJS += cpp/plugins/scriptsEx.o 
KIRIKIROID2_OBJS += cpp/plugins/shrinkCopy.o 
KIRIKIROID2_OBJS += cpp/plugins/textrender.o 
KIRIKIROID2_OBJS += cpp/plugins/TVPPlugin.o 
KIRIKIROID2_OBJS += cpp/plugins/varfile.o 
KIRIKIROID2_OBJS += cpp/plugins/win32dialog.o 
KIRIKIROID2_OBJS += cpp/plugins/windowEx.o 
KIRIKIROID2_OBJS += cpp/plugins/wutcwf.o 
KIRIKIROID2_OBJS += cpp/plugins/xp3filter.o 

KIRIKIROID2_OBJS += cpp/plugins/emoteplayer/emotefile.o 
KIRIKIROID2_OBJS += cpp/plugins/emoteplayer/emoteplayer.o 
KIRIKIROID2_OBJS += cpp/plugins/emoteplayer/emoteplayerclass.o 
KIRIKIROID2_OBJS += cpp/plugins/emoteplayer/emoterunner.o 

KIRIKIROID2_OBJS += cpp/plugins/extrans/extrans.o 
KIRIKIROID2_OBJS += cpp/plugins/extrans/mosaic.o 
KIRIKIROID2_OBJS += cpp/plugins/extrans/ripple.o 
KIRIKIROID2_OBJS += cpp/plugins/extrans/rotatebase.o 
KIRIKIROID2_OBJS += cpp/plugins/extrans/rotatetrans.o 
KIRIKIROID2_OBJS += cpp/plugins/extrans/turn.o 
KIRIKIROID2_OBJS += cpp/plugins/extrans/turntrans_table.o 
KIRIKIROID2_OBJS += cpp/plugins/extrans/wave.o 

KIRIKIROID2_OBJS += cpp/plugins/KAGParserEx/KAG.o 
KIRIKIROID2_OBJS += cpp/plugins/KAGParserEx/KAGParserEx.o 

KIRIKIROID2_OBJS += cpp/plugins/ExtKAGParser/ExtKAG.o 
KIRIKIROID2_OBJS += cpp/plugins/ExtKAGParser/ExtKAGParser.o 
KIRIKIROID2_OBJS += cpp/plugins/ExtKAGParser/TJSAry.o 
KIRIKIROID2_OBJS += cpp/plugins/ExtKAGParser/TJSDic.o 

#KIRIKIROID2_OBJS += cpp/plugins/LayerExDraw/LayerExDraw.o 

KIRIKIROID2_OBJS += cpp/plugins/psbfile/PSB.o 
KIRIKIROID2_OBJS += cpp/plugins/psbfile/PSBData.o 
KIRIKIROID2_OBJS += cpp/plugins/psbfile/PSBFile.o 
KIRIKIROID2_OBJS += cpp/plugins/psbfile/PSBMedia.o 

KIRIKIROID2_OBJS += cpp/plugins/sqlite3/extend.o 
KIRIKIROID2_OBJS += cpp/plugins/sqlite3/sqlite3_.o  # .cpp is same name
KIRIKIROID2_OBJS += cpp/plugins/sqlite3/sqlite3.o 
KIRIKIROID2_OBJS += cpp/plugins/sqlite3/xp3_vfs.o 

#main entry move to below, don't use here
###KIRIKIROID2_OBJS += cpp/krkrsdl.o 

KIRIKIROID2_OBJS += cpp/krkrsdl_gl.o 
KIRIKIROID2_OBJS += cpp/krkrsdl_menu.o

#krkrsdl3
#################

#glad
KIRIKIROID2_OBJS += external/glad/src/glad.o
#SDL3_ttf
KIRIKIROID2_OBJS += external/SDL_ttf-release-3.2.2/src/SDL_gpu_textengine.o
KIRIKIROID2_OBJS += external/SDL_ttf-release-3.2.2/src/SDL_hashtable_ttf.o
KIRIKIROID2_OBJS += external/SDL_ttf-release-3.2.2/src/SDL_surface_textengine.o
KIRIKIROID2_OBJS += external/SDL_ttf-release-3.2.2/src/SDL_hashtable.o
KIRIKIROID2_OBJS += external/SDL_ttf-release-3.2.2/src/SDL_renderer_textengine.o
KIRIKIROID2_OBJS += external/SDL_ttf-release-3.2.2/src/SDL_ttf.o


#################



all : krkrsdl3

libSDL3.a : $(OBJS)
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

krkrsdl3: libSDL3.a $(KIRIKIROID2_OBJS)
	$(CPP) ./cpp/krkrsdl.cpp $(KIRIKIROID2_OBJS) libSDL3.a -o $@ -I./Classes $(CPPFLAGS) $(LDFLAGS)

%.o : %.cpp
	$(CPP) $(CPPFLAGS2) $(CPPFLAGS) -o $@ -c $<

%.o : %.cc
	$(CPP) $(CPPFLAGS2) $(CPPFLAGS) -o $@ -c $<

%.o : %.c
	$(CC) $(CPPFLAGS) -o $@ -c $<

#file:///home/wmt/_testata/data.xp3
test:
	./krkrsdl3 `realpath ./_testdata/data.xp3`
#need `realpath ./krkrsdl3`
#`realpath ./krkrsdl3` `realpath ./_testdata/data.xp3`

#need full path, relative path bad#file://./_testdata/data.xp3

#(gdb) catch throw
##2  0x000055555588c9a8 in std::filesystem::__cxx11::directory_iterator::directory_iterator (this=0x7fffffffda40, __p=filesystem::path "file://./_test//fonts" = {...})
#    at /usr/include/c++/9/bits/fs_dir.h:369
##3  0x000055555588bc89 in TVPGetLocalFileListAt(TJS::tTJSString const&, std::function<void (TJS::tTJSString const&, tTVPLocalFileInfo*)> const&) (name=..., cb=...)
#    at cpp/environ/Other.cpp:1123
##4  0x0000555555624329 in TVPInitFontNames () at cpp/core/media/font/TVPFont.cpp:198
##5  0x00005555555f7ac0 in tTVPApplication::StartApplication (this=0x555556168fc0, argc=2, argv=0x7fffffffdfc8) at cpp/core/main/TVPApplication.cpp:199
debug:
	gdb --args ./krkrsdl3 `realpath ./_testdata/data.xp3`

#need full path, relative path bad#file://./_testdata/data.xp3

#b TVPStorage.cpp:1092
#b TVPCreateStream

##0  TVPCreateStream (_name=..., flags=0) at cpp/core/archive/TVPStorage.cpp:1092
##1  0x00005555555db1b0 in TVPCreateBinaryStreamForRead (name=..., modestr=...) at cpp/core/archive/TVPStorage.cpp:1354
##2  0x000055555588bda7 in GetResourceStream (filename=...) at cpp/environ/linux/Platform.cpp:452
##3  0x000055555562124a in TVPInitFontNames () at cpp/core/media/font/TVPFont.cpp:172
##4  0x00005555555f4ac4 in tTVPApplication::StartApplication (this=0x555556151fc0, argc=2, argv=0x7fffffffdfc8) at cpp/core/main/TVPApplication.cpp:200
##5  0x00005555555cd5fd in SDL_AppInit (appstate=0x555556135a38 <SDL_main_appstate>, argc=2, argv=0x7fffffffdfc8) at ./cpp/krkrsdl.cpp:88
##6  0x0000555555c6320f in SDL_InitMainCallbacks (argc=2, argv=0x7fffffffdfc8, appinit=0x5555555cd418 <SDL_AppInit(void**, int, char**)>, 
#    appiter=0x5555555cd9d3 <SDL_AppIterate(void*)>, appevent=0x5555555cd63c <SDL_AppEvent(void*, SDL_Event*)>, appquit=0x5555555cdb47 <SDL_AppQuit(void*, SDL_AppResult)>)
#    at external/SDL3-f600c74/src/main/SDL_main_callbacks.c:104
##7  0x0000555555c3751d in SDL_EnterAppMainCallbacks_REAL (argc=2, argv=0x7fffffffdfc8, appinit=0x5555555cd418 <SDL_AppInit(void**, int, char**)>, 
#    appiter=0x5555555cd9d3 <SDL_AppIterate(void*)>, appevent=0x5555555cd63c <SDL_AppEvent(void*, SDL_Event*)>, appquit=0x5555555cdb47 <SDL_AppQuit(void*, SDL_AppResult)>)
#    at external/SDL3-f600c74/src/main/generic/SDL_sysmain_callbacks.c:56
##8  0x0000555555a72351 in SDL_EnterAppMainCallbacks (a=2, b=0x7fffffffdfc8, c=0x5555555cd418 <SDL_AppInit(void**, int, char**)>, d=0x5555555cd9d3 <SDL_AppIterate(void*)>, 
#    e=0x5555555cd63c <SDL_AppEvent(void*, SDL_Event*)>, f=0x5555555cdb47 <SDL_AppQuit(void*, SDL_AppResult)>) at external/SDL3-f600c74/src/dynapi/SDL_dynapi_procs.h:207
##9  0x00005555555cd3e6 in SDL_main (argc=2, argv=0x7fffffffdfc8) at external/SDL3-f600c74/include/SDL3/SDL_main_impl.h:59
##10 0x0000555555c633d9 in SDL_CallMainFunction (argc=2, argv=0x7fffffffdfc8, mainFunction=0x5555555cd3a9 <SDL_main(int, char**)>)
#    at external/SDL3-f600c74/src/main/SDL_main_callbacks.c:164
##11 0x0000555555aba73b in SDL_RunApp_REAL (argc=2, argv=0x7fffffffdfc8, mainFunction=0x5555555cd3a9 <SDL_main(int, char**)>, reserved=0x0)
#    at external/SDL3-f600c74/src/main/SDL_runapp.c:37
##12 0x0000555555a694ab in SDL_RunApp_DEFAULT (a=2, b=0x7fffffffdfc8, c=0x5555555cd3a9 <SDL_main(int, char**)>, d=0x0) at external/SDL3-f600c74/src/dynapi/SDL_dynapi_procs.h:804
##13 0x0000555555a77daf in SDL_RunApp (a=2, b=0x7fffffffdfc8, c=0x5555555cd3a9 <SDL_main(int, char**)>, d=0x0) at external/SDL3-f600c74/src/dynapi/SDL_dynapi_procs.h:804
##14 0x00005555555cd415 in main (argc=2, argv=0x7fffffffdfc8) at external/SDL3-f600c74/include/SDL3/SDL_main_impl.h:137

#tTVPMemoryStream* GetResourceStream(const ttstr& filename)
#{
#./cpp/core/main/TVPApplication.cpp:ExePath()--->    tTJSBinaryStream* tmp = TVPCreateBinaryStreamForRead(ExePath() + ttstr("/") + filename, 0);

#./cpp/core/main/TVPApplication.cpp:ExePath()
#TVPNativeProjectDir
#./cpp/core/main/TVPApplication.cpp:StartApplication()
##ifdef _KRKRSDL3_LIB
#    TVPNativeProjectDir = std::string(argv[1]);
##else
#    size_t lastSlash = std::string(argv[0]).find_last_of("/\\");
#    if (lastSlash != std::string::npos)
#    {
#        TVPNativeProjectDir = std::string(argv[0]).substr(0, lastSlash + 1) + "Res";
#    }
##endif



clean:
	$(RM) $(OBJS) $(KIRIKIROID2_OBJS) libSDL3.a krkrsdl3

