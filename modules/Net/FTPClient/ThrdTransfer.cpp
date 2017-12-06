// ThrdTransfer.cpp : implementation file
//

#include "stdafx.h"
#include "ThrdTransfer.h"
#include "AutoLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CThrdTransfer

CThrdTransfer::CThrdTransfer()
: m_pFtpxLib(NULL)
{
}

CThrdTransfer::~CThrdTransfer()
{
	m_mapThrdEvent.RemoveAll();
	DeleteArray(m_transTasks);	

	if(m_hExit)
	{
		CloseHandle(m_hExit);
		m_hExit = NULL;
	}

}

void	CThrdTransfer::SetOwner(CFtpxLib* pOwner)
{ 
	m_pFtpxLib = pOwner; 
}

UINT __ThreadProc(LPVOID lpParam)
{
	ASSERT(lpParam);
	CThrdTransfer *pThread = (CThrdTransfer*)lpParam;

	if(pThread->m_bInitCom)
		CoInitialize(NULL);

	UINT uiRet =  pThread->ThreadProc();

	if(pThread->m_bInitCom)
		CoUninitialize();

	pThread->m_csAccess.Lock();
	POSITION pos = pThread->m_mapThrdEvent.GetStartPosition();
	while (pos != NULL)
	{
		CWinThread* p = NULL;
		HANDLE handle;
		pThread->m_mapThrdEvent.GetNextAssoc(pos, p, handle);
		if (p->m_nThreadID == GetCurrentThreadId())
		{
			pThread->m_mapThrdEvent.RemoveKey(p);
			break;
		}
	}

	pThread->m_csAccess.Unlock();
	if (pThread->m_mapThrdEvent.GetCount()<=0)
	{
		delete pThread;
	}
	return uiRet;
}

