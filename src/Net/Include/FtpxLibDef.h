#pragma once

enum TaskState
{
	TS_Invalid,
	TS_Pause,
	TS_Wait,
	TS_Trans,
	TS_Finish,
	TS_Delete,
	TS_Error
};

enum TaskPriority
{
	TP_Low,	
	TP_Normal,
	TP_High
};

enum TaskType
{
	TT_Upload,
	TT_Download
};
struct STransTask 
{
	int			taskType;
	CString 	taskId;
	CString 	localFile;
	CString 	remoteUrl;
	int			state;
	__int64		totalSize;
	__int64		finishSize; 
	int			priority;
	DWORD		errCount;
	CString     strErrNote;
	BOOL        bCoverSame;
};

typedef CArray<STransTask*, STransTask*>CTransTaskArray;
typedef CList<STransTask*, STransTask*>CTransTaskList;

enum RemoteItemType
{
	Ftpx_Dir,
	Ftpx_File
};

struct SRemoteItem
{
	CString		name;		//名称
	__int64		size;		//大小
	int			type;		//类型
	//FILETIME	modify;		//修改时间
	CString		modify;		//修改时间
	CString		attri;		//属性
};

typedef CArray<SRemoteItem*, SRemoteItem*>CRemoteItemArray;
typedef CList<SRemoteItem*, SRemoteItem*>CRemoteItemList;


interface IFtpxNotify
{
	virtual HRESULT OnConnect(HRESULT hr, DWORD dwData) PURE;
	virtual HRESULT OnList(HRESULT hr, LPCTSTR lpPath, CRemoteItemArray& arrRemoteItem, DWORD dwData) PURE;
	virtual HRESULT OnTransfer(HRESULT hr, STransTask* pTransTask, DWORD dwData) PURE;
	virtual HRESULT OnLog(LPCTSTR lpText, DWORD dwData) PURE;
};

enum NetCharCode
{
	NCC_ANSI,
	NCC_UTF8
};

struct SFtpxParam
{
	DWORD	data;
	int		netType;			//0:56K,1:128K,...
	int		taskThreadCount;	//传输任务线程数
	int		netCharCode;		//0:ANSI,1:UTF8
};

interface IFtpxLib
{
	virtual HRESULT Init(SFtpxParam* lpParam, IFtpxNotify* lpNotify) PURE;
	virtual HRESULT	Release()	PURE;

	//控制命令
	virtual HRESULT Connect(LPCTSTR lpHost, UINT nPort, LPCTSTR lpUserName, LPCTSTR lpPassword) PURE;
	virtual HRESULT Disconnect() PURE;
	virtual HRESULT ReConnect() PURE;

	virtual HRESULT Size(LPCTSTR lpPath, __int64& nSize) PURE;

	//扩展功能
	virtual HRESULT UploadBatch(const GUID& dwTaskId, CTransTaskArray& arrTransTasks) PURE;
	virtual HRESULT PauseBatch(const GUID& dwTaskId) PURE;
	virtual HRESULT ContinueBatch(const GUID& dwTaskId) PURE;
	virtual HRESULT CancelBatch(const GUID& dwTaskId) PURE;

	virtual HRESULT DownloadBatch(CTransTaskArray& arrTransTasks) PURE;
	virtual HRESULT GetConnectURL(CString& strURL) PURE;
	virtual HRESULT CreateDir(LPCTSTR strServerDir) PURE;
	virtual HRESULT ListSync(LPCTSTR strPath, CRemoteItemArray& arrRemoteItem) PURE; 
	virtual HRESULT	PutFile(LPCTSTR strLocalFile, LPCTSTR strServerFile) PURE;
	virtual HRESULT	GetFile(LPCTSTR strLocalFile, LPCTSTR strServerFile) PURE;
	virtual HRESULT	DeleteFile(LPCTSTR strServerFile) PURE;
};

extern "C" HRESULT WINAPI CreateFtpxLib(IFtpxLib** ppIFtpxLib);
