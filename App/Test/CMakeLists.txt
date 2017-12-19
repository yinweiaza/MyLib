set(CPP main.cpp)
find_library(lmtc_LIB LMTC ${LIBRARY_DIRS})

if( NOT lmtc_LIB )
	Message("lmtc  lib  not found!!")	
endif( NOT lmtc_LIB )

CREATE_EXAMPLE(MYTEST ${CPP}  LMTC)
