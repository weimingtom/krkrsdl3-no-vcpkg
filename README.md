# krkrsdl3-no-vcpkg
[WIP] My krkrsdl3 fork,without vcpkg and ffmpeg, with apt install instead

## Build for Android, under Windows 11   
* cd android_adt\jni
* Edit and double click .\android_adt\jni\console.bat, point to Android NDK by environment variable PATH    
```
::execute ndk-build

::@set PATH=D:\android-ndk-r9c;%PATH%
::@set PATH=D:\android-ndk-r10e;%PATH%
@set PATH=D:\home\soft\android_studio_sdk\ndk\25.2.9519653;%PATH%
@set NDK_MODULE_PATH=%CD%\..\..
@cmd
```
* ndk-build clean
* ndk-build -j8
* (or ndk-build NDK_DEBUG=1 V=1)
* copy ./android_adt/libs to ./android/app/libs (or copy to ./android-project/app/libs)  
* Open ./android (or ./android-project) with Android Studio, build apk and install  
* (You should put data.xp3 into **SUBFOLDER** of the search path, not the top of search path)    

## Build for Linux, for Xubuntu 20.04 64bit in VirtualBox 7.2.8 
* (Below the apt packages before libfreetype-dev are from https://wiki.libsdl.org/SDL3/README-linux)
* sudo apt install gcc g++ gedit git lftp make
* sudo apt install libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxkbcommon-dev
* sudo apt install libasound2-dev 
* (don't use this, use next line: sudo apt install libegl-dev libegl1-mesa-dev libgles2-mesa-dev libgl1-mesa-dev libdrm-dev libgbm-dev)   
* sudo apt install libegl-mesa0 libegl1 libegl-dev libegl1-mesa-dev libgles2-mesa-dev libgl1-mesa-dev libdrm-dev libgbm-dev 
* sudo apt install libfreetype-dev libturbojpeg-dev libopusfile-dev libvorbis-dev libopencv-core-dev libopencv-imgproc-dev libglm-dev libonig-dev
* make clean
* make -j8
* make test
* make debug
* (Not need: libswscale-dev libwebp-dev libopencv-dev)
* (You should execute krkrsdl3 data.xp3 with **FULL PATH (realpath)**, **both krkrsdl3 and data.xp3**, like: /home/wmt/krkrsdl3 /home/wmt/data.xp3, although I may add some codes to fix this issue)  

## About Original vcpkg build apk and vcpkg build linux version
* NOTE: For Android, you should put data.xp3 into **SUBFOLDER** of the search path, not the top of search path  
like this, if you set your search path to /storage/emulated/0/kr2  
data.xp3 file should be put into /storage/emulated/0/kr2/mygame1/data.xp3  
not /storage/emulated/0/kr2/data.xp3  

* NOTE: For Linux, you should execute krkrsdl3 data.xp3 with **FULL PATH (realpath)**, both krkrsdl3 and data.xp3  
like this:  
/home/wmt/krkrsdl3 /home/wmt/data.xp3  
instead of  
./krkrsdl3 ./data.xp3  

## weibo record
```
编译krkrsdl3/krkrsdl3的linux版，可以编译，但运行失败（好像是sdl初始化失败？）。
很想吐槽vcpkg难用，不过算了，反正找到方法改库代码，当没事了。可能下次编译安卓版，
但这次这样，下次可能要等很久以后。暂时未考虑逆向出这个工程的makefile，
因为我感觉这个项目有可能也是瞎搞的，甚至能不能运行也难说

试了一下，好像krkrsdl3/krkrsdl3的安卓版编译在windows下也是不行的，
提示说什么Unable to find a valid Visual Studio instance。
我怎么给你装visual studio呢，我硬盘都快溢出了。所以还是只能用linux编译。
至于作者是怎么在windows下编译，我已经不想深究了，也许可以，但我做不到。
所以只能上xubuntu编译了（那也正常，那些CI持久集成不可能在windows服务器下执行）

我把krkrsdl3/krkrsdl3的安卓版编译出来了，可以运行，只是需要把data.xp3放在data
目录里面才能搜索出来，不解。没看懂原理，好像soft和opengl绘画都支持（不确定），
需要三手指手势才能弹出菜单，除此以外就没有什么特别的（应该没那个flutter版好看）。
暂时不打算改成ndk-build编译（虽然我可以逆向出来），如果要搞也要等9月份以后，
而且这个作者可能在频繁修改，我就不管了，但这个开源项目有一定参考价值，
基于glad和sdl3，我应该会动手改一下的，但现在不研究。另外这个vcpkg编译
反而比较顺利，没有编译linux版那个麻烦

krkrsdl3研究。目标是把它的linux版和android版的非vcpkg版编译出来
（linux版上次运行失败，只有android版可以成功运行），先试试linux Makefile编译，
目前卡在glad，因为我不会用这个库，apt也没有这个包，我先研究一下怎么用
（可能也是纯头文件的），暂时卡在三分之一的位置，所以我可能有时间研究，
没时间就等年底了（我下个月有事）

krkrsdl3研究。linux makefile版可以编译链接了，但跑不了（和上次
用vcpkg编译linux版不同，这次SDL3没crash，但仍然抛C++异常，和SDL3无关），
bug太多了，暂时不想改，改不来好吗，所以下一步编译Android ndk-build版了，
回头再改linux版的bug。我甚至怀疑linux版可能作者自己都没跑通，或者里面涉及
到一些Android相关代码在编译Linux版时被跳过了，反正就是暂时改不下去

krkrsdl3研究。安卓的ndk-build编译版本也编译链接运行成功了，
和之前用vcpkg编译的版本差不多，都是只能创建一个data文件夹，
然后把data.xp3放进去才能识别到（在data目录的上一个目录识别）。
接下来的任务是重新调试linux版（搞完这个就没了，可以开源，
但现在还没开源），
因为linux版目前仍然没跑通。至于之前说的glad，简单来说这个
库它不是纯头文件的，需要编译，而且安卓版和pc版使用的源代码不同，
我是直接从vcpkg生成文件里面复制出来用，懒得研究了

krkrsdl3研究。准确来说这个app没有bug，只是我的理解和它不一样导致
用起来不好用（data.xp3不能放在搜索目录的第一层而应该在第二层）。
首先，这个项目的思路其实有点类似于onscripter，专门有一个
KRKRActivity是负责运行游戏的，而另外一个MainActivity是负责启动器的，
之间会通过参数传递（参考startkrkrsdl方法），而这个参数数组
的第一项就是xp3的绝对路径（不包含file:前缀）。其次，我一开始以为
它的设置游戏根路径搜索游戏的功能有bug，需要创建data目录，但不是这样，
准确来说你创建任意名字的子目录都行，不必叫data，它之所以搜索不到目录
添加到游戏列表，单纯因为data.xp3不能放在第一层目录，而一定要放在第二层目录，
例如选择了/storage/emulated/0/kr2，则这个第一层目录就算包含data.xp3文件
会导致搜索不到，必须在这层目录里创建一个子目录（例如data），
把data.xp3移进去才能显示在游戏列表中，这个判断逻辑在updateGamelist()和
getAppDirectories()和m_gamedir方法中（优先搜索app的files目录，
getFilesDir，然后搜索m_gamedir的子目录（只含目录不含文件，dir.listFiles()
和gameinfo.m_name != null即file.isDirectory()），
但我试过data.xp3放在搜索目录的最顶不行），至于data.xp3这个文件名则是在
buildLaunchArgs()方法中拼接到intent参数中（拼接前是子目录路径名，
拼接后变成data.xp3文件路径名），而data.xp3的名字可以动态配置，
点击游戏列表每一项右侧播放按钮的左边区域（仅限于列表模式）就
会弹出设置对话框

krkrsdl3研究。似乎可以勉强在xubuntu 20.04下跑起来，不过有问题：
遇到文本对话框输出时会卡住，不知道是不是声音卡了；需要把命令行第一个
参数从xp3文件的相对路径改成绝对路径；只能在gdb下运行，如果非调试
模式会在开头就退出。所以暂时还不能放到gh上，继续改 

krkrsdl3研究，好吧，原版代码是对的。我知道为什么运行安卓版和Linux版有区别，
还有为什么运行调试版可以但直接运行则crash，这两个问题的原因是同一个，
TVPApplication.cpp的_KRKRSDL3_LIB代码，如果是安卓版会运行这个条件宏的第一个分支，
如果是Linux版则会运行另一个分支，所以安卓版和Linux版的效果会不一样，
gdb运行和直接运行也会在这里不同，因为argv[0]不同。简单来说，
原版的代码其实可以正常运行，前提是要用elf文件krkrsdl3的完整路径来调用，
如果通过相对路径来调用elf文件就会crash，例如./krkrsdl3 xp3路径名，
需要改成/home/aaa/krkrsdl3 xp3路径名。我改代码自己把realpath集成到代码里面，
就可以不需要在调用时使用完整路径了。相关的修改我会放到gh上，
目前可以在xubuntu 20.04上编译运行


```

------
Original README.md
======

# 介绍

一款基于SDL3开发的krkr-like视觉小说引擎(支持windows/linux/android)。

# 目录结构说明

```
📁android/ # 安卓工程文件夹
📁cpp/ # 主要代码文件夹
├── 📁 core/ # 核心代码
    ├── 📁 tjs2/   # tjs2语言内核代码
    ├── 📁 script/ # tjs2 native绑定代码
    ├── 📁 main/   # 引擎运行内核代码 窗体/事件循环/线程等
    ├── 📁 msg/    # 调试信息/提示信息
    ├── 📁 archive/# 数据包格式相关代码
    ├── 📁 media/  # 媒体文件格式相关代码
        ├── 📁 font/      # 字体系统
        ├── 📁 graphics/  # Layer渲染
        ├── 📁 image/     # 图片解码
        ├── 📁 movie/     # 视频解码
        ├── 📁 sound/     # 音频解码
    ├── 📁 utils/  # 工具包
├── 📁 environ/ # 不同系统/芯片架构之间的差异化代码
├── 📁 plugins/ # 扩展插件代码
├── 📄 krkrsdl.cpp       # 启动文件
├── 📄 krkrsdl_gl.cpp    # 渲染后端文件
├── 📄 krkrsdl_menu.cpp  # 菜单功能文件
📁Res/   # 程序资源文件
📁vcpkg/ # 自定义vcpkg依赖
📄.clang-format # 格式化代码风格定义文件
📄build-*** # 各平台的程序构建脚本
📄CMakeLists.txt/CMakePresets.json # CMake配置文件
📄vcpkg.json/vcpkg-configuration.json # vcpkg配置文件
```

# 依赖库说明

- oniguruma:用于tjs2语言内核的正则表达式匹配
- zlib:基础压缩算法
- ffmpeg:音视频解码
- libpng/libwebp/libjpeg-turbo:图片解码
- libvorbis/opusfile:音频解码
- freetype:字体渲染
- plutosvg:2D矢量图绘制
- sdl3/glad:跨平台核心

使用说明：对于api稳定的库默认采用最新版本，对于api有较大改动的库采用能兼容的最高版本。

# 外部环境依赖

- cmake/ninja:跨平台构建工具
- vcpkg:包管理工具
- LLVM-MinGW/Visual Studio 2022:windows构建工具链
- Android SDK/Android NDK:安卓构建工具链

# 补充说明

系统越复杂问题就越难以排除，所以非必要时请保持当前的构建系统

