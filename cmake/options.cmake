#######################################################################
option(OPTION_BUILD_EXAMPLES "build example programs" ON)
#######################################################################

#######################################################################
option(OPTION_USE_FFMPEG_GUI "use ffmpeg  lib" OFF)
#######################################################################

#######################################################################
option(OPTION_USE_FLTK_GUI "use fltk as gui lib" ON)
#######################################################################

#######################################################################
option(OPTION_USE_SDL_GUI "use sdl as gui lib" OFF)
#######################################################################

#######################################################################
option(OPTION_USE_LMTC_LIB  "use lmtc lib or not"  ON)
#######################################################################

#######################################################################
option(OPTION_USE_ZIP  "use ZIP lib or not"  ON)
#######################################################################
if (OPTION_USE_LMTC_LIB)
	set(LMTC_INCLUDE_DIR "${MYLIB_SOURCE_DIR}/Include/Math" "${MYLIB_SOURCE_DIR}/Include")
	Message(STATUS "lmtc lib  used")
	include_directories(${LMTC_INCLUDE_DIR})
	add_subdirectory("${MYLIB_SOURCE_DIR}/modules/Math")
endif(OPTION_USE_LMTC_LIB)

if (OPTION_USE_SDL_GUI OR OPTION_USE_FLTK_GUI OR OPTION_USE_FFMPEG)
	#set(3RD_PARTY ${MYLIB_SOURCE_DIR}/3rdparty/include)
	#include_directories(3RD_PARTY)
	add_subdirectory(${MYLIB_SOURCE_DIR}/3rdparty)
endif(OPTION_USE_SDL_GUI OR OPTION_USE_FLTK_GUI OR OPTION_USE_FFMPEG)


if (OPTION_BUILD_EXAMPLES)
	Message(STATUS "BUILD EXAMPLES")
	add_subdirectory(${MYLIB_SOURCE_DIR}/App)
endif(OPTION_BUILD_EXAMPLES)

if(OPTION_USE_ZIP)
	Message(STATUS "BUILD ZIP lib!")
	add_subdirectory("${MYLIB_SOURCE_DIR}/modules/Zip")
endif(OPTION_USE_ZIP)
