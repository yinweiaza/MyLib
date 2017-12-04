#include "stdafx.h"
#include "ThrdHttpTransfer.h"

CThrdHttpTransfer::CThrdHttpTransfer()
{
	m_pHttpClient = NULL;
	m_ClientID    = GUID_NULL;
	m_pNotify = NULL;

	m_pCurl = NULL;  

	m_hUploadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_nTotalSize = 0;
	m_curTaskIndex	=-1;
}

CThrdHttpTransfer::~CThrdHttpTransfer()
{
	End();

	DeleteArray(m_arrTransTask);	
	if(m_hUploadEvent)
	{
		CloseHandle(m_hUploadEvent);
		m_hUploadEvent = NULL;
	}
}

void CThrdHttpTransfer::SetData(CHttpClient* pOwner,IHttpClientNotify* pNotify,GUID clientID,CHttpTransTaskArray& arrTransTasks)
{
	m_pHttpClient = pOwner;
	m_pNotify = pNotify;
	m_ClientID =clientID;
	
	m_nTotalSize = 0;
	CSingleLock sLock(&m_mutexTask, TRUE);
	int fileCnt = arrTransTasks.GetCount();
	for (int i=0; i<fileCnt; i++)
	{
		sHttpTransTask* pItem = new sHttpTransTask((*arrTransTasks[i]));
		m_nTotalSize += pItem->totalSize;
		m_arrTransTask.Add(pItem);
	}
}

UINT CThrdHttpTransfer::ThreadProc()
{
	HANDLE hEvents[2];

	hEvents[0] = m_hExit;
	hEvents[1] = m_hUploadEvent;

	SetEvent(hEvents[1]);

	DWORD dwResult = 0;
	while (TRUE)
	{
		dwResult = WaitForMultipleObjects(sizeof(hEvents)/sizeof(HANDLE),hEvents, FALSE, INFINITE);

		if (dwResult == WAIT_OBJECT_0) //�˳�
		{
			break;
		}
		else if (dwResult == WAIT_OBJECT_0+1) //�ϴ�
		{
			sHttpTransTask* pTask = GetUploadTask();
			if (pTask != NULL)
			{
				if (pTask->taskType==enTask_Upload)
				{
					BOOL bExist = FALSE;
					HRESULT hr = IsFileRemoteExist(pTask,bExist);
					if (FAILED(hr))
						break;

					if (bExist)
					{
						pTask->state = en_http_end;
						pTask->finishSize = pTask->totalSize;
						OnTransfer(0,0,pTask->totalSize,pTask->finishSize);
					}
					else
					{
						if (FAILED(DoUpload(pTask)))
							break;
					}
				}
			}		
			m_curTaskIndex = -1;

			if (WaitForSingleObject(m_hExit, 0)==WAIT_OBJECT_0)
				break;

			if (HasWaitTask())
			{
				Sleep(1000);
				SetEvent(hEvents[1]);
			}
			else
			{
				OnEnd(CURLE_OK,201);
				break;
			}
		}
		else 
		{
			break;
		}
	}
	
	if (m_pHttpClient)
	{
		m_pHttpClient->Http_end(m_ClientID);
	}

	return 0;
}

size_t CThrdHttpTransfer::ReceiveResponseData(void *buffer, size_t size, size_t nmemb, void *userp)
{
	CThrdHttpTransfer* pClient = (CThrdHttpTransfer*)userp;
	//��Ҫע�⣬һ��http������ܻ�ִ�ж�λص�,�����������洦�����ݣ���ֻ�ǲ������ݡ�
	//�������Ȼ���������
	if (NULL != pClient)
	{
		pClient->m_rcvData.append((char*)buffer, size*nmemb);
	}
	return size*nmemb;
}

int	CThrdHttpTransfer::OnProgress(void *pClient,  curl_off_t dltotal, curl_off_t dlnow,curl_off_t ultotal, curl_off_t ulnow)
{
	CThrdHttpTransfer* pTransfer = (CThrdHttpTransfer *)pClient;
	if(pTransfer)
	{
		pTransfer->OnTransfer(dltotal,dlnow,ultotal,ulnow);
	}
	return 0;
}

