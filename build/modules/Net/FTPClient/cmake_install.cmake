# Install script for directory: F:/study/MyLib/modules/Net/FTPClient

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
     "F:/study/MyLib/build/bin/tnet_ftpd.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "F:/study/MyLib/build/bin" TYPE STATIC_LIBRARY FILES "F:/study/MyLib/build/lib/Debug/tnet_ftpd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "F:/study/MyLib/build/bin/tnet_ftp.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "F:/study/MyLib/build/bin" TYPE STATIC_LIBRARY FILES "F:/study/MyLib/build/lib/Release/tnet_ftp.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "F:/study/MyLib/build/bin/tnet_ftp.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "F:/study/MyLib/build/bin" TYPE STATIC_LIBRARY FILES "F:/study/MyLib/build/lib/MinSizeRel/tnet_ftp.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "F:/study/MyLib/build/bin/tnet_ftp.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "F:/study/MyLib/build/bin" TYPE STATIC_LIBRARY FILES "F:/study/MyLib/build/lib/RelWithDebInfo/tnet_ftp.lib")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "F:/study/MyLib/build/include/ftp/AutoLock.h;F:/study/MyLib/build/include/ftp/FTPClientImp.h;F:/study/MyLib/build/include/ftp/FtpxData.h;F:/study/MyLib/build/include/ftp/FtpxLibImp.h;F:/study/MyLib/build/include/ftp/GroupFile.h;F:/study/MyLib/build/include/ftp/ThrdControl.h;F:/study/MyLib/build/include/ftp/ThrdTransfer.h;F:/study/MyLib/build/include/ftp/ftplib.h;F:/study/MyLib/build/include/ftp/stdafx.h;F:/study/MyLib/build/include/ftp/targetver.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "F:/study/MyLib/build/include/ftp" TYPE FILE FILES
    "F:/study/MyLib/modules/Net/FTPClient/./AutoLock.h"
    "F:/study/MyLib/modules/Net/FTPClient/./FTPClientImp.h"
    "F:/study/MyLib/modules/Net/FTPClient/./FtpxData.h"
    "F:/study/MyLib/modules/Net/FTPClient/./FtpxLibImp.h"
    "F:/study/MyLib/modules/Net/FTPClient/./GroupFile.h"
    "F:/study/MyLib/modules/Net/FTPClient/./ThrdControl.h"
    "F:/study/MyLib/modules/Net/FTPClient/./ThrdTransfer.h"
    "F:/study/MyLib/modules/Net/FTPClient/./ftplib.h"
    "F:/study/MyLib/modules/Net/FTPClient/./stdafx.h"
    "F:/study/MyLib/modules/Net/FTPClient/./targetver.h"
    )
endif()