HRESULT		CThrdTransfer::Init(BOOL bInitCom)
{
	CAutoLock lock(&m_csAccess);

	m_bInitCom = bInitCom;
	m_hExit = CreateEvent(NULL, TRUE, FALSE, NULL);

	for (int i=0; i<m_pFtpxLib->m_param.taskThreadCount; i++)
	{
		HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		CWinThread* pThread = AfxBeginThread(::__ThreadProc, LPVOID(this), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		pThread->m_bAutoDelete = TRUE;

		m_mapThrdEvent.SetAt(pThread, hEvent);
	}

	return NOERROR;
}

void		CThrdTransfer::UnInit(BOOL bWaitExit)
{
	CArray<HANDLE,HANDLE>m_hHandles;
	CWinThread* pThread; HANDLE hEvent;
	POSITION pos = m_mapThrdEvent.GetStartPosition();
	while (pos != NULL)
	{
		m_mapThrdEvent.GetNextAssoc(pos, pThread, hEvent);
		m_hHandles.Add(pThread->m_hThread);
	}

	SetEvent(m_hExit);

	if (bWaitExit)
	{
		for (int i=0; i<m_hHandles.GetCount(); i++)
		{
			WaitForSingleObject(m_hHandles[i], INFINITE);
		}
	}

}
HRESULT     CThrdTransfer::PauseTrans()
{
	
	CWinThread* pThread; HANDLE hEvent;
	POSITION pos = m_mapThrdEvent.GetStartPosition();
	while (pos != NULL)
	{
		m_mapThrdEvent.GetNextAssoc(pos, pThread, hEvent);
		pThread->SuspendThread();
	}
	return NOERROR;
}
HRESULT		CThrdTransfer::ResumeTrans()
{
	CWinThread* pThread; HANDLE hEvent;
	POSITION pos = m_mapThrdEvent.GetStartPosition();
	while (pos != NULL)
	{
		m_mapThrdEvent.GetNextAssoc(pos, pThread, hEvent);
		pThread->ResumeThread();
	}
	return NOERROR;
}

HANDLE		CThrdTransfer::GetThreadEvent(DWORD dwThreadId)
{
	CAutoLock lock(&m_csAccess);

	HANDLE hEvent = INVALID_HANDLE_VALUE;

	CWinThread* pThread = NULL;
	POSITION pos = m_mapThrdEvent.GetStartPosition();
	while (pos != NULL)
	{
		m_mapThrdEvent.GetNextAssoc(pos, pThread, hEvent);
		if (pThread->m_nThreadID == dwThreadId)
		{
			return hEvent;
		}
	}

	return hEvent;
}

UINT		CThrdTransfer::ThreadProc()
{
	HANDLE hEvents[2];

	hEvents[0] = m_hExit;
	hEvents[1] = GetThreadEvent(::GetCurrentThreadId());

	SetEvent(hEvents[1]);	//首先查询一次

	DWORD dwResult = 0;
	while (TRUE)
	{
		dwResult = WaitForMultipleObjects(sizeof(hEvents)/sizeof(HANDLE), 
			hEvents, FALSE, INFINITE);

		if (dwResult == WAIT_OBJECT_0) //退出
		{
			break;
		}
		else if (dwResult == WAIT_OBJECT_0+1) //上传
		{
			STransTask* pTask = GetFirstTask(TS_Wait, TS_Trans);
			if (pTask != NULL)
			{
				if (pTask->taskType==TT_Upload)
				{
					DoUpload(pTask);
					
				}
				else if (pTask->taskType==TT_Download)
				{
					DoDownload(pTask);
				}
			}			

			if (IsExitThread())
				break;

			if (HasWaitTask())
			{
				Sleep(1000);
				SetEvent(hEvents[1]);
			}
		}
		else 
		{
			ASSERT(FALSE);
			TRACE(_T("WaitForMultipleObjects fail: %d\n"), GetLastError());
		}
	}

	return 0;
}

TaskState	CThrdTransfer::DoUpload(STransTask* pTask)
{
	int	nRet = 1;
	HRESULT hr = G_NOERROR;

	ftplib ftp;	
	ftphandle* datahandle = NULL;

	CFile localFile;
	__int64 oldFinishSize = 0;
	CStringA remoteFile;
	do 
	{
		oldFinishSize = pTask->finishSize;

		DWORD dwServiceType = 0;
		CString strServer(_T(""));
		CString strObject(_T(""));
		INTERNET_PORT nPort = 21;
		CString strUsername(_T(""));
		CString strPassword(_T(""));
		//DWORD dwFlags = 0; 
		DWORD dwFlags = ICU_NO_ENCODE; 

		CString str(_T("")), str2(_T(""));
		CString strURL = pTask->remoteUrl;
		if (strURL.Left(6).CompareNoCase(_T("ftp://")) != 0)
			break;
		int nFind1 = strURL.ReverseFind('@');
		if (nFind1 > 6)
		{
			str = strURL.Mid(6, nFind1-6);
			strURL = _T("ftp://") + strURL.Right(strURL.GetLength()-nFind1-1);

			//取用户名、密码
			int nFind2 = str.Find(':');
			if (nFind2 >= 0)
			{
				strUsername = str.Left(nFind2);
				strPassword = str.Right(str.GetLength()-nFind2-1);
			}
		}

		//if (!AfxParseURLEx(pTask->remoteUrl, dwServiceType, strServer, strObject, nPort, strUsername, strPassword, dwFlags))
		//	break;

		if (!AfxParseURLEx(strURL, dwServiceType, strServer, strObject, nPort, str, str2, dwFlags))
			break;

		if (dwServiceType != AFX_INET_SERVICE_FTP)
			break;

		if (!IsContinue(pTask))
			break;

		CString strHost;
		strHost.Format(_T("%s:%d"), strServer, nPort);
		CT2A host(strHost);
		if (!ftp.Connect(host)) 
			break;

		if (!IsContinue(pTask))
			break;

		if (strUsername == _T(""))
			strUsername = _T("Anonymous");

		//CT2A user(strUsername), pass(strPassword);
		CStringA user = ConvToNetwork(strUsername);
		CStringA pass = ConvToNetwork(strPassword);

		if (!ftp.Login(user, pass))
		{
			hr = G_FTP_LOGINFAILED;
			break;
		}

		//CStringA path = ConvToNetwork(strObject);

		//进入文件所在目录
		//CString strPath(path);
		CString strPath(strObject);
		CString strDir;
		int iStart = 0, iFind;
		BOOL bMkdirSuccessful = TRUE;

		do 
		{
			iFind = strPath.Find('/', iStart);

			if (iFind < 0)
				break;

			strDir = strPath.Left(iFind+1);
			if (strDir != _T("/") && strDir != _T("./"))
			{
				//因为如果目录已存在,会返回失败,所以尝试创建目录即可
				CStringA dir = ConvToNetwork(strDir);
				bMkdirSuccessful = ftp.Mkdir(dir);	
			}

			iStart = iFind + 1;
		} while(TRUE);

		CStringA dir = ConvToNetwork(strDir);
		ftp.Chdir(dir);

		if (!IsContinue(pTask))
			break;


		iFind = strObject.ReverseFind('/');
		if (iFind >= 0)
			remoteFile = ConvToNetwork(strObject.Right(strObject.GetLength()-iFind-1));
		else
			remoteFile = ConvToNetwork(strObject);

		ASSERT(pTask->finishSize >= 0);
		if (pTask->finishSize == 0)	//新上传
		{
			if(pTask->bCoverSame)//如果服务器存在，删除服务器文件
			{
				ftp.Delete(remoteFile);
			}
			else//如果服务器存在，先判断是不是完整的，如果不是，重新上传(这里没做续传，就考虑到多线程写同一个文件的影响)
			{
				__int64 nWriteOff = 0;
				ftp.Size(remoteFile, &nWriteOff, ftplib::ascii);
				if (nWriteOff == pTask->totalSize)
				{
					pTask->finishSize = nWriteOff;
				}
				else
				{
					pTask->finishSize = 0;
				}
			}
		}
		else	//续传
		{
			//已上传到服务器大小
			__int64 nWriteOff = 0;
			ftp.Size(remoteFile, &nWriteOff, ftplib::ascii);

			pTask->finishSize = nWriteOff;
		}

		if (!IsContinue(pTask))
			break;

		UINT nOpenFlags = 0;

		if (!PathFileExists(pTask->localFile))
			break;

		nOpenFlags = CFile::modeRead|CFile::shareDenyNone|CFile::typeBinary; 

		if (!localFile.Open(pTask->localFile, nOpenFlags))	
		{
			hr = G_OPENFILEFAILED;
			break; 
		}

		pTask->totalSize = localFile.GetLength();
		Fire_OnTransfer(hr, pTask);

		//传输
#define TRANS_BUFSIZ		8192

		if (!IsContinue(pTask))
			break;

		int write = 0;
		char buf[TRANS_BUFSIZ];
		int bufLen;
		__int64 nReadOff = pTask->finishSize;

		datahandle = ftp.RawOpenEx(remoteFile, ftplib::filewrite, ftplib::image, pTask->finishSize); 
		if (datahandle == NULL)
			break;

		while (TRUE)
		{
			if (IsExitThread())
				break;

			if (!IsContinue(pTask))
				break;

			if (nReadOff >= pTask->totalSize)
			{
				pTask->state = TS_Finish;
				break;
			}

			localFile.Seek(nReadOff, CFile::begin);
			bufLen = localFile.Read(buf, TRANS_BUFSIZ);

			write = ftp.RawWrite(buf, bufLen, datahandle);
			//if (write < bufLen)
			if (write == 0)
			{
				TRACE(_T("LastError=%d\n"), GetLastError());
				break;
			}

			nReadOff += write;

			pTask->finishSize = nReadOff;
			Fire_OnTransfer(hr, pTask);
		}
	
	} while(FALSE);


	//关闭
 	if (datahandle != NULL)
		ftp.RawClose(datahandle);

	if (localFile.m_hFile != INVALID_HANDLE_VALUE)
		localFile.Close();

	TaskState state = TS_Invalid;

	//更新任务状态
	if (pTask != NULL)
	{
		if (pTask->finishSize - oldFinishSize <= 0)
			pTask->errCount += 1;
		else
			pTask->errCount = 0;

		if (FAILED(hr) || pTask->errCount > 30)
		{
			if (pTask->state != TS_Delete)
				SetTaskState(pTask, TS_Error);
		}
		else if (pTask->state == TS_Trans)
		{
			SetTaskState(pTask, TS_Wait);
		}

		Fire_OnTransfer(hr, pTask);

		state = (TaskState)pTask->state;
	}

	ftp.Quit();

	return state;
}

BOOL		CThrdTransfer::GetMD5FromServerFile(const CString& strServerFile, GUID& md5)
{
	CString strTemp = strServerFile;
	int iFirst = strTemp.ReverseFind(_T('/'));
	if (iFirst>=0)
	{
		strTemp = strTemp.Mid(iFirst+1);
	}

	iFirst = strTemp.ReverseFind(_T('.'));
	if (iFirst>=0)
	{
		strTemp = strTemp.Left(iFirst);
	}

	GUID guid = String2Guid(strTemp);

	if (guid != GUID_NULL)
	{
		md5 = guid;
		return TRUE;
	}

	return FALSE;
}

TaskState	CThrdTransfer::DoDownload(STransTask* pTask)
{
	int	nRet = 1;
	HRESULT hr = G_NOERROR;

	ftplib ftp;	
	ftphandle* datahandle = NULL;

	CFile localFile;
	__int64 oldFinishSize = 0;
	GUID serverMD5 = GUID_NULL;
	do 
	{
		GetMD5FromServerFile(pTask->remoteUrl, serverMD5);

		//检查本地文件是否存在
		if (serverMD5 != GUID_NULL && PathFileExists(pTask->localFile))
		{
			GUID localMD5;
			CreateMD5Quickly(pTask->localFile, localMD5);

			if (IsEqualGUID(localMD5, serverMD5))
			{
				//本地文件已存在
				if (localFile.Open(pTask->localFile, CFile::modeRead|CFile::shareDenyRead|CFile::typeBinary))
				{
					pTask->totalSize = localFile.GetLength();
					pTask->finishSize = pTask->totalSize;
					pTask->state = TS_Finish;
					localFile.Close();
					break;
				}
			}
		}

		oldFinishSize = pTask->finishSize;

		DWORD dwServiceType = 0;
		CString strServer(_T(""));
		CString strObject(_T(""));
		INTERNET_PORT nPort = 21;
		CString strUsername(_T(""));
		CString strPassword(_T(""));
		//DWORD dwFlags = 0; 
		DWORD dwFlags = ICU_NO_ENCODE; 

		CString str(_T("")), str2(_T(""));
		CString strURL = pTask->remoteUrl;
		if (strURL.Left(6).CompareNoCase(_T("ftp://")) != 0)
			break;
		int nFind1 = strURL.ReverseFind('@');
		if (nFind1 > 6)
		{
			str = strURL.Mid(6, nFind1-6);
			strURL = _T("ftp://") + strURL.Right(strURL.GetLength()-nFind1-1);

			//取用户名、密码
			int nFind2 = str.Find(':');
			if (nFind2 >= 0)
			{
				strUsername = str.Left(nFind2);
				strPassword = str.Right(str.GetLength()-nFind2-1);
			}
		}

		//if (!AfxParseURLEx(pTask->remoteUrl, dwServiceType, strServer, strObject, nPort, strUsername, strPassword, dwFlags))
		//	break;

		if (!AfxParseURLEx(strURL, dwServiceType, strServer, strObject, nPort, str, str2, dwFlags))
			break;

		if (dwServiceType != AFX_INET_SERVICE_FTP)
			break;

		if (!IsContinue(pTask))
			break;

		CString strHost;
		strHost.Format(_T("%s:%d"), strServer, nPort);
		CT2A host(strHost);
		if (!ftp.Connect(host)) 
			break;

		if (!IsContinue(pTask))
			break;

		if (strUsername == _T(""))
			strUsername = _T("Anonymous");

		//CT2A user(strUsername), pass(strPassword);
		CStringA user = ConvToNetwork(strUsername);
		CStringA pass = ConvToNetwork(strPassword);

		if (!ftp.Login(user, pass))
		{
			hr = G_FTP_LOGINFAILED;
			break;
		}

		//CStringA path = ConvToNetwork(strObject);

		//进入文件所在目录
		//CString strPath(path);
		CString strPath(strObject);
		CString strDir;
		int iStart = 0, iFind;
		BOOL bMkdirSuccessful = TRUE;

		do 
		{
			if (!IsContinue(pTask))
				break;

			iFind = strPath.Find('/', iStart);

			if (iFind < 0)
				break;

			strDir = strPath.Left(iFind+1);
			if (strDir != _T("/") && strDir != _T("./"))
			{
				//CStringA dir = CT2A(strDir);
				CStringA dir = ConvToNetwork(strDir);
				if (!ftp.Chdir(dir))
					break;
			}

			iStart = iFind + 1;
		} while(TRUE);

		if (!IsContinue(pTask))
			break;

		CStringA remoteFile;
		iFind = strObject.ReverseFind('/');
		if (iFind >= 0)
			remoteFile = ConvToNetwork(strObject.Right(strObject.GetLength()-iFind-1));
		else
			remoteFile = ConvToNetwork(strObject);

		//查询文件长度
		__int64 nTotalSize = 0;
		ftp.Size(remoteFile, &nTotalSize, ftplib::ascii);
		if (nTotalSize <= 0)
			break;

		if (!IsContinue(pTask))
			break;

		UINT nOpenFlags = 0;
		BOOL bResumeTrans = FALSE;
		__int64 nOffset = 0;

		CString	strTempFile(pTask->localFile+_T(".tmp"));
		if (PathFileExists(strTempFile))
		{
			if (localFile.Open(strTempFile, CFile::modeRead|CFile::shareDenyRead|CFile::typeBinary))
			{
				if (serverMD5 != GUID_NULL)	
				{
					//以MD5码的方式验证
					if (localFile.GetLength() > sizeof(GUID))
					{
						GUID md5 = GUID_NULL;
						localFile.Seek(localFile.GetLength()-sizeof(GUID), CFile::begin);
						localFile.Read(&md5, sizeof(GUID));
						if (IsEqualGUID(md5, serverMD5))
						{
							nOffset = localFile.GetLength() - sizeof(GUID);
							bResumeTrans = TRUE;
						}
					}
				}
				else
				{
					nOffset = localFile.GetLength();
					bResumeTrans = TRUE;
				}

				localFile.Close();
			}
		}

		if (bResumeTrans)	//续传
		{
			nOpenFlags = CFile::modeWrite|CFile::shareDenyWrite|CFile::typeBinary;
		}
		else				//新建
		{
			nOpenFlags = CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite|CFile::typeBinary; 
		}

		TCHAR szPath[MAX_PATH];
		_stprintf(szPath, _T("%s"), strTempFile);
		PathRemoveFileSpec(szPath);
		CreateDirectory(szPath, NULL);
		if (!localFile.Open(strTempFile, nOpenFlags))	
		{
			hr = G_OPENFILEFAILED;
			break; // 尝试以写独占方式打开, 失败该文件被占用
		}

		pTask->totalSize = nTotalSize;
		pTask->finishSize = nOffset;
		oldFinishSize = nOffset;

		Fire_OnTransfer(hr, pTask);

		//传输
#define TRANS_BUFSIZ		8192

		if (!IsContinue(pTask))
			break;

		int read = 0;
		char buf[TRANS_BUFSIZ];
		int bufLen = TRANS_BUFSIZ;

		datahandle = ftp.RawOpenEx(remoteFile, ftplib::fileread, ftplib::image, nOffset);
		if (datahandle == NULL)
			break;

		while (TRUE)
		{
			if (IsExitThread())
				break;

			if (!IsContinue(pTask))
				break;

			if (nOffset >= pTask->totalSize)
			{
				if (serverMD5 != GUID_NULL)
				{
					localFile.SetLength(localFile.GetLength()-sizeof(GUID));
				}

				//文件由临时文件名改为正式文件名
				localFile.Close();
				CString strFileName(pTask->localFile);
				DeleteFile(strFileName);
				MoveFile(strTempFile, strFileName);

				pTask->state = TS_Finish;
				break;
			}

			read = ftp.RawRead(buf, bufLen, datahandle);
			if (read == 0)
			{
				break;
			}

			localFile.Seek(nOffset, CFile::begin);
			localFile.Write(buf, read);

			if (serverMD5 != GUID_NULL)
			{
				localFile.Write(&serverMD5, sizeof(GUID));
				localFile.Seek(-LONGLONG(sizeof(GUID)), CFile::current);
			}

			nOffset += read;
			pTask->finishSize = nOffset;

			Fire_OnTransfer(hr, pTask);
		}
	} while(FALSE);


	//关闭
	if (datahandle != NULL)
		ftp.RawClose(datahandle);

	if (localFile.m_hFile != INVALID_HANDLE_VALUE)
		localFile.Close();

	TaskState state = TS_Invalid;

	if (pTask != NULL)
	{
		if (pTask->finishSize - oldFinishSize <= 0)
			pTask->errCount += 1;
		else
			pTask->errCount = 0;

		if (FAILED(hr) || pTask->errCount > 30)
		{
			if (pTask->state != TS_Delete)
				SetTaskState(pTask, TS_Error);
		}
		else if (pTask->state == TS_Trans)
		{
			SetTaskState(pTask, TS_Wait);
		}

		Fire_OnTransfer(hr, pTask);

		state = (TaskState)pTask->state;
	}

	ftp.Quit();

	return state;
}

int			CThrdTransfer::GetWaitTaskCount()
{
	CAutoLock lock(&m_csAccess);

	int count = 0;
	for (int i=0; i<m_transTasks.GetCount(); i++)
	{
		STransTask* p = m_transTasks[i];
		if (p->state == TS_Wait)
			++count;
	}

	return count;
}

BOOL		CThrdTransfer::HasWaitTask()
{
	CAutoLock lock(&m_csAccess);

	int count = 0;
	for (int i=0; i<m_transTasks.GetCount(); i++)
	{
		STransTask* p = m_transTasks[i];
		if (p->state == TS_Wait)
			return TRUE;
	}

	return FALSE;
}

STransTask*	CThrdTransfer::GetFirstTask(TaskState curState, TaskState newState)
{
	CAutoLock lock(&m_csAccess);

	STransTask* pTask = NULL;
	for (int i=0; i<m_transTasks.GetCount(); i++)
	{
		STransTask* p = m_transTasks[i];
		if (p->state == curState)
		{
			if (pTask == NULL)
				pTask = p;
			else if (p->priority > pTask->priority)
				pTask = p;
			else
				;
		}
	}
	
	if (pTask != NULL)
	{
		pTask->state = newState;
		return pTask;
	}

	return NULL;
}

void		CThrdTransfer::SetTaskState(STransTask* pTask, TaskState state,  BOOL bNotify)
{
	CAutoLock lock(&m_csAccess);

	pTask->state = state;

	if (bNotify && m_pFtpxLib != NULL)
	{
		Fire_OnTransfer(G_NOERROR, pTask);
	}
}

STransTask* CThrdTransfer::FindTransTask(LPCTSTR lpTaskId)
{
	for (int i=0; i<m_transTasks.GetCount(); i++)
	{
		STransTask* p = m_transTasks[i];
		if (p->taskId.CompareNoCase(lpTaskId) == 0)
			return p;
	}
	return NULL;
}

//文件传输
HRESULT		CThrdTransfer::Upload(LPCTSTR lpTaskId, LPCTSTR lpLocalFile, LPCTSTR lpRemoteUrl, __int64 nOffset, int nPriority,BOOL bCoverSame)
{
	if (FindTransTask(lpTaskId) != NULL)
		return E_FAIL;
	
	if (!IsFileExist(lpLocalFile))
		return E_FAIL;

	CFile file;
	if (!file.Open(lpLocalFile, CFile::modeRead|CFile::shareDenyNone))
		return E_FAIL;
	__int64 nTotalSize = file.GetLength();
	file.Close();

	CAutoLock	lock(&m_csAccess);

	STransTask*	pTask = new STransTask;
	pTask->taskType = TT_Upload;
	pTask->taskId = lpTaskId;
	pTask->localFile = lpLocalFile;
	pTask->remoteUrl = lpRemoteUrl;
	pTask->priority = nPriority;
	pTask->totalSize = nTotalSize;
	pTask->finishSize = nOffset;
	pTask->state = TS_Pause;
	pTask->errCount = 0;
	pTask->bCoverSame = bCoverSame;
	m_transTasks.Add(pTask);

	return NOERROR;
}

HRESULT		CThrdTransfer::Download(LPCTSTR lpTaskId, LPCTSTR lpLocalFile, LPCTSTR lpRemoteUrl, __int64 nOffset, int nPriority)
{
	if (FindTransTask(lpTaskId) != NULL)
		return E_FAIL;

	if (nOffset>0 && !IsFileExist(lpLocalFile))
		return E_FAIL;

	CAutoLock	lock(&m_csAccess);

	STransTask*	pTask = new STransTask;
	pTask->taskType = TT_Download;
	pTask->taskId = lpTaskId;
	pTask->localFile = lpLocalFile;
	pTask->remoteUrl = lpRemoteUrl;
	pTask->priority = nPriority;
	pTask->totalSize = 0;	//下载时向服务器查询
	pTask->finishSize = nOffset;
	pTask->state = TS_Pause;
	pTask->errCount = 0;
	m_transTasks.Add(pTask);

	return NOERROR;
}

HRESULT		CThrdTransfer::Begin(LPCTSTR lpTaskId)
{
	CAutoLock lock(&m_csAccess);

	STransTask* pTask = FindTransTask(lpTaskId);
	if (pTask == NULL)
		return E_FAIL;

	//修改任务状态为等候，可重传(由完成状态改变为等候状态)
	if (pTask->state != TS_Wait && pTask->state != TS_Trans)
	{
		pTask->errCount = 0;
		SetTaskState(pTask, TS_Wait, TRUE);
	}

	//通知传输线程
	CWinThread* pThread; HANDLE hEvent;
	POSITION pos = m_mapThrdEvent.GetStartPosition();
	while (pos != NULL)
	{
		m_mapThrdEvent.GetNextAssoc(pos, pThread, hEvent);
		SetEvent(hEvent);
	}

	return NOERROR;
}

HRESULT		CThrdTransfer::Pause(LPCTSTR lpTaskId)
{
	CAutoLock lock(&m_csAccess);

	STransTask* pTask = FindTransTask(lpTaskId);
	if (pTask == NULL)
		return E_FAIL;

	//修改任务状态
	if (pTask->state==TS_Wait || pTask->state==TS_Trans)
	{
		TaskState state = (TaskState)pTask->state;
		SetTaskState(pTask, TS_Pause);

		//当前任务等候，改变状态需要通知。如果正传输，则由传输线程处理。
		if (state==TS_Wait)
		{
			Fire_OnTransfer(G_NOERROR, pTask);
		}
	}

	return NOERROR;
}

HRESULT		CThrdTransfer::Remove(LPCTSTR lpTaskId)
{
	CAutoLock lock(&m_csAccess);

	STransTask* pTask = FindTransTask(lpTaskId);
	if (pTask == NULL)
		return E_FAIL;

	//修改任务状态，线程根据状态自动处理该任务
	pTask->state = TS_Delete;

	return NOERROR;
}

HRESULT		CThrdTransfer::SetPriority(LPCTSTR lpTaskId, int nPriority)
{
	CAutoLock lock(&m_csAccess);

	STransTask* pTask = FindTransTask(lpTaskId);
	if (pTask == NULL)
		return E_FAIL;

	pTask->priority = nPriority;

	return NOERROR;
}

BOOL		CThrdTransfer::IsContinue(STransTask* pTask)
{
	if (pTask->state != TS_Trans)
		return FALSE;

	if (IsExitThread())
		return FALSE;

	return TRUE;
}

BOOL		CThrdTransfer::IsExitThread()
{
	return (WaitForSingleObject(m_hExit, 0)==WAIT_OBJECT_0);
}

CStringA	CThrdTransfer::ConvToNetwork(CStringW& str)
{
	if (m_pFtpxLib->m_param.netCharCode == NCC_ANSI)
	{
		return CW2A(str);
	}
	else if (m_pFtpxLib->m_param.netCharCode == NCC_UTF8)
	{
		return UTF16toUTF8(str);
	}
	else
	{
		ASSERT(0);
		return NULL;
	}
}

void		CThrdTransfer::Fire_OnTransfer(HRESULT hr, STransTask* pTransTask)
{
	if (!IsExitThread() && m_pFtpxLib != NULL)
	{
		m_pFtpxLib->Fire_OnTransfer(hr, pTransTask);
	}
}