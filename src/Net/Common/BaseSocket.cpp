// BaseSocket.cpp : implementation file
//

#include "stdafx.h"
#include "BaseSocket.h"

#include <Mmsystem.h>
#pragma comment( lib,"winmm.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4200)
struct SHeader 
{
	int		nDataLen;
	char	pDataBuf[0];
};
#pragma warning(default:4200)


// BaseSocket

CBaseSocket::CBaseSocket()
{
	m_pSendBuffer = NULL;
	m_nSendLeft = 0;
	m_nSendBegin = 0;

	m_pRecvBuffer = new BYTE[sizeof(SHeader)];
	ZeroMemory(m_pRecvBuffer, sizeof(SHeader));
	m_nRecvLeft = sizeof(SHeader);
	m_nRecvLength = 0;
}

CBaseSocket::~CBaseSocket()
{
	if (m_pSendBuffer != NULL)
		delete m_pSendBuffer;

	if (m_pRecvBuffer != NULL)
		delete m_pRecvBuffer;

}

// CBaseSocket member functions
void	CBaseSocket::OnReceivedMessage(void* lpBuf, int nBufLen)
{

}

void	CBaseSocket::OnReceive(int nErrorCode)
{
	if (nErrorCode)
		Close();

	if(nErrorCode == 0)
	{
		int headLen = sizeof(SHeader);

		if (m_nRecvLeft <= 0)
			return;
		
		int received = Receive(m_pRecvBuffer+m_nRecvLength, m_nRecvLeft);
		if (received == 0)
		{
			Close();
			return;
		}
		else if (received == SOCKET_ERROR)
		{
			if (GetLastError() != WSAEWOULDBLOCK) 
				Close();
			else
				AsyncSelect();

			return;
		}

//		TRACE("\n CBaseSocket::OnReceive-> Receive Data Len:%d  time:%d\n", received, timeGetTime());
// 		for( int i=0; i< min(received , 100); i++ )
// 		{
// 			TRACE("  %02X", *((BYTE*)m_pRecvBuffer+m_nRecvLength+i));
// 		}

		m_nRecvLength += received;
		m_nRecvLeft -= received;

		if (m_nRecvLength >= headLen)
		{
			SHeader header = *(SHeader*)m_pRecvBuffer;

			if (m_nRecvLength+m_nRecvLeft < headLen+header.nDataLen)
			{
				BYTE* pBuffer = new BYTE[headLen+header.nDataLen];
				ZeroMemory(pBuffer, headLen+header.nDataLen);
				memcpy(pBuffer, m_pRecvBuffer, m_nRecvLength);
				delete m_pRecvBuffer;
				m_pRecvBuffer = pBuffer;
				m_nRecvLeft = headLen+header.nDataLen - m_nRecvLength;
			}

			if (m_nRecvLength == headLen+header.nDataLen)
			{
				SHeader* pHeader = (SHeader*)m_pRecvBuffer;
				OnReceivedMessage(pHeader->pDataBuf, pHeader->nDataLen);
				delete m_pRecvBuffer;
				m_pRecvBuffer = new BYTE[sizeof(SHeader)];
				ZeroMemory(m_pRecvBuffer, sizeof(SHeader));
				m_nRecvLeft = sizeof(SHeader);
				m_nRecvLength = 0;
			}
			else if (m_nRecvLength > headLen+header.nDataLen)
			{
				ASSERT(_T("m_nRecvLength > headLen+header.nDataLen"));
			}
		}

		AsyncSelect();
	}
}

