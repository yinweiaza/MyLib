// ThrdControl.cpp : implementation file
//

#include "stdafx.h"
#include "ThrdControl.h"
#include "FtpxLibImp.h"
#include "..\\include\MessageDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CThrdControl, CWinThread)

CThrdControl::CThrdControl()
{
//	m_bConnected = FALSE;
	m_pFtpxLib = NULL;
	m_strHost = _T("");
	m_nPort = 0;
	m_strUserName = _T("");
	m_strPassword = _T("");

	m_strPath = _T("");

	m_hEventInit = CreateEvent(0, TRUE, FALSE, _T("FtpxLib Thread Contorl"));
}

CThrdControl::~CThrdControl()
{
}

BOOL CThrdControl::InitInstance()
{
	CWinThread::InitInstance();
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//初始化消息队列
	MSG msg;
	PeekMessage(&msg, NULL, WM_MESSAGE_NET_ASYNCSOCK_SEND, WM_MESSAGE_NET_ASYNCSOCK_SEND, PM_NOREMOVE);

	SocketThreadInit();

	SetEvent(m_hEventInit);

	TRACE(_T("CThrdControl::InitInstance: ThreadID %d\n"), GetCurrentThreadId());


	return TRUE;
}

int CThrdControl::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	m_ftp.Quit();

	if(m_hEventInit)
	{
		CloseHandle(m_hEventInit);
		m_hEventInit = NULL;
	}

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThrdControl, CWinThread)
	ON_THREAD_MESSAGE(WM_MESSAGE_NET_THREAD_CONNECT, OnThreadConnect)
	ON_THREAD_MESSAGE(WM_MESSAGE_NET_THREAD_LIST, OnThreadList)
END_MESSAGE_MAP()


// CThrdControl message handlers
void CThrdControl::SetOwner(CFtpxLib* pOwner)
{ 
	m_pFtpxLib = pOwner; 
	m_ftp.SetCallbackArg(m_pFtpxLib);
	m_ftp.SetCallbackLogFunction(OnFtpCallbackLog);
}


void CThrdControl::OnFtpCallbackLog(char *str, void* arg, bool out)
{
	CFtpxLib* pFtpxLib = (CFtpxLib*)arg;
	CA2T a2t(str);
	pFtpxLib->Fire_OnLog(a2t);
}

HRESULT CThrdControl::Connect(LPCTSTR lpHost, UINT nPort, LPCTSTR lpUserName, LPCTSTR lpPassword)
{
	m_strHost = lpHost;
	m_nPort = nPort;
	m_strUserName = lpUserName;
	m_strPassword = lpPassword;

	TRACE(_T("CThrdControl::Connect: ThreadID %d\n"), m_nThreadID);
	PostThreadMessage(WM_MESSAGE_NET_THREAD_CONNECT, 0, 0);

	return NOERROR;
}

HRESULT CThrdControl::List(LPCTSTR lpPath)
{
	m_strPath = lpPath;

	PostThreadMessage(WM_MESSAGE_NET_THREAD_LIST, 0, 0);

	return NOERROR;
}

HRESULT CThrdControl::Disconnect()
{
	PostThreadMessage(WM_MESSAGE_NET_THREAD_DISCONNECT, 0, 0);	

	return NOERROR;
}

HRESULT CThrdControl::ReConnect()
{
	PostThreadMessage(WM_MESSAGE_NET_THREAD_CONNECT, 0, 0);

	return NOERROR;
}

HRESULT CThrdControl::Size(LPCTSTR lpPath, __int64& nSize)
{
	CAutoLock lock(&m_scAccess);

	CString strPath(lpPath);
	HRESULT hr = NOERROR;

	do 
	{
		hr = MakesureConnect(m_ftp);
		if (FAILED(hr))
			break;

		hr = ChdirToRoot(m_ftp);
		if (FAILED(hr))
			break;		

		//已上传到服务器大小
		CStringA remoteFile = ConvToNetwork(strPath);
		if (m_ftp.Size(remoteFile, &nSize, ftplib::ascii) != 1)
			hr = E_FAIL;

	} while(FALSE);

	return hr;
}

