#include "stdafx.h"
#include "HttpCient.h"
#include "ThrdHttpComm.h"
#include "ThrdHttpTransfer.h"

HRESULT WINAPI CreateHTTPClient(IHttpClient** ppHTTPClient)
{
	CHttpClient* pClient = new CHttpClient();

	*ppHTTPClient = pClient;
	pClient->Begin();

	return NOERROR;

}

// HttpClient.cpp
CHttpClient::CHttpClient()
{
	m_pNotify =NULL;

	int t1 =GetTickCount();
	/*使用http和https需要初始化sock库、ssl相关库，但是调用curl_global_init(CURL_GLOBAL_ALL)耗时较长，需要3秒左右，
	这里先手动加载和初始化sock库、ssl库,节约时间,
	由于sock、ssl都在在主线程先初始化*/
	CURLcode res = curl_global_init(CURL_GLOBAL_NOTHING);
	int t2 =GetTickCount();
	TRACE("\nSSL初始化时间:%d\n",t2-t1);
}

CHttpClient::~CHttpClient()
{
	curl_global_cleanup();
}

HRESULT CHttpClient::SetNotify(IHttpClientNotify* pNotify)
{
	m_pNotify = pNotify;

	return NO_ERROR;
}

HRESULT CHttpClient::Release() 
{
	CSingleLock sLock(&m_mutexThrd, TRUE);

	POSITION pos = m_httpCommList.GetHeadPosition();
	while(pos)
	{
		CThrdHttpComm *pThrd = m_httpCommList.GetNext(pos);
		ASSERT(pThrd);
		
		pThrd->End();
	}

	DeleteList(m_httpCommList);

	pos = m_httpTransList.GetHeadPosition();
	while(pos)
	{
		CThrdHttpTransfer *pThrd = m_httpTransList.GetNext(pos);
		ASSERT(pThrd);

		pThrd->End();
	}

	DeleteList(m_httpTransList);
	sLock.Unlock();

	End();
	CSingleLock(&m_mutexNeedDelThrd,TRUE);
	//删除已经完成的线程
	pos = m_NeedDelCommList.GetHeadPosition();
	while(pos)
	{
		CThrdHttpComm *pThrd = m_NeedDelCommList.GetNext(pos);
		ASSERT(pThrd);
		pThrd->End();
	}
	DeleteList(m_NeedDelCommList);
	pos = m_NeedDelTransList.GetHeadPosition();
	while(pos)
	{
		CThrdHttpTransfer *pThrd = m_NeedDelTransList.GetNext(pos);
		ASSERT(pThrd);
		pThrd->End();
	}
	DeleteList(m_NeedDelTransList);
	m_mutexNeedDelThrd.Unlock();

	delete this;
	return NOERROR;
}

HRESULT CHttpClient::SetUserInfo(LPCTSTR lpUserName,LPCTSTR lpPassword)
{
	m_username = lpUserName;
	m_password = lpPassword;

	return NO_ERROR;
}

HRESULT CHttpClient::Get(GUID guid,LPCTSTR lpURL,BOOL bAuth,LPCTSTR lpCaPath)
{
	CSingleLock sLock(&m_mutexThrd, TRUE);
	sHttpRequest req;
	req.clientId = guid;
	req.bAuth = bAuth;
	req.szURL = lpURL;
	req.type = en_Get;
	req.username = m_username;
	req.password = m_password;

	CThrdHttpComm* pThrdHttpComm = new CThrdHttpComm();
	pThrdHttpComm->SetData(this,m_pNotify,req);
	pThrdHttpComm->Begin();

	m_httpCommList.AddTail(pThrdHttpComm);
	return NO_ERROR;
}

HRESULT CHttpClient::Post(GUID guid,LPCTSTR lpURL,CStringArray& arrHeader,BOOL bAuth,LPCTSTR lpCaPath)
{
	CSingleLock sLock(&m_mutexThrd, TRUE);
	sHttpRequest req;
	req.clientId = guid;
	req.bAuth = bAuth;
	req.szURL = lpURL;
	req.type = en_Post;
	req.username = m_username;
	req.password = m_password;
	
	int cnt = arrHeader.GetCount();
	for (int i=0;i<cnt;i++)
	{
		req.arrHeader.Add(arrHeader.GetAt(i));
	}

	CThrdHttpComm* pThrdHttpComm = new CThrdHttpComm();
	pThrdHttpComm->SetData(this,m_pNotify,req);
	pThrdHttpComm->Begin();
	m_httpCommList.AddTail(pThrdHttpComm);

	return NO_ERROR;
}

