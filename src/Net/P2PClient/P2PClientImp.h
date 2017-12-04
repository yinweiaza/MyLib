#pragma once

#include "ThrdClient.h"
#include "ClientSocket.h"
#include "ThrdTask.h"

class CP2PClient :	public IP2PClient
{
	friend class CThrdClient;
	friend class CThrdTask;
public:
	CP2PClient(void);
	virtual ~CP2PClient(void);

	virtual HRESULT SetParam(SP2PClientParam* pParam);
	virtual HRESULT SetNotify(IP2PClientNotify* pNotify);
	virtual HRESULT Release();

	virtual HRESULT AddConnect(CString strIP, DWORD dwPort, DWORD& dwConnection);
	virtual HRESULT RetryConnect(DWORD dwConnection);
	virtual HRESULT DeleteConnect(DWORD dwConnection);
	virtual HRESULT ModifyConnect(DWORD dwConnection, CString strIP, DWORD dwPort);

	virtual HRESULT SendData(DWORD dwConnection, int nDataLen, char* lpData, int nPriority);
	virtual HRESULT SendFileList(DWORD dwConnection, const GUID & dwTaskId, CStringList& strFiles, CString strServerDir);
	virtual HRESULT SendDir(DWORD dwConnection, const GUID & dwTaskId, CString strDir, CString strServerDir);
	virtual HRESULT SendProgram(DWORD dwConnection, const GUID & dwTaskId, STranFileInfoArray &progFileInfo, CString strServerDir);

	virtual HRESULT PauseTask(const GUID & dwTaskId);
	virtual HRESULT ContinueTask(const GUID & dwTaskId);
	virtual HRESULT CancelTask(const GUID & dwTaskId);
protected:
	HRESULT Fire_OnNotify(DWORD dwConnection, DWORD dwType, int nDataLen, void* lpData);

	CThrdClient*	GetThrdClient(DWORD dwConnection);
	CThrdTask*		GetThrdTask(const GUID& dwTaskId);

	void			OnCloseTask(GUID& dwTaskId);
protected:
	IP2PClientNotify* m_pNotify;
	CMap<DWORD,DWORD, CThrdClient*,CThrdClient*>m_connects;
//	CThrdTaskMap	m_thrdTasks;
	CMap<CThrdTask*, CThrdTask*, CThrdClient*,CThrdClient*>m_connectTasks; 
};
