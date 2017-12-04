#include "StdAfx.h"
#include ".\p2pclientimp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//

CP2PClient::CP2PClient(void)
{
	m_pNotify = NULL;
}

CP2PClient::~CP2PClient(void)
{
	DWORD dwConnection; 
	CThrdClient* pThrdClient;
	POSITION pos = m_connects.GetStartPosition();
	while (pos != NULL)
	{
		m_connects.GetNextAssoc(pos, dwConnection, pThrdClient);
		pThrdClient->PostThreadMessage(WM_QUIT, 0, 0);
	}
	m_connects.RemoveAll();

	CThrdTask* pThrdTask;
	pos = m_connectTasks.GetStartPosition();
	while (pos != NULL)
	{
		m_connectTasks.GetNextAssoc(pos, pThrdTask, pThrdClient);
		TRACE(_T("pThrdTask->PostThreadMessage\n"));
		pThrdTask->PostThreadMessage(WM_QUIT, 0, 0);
		WaitForSingleObject(pThrdTask->m_hThread, INFINITE);
	}
	m_connectTasks.RemoveAll();
}

HRESULT CP2PClient::Fire_OnNotify(DWORD dwConnection, DWORD dwType, int nDataLen, void* lpData)
{
	HRESULT hr = NOERROR;
	if (m_pNotify)
	{
		hr = m_pNotify->OnP2PNotify(dwConnection, dwType, nDataLen, lpData);
	}
	return hr;
}

HRESULT CP2PClient::SetParam(SP2PClientParam* pParam)
{
	HRESULT hr = NOERROR;
	return hr;
}

HRESULT CP2PClient::SetNotify(IP2PClientNotify* pNotify)
{
	HRESULT hr = NOERROR;
	m_pNotify = pNotify;
	return hr;
}

HRESULT CP2PClient::Release() 
{
	HRESULT hr = NOERROR;

	delete this;
	return hr;
}


