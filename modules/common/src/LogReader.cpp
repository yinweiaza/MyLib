#include "stdafx.h"
#include ".\logreader.h"
#include ".\StdioFileEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLogReader::CLogReader(void)
{
	m_pLast		= NULL;
	m_prefixLen	= _tcsclen(LOG_LINE_PREFIX);
	/*m_description = _T("");*/
}

CLogReader::~CLogReader(void)
{
	Clear();
}

BOOL CLogReader::Read(LPCTSTR lpFile)
{
	CStdioFileEx file;
	if(!file.Open(lpFile, CFile::modeRead|CFile::shareDenyNone|CFile::typeText))
		return FALSE;

	CString strLine;
	while(file.ReadString(strLine))
	{
		if(strLine.Find(LOG_LINE_PREFIX) == 0)
		{
			strLine.Delete(0, 4);
			AddNew(strLine);
		}
		else
		{
			AddToLast(strLine);
		}

		strLine.Empty();
	}

	return TRUE;
}

void CLogReader::Clear()
{
	DeleteList(m_loglist);
	m_pLast	= NULL;
}

void CLogReader::AddNew(CString &strLine)
{
	m_pLast = NULL;
	m_errCode = 0;
	CString str,str1;
	//22__4444-22-22_22:22:22:333__0X88888888__  
	int count = _stscanf(strLine, _T("%2d  %4d-%2d-%2d %2d:%2d:%2d:%3d  0X%X  0X%8X  %[^{]  "),
		&m_type, &m_year, &m_month, &m_day, &m_h, &m_m, &m_s, &m_ms,&m_threadId,&m_errCode,m_description.GetBuffer(1024));	
	_stscanf(strLine,_T("%*[^{]{%[^}]"),m_codeFile.GetBuffer(256));
	_stscanf(m_codeFile,_T("%*[^(](%d"),&m_codeLine);
	m_description.ReleaseBuffer();
	m_codeFile.ReleaseBuffer();
	if (count == 10)
	{
		m_description = _T("");
	}
	if(count<10)
	{
		count = _stscanf(strLine, _T("%2d  %4d-%2d-%2d %2d:%2d:%2d:%3d  0X%X              %[^{]  "),
			&m_type, &m_year, &m_month, &m_day, &m_h, &m_m, &m_s, &m_ms,&m_threadId,m_description.GetBuffer(1024));	
		m_errCode = 0;
		_stscanf(strLine,_T("%*[^{]{%[^}]"),m_codeFile.GetBuffer(256));
		_stscanf(m_codeFile,_T("%*[^(](%d"),&m_codeLine);	
		m_description.ReleaseBuffer();
		m_codeFile.ReleaseBuffer();
		if (count<10)
		{
			if (count == 9)
			{
				m_description = _T("");
			}
			else
			{
				return;
			}
			
		}
	}
	m_pLast					= new SLogItem();
	m_pLast->type					= m_type;
	m_pLast->sysTime.wYear			= m_year;
	m_pLast->sysTime.wMonth			= m_month;
	m_pLast->sysTime.wDay			= m_day;
	m_pLast->sysTime.wHour			= m_h;
	m_pLast->sysTime.wMinute		= m_m;
	m_pLast->sysTime.wSecond		= m_s;
	m_pLast->sysTime.wMilliseconds	= m_ms;

	//strLine.Delete(0, 41);
	//m_pLast->description			= strLine;
	m_pLast->errCode				= m_errCode;
 	m_pLast->threadId				= m_threadId;
	m_pLast->description			= m_description;
	m_pLast->codeLine				= m_codeLine;
	m_pLast->codeFile				= m_codeFile;
	m_loglist.AddTail(m_pLast);
}

void CLogReader::AddToLast(CString &strLine)
{
	if(m_pLast)
	{
// 		m_pLast->description += _T("\n");
		m_pLast->description += strLine;
	}
}
