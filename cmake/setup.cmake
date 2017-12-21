#####################################################################
# basic setup
#####################################################################
set(MYLIB_VERSION_MAJOR "0")
set(MYLIB_VERSION_MINOR "1")
set(MYLIB_VERSION_PATCH "1")
set(MYLIB_VERSION "${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}")
set(MYLIB_VERSION_FULL "${MYLIB_VERSION}.${MYLIB_VERSION_PATCH}")

#exe output path and lib or archive output path;
set(EXE_BIN_DIR ${MYLIB_BINARY_DIR}/bin)
set(LIB_DIR ${MYLIB_BINARY_DIR}/lib)

# moudles path
set(CMAKE_MOUDLE_PATH "${MYLIB_SOURCE_DIR}/cmake")

#include path
set(CMAKE_INCLUDE_DIRS ${MYLIB_SOURCE_DIR}/include ${MYLIB_BINARY_DIR}/include)
include_directories(${CMAKE_INCLUDE_DIRS})

#link path
set(CMAKE_LIBRARY_PATH  ${MYLIB_SOURCE_DIR}/lib  ${MYLIB_SOURCE_DIR}/3rdparty/lib ${MYLIB_BINARY_DIR}/lib)

#vc include lib bin
set(VC_INSTALL_ROOT  "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC")
set(VC_INCLUDE  ${VC_INSTALL_ROOT}\include ${VC_INSTALL_ROOT}\atlmfc) 
include_directories(${VC_INCLUDE})