void		CThrdControl::OnThreadConnect(WPARAM wParam, LPARAM lParam)
{
	CAutoLock lock(&m_scAccess);

	m_ftp.Quit();

	HRESULT hr = MakesureConnect(m_ftp);
	m_pFtpxLib->Fire_OnConnect(hr);
}

void		CThrdControl::OnThreadDisconnect(WPARAM wParam, LPARAM lParam)
{
	CAutoLock lock(&m_scAccess);

	m_ftp.Quit();

//	m_bConnected = FALSE;
}

void		CThrdControl::OnThreadList(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = G_NOERROR;

	CAutoLock lock(&m_scAccess);

	ftphandle* datahandle = NULL;
	char* dirBuf = NULL;
	CRemoteItemArray remoteItems;

	do 
	{
		hr = MakesureConnect(m_ftp);
		if (FAILED(hr))
			break;

		CStringA path = ConvToNetwork(m_strPath);

		datahandle = m_ftp.RawOpen(path, ftplib::dirverbose, ftplib::ascii);
		if (datahandle == NULL)
		{
			hr = G_FTP_LISTFAILED;
			break;
		}

#define BUFSIZE	1024

		int dirBufLen = BUFSIZE*8;
		int dirDataLen = 0;
		dirBuf = (char*)malloc(dirBufLen); 

		int read;
		char buf[BUFSIZE];  

		MSG msg;
		while (TRUE)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					break;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			read = m_ftp.RawRead(buf, BUFSIZE, datahandle);
			if (read <= 0)	//完成或中断
			{
				break;
			}

			dirDataLen += read;
			if (dirDataLen > dirBufLen)
			{
				dirBufLen += BUFSIZE*8;
				dirBuf = (char*)realloc(dirBuf, dirBufLen);
			}
			memcpy(dirBuf+(dirDataLen-read), buf, read);
		}

		//文件加入列表
		dirBuf[dirDataLen] = '\0';

		if (ParseListBuf(dirBuf, remoteItems))
		{
			m_pFtpxLib->Fire_OnList(NOERROR, m_strPath, remoteItems);
		}
		DeleteArray(remoteItems);
	} while(FALSE);

	if (dirBuf != NULL)
		free(dirBuf);

	if (datahandle != NULL)
		m_ftp.RawClose(datahandle);

	if (FAILED(hr))
	{
		m_pFtpxLib->Fire_OnList(hr, m_strPath, remoteItems);
	}
}

//MS-DOS文件列表格式解析:
//02-23-05  09:24AM                 2245 readme.ESn
//05-25-04  08:56AM             19041660 VC.ESn
//
//UNIX文件列表格式解析:
//-rw-rw-rw-   1 user     group      642048 Jan 19  2007 Depends.Exe
//drw-rw-rw-   1 user     group           0 Aug 20  2007 Dongle

BOOL CThrdControl::ParseListBuf(char* dirBuf,  CRemoteItemArray& remoteItems)
{
	CString str;
	if (!ConvToLocal(dirBuf, str))
		return FALSE;

//	TRACE(str);

	CString strLineInfo;
	int curPos = 0;
	CString strFileName;
	CString strTemp;

	strLineInfo = str.Tokenize(_T("\n"), curPos);
	while (strLineInfo != "")
	{
		//TRACE(strLineInfo+_T("\n"));

		int nFmt = LineInfoFmt(strLineInfo);
		if (nFmt != FMT_Invalid)
		{
			//加入到列表
			SRemoteItem* pItem = new SRemoteItem;
			pItem->name = GetItemName(strLineInfo, nFmt);
			pItem->size = GetItemSize(strLineInfo, nFmt);
			pItem->type = GetItemType(strLineInfo, nFmt);
			pItem->modify = GetItemDate(strLineInfo, nFmt);
			pItem->attri = GetItemAttri(strLineInfo, nFmt);
			remoteItems.Add(pItem);
		}

		strLineInfo = str.Tokenize(_T("\n"), curPos);
	}

	return TRUE;
}

