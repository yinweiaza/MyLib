set(ZIP_INCLUDE 
	unzip.h
	zconf.h
	zip.h
	zlib.h
	)
set(ZIP_CPP
	unzip.cpp
	zip.cpp
	)

set(ZIP_SRC ${ZIP_INCLUDE} ${ZIP_CPP})
MYLIB_ADD_LIBRARY(zip STATIC ${ZIP_SRC})
