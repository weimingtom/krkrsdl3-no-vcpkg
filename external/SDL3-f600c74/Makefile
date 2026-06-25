## gcc -I./include -o hello hello.c libSDL3.so.0 -lpthread -ldl -lm 
## LD_LIBRARY_PATH=. ./hello 

## If no title bar under Ubuntu 25.04, switch to Ubuntu on Xorg mode when login (click and enter passwd, then choose at right bottom button of the login screen)

#PLAT := /home/wmt/riscv64-lp64d--uclibc--stable-2021.11-1/bin/riscv64-linux-

CC := $(PLAT)gcc 
CPP := $(PLAT)g++
LD := $(PLAT)ld
AR := $(PLAT)ar rcu 
RANLIB := $(PLAT)ranlib
STRIP := $(PLAT)strip
RM := rm -f

CFLAGS :=

CFLAGS += -O0 -g3 

CFLAGS += -DDLL_EXPORT 
CFLAGS += -DSDL_BUILD_MAJOR_VERSION=3 
CFLAGS += -DSDL_BUILD_MICRO_VERSION=0 
CFLAGS += -DSDL_BUILD_MINOR_VERSION=5 
CFLAGS += -DUSING_GENERATED_CONFIG_H 

CFLAGS += -D_REENTRANT 

###FIXME:
CFLAGS += -Iinclude-config-relwithdebinfo/build_config 
###CFLAGS += -I/home/wmt/SDL3/build/include-revision 
CFLAGS += -Iinclude 
CFLAGS += -Isrc 
###FIXME:
###CFLAGS += -isystem ./wayland-generated-protocols 
###CFLAGS += -isystem /usr/include/dbus-1.0 
###CFLAGS += -isystem /usr/lib/x86_64-linux-gnu/dbus-1.0/include 
#CFLAGS += -isystem /usr/include/blkid 
#CFLAGS += -isystem /usr/include/libmount 
###ibus depends glib
#CFLAGS += -isystem /usr/include/ibus-1.0 
#CFLAGS += -isystem /usr/include/glib-2.0 
#CFLAGS += -isystem /usr/lib/x86_64-linux-gnu/glib-2.0/include  
###CFLAGS += -include /home/wmt/SDL3/build/CMakeFiles/SDL3-shared.dir/cmake_pch.h 
CFLAGS += -idirafter./src/video/khronos 
CFLAGS += -isystem /usr/include/libdrm

###CFLAGS += -O2 -g 
CFLAGS += -DNDEBUG 

CFLAGS += -fPIC 
CFLAGS += -fvisibility=hidden 
CFLAGS += -fno-strict-aliasing 
CFLAGS += -fdiagnostics-color=always 
  
CFLAGS += -Wall 
CFLAGS += -Wundef 
CFLAGS += -Wfloat-conversion 
CFLAGS += -Wshadow 
CFLAGS += -Wno-unused-local-typedefs 
CFLAGS += -Wimplicit-fallthrough 
CFLAGS += -Winvalid-pch 

####################

OBJECTS := 

OBJECTS += src/SDL.o 
OBJECTS += src/SDL_assert.o 
OBJECTS += src/SDL_error.o 
OBJECTS += src/SDL_guid.o 
OBJECTS += src/SDL_hashtable.o 
OBJECTS += src/SDL_hints.o 
OBJECTS += src/SDL_list.o 
OBJECTS += src/SDL_log.o 
OBJECTS += src/SDL_properties.o 
OBJECTS += src/SDL_utils.o 

OBJECTS += src/atomic/SDL_atomic.o 
OBJECTS += src/atomic/SDL_spinlock.o 

OBJECTS += src/audio/SDL_audio.o 
OBJECTS += src/audio/SDL_audiocvt.o
OBJECTS += src/audio/SDL_audiodev.o 
OBJECTS += src/audio/SDL_audioqueue.o 
OBJECTS += src/audio/SDL_audioresample.o 
OBJECTS += src/audio/SDL_audiotypecvt.o 
OBJECTS += src/audio/SDL_mixer.o 
OBJECTS += src/audio/SDL_wave.o 

