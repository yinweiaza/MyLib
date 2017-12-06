// stdafx.cpp : source file that includes just the standard includes
// FTPClient.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file


CStringA UTF16toUTF8(const CStringW& szUTF16_)
{
	LPSTR pszUtf8 = NULL;
	CStringA szUTF8("");

	if (szUTF16_.IsEmpty()) 
		return szUTF8; //empty input string

	size_t nLen16 = szUTF16_.GetLength();
	size_t nLen8 = 0;
	size_t nBufLen = 0;

	if ((nLen8 = WideCharToMultiByte (CP_UTF8, 0, szUTF16_, nLen16, NULL, 0, 0, 0) ) == 0)
		return szUTF8; //conversion error!

	nBufLen = nLen8 + 1;
	pszUtf8 = new char [nBufLen];
	if (pszUtf8)
	{
		memset (pszUtf8, 0x00, nBufLen);
		WideCharToMultiByte(CP_UTF8, 0, szUTF16_, nLen16, pszUtf8, nLen8, 0, 0);
		szUTF8 = CStringA(pszUtf8);
	}

	delete [] pszUtf8;
	return szUTF8; //utf8 encoded string
}

namespace FTPClient
{
void   SocketThreadInit()   
{   
#ifndef   _AFXDLL   
#define   _AFX_SOCK_THREAD_STATE   AFX_MODULE_THREAD_STATE   
#define   _afxSockThreadState   AfxGetModuleThreadState()   

	_AFX_SOCK_THREAD_STATE*   pState   =   _afxSockThreadState;   
	if   (pState->m_pmapSocketHandle   ==   NULL)   
		pState->m_pmapSocketHandle   =   new   CMapPtrToPtr;   
	if   (pState->m_pmapDeadSockets   ==   NULL)   
		pState->m_pmapDeadSockets   =   new   CMapPtrToPtr;   
	if   (pState->m_plistSocketNotifications   ==   NULL)   
		pState->m_plistSocketNotifications   =   new   CPtrList;   

#endif   
}
};
