#pragma once
#include "..\..\include\OpenSource\curl\curl.h"
#include "..\Include\Httpclientlib.h"
#include <string>
#include <afxmt.h>

enum  enRequestType
{
	en_Get=0,
	en_Post,
	en_Put,
	en_Delete,
	en_Upload
};

struct sHttpRequest
{
	GUID	clientId;	//任务ID
	BOOL	bAuth;		//是否授权
	CString username;	//用户名
	CString password;   //密码
	int		type;       //请求类型
	CString szURL;		//url地址
	CStringArray arrHeader;		//额外的http头
	int    dataType;   //上传的数据类型：0：无；1：缓冲区；2：文件
	LPBYTE  lpPost;		//需要传送的数据
	size_t  nPost;		//需要传送的数据长度
	CString inFilePath;//需要读取的文件路径
	LONGLONG  uTotallen; //需要读取的文件的总长度
	LONGLONG  uWirtten;	//上传时已经读取的文件长度
	BOOL     bSaveToFile;//是否收到的数据保存到文件，下载文件时需要
	LONGLONG  uTotalDownloadlen; //需要下载的文件的总长度
	LONGLONG  uDownloadLen;	//已经下载的长度
	std::string  rcvData;  //收到的回复内容
	long	headerLen;   //回复的内容中头的长度；
	CString OutFilePath;//保存的文件路径
	CString caPath;//https协议时候的证书路径
	
	sHttpRequest()
	{
		clientId = GUID_NULL;
		bAuth	 = TRUE;
		type = en_Get;
		dataType = 0;
		lpPost = NULL;
		nPost = 0;
		uTotallen = 0;
		uWirtten = 0;
		bSaveToFile = FALSE;
		uTotalDownloadlen = 0;
		uDownloadLen = 0;
		headerLen = 0;
	}

	sHttpRequest(const sHttpRequest& other)
	{
		*this = other;

		SafeDelete(lpPost);
		nPost = other.nPost;
		if (other.lpPost &&nPost>0)
		{
			lpPost = new BYTE[nPost];
			memcpy(lpPost,other.lpPost,nPost);
			
		}

		arrHeader.RemoveAll();
		int cnt = other.arrHeader.GetCount();
		for (int i=0;i<cnt;i++)
		{
			arrHeader.Add(other.arrHeader.GetAt(i));
		}
	}

	sHttpRequest& operator=(const sHttpRequest& other)
	{
		clientId = other.clientId;
		bAuth    = other.bAuth;
		username = other.username;	
		password = other.password;   
		type = other.type;      
		szURL = other.szURL;		
		dataType = other.dataType;   	
		inFilePath = other.inFilePath;
		uTotallen = other.uTotallen;
		uWirtten = other.uWirtten;	
		bSaveToFile = other.bSaveToFile;
		rcvData = other.rcvData; 
		OutFilePath = other.OutFilePath;
		caPath = other.caPath;
		uTotalDownloadlen = other.uTotalDownloadlen;
		uDownloadLen = other.uDownloadLen;
		headerLen    = other.headerLen;

		SafeDelete(lpPost);
		nPost = other.nPost;
		if (other.lpPost &&nPost>0)
		{
			lpPost = new BYTE[nPost];
			memcpy(lpPost,other.lpPost,nPost);

		}

		arrHeader.RemoveAll();
		int cnt = other.arrHeader.GetCount();
		for (int i=0;i<cnt;i++)
		{
			arrHeader.Add(other.arrHeader.GetAt(i));
		}

		return *this;
	}
	~sHttpRequest()
	{
		SafeDelete(lpPost);
		arrHeader.RemoveAll();
	}
};

class CThrdHttpComm;
class  CThrdHttpTransfer;

typedef CList<CThrdHttpComm*, CThrdHttpComm*> CThrdHttpComList;
typedef CList<CThrdHttpTransfer*, CThrdHttpTransfer*> CThrdHttpTransList;

// CHttpClient
class CHttpClient:public IHttpClient,public CUserThread
{
	friend class CThrdHttpComm;
	friend class CThrdHttpTransfer;
public:
	CHttpClient();
	~CHttpClient();

	//支持的功能
public:
	virtual HRESULT SetNotify(IHttpClientNotify* pNotify);
	virtual HRESULT Release();

	virtual HRESULT SetUserInfo(LPCTSTR lpUserName,LPCTSTR lpPassword);
	virtual HRESULT	CancelCmd(GUID guid);

	/*
	lpCaPath参数默认为空， https协议时才用得上，如果为空，则不验证服务器证书
	*/
	//get请求
	virtual HRESULT Get(GUID guid,LPCTSTR lpURL,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);

	//post请求
	virtual HRESULT Post(GUID guid,LPCTSTR lpURL,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath=NULL);

	virtual HRESULT Post(GUID guid,LPCTSTR lpURL, LPBYTE lpPost, size_t nPost,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);

	//put请求
	virtual HRESULT Put(GUID guid,LPCTSTR lpURL,LPBYTE lpData, size_t nDataLen,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);

	//delete请求
	virtual HRESULT DeleteReq(GUID guid,LPCTSTR lpURL,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);

	//上传文件
	virtual HRESULT	UploadFile(GUID guid,LPCTSTR lpURL,LPBYTE lpData, size_t nDataLen,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);
	
	virtual HRESULT	UploadFile(GUID guid,LPCTSTR lpURL,LPCTSTR lpFilePath,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);
	
	virtual	HRESULT UploadFileList(GUID guid,LPCTSTR lpURL,LPCTSTR lpFileURLFormat,sHttpTranFileInfoArray &arrFileInfo,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);

	//下载文件
	virtual HRESULT	DownLoadFile(GUID guid,LPCTSTR lpURL,LPCTSTR lpFilePath,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);
	
	//得到下载文件大小
	virtual long	GetDownloadFileSize(GUID guid,LPCTSTR lpURL,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);

protected:
	//线程函数
	virtual UINT	ThreadProc();

 	void	Http_end(GUID guid);	
	//void	Http_Upload(double dtotal, double dnow);
	//void	Http_DownLoad(double dtotal, double dnow);

protected:
	CString				m_username;
	CString				m_password;

	CThrdHttpComList    m_httpCommList;
	CThrdHttpTransList  m_httpTransList;
	CMutex				m_mutexThrd;

	IHttpClientNotify*	m_pNotify;

	CThrdHttpComList    m_NeedDelCommList;
	CThrdHttpTransList  m_NeedDelTransList;
	CMutex				m_mutexNeedDelThrd;
};