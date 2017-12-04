#pragma once
#include "..\Common/BaseSocket.h"
#include "..\Include/CLP2PMsgDefine.h"
#include "..\\include\MessageDefine.h"

#define MsgIdLen	(sizeof(WORD))

interface IClientSocketOwner
{
	virtual void	OnSocketConnect(int nErrorCode) PURE;
	virtual void	OnSocketReceivedMessage(void* lpBuf, int nBufLen) PURE;
	virtual void	OnSocketClose(int nErrorCode) PURE;
};

class CClientSocket : public CBaseSocket
{
public:
	CClientSocket(void);
	virtual ~CClientSocket(void);

	void			SetOwner(IClientSocketOwner* pOwner);

	virtual void	Close();
protected:
	virtual	void	OnReceivedMessage(void* lpBuf, int nBufLen);
	virtual void	OnConnect(int nErrorCode);
	virtual void	OnClose(int nErrorCode);


	IClientSocketOwner*		m_pOwner;

};
