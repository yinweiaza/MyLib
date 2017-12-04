#pragma once

#include "..\Include\zconf.h"
#include "..\Include\zlib.h"
#pragma comment(lib,"zdll.lib")

#ifdef NOUSE_MASSIVEBUF_IMPORT_EXPORT
	#define IP_EP_MASSIVEBUF 
#else
	#define IP_EP_MASSIVEBUF T9_EXT_CLASS
#endif

class IP_EP_MASSIVEBUF CMassiveBuf
{
	struct SBuf
	{
		LPBYTE	lpBuf;
		DWORD	bufSize;

		SBuf()
		{
			lpBuf	= NULL;
			bufSize = 0;
		}

		~SBuf()
		{
			Release();
		}

		void Release()
		{
			if(lpBuf)
			{
				delete lpBuf;
				lpBuf = NULL;
			}

			bufSize = 0;
		}

		BOOL Allocate(DWORD size)
		{
			if(bufSize==size)
				return TRUE;

			Release();

			if(size>0)
			{
				try
				{
					lpBuf = new BYTE[size];
				}
				catch (...)
				{
					return FALSE;
				}
				bufSize = size;
				return TRUE;
			}

			return TRUE;
		}
	};

public:
	CMassiveBuf(DWORD blockSize=1024*1024*4);
	~CMassiveBuf(void);

	LONGLONG		GetBufSize()		{	return m_bufSize;	}

	BOOL		IsEmpty() const		{	return (m_bufSize<=0);	}
	BOOL		NotEmpty() const	{	return (m_bufSize>0);	}

	BOOL		Allocate(DWORD bufSize);	//��󲻳���4G
	void		Release();

	BOOL		CopyFrom(DWORD offset, LPBYTE lpBuf, DWORD bufLen);
	BOOL		CopyTo(DWORD offset, LPBYTE lpBuf, DWORD bufLen) const;
	BOOL		CopyFrom(const CMassiveBuf &other);
	BOOL		WriteToFile(CFile &file) const;
	BOOL		ReadFromFile(CFile &file);
	LPBYTE		GetAddress(DWORD offset);
	void		FillWith(LPBYTE lpBuffer, int len);

	BOOL		CalMD5(CString& strMD5);
	DWORD		CalCRC32();

	//ѹ����ʼ��
	int			CompressInit();
	//���һ��ѹ������
	//pBuf �������ѹ������
	//len  ����ʱ��֪pBuf�Ĵ�С�������֪pBuf���ŵ���Ч���ݳ���
	int			Compress(LPBYTE pBuf, DWORD &len);
	//ѹ������
	int			CompressEnd();
	//ѹ�����ļ������ļ��ĵ�ǰλ�ÿ�ʼ����
	HRESULT		CompressToFile(CFile &file);
	//ѹ����ָ���ڴ��
	HRESULT		CompressToBuff(LPBYTE pCompressed, DWORD &len);

	//��ѹ��ʼ��
	int			UncompressInit();
	//��ѹ��
	int			Uncompress(LPBYTE lpBuf, DWORD len);
	//��ѹ������
	int			UncompressEnd();
	//���ļ��н�ѹ�������ļ��ĵ�ǰλ�ÿ�ʼ��ȡ
	HRESULT		UncompressFromFile(	CFile		&file,
									ULONGLONG	len);
	//���ڴ��н�ѹ��
	HRESULT		UncompressFromBuff(LPBYTE pCompressed, DWORD len);

protected:
	void		InitStream();
	BOOL		AddOneBlock();
	void		DeleteBlock(int index);
	void		AddBuf(SBuf *pBuf);
	void		RemoveAt(int index);

protected:
	DWORD					m_blockMaxSize;	//ÿ��������Ĵ�С
	LONGLONG				m_bufSize;		//�ڴ��С	

	int						m_bufCount;
	SBuf					**m_ppBufs;
// 	CArray<SBuf*, SBuf*>	m_bufs;			//�ڴ��б�


	//ѹ����ѹ��ʱ��ʱ��
	z_stream				m_strm;			
	int						m_nCurBlock;		//��¼��ǰ����ָ��ָ��ڼ���
	int						m_nCurBlockOffset;	//��¼��ǰ����ָ���ڵ�ǰ���ڵ�ƫ��
};
