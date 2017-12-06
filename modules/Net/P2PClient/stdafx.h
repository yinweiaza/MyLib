// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components



// TODO: reference additional headers your program requires here

#include <afxsock.h>		// MFC socket extensions
#include <afxtempl.h>
#include <afxmt.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <map>
#include <list>
#include <vector>
#include <set>

#include "..\Include\P2PClientLib.h"

#include "..\..\Include\CommonDefine.h"

#include "..\..\Include\T9CommonDefine.h"
#include "..\..\Include\CommonFunction.h"

#include "..\..\Player\Include\IProjectXmlDefine.h"
#include "..\..\Player\Include\IProjectInterface.h"

#include "..\..\Include\T9ImportDefine.h"

#include "..\..\Include\T9ExportDefine.h"

#include "..\Include\CLP2PMsgDefine.h"


extern void   SocketThreadInit(); 
