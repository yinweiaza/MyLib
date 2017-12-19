#===========================================================
#				FFMPEG
#===========================================================*/
set(3RD_DIR_ROOT	"${MYLIB_SOURCE_DIR}/3rdparty")
if(OPTION_USE_FFMPEG)
	Message(STATUS "FFMPEG used")
	#首先将头文件copy过去;
	INSTALL_FILES_FUN("${3RD_DIR_ROOT}/include/ffmpeg"   ".h"   "${MYLIB_BINARY_DIR}/include")
	#再lib
	INSTALL_FILES_FUN("${3RD_DIR_ROOT}/lib/ffmpeg"     "lib"   "${MYLIB_BINARY_DIR}/lib")
	#最后bin
	INSTALL_FILES_FUN("${3RD_DIR_ROOT}/bin/ffmpeg"	 "dll"   "${MYLIB_BINARY_DIR}/bin")
endif(OPTION_USE_FFMPEG)

#===========================================================
#				SDL
#===========================================================*/
if(OPTION_USE_SDL_GUI)
	Message(STATUS "SDL used")
	INSTALL_FILES_FUN("${3RD_DIR_ROOT}/include/sdl"   ".h"	"${MYLIB_BINARY_DIR}/include")	
	INSTALL_FILES_FUN("${3RD_DIR_ROOT}/lib/sdl"   "lib"	"${MYLIB_BINARY_DIR}/lib")	
	INSTALL_FILES_FUN("${3RD_DIR_ROOT}/bin/sdl"	"dll"	"${MYLIB_BINARY_DIR}/bin")
endif(OOPTION_USE_SDL_GUI)

#===========================================================
#				FLTK
#===========================================================*/
if(OPTION_USE_FLTK_GUI)
	Message(STATUS "FLTK used")
	INSTALL_FILES_FUN("${3RD_DIR_ROOT}/include/fltk"	".h"	"${MYLIB_BINARY_DIR}/include")
	INSTALL_FILES_FUN("${3RD_DIR_ROOT}/lib/fltk"		"lib"	"${MYLIB_BINARY_DIR}/lib")
	INSTALL_FILES_FUN("${3RD_DIR_ROOT}/bin/fltk"		"dll"	"${MYLIB_BINARY_DIR}/bin")
endif(OPTION_USE_FLTK_GUI)



