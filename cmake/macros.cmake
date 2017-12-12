macro(MYLIB_ADD_LIBRARY LIBNAME LIBTYPE LIBFILES)
	# Lib Name Setting
	if ( ${LIBTYPE} STREQUAL "SHARED" )
		set(LIBRARY_NAME ${LIBNAME}_SHARED)
	else()
		set(LIBRARY_NAME ${LIBNAME})
	endif( ${LIBTYPE} STREQUAL "SHARED" )

	#set lib debug name
	if (MSVC)
		set(LIBRARY_NAME_DEBUG "${LIBRARY_NAME}d")
	else()
		set(LIBRARY_NAME_DEBUG "${LIBRARY_NAME}")
	endif(MSVC)

	add_library(${LIBRARY_NAME} ${LIBTYPE} ${LIBFILES})

	#set lib properties
	set_target_properties(${LIBRARY_NAME}
		PROPERTIES
		OUTPUT_NAME ${LIBRARY_NAME}
		DEBUG_OUTPUT_NAME ${LIBRARY_NAME_DEBUG}
		CLEAN_DIRECT_OUTPUT TRUE
		COMPLIER_DEFINITIONS "MYLIB_LIBRARY"
		)

	if (${LIBTYPE} STREQUAL "SHARED")
		set_target_properties(${LIBRARY_NAME}
			PROPERTIES
			VERSION ${MYLIB_VERSION_FULL}
			SOVERSION ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}
			PREFIX "lib"
			)
	endif(${LIBTYPE} STREQUAL "SHARED")

	if (MSVC)
		if(${LIBTYPE} STREQUAL "SHARED")
			set_target_properties(${LIBRARY_NAME}
				PROPERTIES
				COMPLIER_DEFINITIONS "MYLIB_DLL"
				)
		endif(${LIBTYPE} STREQUAL "SHARED")
	endif(MSVC)

	#install setting
	install(TARGETS ${LIBRARY_NAME}
		EXPORT MYLIB-Targets
		RUNTIME DESTINATION ${MYLIB_BINDIR}
		LIBRARY DESTINATION ${MYLIB_LIBDIR}
		ARCHIVE DESTINATION ${MYLIB_LIBDIR}
		}
endmacro(MYLIB_ADD_LIBRARY LIBNAME LIBTYPE LIBFILES)


function(CREATE_EXAMPLE NAME SOURCES lIBRARIES)
	set(flsrcs)
	set(srcs)
	set(tname ${NAME})

	foreach(src ${SOURCES)
		if("${src}" MATCHES  "\\.fl$")
			list(APPEND flsrcs ${src})
		else()
			list(APPEND srcs ${src})
		endif("${src}" MATCHES  "\\.fl$")
	endforeach(src ${SOURCES)

	endforeach(src ${SOURCES)

	add_executable(${tname} WIN32 ${srcs})

	set_target_properties(${tname}
		PROPERTIES OUTPUT_NAME ${NAME}
		)

	target_link_libraries(${tname} ${LIBRARIES})
endfunction(CREATE_EXAMPLE NAME SOURCES lIBRARIES)
