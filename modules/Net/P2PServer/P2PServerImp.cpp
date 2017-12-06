#include "StdAfx.h"
#include ".\p2pserverimp.h"
// #include ".\P2PServerApp.h"
#include "ListenSocket.h"
#include "..\\include\\MessageDefine.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HRESULT WINAPI CreateP2PServer(IP2PServer** ppP2PServer, BOOL bDealInnerMsg)
{
	HRESULT hr = T9_NOERROR;

	CP2PServer* pP2PServer = new CP2PServer(bDealInnerMsg);

	*ppP2PServer = pP2PServer;

	return hr;
}


CP2PServer::CP2PServer(BOOL bDealInnerMsg)
{
	m_nPort			= 0;
	m_pThrdListen	= NULL;
	m_pNotify		= NULL;
	m_bDealInnerMsg	= bDealInnerMsg;
}

CP2PServer::~CP2PServer(void)
{
	DWORD dwConnection; 
	CThrdServer* pThrdServer;
	POSITION pos = m_connects.GetStartPosition();
	while (pos != NULL)
	{
		m_connects.GetNextAssoc(pos, dwConnection, pThrdServer);
		pThrdServer->PostThreadMessage(WM_QUIT, 0, 0);
		WaitForSingleObject(pThrdServer->m_hThread, 5000);
	}
	m_connects.RemoveAll();

	if(m_pThrdListen)
		m_pThrdListen->PostThreadMessage(WM_QUIT, 0, 0);
}

HRESULT CP2PServer::Fire_OnNotify(DWORD dwConnection, DWORD dwType, int nDataLen, void* lpData)
{
	HRESULT hr = NOERROR;
	if (m_pNotify)
	{
		hr = m_pNotify->OnP2PNotify(dwConnection, dwType, nDataLen, lpData);
	}
	return hr;
}

HRESULT CP2PServer::SetParam(SP2PServerParam* pParam)
{
	m_strFileDir = pParam->lpszFileDir;
	return T9_NOERROR;
}

HRESULT CP2PServer::SetNotify(IP2PServerNotify* pNotify)
{
	m_pNotify = pNotify;

	return T9_NOERROR;
}

HRESULT CP2PServer::Release()
{
	delete this;
	return NOERROR;
}

HRESULT	CP2PServer::Start(UINT dwPort)
{
	Stop();

	ASSERT(m_pThrdListen==NULL);

	m_nPort = dwPort;

	m_pThrdListen = (CThrdListen*)AfxBeginThread(RUNTIME_CLASS(CThrdListen), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_pThrdListen->m_bAutoDelete = TRUE;
	m_pThrdListen->SetOwner(this);
	m_pThrdListen->ResumeThread();

	return T9_NOERROR;
}

HRESULT CP2PServer::Stop()
{
	if (m_pThrdListen != NULL)
	{
		PostThreadMessage(m_pThrdListen->m_nThreadID, WM_QUIT, 0, 0);
		WaitForSingleObject(m_pThrdListen->m_hThread, INFINITE);

		m_pThrdListen = NULL; //线程已自动删除
	}

	return T9_NOERROR;
}

HRESULT CP2PServer::Close(DWORD dwConnection)
{
	CThrdServer* pThrdServer = GetThrdServer(dwConnection);
	if (pThrdServer != NULL)
	{
		pThrdServer->PostThreadMessage(WM_QUIT, 0, 0);
		m_connects.RemoveKey(dwConnection);
	}

	return NOERROR;
}

void			CP2PServer::OnSocketAccept(int nErrorCode)
{
	CListenSocket* pListenSocket = m_pThrdListen->GetListenSocket();
	CServerSocket  serverSocket;
	if (pListenSocket->Accept(serverSocket))
	{
		SThrdServerParam param;
		param.strFileDir		= m_strFileDir;
		param.pP2PServer		= this;
		param.bDealInnerMsg		= m_bDealInnerMsg;

		CThrdServer* pThrdServer = NULL;
		pThrdServer = (CThrdServer*)AfxBeginThread(RUNTIME_CLASS(CThrdServer), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		pThrdServer->m_bAutoDelete = TRUE;
		pThrdServer->SetParam(&param);
		pThrdServer->ResumeThread();

		pThrdServer->PostThreadMessage(WM_MESSAGE_NET_SERVER_THREAD_ATTACHSOCKET , serverSocket.Detach(), 0);
		
		DWORD dwConnection = (DWORD)pThrdServer;
		m_connects.SetAt(dwConnection, pThrdServer);

		Fire_OnNotify(dwConnection, snt_connected, 0, NULL);
	}
}

void	CP2PServer::OnSocketClose(DWORD dwConnection)
{
	//删除对应线程
	CThrdServer* pThrdServer = GetThrdServer(dwConnection);
	if (pThrdServer != NULL)
	{
		Fire_OnNotify(dwConnection, snt_disconnect, 0, NULL);

		pThrdServer->PostThreadMessage(WM_QUIT, 0, 0);
		m_connects.RemoveKey(dwConnection);
	}
}

HRESULT CP2PServer::SendData(DWORD dwConnection, int nDataLen, char* lpData, int nPriority)
{
	HRESULT hr = NOERROR;
	CThrdServer* pThrdServer = GetThrdServer(dwConnection);
	if (pThrdServer==NULL)
		return E_FAIL;

	pThrdServer->SendData(nDataLen, lpData, nPriority);
	return hr;
}

CThrdServer*	CP2PServer::GetThrdServer(DWORD dwConnection)
{
	CThrdServer* pThrdServer = NULL;
	if (m_connects.Lookup(dwConnection, pThrdServer))
		return pThrdServer;

	return NULL;
}