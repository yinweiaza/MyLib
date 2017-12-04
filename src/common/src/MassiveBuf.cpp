#include "StdAfx.h"
#include "MassiveBuf.h"
#include ".\Buffer.h"

CMassiveBuf::CMassiveBuf(DWORD blockSize)
{
	m_blockMaxSize		= blockSize;
	m_bufSize			= 0;
	m_nCurBlock			= 0;
	m_nCurBlockOffset	= 0;
	m_bufCount			= 0;
	m_ppBufs			= NULL;
}

CMassiveBuf::~CMassiveBuf(void)
{
	Release();
}

BOOL CMassiveBuf::Allocate(DWORD bufSize)
{
	if(m_bufSize==bufSize)
		return TRUE;

	Release();

	if(bufSize<=0)
		return FALSE;

	int blockCount = bufSize/m_blockMaxSize;
	if((bufSize%m_blockMaxSize)!=0)
		blockCount++;

	for(int i=0; i<blockCount; i++)
	{
		SBuf *pBuf = new SBuf();
		if(pBuf== NULL)
		{
			Release();
			return FALSE;
		}

		if(!pBuf->Allocate(min(m_blockMaxSize, bufSize-m_blockMaxSize*i)))
		{
			delete pBuf;
			Release();
			return FALSE;
		}

		AddBuf(pBuf);
	}

	return TRUE;
}

void CMassiveBuf::AddBuf(SBuf *pBuf)
{
	ASSERT(pBuf && pBuf->bufSize>0 && pBuf->lpBuf!=NULL);

	SBuf **ppBufs = (SBuf**)new BYTE[(m_bufCount+1)*sizeof(SBuf*)];

	if(m_ppBufs)
	{
		memcpy(ppBufs, m_ppBufs, sizeof(SBuf*)*m_bufCount);
		delete m_ppBufs;
	}

	ppBufs[m_bufCount] = pBuf;

	m_bufCount++;
	m_ppBufs	= ppBufs;
	m_bufSize	+= pBuf->bufSize;
}

void CMassiveBuf::RemoveAt(int index)
{
	ASSERT(index>=0 && index<m_bufCount);

	m_bufSize -= m_ppBufs[index]->bufSize;

	for(int i=index; i<m_bufCount-1; i++)
		m_ppBufs[index] = m_ppBufs[index+1];

	m_ppBufs[m_bufCount-1] = NULL;
	m_bufCount--;
}

void CMassiveBuf::Release()
{
	m_bufSize			= 0;
	m_nCurBlock			= 0;
	m_nCurBlockOffset	= 0;

	if(m_ppBufs)
	{
		for(int i=0; i<m_bufCount; i++)
			SafeDelete(m_ppBufs[i]);

		delete m_ppBufs;

		m_bufCount	= 0;
		m_ppBufs	= NULL;
	}

	m_bufSize	= 0;
}

BOOL CMassiveBuf::AddOneBlock()
{
	Pointer<SBuf> pBuf = new SBuf();
	if(pBuf==NULL)
		return FALSE;

	if(!pBuf->Allocate(m_blockMaxSize))
	{
		return FALSE;
	}

	AddBuf(pBuf);
	pBuf = NULL;

	return TRUE;
}

void CMassiveBuf::DeleteBlock(int index)
{
	if(index<0 || index>=m_bufCount)
		return;

	SBuf *pBuf = m_ppBufs[index];
	RemoveAt(index);
	delete pBuf;
}

