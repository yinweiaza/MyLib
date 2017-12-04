#pragma once

#include "..\..\include\OpenSource\curl\curl.h"
#include "HttpTranFileInfo.h"

struct sHttpAnawer 
{
	GUID	clientId;	//任务ID
	CURLcode res;		//执行结果
	long retcode;		//返回的http代码
	LPBYTE lpHeaderbuf;	//返回的http的header数据
	size_t nHeaderLen;	//header数据长度
	LPBYTE lpBodybuf;		//返回的body数据
	size_t nBodyLen;		//body数据长度

	sHttpAnawer()
	{
		clientId = GUID_NULL;
		res = CURLE_OK;
		retcode = 500;
		lpHeaderbuf = NULL;
		nHeaderLen = 0;
		lpBodybuf = NULL;
		nBodyLen = 0;
	}
};

interface IHttpClientNotify
{
	virtual HRESULT OnAnswer(sHttpAnawer* pAnswer) PURE;
	virtual HRESULT OnProgress(GUID guid,LONGLONG ultotal,LONGLONG ulnow) PURE;
	virtual HRESULT OnFileExist(GUID guid,LPBYTE lpBuf,int nBufLen,BOOL& bExist) PURE;
};

interface IHttpClient
{
	virtual HRESULT SetNotify(IHttpClientNotify* pNotify) PURE;
	virtual HRESULT Release() PURE;

	//设置登录信息
	virtual HRESULT SetUserInfo(LPCTSTR lpUserName,LPCTSTR lpPassword) PURE;
	virtual HRESULT	CancelCmd(GUID guid) PURE;

	//Get请求	
	virtual HRESULT Get(GUID guid,LPCTSTR lpURL,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//post请求
	virtual HRESULT Post(GUID guid,LPCTSTR lpURL, CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath=NULL) PURE;

	virtual HRESULT Post(GUID guid,LPCTSTR lpURL, LPBYTE lpPost, size_t nPost,
						CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//put请求
	virtual HRESULT Put(GUID guid,LPCTSTR lpURL,LPBYTE lpData, size_t nDataLen,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//delete请求
	virtual HRESULT DeleteReq(GUID guid,LPCTSTR lpURL,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//上传文件,直接传递文件内容
	virtual HRESULT	UploadFile(GUID guid,LPCTSTR lpURL,LPBYTE lpData, size_t nDataLen,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//上传文件，从文件读取内容
	virtual HRESULT	UploadFile(GUID guid,LPCTSTR lpURL,LPCTSTR lpFilePath,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//上传文件列表
	virtual	HRESULT UploadFileList(GUID guid,LPCTSTR lpURL,LPCTSTR lpFileURLFormat,sHttpTranFileInfoArray &arrFileInfo,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//下载文件
	virtual HRESULT	DownLoadFile(GUID guid,LPCTSTR lpURL,LPCTSTR lpFilePath,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//得到下载文件大小
	virtual long	GetDownloadFileSize(GUID guid,LPCTSTR lpURL,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;
};

extern "C" HRESULT WINAPI CreateHTTPClient(IHttpClient** ppHTTPClient);
//extern "C" HRESULT WINAPI DestoryHTTPClient();

#ifdef _DEBUG
#pragma comment(lib, "HTTPClientD.lib")
#else
#pragma comment(lib, "HTTPClient.lib")
#endif
