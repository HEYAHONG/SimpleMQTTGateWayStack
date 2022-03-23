# 注意

- 现阶段只支持发送QOS0的消息。

# 说明

简易MQTT网关协议栈调试工具,模拟后台对网关的操作,仅用于网关数量较少时的调试工作。

## 依赖库

- [wxWidgets](http://wxwidgets.org/)
- [sqlite3](https://www.sqlite.org)

## GUI设计工具

[wxFormBuilder](https://github.com/wxFormBuilder/wxFormBuilder)

# 编译

采用CMAKE编译,理论上只要安装好wxWidgets与sqlite3即可编译。

主要支持与测试以下编译环境:

- Ubuntu 20.04

  ```bash
  #安装相应工具及库
  sudo apt-get install build-essential make cmake pkg-config libsqlite3-dev libwxgtk3.0-gtk3-dev libwxgtk-media3.0-gtk3-dev libwxgtk-webview3.0-gtk3-dev
  #创建目录
  mkdir build
  #进入刚才的目录并使用CMake生成工程
  cd build 
  cmake ../
  #编译
  make
  ```

  

- MSYS2 Mingw32

  ```bash
  #安装相应工具及库
  pacman -S mingw-w64-i686-cmake mingw-w64-i686-ninja mingw-w64-i686-make mingw-w64-i686-pkgconf mingw-w64-i686-toolchain mingw-w64-i686-wxWidgets mingw-w64-i686-sqlite3
  #创建目录
  mkdir build
  #进入刚才的目录并使用CMake生成工程
  cd build 
  cmake ../
  #编译(默认生成ninja工程使用ninja编译,若默认生成make工程则使用mingw32-make编译)
  ninja #ninja工程
  mingw32-make #make工程
  ```

  