OBJECTS += src/camera/SDL_camera.o 

OBJECTS += src/core/SDL_core_unsupported.o 

OBJECTS += src/cpuinfo/SDL_cpuinfo.o 

OBJECTS += src/dynapi/SDL_dynapi.o 

OBJECTS += src/events/SDL_categories.o 
OBJECTS += src/events/SDL_clipboardevents.o 
OBJECTS += src/events/SDL_displayevents.o 
OBJECTS += src/events/SDL_dropevents.o 
OBJECTS += src/events/SDL_events.o 
OBJECTS += src/events/SDL_eventwatch.o 
OBJECTS += src/events/SDL_keyboard.o 
OBJECTS += src/events/SDL_keymap.o 
OBJECTS += src/events/SDL_keysym_to_keycode.o 
OBJECTS += src/events/SDL_keysym_to_scancode.o 
OBJECTS += src/events/SDL_mouse.o 
OBJECTS += src/events/SDL_pen.o 
OBJECTS += src/events/SDL_quit.o 
OBJECTS += src/events/SDL_scancode_tables.o 
OBJECTS += src/events/SDL_touch.o 
OBJECTS += src/events/SDL_windowevents.o 
OBJECTS += src/events/imKStoUCS.o 

OBJECTS += src/filesystem/SDL_filesystem.o 

OBJECTS += src/gpu/SDL_gpu.o 
OBJECTS += src/gpu/xr/SDL_gpu_openxr.o 
OBJECTS += src/gpu/xr/SDL_openxrdyn.o 

OBJECTS += src/haptic/SDL_haptic.o 

OBJECTS += src/hidapi/SDL_hidapi.o 

OBJECTS += src/io/SDL_asyncio.o 
OBJECTS += src/io/SDL_iostream.o 
OBJECTS += src/io/generic/SDL_asyncio_generic.o 

OBJECTS += src/joystick/SDL_gamepad.o 
OBJECTS += src/joystick/SDL_joystick.o 
OBJECTS += src/joystick/SDL_steam_virtual_gamepad.o 
OBJECTS += src/joystick/controller_type.o 

OBJECTS += src/locale/SDL_locale.o 

OBJECTS += src/main/SDL_main_callbacks.o 
OBJECTS += src/main/SDL_runapp.o 

OBJECTS += src/misc/SDL_libusb.o 
OBJECTS += src/misc/SDL_url.o 

OBJECTS += src/power/SDL_power.o 

OBJECTS += src/render/SDL_render.o 
OBJECTS += src/render/SDL_render_unsupported.o 
OBJECTS += src/render/SDL_yuv_sw.o 

OBJECTS += src/render/direct3d/SDL_render_d3d.o 
OBJECTS += src/render/direct3d/SDL_shaders_d3d.o 

OBJECTS += src/render/direct3d11/SDL_render_d3d11.o 
OBJECTS += src/render/direct3d11/SDL_shaders_d3d11.o 
OBJECTS += src/render/direct3d12/SDL_render_d3d12.o 
OBJECTS += src/render/direct3d12/SDL_shaders_d3d12.o 

OBJECTS += src/render/gpu/SDL_pipeline_gpu.o 
OBJECTS += src/render/gpu/SDL_render_gpu.o 
OBJECTS += src/render/gpu/SDL_shaders_gpu.o 

OBJECTS += src/render/ngage/SDL_render_ngage.o 

OBJECTS += src/render/opengl/SDL_render_gl.o 
OBJECTS += src/render/opengl/SDL_shaders_gl.o 

OBJECTS += src/render/opengles2/SDL_render_gles2.o 
OBJECTS += src/render/opengles2/SDL_shaders_gles2.o 

OBJECTS += src/render/ps2/SDL_render_ps2.o 
OBJECTS += src/render/psp/SDL_render_psp.o 