HRESULT	CThrdHttpTransfer::IsFileRemoteExist(sHttpTransTask* pTask,BOOL& bExist)
{
	HRESULT hr = T9_E_FAIL;
	bExist = FALSE;
	m_rcvData.clear();
	CString szUrl;
	CString nameTitle = pTask->destTitle;
	//���ļ�������URL encoded
	CHAR buf[1024] = {0};
	UrlEncodeEx(nameTitle,buf,1024);
	nameTitle = buf;
	szUrl.Format(pTask->fileUrlFormat,nameTitle);
	char* pURL = CTextConversion::TCHARToUTF8(szUrl);
	CURLcode res;

	m_pCurl = curl_easy_init();  
	//curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, http_header);
	curl_easy_setopt(m_pCurl, CURLOPT_URL, pURL); 
	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);  

	//��֤��ʽ,ֻ֧�ֲ���֤��basic��֤
	char* pData = NULL;
	if(pTask->bAuth)
	{
		CString szData;
		szData.Format(_T("%s:%s"),pTask->username,pTask->password);
		char* pData = CTextConversion::TCHARToUTF8(szData);
		curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, pData); //�����û���������
		curl_easy_setopt(m_pCurl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);//������֤��ʽΪbasic
	}
	else
	{
		curl_easy_setopt(m_pCurl, CURLOPT_HTTPAUTH, CURLAUTH_NONE);
	}
	
	curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 0);  //���ص��������Ƿ����header��0��ʾ��������1��ʾ����
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, ReceiveResponseData);  //�����յ�����ʱ�Ļص�����
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this); //���ûص������еĵ��ĸ�����
	curl_easy_setopt(m_pCurl, CURLOPT_POST,0);  //����ΪGet����
	curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1);  //����쳣��Ϣ

	if (pTask->caPath.IsEmpty())
	{
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0);//�Ƿ����������֤���Ƿ��������������֤������ȨCA�䷢��
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0);//�Ƿ����������������֤���ϵ��Ƿ�һ��
	}
	else//У�������
	{
		//δ���
	}

	res = curl_easy_perform(m_pCurl); 
	long retcode = -1;
	if (CURLE_OK == res)
	{
		curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &retcode);
	}

	curl_easy_cleanup(m_pCurl);  

	if (pData)
	{
		delete []pData;
		pData = NULL;
	}

	delete []pURL;
	pURL = NULL;

	if (CURLE_OK ==res && 200 == retcode )//��ѯ�ɹ�
	{
		hr = T9_S_OK;

		if (m_pNotify)
		{
			//֪ͨ�ϲ�������,����������ļ����ڲ�����
			m_pNotify->OnFileExist(m_ClientID,(LPBYTE)m_rcvData.c_str(),m_rcvData.length(),bExist);
		}
	}

	m_rcvData.clear();

	return hr;
}

