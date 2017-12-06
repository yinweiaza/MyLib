#pragma once


#ifdef NOUSE_BUFFER_IMPORT_EXPORT
	#define IP_EP_BUFFER 
#else
	#define IP_EP_BUFFER T9_EXT_CLASS
#endif

class IP_EP_BUFFER CBuffer
{
public:
	CBuffer(int bufSize=0);
	~CBuffer(void);

	CBuffer(const CBuffer &other);
	CBuffer& operator=(const CBuffer &other);

	void		SetBufSize(int bufSize);
	int			GetBufSize()	{ return m_bufSize; }

	void		SetBuffer(LPBYTE lpBuf, int bufLen);
	LPBYTE		GetBuffer()		{ return m_pBuf;	}

	void		ZeroMem();

	operator	LPBYTE()		{ return m_pBuf;	}

protected:
	LPBYTE		m_pBuf;
	int			m_bufSize;
};
