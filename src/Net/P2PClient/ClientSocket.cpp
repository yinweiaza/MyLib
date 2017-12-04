#include "StdAfx.h"
#include ".\clientsocket.h"
#include "P2PClientImp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CClientSocket::CClientSocket(void)
{
	m_pOwner = NULL;

}

CClientSocket::~CClientSocket(void)
{

}

void			CClientSocket::SetOwner(IClientSocketOwner* pOwner)
{
	m_pOwner = pOwner;
}

void			CClientSocket::OnReceivedMessage(void* lpBuf, int nBufLen)
{
	if (m_pOwner)
	{
		m_pOwner->OnSocketReceivedMessage(lpBuf, nBufLen);
	}
}

void			CClientSocket::OnConnect(int nErrorCode)
{
	ASSERT(m_pOwner!=NULL);
	if (m_pOwner)
	{
		m_pOwner->OnSocketConnect(nErrorCode);
	}
}

void			CClientSocket::OnClose(int nErrorCode)
{
	Close();
}

void			CClientSocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		CBaseSocket::Close();

		if (m_pOwner)
		{
			m_pOwner->OnSocketClose(0);
		}
	}
}