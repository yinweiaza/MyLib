#include "StdAfx.h"
#include ".\ftpclientimp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//

CFTPClient::CFTPClient(void)
{
	m_pNotify = NULL;
}

CFTPClient::~CFTPClient(void)
{

	DWORD dwConnection; 
	IFtpxLib* pFtpxLib = NULL;
	POSITION pos = m_connects.GetStartPosition();
	while (pos != NULL)
	{
		m_connects.GetNextAssoc(pos, dwConnection, pFtpxLib);
		pFtpxLib->Release();
	}
	m_connects.RemoveAll();

	DeleteArray(m_groupFiles);
}


HRESULT CFTPClient::SetParam(SFTPClientParam* pParam)
{
	HRESULT hr = NOERROR;

	m_param = *pParam;

	return hr;
}

HRESULT CFTPClient::SetNotify(IFTPClientNotify* pNotify)
{
	HRESULT hr = NOERROR;
	m_pNotify = pNotify;
	return hr;
}

HRESULT CFTPClient::Release() 
{
	HRESULT hr = NOERROR;

	delete this;
	return hr;
}

HRESULT CFTPClient::AddConnect(CString strIP, DWORD dwPort, LPCTSTR strUserName, LPCTSTR strPassword, DWORD& dwConnection)
{
	HRESULT hr = NOERROR;

	IFtpxLib*	pFtpxLib = NULL;
	hr = CreateFtpxLib(&pFtpxLib);
	if (SUCCEEDED(hr))
	{
		dwConnection = (DWORD)pFtpxLib;

		m_connects.SetAt(dwConnection, pFtpxLib);

		SFtpxParam param;
		memset(&param, 0, sizeof(SFtpxParam));
		param.data = dwConnection;
		param.netCharCode = m_param.netCharCode;
		param.taskThreadCount = m_param.taskThreadCount;

		pFtpxLib->Init(&param, this);
		pFtpxLib->Connect(strIP, dwPort, strUserName, strPassword);		
	}

	return hr;
}

HRESULT CFTPClient::RetryConnect(DWORD dwConnection)
{
	HRESULT hr = NOERROR;

	IFtpxLib* pFtpxLib = GetFtpxLib(dwConnection);
	if (pFtpxLib != NULL)
	{
		return pFtpxLib->ReConnect();
	}

	return E_FAIL;
}

HRESULT CFTPClient::DeleteConnect(DWORD dwConnection)
{
	HRESULT hr = NOERROR;

	IFtpxLib* pFtpxLib = GetFtpxLib(dwConnection);
	if (pFtpxLib != NULL)
	{
		pFtpxLib->Release();
		m_connects.RemoveKey(dwConnection);
	}

	return hr;
}

CString UrlCombine2(const CString& strBase, const CString& strRelative)
{
	TCHAR szPath[MAX_PATH];
	DWORD dwLength = MAX_PATH;
	TCHAR szBase[MAX_PATH];
	TCHAR szRelative[MAX_PATH];
	_stprintf(szBase, _T("%s"), strBase);
	_stprintf(szRelative, _T("%s"), strRelative);
	for (int i=0; i<_tcslen(szRelative); i++)
	{
		if (szRelative[i]=='\\')
			szRelative[i]='/';
	}

	HRESULT hr = UrlCombine(szBase, szRelative, szPath, &dwLength, 0);
	if (SUCCEEDED(hr))
		return szPath;
	else
		return _T("");
}