/*
-rw-rw-rw-   1 user     group      642048 Jan 19  2007 Depends.Exe
drw-rw-rw-   1 user     group           0 Aug 20  2007 Dongle
*/
CString CThrdControl::GetSegmentInfo(CString strFileInfo, int nSegmentStart, BOOL bToEnd/*=FALSE*/)
{
	ASSERT(nSegmentStart<=8);

	int nIdx = -1;
	int nSeg = 0;
	CString strInfo = _T("");

	strFileInfo.TrimLeft();
	while(nSeg < nSegmentStart + 1)			//逐段切隔
	{
		nIdx = strFileInfo.Find(_T(" "));	//以空格为切隔
		if(nIdx < 0)					//如果已没有空格，即最后一段信息
		{
			if(nSeg == nSegmentStart)	//如果最后一段正好是需要的段
				strInfo = strFileInfo;	//返回信息
			else
				strInfo = _T("");
			break;
		}
		else
		{
			if ((nSeg == nSegmentStart) && bToEnd)	//如果到段尾
			{
				strInfo = strFileInfo;
				break;
			}
			strInfo = strFileInfo.Left(nIdx);		//得到段信息
			strFileInfo = strFileInfo.Mid(nIdx+1);	//切隔信息
			strFileInfo.TrimLeft();					//过滤左侧的空格符
		}
		++nSeg;
	}
	return strInfo;
}

BOOL CThrdControl::IsDataTime(CString& str)
{
	COleDateTime dt;
	return dt.ParseDateTime(str);
}

int			CThrdControl::LineInfoFmt(const CString& strLineInfo)
{
	COleDateTime dt;
	CString str = GetSegmentInfo(strLineInfo, 0, FALSE);
	if (str.GetAt(0) == 'd' || str.GetAt(0) == '-')		//UNIX格式
		return FMT_UNIX;
	else if (dt.ParseDateTime(str))
		return FMT_MSDOS;
	else
	{
		//ASSERT(0);
		return FMT_Invalid;
	}
}

CString		CThrdControl::GetItemName(const CString& strLineInfo, int nFmt)
{
	CString strName(_T(""));
	if (nFmt == FMT_MSDOS)
		strName = GetSegmentInfo(strLineInfo, 3, TRUE);
	else if (nFmt == FMT_UNIX)
		strName = GetSegmentInfo(strLineInfo, 8, TRUE);	
	else
		ASSERT(0);
	return strName;
}

__int64		CThrdControl::GetItemSize(const CString& strLineInfo, int nFmt)
{
	__int64 i64Size = 0;
	if (nFmt == FMT_MSDOS)
		i64Size = _ttoi64(GetSegmentInfo(strLineInfo, 2, FALSE));
	else if (nFmt == FMT_UNIX)
		i64Size = _ttoi64(GetSegmentInfo(strLineInfo, 4, FALSE));
	else
		ASSERT(0);
	return i64Size;
}

CString		CThrdControl::GetItemDate(const CString& strLineInfo, int nFmt)
{
	CString strDate = _T("");

	if (nFmt == FMT_MSDOS)
	{
		COleDateTime dt; 
		strDate = GetSegmentInfo(strLineInfo, 0, FALSE);
	}
	else if (nFmt == FMT_UNIX)
	{
		WORD wMonth = 0;
		CString str = GetSegmentInfo(strLineInfo, 5, FALSE).MakeLower();
		if (str==_T("jan")) wMonth = 1;
		else if (str==_T("feb")) wMonth = 2;
		else if (str==_T("mar")) wMonth = 3;
		else if (str==_T("apr")) wMonth = 4;
		else if (str==_T("may")) wMonth = 5;
		else if (str==_T("jun")) wMonth = 6;
		else if (str==_T("jul")) wMonth = 7;
		else if (str==_T("aug")) wMonth = 8;
		else if (str==_T("sep")) wMonth = 9;
		else if (str==_T("oct")) wMonth = 10;
		else if (str==_T("nov")) wMonth = 11;
		else if (str==_T("dec")) wMonth = 12;
		else ASSERT(0);

		WORD wDay = _ttoi(GetSegmentInfo(strLineInfo, 6, FALSE));
		
		SYSTEMTIME st;
		ZeroMemory(&st, sizeof(st));
		GetSystemTime(&st);

		CString strTemp = GetSegmentInfo(strLineInfo, 7, FALSE);
		if (strTemp.Find(':') < 0)
			st.wYear = _ttoi(strTemp);
		else
		{
			if ((st.wMonth < wMonth) || (st.wMonth==wMonth && st.wDay<wDay))
				st.wYear -= 1;
		}

		st.wMonth = wMonth;
		st.wDay = wDay;
		strDate.Format(_T("%.4d-%.2d-%.2d"), st.wYear, st.wMonth, st.wDay);
	}
	else
	{
		ASSERT(0);
	}
	
	return strDate;
}

