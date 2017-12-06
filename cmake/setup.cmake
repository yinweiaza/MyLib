#####################################################################
# basic setup
#####################################################################
set(MYLIB_VERSION_MAJOR "0")
set(MYLIB_VERSION_MINOR "1")
set(MYLIB_VERSION_PATCH "1")
set(MYLIB_VERSION "${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}")
set(MYLIB_VERSION_FULL "${MYLIB_VERSION}.${MYLIB_VERSION_PATCH}")


set(EXECUTABLE_OUTUPT_PATH ${MYLIB_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${MYLIB_BINARY_LIB}/lib)
set(ARCHIVE_OUTPUT_PATH ${MYLIB_BINARY_LIB}/lib)

# moudles path
set(CMAKE_MOUDLE_PATH "${MYLIB_SOURCE_DIR}/cmake")

#include path
set(CMAKE_INCLUDE_DIRS ${MYLIB_SOURCE_DIR} ${MYLIB_BINARY_DIR})
include_directories(${CMAKE_INCLUDE_DIRS})

#install locations (NO_IMPLIMENT)

