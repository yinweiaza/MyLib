#pragma once

// CThrdTransfer
#include "FtpxLibImp.h"

extern UINT __ThreadProc(LPVOID lpParam);
class CThrdTransfer
{
	friend UINT __ThreadProc(LPVOID lpParam);
	friend class CFtpxLib;
public:
	CThrdTransfer();      
	virtual ~CThrdTransfer();

	void		SetOwner(CFtpxLib* pOwner);

	HRESULT		Init(BOOL bInitCom = FALSE);
	void		UnInit(BOOL bWaitExit = FALSE);

	//文件传输
	HRESULT		Upload(LPCTSTR lpTaskId, LPCTSTR lpLocalFile, LPCTSTR lpRemoteUrl, __int64 nOffset, int nPriority,BOOL bCoverSame);
	HRESULT		Download(LPCTSTR lpTaskId, LPCTSTR lpLocalFile, LPCTSTR lpRemoteUrl, __int64 nOffset, int nPriority);
	HRESULT		Begin(LPCTSTR lpTaskId);
	HRESULT		Pause(LPCTSTR lpTaskId);
	HRESULT		Remove(LPCTSTR lpTaskId);
	HRESULT		SetPriority(LPCTSTR lpTaskId, int nPriority);
	HRESULT     PauseTrans();
	HRESULT		ResumeTrans();

protected:
	UINT		ThreadProc();

	GUID		m_dwTaskId;
	void		SetTaskId(const GUID& dwTaskId);
	GUID&		GetTaskId();
	
	CFtpxLib*	m_pFtpxLib;
	HANDLE		m_hExit;
	BOOL		m_bInitCom;
	CCriticalSection m_csAccess;

	//<CWinThread*,事件句柄>
	CMap<CWinThread*,CWinThread*,HANDLE,HANDLE>m_mapThrdEvent;
	CTransTaskArray	m_transTasks;

	HANDLE		GetThreadEvent(DWORD dwThreadId);
	int			GetWaitTaskCount();
	BOOL		HasWaitTask();	
	STransTask*	GetFirstTask(TaskState curState, TaskState newState);
	TaskState	DoUpload(STransTask* pTask);
	TaskState	DoDownload(STransTask* pTask);
	void		SetTaskState(STransTask* pTask, TaskState state, BOOL bNotify=FALSE);

	STransTask* FindTransTask(LPCTSTR lpTaskId);

	BOOL		IsContinue(STransTask* pTask);
	BOOL		IsExitThread();

	CStringA	ConvToNetwork(CStringW& str);

	BOOL		GetMD5FromServerFile(const CString& strServerFile, GUID& md5);

	void		Fire_OnTransfer(HRESULT hr, STransTask* pTransTask);
};

inline void		CThrdTransfer::SetTaskId(const GUID& dwTaskId) { m_dwTaskId = dwTaskId; }
inline GUID&	CThrdTransfer::GetTaskId() { return m_dwTaskId; }

