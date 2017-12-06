#pragma once

#include "..\..\include\OpenSource\curl\curlbuild.h"
#include "..\..\include\OpenSource\curl\curl.h"
#include "HttpCient.h"

//上传或者下载时的进度回调
typedef int (*pProressCallBack)(GUID taskID,double dtotal, double dnow);

// CThrdHttpComm
class CThrdHttpComm:public CUserThread
{
	friend class CHttpClient;
public:
	CThrdHttpComm();
	virtual ~CThrdHttpComm();
public:
	void SetData(CHttpClient* pOwner,IHttpClientNotify* pNotify,sHttpRequest& httpReq);
	virtual void	End();

protected:
	//线程函数
	virtual UINT	ThreadProc();
	
	//与服务器通信
	void			HttpCommutication();

	//得到下载文件大小
	long		    GetDownloadFileSize(const char* strUrl);

	static size_t	ReceiveResponseData(void *buffer, size_t size, size_t nmemb, void *userp); 
	static size_t	ReadSendData(void *buffer, size_t size, size_t nmemb, void *userp);
	static int		OnProgress(void *pClient, curl_off_t dltotal, curl_off_t dlnow,curl_off_t ultotal, curl_off_t ulnow);

protected:
	IHttpClientNotify* m_pNotify;
	CHttpClient*	  m_pHttpClient;

	sHttpRequest     m_httpReq;

	CURL*			  m_pCurl;

	pProressCallBack m_progressCB;  //上传或者下载时的进度回调函数
};