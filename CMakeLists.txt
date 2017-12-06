CMAKE_MINIMUM_REQUIRED(VERSION 2.5 FATAL_ERROR)
#set(CMAKE_C_COMPILER "C:/MinGW/bin/mingw32-gcc.exe")
#set(CMAKE_C_FLAGS "-g -Wall")
#set(CMAKE_CXX_COMPILER "C:/MinGW/bin/mingw32-g++.exe")
#set(CMAKE_CXX_FLAGS "-g -Wall")
Project(MYLIB CXX C)
if( " ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "
	FATAL: In Source builds are not allowed,
	you should create separate directory for build files.
	")
SET(CMAKE_BUILD_TYPE release)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(APP)
