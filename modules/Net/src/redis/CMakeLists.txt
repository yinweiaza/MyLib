set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

set (SRCS
    hiredis.c
    redisclient.cpp
    redisconnection.cpp
    redistrans.cpp
    sds.c
    )

include_directories(../)

MYLIB_ADD_LIBRARY(tnet_redis  STATIC  "${SRCS}") 
target_link_libraries(tnet_redis tnet)

#install(TARGETS tnet_redis DESTINATION ${MYLIB_BINARY_DIR}/lib)

set (HEADERS
    hiredis.h
    redisclient.h
    redistrans.h
    tnet_redis.h
    )

install(FILES ${HEADERS} DESTINATION ${MYLIB_BINARY_DIR}/include) 
   
