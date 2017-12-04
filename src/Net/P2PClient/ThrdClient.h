#pragma once

#include "ClientSocket.h"

// CThrdClient
class CP2PClient;

struct SDataItem 
{
	int		nDataLen;
	char*	pData;
	int		nPriority;
};

struct SThrdClientParam
{
	CString		strIP;
	DWORD		dwPort;
	CP2PClient* pP2PClient;
};

class CThrdClient : public CWinThread, public IClientSocketOwner
{
	friend class	CClientSocket;
	DECLARE_DYNCREATE(CThrdClient)
public:
	CString			GetIP();
	DWORD			GetPort();
	void			SetParam(SThrdClientParam* pParam);

	void			SendData(int nDataLen, char* lpData, int nPriority);

protected:
	CThrdClient();           
	virtual ~CThrdClient();

	CList<SDataItem*, SDataItem*>m_datas;
	CCriticalSection m_csData;

	CString			m_strIP;
	UINT			m_dwPort;
	CP2PClient*		m_pP2PClient;
	CClientSocket*	m_pClientSocket;
	void			OnRetryConnect(WPARAM wParam, LPARAM lParam);
	BOOL			OnInnerNetMessage(DWORD dwConnection, int nDataLen, void* lpData);	

	void			DeleteDataItem(SDataItem* pItem);
	void			DeleteAllData();
	void			OnSendDataPump(WPARAM wParam, LPARAM lParam);
	BOOL			IsSocketClosed();
public:
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();

	virtual void	OnSocketConnect(int nErrorCode);
	virtual void	OnSocketReceivedMessage(void* lpBuf, int nBufLen);
	virtual void	OnSocketClose(int nErrorCode);
protected:
	DECLARE_MESSAGE_MAP()
};


inline CString	CThrdClient::GetIP()	{ return m_strIP;	}
inline DWORD	CThrdClient::GetPort()	{ return m_dwPort;	}
