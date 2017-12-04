#pragma once

#include "GroupFile.h"
#include "ClientSocket.h"

// CThrdTask
class CThrdClient;
class CP2PClient;
class CClientSocket;

#define THRDTASK_SOCKET_MAXBUF (50*1024*1024)

struct SThrdTaskParam 
{
	CString			strIP;
	DWORD			dwPort;
	GUID			dwTask;
	CP2PClient*		pP2PClient;
	CThrdClient*	pThrdClient;
};
class CThrdTask : public CWinThread, public IClientSocketOwner
{
	DECLARE_DYNCREATE(CThrdTask)
public:
	void	InitFromFileList(const GUID & dwTaskId, CStringList& strFiles, CString strServerDir);
	void	InitFromDir(const GUID & dwTaskId, CString strDir, CString strServerDir);
	void	InitFromProgram(const GUID & dwTaskId, STranFileInfoArray &progFileInfo, CString strServerDir);

	void	SetParam(SThrdTaskParam* pParam);
	GUID	GetTaskId();

	HRESULT	Pause();
	HRESULT Continue();
	HRESULT Cancel();
protected:
	CThrdTask();           // protected constructor used by dynamic creation
	virtual ~CThrdTask();

	CString			m_strIP;
	DWORD			m_dwPort;
	GUID			m_dwTask;
	CP2PClient*		m_pP2PClient;
	CThrdClient*	m_pThrdClient;
	CClientSocket*	m_pClientSocket;
	DWORD			m_aliveMsgQueue;

	//
	enum ThreadTaskType 
	{
		ttt_null=0,
		ttt_fileList,
		ttt_dir,
		ttt_program
	};
	ThreadTaskType	m_taskType;
	CStringList		m_strFiles;
	CString			m_strDir;
	CString			m_strServerDir;
	STranFileInfoArray m_progFileInfo;

	CCriticalSection m_csAccess;

	//´«ÊäÎÄ¼þ
	CGroupFile		m_groupFile;
	CFile			m_transFile;

	void			BeginTransFile();

	BOOL			OnInnerNetMessage(DWORD dwConnection, int nDataLen, void* lpData);			
	void			Fire_TaskNotify(BOOL bFinished, float fProgress);

	void			OnTrandFilePump(WPARAM wParam, LPARAM lParam);
	void			OnThrdTaskPause(WPARAM wParam, LPARAM lParam);
	void			OnThrdTaskContinue(WPARAM wParam, LPARAM lParam);
	void			OnThrdTaskCancel(WPARAM wParam, LPARAM lParam);

	void			PumpTransFileReq();
	void			ProcTransFileRes(int nDataLen, void* lpData);
	void			PumpTransFileBuf();
	void			PumpTransFileEnd();
	void			ProcTransFileEnd(int nDataLen, void* lpData);
	void			PumpTransTaskEnd();
public:
	void			SendData(int nDataLen, char* lpData);
	BOOL			IsSocketClosed();
	BOOL			IsSocketBufFull();

	virtual BOOL	InitInstance();
	virtual int		ExitInstance();

	virtual void	OnSocketConnect(int nErrorCode);
	virtual void	OnSocketReceivedMessage(void* lpBuf, int nBufLen);
	virtual void	OnSocketClose(int nErrorCode);

protected:
	DECLARE_MESSAGE_MAP()
};

typedef CMap<CThrdTask*, CThrdTask*, CThrdTask*, CThrdTask*> CThrdTaskMap; 

inline GUID	CThrdTask::GetTaskId() { return m_dwTask; }