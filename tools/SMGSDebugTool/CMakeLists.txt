
Project(SMGSDebugTool)

cmake_minimum_required(VERSION 3.8)


find_program(BASH
                NAMES sh bash sh.exe bash.exe
                REQUIRED)

message( "BASH : ${BASH}")


find_program(WX-CONFIG 
		NAMES wx-config
		REQUIRED)

option(STATIC_BUILD "static build" OFF)

message( "WX-CONFIG :${WX-CONFIG} ")

if(STATIC_BUILD)
	set(WX_CONFIG_STATIC_FLAGS "--static")
endif()

execute_process(COMMAND ${BASH} ${WX-CONFIG} ${WX_CONFIG_STATIC_FLAGS} --cxxflags all 
		OUTPUT_VARIABLE WX_CXXFLAGS)


execute_process(COMMAND ${BASH} ${WX-CONFIG} ${WX_CONFIG_STATIC_FLAGS} --cflags all 
                OUTPUT_VARIABLE WX_CFLAGS)

execute_process(COMMAND ${BASH} ${WX-CONFIG} ${WX_CONFIG_STATIC_FLAGS} --libs all 
                OUTPUT_VARIABLE WX_LIBS)

string(FIND "${WX_CFLAGS}" "\n" WX_CFLAGS_LEN)
string(FIND "${WX_CXXFLAGS}" "\n" WX_CXXFLAGS_LEN)
string(FIND "${WX_LIBS}" "\n" WX_LIBS_LEN)

string(SUBSTRING "${WX_CFLAGS}" 0 ${WX_CFLAGS_LEN} WX_CFLAGS)
string(SUBSTRING "${WX_CXXFLAGS}" 0 ${WX_CXXFLAGS_LEN} WX_CXXFLAGS)
string(SUBSTRING "${WX_LIBS}" 0 ${WX_LIBS_LEN} WX_LIBS)


message( "WX-CFLAGS:${WX_CFLAGS} WX-CXXFLAGS:${WX_CXXFLAGS} WX-LIBS:${WX_LIBS}")

set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} ${WX_CFLAGS} ")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} ${WX_CXXFLAGS} ")



file(GLOB MAIN_C_CXX_FILES *.c *.cpp MainFrame/*cpp MainFrame/*c )

list(APPEND MAIN_SRCS ${MAIN_C_CXX_FILES})

if(WIN32)
	file(GLOB_RECURSE MAIN_RC_FILES win32/*.rc)
	list(APPEND  MAIN_SRCS ${MAIN_RC_FILES})
endif()

set(MAIN_LIBS "${WX_LIBS}   -O2")

if(WIN32)
	set(MAIN_LIBS "${MAIN_LIBS} -static-libgcc -static-libstdc++")
endif()

if(STATIC_BUILD)
	set(MAIN_LIBS "${MAIN_LIBS} -static")
endif()

include_directories(
.
MainFrame
)


add_executable(${PROJECT_NAME} ${MAIN_SRCS})
target_link_libraries(${PROJECT_NAME} ${MAIN_LIBS})

#添加WS2_32(WIN32)
if(WIN32)
target_link_libraries(${PROJECT_NAME}  ws2_32)
endif()

#添加libSMGSS库
add_subdirectory(../lib lib)
target_link_libraries(${PROJECT_NAME} SMGSS)

#添加Sqlite3
find_package(PkgConfig)
pkg_check_modules(SQLITE3 REQUIRED IMPORTED_TARGET  sqlite3 )
target_link_libraries(${PROJECT_NAME} PkgConfig::SQLITE3)
