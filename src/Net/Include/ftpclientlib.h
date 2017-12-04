#pragma once

#include "TranFileInfo.h"
#include "FtpxLibDef.h"

interface IFTPClientNotify
{
	virtual HRESULT OnFTPNotify(DWORD dwConnection, DWORD dwType, int nDataLen, void* lpData) PURE;
};

struct SFTPClientParam 
{
	int		taskThreadCount;	//传输任务线程数
	int		netCharCode;		//0:ANSI,1:UTF8
};

//////////////////////////////////////////////////////////////////////////
// IP2PClient，点对点传输控制组件接口

interface IFTPClient
{
	virtual HRESULT SetParam(SFTPClientParam* pParam) PURE;
	virtual HRESULT SetNotify(IFTPClientNotify* pNotify) PURE;
	virtual HRESULT Release() PURE;

	virtual HRESULT AddConnect(CString strIP, DWORD dwPort, LPCTSTR strUserName, LPCTSTR strPassword, DWORD& dwConnection) PURE;
	virtual HRESULT RetryConnect(DWORD dwConnection) PURE;
	virtual HRESULT DeleteConnect(DWORD dwConnection) PURE;

	virtual	HRESULT UploadFileList(DWORD dwConnection, const GUID & dwTaskId, STranFileInfoArray &progFileInfo, CString strServerDir) PURE;
	virtual	HRESULT DownloadFileList(DWORD dwConnection, const GUID & dwTaskId, STranFileInfoArray &progFileInfo, CString strServerDir) PURE;

	virtual HRESULT PauseTask(const GUID & dwTaskId) PURE;
	virtual HRESULT ContinueTask(const GUID & dwTaskId) PURE;
	virtual HRESULT CancelTask(const GUID & dwTaskId) PURE;

	virtual HRESULT ListSync(DWORD dwConnection, LPCTSTR strPath, CRemoteItemArray& arrRemoteItem) PURE; 
	virtual HRESULT PutFile(DWORD dwConnection, LPCTSTR	strLocalFile, LPCTSTR strServerFile) PURE;
	virtual HRESULT GetFile(DWORD dwConnection, LPCTSTR	strLocalFile, LPCTSTR strServerFile) PURE;
	virtual HRESULT SendCommand(DWORD dwConnection, LPCTSTR	strCmdFile, LPCTSTR strServerDir) PURE;
};

extern "C" HRESULT WINAPI CreateFTPClient(IFTPClient** ppFTPClient);

#ifdef _DEBUG
	#pragma comment(lib, "FTPClientD.lib")
#else
	#pragma comment(lib, "FTPClient.lib")
#endif