#include "stdafx.h"
#include "ThrdHttpComm.h"

CThrdHttpComm::CThrdHttpComm()
{
	m_pHttpClient = NULL;
	m_pNotify = NULL;

	m_pCurl = NULL;  
}

CThrdHttpComm::~CThrdHttpComm()
{
	End();
}

void CThrdHttpComm::SetData(CHttpClient* pOwner,IHttpClientNotify* pNotify,sHttpRequest& httpReq)
{
	m_pHttpClient = pOwner;
	m_pNotify = pNotify;
	m_httpReq = httpReq;
}

UINT CThrdHttpComm::ThreadProc()
{
	HttpCommutication();
	return 0;
}

size_t CThrdHttpComm::ReceiveResponseData(void *buffer, size_t size, size_t nmemb, void *userp)
{
	CThrdHttpComm* pClient = (CThrdHttpComm*)userp;

	//��Ҫע�⣬һ��http������ܻ�ִ�ж�λص�,�����������洦�����ݣ���ֻ�ǲ������ݡ�
	//�������Ȼ���������
	if (NULL != pClient)
	{
		//���浽�ڲ������ļ�
		if (pClient->m_httpReq.bSaveToFile)
		{
			CString filepath = pClient->m_httpReq.OutFilePath;
			CFile file;
			if(file.Open(filepath,CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite,NULL))
			{
				file.SeekToEnd();
				file.Write((char*)buffer, size*nmemb);
				pClient->m_httpReq.uTotalDownloadlen += size*nmemb;
				file.Close();
			}
		}
		else
		{
			pClient->m_httpReq.rcvData.append((char*)buffer, size*nmemb);
		}
	}
	return size*nmemb;
}

size_t CThrdHttpComm::ReadSendData(void *buffer, size_t size, size_t nmemb, void *userp)
{
	ULONGLONG nSize=0;
	ULONGLONG nTotalSize = 0;

	CThrdHttpComm* pClient = (CThrdHttpComm*)userp;

	if (NULL != pClient)
	{
		//���ļ���ȡ���Ǵӻ�����
		if (pClient->m_httpReq.dataType == 2)
		{
			CString filepath = pClient->m_httpReq.inFilePath;
			CFile file;
			if(file.Open(filepath,CFile::modeRead | CFile::shareDenyNone,NULL))
			{
				file.Seek(pClient->m_httpReq.uWirtten ,0);
				nSize = file.Read(buffer, size*nmemb);
				file.Close();

				pClient->m_httpReq.uWirtten += nSize;
			}
		}
	}

	return (size_t)nSize;
}

int	CThrdHttpComm::OnProgress(void *pClient, curl_off_t dltotal, curl_off_t dlnow,curl_off_t ultotal, curl_off_t ulnow)
{
	CThrdHttpComm* pComm = (CThrdHttpComm *)pClient;
	if(pComm && pComm->m_pNotify)
	{
		if (pComm->m_httpReq.dataType == 2)//���ļ���ȡ����Ϊ�ļ��ϴ��Ľ���
		{
			pComm->m_pNotify->OnProgress(pComm->m_httpReq.clientId,pComm->m_httpReq.uTotallen, pComm->m_httpReq.uWirtten);
		}
		else if (pComm->m_httpReq.bSaveToFile)//���浽�ļ�����Ϊ���صĽ���
		{
			pComm->m_pNotify->OnProgress(pComm->m_httpReq.clientId,pComm->m_httpReq.uTotalDownloadlen,pComm->m_httpReq.uDownloadLen);
		}
	}
	return 0;
}

