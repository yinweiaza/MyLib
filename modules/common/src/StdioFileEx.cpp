// StdioFileEx.cpp: implementation of the CStdioFileEx class.
//
// Version 1.1 23 August 2003.	Incorporated fixes from Dennis Jeryd.
// Version 1.3 19 February 2005. Incorporated fixes from Howard J Oh and some of my own.
// Version 1.4 26 February 2005. Fixed stupid screw-up in code from 1.3.
// Version 1.5 18 November 2005. - Incorporated fixes from Andy Goodwin.
//											- Allows code page to be specified for reading/writing
//											- Properly calculates multibyte buffer size instead of
//												assuming lstrlen(s).
//											- Should handle UTF8 properly.
//
// Copyright David Pritchard 2003-2005. davidpritchard@ctv.es
//
// You can use this class freely, but please keep my ego happy 
// by leaving this comment in place.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdioFileEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

///*static*/ const UINT CStdioFileEx::modeWriteUnicode = 0x00800; // Add this flag to write in Unicode

CStdioFileEx::CStdioFileEx(): CStdioFile()
{
	m_tft			= tft_ansi;
	m_nFileCodePage = -1;
}

CStdioFileEx::CStdioFileEx(LPCTSTR lpszFileName,UINT nOpenFlags)
:CStdioFile(lpszFileName, PreOpenFile(lpszFileName, nOpenFlags))
{
	m_nFileCodePage = CP_ACP;
}

BOOL CStdioFileEx::Open(LPCTSTR lpszFileName,UINT nOpenFlags,CFileException* pError /*=NULL*/)
{
	// Process any Unicode stuff
	PreOpenFile(lpszFileName, nOpenFlags);

	return CStdioFile::Open(lpszFileName, nOpenFlags, pError);
}

UINT CStdioFileEx::PreOpenFile(LPCTSTR lpFile, UINT& nOpenFlags)
{
	// If we have writeUnicode we must have write or writeRead as well
#ifdef _DEBUG
	if (nOpenFlags & (FileMode_UnicodeFile|FileMode_UTF8File))
	{
		ASSERT(nOpenFlags & CFile::modeWrite || nOpenFlags & CFile::modeReadWrite);
		ASSERT(!( (nOpenFlags &FileMode_UnicodeFile)&&(nOpenFlags &FileMode_UTF8File) ));
	}
#endif

	if(nOpenFlags & FileMode_UnicodeFile)
		m_tft = tft_unicode;
	else if(nOpenFlags & FileMode_UTF8File)
		m_tft = tft_utf8;

	// If reading in text mode and not creating... ; fixed by Dennis Jeryd 6/8/03
	if (nOpenFlags & CFile::typeText && !(nOpenFlags & CFile::modeCreate) && !(nOpenFlags & CFile::modeWrite ))
	{
		GetFileType(lpFile);

		// If it's Unicode, switch to binary mode
		if (tft_unicode == m_tft)
		{
			nOpenFlags ^= CFile::typeText;
			nOpenFlags |= CFile::typeBinary;
		}
	}

	if(nOpenFlags & FileMode_UnicodeFile)
		nOpenFlags ^= FileMode_UnicodeFile;

	if(nOpenFlags & FileMode_UTF8File)
		nOpenFlags ^= FileMode_UTF8File;

	return nOpenFlags;
}

