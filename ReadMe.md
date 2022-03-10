

# 注意

***当前进度为文档设计，并无实际有效代码。***

# 说明

SimpleMQTTGateWayStack是一个基于MQTT的网关的自定义协议代码，并非标准协议。

通常用于不接入第三方平台时使用。

此源代码不单独使用，通常作为某个固件程序的子模块。

## 源代码下载

由于本源代码包含第三方源代码,故直接下载可能有部分源代码缺失，需要通过以下方法解决:

- 在进行git clone 使用--recurse-submodules参数。

- 若已通过git clone下载,则在源代码目录中执行以下命令下载子模块:

  ```bash
   git submodule update --init --recursive
  ```

  

## 用途

- 编写MQTT Demo与上位机程序。
- 快速开发简易产品。

# 协议

详见 [doc/Protocol]( doc/Protocol)





# C语言版本的协议栈库

目录 [lib/](lib/)

## 库说明

- 依赖极少的简易MQTT网关协议栈库,可方便移植到各种设备上。
- 支持CMake编译,非CMake编译需直接添加源代码程序与头文件包含目录。





# 测试程序

目录 [tests/](tests/)

用于测试协议栈库,通常可进入相应目录使用CMake编译测试程序。若未做特殊说明,通常支持以下编译环境:

- Linux
- MSYS2

# 工具

目录 [tools](tools)

包含部分用于开发阶段调试硬件的工具。