OBJECTS += src/render/software/SDL_blendfillrect.o 
OBJECTS += src/render/software/SDL_blendline.o 
OBJECTS += src/render/software/SDL_blendpoint.o 
OBJECTS += src/render/software/SDL_drawline.o 
OBJECTS += src/render/software/SDL_drawpoint.o 
OBJECTS += src/render/software/SDL_render_sw.o 
OBJECTS += src/render/software/SDL_triangle.o 

OBJECTS += src/render/vitagxm/SDL_render_vita_gxm.o 
OBJECTS += src/render/vitagxm/SDL_render_vita_gxm_memory.o 
OBJECTS += src/render/vitagxm/SDL_render_vita_gxm_tools.o 

OBJECTS += src/render/vulkan/SDL_render_vulkan.o 
OBJECTS += src/render/vulkan/SDL_shaders_vulkan.o 

OBJECTS += src/sensor/SDL_sensor.o 

OBJECTS += src/stdlib/SDL_crc16.o 
OBJECTS += src/stdlib/SDL_crc32.o 
OBJECTS += src/stdlib/SDL_getenv.o 
OBJECTS += src/stdlib/SDL_iconv.o 
OBJECTS += src/stdlib/SDL_malloc.o 
OBJECTS += src/stdlib/SDL_memcpy.o 
OBJECTS += src/stdlib/SDL_memmove.o 
OBJECTS += src/stdlib/SDL_memset.o 
OBJECTS += src/stdlib/SDL_mslibc.o 
OBJECTS += src/stdlib/SDL_murmur3.o 
OBJECTS += src/stdlib/SDL_qsort.o 
OBJECTS += src/stdlib/SDL_random.o 
OBJECTS += src/stdlib/SDL_stdlib.o 
OBJECTS += src/stdlib/SDL_string.o 
OBJECTS += src/stdlib/SDL_strtokr.o 

OBJECTS += src/storage/SDL_storage.o 

OBJECTS += src/thread/SDL_thread.o 

OBJECTS += src/time/SDL_time.o 

OBJECTS += src/timer/SDL_timer.o 

OBJECTS += src/video/SDL_RLEaccel.o 
OBJECTS += src/video/SDL_blit.o 
OBJECTS += src/video/SDL_blit_0.o 
OBJECTS += src/video/SDL_blit_1.o 
OBJECTS += src/video/SDL_blit_A.o 
OBJECTS += src/video/SDL_blit_N.o 
OBJECTS += src/video/SDL_blit_auto.o 
OBJECTS += src/video/SDL_blit_copy.o 
OBJECTS += src/video/SDL_blit_slow.o 
OBJECTS += src/video/SDL_bmp.o 
OBJECTS += src/video/SDL_clipboard.o 
OBJECTS += src/video/SDL_egl.o 
OBJECTS += src/video/SDL_fillrect.o 
OBJECTS += src/video/SDL_pixels.o 
OBJECTS += src/video/SDL_rect.o 
OBJECTS += src/video/SDL_rotate.o 
OBJECTS += src/video/SDL_stb.o 
OBJECTS += src/video/SDL_stretch.o 
OBJECTS += src/video/SDL_surface.o 
OBJECTS += src/video/SDL_video.o 
OBJECTS += src/video/SDL_video_unsupported.o 
OBJECTS += src/video/SDL_vulkan_utils.o
OBJECTS += src/video/SDL_yuv.o 

OBJECTS += src/video/yuv2rgb/yuv_rgb_lsx.o 
OBJECTS += src/video/yuv2rgb/yuv_rgb_sse.o 
OBJECTS += src/video/yuv2rgb/yuv_rgb_std.o 

OBJECTS += src/audio/dummy/SDL_dummyaudio.o 

OBJECTS += src/audio/disk/SDL_diskaudio.o 

OBJECTS += src/camera/dummy/SDL_camera_dummy.o 

OBJECTS += src/loadso/dlopen/SDL_sysloadso.o 

