

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