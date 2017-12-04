// ThrdServer.cpp : implementation file
//

#include "stdafx.h"
#include "P2PServerImp.h"
#include "ThrdServer.h"
#include "ListenSocket.h"
#include "..\\include\\\MessageDefine.h"
// #include "P2PServerApp.h"
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CThrdServer

CCriticalSection CThrdServer::m_csFile;
#define MsgIdLen	(sizeof(WORD))
#define TransFile_BufSize	(8192)

#define PriorityOfTransFile	1

IMPLEMENT_DYNCREATE(CThrdServer, CWinThread)

CThrdServer::CThrdServer()
{
	m_pP2PServer = NULL;
	m_pServerSocket = NULL;
	m_bDealInnerMsg	= TRUE;
}

CThrdServer::~CThrdServer()
{

	if (m_transFile.m_hFile != INVALID_HANDLE_VALUE)
		m_transFile.Close();

	TRACE(_T("SafeDelete(m_pServerSocket)\n"));
	SafeDelete(m_pServerSocket);

	POSITION pos = m_datas.GetHeadPosition();
	while (pos != NULL)
	{
		SDataItem* pItem = m_datas.GetNext(pos);
		DeleteDataItem(pItem);
	}
	m_datas.RemoveAll();
}

BOOL CThrdServer::InitInstance()
{
	CWinThread::InitInstance();
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//初始化消息队列
	MSG msg;
	PeekMessage(&msg, NULL, WM_MESSAGE_NET_ASYNCSOCK_SEND, WM_MESSAGE_NET_ASYNCSOCK_SEND, PM_NOREMOVE);

	SocketThreadInit();

	TRACE(_T("Thread InitInstance:%d\n"), GetCurrentThreadId());
	return TRUE;
}

int CThrdServer::ExitInstance()
{
	TRACE(_T("Thread ExitInstance:%d\n"), GetCurrentThreadId());
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThrdServer, CWinThread)
	ON_THREAD_MESSAGE(WM_MESSAGE_NET_SERVER_THREAD_ATTACHSOCKET, OnAttachSocket)
	ON_THREAD_MESSAGE(WM_MESSAGE_NET_SERVER_PUMP_SENDDATA, OnPumpSendData)
END_MESSAGE_MAP()


// CThrdServer message handlers
void			CThrdServer::SetParam(SThrdServerParam* pParam)
{
	m_pP2PServer = pParam->pP2PServer;
	m_strFileDir = pParam->strFileDir;
	m_bDealInnerMsg = pParam->bDealInnerMsg;
}

void			CThrdServer::SendData(int nDataLen, char* lpData, int nPriority)
{
	SDataItem* pItem = new SDataItem;
	pItem->nDataLen = nDataLen;
	pItem->pData = (char*)new char[nDataLen];
	memcpy(pItem->pData, lpData, nDataLen);
	pItem->nPriority = nPriority;

	CSingleLock lock(&m_csData, TRUE);
	m_datas.AddTail(pItem);

	PostThreadMessage(WM_MESSAGE_NET_SERVER_PUMP_SENDDATA, 0, 0);
}

void	CThrdServer::OnAttachSocket(WPARAM wParam, LPARAM lParam)
{
	if (m_pServerSocket==NULL)
	{
		TRACE(_T("m_pServerSocket = new CServerSocket()\n"));

		m_pServerSocket = new CServerSocket();
		m_pServerSocket->SetOwner(this);
		m_pServerSocket->Attach(wParam);
	}
}

void	CThrdServer::OnSocketClose()
{
	if (m_transFile.m_hFile != INVALID_HANDLE_VALUE)
	{
		m_transFile.Close();
		m_transFile.m_hFile = INVALID_HANDLE_VALUE;
	}

	if (m_pP2PServer)
	{
		m_pP2PServer->OnSocketClose((DWORD)this);
	}
}

void	CThrdServer::OnSocketReceivedMessage(void* lpBuf, int nBufLen)
{
	if (!m_bDealInnerMsg || !OnInnerNetMessage((DWORD)this, nBufLen, lpBuf))
	{
		m_pP2PServer->Fire_OnNotify((DWORD)this, snt_data, nBufLen, lpBuf);
	}
}