BOOL CMassiveBuf::CopyFrom(DWORD offset, LPBYTE lpBuf, DWORD bufLen)
{
	if(offset+bufLen>m_bufSize)
		return FALSE;

	int	offsetBlock		= offset/m_blockMaxSize;
	int	offsetInBlock	= offset%m_blockMaxSize;

	for(int i=offsetBlock; i<m_bufCount; i++)
	{
		DWORD copySize = min(m_ppBufs[i]->bufSize-offsetInBlock, bufLen);
		memcpy(m_ppBufs[i]->lpBuf + offsetInBlock, lpBuf, copySize);

		lpBuf  += copySize;
		bufLen -= copySize;

		offsetInBlock = 0;
	}

	return TRUE;
}
BOOL CMassiveBuf::CopyTo(DWORD offset, LPBYTE lpBuf, DWORD bufLen) const
{
	if(offset+bufLen>m_bufSize)
		return FALSE;

	int	offsetBlock		= offset/m_blockMaxSize;
	int	offsetInBlock	= offset%m_blockMaxSize;

	for(int i=offsetBlock; i<m_bufCount; i++)
	{
		int copySize = min(m_ppBufs[i]->bufSize-offsetInBlock, bufLen);
		memcpy(lpBuf, m_ppBufs[i]->lpBuf + offsetInBlock, copySize);

		lpBuf  += copySize;
		bufLen -= copySize;

		offsetInBlock = 0;
	}

	return TRUE;
}

BOOL CMassiveBuf::CopyFrom(const CMassiveBuf &other)
{
	Release();

	int bufCount = other.m_bufCount;
	for(int i=0; i<bufCount; i++)
	{
		SBuf *pBuf = new SBuf();
		if(pBuf== NULL)
		{
			Release();
			return FALSE;
		}

		if(!pBuf->Allocate(other.m_ppBufs[i]->bufSize))
		{
			delete pBuf;
			Release();
			return FALSE;
		}

		memcpy(pBuf->lpBuf, other.m_ppBufs[i]->lpBuf, pBuf->bufSize);

		AddBuf(pBuf);
	}

	m_blockMaxSize	= other.m_blockMaxSize;

	return TRUE;
}


BOOL CMassiveBuf::WriteToFile(CFile &file) const
{
	int bufCount = m_bufCount;
	for(int i=0; i<bufCount; i++)
	{
		file.Write(m_ppBufs[i]->lpBuf, m_ppBufs[i]->bufSize);
	}

	return TRUE;
}

BOOL CMassiveBuf::ReadFromFile(CFile &file)
{
	int bufCount = m_bufCount;
	for(int i=0; i<bufCount; i++)
	{
		if(m_ppBufs[i]->bufSize != file.Read(m_ppBufs[i]->lpBuf, m_ppBufs[i]->bufSize))
			return FALSE;
	}

	return TRUE;
}

LPBYTE CMassiveBuf::GetAddress(DWORD offset)
{
	if(offset>=m_bufSize)
		return NULL;

	int bufCount = m_bufCount;
	for(int i=0; i<bufCount; i++)
	{
		if(offset>=m_ppBufs[i]->bufSize)
		{
			offset -= m_ppBufs[i]->bufSize;
		}
		else
		{
			return m_ppBufs[i]->lpBuf + offset;
		}
	}

	return NULL;
}

void CMassiveBuf::FillWith(LPBYTE lpBuffer, int len)
{
	if(lpBuffer==NULL || len<=0)
		return;

	if(len>=m_blockMaxSize/2)
		return;

	int bufCount = m_bufCount;
	if(bufCount<=0)
		return;

	if(len==1)
	{
		for(int i=0; i<bufCount; i++)
		{
			memset(m_ppBufs[i]->lpBuf, *lpBuffer, m_ppBufs[i]->bufSize);
		}
	}
	else
	{
		//先拷贝第一块
		memcpy(m_ppBufs[0]->lpBuf, lpBuffer, len);

		int i = 0;
		int copyLen = len;

		while(copyLen*2<=m_ppBufs[0]->bufSize)
		{
			memcpy(m_ppBufs[0]->lpBuf+copyLen, m_ppBufs[0]->lpBuf, copyLen);
			copyLen += copyLen;
		}

		memcpy(m_ppBufs[0]->lpBuf+copyLen, m_ppBufs[0]->lpBuf, m_ppBufs[i]->bufSize-copyLen);

		//再拷贝其他行
		for(int i=1; i<bufCount; i++)
		{
			//计算前面块余数，得到这块开始要补充拷贝的字节数
			int offset			= (m_blockMaxSize*i)%len;
			int firstCopyLen	= len-offset;

			if(firstCopyLen>0)
				memcpy(m_ppBufs[i]->lpBuf, lpBuffer+offset, firstCopyLen);

			//从第一块拷贝
			memcpy(m_ppBufs[i]->lpBuf+firstCopyLen, m_ppBufs[0]->lpBuf, min(m_ppBufs[0]->bufSize, m_ppBufs[i]->bufSize-firstCopyLen));
		}
	}
}

