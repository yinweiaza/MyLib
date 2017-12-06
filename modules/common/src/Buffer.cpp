#include "StdAfx.h"
#include "Buffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBuffer::CBuffer(int bufSize)
{
	m_pBuf		= NULL;
	m_bufSize	= bufSize;

	if(m_bufSize>0)
		m_pBuf = new BYTE[m_bufSize];
}

CBuffer::~CBuffer(void)
{
	if(m_pBuf)
		delete []m_pBuf;
}

CBuffer::CBuffer(const CBuffer &other)
{
	*this = other;

}

CBuffer& CBuffer::operator=(const CBuffer &other)
{
	if(this!=&other)
	{
		SetBuffer(other.m_pBuf, other.m_bufSize);
	}

	return *this;
}

void CBuffer::SetBufSize(int bufSize)
{
	if(bufSize!=m_bufSize)
	{
		if(m_pBuf)
			delete []m_pBuf;

		if(bufSize>0)
			m_pBuf		= new BYTE[bufSize];
		else
			m_pBuf = NULL;

		m_bufSize	= bufSize;
	}
}

void CBuffer::SetBuffer(LPBYTE lpBuf, int bufLen)
{
	SetBufSize(bufLen);

	if(bufLen>0)
		memcpy(m_pBuf, lpBuf, bufLen);
}

void CBuffer::ZeroMem()
{
	if(m_pBuf && m_bufSize>0)
	{
		ZeroMemory(m_pBuf, m_bufSize);
	}
}


