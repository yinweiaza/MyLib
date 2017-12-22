FILE(GLOB SRCS ./*.cc)
FILE(GLOB HEADHS ./*.h)

include_directories(../)

MYLIB_ADD_LIBRARY(tnet_asio STATIC "${SRCS}")

install(FILES ${HEADHS} DESTINATION  ${MYLIB_BINARY_DIR}/include/net/asio)