HRESULT CFTPClient::UploadFileList(DWORD dwConnection, const GUID & dwTaskId, STranFileInfoArray &progFileInfo, CString strServerDir)
{
	HRESULT hr = NOERROR;

	if (progFileInfo.GetCount() <= 0)
		return hr;

	IFtpxLib* pFtpxLib = GetFtpxLib(dwConnection);
	if (pFtpxLib == NULL)
		return E_FAIL;
 
	CString strBaseUrl;
	pFtpxLib->GetConnectURL(strBaseUrl);

	CGroupFile* pGf = new CGroupFile();
	pGf->LoadFromFileList(progFileInfo, strServerDir);
	pGf->SetTaskId(dwTaskId);
	pGf->SetData((DWORD)pFtpxLib);
	m_groupFiles.Add(pGf);

	CTransTaskArray arrTransTasks;
	CFileTaskArray& fileTasks = pGf->GetFileTasks();
	for (int i=0; i<fileTasks.GetCount(); i++)
	{
		SFileTask* pCurrent = fileTasks[i];

		STransTask* pItem = new STransTask;
		pItem->taskType = TT_Upload;
		pItem->taskId = pCurrent->taskId;
		pItem->localFile = pCurrent->clientFile;
		pItem->remoteUrl = UrlCombine2(strBaseUrl, pCurrent->serverFile);
		pItem->state = TS_Wait;
		pItem->totalSize = pCurrent->totalSize;
		if (pCurrent->bResume)
		{
			__int64 size = 0;
			pFtpxLib->Size(pCurrent->serverFile, size);
			pGf->SetFinishSize(pCurrent, size);
		}
		pItem->finishSize = pCurrent->finishSize;
		pItem->priority = TP_Normal;
		pItem->errCount = 100000;
		pItem->strErrNote = _T("");
		pItem->bCoverSame = pCurrent->bCoverSame;
		arrTransTasks.Add(pItem);
	}

	pFtpxLib->UploadBatch(dwTaskId, arrTransTasks);

	DeleteArray(arrTransTasks);

	return hr;
}

HRESULT CFTPClient::DownloadFileList(DWORD dwConnection, const GUID & dwTaskId, STranFileInfoArray &progFileInfo, CString strServerDir) 
{
	HRESULT hr = NOERROR;

	if (progFileInfo.GetCount() <= 0)
		return hr;

	IFtpxLib* pFtpxLib = GetFtpxLib(dwConnection);
	if (pFtpxLib == NULL)
		return E_FAIL;

	CString strBaseUrl;
	pFtpxLib->GetConnectURL(strBaseUrl);

	CGroupFile* pGf = new CGroupFile();
	pGf->LoadFromFileList(progFileInfo, strServerDir);
	pGf->SetTaskId(dwTaskId);
	m_groupFiles.Add(pGf);

	CTransTaskArray arrTransTasks;
	CFileTaskArray& fileTasks = pGf->GetFileTasks();
	for (int i=0; i<fileTasks.GetCount(); i++)
	{
		SFileTask* pCurrent = fileTasks[i];

		STransTask* pItem = new STransTask;
		pItem->taskType = TT_Download;
		pItem->taskId = pCurrent->taskId;
		pItem->localFile = pCurrent->clientFile;
		pItem->remoteUrl = UrlCombine2(strBaseUrl, pCurrent->serverFile);
		pItem->state = TS_Wait;
		pItem->totalSize = pCurrent->totalSize;
		pItem->finishSize = pCurrent->finishSize;
		pItem->priority = TP_Normal;
		pItem->errCount = 100000;
		pItem->strErrNote = _T("");
		arrTransTasks.Add(pItem);
	}

	pFtpxLib->DownloadBatch(arrTransTasks);

	DeleteArray(arrTransTasks);

	return hr;
}

HRESULT CFTPClient::PauseTask(const GUID & dwTaskId)
{
	HRESULT hr = NOERROR;

	for (int i=0; i<m_groupFiles.GetCount(); i++)
	{
		CGroupFile* pGf = m_groupFiles[i];
		if (IsEqualGUID(pGf->GetTaskId(), dwTaskId))
		{
			IFtpxLib* pFtpxLib = GetFtpxLib(pGf->GetData());
			if (pFtpxLib != NULL)
			{
				hr = pFtpxLib->PauseBatch(dwTaskId);
				break;
			}
		}
	}

	return hr;
}

HRESULT CFTPClient::ContinueTask(const GUID & dwTaskId)
{
	HRESULT hr = NOERROR;

	for (int i=0; i<m_groupFiles.GetCount(); i++)
	{
		CGroupFile* pGf = m_groupFiles[i];
		if (IsEqualGUID(pGf->GetTaskId(), dwTaskId))
		{
			IFtpxLib* pFtpxLib = GetFtpxLib(pGf->GetData());
			if (pFtpxLib != NULL)
			{
				hr = pFtpxLib->ContinueBatch(dwTaskId);
				break;
			}
		}
	}

	return hr;
}

