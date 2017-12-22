Set(CPP test.cpp)
set(fltk_lib)
foreach(LIB_PATH ${ALL_FLTK_LIB})
	string(REGEX REPLACE ".+/(.+)\\..*" "\\1"  FILE_NAME ${LIB_PATH})	
	find_library(LIB_DIR ${FILE_NAME} ${CMAKE_LIBRARY_PATH})	
	list(APPEND fltk_lib ${LIB_DIR}/${FILE_NAME})
endforeach(LIB_PATH ${ALL_FLTK_LIB})

CREATE_EXAMPLE(FLTK_TEST "${CPP}" "${fltk_lib}")
add_dependencies(FLTK_TEST LMTC)
set_target_properties(FLTK_TEST
	PROPERTIES
	LINK_FLAGS_DEBUG  "/SUBSYSTEM:WINDOWS"
	RUNTIME_OUTPUT_DIRECTORY   ${EXE_BIN_DIR}
	)
