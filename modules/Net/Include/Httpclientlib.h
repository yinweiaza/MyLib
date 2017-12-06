#pragma once

#include "..\..\include\OpenSource\curl\curl.h"
#include "HttpTranFileInfo.h"

struct sHttpAnawer 
{
	GUID	clientId;	//����ID
	CURLcode res;		//ִ�н��
	long retcode;		//���ص�http����
	LPBYTE lpHeaderbuf;	//���ص�http��header����
	size_t nHeaderLen;	//header���ݳ���
	LPBYTE lpBodybuf;		//���ص�body����
	size_t nBodyLen;		//body���ݳ���

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

	//���õ�¼��Ϣ
	virtual HRESULT SetUserInfo(LPCTSTR lpUserName,LPCTSTR lpPassword) PURE;
	virtual HRESULT	CancelCmd(GUID guid) PURE;

	//Get����	
	virtual HRESULT Get(GUID guid,LPCTSTR lpURL,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//post����
	virtual HRESULT Post(GUID guid,LPCTSTR lpURL, CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath=NULL) PURE;

	virtual HRESULT Post(GUID guid,LPCTSTR lpURL, LPBYTE lpPost, size_t nPost,
						CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//put����
	virtual HRESULT Put(GUID guid,LPCTSTR lpURL,LPBYTE lpData, size_t nDataLen,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//delete����
	virtual HRESULT DeleteReq(GUID guid,LPCTSTR lpURL,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//�ϴ��ļ�,ֱ�Ӵ����ļ�����
	virtual HRESULT	UploadFile(GUID guid,LPCTSTR lpURL,LPBYTE lpData, size_t nDataLen,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//�ϴ��ļ������ļ���ȡ����
	virtual HRESULT	UploadFile(GUID guid,LPCTSTR lpURL,LPCTSTR lpFilePath,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//�ϴ��ļ��б�
	virtual	HRESULT UploadFileList(GUID guid,LPCTSTR lpURL,LPCTSTR lpFileURLFormat,sHttpTranFileInfoArray &arrFileInfo,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//�����ļ�
	virtual HRESULT	DownLoadFile(GUID guid,LPCTSTR lpURL,LPCTSTR lpFilePath,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;

	//�õ������ļ���С
	virtual long	GetDownloadFileSize(GUID guid,LPCTSTR lpURL,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL) PURE;
};

extern "C" HRESULT WINAPI CreateHTTPClient(IHttpClient** ppHTTPClient);
//extern "C" HRESULT WINAPI DestoryHTTPClient();

#ifdef _DEBUG
#pragma comment(lib, "HTTPClientD.lib")
#else
#pragma comment(lib, "HTTPClient.lib")
#endif
