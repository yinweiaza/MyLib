set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

FILE(GLOB  SRCS ./*.cpp)
FILE(GLOB  HEADERS ./*.h)

include_directories(../)

MYLIB_ADD_LIBRARY(tnet_ftp STATIC "${SRCS}")

install(FILES ${HEADERS} DESTINATION ${MYLIB_BINARY_DIR}/include/ftp)

