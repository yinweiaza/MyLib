// ThrdBase.cpp : implementation file
//

#include "stdafx.h"
#include "ThrdBase.h"
#include "ClientSocket.h"
#include "..\include\MessageDefine.h"

// CThrdBase

IMPLEMENT_DYNCREATE(CThrdBase, CWinThread)

CThrdBase::CThrdBase()
{
	m_bConnected= FALSE;
}

CThrdBase::~CThrdBase()
{
	POSITION pos = m_datas.GetHeadPosition();
	while (pos != NULL)
	{
		SDataItem* pItem = m_datas.GetNext(pos);
		DeleteDataItem(pItem);
	}
	m_datas.RemoveAll();
}


BOOL CThrdBase::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	CoInitialize(NULL);

	//初始化消息队列
	MSG msg;
	PeekMessage(&msg, NULL, WM_MESSAGE_NET_ASYNCSOCK_SEND, WM_MESSAGE_NET_ASYNCSOCK_SEND, PM_NOREMOVE);

	SocketThreadInit();

	//创建Socket
	m_pClientSocket = new CClientSocket();

	return TRUE;
}

int CThrdBase::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	SafeDelete(m_pClientSocket);

	CoUninitialize();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThrdBase, CWinThread)
	ON_THREAD_MESSAGE(WM_SendData_Pump, OnSendDataPump)
END_MESSAGE_MAP()


// CThrdBase message handlers


void	CThrdBase::DeleteDataItem(SDataItem* pItem)
{
	if (pItem != NULL)
	{
		delete pItem->pData;
		delete pItem;
	}
}

void	CThrdBase::OnSendDataPump(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = NOERROR;

	CSingleLock lock(&m_csData, TRUE);
	if (m_bConnected && m_datas.GetCount() > 0)
	{
		SDataItem* pItem = m_datas.GetHead();
		if (pItem != NULL)
		{
			hr = m_pClientSocket->SendMessage(pItem->pData, pItem->nDataLen);
			if (SUCCEEDED(hr))
			{
				TRACE(_T("Send: %d\n"), pItem->nDataLen);
				DeleteDataItem(pItem);
				m_datas.RemoveHead();
			}
		}
	}

	if (m_datas.GetCount() > 0)
		PostThreadMessage(WM_SendData_Pump, 0, 0);
}


void	CThrdBase::OnSocketConnect(int nErrorCode) 
{ 
	if (nErrorCode==0)
	{		
		m_bConnected = TRUE; 
	}
}

void	CThrdBase::OnSocketReceivedMessage(void* lpBuf, int nBufLen)
{
	TRACE(_T("Recv: %d\n"), nBufLen);
}

void	CThrdBase::OnSocketClose(int nErrorCode)
{
	m_bConnected = FALSE;
}