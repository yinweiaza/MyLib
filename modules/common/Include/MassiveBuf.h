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

	BOOL		Allocate(DWORD bufSize);	//最大不超过4G
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

	//压缩初始化
	int			CompressInit();
	//输出一块压缩数据
	//pBuf 用来存放压缩数据
	//len  输入时告知pBuf的大小，输出告知pBuf里存放的有效数据长度
	int			Compress(LPBYTE pBuf, DWORD &len);
	//压缩结束
	int			CompressEnd();
	//压缩到文件，从文件的当前位置开始保存
	HRESULT		CompressToFile(CFile &file);
	//压缩至指定内存块
	HRESULT		CompressToBuff(LPBYTE pCompressed, DWORD &len);

	//解压初始化
	int			UncompressInit();
	//解压缩
	int			Uncompress(LPBYTE lpBuf, DWORD len);
	//接压缩结束
	int			UncompressEnd();
	//从文件中解压缩，从文件的当前位置开始读取
	HRESULT		UncompressFromFile(	CFile		&file,
									ULONGLONG	len);
	//从内存中解压缩
	HRESULT		UncompressFromBuff(LPBYTE pCompressed, DWORD len);

protected:
	void		InitStream();
	BOOL		AddOneBlock();
	void		DeleteBlock(int index);
	void		AddBuf(SBuf *pBuf);
	void		RemoveAt(int index);

protected:
	DWORD					m_blockMaxSize;	//每块最大分配的大小
	LONGLONG				m_bufSize;		//内存大小	

	int						m_bufCount;
	SBuf					**m_ppBufs;
// 	CArray<SBuf*, SBuf*>	m_bufs;			//内存列表


	//压缩解压缩时临时用
	z_stream				m_strm;			
	int						m_nCurBlock;		//记录当前数据指针指向第几块
	int						m_nCurBlockOffset;	//记录当前数据指针在当前块内的偏移
};
