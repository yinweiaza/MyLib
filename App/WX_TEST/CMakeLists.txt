set(SRCS  WxApp.cpp)

Set(HEADHS WxApp.h)
include_directories(./)

#wxWidgets setting
if(MSVC)
	set(WX_INCLUDE "C:\wxWidgets\include;C:\wxWidgets\include\msvc;C:\wxWidgets\lib\vc_lib\mswud")
	set(WX_LIB  "C:\wxWidgets\lib\vc_lib")
	include_directories(${WX_INCLUDE})
	link_directories(${WX_LIB})
endif(MSVC)

CREATE_EXAMPLE(WXTEST "${SRCS}"  "")

set_target_properties(WXTEST
	PROPERTIES
	LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS"
	RUNTIME_OUTPUT_DIRECTORY ${EXE_BIN_DIR}
	)