void	CThrdServer::OnPumpSendData(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = NOERROR;

	CSingleLock lock(&m_csData, TRUE);
	if (m_datas.GetCount() > 0)
	{
		SDataItem* pItem = m_datas.GetHead();
		if (pItem != NULL)
		{
			m_pServerSocket->SendMessage(pItem->pData, pItem->nDataLen);
			DeleteDataItem(pItem);
			m_datas.RemoveHead();
		}
	}

	if (m_datas.GetCount() > 0)
		PostThreadMessage(WM_MESSAGE_NET_SERVER_PUMP_SENDDATA, 0, 0);
}

void	CThrdServer::DeleteDataItem(SDataItem* pItem)
{
	if (pItem != NULL)
	{
		delete pItem->pData;
		delete pItem;
	}
}

BOOL	CThrdServer::OnInnerNetMessage(DWORD dwConnection, int nDataLen, void* lpData)
{
	if (nDataLen >= sizeof(WORD))
	{
		WORD wMsgId = *(WORD*)lpData;

		TRACE(_T("\nwMsgId=%d\n"), wMsgId);

		if (wMsgId==NETMSG_TRANSFILE_REQ)
		{
			ProcTransFileReq(nDataLen, lpData);		
		}
		else if (wMsgId==NETMSG_TRANSFILE_BUF)
		{
			ProcTransFileBuf(nDataLen, lpData);
		}
		else if (wMsgId==NETMSG_TRANSFILE_END)
		{
			ProcTransFileEnd(nDataLen, lpData);
		}
		else if (wMsgId==NETMSG_TRANSTASK_END)
		{
			ProcTransTaskEnd(nDataLen, lpData);
		}
		else
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

void	CThrdServer::AddDataItem(WORD wMsgId, CString& strXML)
{
	int nXmlLen = (strXML.GetLength()+1)*sizeof(WCHAR);
	int nMsgIdLen = sizeof(wMsgId);
	int nDataLen = nMsgIdLen + nXmlLen;

	SDataItem* pItem = new SDataItem;
	pItem->nDataLen = nDataLen;
	pItem->pData = (char*)new char[nDataLen];
	memcpy(pItem->pData, &wMsgId, nMsgIdLen);
	memcpy(pItem->pData+nMsgIdLen, strXML.GetBuffer(0), nXmlLen);
	pItem->nPriority = 1;

	CSingleLock lock(&m_csData, TRUE);
	m_datas.AddTail(pItem);
}

HRESULT			CThrdServer::ProcTransFileReq(int nDataLen, void* lpData)
{
	CSingleLock	lock(&m_csFile, TRUE);

	LPTRANSFILE_REQ lpReq = (LPTRANSFILE_REQ)((char*)lpData+MsgIdLen);

	CString strServerFile = lpReq->serverFile;

	TCHAR szPath[MAX_PATH];
	if(strServerFile.Find(InstallerPath)!=-1)
	{
		CString strInstallerPath;
		strInstallerPath = GetAppPath();
		strInstallerPath.Delete(strInstallerPath.GetLength()-1, 1);
		strServerFile.Replace(InstallerPath, strInstallerPath);
		_tcscpy(szPath, strServerFile);
	}
	else
	{
		PathCombine(szPath, m_strFileDir, lpReq->serverFile);
	}

	DWORD t1, t2;
	t1 = GetTickCount();

	BOOL	bFileExist = FALSE;
	__int64 nFinishSize = 0;
	HRESULT	hr	= NOERROR;

	do 
	{
		CString	strFile = szPath;
		if (PathFileExists(strFile))
		{
			//检查MD5码，如果正确，则返回文件完成
			GUID md5;
//			CreateMD5FromFile2(strFile, md5);
			CreateMD5Quickly(strFile, md5);
			if (IsEqualGUID(md5, lpReq->fileMD5))
			{
				bFileExist = TRUE;
				nFinishSize = lpReq->fileSize;
			}
		}

		CString strResumeFile;
		if (!bFileExist)
		{
			//检查续传情况
			strResumeFile = strFile + _T(".tmp");
			BOOL bResumeTrans = FALSE;

			//如果存在续传文件且MD5检查通过
			BOOL bCheckMD5 = TRUE;
			if (PathFileExists(strResumeFile))
			{
				if (m_transFile.Open(strResumeFile, CFile::modeReadWrite|CFile::shareDenyWrite))
				{
					if (m_transFile.GetLength() > sizeof(GUID))
					{
						GUID md5;
						m_transFile.Seek(m_transFile.GetLength() - sizeof(GUID), CFile::begin);
						m_transFile.Read(&md5, sizeof(GUID));

						if (IsEqualGUID(md5, lpReq->fileMD5))
						{
							bResumeTrans = TRUE;

							nFinishSize = m_transFile.GetLength() - sizeof(GUID);
						}
						else
						{
							m_transFile.Close();
						}
					}
					else
					{
						m_transFile.Close();
					}
				}
				else
				{
					//另外的任务正在传送该文件
					hr = E_FAIL;
					break;
				}
			}
			
			if (!bResumeTrans)
			{
				//创建目录
				_stprintf(szPath, _T("%s"), strResumeFile);
				PathRemoveFileSpec(szPath);
				_tcscat(szPath, _T("\\"));
// 				CT2A t2a(szPath);
				MakeSureDirectoryPathExistsEx(szPath);
				//SHCreateDirectoryEx(NULL,szPath,NULL);

				//创建文件
				if (m_transFile.m_hFile != INVALID_HANDLE_VALUE)
					m_transFile.Close();

				m_transFile.Open(strResumeFile, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite);
			}

			m_fileMD5 = lpReq->fileMD5;
		}
	} while (FALSE);

	t2 = GetTickCount();
	TRACE(_T("ProcTransFileReq:%d\n"), t2-t1);

	int nDataLen2 = sizeof(WORD)+sizeof(TRANSFILE_RES);
	char* lpData2 = new char[nDataLen2];
	*(WORD*)(lpData2) = NETMSG_TRANSFILE_RES;
	LPTRANSFILE_RES lpRes = (LPTRANSFILE_RES)(lpData2+sizeof(WORD));
	lpRes->fileExist = bFileExist;
	lpRes->finishSize = nFinishSize;
	lpRes->hr	= hr;

	SendData(nDataLen2, lpData2, PriorityOfTransFile);
	SafeDelete(lpData2);

	return NOERROR;
}

HRESULT			CThrdServer::ProcTransFileBuf(int nDataLen, void* lpData)
{
	if (m_transFile.m_hFile==INVALID_HANDLE_VALUE)
		return E_FAIL;

	LPTRANSFILE_BUF lpBuf = (LPTRANSFILE_BUF)((char*)lpData+MsgIdLen);
	if ((lpBuf->offset >= 0) && (lpBuf->length > 0) 
		&& (lpBuf->offset <= m_transFile.GetLength())) 
	{
//		TRACE(_T("ProcTransFileBuf:%d\n"), lpBuf->offset);
		m_transFile.Seek(lpBuf->offset, CFile::begin);
		m_transFile.Write(lpBuf->pBuf, lpBuf->length);
		m_transFile.Write(&m_fileMD5, sizeof(GUID));

		int nDataLen2 = sizeof(WORD)+sizeof(TRANSFILE_ACK);
		char* lpData2 = new char[nDataLen];
		*(WORD*)(lpData2) = NETMSG_TRANSFILE_ACK;
		LPTRANSFILE_ACK lpAck = (LPTRANSFILE_ACK)(lpData2+sizeof(WORD));
		lpAck->hr = NOERROR;
		lpAck->recvLen = lpBuf->length;

		SendData(nDataLen2, lpData2, PriorityOfTransFile);
		SafeDelete(lpData2);
	}
	else
	{
		ASSERT(FALSE);
	}

	return NOERROR;
}

HRESULT			CThrdServer::ProcTransFileEnd(int nDataLen, void* lpData)
{
	SendData(nDataLen, (char*)lpData, PriorityOfTransFile);

	if (m_transFile.m_hFile != INVALID_HANDLE_VALUE)
	{
		CString strOld = m_transFile.GetFilePath();
		CString strNew = strOld.Left(strOld.GetLength()-4);
		BOOL bCanRename = TRUE;
		if (PathFileExists(strNew))
		{
			if (!DeleteFile(strNew))
			{
				bCanRename = FALSE;
			}
		}

		if (bCanRename)
		{
			m_transFile.SetLength(m_transFile.GetLength() - sizeof(GUID));	
		}
		m_transFile.Close();

		if (bCanRename)
		{
			CFile::Rename(strOld, strNew);
		}
	}

	return NOERROR;
}

HRESULT			CThrdServer::ProcTransTaskEnd(int nDataLen, void* lpData)
{
	SendData(nDataLen, (char*)lpData, PriorityOfTransFile);

	return NOERROR;
}