int			CThrdControl::GetItemType(const CString& strLineInfo, int nFmt)
{
	int nType = -1;
	if (nFmt == FMT_MSDOS)
	{
		if (GetSegmentInfo(strLineInfo, 2, FALSE).CompareNoCase(_T("<DIR>"))==0)
			nType = Ftpx_Dir;
		else
			nType = Ftpx_File;
	}
	else if (nFmt == FMT_UNIX)
	{
		if (GetSegmentInfo(strLineInfo, 0, FALSE).GetAt(0) == 'd') 
			nType = Ftpx_Dir;
		else 
			nType = Ftpx_File;
	}
	else
	{
		ASSERT(0);
	}
	return nType;
}

CString		CThrdControl::GetItemAttri(const CString& strLineInfo, int nFmt)
{
	CString strAttri(_T(""));
	if (nFmt == FMT_MSDOS)
	{
		strAttri = _T("");
	}
	else if (nFmt == FMT_UNIX)
	{
		strAttri = GetSegmentInfo(strLineInfo, 0, FALSE);
	}
	else
	{
		
		ASSERT(0);
	}
	return strAttri;
}

BOOL		CThrdControl::ConvToLocal(const char* lpBuffer, CString& strOut)
{
	CString str;

	int nLength = MultiByteToWideChar(CP_UTF8, 0, lpBuffer, -1, NULL, 0);
	if (nLength==0)
		return FALSE;

	nLength = MultiByteToWideChar(CP_UTF8, 0, lpBuffer, -1,	str.GetBuffer(nLength), nLength);	
	if (nLength==0)
		return FALSE;

	strOut = str;
	str.ReleaseBuffer();

	return TRUE;
}

CStringA	CThrdControl::ConvToNetwork(CStringW& str)
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

HRESULT		CThrdControl::CreateDir(CString strServerDir)
{
	CAutoLock lock(&m_scAccess);

	CString strPath = UrlCanonicalize2(strServerDir);
	CString strDir;

	HRESULT hr = NOERROR;

	do 
	{
		hr = MakesureConnect(m_ftp);
		if (FAILED(hr))
			break;	

		//进入文件所在目录
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
				bMkdirSuccessful = m_ftp.Mkdir(dir);	
			}

			iStart = iFind + 1;
		} while(TRUE);

		CStringA dir = ConvToNetwork(strDir);
		if (m_ftp.Chdir(dir) != 1)
		{
			hr = E_FAIL;
		}

	} while(FALSE);

	return hr;
}

HRESULT		CThrdControl::ListSync(CString strPath, CRemoteItemArray& arrRemoteItem)
{
	HRESULT hr = NOERROR;
	CAutoLock lock(&m_scAccess);

	ftphandle* datahandle = NULL;
	char* dirBuf = NULL;

	do 
	{
		hr = MakesureConnect(m_ftp);
		if (FAILED(hr))
			break;

		//到根目录
		hr = ChdirToRoot(m_ftp);
		if (FAILED(hr))
			break;

		CStringA path = ConvToNetwork(UrlCanonicalize2(strPath));
		datahandle = m_ftp.RawOpen(path, ftplib::dirverbose, ftplib::ascii);
		if (datahandle == NULL)
		{
			hr = G_FTP_LISTFAILED;
			break;
		}

#define BUFSIZE	1024

		int dirBufLen = BUFSIZE*8;
		int dirDataLen = 0;
		dirBuf = (char*)malloc(dirBufLen); 

		int read;
		char buf[BUFSIZE];  

		MSG msg;
		while (TRUE)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					break;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			read = m_ftp.RawRead(buf, BUFSIZE, datahandle);
			if (read <= 0)	//完成或中断
			{
				break;
			}

			dirDataLen += read;
			if (dirDataLen > dirBufLen)
			{
				dirBufLen += BUFSIZE*8;
				dirBuf = (char*)realloc(dirBuf, dirBufLen);
			}
			memcpy(dirBuf+(dirDataLen-read), buf, read);
		}

		//文件加入列表
		dirBuf[dirDataLen] = '\0';

		if (!ParseListBuf(dirBuf, arrRemoteItem))
		{
			hr = E_FAIL;
		}
	} while(FALSE);

	if (dirBuf != NULL)
		free(dirBuf);

	if (datahandle != NULL)
		m_ftp.RawClose(datahandle);

	return hr;
}