HRESULT CP2PClient::AddConnect(CString strIP, DWORD dwPort, DWORD& dwConnection)
{
	HRESULT hr = NOERROR;

	//创建线程
	CThrdClient* pThrdClient = NULL;
	SThrdClientParam param;
	param.strIP = strIP;
	param.dwPort = dwPort;
	param.pP2PClient = this;

	pThrdClient = (CThrdClient*)AfxBeginThread(RUNTIME_CLASS(CThrdClient), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pThrdClient->m_bAutoDelete = TRUE;
	pThrdClient->SetParam(&param);
	pThrdClient->ResumeThread();
	//加入列表
	dwConnection = (DWORD)pThrdClient;
	m_connects.SetAt(dwConnection, pThrdClient);
	
	return hr;
}

HRESULT CP2PClient::RetryConnect(DWORD dwConnection)
{
	HRESULT hr = NOERROR;
	CThrdClient* pThrdClient = GetThrdClient(dwConnection);
	if (pThrdClient==NULL)
		return E_FAIL;
	pThrdClient->PostThreadMessage(WM_MESSAGE_NET_CLIENT_THREAD_RETRYCONNECT , 0, 0);
	return hr;
}

HRESULT CP2PClient::DeleteConnect(DWORD dwConnection)
{
	HRESULT hr = NOERROR;

	CThrdClient* pThrdClient;
	CThrdTask* pThrdTask;

	CArray<CThrdTask*, CThrdTask*>thrdTasks;

	POSITION pos = m_connectTasks.GetStartPosition();
	while (pos != NULL)
	{
		m_connectTasks.GetNextAssoc(pos, pThrdTask, pThrdClient);
		if ((DWORD)pThrdClient ==dwConnection)
		{
			thrdTasks.Add(pThrdTask);
			pThrdTask->PostThreadMessage(WM_QUIT, 0, 0);
		}
	}

	for (int i=0; i<thrdTasks.GetCount(); i++)
	{
		m_connectTasks.RemoveKey(thrdTasks[i]);
	}

	pThrdClient = GetThrdClient(dwConnection);
	if (pThrdClient != NULL)
	{
		pThrdClient->PostThreadMessage(WM_QUIT, 0, 0);
		m_connects.RemoveKey(dwConnection);
	}

	return hr;
}

HRESULT CP2PClient::ModifyConnect(DWORD dwConnection, CString strIP, DWORD dwPort)
{
	HRESULT hr = NOERROR;
	return hr;
}

HRESULT CP2PClient::SendData(DWORD dwConnection, int nDataLen, char* lpData, int nPriority)
{
	HRESULT hr = NOERROR;
	CThrdClient* pThrdClient = GetThrdClient(dwConnection);
	if (pThrdClient==NULL)
		return E_FAIL;
	
	pThrdClient->SendData(nDataLen, lpData, nPriority);
	
	return hr;
}

HRESULT CP2PClient::SendFileList(DWORD dwConnection, const GUID & dwTaskId, CStringList& strFiles, CString strServerDir)
{
	HRESULT hr = NOERROR;

	CThrdClient* pThrdClient = GetThrdClient(dwConnection);
	if (pThrdClient==NULL)
		return E_FAIL;

	//创建线程
	CThrdTask* pThrdTask = NULL;

	SThrdTaskParam param;
	param.strIP = pThrdClient->GetIP();
	param.dwPort = pThrdClient->GetPort();
	param.dwTask = dwTaskId;
	param.pP2PClient = this;
	param.pThrdClient = pThrdClient;

	pThrdTask = (CThrdTask*)AfxBeginThread(RUNTIME_CLASS(CThrdTask), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pThrdTask->m_bAutoDelete = TRUE;
	pThrdTask->SetParam(&param);
	pThrdTask->InitFromFileList(dwTaskId, strFiles, strServerDir);
	pThrdTask->ResumeThread();

	m_connectTasks.SetAt(pThrdTask, pThrdClient);

	return hr;
}

HRESULT CP2PClient::SendDir(DWORD dwConnection, const GUID & dwTaskId, CString strDir, CString strServerDir)
{
	HRESULT hr = NOERROR;

	CThrdClient* pThrdClient = GetThrdClient(dwConnection);
	if (pThrdClient==NULL)
		return E_FAIL;

	//创建线程
	CThrdTask* pThrdTask = NULL;

	SThrdTaskParam param;
	param.strIP = pThrdClient->GetIP();
	param.dwPort = pThrdClient->GetPort();
	param.dwTask = dwTaskId;
	param.pP2PClient = this;
	param.pThrdClient = pThrdClient;

	pThrdTask = (CThrdTask*)AfxBeginThread(RUNTIME_CLASS(CThrdTask), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pThrdTask->m_bAutoDelete = TRUE;
	pThrdTask->SetParam(&param);
	pThrdTask->InitFromDir(dwTaskId, strDir, strServerDir);
	pThrdTask->ResumeThread();

	m_connectTasks.SetAt(pThrdTask, pThrdClient);

	return hr;
}

HRESULT CP2PClient::SendProgram(DWORD dwConnection, const GUID & dwTaskId, STranFileInfoArray &progFileInfo, CString strServerDir)
{
	HRESULT hr = NOERROR;

	CThrdClient* pThrdClient = GetThrdClient(dwConnection);
	if (pThrdClient==NULL)
		return E_FAIL;

	//创建线程
	CThrdTask* pThrdTask = NULL;

	SThrdTaskParam param;
	param.strIP = pThrdClient->GetIP();
	param.dwPort = pThrdClient->GetPort();
	param.dwTask = dwTaskId;
	param.pP2PClient = this;
	param.pThrdClient = pThrdClient;

	pThrdTask = (CThrdTask*)AfxBeginThread(RUNTIME_CLASS(CThrdTask), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	pThrdTask->m_bAutoDelete = TRUE;
	pThrdTask->SetParam(&param);
	pThrdTask->InitFromProgram(dwTaskId, progFileInfo, strServerDir);
	pThrdTask->ResumeThread();

	m_connectTasks.SetAt(pThrdTask, pThrdClient);

	return hr;
}

HRESULT CP2PClient::PauseTask(const GUID & dwTaskId)
{
	HRESULT hr = NOERROR;

	CThrdTask* pThrdTask = GetThrdTask(dwTaskId);
	if (pThrdTask != NULL)
	{
		pThrdTask->Pause();
	}

	return hr;
}

HRESULT CP2PClient::ContinueTask(const GUID & dwTaskId)
{
	HRESULT hr = NOERROR;

	CThrdTask* pThrdTask = GetThrdTask(dwTaskId);
	if (pThrdTask != NULL)
	{
		pThrdTask->Continue();
	}

	return hr;
}

HRESULT CP2PClient::CancelTask(const GUID & dwTaskId)
{
	HRESULT hr = NOERROR;

	CThrdTask* pThrdTask = GetThrdTask(dwTaskId);
	if (pThrdTask != NULL)
	{
		pThrdTask->Cancel();
	}

	return hr;
}


HRESULT WINAPI CreateP2PClient(IP2PClient** ppP2PClient)
{
	HRESULT hr = NOERROR;

	CP2PClient* pP2PClient = new CP2PClient();

	*ppP2PClient = pP2PClient;

	return hr;
}

CThrdClient*	CP2PClient::GetThrdClient(DWORD dwConnection)
{
	CThrdClient* pThrdClient = NULL;
	if (m_connects.Lookup(dwConnection, pThrdClient))
		return pThrdClient;

	return NULL;
}

CThrdTask*		CP2PClient::GetThrdTask(const GUID& dwTaskId)
{
	CThrdClient* pThrdClient = NULL;
	CThrdTask* pThrdTask = NULL;
	POSITION pos = m_connectTasks.GetStartPosition();
	while (pos != NULL)
	{
		m_connectTasks.GetNextAssoc(pos, pThrdTask, pThrdClient);
		if (pThrdTask != NULL)
		{
			if (IsEqualGUID(dwTaskId, pThrdTask->GetTaskId()))
			{
				return pThrdTask;
			}
		}
	}

	return NULL;
}

void			CP2PClient::OnCloseTask(GUID& dwTaskId)
{
	//删除对应线程
	CThrdTask* pThrdTask = GetThrdTask(dwTaskId);
	if (pThrdTask != NULL)
	{
		pThrdTask->PostThreadMessage(WM_QUIT, 0, 0);

		m_connectTasks.RemoveKey(pThrdTask);
	}
}