HRESULT CFTPClient::CancelTask(const GUID & dwTaskId)
{
	HRESULT hr = NOERROR;

	for (int i=0; i<m_groupFiles.GetCount(); i++)
	{
		CGroupFile* pGf = m_groupFiles[i];
		if (IsEqualGUID(pGf->GetTaskId(), dwTaskId))
		{
			IFtpxLib* pFtpxLib = GetFtpxLib(pGf->GetData());
			if (pFtpxLib != NULL)
			{
				pFtpxLib->CancelBatch(dwTaskId);
			}
			break;
		}
	}

	return hr;
}

/*
HRESULT CFTPClient::List(LPCTSTR strPath, DWORD dwConnection)
{
	IFtpxLib* pFtpxLib = GetFtpxLib(dwConnection);
	if (pFtpxLib==NULL)
		return E_FAIL;

	return pFtpxLib->List(strPath);
}
*/


HRESULT WINAPI CreateFTPClient(IFTPClient** ppFTPClient)
{
	HRESULT hr = NOERROR;

	CFTPClient* pFTPClient = new CFTPClient();

	*ppFTPClient = pFTPClient;

	return hr;
}

IFtpxLib*	CFTPClient::GetFtpxLib(DWORD dwConnection)
{
	IFtpxLib* pFtpxLib = NULL;
	if (m_connects.Lookup(dwConnection, pFtpxLib))
		return pFtpxLib;

	return NULL;
}

HRESULT CFTPClient::OnConnect(HRESULT hr, DWORD dwData)
{
	if (m_pNotify != NULL)
	{
		CString str;
		if (FAILED(hr))
		{
			str.Format(_T("FTP连接错误"));
		}
		else
		{
			str = _T("成功建立连接");

			m_pNotify->OnFTPNotify(dwData, cnt_connected, 0, NULL);
		}
		
		int nDataLen = (str.GetLength()+1)*sizeof(TCHAR);
		void* lpData = (void*)str.GetBuffer(0);
		m_pNotify->OnFTPNotify(dwData, cnt_state, nDataLen, lpData);
	}
	return hr;
}

HRESULT CFTPClient::OnList(HRESULT hr, LPCTSTR lpPath, CRemoteItemArray& arrRemoteItem, DWORD dwData)
{
	if (m_pNotify != NULL && SUCCEEDED(hr))
	{
		return m_pNotify->OnFTPNotify(dwData, cnt_list, sizeof(char*), (void*)&arrRemoteItem);
	}
	return NOERROR;
}

HRESULT CFTPClient::OnTransfer(HRESULT hr, STransTask* pTransTask, DWORD dwData)
{
	if (m_pNotify != NULL && SUCCEEDED(hr))
	{
		IFtpxLib* pFtpxLib = GetFtpxLib(dwData);
		if (pFtpxLib == NULL)
			return E_FAIL;

		SFileTask* pFT = NULL;
		CGroupFile* pGF = FindGroupFile(pTransTask->taskId, &pFT);
		if (pGF != NULL)
		{
			pGF->SetFinishSize(pFT, pTransTask->finishSize);

			BOOL bFinished = FALSE;
			if (pTransTask->state>=TS_Finish && pGF->GetFinishSize()>=pGF->GetTotalSize())
			{
				bFinished = TRUE;
			}

			SCNTask task;
			task.dwTaskId = pGF->GetTaskId();
			task.bFinished = bFinished;
			task.fProgress = (pGF->GetFinishSize()*1.0/pGF->GetTotalSize());

			return m_pNotify->OnFTPNotify(dwData, cnt_task, sizeof(task), (void*)&task);	
		}
	}

	return NOERROR;
}

HRESULT CFTPClient::OnLog(LPCTSTR lpText, DWORD dwData)
{
	HRESULT hr = NOERROR;
	return hr;
}

