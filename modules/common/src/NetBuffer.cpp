// NetBuffer.cpp : implementation file
//

#include "stdafx.h"
#include "NetBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNetBuffer::CNetBuffer(int nSize)
{
	ASSERT(nSize>0);
	m_lpBuf = new BYTE[nSize];
	m_nSize = nSize;
	ZeroMemory(m_lpBuf, nSize);
}

CNetBuffer::~CNetBuffer(void)
{
	if (m_lpBuf != NULL)
	{
		delete []m_lpBuf;
		m_lpBuf = NULL;
	}
	m_nSize = 0;
}

int CNetBuffer::SizeOf(const CString& strValue)
{
	CT2W t2w(strValue);
	return (wcslen(t2w)+1)*sizeof(WCHAR);
}

void CNetBuffer::Write(int& nVar, const int& nValue)
{
	nVar = nValue;
}

void CNetBuffer::Write(__int64& i64Var, const int& i64Value)
{
	i64Var = i64Value;
}

void CNetBuffer::Write(LPWSTR lpwstr, const CString& strValue)
{
	CT2W t2w(strValue);
	wcscpy(lpwstr, t2w);
}

void CNetBuffer::Write(LPBYTE lpbyte, const CString& strValue)
{
	Write((LPWSTR)lpbyte, strValue);
}

void CNetBuffer::Write(LPBYTE lpbyte, const void* lpValue, UINT nCount)
{
	memcpy(lpbyte, lpValue, nCount);
}

