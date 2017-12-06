#include "StdAfx.h"
#include ".\userthread.h"
//#include "utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT __ThreadProc(LPVOID lpParam)
{
	ASSERT(lpParam);
	CUserThread *pThread = (CUserThread*)lpParam;

	if(pThread->m_bInitCom)
		CoInitialize(NULL);

#ifdef DEBUG
	if(!pThread->m_strName.IsEmpty())
	{
		CString strMsg = _T("Thread \"");
		strMsg +=	pThread->m_strName;

		CString strId;
		if(pThread->m_pThread)
		{
			strId.Format(_T("%d"), pThread->m_pThread->m_nThreadID);

			strMsg +=	_T("(Id=");
			strMsg +=	strId;
			strMsg +=	_T(")\" ");
		}

		strMsg +=	_T("Is Created\n");

		TRACE(strMsg);
	}
#endif

	UINT uiRet =  pThread->ThreadProc();

	if(pThread->m_bInitCom)
		CoUninitialize();

#ifdef DEBUG
	if(!pThread->m_strName.IsEmpty())
	{
		CString strMsg = _T("Thread \"");
		strMsg +=	pThread->m_strName;
		strMsg +=	_T("\" Is Exited\n");

		TRACE(strMsg);
	}
#endif

	return uiRet;
}

CUserThread::CUserThread(void)
{
	m_pThread	= NULL;
	m_hExit		= NULL;
	m_bInitCom	= FALSE;
}

CUserThread::~CUserThread(void)
{
	End();
}

HRESULT CUserThread::Begin(BOOL bInitCom)
{
	if(m_pThread)
		return T9_S_FALSE;

	ASSERT(m_hExit == NULL && m_hExit == NULL);
	m_hExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(m_hExit == NULL)
		return T9E_NullPoint;


	m_bInitCom = bInitCom;
	m_pThread = AfxBeginThread(::__ThreadProc, LPVOID(this), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

	if(m_pThread == NULL)
		return T9E_NullPoint;
	else
	{
		m_pThread->m_bAutoDelete = FALSE;
		m_pThread->ResumeThread();
	}

	return NOERROR;
}

void CUserThread::End()
{
	if(m_pThread && m_hExit)
	{
		SetEvent(m_hExit);
// 		::PostThreadMessage(m_pThread->m_nThreadID, WM_QUIT, 0, 0);
		WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		delete m_pThread;
		m_pThread = NULL;

		CloseHandle(m_hExit);
		m_hExit = NULL;

	}
}

BOOL CUserThread::SetPriority(int nPriority)
{
	if(m_pThread)
		return m_pThread->SetThreadPriority(nPriority);

	return FALSE;
}

#ifdef DEBUG

CString	CUserThread::GetName()
{
	return m_strName;
}

void CUserThread::SetName(LPCTSTR lpName)
{
	m_strName = lpName;
}

#endif



