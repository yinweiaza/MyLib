// ThrdTask.cpp : implementation file
//

#include "stdafx.h"
#include "ThrdTask.h"
#include "ThrdClient.h"
#include "P2PClientImp.h"

// CThrdTask
#define PriorityOfTransFile	1
#define TransFile_BufSize	(8192)

IMPLEMENT_DYNCREATE(CThrdTask, CWinThread)

CThrdTask::CThrdTask()
{
	m_pClientSocket = NULL;

	m_strIP = _T("");
	m_dwPort = 0;

	m_pP2PClient = NULL;
	m_pThrdClient = NULL;

	m_aliveMsgQueue=0;
}

CThrdTask::~CThrdTask()
{
}

BOOL CThrdTask::InitInstance()
{
	CWinThread::InitInstance();
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	
	//初始化消息队列
	MSG msg;
	PeekMessage(&msg, NULL, WM_MESSAGE_NET_ASYNCSOCK_SEND, WM_MESSAGE_NET_ASYNCSOCK_SEND, PM_NOREMOVE);

	SocketThreadInit();

	if (m_taskType==ttt_fileList)
	{
		m_groupFile.LoadFromFileList(m_strFiles, m_strServerDir);
	}
	else if (m_taskType==ttt_dir)
	{
		m_groupFile.LoadFromDir(m_strDir, m_strServerDir);
	}
	else if (m_taskType==ttt_program)
	{
		m_groupFile.LoadFromProgram(m_progFileInfo, m_strServerDir);
	}

	BeginTransFile();

	TRACE(_T("CThrdTask InitInstance:%d\n"), GetCurrentThreadId());
	return TRUE;
}

int CThrdTask::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	SafeDelete(m_pClientSocket);

	TRACE(_T("CThrdTask ExitInstance:%d\n"), GetCurrentThreadId());
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThrdTask, CWinThread)
	ON_THREAD_MESSAGE(WM_MESSAGE_NET_CLIENT_TRANSFILE_PUMP, OnTrandFilePump)
	ON_THREAD_MESSAGE(WM_MESSAGE_NET_CLIENT_THRDTASK_PAUSE, OnThrdTaskPause)
	ON_THREAD_MESSAGE( WM_MESSAGE_NET_CLIENT_THRDTASK_CONTINUE, OnThrdTaskContinue)
	ON_THREAD_MESSAGE(WM_MESSAGE_NET_CLIENT_THRDTASK_CANCEL, OnThrdTaskCancel)
END_MESSAGE_MAP()


// CThrdTask message handlers
void	CThrdTask::SetParam(SThrdTaskParam* pParam)
{
	m_strIP = pParam->strIP;
	m_dwPort = pParam->dwPort;
	m_dwTask = pParam->dwTask;
	m_pP2PClient = pParam->pP2PClient;
	m_pThrdClient = pParam->pThrdClient;
}

void	CThrdTask::InitFromFileList(const GUID& dwTaskId, CStringList& strFiles, CString strServerDir)
{
	m_strFiles.RemoveAll();
	m_taskType		= ttt_fileList;
	m_dwTask		= dwTaskId;
	POSITION pos = strFiles.GetHeadPosition();
	while (pos != NULL)
	{
		CString& strFile = strFiles.GetNext(pos);
		m_strFiles.AddTail(strFile);
	}
	m_strServerDir	= strServerDir;
}

void	CThrdTask::InitFromDir(const GUID& dwTaskId, CString strDir, CString strServerDir)
{
	m_taskType		= ttt_dir;
	m_dwTask		= dwTaskId;
	m_strDir		= strDir;
	m_strServerDir	= strServerDir;
}

void	CThrdTask::InitFromProgram(const GUID& dwTaskId, STranFileInfoArray &progFileInfo, CString strServerDir)
{
	m_taskType		= ttt_program;
	m_dwTask		= dwTaskId;
	m_progFileInfo.Copy(progFileInfo);
	m_strServerDir	= strServerDir;
}

void	CThrdTask::OnSocketConnect(int nErrorCode)
{
	CSingleLock lock(&m_csAccess, TRUE);

	CString str;
	if (nErrorCode == 0)
	{
		PostThreadMessage(WM_MESSAGE_NET_CLIENT_TRANSFILE_PUMP, NETMSG_TRANSFILE_REQ, m_aliveMsgQueue);
	}
}

