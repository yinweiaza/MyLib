Project(MYLIB)
CMAKE_MINIMUM_REQUIRED(VERSION 2.5)

#make sure out-of-tree compile
if( " ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "
	FATAL: In Source builds are not allowed,
	you should create separate directory for build files.
	")

#functions  macros
include(cmake/FLTK-Functions.cmake)
include(cmake/macros.cmake)

#base setup
include(cmake/setup.cmake)

# include header
include(cmake/resources.cmake)

#options
include(cmake/options.cmake)

#build the test examples or not?
if(OPTION_BUILD_EXAMPLES)
	add_subdirectory(App)
endif(OPTION_BUILD_EXAMPLES)

#variables shared by export and install
include(cmake/variables.cmake)

#final config and export
include(cmake/export.cmake)

#install
include(cmake/install.cmake)

