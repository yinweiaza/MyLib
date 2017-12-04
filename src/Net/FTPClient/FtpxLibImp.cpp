#include "StdAfx.h"
#include ".\ftpxlibimp.h"
#include "ThrdControl.h"
#include "ThrdTransfer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT WINAPI CreateFtpxLib(IFtpxLib** ppIFtpxLib)
{
	CFtpxLib* p = new CFtpxLib();

	*ppIFtpxLib = p;

	return NOERROR;
}

CFtpxLib::CFtpxLib(void)
{
	ZeroMemory(&m_param, sizeof(SFtpxParam));
	m_pNotify = NULL;
	m_pThrdControl = NULL;
	m_pThrdTransfer = NULL;
	m_pData = NULL;
	m_bInit = FALSE;

}

CFtpxLib::~CFtpxLib(void)
{
}

int CFtpxLib::BufSize(int nNetType)
{
	int nBufSize = 1024;
	switch (nNetType)
	{
	case 0:
		nBufSize = 1024;
		break;
	case 1:
		nBufSize = 2048;
		break;
	default:
		break;
	}
	return nBufSize;
}

HRESULT CFtpxLib::Init(SFtpxParam* lpParam, IFtpxNotify* lpNotify)
{
	m_param = *lpParam;
	m_pNotify = lpNotify;

	//开始控制命令线程
	m_pThrdControl = (CThrdControl*)AfxBeginThread(RUNTIME_CLASS(CThrdControl), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_pThrdControl->m_bAutoDelete = TRUE;
	m_pThrdControl->SetOwner(this);
	m_pThrdControl->ResumeThread();

	WaitForSingleObject(m_pThrdControl->m_hEventInit, INFINITE);

	return NOERROR;
}


HRESULT	CFtpxLib::Release()	
{
	if (m_pThrdControl != NULL)
	{
		m_pThrdControl->PostThreadMessage(WM_QUIT, 0, 0);
		WaitForSingleObject(m_pThrdControl->m_hThread, 1000);
	}

	POSITION pos = m_thrdTasks.GetStartPosition();
	while (pos != NULL)
	{
		CThrdTransfer* p = NULL;
		m_thrdTasks.GetNextAssoc(pos, p, p);
		p->UnInit(TRUE);
		//SafeDelete(p);
	}
	m_thrdTasks.RemoveAll();

	delete this;
	return NOERROR;
}

HRESULT CFtpxLib::Connect(LPCTSTR lpHost, UINT nPort, LPCTSTR lpUserName, LPCTSTR lpPassword)
{
	if (m_pThrdControl == NULL)
		return E_FAIL;

	return m_pThrdControl->Connect(lpHost, nPort, lpUserName, lpPassword);
}

HRESULT CFtpxLib::Disconnect()
{
	if (m_pThrdControl == NULL)
		return E_FAIL;

	return m_pThrdControl->Disconnect();
}

HRESULT CFtpxLib::ReConnect() 
{
	if (m_pThrdControl == NULL)
		return E_FAIL;

	return m_pThrdControl->ReConnect();
}

HRESULT CFtpxLib::Size(LPCTSTR lpPath, __int64& nSize)
{
	if (m_pThrdControl == NULL)
		return E_FAIL;

	return m_pThrdControl->Size(lpPath, nSize);
}

void			CFtpxLib::Fire_OnConnect(HRESULT hr)
{
	if (m_pNotify != NULL)
	{
		m_pNotify->OnConnect(hr, m_param.data);
	}
}

void			CFtpxLib::Fire_OnList(HRESULT hr, LPCTSTR lpPath, CRemoteItemArray& arrRemoteItem)
{
	CString str;
	for (int i=0; i<arrRemoteItem.GetCount(); i++)
	{
		SRemoteItem* p = arrRemoteItem[i];
		//SYSTEMTIME st;
		//FileTimeToSystemTime(&p->modify, &st);
		//str.Format(_T("%s,	%I64d,	%d,	%d-%d-%d,	%s\n"), p->name, p->size, p->type, st.wYear, st.wMonth, st.wDay, p->attri);
		//TRACE(str);
	}
	
	if (m_pNotify != NULL)
	{
		m_pNotify->OnList(hr, lpPath, arrRemoteItem, m_param.data);
	}
}

void			CFtpxLib::Fire_OnTransfer(HRESULT hr, STransTask* pTransTask)
{
	if (m_pNotify != NULL)
	{
		m_pNotify->OnTransfer(hr, pTransTask, m_param.data);
	}
}

void			CFtpxLib::Fire_OnLog(LPCTSTR lpText)
{
	//TRACE(lpText);
	if (m_pNotify != NULL)
	{
		m_pNotify->OnLog(lpText, m_param.data);
	}
}

HRESULT CFtpxLib::UploadBatch(const GUID& dwTaskId, CTransTaskArray& arrTransTasks) 
{
	CThrdTransfer* pThrdTransfer = new CThrdTransfer();
	pThrdTransfer->SetOwner(this);
	pThrdTransfer->Init();
	pThrdTransfer->SetTaskId(dwTaskId);
	m_thrdTasks.SetAt(pThrdTransfer, pThrdTransfer);

	for (int i=0; i<arrTransTasks.GetCount(); i++)
	{
		STransTask* p = arrTransTasks[i];
		pThrdTransfer->Upload(p->taskId, p->localFile, p->remoteUrl, p->finishSize, p->priority,p->bCoverSame);
		pThrdTransfer->Begin(p->taskId);
	}

	pThrdTransfer->ResumeTrans();

	return NOERROR;
}

HRESULT CFtpxLib::PauseBatch(const GUID& dwTaskId)
{
	POSITION pos = m_thrdTasks.GetStartPosition();
	while (pos != NULL)
	{
		CThrdTransfer* p = NULL;
		m_thrdTasks.GetNextAssoc(pos, p, p);
		if (p != NULL && IsEqualGUID(p->GetTaskId(), dwTaskId))
		{
			p->PauseTrans();
			break;
		}
	}

	return NOERROR;
}

HRESULT CFtpxLib::ContinueBatch(const GUID& dwTaskId)
{
	POSITION pos = m_thrdTasks.GetStartPosition();
	while (pos != NULL)
	{
		CThrdTransfer* p = NULL;
		m_thrdTasks.GetNextAssoc(pos, p, p);
		if (p != NULL && IsEqualGUID(p->GetTaskId(), dwTaskId))
		{
			p->ResumeTrans();
			break;
		}
	}

	return NOERROR;
}

HRESULT CFtpxLib::CancelBatch(const GUID& dwTaskId)
{
	POSITION pos = m_thrdTasks.GetStartPosition();
	while (pos != NULL)
	{
		CThrdTransfer* p = NULL;
		m_thrdTasks.GetNextAssoc(pos, p, p);
		if (p != NULL && IsEqualGUID(p->GetTaskId(), dwTaskId))
		{
			//线程正常运行
			p->ResumeTrans();

			//停止线程
			p->UnInit();
			m_thrdTasks.RemoveKey(p);
			//SafeDelete(p);
			break;
		}
	}

	return NOERROR;
}

HRESULT CFtpxLib::DownloadBatch(CTransTaskArray& arrTransTasks)
{
	CThrdTransfer* pThrdTransfer = new CThrdTransfer();
	pThrdTransfer->SetOwner(this);
	pThrdTransfer->Init();
	m_thrdTasks.SetAt(pThrdTransfer, pThrdTransfer);

	for (int i=0; i<arrTransTasks.GetCount(); i++)
	{
		STransTask* p = arrTransTasks[i];
		pThrdTransfer->Download(p->taskId, p->localFile, p->remoteUrl, p->finishSize, p->priority);
		pThrdTransfer->Begin(p->taskId);
	}

	pThrdTransfer->ResumeTrans();

	return NOERROR;
}

HRESULT CFtpxLib::GetConnectURL(CString& strURL)
{
	strURL.Format(_T("ftp://%s:%s@%s:%d"), 
		m_pThrdControl->m_strUserName, 
		m_pThrdControl->m_strPassword, 
		m_pThrdControl->m_strHost, 
		m_pThrdControl->m_nPort);

	return NOERROR;
}

HRESULT CFtpxLib::CreateDir(LPCTSTR lpPath)
{
	if (m_pThrdControl == NULL)
		return E_FAIL;

	return m_pThrdControl->CreateDir(lpPath);
}

HRESULT	CFtpxLib::ListSync(LPCTSTR lpPath, CRemoteItemArray& arrRemoteItem)
{
	if (m_pThrdControl == NULL)
		return E_FAIL;

	return m_pThrdControl->ListSync(lpPath, arrRemoteItem);
}

HRESULT	CFtpxLib::PutFile(LPCTSTR strLocalFile, LPCTSTR strServerFile)
{
	if (m_pThrdControl == NULL)
		return E_FAIL;

	return m_pThrdControl->PutFile(strLocalFile, strServerFile);
}

HRESULT	CFtpxLib::GetFile(LPCTSTR strLocalFile, LPCTSTR strServerFile)
{
	if (m_pThrdControl == NULL)
		return E_FAIL;

	return m_pThrdControl->GetFile(strLocalFile, strServerFile);
}

HRESULT	CFtpxLib::DeleteFile(LPCTSTR strServerFile)
{
	if (m_pThrdControl == NULL)
		return E_FAIL;

	return m_pThrdControl->DeleteFile(strServerFile);
}