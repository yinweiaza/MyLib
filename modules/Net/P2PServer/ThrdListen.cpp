// ThrdServer.cpp : implementation file
//

#include "stdafx.h"
#include "P2PServerImp.h"
#include "ThrdServer.h"
#include "ListenSocket.h"
#include "..\\include\\\MessageDefine.h"
// #include "P2PServerApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CThrdListen

IMPLEMENT_DYNCREATE(CThrdListen, CWinThread)

CThrdListen::CThrdListen()
{
	m_pOwner = NULL;
	m_pListenSocket = NULL;

}

CThrdListen::~CThrdListen()
{
	SafeDelete(m_pListenSocket);
}


BOOL CThrdListen::InitInstance()
{
	CWinThread::InitInstance();
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	
	//初始化消息队列
	MSG msg;
	PeekMessage(&msg, NULL, WM_MESSAGE_NET_ASYNCSOCK_SEND, WM_MESSAGE_NET_ASYNCSOCK_SEND, PM_NOREMOVE);
	
	SocketThreadInit();

	if (m_pListenSocket == NULL)
	{
		m_pListenSocket = new CListenSocket();
		m_pListenSocket->SetOwner(this);
		m_pListenSocket->Create(m_pOwner->m_nPort);

		m_pListenSocket->Listen();
	}


	return TRUE;
}

int CThrdListen::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThrdListen, CWinThread)

END_MESSAGE_MAP()


// CThrdListen message handlers
void			CThrdListen::SetOwner(CP2PServer* pOwner)
{
	m_pOwner = pOwner;
}

void			CThrdListen::OnSocketAccept(int nErrorCode)
{
	if (m_pOwner)
	{
		m_pOwner->OnSocketAccept(nErrorCode);
	}
}
