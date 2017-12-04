//===============================================================================================================
// ��    �ߣ�  ��־��
//---------------------------------------------------------------------------------------------------------------
// �������ڣ�
//---------------------------------------------------------------------------------------------------------------
// ���������� �ı��ļ���ȡ��չ�࣬����ֻ֧����unicode�����±���
//			 ����֧��ascii��utf-8��unicode���ָ�ʽ���ı��ļ���ȡ
//			  
//---------------------------------------------------------------------------------------------------------------
// �޸ļ�¼��
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
	// ��  �ܣ����ı��ļ�
	//---------------------------------------------------------------------------------
	// ��  ����lpszFileName:	�ļ���
	//		   nOpenFlags:		�򿪱�־
	//		   pError:			�쳣ָ��
	//---------------------------------------------------------------------------------
	// ����ֵ��
	//=================================================================================
	virtual BOOL	Open( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL );

	//=================================================================================
	// ��  �ܣ���һ���ı�
	//---------------------------------------------------------------------------------
	// ��  ����rString[in,out]:	�洢��ȡ���ı���Ϣ
	//---------------------------------------------------------------------------------
	// ����ֵ��
	//=================================================================================
	virtual BOOL	ReadString(CString& rString);

	//=================================================================================
	// ��  �ܣ�дһ���ı�
	//---------------------------------------------------------------------------------
	// ��  ����lpsz: д����ı���Ϣ
	//---------------------------------------------------------------------------------
	// ����ֵ��
	//=================================================================================
	virtual void	WriteString( LPCTSTR lpsz );

	//����code page
	void			SetCodePage(IN const UINT nCodePage);

	//Multi Byte String �� unicode stringת��
	static int		GetUnicodeStringFromMultiByteString(IN LPCSTR szMultiByteString,
														OUT wchar_t* szUnicodeString,
														IN int nUnicodeBufferSize,
														IN int nCodePage=-1);

	
	//unicode string �� Multi Byte Stringת��
	static int		GetMultiByteStringFromUnicodeString(wchar_t * szUnicodeString,
														char* szMultiByteString,
														int nMultiByteBufferSize,
														int nCodePage=-1);

	//����unicode string �� Multi Byte Stringת��ʱ����Ҫ������ڴ泤��
	static int		GetRequiredMultiByteLengthForUnicodeString(	wchar_t * szUnicodeString,
																int nCodePage=-1);

protected:
//	UINT	ProcessFlags(const CString& sFilePath, UINT& nOpenFlags);

	//���ļ�ǰ�޸Ĵ򿪱�־
	UINT	PreOpenFile(LPCTSTR lpFile, UINT& nOpenFlags);

	//��ȡ�ļ�����
	BOOL	GetFileType(LPCTSTR lpFile);

	virtual LPTSTR	ReadUTF8String(LPTSTR lpsz, UINT nMax);
	virtual BOOL	ReadUTF8String(CString& rString);
	virtual void	WriteUTF8String(LPCTSTR lpsz);

	//дutf-8�ļ�ͷ
	void			WriteUTF8BOM();

	TextFileType	m_tft;				//�ļ�����
	int				m_nFileCodePage;	//code page

	//�ж��Ƿ���UTF-8�ļ�,�����Ǳ�׼��
	BOOL IsUTF8(const void* pBuffer, long size);
};

#endif // !defined(AFX_STDIOFILEEX_H__41AFE3CA_25E0_482F_8B00_C40775BCDB81__INCLUDED_)