BOOL CMassiveBuf::CalMD5(CString& strMD5)
{
#define BUFSIZE 1024
#define MD5LEN  16

	DWORD		dwStatus = 0;
	HCRYPTPROV	hProv = 0;
	HCRYPTHASH	hHash = 0;
	HANDLE		hFile = NULL;
	DWORD		cbRead = 0;
	BYTE		rgbHash[MD5LEN];
	DWORD		cbHash = 0;
	TCHAR		rgbDigits[] = _T("0123456789ABCDEF");

	if (!CryptAcquireContext(&hProv,	//在内存创建密钥容器
							NULL,
							NULL,
							PROV_RSA_FULL,
							CRYPT_VERIFYCONTEXT))
	{
		return FALSE;
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))	//创建hash对象
	{
		CryptReleaseContext(hProv, 0);
		return FALSE;
	}

	BOOL	bResult = TRUE;
	int		nIndex = 0;
	DWORD	dwBufsize = 0;
	LPBYTE	lpBuf = NULL;
	SBuf*   pSBuf = NULL;

	int		bufCount = m_bufCount;

	for(int i=0; i<bufCount; i++)
	{
		nIndex	  = 0;
		pSBuf	  = m_ppBufs[i];
		if(!pSBuf)
		{
			CryptReleaseContext(hProv, 0);
			return FALSE;
		}
		dwBufsize = pSBuf->bufSize;
		lpBuf	  = pSBuf->lpBuf;

		while (nIndex < dwBufsize)
		{
			DWORD dwDataLen = (dwBufsize-nIndex)>BUFSIZE?BUFSIZE:(dwBufsize-nIndex);

			if (!CryptHashData(hHash, lpBuf+nIndex, dwDataLen, 0))
			{
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				return FALSE;
			}

			nIndex += dwDataLen;
		}
	}

	cbHash = MD5LEN;

	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
	{
		strMD5.Empty();
		for (DWORD i = 0; i < cbHash; i++)
		{
			strMD5 += rgbDigits[rgbHash[i] >> 4];
			strMD5 += rgbDigits[rgbHash[i] & 0x0F];
		}

		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}


	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	return bResult;
}

/*  
**	计算大文件的CRC校验码
**	如果一个文件相对较大,显然不能直接读取到内存当中  
**	所以只能将文件分段读取出来进行crc校验,  
**	然后循环将上一次的crc校验码再传递给新的buffer校验函数,  
**	到最后，生成的crc校验码就是该文件的crc校验码. 
*/   

DWORD CMassiveBuf::CalCRC32()
{
	int		bufCount = m_bufCount;

	DWORD	dwBufsize = 0;
	LPBYTE	lpBuf = NULL;
	SBuf*   pSBuf = NULL;

	DWORD	crc = 0xffffffff; 
	DWORD	maxSize = 0;

	DWORD crc32Table[256];
	InitCRC32DirectToTurnTable(crc32Table);

	for(int i=0; i<bufCount; i++)
	{
		pSBuf = m_ppBufs[i];
		if(!pSBuf)
			return 0;

		lpBuf	  = pSBuf->lpBuf;
		dwBufsize = pSBuf->bufSize;

		while(lpBuf < pSBuf->lpBuf + dwBufsize)
			crc =  crc32Table[( crc ^(*(lpBuf++)) )& 0xff] ^ (crc >> 8); 
	}
	return  ~crc ;
}

