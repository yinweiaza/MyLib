// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ListenSocket.h"
#include "P2PServerImp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CListenSocket

CListenSocket::CListenSocket()
{
	m_pOwner = NULL;
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket member functions

void			CListenSocket::SetOwner(CThrdListen*	pOwner)
{
	m_pOwner = pOwner;
}

void			CListenSocket::OnAccept(int nErrorCode)
{
	if (m_pOwner != NULL)
	{
		m_pOwner->OnSocketAccept(nErrorCode);
	}
}
