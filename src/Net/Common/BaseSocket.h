#pragma once


#include <afxsock.h>
//#include "AsyncSocketEx.h"
// CBaseSocket command target

class CBaseSocket : public CAsyncSocket
{
public:
	CBaseSocket();
	virtual ~CBaseSocket();

	int		GetSendLeftLen();
public:
	virtual	void	OnReceivedMessage(void* lpBuf, int nBufLen);
	virtual void	OnReceive(int nErrorCode);
	virtual HRESULT	SendMessage(const void* lpBuf, int nBufLen);
	virtual HRESULT SendMessage(WORD nMsgId, const void* lpBuf, int nBufLen);
	virtual void	OnSend(int nErrorCode);
protected:
	BYTE*	m_pSendBuffer;
	int		m_nSendLeft;
	int		m_nSendBegin;

	BYTE*	m_pRecvBuffer;
	int		m_nRecvLeft;
	int		m_nRecvLength;
};

inline int	CBaseSocket::GetSendLeftLen() { return m_nSendLeft; }