HRESULT		CThrdControl::PutFile(CString strLocalFile, CString strServerFile)
{
	CAutoLock lock(&m_scAccess);

	CString strPath = UrlCanonicalize2(strServerFile);
	CString strDir;

	HRESULT hr = E_FAIL;

	do 
	{
		hr = MakesureConnect(m_ftp);
		if (FAILED(hr))
			break;

		//创建多级目录
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
				bMkdirSuccessful = m_ftp.Mkdir(dir);	
			}

			iStart = iFind + 1;
		} while(TRUE);

		hr = ChdirToRoot(m_ftp);
		if (FAILED(hr))
			break;

		CStringA remoteFile = ConvToNetwork(strPath);
		CStringA localFile = ConvToNetwork(strLocalFile);
		if (m_ftp.Put(localFile, remoteFile, ftplib::image) > 0)
		{
			hr = NOERROR;
		}

	} while(FALSE);

	return hr;
}

HRESULT		CThrdControl::GetFile(CString strLocalFile, CString strServerFile)
{
	CAutoLock lock(&m_scAccess);

	HRESULT hr = NOERROR;

	do 
	{
		hr = MakesureConnect(m_ftp);
		if (FAILED(hr))
			break;

		hr = ChdirToRoot(m_ftp);
		if (FAILED(hr))
			break;	

		//创建本地目录
		hr = MakeSureDirectory2(strLocalFile);
		if (FAILED(hr))
			break;	

		CStringA localFile = ConvToNetwork(strLocalFile);
		CStringA remoteFile = ConvToNetwork(strServerFile);
		if (m_ftp.Get(localFile, remoteFile, ftplib::image) != 1)
		{
			hr = E_FAIL;
		}
	} while(FALSE);

	return hr;
}

HRESULT		CThrdControl::DeleteFile(CString strServerFile)
{
	CAutoLock lock(&m_scAccess);

	CString strPath(strServerFile);
	CString strDir;
	int iStart = 0;

	HRESULT hr = NOERROR;

	do 
	{
		hr = MakesureConnect(m_ftp);
		if (FAILED(hr))
			break;

		hr = ChdirToRoot(m_ftp);
		if (FAILED(hr))
			break;	

		CStringA remoteFile = ConvToNetwork(strPath);
		if (m_ftp.Delete(remoteFile) != 1)
		{
			hr = E_FAIL;
			break;
		}
	} while(FALSE);

	return hr;
}

HRESULT		CThrdControl::MakesureConnect(ftplib& ftp)
{
	CAutoLock lock(&m_scAccess);

	CString strDir;
	int iStart = 0;

	HRESULT hr = NOERROR;

	do 
	{
		char workDir[MAX_PATH];	
		if (ftp.Pwd(workDir, MAX_PATH) == 1)
		{
			hr = NOERROR;
			break;	//已连接
		}

		CString strHost;
		strHost.Format(_T("%s:%d"), m_strHost, m_nPort);
		CT2A host(strHost);
		if (!ftp.Connect(host))
		{
			hr = G_FTP_CONNECTFAILED;
			break;
		}

		CStringA user = ConvToNetwork(m_strUserName);
		CStringA pass = ConvToNetwork(m_strPassword);

		if (!ftp.Login(user, pass))
		{
			hr = G_FTP_LOGINFAILED;
			break;
		}
	}while(FALSE);

	return hr;
}

HRESULT		CThrdControl::ChdirToRoot(ftplib& ftp)
{
	CString strRoot = _T("/");
	if (ftp.Chdir(ConvToNetwork(strRoot)) != 1)
	{
		return  E_FAIL;
	}
	return NOERROR;
}