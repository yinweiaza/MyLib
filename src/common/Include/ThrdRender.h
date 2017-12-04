#pragma once

#ifdef NOUSE_THRDRENDER_IMPORT_EXPORT
#define IP_EP_THRDRENDER 
#else
#define IP_EP_THRDRENDER T9_EXT_CLASS
#endif

#include "Mmsystem.h"
#include "..\Player\Include\IRenderInnerCall.h"
#include ".\UserThread.h"


class IP_EP_THRDRENDER CThrdRender: public CUserThread
{
public:
	CThrdRender(IRenderInnerCall *pRenderInnerCall);
	virtual ~CThrdRender(void);

protected:
	IRenderInnerCall	*m_pRenderInnerCall;

	MMRESULT			m_TimerID;
	float				m_frameTime;	//million second
	HANDLE				m_hSemaphore;
	BOOL				m_bRealTime;

	LONG				m_preCount;

	LONG				m_notCallFrames;
	LONG				m_drop1Every;
	LONG				m_renderCount;

	__int64				m_period;
	__int64				m_timeInMs;
	__int64				m_frameCount;

public:
	virtual HRESULT		Init(float frameTime/*ms*/, BOOL bRealTime);
	void				OnTimerPulse();
	virtual	HRESULT		NeedDrop(int percent);


protected:
	virtual UINT		ThreadProc();
};
