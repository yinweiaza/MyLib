#pragma once

#include "HttpCient.h"
// CThrdHttpTransfer

enum enTaskType
{
	enTask_Upload,
	enTask_Download
};

struct sHttpTransTask 
{
	CString 	remoteUrl;						//服务器URL
	CString		fileUrlFormat;					//查询文件是否存在的URL格式字符串
	BOOL		bAuth;							//是否授权
	CString		username;						//用户名
	CString		password;						 //密码
	CString		caPath;							//https协议时候的证书路径
	int			taskType;
	GUID		taskID;
	CString 	filePath;
	CString		sourceName;						//需要传送的文件名,包括后缀名
	CString     destName;						//服务器上的显示名字，包括后缀名
	CString     destTitle;						//服务器上的显示名字,不包括后缀名
	CString		fileExtension;					//文件扩展名
	int			state;
	__int64		totalSize;
	__int64		finishSize; 
	CString		szMD5;

	sHttpTransTask()
	{
		bAuth = TRUE;
		taskType = enTask_Upload;
		taskID = GUID_NULL;
		state = en_http_prepared;
		totalSize = 0;
		finishSize = 0;
	}
	sHttpTransTask(const sHttpTransTask& other)
	{
		*this = other;
	}

	sHttpTransTask& operator = (const sHttpTransTask& other)
	{
		remoteUrl	= other.remoteUrl;
		fileUrlFormat=other.fileUrlFormat;
		bAuth		= other.bAuth;
		username	= other.username;
		password	= other.password;
		caPath		= other.caPath;
		taskType	= other.taskType;
		taskID		= other.taskID;
		filePath	= other.filePath;
		sourceName	= other.sourceName;
		destName    = other.destName;
		destTitle   = other.destTitle;
		fileExtension=other.fileExtension;
		state		= other.state;
		totalSize	= other.totalSize;
		finishSize	= other.finishSize; 
		szMD5		= other.szMD5;

		return *this;
	}
};

typedef CArray<sHttpTransTask*, sHttpTransTask*> CHttpTransTaskArray;

class CThrdHttpTransfer:public CUserThread
{
	friend class CHttpClient;
public:
	CThrdHttpTransfer();      
	virtual ~CThrdHttpTransfer();

public:
	void SetData(CHttpClient* pOwner,IHttpClientNotify* pNotify,GUID clientID,CHttpTransTaskArray& arrTransTasks);
	virtual void	End();

protected:
	//线程函数
	virtual UINT	ThreadProc();

	//查询服务器上文件是否存在
	HRESULT			IsFileRemoteExist(sHttpTransTask* pTask,BOOL& bExist);

	//上传文件
	HRESULT			DoUpload(sHttpTransTask* pTask);

	static size_t	ReceiveResponseData(void *buffer, size_t size, size_t nmemb, void *userp); 
	static int		OnProgress(void *pClient, curl_off_t dltotal, curl_off_t dlnow,curl_off_t ultotal, curl_off_t ulnow);

	HRESULT			GetUplodheader(sHttpTransTask* pTask,CStringArray& arrHeader);
	sHttpTransTask*	GetUploadTask();
	BOOL			HasWaitTask();

	void			OnTransfer(curl_off_t dltotal, curl_off_t dlnow,curl_off_t ultotal, curl_off_t ulnow);
	void            OnEnd(CURLcode res,long retcode);

	char*	        WCHARToACP(const wchar_t* str);

protected:
	IHttpClientNotify*	m_pNotify;
	CHttpClient*		m_pHttpClient;

	CURL*				m_pCurl;
	std::string			m_rcvData;

	HANDLE				m_hUploadEvent;
	CMutex				m_mutexTask;
	CHttpTransTaskArray m_arrTransTask;
	GUID				m_ClientID;
	__int64				m_nTotalSize;
	int					m_curTaskIndex;
};