OBJECTS += src/joystick/virtual/SDL_virtualjoystick.o 

OBJECTS += src/video/dummy/SDL_nullevents.o 
OBJECTS += src/video/dummy/SDL_nullframebuffer.o 
OBJECTS += src/video/dummy/SDL_nullvideo.o 

OBJECTS += src/audio/alsa/SDL_alsa_audio.o 

OBJECTS += src/audio/pulseaudio/SDL_pulseaudio.o 

OBJECTS += src/video/x11/SDL_x11clipboard.o 
OBJECTS += src/video/x11/SDL_x11dyn.o 
OBJECTS += src/video/x11/SDL_x11events.o 
OBJECTS += src/video/x11/SDL_x11framebuffer.o 
OBJECTS += src/video/x11/SDL_x11keyboard.o 
OBJECTS += src/video/x11/SDL_x11messagebox.o 
OBJECTS += src/video/x11/SDL_x11modes.o 
OBJECTS += src/video/x11/SDL_x11mouse.o 
OBJECTS += src/video/x11/SDL_x11opengl.o 
OBJECTS += src/video/x11/SDL_x11opengles.o 
OBJECTS += src/video/x11/SDL_x11pen.o 
OBJECTS += src/video/x11/SDL_x11settings.o 
OBJECTS += src/video/x11/SDL_x11shape.o 
OBJECTS += src/video/x11/SDL_x11toolkit.o 
OBJECTS += src/video/x11/SDL_x11touch.o 
OBJECTS += src/video/x11/SDL_x11video.o 
OBJECTS += src/video/x11/SDL_x11vulkan.o 
OBJECTS += src/video/x11/SDL_x11window.o 
OBJECTS += src/video/x11/SDL_x11xfixes.o 
OBJECTS += src/video/x11/SDL_x11xinput2.o 
OBJECTS += src/video/x11/SDL_x11xsync.o 
OBJECTS += src/video/x11/SDL_x11xtest.o 
OBJECTS += src/video/x11/edid-parse.o 
OBJECTS += src/video/x11/xsettings-client.o 

##OBJECTS += src/video/wayland/SDL_waylandclipboard.o 
##OBJECTS += src/video/wayland/SDL_waylandcolor.o 
##OBJECTS += src/video/wayland/SDL_waylanddatamanager.o 
##OBJECTS += src/video/wayland/SDL_waylanddyn.o 
##OBJECTS += src/video/wayland/SDL_waylandevents.o 
##OBJECTS += src/video/wayland/SDL_waylandkeyboard.o 
##OBJECTS += src/video/wayland/SDL_waylandmessagebox.o 
##OBJECTS += src/video/wayland/SDL_waylandmouse.o 
##OBJECTS += src/video/wayland/SDL_waylandopengles.o 
##OBJECTS += src/video/wayland/SDL_waylandshmbuffer.o 
##OBJECTS += src/video/wayland/SDL_waylandvideo.o 
##OBJECTS += src/video/wayland/SDL_waylandvulkan.o 
##OBJECTS += src/video/wayland/SDL_waylandwindow.o 

##OBJECTS += wayland-generated-protocols/alpha-modifier-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/color-management-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/cursor-shape-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/fractional-scale-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/frog-color-management-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/idle-inhibit-unstable-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/input-timestamps-unstable-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/keyboard-shortcuts-inhibit-unstable-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/pointer-constraints-unstable-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/pointer-gestures-unstable-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/pointer-warp-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/primary-selection-unstable-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/relative-pointer-unstable-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/tablet-v2-protocol.o 
##OBJECTS += wayland-generated-protocols/text-input-unstable-v3-protocol.o 
##OBJECTS += wayland-generated-protocols/viewporter-protocol.o 
##OBJECTS += wayland-generated-protocols/wayland-protocol.o 
##OBJECTS += wayland-generated-protocols/xdg-activation-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/xdg-decoration-unstable-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/xdg-dialog-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/xdg-foreign-unstable-v2-protocol.o 
##OBJECTS += wayland-generated-protocols/xdg-output-unstable-v1-protocol.o 
##OBJECTS += wayland-generated-protocols/xdg-shell-protocol.o 
##OBJECTS += wayland-generated-protocols/xdg-toplevel-icon-v1-protocol.o 

