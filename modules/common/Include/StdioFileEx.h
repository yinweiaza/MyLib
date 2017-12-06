//===============================================================================================================
// 作    者：  何志民
//---------------------------------------------------------------------------------------------------------------
// 创建日期：
//---------------------------------------------------------------------------------------------------------------
// 内容描述： 文本文件读取扩展类，该类只支持在unicode环境下编译
//			 该类支持ascii，utf-8，unicode三种格式的文本文件读取
//			  
//---------------------------------------------------------------------------------------------------------------
// 修改记录：
//===============================================================================================================

#if !defined(AFX_STDIOFILEEX_H__41AFE3CA_25E0_482F_8B00_C40775BCDB81__INCLUDED_)
#define AFX_STDIOFILEEX_H__41AFE3CA_25E0_482F_8B00_C40775BCDB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NOUSE_STDIOFILEEX_IMPORT_EXPORT
#define IP_EP_STDIOFILEEX
#else
#define IP_EP_STDIOFILEEX T9_EXT_CLASS
#endif

#define	nUNICODE_BOM						0xFEFF		// Unicode "byte order mark" which goes at start of file
#define	sNEWLINE							_T("\r\n")	// New line characters
#define	sDEFAULT_UNICODE_FILLER_CHAR		"#"			// Filler char used when no conversion from Unicode to local code page is possible




#define FileMode_UnicodeFile	0x00800
#define FileMode_UTF8File		0x00400

class IP_EP_STDIOFILEEX CStdioFileEx: public CStdioFile
{
public:
	enum TextFileType
	{
		tft_ansi,
		tft_unicode,
		tft_utf8,
	};

public:
	CStdioFileEx();
	CStdioFileEx( LPCTSTR lpszFileName, UINT nOpenFlags );

	//=================================================================================
	// 功  能：打开文本文件
	//---------------------------------------------------------------------------------
	// 参  数：lpszFileName:	文件名
	//		   nOpenFlags:		打开标志
	//		   pError:			异常指针
	//---------------------------------------------------------------------------------
	// 返回值：
	//=================================================================================
	virtual BOOL	Open( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL );

	//=================================================================================
	// 功  能：读一行文本
	//---------------------------------------------------------------------------------
	// 参  数：rString[in,out]:	存储读取的文本信息
	//---------------------------------------------------------------------------------
	// 返回值：
	//=================================================================================
	virtual BOOL	ReadString(CString& rString);

	//=================================================================================
	// 功  能：写一行文本
	//---------------------------------------------------------------------------------
	// 参  数：lpsz: 写入的文本信息
	//---------------------------------------------------------------------------------
	// 返回值：
	//=================================================================================
	virtual void	WriteString( LPCTSTR lpsz );

	//设置code page
	void			SetCodePage(IN const UINT nCodePage);

	//Multi Byte String 向 unicode string转换
	static int		GetUnicodeStringFromMultiByteString(IN LPCSTR szMultiByteString,
														OUT wchar_t* szUnicodeString,
														IN int nUnicodeBufferSize,
														IN int nCodePage=-1);

	
	//unicode string 向 Multi Byte String转换
	static int		GetMultiByteStringFromUnicodeString(wchar_t * szUnicodeString,
														char* szMultiByteString,
														int nMultiByteBufferSize,
														int nCodePage=-1);

	//计算unicode string 向 Multi Byte String转换时，需要分配的内存长度
	static int		GetRequiredMultiByteLengthForUnicodeString(	wchar_t * szUnicodeString,
																int nCodePage=-1);

protected:
//	UINT	ProcessFlags(const CString& sFilePath, UINT& nOpenFlags);

	//打开文件前修改打开标志
	UINT	PreOpenFile(LPCTSTR lpFile, UINT& nOpenFlags);

	//获取文件类型
	BOOL	GetFileType(LPCTSTR lpFile);

	virtual LPTSTR	ReadUTF8String(LPTSTR lpsz, UINT nMax);
	virtual BOOL	ReadUTF8String(CString& rString);
	virtual void	WriteUTF8String(LPCTSTR lpsz);

	//写utf-8文件头
	void			WriteUTF8BOM();

	TextFileType	m_tft;				//文件类型
	int				m_nFileCodePage;	//code page

	//判断是否是UTF-8文件,包括非标准的
	BOOL IsUTF8(const void* pBuffer, long size);
};

#endif // !defined(AFX_STDIOFILEEX_H__41AFE3CA_25E0_482F_8B00_C40775BCDB81__INCLUDED_)