void CMassiveBuf::InitStream()
{
	m_strm.zalloc = Z_NULL;
	m_strm.zfree  = Z_NULL;
	m_strm.opaque = Z_NULL;

	m_strm.avail_in  = 0;
	m_strm.avail_out = 0;

	m_strm.next_in	= Z_NULL;
	m_strm.next_out	= Z_NULL;

	m_strm.total_in	 = 0;
	m_strm.total_out = 0;

	m_strm.data_type = Z_UNKNOWN;
}

//初始化压缩，返回0代表成功
int CMassiveBuf::CompressInit()
{
	m_nCurBlock			= 0;
	m_nCurBlockOffset	= 0;

	InitStream();

	return deflateInit(&m_strm, Z_BEST_SPEED);
}

int CMassiveBuf::Compress( LPBYTE pBuf, DWORD & len )
{
	if(len<=0)
		return Z_ERRNO;

	int outputOffset = 0;

	int err = 0;
	do 
	{
		if(m_nCurBlock>=m_bufCount)
			break;

		int inputLen = m_ppBufs[m_nCurBlock]->bufSize - m_nCurBlockOffset;
		int outputLen = len - outputOffset;

		// 设置压缩操作的输入数据和输出缓冲区
		m_strm.next_in   = m_ppBufs[m_nCurBlock]->lpBuf + m_nCurBlockOffset;
		m_strm.avail_in  = inputLen;

		m_strm.next_out  = pBuf + outputOffset;  
		m_strm.avail_out = outputLen;

		//压缩
		err = deflate(&m_strm, Z_NO_FLUSH);
		if (err != Z_OK)
			return err;

		int inputUsedLen = inputLen - m_strm.avail_in;
		m_nCurBlockOffset += inputUsedLen;

		int outputUsedLen = outputLen - m_strm.avail_out;
		outputOffset += outputUsedLen;

		//当前块压缩完
		if(m_strm.avail_in<=0)
		{
			m_nCurBlock++;
			m_nCurBlockOffset = 0;
		}

		//如果输出数据区已经被填满
		if(m_strm.avail_out<=0)
		{
			return err;
		}

	} while(m_nCurBlock<m_bufCount);

	//输入结束后，将输出缓冲强制输出
	while(m_nCurBlock==m_bufCount)
	{
		int outputLen = len - outputOffset;

		m_strm.next_out  = pBuf + outputOffset;  
		m_strm.avail_out = outputLen;

		//
		err = deflate(&m_strm, Z_FINISH);
		if (err != Z_STREAM_END && err != Z_OK)
			return err;


		//如果输出数据区已经被填满
		if(m_strm.avail_out<=0)
		{
			return err;
		}

		int outputUsedLen = outputLen - m_strm.avail_out;
		outputOffset += outputUsedLen;

		//压缩完成
		if (err == Z_STREAM_END)
		{
			len = outputOffset;

			return err;
		}
	}

	return Z_OK;
}

//结束压缩，返回0代表成功
int CMassiveBuf::CompressEnd()
{
	m_nCurBlock			= 0;
	m_nCurBlockOffset	= 0;

	return deflateEnd(&m_strm);;
}

//初始化解压，返回0代表成功
int CMassiveBuf::UncompressInit()
{
	Release();

	m_nCurBlock			= 0;
	m_nCurBlockOffset	= 0;

	InitStream();
	return inflateInit(&m_strm);
}


