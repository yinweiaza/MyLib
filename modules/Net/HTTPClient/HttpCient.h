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
	GUID	clientId;	//����ID
	BOOL	bAuth;		//�Ƿ���Ȩ
	CString username;	//�û���
	CString password;   //����
	int		type;       //��������
	CString szURL;		//url��ַ
	CStringArray arrHeader;		//�����httpͷ
	int    dataType;   //�ϴ����������ͣ�0���ޣ�1����������2���ļ�
	LPBYTE  lpPost;		//��Ҫ���͵�����
	size_t  nPost;		//��Ҫ���͵����ݳ���
	CString inFilePath;//��Ҫ��ȡ���ļ�·��
	LONGLONG  uTotallen; //��Ҫ��ȡ���ļ����ܳ���
	LONGLONG  uWirtten;	//�ϴ�ʱ�Ѿ���ȡ���ļ�����
	BOOL     bSaveToFile;//�Ƿ��յ������ݱ��浽�ļ��������ļ�ʱ��Ҫ
	LONGLONG  uTotalDownloadlen; //��Ҫ���ص��ļ����ܳ���
	LONGLONG  uDownloadLen;	//�Ѿ����صĳ���
	std::string  rcvData;  //�յ��Ļظ�����
	long	headerLen;   //�ظ���������ͷ�ĳ��ȣ�
	CString OutFilePath;//������ļ�·��
	CString caPath;//httpsЭ��ʱ���֤��·��
	
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

	//֧�ֵĹ���
public:
	virtual HRESULT SetNotify(IHttpClientNotify* pNotify);
	virtual HRESULT Release();

	virtual HRESULT SetUserInfo(LPCTSTR lpUserName,LPCTSTR lpPassword);
	virtual HRESULT	CancelCmd(GUID guid);

	/*
	lpCaPath����Ĭ��Ϊ�գ� httpsЭ��ʱ���õ��ϣ����Ϊ�գ�����֤������֤��
	*/
	//get����
	virtual HRESULT Get(GUID guid,LPCTSTR lpURL,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);

	//post����
	virtual HRESULT Post(GUID guid,LPCTSTR lpURL,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath=NULL);

	virtual HRESULT Post(GUID guid,LPCTSTR lpURL, LPBYTE lpPost, size_t nPost,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);

	//put����
	virtual HRESULT Put(GUID guid,LPCTSTR lpURL,LPBYTE lpData, size_t nDataLen,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);

	//delete����
	virtual HRESULT DeleteReq(GUID guid,LPCTSTR lpURL,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);

	//�ϴ��ļ�
	virtual HRESULT	UploadFile(GUID guid,LPCTSTR lpURL,LPBYTE lpData, size_t nDataLen,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);
	
	virtual HRESULT	UploadFile(GUID guid,LPCTSTR lpURL,LPCTSTR lpFilePath,CStringArray& arrHeader,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);
	
	virtual	HRESULT UploadFileList(GUID guid,LPCTSTR lpURL,LPCTSTR lpFileURLFormat,sHttpTranFileInfoArray &arrFileInfo,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);

	//�����ļ�
	virtual HRESULT	DownLoadFile(GUID guid,LPCTSTR lpURL,LPCTSTR lpFilePath,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);
	
	//�õ������ļ���С
	virtual long	GetDownloadFileSize(GUID guid,LPCTSTR lpURL,BOOL bAuth=TRUE,LPCTSTR lpCaPath = NULL);

protected:
	//�̺߳���
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