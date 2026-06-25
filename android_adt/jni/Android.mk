###from krkrz_wamsoft_fork

LOCAL_PATH := $(call my-dir)


#/home/wmt/android-ndk-r27d/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ 
#--target=aarch64-none-linux-android34 
#--sysroot=/home/wmt/android-ndk-r27d/toolchains/llvm/prebuilt/linux-x86_64/sysroot 
#-isystem /home/wmt/krkrsdl3/build/vcpkg_installed/arm64-android/include 
#-isystem /home/wmt/krkrsdl3/build/vcpkg_installed/arm64-android/include/webp 
#-isystem /home/wmt/krkrsdl3/build/vcpkg_installed/arm64-android/include/opencv4 
#-isystem /home/wmt/krkrsdl3/build/vcpkg_installed/arm64-android/include/opus 
#-isystem /home/wmt/krkrsdl3/build/vcpkg_installed/arm64-android/include/plutovg 

####################################
include $(CLEAR_VARS)

LOCAL_MODULE    := krkrsdl3_ext

LOCAL_CFLAGS += \
-DMY_USE_MINLIB \
-DONIG_STATIC \
-DPLUTOVG_BUILD_STATIC \
-D_KRKRSDL3_ANDROID=1 \
-D_KRKRSDL3_EGL=1 \
-D_KRKRSDL3_LIB=1 \
-Dkrkrsdl3_EXPORTS




LOCAL_CPPFLAGS += -DANDROID 
LOCAL_CPPFLAGS += -fdata-sections 
LOCAL_CPPFLAGS += -ffunction-sections 
LOCAL_CPPFLAGS += -funwind-tables 
LOCAL_CPPFLAGS += -fstack-protector-strong 
LOCAL_CPPFLAGS += -no-canonical-prefixes 
LOCAL_CPPFLAGS += -D_FORTIFY_SOURCE=2 -Wformat 
LOCAL_CPPFLAGS += -Werror=format-security 
LOCAL_CPPFLAGS += -frtti 
LOCAL_CPPFLAGS += -fexceptions  
LOCAL_CPPFLAGS += -fPIC   
LOCAL_CPPFLAGS += -fno-limit-debug-info    
LOCAL_CPPFLAGS += -std=gnu++17

LOCAL_CPP_FEATURES += exceptions

LOCAL_C_INCLUDES += \
$(LOCAL_PATH) \
$(LOCAL_PATH)/../../cpp/core/archive \
$(LOCAL_PATH)/../../cpp/core/main \
$(LOCAL_PATH)/../../cpp/core/media/font \
$(LOCAL_PATH)/../../cpp/core/media/image \
$(LOCAL_PATH)/../../cpp/core/media/movie \
$(LOCAL_PATH)/../../cpp/core/media/sound \
$(LOCAL_PATH)/../../cpp/core/msg \
$(LOCAL_PATH)/../../cpp/core/render/public \
$(LOCAL_PATH)/../../cpp/core/render \
$(LOCAL_PATH)/../../cpp/core/script \
$(LOCAL_PATH)/../../cpp/core/tjs2 \
$(LOCAL_PATH)/../../cpp/core/utils \
$(LOCAL_PATH)/../../cpp/core/utils/math \
$(LOCAL_PATH)/../../cpp/core/utils/simd \
$(LOCAL_PATH)/../../cpp/environ \
$(LOCAL_PATH)/../../cpp/plugins \
$(LOCAL_PATH)/../../external/SDL3-f600c74/include-config-relwithdebinfo_android \
$(LOCAL_PATH)/../../external/SDL3-f600c74/include \
$(LOCAL_PATH)/../../external/onig/src \
$(LOCAL_PATH)/../../external/glm \
$(LOCAL_PATH)/../../external/opusfile/include \
$(LOCAL_PATH)/../../external/libogg-1.1.3/include \
$(LOCAL_PATH)/../../external/opus/include \
$(LOCAL_PATH)/../../external/libvorbis-1.2.0/include \
$(LOCAL_PATH)/../../external/freetype/include \
$(LOCAL_PATH)/../../external/libjpeg-turbo \
$(LOCAL_PATH)/../../external/libjpeg-turbo/android \
$(LOCAL_PATH)/../../external/lpng \
$(LOCAL_PATH)/../../external/opencv-2.4.13/modules/imgproc/include \
$(LOCAL_PATH)/../../external/opencv-2.4.13/modules/core/include \
$(LOCAL_PATH)/../../external/SDL_ttf-release-3.2.2/include \
$(LOCAL_PATH)/../../external/glad-arm64-android-rel/include

