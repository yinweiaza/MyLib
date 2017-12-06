#pragma once

#include "ListenSocket.h"

// CThrdListen
class CP2PServer;
class CListenSocket;

class CThrdListen : public CWinThread
{
	friend class CP2PServer;
	DECLARE_DYNCREATE(CThrdListen)
public:
	void			SetOwner(CP2PServer* pOwner);
	void			OnSocketAccept(int nErrorCode);

	CListenSocket*	m_pListenSocket;
	CListenSocket*	GetListenSocket() { return m_pListenSocket; }
protected:
	CThrdListen();           // protected constructor used by dynamic creation
	virtual ~CThrdListen();

	CP2PServer*		m_pOwner;

public:
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