void CThrdHttpComm::HttpCommutication()
{
	char* pURL = CTextConversion::TCHARToUTF8(m_httpReq.szURL);
	CURLcode res;

	int cnt = m_httpReq.arrHeader.GetCount();

	struct curl_slist *http_header = NULL;
	CList<char*, char*> headerList;
	if (cnt >0)
	{
		//httpͷ�����������
		for (int i=0;i<cnt;i++)
		{
			CString header = m_httpReq.arrHeader.GetAt(i);
			char* pheader = CTextConversion::TCHARToUTF8(header);
			http_header = curl_slist_append(http_header, pheader);
			headerList.AddTail(pheader);
		}
	}

	m_pCurl = curl_easy_init();  
	curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, http_header);
	curl_easy_setopt(m_pCurl, CURLOPT_URL, pURL); 
	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);  

	//��֤��ʽ,ֻ֧�ֲ���֤��basic��֤
	char* pData = NULL;
	if(m_httpReq.bAuth)
	{
		CString szData;
		szData.Format(_T("%s:%s"),m_httpReq.username,m_httpReq.password);
		pData = CTextConversion::TCHARToUTF8(szData);
		curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, pData); //�����û���������
		curl_easy_setopt(m_pCurl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);//������֤��ʽΪbasic
	}
	else
	{
		curl_easy_setopt(m_pCurl, CURLOPT_HTTPAUTH, CURLAUTH_NONE);
	}

	if (m_httpReq.bSaveToFile)
	{
		curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 0);  //���ص��������Ƿ����header��0��ʾ��������1��ʾ����
	}
	else
	{
		curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 1);  //���ص��������Ƿ����header��0��ʾ��������1��ʾ����
	}
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, ReceiveResponseData);  //�����յ�����ʱ�Ļص�����
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this); //���ûص������еĵ��ĸ�����

	switch(m_httpReq.type)
	{
	case en_Get:
		curl_easy_setopt(m_pCurl, CURLOPT_POST,0);  //����ΪGet����
		break;
	case en_Post:
		curl_easy_setopt(m_pCurl,CURLOPT_POST,1);  //����ΪPost����
		break;
	case en_Put:
		curl_easy_setopt(m_pCurl, CURLOPT_CUSTOMREQUEST,"PUT");  //����ΪPut����
		break;
	case en_Delete:
		curl_easy_setopt(m_pCurl, CURLOPT_CUSTOMREQUEST,"DELETE");  //����ΪDELETE����
		break;
	case en_Upload:
		curl_easy_setopt(m_pCurl,CURLOPT_CUSTOMREQUEST,"POST");  //����Ϊ�Զ����Post���󣬲�֪����en_Post�����𣬵����ϴ��ӱ��ض�ȡ�ļ�ʱ�����������
		break;
	default:
		break;
	}

	if (1 ==m_httpReq.dataType)//���ͻ���������
	{
		curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, m_httpReq.lpPost);//��Ҫ���͵�json����
		curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, m_httpReq.nPost);//���ݳ���
	}
	else if(2 ==m_httpReq.dataType)//���ļ���ȡ
	{
		CFile file;
		ULONGLONG nSize = 0;
		DWORD err =0;
		if(file.Open(m_httpReq.inFilePath,CFile::modeRead| CFile::shareDenyNone,NULL))
		{
			nSize = file.GetLength();
			m_httpReq.uTotallen = nSize;
			file.Close();
		}
		else
		{
			err= GetLastError();
		}
		curl_easy_setopt(m_pCurl, CURLOPT_INFILESIZE_LARGE,nSize);//���߷��������ļ��ߴ�
		curl_easy_setopt(m_pCurl, CURLOPT_UPLOAD, 1);//���߷�����ִ�е����ϴ�����
		curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, ReadSendData);  //���÷�������ʱ�Ļص�����
		curl_easy_setopt(m_pCurl, CURLOPT_READDATA, this); //���ûص������еĵ��ĸ�����
	}

	if (m_httpReq.bSaveToFile)//�����ļ�
	{
		m_httpReq.uTotalDownloadlen = GetDownloadFileSize(pURL);
	}

	curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1);  //����쳣��Ϣ

	if (m_httpReq.caPath.IsEmpty())
	{
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0);//�Ƿ����������֤���Ƿ��������������֤������ȨCA�䷢��
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0);//�Ƿ����������������֤���ϵ��Ƿ�һ��
	}
	else//У�������
	{
		//δ���
	}

	//��ʾ���������
	curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(m_pCurl, CURLOPT_XFERINFOFUNCTION, OnProgress);
	curl_easy_setopt(m_pCurl, CURLOPT_XFERINFODATA, this);

	res = curl_easy_perform(m_pCurl); 
	long retcode = -1;
	if (CURLE_OK == res)
	{
		curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &retcode);
		if (!m_httpReq.bSaveToFile && m_httpReq.rcvData.length() >0)
		{
			curl_easy_getinfo(m_pCurl, CURLINFO_HEADER_SIZE, &m_httpReq.headerLen);
		}
	}

	curl_easy_cleanup(m_pCurl);  

	//last free the header list
	curl_slist_free_all(http_header); /* free the header list */

	if (pData)
	{
		delete []pData;
		pData = NULL;
	}

	delete []pURL;
	pURL = NULL;

	DeleteList(headerList);

	if (m_pNotify)
	{
		//֪ͨ
		sHttpAnawer* pAnswer = new sHttpAnawer();
		pAnswer->clientId = m_httpReq.clientId;
		pAnswer->res = res;
		pAnswer->retcode = retcode;
		if (m_httpReq.headerLen >0 )
		{
			std::string headerData=m_httpReq.rcvData.substr(0,m_httpReq.headerLen);
			pAnswer->nHeaderLen = headerData.length()+1;
			if (pAnswer->nHeaderLen)
			{
				pAnswer->lpHeaderbuf = new BYTE[pAnswer->nHeaderLen];
				ZeroMemory(pAnswer->lpHeaderbuf,pAnswer->nHeaderLen);
				memcpy(pAnswer->lpHeaderbuf,headerData.c_str(),pAnswer->nHeaderLen-1);
			}
		}

		std::string bodyData=m_httpReq.rcvData.substr(m_httpReq.headerLen);
		pAnswer->nBodyLen = bodyData.length()+1;
		if (pAnswer->nBodyLen)
		{
			pAnswer->lpBodybuf = new BYTE[pAnswer->nBodyLen];
			ZeroMemory(pAnswer->lpBodybuf,pAnswer->nBodyLen);
			memcpy(pAnswer->lpBodybuf,bodyData.c_str(),pAnswer->nBodyLen-1);
		}

		m_pNotify->OnAnswer(pAnswer);

		if(pAnswer->lpHeaderbuf)
		{
			delete [] pAnswer->lpHeaderbuf;
			pAnswer->lpHeaderbuf = NULL;
		}

		if(pAnswer->lpBodybuf)
		{
			delete [] pAnswer->lpBodybuf;
			pAnswer->lpBodybuf = NULL;
		}
		delete pAnswer;
		pAnswer = NULL;
	}

	if (m_pHttpClient)
	{
		m_pHttpClient->Http_end(m_httpReq.clientId);
	}
}