HRESULT CHttpClient::Post(GUID guid,LPCTSTR lpURL, LPBYTE lpPost, size_t nPost,CStringArray& arrHeader,BOOL bAuth,LPCTSTR lpCaPath)
{
	CSingleLock sLock(&m_mutexThrd, TRUE);
	sHttpRequest req;
	req.clientId = guid;
	req.bAuth = bAuth;
	req.szURL = lpURL;
	req.type = en_Post;
	req.username = m_username;
	req.password = m_password;

	int cnt = arrHeader.GetCount();
	for (int i=0;i<cnt;i++)
	{
		req.arrHeader.Add(arrHeader.GetAt(i));
	}

	req.dataType = 1;
	req.lpPost = new BYTE[nPost];
	memcpy(req.lpPost,lpPost,nPost);
	req.nPost = nPost;

	CThrdHttpComm* pThrdHttpComm = new CThrdHttpComm();
	pThrdHttpComm->SetData(this,m_pNotify,req);
	pThrdHttpComm->Begin();
	m_httpCommList.AddTail(pThrdHttpComm);
	
	return NO_ERROR;
}

HRESULT CHttpClient::Put(GUID guid,LPCTSTR lpURL,LPBYTE lpData, size_t nDataLen,CStringArray& arrHeader,BOOL bAuth,LPCTSTR lpCaPath)
{
	CSingleLock sLock(&m_mutexThrd, TRUE);
	sHttpRequest req;
	req.clientId = guid;
	req.bAuth = bAuth;
	req.szURL = lpURL;
	req.type = en_Put;
	req.username = m_username;
	req.password = m_password;

	int cnt = arrHeader.GetCount();
	for (int i=0;i<cnt;i++)
	{
		req.arrHeader.Add(arrHeader.GetAt(i));
	}

	req.dataType = 1;
	req.lpPost = new BYTE[nDataLen];
	memcpy(req.lpPost,lpData,nDataLen);
	req.nPost = nDataLen;

	CThrdHttpComm* pThrdHttpComm = new CThrdHttpComm();
	pThrdHttpComm->SetData(this,m_pNotify,req);
	pThrdHttpComm->Begin();

	m_httpCommList.AddTail(pThrdHttpComm);

	return NO_ERROR;
}

HRESULT CHttpClient::DeleteReq(GUID guid,LPCTSTR lpURL,BOOL bAuth,LPCTSTR lpCaPath)
{
	CSingleLock sLock(&m_mutexThrd, TRUE);
	sHttpRequest req;
	req.clientId = guid;
	req.bAuth = bAuth;
	req.szURL = lpURL;
	req.type = en_Delete;
	req.username = m_username;
	req.password = m_password;

	CThrdHttpComm* pThrdHttpComm = new CThrdHttpComm();
	pThrdHttpComm->SetData(this,m_pNotify,req);
	pThrdHttpComm->Begin();

	m_httpCommList.AddTail(pThrdHttpComm);

	return NO_ERROR;
}

HRESULT	CHttpClient::UploadFile(GUID guid,LPCTSTR lpURL,LPBYTE lpData, size_t nDataLen,CStringArray& arrHeader,BOOL bAuth,LPCTSTR lpCaPath)
{
	CSingleLock sLock(&m_mutexThrd, TRUE);
	sHttpRequest req;
	req.clientId = guid;
	req.bAuth = bAuth;
	req.szURL = lpURL;
	req.type = en_Post;
	req.username = m_username;
	req.password = m_password;

	int cnt = arrHeader.GetCount();
	for (int i=0;i<cnt;i++)
	{
		req.arrHeader.Add(arrHeader.GetAt(i));
	}

	req.dataType = 1;
	req.lpPost = new BYTE[nDataLen];
	memcpy(req.lpPost,lpData,nDataLen);
	req.nPost = nDataLen;

	CThrdHttpComm* pThrdHttpComm = new CThrdHttpComm();
	pThrdHttpComm->SetData(this,m_pNotify,req);
	pThrdHttpComm->Begin();

	m_httpCommList.AddTail(pThrdHttpComm);

	return NO_ERROR;
}

