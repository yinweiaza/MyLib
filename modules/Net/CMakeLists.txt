FILE(GLOB SRCS ./*.cc)
FILE(GLOB HEADHS ./*.h)

include_directories(../)
add_subdirectory(asio)

MYLIB_ADD_LIBRARY(tnet STATIC "${SRCS}")
target_link_libraries(tnet  tnet_asio)

install(FILES ${HEADHS} DESTINATION  ${MYLIB_BINARY_DIR}/include/net/)
