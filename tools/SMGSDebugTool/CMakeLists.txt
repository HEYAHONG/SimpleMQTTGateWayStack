
Project(SMGSDebugTool)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

option(STATIC_BUILD "static build" OFF)


file(GLOB MAIN_C_CXX_FILES *.c *.cpp MainFrame/*cpp MainFrame/*c )

list(APPEND MAIN_SRCS ${MAIN_C_CXX_FILES})

if(WIN32)
	file(GLOB_RECURSE MAIN_RC_FILES win32/*.rc)
	list(APPEND  MAIN_SRCS ${MAIN_RC_FILES})
endif()


if(WIN32)
	set(MAIN_LIBS "-static-libgcc -static-libstdc++")
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

#添加线程库
FIND_PACKAGE(Threads REQUIRED)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${CMAKE_THREAD_LIBS_INIT})

#添加wxWidgets库
if(STATIC_BUILD)
        set(wxWidgets_USE_STATIC ON)
endif()
find_package(wxWidgets REQUIRED all )
include(${wxWidgets_USE_FILE})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

