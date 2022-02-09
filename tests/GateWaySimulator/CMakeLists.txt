cmake_minimum_required(VERSION 3.12)

project(GateWaySimulator C CXX ASM)


#添加可执行文件
add_executable(GateWaySimulator)

#设置C++标准
set_property(TARGET GateWaySimulator PROPERTY CXX_STANDARD 20)

#添加SMGS库
add_subdirectory(../../lib lib)
target_link_libraries(GateWaySimulator SMGS)

#添加argtable3库
cmake_policy(SET CMP0077 NEW)
set(ARGTABLE3_ENABLE_TESTS OFF)
set(ARGTABLE3_ENABLE_EXAMPLES OFF)
add_subdirectory(lib/3rdparty/argtable3/)
target_link_libraries(GateWaySimulator argtable3)
target_include_directories(GateWaySimulator PRIVATE lib/3rdparty/argtable3/src)

#添加线程库
FIND_PACKAGE(Threads REQUIRED)
TARGET_LINK_LIBRARIES(GateWaySimulator  ${CMAKE_THREAD_LIBS_INIT})


#添加源代码
file(GLOB  GateWaySimulator_C_FILES *.cpp *.CPP *.c *.C)

target_sources(GateWaySimulator PUBLIC ${GateWaySimulator_C_FILES})