LOCAL_SRC_FILES += \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjs.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjs.tab.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsArray.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsBinarySerializer.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsByteCodeLoader.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsCompileControl.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsConfig.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsConstArrayData.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsDate.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsdate.tab.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsDateParser.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsDebug.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsDictionary.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsDisassemble.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsError.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsException.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsGlobalStringMap.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsInterCodeExec.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsInterCodeGen.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsInterface.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsLex.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsMath.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsMessage.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsMT19937ar-cok.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsNamespace.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsNative.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsObject.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsObjectExtendable.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsOctPack.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjspp.tab.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsRandomGenerator.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsRegExp.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsScriptBlock.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsScriptCache.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsString.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsUtils.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsVariant.cpp \
$(LOCAL_PATH)/../../cpp/core/tjs2/tjsVariantString.cpp \
$(LOCAL_PATH)/../../external/glad-arm64-android-rel/src/glad.c \
$(LOCAL_PATH)/../../external/glad-arm64-android-rel/src/glad_egl.c \
$(LOCAL_PATH)/../../external/SDL_ttf-release-3.2.2/src/SDL_gpu_textengine.c \
$(LOCAL_PATH)/../../external/SDL_ttf-release-3.2.2/src/SDL_hashtable_ttf.c \
$(LOCAL_PATH)/../../external/SDL_ttf-release-3.2.2/src/SDL_surface_textengine.c \
$(LOCAL_PATH)/../../external/SDL_ttf-release-3.2.2/src/SDL_hashtable.c \
$(LOCAL_PATH)/../../external/SDL_ttf-release-3.2.2/src/SDL_renderer_textengine.c \
$(LOCAL_PATH)/../../external/SDL_ttf-release-3.2.2/src/SDL_ttf.c


include $(BUILD_STATIC_LIBRARY)

####################################
####################################

include $(CLEAR_VARS)

LOCAL_MODULE    := krkrsdl3

LOCAL_CFLAGS += \
-DMY_USE_MINLIB \
-DONIG_STATIC \
-DPLUTOVG_BUILD_STATIC \
-D_KRKRSDL3_ANDROID=1 \
-D_KRKRSDL3_EGL=1 \
-D_KRKRSDL3_LIB=1 \
-Dkrkrsdl3_EXPORTS




LOCAL_CPPFLAGS += -DANDROID 
LOCAL_CPPFLAGS += -fdata-sections 
LOCAL_CPPFLAGS += -ffunction-sections 
LOCAL_CPPFLAGS += -funwind-tables 
LOCAL_CPPFLAGS += -fstack-protector-strong 
LOCAL_CPPFLAGS += -no-canonical-prefixes 
LOCAL_CPPFLAGS += -D_FORTIFY_SOURCE=2 -Wformat 
LOCAL_CPPFLAGS += -Werror=format-security 
LOCAL_CPPFLAGS += -frtti 
LOCAL_CPPFLAGS += -fexceptions  
LOCAL_CPPFLAGS += -fPIC   
LOCAL_CPPFLAGS += -fno-limit-debug-info    
LOCAL_CPPFLAGS += -std=gnu++17

LOCAL_CPP_FEATURES += exceptions

