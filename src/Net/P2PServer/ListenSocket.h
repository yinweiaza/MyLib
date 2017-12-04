#pragma once

#include "..\Common/BaseSocket.h"

// CListenSocket command target
class CThrdListen;
class CListenSocket : public CBaseSocket
{
public:
	CListenSocket();
	virtual ~CListenSocket();

	void			SetOwner(CThrdListen* pOwner);
protected:
	virtual void	OnAccept(int nErrorCode);
protected:
	CThrdListen*	m_pOwner;
};