HRESULT CThrdHttpTransfer::DoUpload(sHttpTransTask* pTask)
{
	HRESULT hr = T9_E_FAIL;
	m_rcvData.clear();
	CString szUrl;
	CString nameTitle = pTask->sourceName;
	//ȥ����׺��
	nameTitle = nameTitle.Left(nameTitle.ReverseFind(_T('.')));
	//���ļ�������URL encoded
	CHAR buf[1024] = {0};
	UrlEncodeEx(nameTitle,buf,1024);
	nameTitle = buf;
	szUrl.Format(pTask->remoteUrl,nameTitle);
	char* pURL = CTextConversion::TCHARToUTF8(szUrl);
	CURLcode res;

	m_pCurl = curl_easy_init();  
	
	//��֤��ʽ,ֻ֧�ֲ���֤��basic��֤
	char* pData = NULL;
	if(pTask->bAuth)
	{
		CString szData;
		szData.Format(_T("%s:%s"),pTask->username,pTask->password);
		pData = CTextConversion::TCHARToUTF8(szData);
		curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, pData); //�����û���������
		curl_easy_setopt(m_pCurl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);//������֤��ʽΪbasic
	}
	else
	{
		curl_easy_setopt(m_pCurl, CURLOPT_HTTPAUTH, CURLAUTH_NONE);
	}

	switch(pTask->taskType)
	{
	case enTask_Download:
		curl_easy_setopt(m_pCurl, CURLOPT_POST,0);  //����ΪGet����
		break;
	case enTask_Upload:
		curl_easy_setopt(m_pCurl,CURLOPT_CUSTOMREQUEST,"POST");  //����Ϊ�Զ����Post���󣬲�֪����en_Post�����𣬵����ϴ��ӱ��ض�ȡ�ļ�ʱ�����������
		break;
	default:
		break;
	}
	curl_easy_setopt(m_pCurl, CURLOPT_URL, pURL); 

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "content-type: multipart/form-data");
	curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, headers);

	struct curl_httppost *post=NULL;  
	struct curl_httppost *last=NULL;  

	const char * remoteNewFileKey = CTextConversion::TCHARToUTF8(pTask->destName);
	const char *localFile = WCHARToACP(pTask->filePath);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "file", CURLFORM_FILE, localFile,CURLFORM_FILENAME,remoteNewFileKey,CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "submit", CURLFORM_COPYCONTENTS, "send",CURLFORM_END);
	curl_easy_setopt(m_pCurl, CURLOPT_HTTPPOST, post);

	curl_easy_setopt(m_pCurl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)pTask->totalSize); // upload file size--- Content-Length: size

	curl_easy_setopt(m_pCurl, CURLOPT_MAX_SEND_SPEED_LARGE, 20*1000);    // speed limit

	curl_easy_setopt(m_pCurl, CURLOPT_HEADER, 0);  //���ص��������Ƿ����header��0��ʾ��������1��ʾ����
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, ReceiveResponseData);  //�����յ�����ʱ�Ļص�����
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this); //���ûص������еĵ��ĸ�����

	curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1);  //����쳣��Ϣ

	if (pTask->caPath.IsEmpty())
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
	}
	else
	{
		TRACE(curl_easy_strerror(res));
		TRACE("\n");
	}

	//last free the header list
	curl_slist_free_all(headers); /* free the header list */

	/* then cleanup the formpost chain */
	curl_formfree((curl_httppost*)post);

	curl_easy_cleanup(m_pCurl);  

	if (pData)
	{
		delete []pData;
		pData = NULL;
	}

	if (remoteNewFileKey)
	{
		delete []remoteNewFileKey;
		remoteNewFileKey = NULL;
	}
	if (localFile)
	{
		delete []localFile;
		localFile = NULL;
	}

	delete []pURL;
	pURL = NULL;

	if (CURLE_OK ==res && 201 ==retcode)//�ϴ��ɹ�
	{
		hr = T9_S_OK;
		pTask->state = en_http_end;
	}
	else
	{
		pTask->state = en_http_break;
		OnEnd(res,retcode);
	}

	m_rcvData.clear();
	return hr;
}