void CThrdHttpComm::End()
{
	if(m_pThread && m_hExit)
	{
		SetEvent(m_hExit);
		// 		::PostThreadMessage(m_pThread->m_nThreadID, WM_QUIT, 0, 0);
		DWORD dwRet= WaitForSingleObject(m_pThread->m_hThread, 500);
		if(dwRet==WAIT_TIMEOUT)
		{
			if(m_pThread)
				TerminateThread(m_pThread->m_hThread, 0);
		}

		delete m_pThread;
		m_pThread = NULL;

		CloseHandle(m_hExit);
		m_hExit = NULL;
	}
}

long CThrdHttpComm::GetDownloadFileSize(const char* strUrl)
{
	if(NULL == strUrl)
		return -1;

	CURL* curl = curl_easy_init();
	if(NULL == curl)
		return -1;

	curl_easy_setopt(curl, CURLOPT_URL, strUrl);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);

	if (m_httpReq.caPath.IsEmpty())
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);//�Ƿ����������֤���Ƿ��������������֤������ȨCA�䷢��
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);//�Ƿ����������������֤���ϵ��Ƿ�һ��
	}
	else
	{
		;
	}

	CURLcode res = curl_easy_perform(curl);
	if(res == CURLE_OK)
	{
		double sz = 0;
		res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &sz);
		curl_easy_cleanup(curl);

		return (long)sz;
	}
	else
	{
		TRACE(curl_easy_strerror(res));
		TRACE("\n");
	}

	curl_easy_cleanup(curl);

	return 0;
}