#pragma once

#include "TranFileInfo.h"

//////////////////////////////////////////////////////////////////////////
// IP2PClientNotify�����֪ͨ�ӿ�
// ˵�����ӿ��к������������Լ���֤�̰߳�ȫ
//////////////////////////////////////////////////////////////////////////

interface IP2PClientNotify
{
	virtual HRESULT OnP2PNotify(DWORD dwConnection, DWORD dwType, int nDataLen, void* lpData) PURE;
};

struct SP2PClientParam 
{
	int nFilePriority;
};

//////////////////////////////////////////////////////////////////////////
// IP2PClient����Ե㴫���������ӿ�

interface IP2PClient
{
	virtual HRESULT SetParam(SP2PClientParam* pParam) PURE;
	virtual HRESULT SetNotify(IP2PClientNotify* pNotify) PURE;
	virtual HRESULT Release() PURE;

	virtual HRESULT AddConnect(CString strIP, DWORD dwPort, DWORD& dwConnection) PURE;
	virtual HRESULT RetryConnect(DWORD dwConnection) PURE;
	virtual HRESULT DeleteConnect(DWORD dwConnection) PURE;
	virtual HRESULT ModifyConnect(DWORD dwConnection, CString strIP, DWORD dwPort) PURE;

	virtual HRESULT SendData(DWORD dwConnection, int nDataLen, char* lpData, int nPriority) PURE;
	virtual HRESULT SendFileList(DWORD dwConnection, const GUID & dwTaskId, CStringList& strFiles, CString strServerDir) PURE;
	virtual HRESULT SendDir(DWORD dwConnection, const GUID & dwTaskId, CString strDir, CString strServerDir) PURE;
	virtual HRESULT SendProgram(DWORD dwConnection, const GUID & dwTaskId, STranFileInfoArray &progFileInfo, CString strServerDir) PURE;

	virtual HRESULT PauseTask(const GUID & dwTaskId) PURE;
	virtual HRESULT ContinueTask(const GUID & dwTaskId) PURE;
	virtual HRESULT CancelTask(const GUID & dwTaskId) PURE;
};

extern "C" HRESULT WINAPI CreateP2PClient(IP2PClient** ppP2PClient);

#ifdef DEBUG
	#pragma comment(lib, "P2PClientD.lib")
#else
	#pragma comment(lib, "P2PClient.lib")
#endif