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


#include <ATLComTime.h>
#include <afxsock.h>		// MFC socket extensions
#include <afxtempl.h>
#include <afxmt.h>
#include <afxinet.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include "..\..\Include\ErrorDefine.h"
#include "AutoLock.h"
#include "..\..\Include\CommonDefine.h"
#include "..\..\include\CommonFunction.h"

#define		NOSSL
#define		NOLFS
#include "ftplib.h"

CStringA UTF16toUTF8( const CStringW& szUTF16_ );


namespace FTPClient
{
extern void   SocketThreadInit(); 
};
using namespace FTPClient;