void CThrdHttpTransfer::End()
{
	if(m_pThread && m_hExit)
	{
		SetEvent(m_hExit);
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

sHttpTransTask*	CThrdHttpTransfer::GetUploadTask()
{
	CSingleLock sLock(&m_mutexTask, TRUE);

	m_curTaskIndex = -1;
	sHttpTransTask* pTask = NULL;
	int count = m_arrTransTask.GetCount();
	for (int i=0; i<count; i++)
	{
		sHttpTransTask* p = m_arrTransTask[i];
		if (p->state == en_http_prepared)
		{
			pTask = p;
			m_curTaskIndex = i;
			break;
		}
	}

	if (pTask != NULL)
	{
		pTask->state = en_http_translate;
		return pTask;
	}

	return NULL;
}

BOOL CThrdHttpTransfer::HasWaitTask()
{
	CSingleLock sLock(&m_mutexTask, TRUE);
	int count = m_arrTransTask.GetCount();
	for (int i=0; i<count; i++)
	{
		sHttpTransTask* p = m_arrTransTask[i];
		if (p->state == en_http_prepared)
			return TRUE;
	}

	return FALSE;
}

HRESULT CThrdHttpTransfer::GetUplodheader(sHttpTransTask* pTask,CStringArray& arrHeader)
{
	if (pTask == NULL)
		return T9E_ParamError;

	arrHeader.RemoveAll();

	CString filePath = pTask->filePath;
	if (filePath.IsEmpty() || !IsFileExist(filePath))
		return T9E_ParamError;

	CString szSuffix = pTask->fileExtension;
	szSuffix.MakeLower();

	if(0 == szSuffix.Compare(_T(".vsn")))//xml��ʽ
	{
		arrHeader.Add(_T("Content-Type:application/xml"));
	}
	else if(0 == szSuffix.Compare(_T(".avi")))
	{
		arrHeader.Add(_T("Content-Type:video/x-msvideo"));
	}
	else if(0 == szSuffix.Compare(_T(".rmvb")))
	{
		arrHeader.Add(_T("Content-Type:video/vnd.rn-realvideo"));
	} 
	else if(0 == szSuffix.Compare(_T(".rm")))
	{
		arrHeader.Add(_T("Content-Type:audio/x-pn-realaudio")); 
	}
	else if(0 == szSuffix.Compare(_T(".wmv")))
	{
		arrHeader.Add(_T("Content-Type:audio/x-ms-wmv"));
	} 
	else if(0 == szSuffix.Compare(_T(".asf")))
	{
		arrHeader.Add(_T("Content-Type:video/x-ms-asf")); 
	}
	else if( 0 == szSuffix.Compare(_T(".mpeg"))
		||0 == szSuffix.Compare(_T(".mpg"))
		||0 == szSuffix.Compare(_T(".mpe")))
	{
		arrHeader.Add(_T("Content-Type:video/mpeg"));
	}
	else if(0 == szSuffix.Compare(_T(".mp4")))
	{
		arrHeader.Add(_T("Content-Type:video/mp4")); 
	}
	else if( 0 == szSuffix.Compare(_T(".qt"))
		||0 == szSuffix.Compare(_T(".mov")))
	{
		arrHeader.Add(_T("Content-Type:video/quicktime"));
	}
	else if(0 == szSuffix.Compare(_T(".3gp")))
	{
		arrHeader.Add(_T("Content-Type:video/3gpp")); 
	}
	else if(0 == szSuffix.Compare(_T(".bmp")))
	{
		arrHeader.Add(_T("Content-Type:application/x-MS-bmp")); 
	}else if(0 == szSuffix.Compare(_T(".png")))
	{
		arrHeader.Add(_T("Content-Type:image/png")); 
	}
	else if( 0 == szSuffix.Compare(_T(".jpeg"))
		||0 == szSuffix.Compare(_T(".jpg"))
		||0 == szSuffix.Compare(_T(".jpe")))
	{
		arrHeader.Add(_T("Content-Type:image/jpeg"));
	}
	else if(0 == szSuffix.Compare(_T(".tiff")))
	{
		arrHeader.Add(_T("Content-Type:image/tiff"));
	}
	else if(0 == szSuffix.Compare(_T(".gif")))
	{
		arrHeader.Add(_T("Content-Type:image/gif"));
	}
	else if( 0 == szSuffix.Compare(_T(".swf"))
		||0 == szSuffix.Compare(_T("cab")))
	{
		arrHeader.Add(_T("Content-Type:application/x-shockwave-flash"));
	}
	else if(0 == szSuffix.Compare(_T(".zip")))
	{
		arrHeader.Add(_T("Content-Type:application/zip"));
	}
	else//Ĭ��Ϊxml
	{
		arrHeader.Add(_T("Content-Type:application/xml"));
	}

	arrHeader.Add(_T("Content-Disposition:attachment;filename=") + pTask->destName);
	arrHeader.Add(pTask->szMD5);
	return T9_S_OK;
}

void CThrdHttpTransfer::OnTransfer(curl_off_t dltotal, curl_off_t dlnow,curl_off_t ultotal, curl_off_t ulnow)
{
	//���������ϴ����ļ��Ľ���
	if(m_curTaskIndex >=0 || m_curTaskIndex< m_arrTransTask.GetCount())
	{
		float progress = 0.00f;
		if (ultotal >0)
		{
			progress = (float)ulnow/ultotal;
		}

		m_arrTransTask[m_curTaskIndex]->finishSize = Round(m_arrTransTask[m_curTaskIndex]->totalSize*progress);
	}
	//�����Ѿ��ϴ��Ĵ�С
	CSingleLock sLock(&m_mutexTask, TRUE);
	__int64	nFinishSize = 0;
	int count = 0;
	for (int i=0; i<m_arrTransTask.GetCount(); i++)
	{
		nFinishSize += m_arrTransTask[i]->finishSize;
	}
	sLock.Unlock();

	if(m_pNotify)
	{
		 m_pNotify->OnProgress(m_ClientID, m_nTotalSize, nFinishSize);	
	}
}

void CThrdHttpTransfer::OnEnd(CURLcode res,long retcode)
{
	if (m_pNotify)
	{
		//֪ͨ
		sHttpAnawer* pAnswer = new sHttpAnawer();
		pAnswer->clientId = m_ClientID;
		pAnswer->res = res;
		pAnswer->retcode = retcode;
		m_pNotify->OnAnswer(pAnswer);
	}
}

char*	 CThrdHttpTransfer::WCHARToACP(const wchar_t* str)
{
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, NULL, NULL, NULL); 

	char * pBuf = new char[nLen + 1]; 
	ZeroMemory(pBuf, nLen + 1); 

	::WideCharToMultiByte(CP_ACP, 0, str, wcslen(str), pBuf, nLen, NULL, NULL); 

	return pBuf; 
}
