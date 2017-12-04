#pragma once

#include "..\Common/BaseSocket.h"

class CThrdServer;
class CServerSocket;
typedef CArray<CServerSocket*, CServerSocket*>CServerSocketArray;

class CServerSocket :	public CBaseSocket
{
public:
	CServerSocket(void);
	virtual ~CServerSocket(void);

	void			SetOwner(CThrdServer* pOwner);

protected:
	virtual	void	OnReceivedMessage(void* lpBuf, int nBufLen);
	virtual void	OnClose(int nErrorCode);
	virtual void	Close();
	CThrdServer*	m_pOwner;
};