LOCAL_C_INCLUDES += \
$(LOCAL_PATH) \
$(LOCAL_PATH)/../../cpp/core/archive \
$(LOCAL_PATH)/../../cpp/core/main \
$(LOCAL_PATH)/../../cpp/core/media/font \
$(LOCAL_PATH)/../../cpp/core/media/image \
$(LOCAL_PATH)/../../cpp/core/media/movie \
$(LOCAL_PATH)/../../cpp/core/media/sound \
$(LOCAL_PATH)/../../cpp/core/msg \
$(LOCAL_PATH)/../../cpp/core/render/public \
$(LOCAL_PATH)/../../cpp/core/render \
$(LOCAL_PATH)/../../cpp/core/script \
$(LOCAL_PATH)/../../cpp/core/tjs2 \
$(LOCAL_PATH)/../../cpp/core/utils \
$(LOCAL_PATH)/../../cpp/core/utils/math \
$(LOCAL_PATH)/../../cpp/core/utils/simd \
$(LOCAL_PATH)/../../cpp/environ \
$(LOCAL_PATH)/../../cpp/plugins \
$(LOCAL_PATH)/../../external/SDL3-f600c74/include-config-relwithdebinfo_android \
$(LOCAL_PATH)/../../external/SDL3-f600c74/include \
$(LOCAL_PATH)/../../external/onig/src \
$(LOCAL_PATH)/../../external/glm \
$(LOCAL_PATH)/../../external/opusfile/include \
$(LOCAL_PATH)/../../external/libogg-1.1.3/include \
$(LOCAL_PATH)/../../external/opus/include \
$(LOCAL_PATH)/../../external/libvorbis-1.2.0/include \
$(LOCAL_PATH)/../../external/freetype/include \
$(LOCAL_PATH)/../../external/libjpeg-turbo \
$(LOCAL_PATH)/../../external/libjpeg-turbo/android \
$(LOCAL_PATH)/../../external/lpng \
$(LOCAL_PATH)/../../external/opencv-2.4.13/modules/imgproc/include \
$(LOCAL_PATH)/../../external/opencv-2.4.13/modules/core/include \
$(LOCAL_PATH)/../../external/SDL_ttf-release-3.2.2/include \
$(LOCAL_PATH)/../../external/glad-arm64-android-rel/include

