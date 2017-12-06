#pragma once

#include "ThrdListen.h"
#include "ThrdServer.h"

class CP2PServer :	public IP2PServer
{
	friend class CThrdServer;
	friend class CThrdListen;
public:
	CP2PServer(BOOL bDealInnerMsg);
	virtual ~CP2PServer(void);

	virtual HRESULT SetParam(SP2PServerParam* pParam);
	virtual HRESULT SetNotify(IP2PServerNotify* pNotify);
	virtual HRESULT Release();

	//¿ªÊ¼¼àÌý
	virtual	HRESULT	Start(UINT dwPort);
	virtual	HRESULT Stop();

	virtual HRESULT Close(DWORD dwConnection);

	virtual	HRESULT SendData(DWORD dwConnection, int nDataLen, char* lpData, int nPriority);	
protected:
	HRESULT Fire_OnNotify(DWORD dwConnection, DWORD dwType, int nDataLen, void* lpData);

	CThrdServer*	GetThrdServer(DWORD dwConnection);

	void			OnSocketAccept(int nErrorCode);
	void			OnSocketClose(DWORD dwConnection);

	UINT			m_nPort;
	CThrdListen*	m_pThrdListen;
	CMap<DWORD, DWORD, CThrdServer*, CThrdServer*>m_connects;
	IP2PServerNotify* m_pNotify;
	BOOL			m_bDealInnerMsg;

	CString			m_strFileDir;
};
