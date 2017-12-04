#pragma once

class CNetBuffer
{
public:
	CNetBuffer(int nSize);
	virtual ~CNetBuffer(void);

	LPVOID	GetBuf() { return (LPVOID)m_lpBuf; }
	int		GetSize() { return m_nSize; }
	operator void*() { return m_lpBuf; }

	static	int SizeOf(const CString& strValue);

	static	void Write(int& nVar, const int& nValue);
	static	void Write(__int64& i64Var, const int& i64Value);
	static	void Write(LPWSTR lpwstr, const CString& strValue);
	static	void Write(LPBYTE lpbyte, const CString& strValue);
	static	void Write(LPBYTE lpbyte, const void* lpValue, UINT nCount);
protected:
	LPBYTE	m_lpBuf;
	int		m_nSize;
};