int CMassiveBuf::Uncompress(LPBYTE lpBuf, DWORD len)
{
	if(len<=0)
		return Z_ERRNO;

	//如果指针向的当前块没有申请，申请
	if(m_nCurBlockOffset==0 && m_nCurBlock==m_bufCount)
	{
		if(!AddOneBlock())
			return Z_MEM_ERROR;
	}

	int inputOffset = 0;
	int outputUsed = 0;

	int err = 0;
	do
	{
		int inputLen  = len - inputOffset;
		int outputLen = m_ppBufs[m_nCurBlock]->bufSize - m_nCurBlockOffset;

		// 设置压缩操作的输入数据和输出缓冲区
		m_strm.next_in		= lpBuf + inputOffset;		//输入缓冲区
		m_strm.avail_in		= inputLen;

		m_strm.next_out   = m_ppBufs[m_nCurBlock]->lpBuf + m_nCurBlockOffset;
		m_strm.avail_out  = outputLen;

		err = inflate(&m_strm, Z_NO_FLUSH);				//解压
		if (err != Z_OK&&err != Z_STREAM_END)
			return err;

		int inputUsedLen = inputLen - m_strm.avail_in;
		inputOffset += inputUsedLen;

		int outputUsedLen = outputLen - m_strm.avail_out;
		m_nCurBlockOffset += outputUsedLen;

		//如果输入没有了，退出
		if(m_strm.avail_in<=0)
		{
			//如果当前块也填满了，指向下一块，下一块没有申请，下次调用Uncompress时申请
			if(m_nCurBlockOffset>=m_ppBufs[m_nCurBlock]->bufSize)
			{
				m_nCurBlockOffset = 0;
				m_nCurBlock++;
			}

			return err;
		}

		//如果解压缩结束，退出
		if(err == Z_STREAM_END)
		{
			m_strm.avail_in = 0;
			break;
		}

		//如果当前块也填满了，指向下一块，下一块没有申请，下次调用Uncompress时申请
		if(m_nCurBlockOffset>=m_ppBufs[m_nCurBlock]->bufSize)
		{
			m_nCurBlockOffset = 0;
			m_nCurBlock++;

			if(!AddOneBlock())
				return Z_MEM_ERROR;
		}

	}while(TRUE);

	return err;
}

int CMassiveBuf::UncompressEnd()
{
	int err = Z_STREAM_END;
	do 
	{
		if(m_strm.avail_in==0)
			break;

		//如果指针向的当前块没有申请，申请
		if(m_nCurBlockOffset==0 && m_nCurBlock==m_bufCount)
		{
			if(!AddOneBlock())
			{
				err = Z_MEM_ERROR;
				break;
			}
		}

		int inputOffset = 0;
		int outputUsed = 0;

		//强制将缓冲区的输出
		int err = 0;
		do
		{
			int outputLen = m_ppBufs[m_nCurBlock]->bufSize - m_nCurBlockOffset;

			m_strm.next_out   = m_ppBufs[m_nCurBlock]->lpBuf + m_nCurBlockOffset;
			m_strm.avail_out  = outputLen;

			int err = inflate(&m_strm, Z_FINISH);				//解压
			if (err != Z_OK && err != Z_STREAM_END)
				break;

			int outputUsedLen = outputLen - m_strm.avail_out;
			m_nCurBlockOffset += outputUsedLen;

			//如果解压缩结束，退出
			if (err == Z_STREAM_END)
				break;

			//如果当前块也填满了，指向下一块，下一块没有申请，下次循环时申请
			if(m_nCurBlockOffset>=m_ppBufs[m_nCurBlock]->bufSize)
			{
				m_nCurBlockOffset = 0;
				m_nCurBlock++;

				if(!AddOneBlock())
				{
					err = Z_MEM_ERROR;
					break;
				}

			}

		}while(TRUE);

	} while(FALSE);


	//处理最后一块数据
	do 
	{
		//如果最后一块数据申请了，但是没有被使用
		if(m_nCurBlockOffset==0)
		{
			if(m_bufCount == m_nCurBlock+1)
				DeleteBlock(m_nCurBlock);
		}
		else if(m_nCurBlockOffset!=m_blockMaxSize)//如果最后一块数据长度不够，改为按实际需要分配内存大小
		{
			SBuf *pBuf = new SBuf();
			if(pBuf==NULL)
			{
				err = Z_MEM_ERROR;
				break;
			}

			if(!pBuf->Allocate(m_nCurBlockOffset))
			{
				err = Z_MEM_ERROR;
				break;
			}

			memcpy(pBuf->lpBuf, m_ppBufs[m_nCurBlock]->lpBuf, pBuf->bufSize);

			DeleteBlock(m_nCurBlock);

			AddBuf(pBuf);
		}

	} while (FALSE);


	int err1 = inflateEnd(&m_strm);
	ASSERT(err1==Z_OK);

	return (err==Z_STREAM_END?Z_OK:Z_ERRNO);
}

