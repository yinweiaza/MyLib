#include "StdAfx.h"
#include "ThrdRender.h"
#pragma comment(lib, "Winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SEMAPHORE_INIT_COUNT	1000

void CALLBACK RenderTimerCB(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	if(dwUser)
		((CThrdRender*)(dwUser))->OnTimerPulse();
}


CThrdRender::CThrdRender(IRenderInnerCall *pRenderInnerCall)
{
	ASSERT(pRenderInnerCall);

	m_hSemaphore		= NULL;
	m_pRenderInnerCall	= pRenderInnerCall;

	m_TimerID			= 0;
	m_frameTime			= 40;
	m_bRealTime			= TRUE;
	m_hSemaphore		= NULL;

	m_preCount			= 0;
	m_renderCount		= 0;

	m_drop1Every		= INT_MAX;

	m_period			= 0;
	m_timeInMs			= 0;
	m_frameCount		= 0;
}

CThrdRender::~CThrdRender(void)
{
	End();
}

HRESULT	CThrdRender::Init(float frameTime, BOOL bRealTime)
{
	m_frameTime = frameTime;
	m_bRealTime	= bRealTime;
	m_renderCount=0;

	return T9_NOERROR;
}

void CThrdRender::OnTimerPulse()
{
	m_timeInMs += m_period;
	if(m_timeInMs<Round(m_frameTime*m_frameCount))
		return;

	m_frameCount++;

	LONG preCount = 0;
	if(ReleaseSemaphore(m_hSemaphore, 1, &preCount))
	{
		InterlockedExchange(&m_preCount, preCount);
	}
	else
	{
		//CPU严重透支
		InterlockedExchange(&m_preCount, preCount);
	}
}

UINT CThrdRender::ThreadProc()
{
	TIMECAPS timeCaps;
	if(m_bRealTime)
	{
		if(timeGetDevCaps(&timeCaps,sizeof(TIMECAPS)) != TIMERR_NOERROR)
			return 0;

		m_hSemaphore = CreateSemaphore(NULL, 0, SEMAPHORE_INIT_COUNT, NULL);
		if(m_hSemaphore == NULL)
			return 0;

		m_period	= timeCaps.wPeriodMin;
// 		if(m_period<4)
// 			m_period = 4;

		m_timeInMs	= -Round(m_frameTime);
		m_frameCount= 0;
		::timeBeginPeriod(Round(m_period));									// set timer resolution
		m_TimerID = timeSetEvent(Round(m_period),
								Round(m_period),
								(LPTIMECALLBACK)RenderTimerCB,
								(DWORD)this,
								TIME_PERIODIC);

	}


	HANDLE handle[2];
	handle[0] = m_hExit;
	handle[1] = m_hSemaphore;
	DWORD dwWait = 0;
	BOOL bNeedSkip = FALSE;

	while (TRUE)
	{
		if(m_bRealTime)
		{
			dwWait = WaitForMultipleObjects(2, handle, FALSE, INFINITE);
			if(dwWait != WAIT_OBJECT_0+1)
			{
				break;
			}
		}
		else
		{
			dwWait = WaitForSingleObject(m_hExit, 2);

			if(dwWait != WAIT_TIMEOUT)
			{
				break;
			}
		}

//		TRACE(_T("CThrdRender::ThreadProc() 0 \n"));

		LONG preCount = 0;
		InterlockedExchange(&preCount, m_preCount);

		InterlockedIncrement(&m_notCallFrames);

		if(m_notCallFrames>=8 && m_drop1Every != INT_MAX)
		{
			LONG count = Round(m_drop1Every*1.05);
			if(count == m_drop1Every)
				count++;

			if(count>200)
				count = INT_MAX;

			InterlockedExchange(&m_drop1Every, count);
			InterlockedExchange(&m_notCallFrames, 0);
		}

		if(preCount>2)
		{
			//skip 测率
			bNeedSkip = TRUE;
		}
		else
		{
			int left = m_renderCount%m_drop1Every;
			if(left == m_drop1Every-1 || left == m_drop1Every/2)
			{
				bNeedSkip = TRUE;
			}
			else
			{
				bNeedSkip = FALSE;
			}

			m_renderCount++;
		}



// 		TRACE(_T("preCount = %d\n"), preCount);

//		TRACE(_T("CThrdRender::ThreadProc() 1 \n"));

		if(bNeedSkip && m_bRealTime)
		{
			m_pRenderInnerCall->Skip();
		}
		else
		{
			m_pRenderInnerCall->Render();
		}

//		TRACE(_T("CThrdRender::ThreadProc() 2 \n"));
	}

//	TRACE(_T("CThrdRender::ThreadProc() 3 \n"));

	if(m_TimerID)
	{
		timeKillEvent(m_TimerID);
		timeEndPeriod(timeCaps.wPeriodMin);
		m_TimerID	= NULL;
	}

//	TRACE(_T("CThrdRender::ThreadProc() 4 \n"));

	if(m_hSemaphore)
	{
		CloseHandle(m_hSemaphore);
		m_hSemaphore = NULL;
	}



//	TRACE(_T("CThrdRender::ThreadProc() 5 \n"));


	return 0;
}

HRESULT	CThrdRender::NeedDrop(int percent)
{
	if(percent == 0)
		return T9_NOERROR;

	LONG count = 0;
	InterlockedExchange(&m_notCallFrames, count);

	if(m_drop1Every==INT_MAX)
	{
		count = 200;
		InterlockedExchange(&m_drop1Every, count);
	}
	else
	{
		count = m_drop1Every;
		if(count>3)
		{
			count = Round(count*0.96);
			if(count<3)
				count = 3;

			InterlockedExchange(&m_drop1Every, count);
		}
	}

	return T9_NOERROR;
}
