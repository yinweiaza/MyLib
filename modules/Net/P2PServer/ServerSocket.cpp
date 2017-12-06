#include "StdAfx.h"
#include ".\serversocket.h"
// #include "P2PServerApp.h"
#include "ThrdServer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CServerSocket::CServerSocket(void)
{
	m_pOwner = NULL;
}

CServerSocket::~CServerSocket(void)
{
}

void			CServerSocket::SetOwner(CThrdServer* pOwner)
{
	m_pOwner = pOwner;
}

void			CServerSocket::OnReceivedMessage(void* lpBuf, int nBufLen)
{
//	TRACE("\n CServerSocket::OnReceivedMessage:Len:%d", nBufLen);
	if (m_pOwner)
	{
		m_pOwner->OnSocketReceivedMessage(lpBuf, nBufLen);
	}
}

void			CServerSocket::OnClose(int nErrorCode)
{
	TRACE(_T("CServerSocket::OnClose\n"));
	Close();
}

void			CServerSocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		CBaseSocket::Close();

		if (m_pOwner)
		{
			m_pOwner->OnSocketClose();
		}
	}
}