OBJECTS += src/video/kmsdrm/SDL_kmsdrmdyn.o 
OBJECTS += src/video/kmsdrm/SDL_kmsdrmevents.o 
OBJECTS += src/video/kmsdrm/SDL_kmsdrmmouse.o 
OBJECTS += src/video/kmsdrm/SDL_kmsdrmopengles.o 
OBJECTS += src/video/kmsdrm/SDL_kmsdrmvideo.o 
OBJECTS += src/video/kmsdrm/SDL_kmsdrmvulkan.o 

####################

OBJECTS += src/tray/unix/SDL_tray.o 
OBJECTS += src/core/unix/SDL_appid.o 
OBJECTS += src/core/unix/SDL_fribidi.o 
OBJECTS += src/core/unix/SDL_gtk.o 
OBJECTS += src/core/unix/SDL_libthai.o 
OBJECTS += src/core/unix/SDL_poll.o 
OBJECTS += src/camera/v4l2/SDL_camera_v4l2.o 
OBJECTS += src/haptic/linux/SDL_syshaptic.o 

#IME
###OBJECTS += src/core/linux/SDL_dbus.o 
###OBJECTS += src/core/linux/SDL_system_theme.o 
###OBJECTS += src/core/linux/SDL_progressbar.o 
###OBJECTS += src/core/linux/SDL_ime.o 
###OBJECTS += src/core/linux/SDL_ibus.o 
###OBJECTS += src/core/linux/SDL_fcitx.o 
###OBJECTS += src/core/linux/SDL_udev.o 

OBJECTS += src/core/linux/SDL_evdev.o 
OBJECTS += src/core/linux/SDL_evdev_kbd.o 
OBJECTS += src/core/linux/SDL_evdev_capabilities.o 
OBJECTS += src/core/linux/SDL_threadprio.o 

OBJECTS += src/joystick/hidapi/SDL_hidapi_8bitdo.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_combined.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_flydigi.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_gamecube.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_gamesir.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_gip.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_lg4ff.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_luna.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_ps3.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_ps4.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_ps5.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_rumble.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_shield.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_sinput.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_stadia.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_steam.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_steam_hori.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_steam_triton.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_steamdeck.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_switch.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_switch2.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_wii.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_xbox360.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_xbox360w.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_xboxone.o 
OBJECTS += src/joystick/hidapi/SDL_hidapi_zuiki.o 
OBJECTS += src/joystick/hidapi/SDL_hidapijoystick.o 
OBJECTS += src/joystick/hidapi/SDL_report_descriptor.o 

OBJECTS += src/haptic/hidapi/SDL_hidapihaptic.o 
OBJECTS += src/haptic/hidapi/SDL_hidapihaptic_lg4ff.o 

OBJECTS += src/joystick/linux/SDL_sysjoystick.o 

OBJECTS += src/thread/pthread/SDL_systhread.o 
OBJECTS += src/thread/pthread/SDL_sysmutex.o 
OBJECTS += src/thread/pthread/SDL_syscond.o 
OBJECTS += src/thread/pthread/SDL_sysrwlock.o 
OBJECTS += src/thread/pthread/SDL_systls.o 
OBJECTS += src/thread/pthread/SDL_syssem.o 

OBJECTS += src/misc/unix/SDL_sysurl.o 

OBJECTS += src/power/linux/SDL_syspower.o 

OBJECTS += src/locale/unix/SDL_syslocale.o 

OBJECTS += src/filesystem/unix/SDL_sysfilesystem.o 

OBJECTS += src/storage/generic/SDL_genericstorage.o 
OBJECTS += src/storage/steam/SDL_steamstorage.o 

OBJECTS += src/filesystem/posix/SDL_sysfsops.o 

