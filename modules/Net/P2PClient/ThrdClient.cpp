// ThrdClient.cpp : implementation file
//

#include "stdafx.h"
#include "ThrdClient.h"
#include "P2PClientImp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CThrdClient

IMPLEMENT_DYNCREATE(CThrdClient, CWinThread)

CThrdClient::CThrdClient()
{
	m_strIP		= _T("");
	m_dwPort	= 0;
	m_pP2PClient= NULL;
	m_pClientSocket = NULL;
}

CThrdClient::~CThrdClient()
{

}

BOOL CThrdClient::InitInstance()
{
	CWinThread::InitInstance();
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//初始化消息队列
	MSG msg;
	PeekMessage(&msg, NULL, WM_MESSAGE_NET_ASYNCSOCK_SEND, WM_MESSAGE_NET_ASYNCSOCK_SEND, PM_NOREMOVE);

	SocketThreadInit();

	PostThreadMessage(WM_MESSAGE_NET_CLIENT_THREAD_RETRYCONNECT , 0, 0);

	TRACE(_T("CThrdClient InitInstance:%d\n"), GetCurrentThreadId());
	return TRUE;
}

int CThrdClient::ExitInstance()
{
	DeleteAllData();

	SafeDelete(m_pClientSocket);

	TRACE(_T("CThrdClient ExitInstance:%d\n"), GetCurrentThreadId());

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThrdClient, CWinThread)
	ON_THREAD_MESSAGE(WM_MESSAGE_NET_CLIENT_THREAD_RETRYCONNECT , OnRetryConnect)
	ON_THREAD_MESSAGE(WM_MESSAGE_NET_CLIENT_SENDDATA_PUMP, OnSendDataPump)
END_MESSAGE_MAP()


// CThrdClient message handlers
void		CThrdClient::SetParam(SThrdClientParam* pParam)
{
	m_strIP		= pParam->strIP;
	m_dwPort	= pParam->dwPort;
	m_pP2PClient= pParam->pP2PClient;
}

void		CThrdClient::SendData(int nDataLen, char* lpData, int nPriority)
{
	SDataItem* pItem = new SDataItem;
	pItem->nDataLen = nDataLen;
	pItem->pData = (char*)new char[nDataLen];
	memcpy(pItem->pData, lpData, nDataLen);
	pItem->nPriority = nPriority;

	CSingleLock lock(&m_csData, TRUE);
	m_datas.AddTail(pItem);

	PostThreadMessage(WM_MESSAGE_NET_CLIENT_SENDDATA_PUMP, 0, 0);
}

void		CThrdClient::OnSocketConnect(int nErrorCode)
{
	CString str;
	if (nErrorCode != 0)
	{
		//连接错误	
		str.Format(_T("网络连接错误:%d"), nErrorCode);
	}
	else
	{
		str = _T("成功建立连接");

		m_pP2PClient->Fire_OnNotify((DWORD)this, cnt_connected, 0, NULL);
	}

	int nDataLen = (str.GetLength()+1)*sizeof(TCHAR);
	void* lpData = (void*)str.GetBuffer(0);
	m_pP2PClient->Fire_OnNotify((DWORD)this, cnt_state, nDataLen, lpData);
}

void	CThrdClient::OnSocketReceivedMessage(void* lpBuf, int nBufLen)
{
	if (!OnInnerNetMessage((DWORD)this, nBufLen, lpBuf))
	{
		m_pP2PClient->Fire_OnNotify((DWORD)this, cnt_data, nBufLen, lpBuf);
	}
}

void	CThrdClient::OnSocketClose(int nErrorCode)
{
	m_pP2PClient->Fire_OnNotify((DWORD)this, cnt_disconnect, 0, NULL);

	CString str = _T("网络连接断开");
	int nDataLen = (str.GetLength()+1)*sizeof(TCHAR);
	void* lpData = (void*)str.GetBuffer(0);
	m_pP2PClient->Fire_OnNotify((DWORD)this, cnt_state, nDataLen, lpData);
}

void	CThrdClient::OnRetryConnect(WPARAM wParam, LPARAM lParam)
{
	//新建Socket连接
	DeleteAllData();

	SafeDelete(m_pClientSocket);
	m_pP2PClient->Fire_OnNotify((DWORD)this, cnt_disconnect, 0, 0);

	m_pClientSocket = new CClientSocket();
	m_pClientSocket->Close();
	m_pClientSocket->SetOwner(this);
	m_pClientSocket->Create();
	m_pClientSocket->Connect(m_strIP, m_dwPort);
}


BOOL	CThrdClient::OnInnerNetMessage(DWORD dwConnection, int nDataLen, void* lpData)
{
	return FALSE;
}

void	CThrdClient::DeleteDataItem(SDataItem* pItem)
{
	if (pItem != NULL)
	{
		delete pItem->pData;
		delete pItem;
	}
}

void	CThrdClient::DeleteAllData()
{
	POSITION pos = m_datas.GetHeadPosition();
	while (pos != NULL)
	{
		SDataItem* pItem = m_datas.GetNext(pos);
		DeleteDataItem(pItem);
	}
	m_datas.RemoveAll();
}

void	CThrdClient::OnSendDataPump(WPARAM wParam, LPARAM lParam)
{
	if (IsSocketClosed())
		return;

	CSingleLock lock(&m_csData, TRUE);
	if (m_datas.GetCount() > 0)
	{
		SDataItem* pItem = m_datas.GetHead();
		if (pItem != NULL)
		{
			m_pClientSocket->SendMessage(pItem->pData, pItem->nDataLen);
			DeleteDataItem(pItem);
			m_datas.RemoveHead();
		}
	}

	if (m_datas.GetCount() > 0)
		PostThreadMessage(WM_MESSAGE_NET_CLIENT_SENDDATA_PUMP, 0, 0);
}

BOOL	CThrdClient::IsSocketClosed()
{
	if (m_pClientSocket == NULL || m_pClientSocket->m_hSocket == INVALID_SOCKET)
		return TRUE;

	return FALSE;
}