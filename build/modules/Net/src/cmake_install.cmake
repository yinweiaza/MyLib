# Install script for directory: F:/study/MyLib/modules/Net/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/MYLIB")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "F:/study/MyLib/build/bin/tnetd.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "F:/study/MyLib/build/bin" TYPE STATIC_LIBRARY FILES "F:/study/MyLib/build/lib/Debug/tnetd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "F:/study/MyLib/build/bin/tnet.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "F:/study/MyLib/build/bin" TYPE STATIC_LIBRARY FILES "F:/study/MyLib/build/lib/Release/tnet.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "F:/study/MyLib/build/bin/tnet.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "F:/study/MyLib/build/bin" TYPE STATIC_LIBRARY FILES "F:/study/MyLib/build/lib/MinSizeRel/tnet.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "F:/study/MyLib/build/bin/tnet.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "F:/study/MyLib/build/bin" TYPE STATIC_LIBRARY FILES "F:/study/MyLib/build/lib/RelWithDebInfo/tnet.lib")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "F:/study/MyLib/build/lib/address.h;F:/study/MyLib/build/lib/connection.h;F:/study/MyLib/build/lib/connector.h;F:/study/MyLib/build/lib/connector.inl;F:/study/MyLib/build/lib/log.h;F:/study/MyLib/build/lib/nocopyable.h;F:/study/MyLib/build/lib/notifier.h;F:/study/MyLib/build/lib/signaler.h;F:/study/MyLib/build/lib/sockutil.h;F:/study/MyLib/build/lib/spinlock.h;F:/study/MyLib/build/lib/stringutil.h;F:/study/MyLib/build/lib/tcpserver.h;F:/study/MyLib/build/lib/timer.h;F:/study/MyLib/build/lib/timingwheel.h;F:/study/MyLib/build/lib/tnet.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "F:/study/MyLib/build/lib" TYPE FILE FILES
    "F:/study/MyLib/modules/Net/src/address.h"
    "F:/study/MyLib/modules/Net/src/connection.h"
    "F:/study/MyLib/modules/Net/src/connector.h"
    "F:/study/MyLib/modules/Net/src/connector.inl"
    "F:/study/MyLib/modules/Net/src/log.h"
    "F:/study/MyLib/modules/Net/src/nocopyable.h"
    "F:/study/MyLib/modules/Net/src/notifier.h"
    "F:/study/MyLib/modules/Net/src/signaler.h"
    "F:/study/MyLib/modules/Net/src/sockutil.h"
    "F:/study/MyLib/modules/Net/src/spinlock.h"
    "F:/study/MyLib/modules/Net/src/stringutil.h"
    "F:/study/MyLib/modules/Net/src/tcpserver.h"
    "F:/study/MyLib/modules/Net/src/timer.h"
    "F:/study/MyLib/modules/Net/src/timingwheel.h"
    "F:/study/MyLib/modules/Net/src/tnet.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("F:/study/MyLib/build/modules/Net/src/http/cmake_install.cmake")
  include("F:/study/MyLib/build/modules/Net/src/redis/cmake_install.cmake")
  include("F:/study/MyLib/build/modules/Net/src/polarssl/cmake_install.cmake")

endif()