HRESULT	CMassiveBuf::CompressToFile(CFile &file)
{
	HRESULT hr = T9_E_FAIL;

	CBuffer buffer(1024*4);
	int err = CompressInit();

	while(err==Z_OK)
	{
		DWORD bufLen = buffer.GetBufSize();
		err = Compress(buffer.GetBuffer(), bufLen);
		if(err==Z_OK || err==Z_STREAM_END)
		{
			file.Write(buffer.GetBuffer(), bufLen);
		}

		if(err!=Z_OK)
			break;
	}

	CompressEnd();

	if(err==Z_STREAM_END)
		hr = T9_NOERROR;
	else
		hr = T9_E_FAIL;

	return hr;
}


HRESULT CMassiveBuf::CompressToBuff( LPBYTE pCompressed, DWORD &len )
{
	HRESULT hr = T9_E_FAIL;

	CBuffer buffer(1024*4);

	int err = CompressInit();

	len = 0;

	while(err==Z_OK)
	{
		DWORD bufLen = buffer.GetBufSize();
		err = Compress(buffer.GetBuffer(), bufLen);
		if(err==Z_OK || err==Z_STREAM_END)
		{
			memcpy(pCompressed, buffer.GetBuffer(), bufLen);
			pCompressed+=bufLen;
			len	+= bufLen;
		}

		if(err!=Z_OK)
			break;
	}

	CompressEnd();
	pCompressed-=len;

	if(err==Z_STREAM_END)
		hr = T9_NOERROR;
	else
		hr = T9_E_FAIL;

	return hr;
}

HRESULT	CMassiveBuf::UncompressFromBuff(LPBYTE pCompressed, DWORD len)
{
	HRESULT hr = T9_E_FAIL;
	CBuffer buffer(1024*4);
	int err = UncompressInit();

	int totalReadLen = 0;
	int readLen = 0;
	while(err==Z_OK && totalReadLen<len)
	{
		err = Uncompress(pCompressed, len);
		if(err==Z_STREAM_END)
			break;

		if(err!=Z_OK)
			break;
	}

	UncompressEnd();

	if(err==Z_STREAM_END)
		hr = T9_NOERROR;
	else
		hr = T9_E_FAIL;

	return hr;
}

HRESULT	CMassiveBuf::UncompressFromFile(CFile		&file,
										ULONGLONG	len)
{
	HRESULT hr = T9_E_FAIL;
	CBuffer buffer(1024*4);
	int err = UncompressInit();

	int totalReadLen = 0;
	int readLen = 0;
	while(err==Z_OK && totalReadLen<len)
	{
		readLen = file.Read(buffer.GetBuffer(), buffer.GetBufSize());
		totalReadLen+=readLen;
		err = Uncompress(buffer.GetBuffer(), readLen);
		if(err==Z_STREAM_END)
			break;

		if(err!=Z_OK)
			break;
	}

	UncompressEnd();

	if(err==Z_STREAM_END)
		hr = T9_NOERROR;
	else
		hr = T9_E_FAIL;

	return hr;
}