void	CThrdTask::OnSocketReceivedMessage(void* lpBuf, int nBufLen)
{
	CSingleLock lock(&m_csAccess, TRUE);

	if (!OnInnerNetMessage((DWORD)this, nBufLen, lpBuf))
	{
		m_pP2PClient->Fire_OnNotify((DWORD)this, cnt_data, nBufLen, lpBuf);
	}
}

void	CThrdTask::OnSocketClose(int nErrorCode)
{
	TRACE(_T("CThrdTask::OnSocketClose\n"));
}

BOOL	CThrdTask::OnInnerNetMessage(DWORD dwConnection, int nDataLen, void* lpData)
{
	CSingleLock lock(&m_csAccess, TRUE);

	if (nDataLen >= sizeof(WORD))
	{
		WORD wMsgId = *(WORD*)lpData;
		if (wMsgId==NETMSG_TRANSFILE_RES)
		{
			ProcTransFileRes(nDataLen, lpData);
		}
		else if (wMsgId==NETMSG_TRANSFILE_ACK)
		{
			LPTRANSFILE_ACK lpAck = (LPTRANSFILE_ACK)((char*)lpData+MsgIdLen);
			if (SUCCEEDED(lpAck->hr) && (lpAck->recvLen>0))
			{
				SFileTask* pItem = m_groupFile.GetCurrentTask();
				ASSERT(pItem != NULL);
				m_groupFile.AddFinishSize(pItem, lpAck->recvLen);

				Fire_TaskNotify(FALSE, m_groupFile.GetProgress());
			}
		}
		else if (wMsgId==NETMSG_TRANSFILE_END)
		{
			ProcTransFileEnd(nDataLen, lpData);
		}
		else if (wMsgId==NETMSG_TRANSTASK_END)
		{
			//进度通知
			Fire_TaskNotify(TRUE, m_groupFile.GetProgress());

			//退出传输线程
			//m_pP2PClient->OnCloseTask(m_dwTask);
		}
		else
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

void	CThrdTask::BeginTransFile()
{
	CSingleLock lock(&m_csAccess, TRUE);
	
	if (m_groupFile.GetFirstWaitTask() == NULL)
	{
		Fire_TaskNotify(TRUE, m_groupFile.GetProgress());

		//如果任务已完成，通知上层，退出现成
		//m_pP2PClient->OnCloseTask(m_dwTask);
		return;
	}

	//创建网络连接，开始传输
	SafeDelete(m_pClientSocket);
	m_aliveMsgQueue++;
	m_pClientSocket = new CClientSocket();
	m_pClientSocket->SetOwner(this);
	m_pClientSocket->Create();
	m_pClientSocket->Connect(m_strIP, m_dwPort);
}

void	CThrdTask::OnTrandFilePump(WPARAM wParam, LPARAM lParam)
{
	CSingleLock lock(&m_csAccess, TRUE);

	if (m_aliveMsgQueue != lParam)	//消息已经失效
		return;

	if (wParam==NETMSG_TRANSFILE_REQ)
	{
		PumpTransFileReq();
	}
	else if (wParam==NETMSG_TRANSFILE_BUF)
	{
		PumpTransFileBuf();
	}
	else if (wParam==NETMSG_TRANSFILE_END)
	{
		PumpTransFileEnd();
	}
	else if (wParam==NETMSG_TRANSTASK_END)
	{
		PumpTransTaskEnd();
	}
	else
	{
		ASSERT(FALSE);
	}
}

void	CThrdTask::PumpTransFileReq()
{
	if (IsSocketClosed())
		return;

	int nDataLen =0;
	char* lpData = NULL;

	SFileTask*	pItem = m_groupFile.GetFirstWaitTask();
	if (pItem == NULL)
		return;

	nDataLen = sizeof(WORD)+sizeof(TRANSFILE_REQ);
	lpData = new char[nDataLen];
	*(WORD*)(lpData) = NETMSG_TRANSFILE_REQ;
	LPTRANSFILE_REQ lpReq = (LPTRANSFILE_REQ)(lpData+sizeof(WORD));
	CString2WCharArray(pItem->serverFile, lpReq->serverFile);
	lpReq->fileSize		= pItem->totalSize;
	lpReq->fileMD5		= pItem->md5;
	SendData(nDataLen, lpData);

	SafeDelete(lpData);
	return;
}

void	CThrdTask::ProcTransFileRes(int nDataLen, void* lpData)
{
	LPTRANSFILE_RES lpRes = (LPTRANSFILE_RES)((char*)lpData+MsgIdLen);

	SFileTask*	pItem = m_groupFile.GetCurrentTask();
	if (pItem == NULL)
		return;

	if (FAILED(lpRes->hr))	//另外的任务正在传送该文件
	{
		Sleep(5000);

		//继续下一个任务
		m_groupFile.MoveToNextTask();

		PostThreadMessage(WM_MESSAGE_NET_CLIENT_TRANSFILE_PUMP, NETMSG_TRANSFILE_REQ, m_aliveMsgQueue);
		return;
	}
	else if (lpRes->fileExist)
	{
		PostThreadMessage(WM_MESSAGE_NET_CLIENT_TRANSFILE_PUMP, NETMSG_TRANSFILE_END, m_aliveMsgQueue);
		return;
	}
	else if (lpRes->finishSize >= 0)
	{
		m_groupFile.SetFinishSize(pItem, lpRes->finishSize);
		Fire_TaskNotify(FALSE, m_groupFile.GetProgress());

		if (m_transFile.m_hFile != INVALID_HANDLE_VALUE)
			m_transFile.Close();

		if (!m_transFile.Open(pItem->clientFile, CFile::modeRead|CFile::shareDenyNone))
		{
			//打开文件失败
			ASSERT(FALSE);
			return;
		}

		m_transFile.Seek(lpRes->finishSize, CFile::begin);
		PostThreadMessage(WM_MESSAGE_NET_CLIENT_TRANSFILE_PUMP, NETMSG_TRANSFILE_BUF, m_aliveMsgQueue);
		return;
	}
	else
	{
		ASSERT(FALSE);
	}
}

void	CThrdTask::PumpTransFileBuf()
{
	if (IsSocketClosed())
		return;

	if (IsSocketBufFull())
	{
		PostThreadMessage(WM_MESSAGE_NET_CLIENT_TRANSFILE_PUMP, NETMSG_TRANSFILE_BUF, m_aliveMsgQueue);
		return;
	}

	int nDataLen =0;
	char* lpData = NULL;

	int nBufSize = 0;
	char buf[TransFile_BufSize];
	__int64 nOffset = m_transFile.GetPosition();
	nBufSize = m_transFile.Read(buf, TransFile_BufSize);

	if (nBufSize > 0)
	{
		nDataLen = sizeof(WORD)+sizeof(TRANSFILE_BUF)+nBufSize;
		lpData = new char[nDataLen];
		*(WORD*)(lpData) = NETMSG_TRANSFILE_BUF;
		LPTRANSFILE_BUF lpBuf = (LPTRANSFILE_BUF)(lpData+MsgIdLen);
		lpBuf->offset = nOffset;
		lpBuf->length = nBufSize;
		memcpy(lpBuf->pBuf, buf, nBufSize);
		SendData(nDataLen, lpData);

		SafeDelete(lpData);

		PostThreadMessage(WM_MESSAGE_NET_CLIENT_TRANSFILE_PUMP, NETMSG_TRANSFILE_BUF, m_aliveMsgQueue);
		return;
	}
	else
	{
		//该文件完成传输
		PostThreadMessage(WM_MESSAGE_NET_CLIENT_TRANSFILE_PUMP, NETMSG_TRANSFILE_END, m_aliveMsgQueue);
		return;
	}
}

void	CThrdTask::PumpTransFileEnd()
{
	if (IsSocketClosed())
		return;

	//发送文件结束标志
	int nDataLen = sizeof(WORD);
	char* lpData = new char[nDataLen];

	*(WORD*)(lpData) = NETMSG_TRANSFILE_END;
	SendData(nDataLen, lpData);

	SafeDelete(lpData);

	//继续下一个文件或结束任务
	if (m_transFile.m_hFile != INVALID_HANDLE_VALUE)
		m_transFile.Close();
}

void	CThrdTask::ProcTransFileEnd(int nDataLen, void* lpData)
{
	SFileTask* pItem = m_groupFile.GetCurrentTask();
	ASSERT(pItem != NULL);

	m_groupFile.SetFinished(pItem);
	Fire_TaskNotify(FALSE, m_groupFile.GetProgress());

	if (m_groupFile.GetFirstWaitTask() != NULL)
	{
		if (!PostThreadMessage(WM_MESSAGE_NET_CLIENT_TRANSFILE_PUMP, NETMSG_TRANSFILE_REQ, m_aliveMsgQueue))
		{
			TRACE(_T("ProcTransFileEnd:%d\n"), m_nThreadID);
//			ASSERT(FALSE);
		}
		return;
	}
	else
	{
		PostThreadMessage(WM_MESSAGE_NET_CLIENT_TRANSFILE_PUMP, NETMSG_TRANSTASK_END, m_aliveMsgQueue);
		return;
	}
}

void	CThrdTask::PumpTransTaskEnd()
{
	if (IsSocketClosed())
		return;

	int nDataLen =0;
	char* lpData = NULL;

	//发送任务结束标志
	nDataLen = sizeof(WORD);
	lpData = new char[nDataLen];
	*(WORD*)(lpData) = NETMSG_TRANSTASK_END;
	SendData(nDataLen, lpData);

	SafeDelete(lpData);
}

void	CThrdTask::Fire_TaskNotify(BOOL bFinished, float fProgress)
{
	CSingleLock lock(&m_csAccess, TRUE);

	SCNTask task;
	task.dwTaskId = m_dwTask;
	task.bFinished = bFinished;
	task.fProgress = m_groupFile.GetProgress();

	m_pP2PClient->Fire_OnNotify((DWORD)m_pThrdClient, cnt_task, sizeof(task), (void*)&task);				
}

void	CThrdTask::SendData(int nDataLen, char* lpData)
{
	if (IsSocketClosed())
		return;

	if (m_pClientSocket != NULL)
	{
		m_pClientSocket->SendMessage(lpData, nDataLen);
	}
}

BOOL	CThrdTask::IsSocketClosed()
{
	if (m_pClientSocket == NULL || m_pClientSocket->m_hSocket == INVALID_SOCKET)
		return TRUE;

	return FALSE;
}

BOOL	CThrdTask::IsSocketBufFull()
{
	if (m_pClientSocket == NULL)
		return FALSE;

	return (m_pClientSocket->GetSendLeftLen() > THRDTASK_SOCKET_MAXBUF);
}

HRESULT	CThrdTask::Pause()
{
	CSingleLock lock(&m_csAccess, TRUE);

	HRESULT hr = NOERROR;
	PostThreadMessage(WM_MESSAGE_NET_CLIENT_THRDTASK_PAUSE, 0, 0);
	return hr;
}

HRESULT CThrdTask::Continue()
{
	CSingleLock lock(&m_csAccess, TRUE);

	HRESULT hr = NOERROR;
	PostThreadMessage( WM_MESSAGE_NET_CLIENT_THRDTASK_CONTINUE, 0, 0);
	return hr;
}

HRESULT CThrdTask::Cancel()
{
	CSingleLock lock(&m_csAccess, TRUE);

	HRESULT hr = NOERROR;
	PostThreadMessage(WM_MESSAGE_NET_CLIENT_THRDTASK_CANCEL, 0, 0);
	return hr;
}

void	CThrdTask::OnThrdTaskPause(WPARAM wParam, LPARAM lParam)
{
	CSingleLock lock(&m_csAccess, TRUE);

	if (m_pClientSocket != NULL)
	{		
		m_aliveMsgQueue++;
		m_pClientSocket->Close();
		SafeDelete(m_pClientSocket);
	}
}

void	CThrdTask::OnThrdTaskContinue(WPARAM wParam, LPARAM lParam)
{
	CSingleLock lock(&m_csAccess, TRUE);

	BeginTransFile();
}

void	CThrdTask::OnThrdTaskCancel(WPARAM wParam, LPARAM lParam)
{
	CSingleLock lock(&m_csAccess, TRUE);

	m_pP2PClient->OnCloseTask(m_dwTask);
}