LOCAL_SRC_FILES += \
$(LOCAL_PATH)/../../cpp/core/archive/TARArchive.cpp \
$(LOCAL_PATH)/../../cpp/core/archive/TextStream.cpp \
$(LOCAL_PATH)/../../cpp/core/archive/TVPArchive.cpp \
$(LOCAL_PATH)/../../cpp/core/archive/TVPStorage.cpp \
$(LOCAL_PATH)/../../cpp/core/archive/UtilStreams.cpp \
$(LOCAL_PATH)/../../cpp/core/archive/XP3Archive.cpp \
$(LOCAL_PATH)/../../cpp/core/archive/ZIPArchive.cpp \
$(LOCAL_PATH)/../../cpp/core/archive/encoding/gbk2unicode.c \
$(LOCAL_PATH)/../../cpp/core/archive/encoding/jis2unicode.c \
$(LOCAL_PATH)/../../cpp/core/archive/minizip/unzip.c \
$(LOCAL_PATH)/../../cpp/core/archive/minizip/ioapi.cpp \
$(LOCAL_PATH)/../../cpp/core/main/NativeEventQueue.cpp \
$(LOCAL_PATH)/../../cpp/core/main/SystemControl.cpp \
$(LOCAL_PATH)/../../cpp/core/main/TVPApplication.cpp \
$(LOCAL_PATH)/../../cpp/core/main/TVPEvent.cpp \
$(LOCAL_PATH)/../../cpp/core/main/TVPSystem.cpp \
$(LOCAL_PATH)/../../cpp/core/main/TVPThread.cpp \
$(LOCAL_PATH)/../../cpp/core/main/WindowIntf.cpp \
$(LOCAL_PATH)/../../cpp/core/media/font/CharacterData.cpp \
$(LOCAL_PATH)/../../cpp/core/media/font/FreeTypeFontRasterizer.cpp \
$(LOCAL_PATH)/../../cpp/core/media/font/FreeTypeImp.cpp \
$(LOCAL_PATH)/../../cpp/core/media/font/PrerenderedFont.cpp \
$(LOCAL_PATH)/../../cpp/core/media/font/TVPFont.cpp \
$(LOCAL_PATH)/../../cpp/core/media/image/BitmapInfomation.cpp \
$(LOCAL_PATH)/../../cpp/core/media/image/GraphicsLoadThread.cpp \
$(LOCAL_PATH)/../../cpp/core/media/image/LoadJPEG.cpp \
$(LOCAL_PATH)/../../cpp/core/media/image/LoadPNG.cpp \
$(LOCAL_PATH)/../../cpp/core/media/image/LoadTLG.cpp \
$(LOCAL_PATH)/../../cpp/core/media/image/LoadWEBP.cpp \
$(LOCAL_PATH)/../../cpp/core/media/image/SaveTLG5.cpp \
$(LOCAL_PATH)/../../cpp/core/media/image/SaveTLG6.cpp \
$(LOCAL_PATH)/../../cpp/core/media/image/TVPGraphicsLoader.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/CodecAudioFFmpeg.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/CodecDemuxFFmpeg.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/CodecVideoFFmpeg.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/InputStream.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/krffmpeg.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/KRMovieLayer.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/KRMovieOverlay.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/KRStreamInfo.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/Message.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/MessageQueue.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/VideoPlayer.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/VideoPlayerAudio.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/VideoPlayerVideo.cpp \
$(LOCAL_PATH)/../../cpp/core/media/movie/VideoReferenceClock.cpp \
$(LOCAL_PATH)/../../cpp/core/media/sound/FFWaveDecoder.cpp \
$(LOCAL_PATH)/../../cpp/core/media/sound/OpusWaveDecoder.cpp \
$(LOCAL_PATH)/../../cpp/core/media/sound/VorbisWaveDecoder.cpp \
$(LOCAL_PATH)/../../cpp/core/media/sound/WaveIntf.cpp \
$(LOCAL_PATH)/../../cpp/core/media/sound/WaveLoopManager.cpp \
$(LOCAL_PATH)/../../cpp/core/media/sound/WaveSegmentQueue.cpp \
$(LOCAL_PATH)/../../cpp/core/msg/Log.cpp \
$(LOCAL_PATH)/../../cpp/core/msg/TVPDebug.cpp \
$(LOCAL_PATH)/../../cpp/core/msg/TVPMsg.cpp \
$(LOCAL_PATH)/../../cpp/core/render/gl/blend_function.cpp \
$(LOCAL_PATH)/../../cpp/core/render/gl/tvpgl.cpp \
$(LOCAL_PATH)/../../cpp/core/render/ComplexRect.cpp \
$(LOCAL_PATH)/../../cpp/core/render/DrawDevice.cpp \
$(LOCAL_PATH)/../../cpp/core/render/LayerBitmap.cpp \
$(LOCAL_PATH)/../../cpp/core/render/LayerManager.cpp \
$(LOCAL_PATH)/../../cpp/core/render/LayerTreeOwner.cpp \
$(LOCAL_PATH)/../../cpp/core/render/RenderManager.cpp \
$(LOCAL_PATH)/../../cpp/core/render/RenderManagerGL.cpp \
$(LOCAL_PATH)/../../cpp/core/render/TransIntf.cpp \
$(LOCAL_PATH)/../../cpp/core/render/TVPColor.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeAsyncTrigger.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeBasicDrawDevice.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeBitmap.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeBitmapLayerTreeOwner.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeCDDASoundBuffer.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeClipboard.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeDebug.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeFont.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeImageFunction.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeKAGParser.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeLayer.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeMenuItem.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeMIDISoundBuffer.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativePad.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativePhaseVocoder.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativePlugins.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeRect.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeScript.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeStorages.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeSystem.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeTimer.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeVideoOverlay.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeWaveSoundBuffer.cpp \
$(LOCAL_PATH)/../../cpp/core/script/tjsNativeWindow.cpp \
$(LOCAL_PATH)/../../cpp/core/script/TVPScript.cpp \
 \
