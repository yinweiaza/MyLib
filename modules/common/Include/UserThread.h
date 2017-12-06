#pragma once

#ifdef NOUSE_USERTHREAD_IMPORT_EXPORT
	#define IP_EP_USERTHREAD 
#else
	#define IP_EP_USERTHREAD T9_EXT_CLASS
#endif

class IP_EP_USERTHREAD CUserThread
{
	friend	 UINT __ThreadProc(LPVOID lpParam);

public:
	CUserThread(void);
	virtual ~CUserThread(void);
	CWinThread	*m_pThread;   //ledMonitor中需要获取线程句柄用来同步
protected:

	HANDLE		m_hExit;
	BOOL		m_bInitCom;

#ifdef DEBUG
	CString		m_strName;
#endif
	
protected:
	virtual UINT	ThreadProc()=0;

public:
	virtual HRESULT	Begin(BOOL bInitCom = FALSE);
	virtual void	End();

	BOOL			SetPriority(int nPriority);

#ifdef DEBUG
	CString			GetName();
	void			SetName(LPCTSTR lpName);
#endif
};