HRESULT	CHttpClient::UploadFile(GUID guid,LPCTSTR lpURL,LPCTSTR lpFilePath,CStringArray& arrHeader,BOOL bAuth,LPCTSTR lpCaPath)
{
	CSingleLock sLock(&m_mutexThrd, TRUE);
	sHttpRequest req;
	req.clientId = guid;
	req.bAuth = bAuth;
	req.szURL = lpURL;
	req.type = en_Upload;
	req.username = m_username;
	req.password = m_password;

	int cnt = arrHeader.GetCount();
	for (int i=0;i<cnt;i++)
	{
		req.arrHeader.Add(arrHeader.GetAt(i));
	}

	req.dataType = 2;
	req.inFilePath = lpFilePath;

	CThrdHttpComm* pThrdHttpComm = new CThrdHttpComm();
	pThrdHttpComm->SetData(this,m_pNotify,req);
	pThrdHttpComm->Begin();

	m_httpCommList.AddTail(pThrdHttpComm);

	return NO_ERROR;
}

HRESULT CHttpClient::UploadFileList(GUID guid,LPCTSTR lpURL,LPCTSTR lpFileURLFormat,sHttpTranFileInfoArray &arrFileInfo,BOOL bAuth,LPCTSTR lpCaPath)
{
	CSingleLock sLock(&m_mutexThrd, TRUE);
	CHttpTransTaskArray arrTransTasks;
	int fileCnt = arrFileInfo.GetCount();
	for (int i=0; i<fileCnt; i++)
	{
		sHttpTranFileInfo info = arrFileInfo[i];

		sHttpTransTask* pItem = new sHttpTransTask;
		pItem->bAuth = bAuth;
		pItem->username = m_username;
		pItem->password = m_password;
		pItem->taskType = enTask_Upload;
		pItem->taskID = guid;
		pItem->filePath = info.filePath;
		pItem->sourceName = info.sourceName;
		pItem->destName = info.destName;
		pItem->destTitle = info.destTitle;
		pItem->fileExtension = info.fileExtension;
		pItem->remoteUrl = (CString)lpURL;
		pItem->fileUrlFormat=(CString)lpFileURLFormat;
		pItem->state = en_http_prepared;
		pItem->totalSize = info.fileSize;
		pItem->finishSize = 0;
		pItem->szMD5 = info.MD5;
		arrTransTasks.Add(pItem);
	}

	CThrdHttpTransfer* pThrdTransfer = new CThrdHttpTransfer();
	pThrdTransfer->SetData(this,m_pNotify,guid,arrTransTasks);
	pThrdTransfer->Begin();

	m_httpTransList.AddTail(pThrdTransfer);

	DeleteArray(arrTransTasks);

	return NO_ERROR;
}

HRESULT	CHttpClient::DownLoadFile(GUID guid,LPCTSTR lpURL,LPCTSTR lpFilePath,BOOL bAuth,LPCTSTR lpCaPath)
{
	CSingleLock sLock(&m_mutexThrd, TRUE);
	sHttpRequest req;
	req.clientId = guid;
	req.bAuth = bAuth;
	req.szURL = lpURL;
	req.type = en_Get;
	req.username = m_username;
	req.password = m_password;

	req.bSaveToFile = TRUE;
	req.OutFilePath = lpFilePath;

	CThrdHttpComm* pThrdHttpComm = new CThrdHttpComm();
	pThrdHttpComm->SetData(this, m_pNotify,req);
	pThrdHttpComm->Begin();

	m_httpCommList.AddTail(pThrdHttpComm);

	return NO_ERROR;
}

long CHttpClient::GetDownloadFileSize(GUID guid,LPCTSTR lpURL,BOOL bAuth,LPCTSTR lpCaPath)
{
	//未完成

	return NO_ERROR;
}

