#pragma once

#include "..\Include\FtpxLibDef.h"
#include "..\Include\FTPClientLib.h"


#include "GroupFile.h"

class CFTPClient :	public IFTPClient, public IFtpxNotify
{
public:
	CFTPClient(void);
	virtual ~CFTPClient(void);

	virtual HRESULT SetParam(SFTPClientParam* pParam);
	virtual HRESULT SetNotify(IFTPClientNotify* pNotify);
	virtual HRESULT Release();

	virtual HRESULT AddConnect(CString strIP, DWORD dwPort, LPCTSTR strUserName, LPCTSTR strPassword, DWORD& dwConnection);
	virtual HRESULT RetryConnect(DWORD dwConnection);
	virtual HRESULT DeleteConnect(DWORD dwConnection);

//	virtual HRESULT List(LPCTSTR strPath, DWORD dwConnection);

	virtual	HRESULT UploadFileList(DWORD dwConnection, const GUID & dwTaskId, STranFileInfoArray &progFileInfo, CString strServerDir);
	virtual	HRESULT DownloadFileList(DWORD dwConnection, const GUID & dwTaskId, STranFileInfoArray &progFileInfo, CString strServerDir);

	virtual HRESULT PauseTask(const GUID & dwTaskId);
	virtual HRESULT ContinueTask(const GUID & dwTaskId);
	virtual HRESULT CancelTask(const GUID & dwTaskId);

	virtual HRESULT ListSync(DWORD dwConnection, LPCTSTR strPath, CRemoteItemArray& arrRemoteItem); 
	virtual HRESULT PutFile(DWORD dwConnection, LPCTSTR	strLocalFile, LPCTSTR strServerFile);
	virtual HRESULT GetFile(DWORD dwConnection, LPCTSTR	strLocalFile, LPCTSTR strServerFile);
	virtual HRESULT SendCommand(DWORD dwConnection, LPCTSTR	strCmdFile, LPCTSTR strServerDir);
protected:
	IFtpxLib*		GetFtpxLib(DWORD dwConnection);
	CGroupFile*		FindGroupFile(const CString& strFileId, SFileTask** pFileTask);

	void			OnCloseTask(GUID& dwTaskId);

protected:
	virtual HRESULT OnConnect(HRESULT hr, DWORD dwData);
	virtual HRESULT OnList(HRESULT hr, LPCTSTR lpPath, CRemoteItemArray& arrRemoteItem, DWORD dwData);
	virtual HRESULT OnTransfer(HRESULT hr, STransTask* pTransTask, DWORD dwData);
	virtual HRESULT OnLog(LPCTSTR lpText, DWORD dwData);

protected:
	IFTPClientNotify*	m_pNotify;
	SFTPClientParam		m_param;
	CMap<DWORD,DWORD, IFtpxLib*,IFtpxLib*>m_connects;

	CArray<CGroupFile*, CGroupFile*>m_groupFiles;
};
