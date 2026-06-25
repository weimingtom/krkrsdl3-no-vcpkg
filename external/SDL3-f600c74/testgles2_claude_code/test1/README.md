工程已创建。文件结构：                                                                                                                                                                                           
                                                                                                                                                                                                                   
  /home/admin/test4/                                                                                                                                                                                               
  ├── CMakeLists.txt                                                                                                                                                                                               
  └── main.c                                                                                                                                                                                                       
                                                                                                                                                                                                                   
  构建方法：                                                                                                                                                                                                       
                                                                                                                                                                                                                   
  cd /home/admin/test4                                                                                                                                                                                             
  cmake -B build                                                                                                                                                                                                   
  cmake --build build                                                                                                                                                                                              
  ./build/red_triangle
                                                                                                                                                                                                                   
  依赖说明： 系统当前未安装 SDL3 和 GLES2 开发库。构建前需安装：
                                                                                                                                                                                                                   
  # Ubuntu/Debian 
  sudo apt install libsdl3-dev libgles2-mesa-dev                                                                                                                                                                   
   
  # 或使用 SDL2 (需修改代码中的 #include <SDL3/...>)                                                                                                                                                               
  sudo apt install libsdl2-dev libgles2-mesa-dev
                                                                                                                                                                                                                   
  代码说明：                                                                                                                                                                                                       
  - main.c — 使用 SDL3 + GLES2 渲染红色三角形
  - CMakeLists.txt — CMake 构建配置                                                                                                                                                                                
  - 三角形居中显示，背景为黑色，按 ESC 或点击关闭窗口
  
  