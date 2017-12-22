set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

set(SRCS
	base64.c
	md5.c
	sha1.c
	)
include_directories(../)

MYLIB_ADD_LIBRARY(tnet_polarssl STATIC "${SRCS}")
target_link_libraries(tnet_polarssl tnet)

set(HEADERS
	base64.h
	md5.h
	sha1.h
	)
install(FILES ${HEADERS} DESTINATION ${MYLIB_BINARY_DIR}/include)