HRESULT	CBaseSocket::SendMessage(const void* lpBuf, int nBufLen)
{
	if (nBufLen < 0)
		return E_FAIL;

	SHeader header;
	ZeroMemory(&header, sizeof(SHeader));
	header.nDataLen = nBufLen;

	int msgLen = sizeof(SHeader) + nBufLen;
	int nSendLen = m_nSendLeft +  msgLen;
	BYTE*	pBuffer = new BYTE[nSendLen];
	ZeroMemory(pBuffer, nSendLen);
	if (m_nSendLeft > 0)
	{
		memcpy(pBuffer, m_pSendBuffer+m_nSendBegin, m_nSendLeft);
	}
	memcpy(pBuffer+m_nSendLeft, &header, sizeof(SHeader));
	memcpy(pBuffer+m_nSendLeft+sizeof(SHeader), lpBuf, nBufLen);

	if (m_pSendBuffer != NULL)
	{
		delete m_pSendBuffer;
		m_pSendBuffer = NULL;
	}

	m_pSendBuffer = pBuffer;
	m_nSendLeft = nSendLen;
	m_nSendBegin = 0;

	int nSended = Send(m_pSendBuffer+m_nSendBegin, m_nSendLeft);
	if (nSended == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			//TRACE(_T("WSAGetLastError() = %d\n"), WSAGetLastError());
			Close();
			return E_FAIL;
		}
		else
		{
			//TRACE(_T("WSAGetLastError() = WSAEWOULDBLOCK\n"));
		}
	}
	else
	{
		m_nSendBegin = nSended;
		m_nSendLeft -= nSended; 
	}

	return 0;
}

HRESULT CBaseSocket::SendMessage(WORD nMsgId, const void* lpBuf, int nBufLen)
{
	if (nBufLen < 0)
		return E_FAIL;

	SHeader header;
	ZeroMemory(&header, sizeof(SHeader));
	header.nDataLen = nBufLen + sizeof(WORD);

	int msgLen = sizeof(SHeader) + sizeof(WORD) + nBufLen;
	int nSendLen = m_nSendLeft +  msgLen;
	BYTE*	pBuffer = new BYTE[nSendLen];
	ZeroMemory(pBuffer, nSendLen);
	if (m_nSendLeft > 0)
	{
		memcpy(pBuffer, m_pSendBuffer+m_nSendBegin, m_nSendLeft);
	}
	memcpy(pBuffer+m_nSendLeft, &header, sizeof(SHeader));
	memcpy(pBuffer+m_nSendLeft+sizeof(SHeader), &nMsgId, sizeof(WORD));
	memcpy(pBuffer+m_nSendLeft+sizeof(SHeader)+sizeof(WORD), lpBuf, nBufLen);

	if (m_pSendBuffer != NULL)
	{
		delete m_pSendBuffer;
		m_pSendBuffer = NULL;
	}

	m_pSendBuffer = pBuffer;
	m_nSendLeft = nSendLen;
	m_nSendBegin = 0;

	int nSended = Send(m_pSendBuffer+m_nSendBegin, m_nSendLeft);
	if (nSended == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			//TRACE(_T("WSAGetLastError() = %d\n"), WSAGetLastError());
			Close();
			return E_FAIL;
		}
		else
		{
			//TRACE(_T("WSAGetLastError() = WSAEWOULDBLOCK\n"));
		}
	}
	else
	{
		m_nSendBegin = nSended;
		m_nSendLeft -= nSended; 
	}

	return NOERROR;
}

void	CBaseSocket::OnSend(int nErrorCode)
{
	//TRACE(_T("OnSend\n"));
	if (nErrorCode != 0)
	{
		CString str;
		str.Format(_T("OnSend Error:%d"), nErrorCode);
		//AfxMessageBox(str);
		Close();
		return;
	}

	if (m_nSendLeft <= 0)
		return;

	int nSended = Send(m_pSendBuffer+m_nSendBegin, m_nSendLeft);
	if (nSended == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			CString str;
			str.Format(_T("OnSend Error:%d,%d,%d"), m_nSendBegin, m_nSendLeft, WSAGetLastError());
			//AfxMessageBox(str);

			Close();
			return;
		}
		else
		{
			//TRACE(_T("WSAGetLastError() = WSAEWOULDBLOCK\n"));
		}
	}

	else
	{
		m_nSendBegin = nSended;
		m_nSendLeft -= nSended; 
	}
}