OBJECTS += src/time/unix/SDL_systime.o 

OBJECTS += src/timer/unix/SDL_systimer.o 

OBJECTS += src/dialog/SDL_dialog.o 
OBJECTS += src/dialog/SDL_dialog_utils.o 
OBJECTS += src/dialog/unix/SDL_unixdialog.o 
OBJECTS += src/dialog/unix/SDL_portaldialog.o 
OBJECTS += src/dialog/unix/SDL_zenitydialog.o 
OBJECTS += src/dialog/unix/SDL_zenitymessagebox.o
 
OBJECTS += src/process/SDL_process.o 
OBJECTS += src/process/posix/SDL_posixprocess.o 

OBJECTS += src/video/offscreen/SDL_offscreenevents.o 
OBJECTS += src/video/offscreen/SDL_offscreenframebuffer.o 
OBJECTS += src/video/offscreen/SDL_offscreenopengles.o 
OBJECTS += src/video/offscreen/SDL_offscreenvideo.o 
OBJECTS += src/video/offscreen/SDL_offscreenvulkan.o 
OBJECTS += src/video/offscreen/SDL_offscreenwindow.o 

OBJECTS += src/tray/SDL_tray_utils.o 

OBJECTS += src/gpu/vulkan/SDL_gpu_vulkan.o 

OBJECTS += src/sensor/dummy/SDL_dummysensor.o 

OBJECTS += src/main/generic/SDL_sysmain_callbacks.o

####################
LIBS := libSDL3.a
BIN := hello

all : $(BIN)

##/usr/bin/ld: /tmp/ccLESpMC.o: in function `quit':
##/home/wmt/krkrz_wamsoft_fork/external/SDL3-f600c74/testgles2.c:115:(.text+0x133a): undefined reference to `SDLTest_CommonQuit'
##/usr/bin/ld: /tmp/ccLESpMC.o: in function `loop_threaded':
##/home/wmt/krkrz_wamsoft_fork/external/SDL3-f600c74/testgles2.c:630:(.text+0x2739): undefined reference to `SDLTest_CommonEvent'
##/usr/bin/ld: /tmp/ccLESpMC.o: in function `loop':
##/home/wmt/krkrz_wamsoft_fork/external/SDL3-f600c74/testgles2.c:644:(.text+0x27be): undefined reference to `SDLTest_CommonEvent'
##/usr/bin/ld: /tmp/ccLESpMC.o: in function `main':
##/home/wmt/krkrz_wamsoft_fork/external/SDL3-f600c74/testgles2.c:683:(.text+0x2923): undefined reference to `SDLTest_CommonCreateState'
##/usr/bin/ld: /home/wmt/krkrz_wamsoft_fork/external/SDL3-f600c74/testgles2.c:690:(.text+0x2966): undefined reference to `SDLTest_CommonArg'
##/usr/bin/ld: /home/wmt/krkrz_wamsoft_fork/external/SDL3-f600c74/testgles2.c:723:(.text+0x2bca): undefined reference to `SDLTest_CommonLogUsage'
##/usr/bin/ld: /home/wmt/krkrz_wamsoft_fork/external/SDL3-f600c74/testgles2.c:746:(.text+0x2cdb): undefined reference to `SDLTest_CommonInit'

###SDL_test_memory_mod.c
###SDL_test_font_mod.c
###SDL_test_log_mod.c 
###SDL_test_common_mod.c 
###testgles2.c

$(BIN) : $(LIBS)
	$(CC) $(CFLAGS) -o $@ triangle_claude.c $(LIBS) -lEGL -lGLESv2 -lpthread -ldl -lm $(LDFLAGS)
	
libSDL3.a : $(OBJECTS)
	$(AR) $@ $(OBJECTS) 
	$(RANLIB) $@
	
%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

test: 
	LD_LIBRARY_PATH=. ./hello
	
clean :
	$(RM) $(OBJECTS) $(BIN) $(LIBS) $(OBJECTS) *.a