$(LOCAL_PATH)/../../cpp/core/utils/PhaseVocoderDSP.cpp \
$(LOCAL_PATH)/../../cpp/core/utils/Random.cpp \
$(LOCAL_PATH)/../../cpp/core/utils/RealFFT.cpp \
$(LOCAL_PATH)/../../cpp/core/utils/ResampleImage.cpp \
$(LOCAL_PATH)/../../cpp/core/utils/TickCount.cpp \
$(LOCAL_PATH)/../../cpp/core/utils/TVPTimer.cpp \
$(LOCAL_PATH)/../../cpp/core/utils/VelocityTracker.cpp \
$(LOCAL_PATH)/../../cpp/core/utils/WeightFunctor.cpp \
$(LOCAL_PATH)/../../cpp/core/utils/math/md5.c \
$(LOCAL_PATH)/../../cpp/core/utils/math/xxhash.c \
$(LOCAL_PATH)/../../cpp/core/utils/math/xxtea.cpp \
$(LOCAL_PATH)/../../cpp/core/utils/simd/simd_image.cpp \
$(LOCAL_PATH)/../../cpp/environ/DetectCPU.cpp \
$(LOCAL_PATH)/../../cpp/environ/KeyCodeConv.cpp \
$(LOCAL_PATH)/../../cpp/environ/MainWindowLayer.cpp \
$(LOCAL_PATH)/../../cpp/environ/Other.cpp \
$(LOCAL_PATH)/../../cpp/environ/TVPConfig.cpp \
$(LOCAL_PATH)/../../cpp/environ/WaveMixer.cpp \
$(LOCAL_PATH)/../../cpp/environ/android/Platform.cpp \
$(LOCAL_PATH)/../../cpp/plugins/ncbind/ncbind.cpp \
$(LOCAL_PATH)/../../cpp/plugins/addFont.cpp \
$(LOCAL_PATH)/../../cpp/plugins/AlphaMovie.cpp \
$(LOCAL_PATH)/../../cpp/plugins/csvParser.cpp \
$(LOCAL_PATH)/../../cpp/plugins/dirlist.cpp \
$(LOCAL_PATH)/../../cpp/plugins/DrawDeviceD3D.cpp \
$(LOCAL_PATH)/../../cpp/plugins/expat.cpp \
$(LOCAL_PATH)/../../cpp/plugins/Extension.cpp \
$(LOCAL_PATH)/../../cpp/plugins/fftgraph.cpp \
$(LOCAL_PATH)/../../cpp/plugins/fstat.cpp \
$(LOCAL_PATH)/../../cpp/plugins/FuncStubs.cpp \
$(LOCAL_PATH)/../../cpp/plugins/getabout.cpp \
$(LOCAL_PATH)/../../cpp/plugins/getSample.cpp \
$(LOCAL_PATH)/../../cpp/plugins/gfxEffect.cpp \
$(LOCAL_PATH)/../../cpp/plugins/gfxFire.cpp \
$(LOCAL_PATH)/../../cpp/plugins/json.cpp \
$(LOCAL_PATH)/../../cpp/plugins/kirikiroid2.cpp \
$(LOCAL_PATH)/../../cpp/plugins/LayerExAreaAverage.cpp \
$(LOCAL_PATH)/../../cpp/plugins/LayerExBase.cpp \
$(LOCAL_PATH)/../../cpp/plugins/LayerExBTOA.cpp \
$(LOCAL_PATH)/../../cpp/plugins/LayerExImage.cpp \
$(LOCAL_PATH)/../../cpp/plugins/LayerExMovie.cpp \
$(LOCAL_PATH)/../../cpp/plugins/LayerExPerspective.cpp \
$(LOCAL_PATH)/../../cpp/plugins/LayerExRaster.cpp \
$(LOCAL_PATH)/../../cpp/plugins/LayerExSave.cpp \
$(LOCAL_PATH)/../../cpp/plugins/motionPlayer.cpp \
$(LOCAL_PATH)/../../cpp/plugins/PackinOne.cpp \
$(LOCAL_PATH)/../../cpp/plugins/saveStruct.cpp \
$(LOCAL_PATH)/../../cpp/plugins/scriptsEx.cpp \
$(LOCAL_PATH)/../../cpp/plugins/shrinkCopy.cpp \
$(LOCAL_PATH)/../../cpp/plugins/textrender.cpp \
$(LOCAL_PATH)/../../cpp/plugins/TVPPlugin.cpp \
$(LOCAL_PATH)/../../cpp/plugins/varfile.cpp \
$(LOCAL_PATH)/../../cpp/plugins/win32dialog.cpp \
$(LOCAL_PATH)/../../cpp/plugins/windowEx.cpp \
$(LOCAL_PATH)/../../cpp/plugins/wutcwf.cpp \
$(LOCAL_PATH)/../../cpp/plugins/xp3filter.cpp \
$(LOCAL_PATH)/../../cpp/plugins/emoteplayer/emotefile.cpp \
$(LOCAL_PATH)/../../cpp/plugins/emoteplayer/emoteplayer.cpp \
$(LOCAL_PATH)/../../cpp/plugins/emoteplayer/emoteplayerclass.cpp \
$(LOCAL_PATH)/../../cpp/plugins/emoteplayer/emoterunner.cpp \
$(LOCAL_PATH)/../../cpp/plugins/extrans/extrans.cpp \
$(LOCAL_PATH)/../../cpp/plugins/extrans/mosaic.cpp \
$(LOCAL_PATH)/../../cpp/plugins/extrans/ripple.cpp \
$(LOCAL_PATH)/../../cpp/plugins/extrans/rotatebase.cpp \
$(LOCAL_PATH)/../../cpp/plugins/extrans/rotatetrans.cpp \
$(LOCAL_PATH)/../../cpp/plugins/extrans/turn.cpp \
$(LOCAL_PATH)/../../cpp/plugins/extrans/turntrans_table.cpp \
$(LOCAL_PATH)/../../cpp/plugins/extrans/wave.cpp \
$(LOCAL_PATH)/../../cpp/plugins/KAGParserEx/KAG.cpp \
$(LOCAL_PATH)/../../cpp/plugins/KAGParserEx/KAGParserEx.cpp \
$(LOCAL_PATH)/../../cpp/plugins/ExtKAGParser/ExtKAG.cpp \
$(LOCAL_PATH)/../../cpp/plugins/ExtKAGParser/ExtKAGParser.cpp \
$(LOCAL_PATH)/../../cpp/plugins/ExtKAGParser/TJSAry.cpp \
$(LOCAL_PATH)/../../cpp/plugins/ExtKAGParser/TJSDic.cpp \
$(LOCAL_PATH)/../../cpp/plugins/LayerExDraw/LayerExDraw.cpp \
$(LOCAL_PATH)/../../cpp/plugins/psbfile/PSB.cpp \
$(LOCAL_PATH)/../../cpp/plugins/psbfile/PSBData.cpp \
$(LOCAL_PATH)/../../cpp/plugins/psbfile/PSBFile.cpp \
$(LOCAL_PATH)/../../cpp/plugins/psbfile/PSBMedia.cpp \
$(LOCAL_PATH)/../../cpp/plugins/sqlite3/extend.cpp \
$(LOCAL_PATH)/../../cpp/plugins/sqlite3/sqlite3_.c \
$(LOCAL_PATH)/../../cpp/plugins/sqlite3/sqlite3.cpp \
$(LOCAL_PATH)/../../cpp/plugins/sqlite3/xp3_vfs.cpp \
$(LOCAL_PATH)/../../cpp/krkrsdl.cpp \
$(LOCAL_PATH)/../../cpp/krkrsdl_gl.cpp \
$(LOCAL_PATH)/../../cpp/krkrsdl_menu.cpp


