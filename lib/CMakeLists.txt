cmake_minimum_required(VERSION 3.10)

project(libSMGS C CXX ASM)

#添加静态库
add_library(SMGS STATIC)

#添加包含目录
target_include_directories(SMGS PRIVATE include)
target_include_directories(SMGS PUBLIC ./)

#添加源代码
file(GLOB  LIBSMGS_C_FILES *.c *.C *.h *.H)

target_sources(SMGS PUBLIC ${LIBSMGS_C_FILES})

#添加paho.mqtt.embedded-c源代码
file(GLOB PAHO_MQTT_EMBEDDED_C_SOURCES
"3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/*.c"
"3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/*.h"
)
target_sources(SMGS PUBLIC ${PAHO_MQTT_EMBEDDED_C_SOURCES})
target_include_directories(SMGS PUBLIC 3rdparty/paho.mqtt.embedded-c/MQTTPacket/src/)

#添加protobuf-c源代码
file(GLOB PROTOBUF_C_SOURCES
"3rdparty/protobuf-c/protobuf-c/*.c"
"3rdparty/protobuf-c/protobuf-c/*.h"
)
target_sources(SMGS PUBLIC ${PROTOBUF_C_SOURCES})
target_include_directories(SMGS PUBLIC 3rdparty/3rdparty/protobuf-c/protobuf-c/)
