#pragma once
#include "..\Include\FtpxLibDef.h"

class CThrdControl;
class CThrdTransfer;
class CFtpxData;

class CFtpxLib : public IFtpxLib
{
	friend class CThrdControl;
	friend class CThrdTransfer;
public:
	CFtpxLib(void);
	virtual ~CFtpxLib(void);

	virtual HRESULT Init(SFtpxParam* lpParam, IFtpxNotify* lpNotify);
	virtual HRESULT	Release();

	//控制命令
	virtual HRESULT Connect(LPCTSTR lpHost, UINT nPort, LPCTSTR lpUserName, LPCTSTR lpPassword);
	virtual HRESULT Disconnect();
	virtual HRESULT ReConnect();

	virtual HRESULT Size(LPCTSTR lpPath, __int64& nSize);

	//扩展功能
	virtual HRESULT UploadBatch(const GUID& dwTaskId, CTransTaskArray& arrTransTasks);
	virtual HRESULT PauseBatch(const GUID& dwTaskId);
	virtual HRESULT ContinueBatch(const GUID& dwTaskId);
	virtual HRESULT CancelBatch(const GUID& dwTaskId);

	virtual HRESULT DownloadBatch(CTransTaskArray& arrTransTasks);
	virtual HRESULT GetConnectURL(CString& strURL);

	virtual HRESULT CreateDir(LPCTSTR lpPath);
	virtual HRESULT	ListSync(LPCTSTR lpPath, CRemoteItemArray& arrRemoteItem);
	virtual HRESULT	PutFile(LPCTSTR strLocalFile, LPCTSTR strServerFile);
	virtual HRESULT	GetFile(LPCTSTR strLocalFile, LPCTSTR strServerFile);
	virtual HRESULT	DeleteFile(LPCTSTR strServerFile);

	static int BufSize(int nNetType);
protected:
	SFtpxParam		m_param;
	IFtpxNotify*	m_pNotify;
	CThrdControl*	m_pThrdControl;
	CThrdTransfer*	m_pThrdTransfer;
	
	//CPtrArray		m_thrdTransfers;
	CFtpxData*		m_pData;
	BOOL			m_bInit;
	CCriticalSection m_csAccess;

	CMap<CThrdTransfer*, CThrdTransfer*, CThrdTransfer*,CThrdTransfer*>m_thrdTasks; 

	void			Fire_OnConnect(HRESULT hr);
	void			Fire_OnList(HRESULT hr, LPCTSTR lpPath, CRemoteItemArray& arrRemoteItem);		
	void			Fire_OnTransfer(HRESULT hr, STransTask* pTransTask);
	void			Fire_OnLog(LPCTSTR lpText);
};