CGroupFile*	CFTPClient::FindGroupFile(const CString& strFileId, SFileTask** pFileTask)
{
	CGroupFile* pGF = NULL;
	SFileTask* pFT = NULL;
	for (int i=0; i<m_groupFiles.GetCount(); i++)
	{
		pGF = m_groupFiles[i];
		if (pGF != NULL)
		{
			pFT = pGF->Find(strFileId);
			if (pFT != NULL)
			{
				*pFileTask = pFT;
				return pGF;
			}
		}
	}

	return NULL;
}


HRESULT CFTPClient::ListSync(DWORD dwConnection, LPCTSTR strPath, CRemoteItemArray& arrRemoteItem)
{
	IFtpxLib* pFtpxLib = GetFtpxLib(dwConnection);
	if (pFtpxLib==NULL)
		return E_FAIL;

	return pFtpxLib->ListSync(strPath, arrRemoteItem);
}

HRESULT CFTPClient::PutFile(DWORD dwConnection, LPCTSTR	strLocalFile, LPCTSTR strServerFile)
{
	IFtpxLib* pFtpxLib = GetFtpxLib(dwConnection);
	if (pFtpxLib==NULL)
		return E_FAIL;

	return pFtpxLib->PutFile(strLocalFile, strServerFile);
}

HRESULT CFTPClient::GetFile(DWORD dwConnection, LPCTSTR	strLocalFile, LPCTSTR strServerFile)
{
	IFtpxLib* pFtpxLib = GetFtpxLib(dwConnection);
	if (pFtpxLib==NULL)
		return E_FAIL;

	return pFtpxLib->GetFile(strLocalFile, strServerFile);
}

typedef CList<int, int> CNumberList;

void	AddToNumberList(CNumberList& numberList, int value)
{
	//从大到小排序
	BOOL bInserted = FALSE;
	POSITION pos = numberList.GetHeadPosition();
	while(pos != NULL)
	{
		POSITION oldPos = pos;
		int num = numberList.GetNext(pos);
		if (value >= num)
		{
			numberList.InsertBefore(oldPos, value);
			bInserted = TRUE;
			break;
		}
	}

	if (!bInserted)
	{
		numberList.AddTail(value);
	}
}

HRESULT CFTPClient::SendCommand(DWORD dwConnection, LPCTSTR	strCmdFile, LPCTSTR strServerDir)
{
	IFtpxLib* pFtpxLib = GetFtpxLib(dwConnection);
	if (pFtpxLib==NULL)
		return E_FAIL;

	HRESULT hr = NOERROR;
 
	CString strRemoteDir = strServerDir;
	if (strRemoteDir.GetAt(strRemoteDir.GetLength()-1) != _T('/'))
	{ 
		strRemoteDir += _T("/");
	}

	hr = pFtpxLib->CreateDir(strRemoteDir);
	FAILED_RETURN_hr;

	CRemoteItemArray arrRemoteItem;
	hr = pFtpxLib->ListSync(strRemoteDir, arrRemoteItem);
	FAILED_RETURN_hr;
 
	//查找目录中最大命令文件名
	CNumberList	numberList;
	int fileNumber;
	for (int i=0; i<arrRemoteItem.GetCount(); i++)
	{
		SRemoteItem* p = arrRemoteItem[i];
		if (p->type==Ftpx_File && GetFileExt(p->name).CompareNoCase(_T("cmd"))==0)
		{
			//取前缀
			CString strNumber = GetFileTitle(p->name);
			fileNumber = _ttoi(strNumber);

			//插入队列
			if (fileNumber > 0)
			{
				AddToNumberList(numberList, fileNumber);
			}
		}
	}
	DeleteArray(arrRemoteItem);

	CString strServerFile = _T("");

	int newNumber = 0;
	if (numberList.GetCount() < 1)
		newNumber = 1;
	else
		newNumber = numberList.GetHead()+1;

	strServerFile.Format(_T("%s%d.cmd"), strRemoteDir, newNumber);
	hr = pFtpxLib->PutFile(strCmdFile, strServerFile);

	//删除超过32个的命令文件
	while (numberList.GetCount()>31)
	{
		int number = numberList.RemoveTail();

		strServerFile.Format(_T("%s%d.cmd"), strRemoteDir, number);
		pFtpxLib->DeleteFile(strServerFile);
	}
	return hr;
}