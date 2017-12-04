// stdafx.cpp : source file that includes just the standard includes
// P2PClient.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


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