## rename sqlite3.c to sqlite3_.c

ifneq ($(filter $(TARGET_ARCH_ABI), x86_64 x86),)
endif

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM -lGLESv3 -lOpenSLES -lz 
#-latomic
#-lGLESv2

###LOCAL_WHOLE_STATIC_LIBRARIES += core_tjs core_extension_visual_simd core_utils core_visual core_sound core_movie core_plugin_environ core_base plugins 
LOCAL_WHOLE_STATIC_LIBRARIES += krkrsdl3_ext
LOCAL_WHOLE_STATIC_LIBRARIES += libonig libpng libfreetype vorbis ogg libopusfile libopus libjpeg-turbo opencv_core_imgproc
###bpg uchardet oboe webp tinyxml2 lz4 libopenal fmt


LOCAL_STATIC_LIBRARIES := 
#LOCAL_STATIC_LIBRARIES += android_native_app_glue cpufeatures ndk_helper
#see blow call import-module

LOCAL_SHARED_LIBRARIES :=
LOCAL_SHARED_LIBRARIES += SDL3
#SDL2 
#libopenal

include $(BUILD_SHARED_LIBRARY)

$(call import-module,external/freetype)
$(call import-module,external/libjpeg-turbo)
$(call import-module,external/onig)
$(call import-module,external/lpng)
$(call import-module,external/opusfile)
$(call import-module,external/opus)
###$(call import-module,external/libogg)

###$(call import-module,android/cpufeatures)
###$(call import-module,android/native_app_glue)
###$(call import-module,android/ndk_helper)

$(call import-module,external/opencv-2.4.13)
$(call import-module,external/SDL3-f600c74)
###$(call import-module,external/oboe-1.8.0)
###$(call import-module,external/fmt-11.2.0)
###$(call import-module,external/openal-soft-1.17.0)
###$(call import-module,external/libwebp-1.0.0)
###$(call import-module,external/tinyxml2-11.0.0)
###$(call import-module,external/lz4-1.10.0)
$(call import-module,external/libogg-1.1.3)
$(call import-module,external/libvorbis-1.2.0)
