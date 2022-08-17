cmake_minimum_required(VERSION 3.10)

project(libSMGSS C CXX ASM)

#添加静态库
add_library(SMGSS STATIC)

#添加包含目录
target_include_directories(SMGSS PUBLIC ./)

#添加源代码
file(GLOB  LIBSMGSS_C_FILES
*.c
*.h
*.C
*.H
)

target_sources(SMGSS PUBLIC ${LIBSMGSS_C_FILES})

#添加SMGS
add_subdirectory(../../lib libSMGS)
target_link_libraries(SMGSS SMGS)