BOOL CStdioFileEx::GetFileType(LPCTSTR lpFile)
{
	CFile	file;
	wchar_t	cFirstChar;
	CFileException	exFile;

	// Open file in binary mode and read first character
	if (file.Open(lpFile, CFile::typeBinary | CFile::modeRead, &exFile))
	{
		// If byte is Unicode byte-order marker, let's say it's Unicode
		if (file.Read(&cFirstChar, sizeof(wchar_t)) > 0 && cFirstChar == (wchar_t)nUNICODE_BOM)
			m_tft = tft_unicode;
		else
		{
			file.SeekToBegin();

			const char	utf8_bom[] = {'\xef', '\xbb', '\xbf'};
			BOOL		ok = FALSE;
			char		buf[3];

			if(file.Read(buf, 3) == 3)
				ok = strncmp(buf, utf8_bom, 3) == 0;

			if(ok)
			{
				m_tft = tft_utf8;
			}
			else
			{
				int Len = (int)file.GetLength();
				char* pBuffer = new char[Len];
				ZeroMemory(pBuffer,Len);
				file.SeekToBegin();
				file.Read(pBuffer, Len);
				ok = IsUTF8(pBuffer,Len);

				if (ok) m_tft = tft_utf8;
				delete []pBuffer;
			}
		}

		file.Close();
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

void CStdioFileEx::SetCodePage(IN const UINT nCodePage)
{
	m_nFileCodePage = (int)nCodePage; 
}

BOOL CStdioFileEx::ReadString(CString& rString)
{
	if(m_tft == tft_utf8)
	{
		// If at position 0, discard byte-order mark before reading
		if (!m_pStream || GetPosition() == 0)
		{
			const char	utf8_bom[] = {'\xef', '\xbb', '\xbf'};
			BOOL		ok = FALSE;
			char        buf[3];

			if(Read(buf, 3) == 3)
				ok = strncmp(buf, utf8_bom, 3) == 0;

			if (!ok)
				SeekToBegin();
		}

		return ReadUTF8String(rString);
	}

	const int	nMAX_LINE_CHARS = 4096;
	BOOL			bReadData = FALSE;
	LPTSTR		lpsz;
	int			nLen = 0;
	wchar_t*		pszUnicodeString = NULL;
	char	*		pszMultiByteString= NULL;
	int			nChars = 0;

	try
	{
		// If at position 0, discard byte-order mark before reading
		if (!m_pStream || (GetPosition() == 0 && m_tft == tft_unicode))
		{
			wchar_t	cDummy;
			//		Read(&cDummy, sizeof(_TCHAR));
			Read(&cDummy, sizeof(wchar_t));
		}

		// If compiled for Unicode
#ifdef _UNICODE
		if (m_tft == tft_unicode)
		{
			// Do standard stuff - Unicode to Unicode. Seems to work OK.
			bReadData = CStdioFile::ReadString(rString);
		}
		else
		{
			pszUnicodeString	= new wchar_t[nMAX_LINE_CHARS]; 
			pszMultiByteString= new char[nMAX_LINE_CHARS]; 

			// Initialise to something safe
			memset(pszUnicodeString, 0, sizeof(wchar_t) * nMAX_LINE_CHARS);
			memset(pszMultiByteString, 0, sizeof(char) * nMAX_LINE_CHARS);

			// Read the string
			bReadData = (NULL != fgets(pszMultiByteString, nMAX_LINE_CHARS, m_pStream));

			if (bReadData)
			{
				// Convert multibyte to Unicode, using the specified code page
				nChars = GetUnicodeStringFromMultiByteString(pszMultiByteString, pszUnicodeString, nMAX_LINE_CHARS, m_nFileCodePage);

				if (nChars > 0)
				{
					rString = (CString)pszUnicodeString;
				}
			}
		}
#endif

		// Then remove end-of-line character if in Unicode text mode
		if (bReadData)
		{
			// Copied from FileTxt.cpp but adapted to Unicode and then adapted for end-of-line being just '\r'. 
			nLen = rString.GetLength();
			if (nLen > 1 && rString.Mid(nLen-2) == sNEWLINE)
			{
				rString.GetBufferSetLength(nLen-2);
			}
			else
			{
				lpsz = rString.GetBuffer(0);
				if (nLen != 0 && (lpsz[nLen-1] == _T('\r') || lpsz[nLen-1] == _T('\n')))
				{
					rString.GetBufferSetLength(nLen-1);
				}
			}
		}
	}
	// Ensure we always delete in case of exception
	catch(...)
	{
		if (pszUnicodeString)	delete [] pszUnicodeString;

		if (pszMultiByteString) delete [] pszMultiByteString;

		throw;
	}

	if (pszUnicodeString)		delete [] pszUnicodeString;

	if (pszMultiByteString)		delete [] pszMultiByteString;

	return bReadData;
}



// --------------------------------------------------------------------------------------------
//
//	CStdioFileEx::WriteString()
//
// --------------------------------------------------------------------------------------------
// Returns:    void
// Parameters: LPCTSTR lpsz
//
// Purpose:		Writes string to file either in Unicode or multibyte, depending on whether the caller specified the
//					CStdioFileEx::modeWriteUnicode flag. Override of base class function.
// Notes:		If writing in Unicode we need to:
//						a) Write the Byte-order-mark at the beginning of the file
//						b) Write all strings in byte-mode
//					-	If we were compiled in Unicode, we need to convert Unicode to multibyte if 
//						we want to write in multibyte
//					-	If we were compiled in multi-byte, we need to convert multibyte to Unicode if 
//						we want to write in Unicode.
// Exceptions:	None.
//
void CStdioFileEx::WriteString(LPCTSTR lpsz)
{
	if(m_tft == tft_utf8)
	{
		// If at position 0, discard byte-order mark before reading
		if (!m_pStream || GetPosition() == 0)
		{
			WriteUTF8BOM();
		}

		return WriteUTF8String(lpsz);
	}

	wchar_t*	pszUnicodeString	= NULL; 
	char	*	pszMultiByteString= NULL;

	try
	{
		// If writing Unicode and at the start of the file, need to write byte mark
		if (m_tft == tft_unicode)
		{
			// If at position 0, write byte-order mark before writing anything else
			if (!m_pStream || GetPosition() == 0)
			{
				wchar_t cBOM = (wchar_t)nUNICODE_BOM;
				CFile::Write(&cBOM, sizeof(wchar_t));
			}
		}

		// If compiled in Unicode...
		// If writing Unicode, no conversion needed
		if (m_tft == tft_unicode)
		{
			// Write in byte mode
			CFile::Write(lpsz, lstrlen(lpsz) * sizeof(wchar_t));
		}
		// Else if we don't want to write Unicode, need to convert
		else
		{
			int		nChars = lstrlen(lpsz) + 1;				// Why plus 1? Because yes
			//			int		nBufferSize = nChars * sizeof(char);	// leave space for multi-byte chars
			int		nCharsWritten = 0;
			int		nBufferSize = 0;

			pszUnicodeString	= new wchar_t[nChars]; 

			// Copy string to Unicode buffer
			lstrcpy(pszUnicodeString, lpsz);

			// Work out how much space we need for the multibyte conversion
			nBufferSize = GetRequiredMultiByteLengthForUnicodeString(pszUnicodeString, m_nFileCodePage);
			pszMultiByteString= new char[nBufferSize];  

			// Get multibyte string
			nCharsWritten = GetMultiByteStringFromUnicodeString(pszUnicodeString, pszMultiByteString, nBufferSize, m_nFileCodePage);

			if (nCharsWritten > 0)
			{
				// Do byte-mode write using actual chars written (fix by Howard J Oh)
				//			CFile::Write((const void*)pszMultiByteString, lstrlen(lpsz));
				CFile::Write((const void*)pszMultiByteString,
					nCharsWritten*sizeof(char));
			}
		}
		// Else if *not* compiled in Unicode
	}
	// Ensure we always clean up
	catch(...)
	{
		if (pszUnicodeString)	delete [] pszUnicodeString;
		if (pszMultiByteString)	delete [] pszMultiByteString;
		throw;
	}

	if (pszUnicodeString)	delete [] pszUnicodeString;
	if (pszMultiByteString)	delete [] pszMultiByteString;
}
// --------------------------------------------------------------------------------------------
//
//	CStdioFileEx::GetUnicodeStringFromMultiByteString()
//
// --------------------------------------------------------------------------------------------
// Returns:    int - num. of chars written (0 means error)
// Parameters: char *		szMultiByteString		(IN)		Multi-byte input string
//					wchar_t*		szUnicodeString		(OUT)		Unicode outputstring
//					int			nUnicodeBufferSize	(IN)		Size of Unicode output buffer in chars(IN)
//					UINT			nCodePage				(IN)		Code page used to perform conversion
//																			Default = -1 (Get local code page).
//
// Purpose:		Gets a Unicode string from a MultiByte string.
// Notes:		None.
// Exceptions:	None.
//
int CStdioFileEx::GetUnicodeStringFromMultiByteString(IN LPCSTR szMultiByteString, OUT wchar_t* szUnicodeString, IN int nUnicodeBufferSize, IN int nCodePage)
{
	bool		bOK = true;
	int		nCharsWritten = 0;

	if (szUnicodeString && szMultiByteString)
	{
		// If no code page specified, take default for system
		if (nCodePage == -1)
		{
			nCodePage = GetACP();
		}

		try 
		{
			// Zero out buffer first. NB: nUnicodeBufferSize is NUMBER OF CHARS, NOT BYTES!
			memset((void*)szUnicodeString, '\0', sizeof(wchar_t) *
				nUnicodeBufferSize);

			// When converting to UTF8, don't set any flags (see Q175392).
			nCharsWritten = MultiByteToWideChar((UINT)nCodePage,
				(nCodePage==CP_UTF8 ? 0:MB_PRECOMPOSED), // Flags
				szMultiByteString,-1,szUnicodeString,nUnicodeBufferSize);
		}
		catch(...)
		{
			TRACE(_T("Controlled exception in MultiByteToWideChar!\n"));
		}
	}

	// Now fix nCharsWritten
	if (nCharsWritten > 0)
	{
		nCharsWritten--;
	}

	ASSERT(nCharsWritten > 0);
	return nCharsWritten;
}

// --------------------------------------------------------------------------------------------
//
//	CStdioFileEx::GetMultiByteStringFromUnicodeString()
//
// --------------------------------------------------------------------------------------------
// Returns:    int - number of characters written. 0 means error
// Parameters: wchar_t *	szUnicodeString			(IN)	Unicode input string
//					char*			szMultiByteString			(OUT)	Multibyte output string
//					int			nMultiByteBufferSize		(IN)	Multibyte buffer size (chars)
//					UINT			nCodePage					(IN)	Code page used to perform conversion
//																			Default = -1 (Get local code page).
//
// Purpose:		Gets a MultiByte string from a Unicode string
// Notes:		Added fix by Andy Goodwin: make buffer into int.
// Exceptions:	None.
//
int CStdioFileEx::GetMultiByteStringFromUnicodeString(wchar_t * szUnicodeString, char* szMultiByteString, 
													  int nMultiByteBufferSize, int nCodePage)
{
	BOOL		bUsedDefChar	= FALSE;
	int		nCharsWritten = 0;

	// Fix by Andy Goodwin: don't do anything if buffer is 0
	if ( nMultiByteBufferSize > 0 )
	{
		if (szUnicodeString && szMultiByteString) 
		{
			// Zero out buffer first
			memset((void*)szMultiByteString, '\0', nMultiByteBufferSize);

			// If no code page specified, take default for system
			if (nCodePage == -1)
			{
				nCodePage = GetACP();
			}

			try 
			{
				// If writing to UTF8, flags, default char and boolean flag must be NULL
				nCharsWritten = WideCharToMultiByte((UINT)nCodePage, 
					(nCodePage==CP_UTF8 ? 0 : WC_COMPOSITECHECK | WC_SEPCHARS), // Flags
					szUnicodeString,-1, 
					szMultiByteString, 
					nMultiByteBufferSize, 
					(nCodePage==CP_UTF8 ? NULL:sDEFAULT_UNICODE_FILLER_CHAR),	// Filler char
					(nCodePage==CP_UTF8 ? NULL: &bUsedDefChar));						// Did we use filler char?

				// If no chars were written and the buffer is not 0, error!
				if (nCharsWritten == 0 && nMultiByteBufferSize > 0)
				{
					TRACE1("Error in WideCharToMultiByte: %d\n", ::GetLastError());
				}
			}
			catch(...) 
			{
				TRACE(_T("Controlled exception in WideCharToMultiByte!\n"));
			}
		} 
	}

	// Now fix nCharsWritten 
	if (nCharsWritten > 0)
	{
		nCharsWritten--;
	}

	return nCharsWritten;
}

//---------------------------------------------------------------------------------------------------
//
//	CStdioFileEx::GetRequiredMultiByteLengthForUnicodeString()
//
//---------------------------------------------------------------------------------------------------
// Returns:    int
// Parameters: wchar_t * szUnicodeString,int nCodePage=-1
//
// Purpose:		Obtains the multi-byte buffer size needed to accommodate a converted Unicode string.
//	Notes:		We can't assume that the buffer length is simply equal to the number of characters
//					because that wouldn't accommodate multibyte characters!
//
/*static*/ int CStdioFileEx::GetRequiredMultiByteLengthForUnicodeString(wchar_t * szUnicodeString,int nCodePage /*=-1*/)
{
	int nCharsNeeded = 0;

	try 
	{
		// If no code page specified, take default for system
		if (nCodePage == -1)
			nCodePage = GetACP();

		// If writing to UTF8, flags, default char and boolean flag must be NULL
		nCharsNeeded = WideCharToMultiByte((UINT)nCodePage, 
			(nCodePage==CP_UTF8 ? 0 : WC_COMPOSITECHECK | WC_SEPCHARS), // Flags
			szUnicodeString,-1, 
			NULL, 
			0,	// Calculate required buffer, please! 
			(nCodePage==CP_UTF8 ? NULL:sDEFAULT_UNICODE_FILLER_CHAR),	// Filler char
			NULL);
	}
	catch(...) 
	{
		TRACE(_T("Controlled exception in WideCharToMultiByte!\n"));
	}

	return nCharsNeeded;
}

LPTSTR CStdioFileEx::ReadUTF8String(LPTSTR lpsz, UINT nMax)
{
	ASSERT(lpsz != NULL);
	ASSERT(AfxIsValidAddress(lpsz, nMax));
	ASSERT(m_pStream != NULL);

	if (lpsz == NULL) 
		AfxThrowInvalidArgException();

	char *buf = new char[nMax]; 
	BOOL read_failed = fgets(buf, nMax, m_pStream) == NULL;
	if (read_failed && !feof(m_pStream)) 
	{
		clearerr(m_pStream);
		AfxThrowFileException(CFileException::genericException, _doserrno, m_strFileName);
	}

	if (!read_failed)
	{
		WCHAR *wbuf = new WCHAR[nMax];
		MultiByteToWideChar(CP_UTF8, 0, buf, strlen(buf)+1, wbuf, nMax);
		wcsncpy(lpsz, wbuf, nMax);
		delete [] wbuf;
	}

	delete [] buf;

	return read_failed ? NULL : lpsz;
}

/**
* Read an UTF8 encoded string from storage and convert it as appropriate.
* The code is snipped from the MFC implementation of CStdioFile::ReadString
* and adapted as appropriate.
*/
BOOL CStdioFileEx::ReadUTF8String(CString& rString)
{
	ASSERT_VALID(this);

	rString = _T("");    // empty string without deallocating
	const int nMaxSize = 1024*128;
	LPTSTR lpsz = rString.GetBuffer(nMaxSize);
	LPTSTR lpszResult;
	int nLen = 0;
	for (;;)
	{
		// Required conversions are done here. If the MFC implementation had
		// called ReadString here as well, instead of _fgetts directly, I
		// wouldn't have to do this override at all.
		lpszResult = ReadUTF8String(lpsz, nMaxSize+1);
		rString.ReleaseBuffer();

		// handle error/eof case
		if (lpszResult == NULL && !feof(m_pStream))
		{
			clearerr(m_pStream);
			AfxThrowFileException(CFileException::genericException, _doserrno,m_strFileName);
		}

		// if string is read completely or EOF
		if (lpszResult == NULL ||
			(nLen = (int)lstrlen(lpsz)) < nMaxSize ||
			lpsz[nLen-1] == '\n')
			break;

		nLen = rString.GetLength();
		lpsz = rString.GetBuffer(nMaxSize + nLen) + nLen;
	}

	// remove '\n' from end of string if present
	lpsz = rString.GetBuffer(0);
	nLen = rString.GetLength();
	if (nLen != 0 && lpsz[nLen-1] == '\n')
		rString.GetBufferSetLength(nLen-1);

	return nLen != 0;
}

/**
* Write the given string in UTF8 format to storage.
* The code is snipped from the MFC implementation of CStdioFile::WriteString
* and adapted as appropriate.
*/
void CStdioFileEx::WriteUTF8String(LPCTSTR lpsz)
{
	ASSERT(lpsz != NULL);
	ASSERT(m_pStream != NULL);

	if (lpsz == NULL)
		AfxThrowInvalidArgException();

	int lpsz_len = lstrlen(lpsz);
	int buf_len = WideCharToMultiByte(CP_UTF8, 0, lpsz, lpsz_len, 0, 0, 0, 0)+1;
	char *buf = new char[buf_len+1];

	WideCharToMultiByte(CP_UTF8, 0, lpsz, lpsz_len, buf, buf_len, 0, 0);
	buf[buf_len] = 0;

	if (fputs(buf, m_pStream) == EOF)
		AfxThrowFileException(CFileException::diskFull, _doserrno, m_strFileName);
	delete [] buf;
}

void CStdioFileEx::WriteUTF8BOM()
{
	if(m_tft == tft_utf8)
	{
		const char utf8_bom[] = {'\xef', '\xbb', '\xbf'};
		Write(utf8_bom, sizeof(utf8_bom));
	}
}

BOOL CStdioFileEx::IsUTF8(const void* pBuffer, long size)  
{  
	//UFT-8转换表表示如下：
	//UNICODE UTF-8 
	//00000000 - 0000007F 0xxxxxxx 1个字节
	//00000080 - 000007FF 110xxxxx 10xxxxxx  2个字节
	//00000800 - 0000FFFF 1110xxxx 10xxxxxx 10xxxxxx 3个字节
	//00010000 - 001FFFFF 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 4个字节
	//00200000 - 03FFFFFF 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 5个字节
	//04000000 - 7FFFFFFF 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 6个字节
	BOOL IsUTF8 = TRUE;  
	unsigned char* start = (unsigned char*)pBuffer;  
	unsigned char* end = (unsigned char*)pBuffer + size;  
	while (start < end)  
	{  
		if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符  
		{  
			start++;  
		}  
		else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符  
		{  
			IsUTF8 = FALSE;  
			break;  
		}  
		else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符  
		{  
			if (start >= end - 1)   
				break;  
			if ((start[1] & (0xC0)) != 0x80)  
			{  
				IsUTF8 = FALSE;  
				break;  
			}  
			start += 2;  
		}   
		else if (*start < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符  
		{  
			if (start >= end - 2)   
				break;  
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)  
			{  
				IsUTF8 = FALSE;  
				break;  
			}  
			start += 3;  
		}
		else if (*start < (0xF8)) // (11111000): 此范围内为4字节UTF-8字符  
		{  
			if (start >= end - 3)   
				break;  
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80 || (start[3] & (0xC0)) != 0x80)  
			{  
				IsUTF8 = FALSE;  
				break;  
			}  
			start += 4;  
		}   
		else if (*start < (0xFC)) // (11111100): 此范围内为5字节UTF-8字符  
		{  
			if (start >= end - 4)   
				break;  
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80|| (start[3] & (0xC0)) != 0x80 || (start[4] & (0xC0)) != 0x80)  
			{  
				IsUTF8 = FALSE;  
				break;  
			}  
			start += 5;  
		}   
		else if (*start < (0xFE)) // (11111110): 此范围内为6字节UTF-8字符  
		{  
			if (start >= end - 5)   
				break;  
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80 || (start[3] & (0xC0)) != 0x80 || (start[4] & (0xC0)) != 0x80 || (start[5] & (0xC0)) != 0x80)  
			{  
				IsUTF8 = FALSE;  
				break;  
			}  
			start += 6;  
		}   
		else  
		{  
			IsUTF8 = FALSE;  
			break;  
		}  
	}  
	return IsUTF8;  
}  