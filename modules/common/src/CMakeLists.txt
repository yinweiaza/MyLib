
set(LOGCPP 
	LogReader.cpp
	LogWriter.cpp
	)

set(bufCPP
	Buffer.cpp
	MassiveBuf.cpp
	NetBuffer.cpp
	)

set(ComCPP
	StdioFileEx.cpp
	UserThread.cpp
	BacCrypt.cpp
	ThrdRender.cpp
	)

set(XMLCPP
	XMLHelper.cpp
	)

list(APPEND ALLCPP "${LOGCPP};${bufCPP};${ComCPP};${XMLCPP}")

MYLIB_ADD_LIBRARY(COMMON STATIC  ${ALLCPP})
