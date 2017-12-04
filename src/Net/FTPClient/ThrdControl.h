#pragma once

// CThrdControl
#include "FtpxLibImp.h"

class CThrdControl : public CWinThread
{
	friend class CFtpxLib;
	DECLARE_DYNCREATE(CThrdControl)
public:
	void	SetOwner(CFtpxLib* pOwner);
	HRESULT Connect(LPCTSTR lpHost, UINT nPort, LPCTSTR lpUserName, LPCTSTR lpPassword);
	HRESULT List(LPCTSTR lpPath);
	HRESULT Disconnect();
	HRESULT ReConnect();

	HRESULT Size(LPCTSTR lpPath, __int64& nSize);
	HANDLE	m_hEventInit;

	HRESULT CreateDir(CString strServerDir);
	HRESULT ListSync(CString strPath, CRemoteItemArray& arrRemoteItem);
	HRESULT	PutFile(CString strLocalFile, CString strServerFile);
	HRESULT	GetFile(CString strLocalFile, CString strServerFile);
	HRESULT	DeleteFile(CString strServerFile);

	HRESULT MakesureConnect(ftplib& ftp);
	HRESULT ChdirToRoot(ftplib& ftp);
protected:
	CThrdControl();           // protected constructor used by dynamic creation
	virtual ~CThrdControl();

	ftplib		m_ftp;
//	BOOL		m_bConnected;

	CFtpxLib*	m_pFtpxLib;
	CString		m_strHost;
	UINT		m_nPort;
	CString		m_strUserName;
	CString		m_strPassword;

	CString		m_strPath;

	CCriticalSection m_scAccess;

	void		OnThreadConnect(WPARAM wParam, LPARAM lParam);
	void		OnThreadList(WPARAM wParam, LPARAM lParam);
	void		OnThreadDisconnect(WPARAM wParam, LPARAM lParam);

	static void OnFtpCallbackLog(char *str, void* arg, bool out);

	BOOL		ParseListBuf(char* dirBuf,  CRemoteItemArray& remoteItems);
	CString		GetSegmentInfo(CString strFileInfo, int nSegmentStart, BOOL bToEnd=FALSE);

	enum
	{
		FMT_Invalid,
		FMT_MSDOS,
		FMT_UNIX
	};
	int			LineInfoFmt(const CString& strLineInfo);
	CString		GetItemName(const CString& strLineInfo, int nFmt);
	__int64		GetItemSize(const CString& strLineInfo, int nFmt);
	//FILETIME	GetItemDate(const CString& strLineInfo, int nFmt);
	CString		GetItemDate(const CString& strLineInfo, int nFmt);
	int			GetItemType(const CString& strLineInfo, int nFmt);
	CString		GetItemAttri(const CString& strLineInfo, int nFmt);

	BOOL		IsDataTime(CString& str);

	BOOL		ConvToLocal(const char* lpBuffer, CString& strOut);
	CStringA	ConvToNetwork(CStringW& str);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


