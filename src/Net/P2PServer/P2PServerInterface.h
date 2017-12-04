#pragma once

//////////////////////////////////////////////////////////////////////////
// IP2PServerNotify，组件通知接口
// 说明：接口中函数由派生类自己保证线程安全
//////////////////////////////////////////////////////////////////////////

enum ServerNotifyType
{
	snt_data,		//收到命令
	snt_connected,		//已连接
	snt_disconnect		//已断开
};

interface IP2PServerNotify
{
	virtual HRESULT OnP2PNotify(DWORD dwConnection, DWORD dwType, int nDataLen, void* lpData) PURE;
};

struct SP2PServerParam 
{
	LPCTSTR	lpszFileDir;
};

interface IP2PServer
{
	virtual HRESULT SetParam(SP2PServerParam* pParam) PURE;
	virtual HRESULT SetNotify(IP2PServerNotify* pNotify) PURE;
	virtual HRESULT Release() PURE;

	//开始监听
	virtual	HRESULT	Start(UINT dwPort) PURE;
	virtual	HRESULT Stop() PURE;

	virtual HRESULT Close(DWORD dwConnection) PURE;

	virtual	HRESULT SendData(DWORD dwConnection, int nDataLen, char* lpData, int nPriority) PURE;
};

extern "C" HRESULT WINAPI CreateP2PServer(IP2PServer** ppP2PServer, BOOL bDealInnerMsg=TRUE);