HRESULT CHttpClient::CancelCmd(GUID guid)
{
	CSingleLock sLock(&m_mutexThrd, TRUE);

	CThrdHttpComm *pThrd = NULL,*pTemp = NULL;
	POSITION pos = m_httpCommList.GetHeadPosition();
	while(pos)
	{
		pTemp = m_httpCommList.GetNext(pos);
		ASSERT(pTemp);

		if(pTemp->m_httpReq.clientId == guid)
		{	
			pThrd = pTemp;
			break;
		}
	}

	if (pThrd)
	{
		pos = m_httpCommList.Find(pThrd);
		if (pos)
		{
			m_httpCommList.RemoveAt(pos);
		}

		SafeDelete(pThrd);

		return T9_NOERROR;
	}
	else
	{
		CThrdHttpTransfer *pTransferThrd = NULL,*pTransferTemp = NULL;
		pos = m_httpTransList.GetHeadPosition();
		while(pos)
		{
			pTransferTemp = m_httpTransList.GetNext(pos);
			ASSERT(pTransferTemp);

			if(pTransferTemp->m_ClientID == guid)
			{	
				pTransferThrd = pTransferTemp;
				break;
			}
		}

		if (pTransferThrd)
		{
			pos = m_httpTransList.Find(pTransferThrd);
			if (pos)
			{
				m_httpTransList.RemoveAt(pos);
			}

			SafeDelete(pTransferThrd);

			return T9_NOERROR;
		}
	}

	return T9_S_FALSE;
}

void CHttpClient::Http_end(GUID guid)
{
	CSingleLock sLock(&m_mutexThrd, TRUE);

	//移除完成http的线程到需要删除的线程队列
	CThrdHttpComm *pThrd = NULL,*pTemp = NULL;
	POSITION pos = m_httpCommList.GetHeadPosition();
	while(pos)
	{
		pTemp = m_httpCommList.GetNext(pos);
		ASSERT(pTemp);

		if(pTemp->m_httpReq.clientId == guid)
		{	
			pThrd = pTemp;
			break;
		}
	}

	if (pThrd)
	{
		pos = m_httpCommList.Find(pThrd);
		if (pos)
		{
			m_httpCommList.RemoveAt(pos);
			CSingleLock(&m_mutexNeedDelThrd,TRUE);
			m_NeedDelCommList.AddTail(pThrd);
		}
	}
	else
	{
		CThrdHttpTransfer *pTransferThrd = NULL,*pTransferTemp = NULL;
		pos = m_httpTransList.GetHeadPosition();
		while(pos)
		{
			pTransferTemp = m_httpTransList.GetNext(pos);
			ASSERT(pTransferTemp);

			if(pTransferTemp->m_ClientID == guid)
			{	
				pTransferThrd = pTransferTemp;
				break;
			}
		}

		if (pTransferThrd)
		{
			pos = m_httpTransList.Find(pTransferThrd);
			if (pos)
			{
				m_httpTransList.RemoveAt(pos);
				CSingleLock(&m_mutexNeedDelThrd,TRUE);
				m_NeedDelTransList.AddTail(pTransferThrd);
			}
		}
	}
}

UINT CHttpClient::ThreadProc()
{
	DWORD dwResult = 0;
	while (TRUE)
	{
		dwResult = WaitForSingleObject(m_hExit,1000);
		if (dwResult != WAIT_TIMEOUT)
			break;

		CSingleLock(&m_mutexNeedDelThrd,TRUE);
		//删除已经完成的线程
		POSITION pos = m_NeedDelCommList.GetHeadPosition();
		while(pos)
		{
			CThrdHttpComm *pThrd = m_NeedDelCommList.GetNext(pos);
			ASSERT(pThrd);
			pThrd->End();
		}

		DeleteList(m_NeedDelCommList);

		pos = m_NeedDelTransList.GetHeadPosition();
		while(pos)
		{
			CThrdHttpTransfer *pThrd = m_NeedDelTransList.GetNext(pos);
			ASSERT(pThrd);
			pThrd->End();
		}
		DeleteList(m_NeedDelTransList);

		Sleep(100);
	}

	return 0;
}
