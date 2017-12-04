#include "StdAfx.h"
#include ".\LogWriter.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD LogTypeIndex2Flag(int index)
{
	return 0x00000001<<index;
}

HRESULT InitLogWriter(LPCTSTR lpPathPrefix, DWORD outputFlags)
{
	CLogWriter *pLogWriter = CLogWriter::CreateInstance();
	ASSERT(pLogWriter);
	return pLogWriter->Init(lpPathPrefix, outputFlags);
}

void UninitLogWriter()
{
	CLogWriter::DestroyInstance();
}

CLogWriter* CLogWriter::m_pLogWriter = NULL;

CLogWriter::CLogWriter(void)
{
	m_outputFlags	= Log_EnableSuccess		|
					  Log_EnableInformation	|
					  Log_EnableWarning		|
					  Log_EnableError;

#ifdef DEBUG
	m_outputFlags |= Log_EnableDebug;
#endif

	m_year			= 0;
	m_month			= 0;
	m_day			= 0;
	m_hEvent		= NULL;

	m_pLogWriter	= NULL;
}

CLogWriter* CLogWriter::CreateInstance()
{
	if(m_pLogWriter == NULL)
		m_pLogWriter = new CLogWriter();

	return m_pLogWriter;
}

void CLogWriter::DestroyInstance()
{
	SafeDelete(m_pLogWriter);
}

CLogWriter::~CLogWriter(void)
{
	Uninit();
}

HRESULT CLogWriter::Init(LPCTSTR lpPrefix, DWORD outputFlags)
{
	m_pathPrefix	= lpPrefix;
	m_hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if(m_hEvent == NULL)
		return E_POINTER;

	m_outputFlags = outputFlags;

	return	NOERROR;
}

void CLogWriter::Uninit()
{
	if(m_logFile.m_hFile != m_logFile.hFileNull)
	{
		m_logFile.Close();
	}

	if(m_hEvent)
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
}


void CLogWriter::EnableLogType(DWORD typeFlags)
{
	m_outputFlags |= typeFlags;
}

void CLogWriter::DisableLogType(DWORD typeFlags)
{
	m_outputFlags &= ~typeFlags;
}

HRESULT CLogWriter::WriteLogItem(SLogItem *pLogItem)
{
	ASSERT(pLogItem);

	if(WaitForSingleObject(m_hEvent, 5*1000) == WAIT_TIMEOUT)
		return E_FAIL;

	CheckCreateNew(pLogItem);
	if(m_logFile.m_hFile == m_logFile.hFileNull)
	{
		SetEvent(m_hEvent);
		return E_FAIL;
	}

	CString	strLog;
	if(pLogItem->codeFile.IsEmpty())
	{
		if(pLogItem->errCode!=0)
		{
			strLog.Format(_T("%s%2d  %.4d-%.2d-%.2d %.2d:%.2d:%.2d:%.3d 0X%X 0X%.8X  %s"), 
						LOG_LINE_PREFIX,
						pLogItem->type,
						pLogItem->sysTime.wYear,
						pLogItem->sysTime.wMonth,
						pLogItem->sysTime.wDay,
						pLogItem->sysTime.wHour,
						pLogItem->sysTime.wMinute,
						pLogItem->sysTime.wSecond,
						pLogItem->sysTime.wMilliseconds,
						pLogItem->threadId,
						pLogItem->errCode,
						pLogItem->description);
		}
		else
		{
				strLog.Format(_T("%s%2d  %.4d-%.2d-%.2d %.2d:%.2d:%.2d:%.3d 0X%X              %s"), 
							LOG_LINE_PREFIX,
							pLogItem->type,
							pLogItem->sysTime.wYear,
							pLogItem->sysTime.wMonth,
							pLogItem->sysTime.wDay,
							pLogItem->sysTime.wHour,
							pLogItem->sysTime.wMinute,
							pLogItem->sysTime.wSecond,
							pLogItem->sysTime.wMilliseconds,
							pLogItem->threadId,
							pLogItem->description);
		}

	}
	else
	{
		if(pLogItem->errCode!=0)
		{
			strLog.Format(_T("%s%2d  %.4d-%.2d-%.2d %.2d:%.2d:%.2d:%.3d 0X%X  0X%.8X  %s  {%s(%d)}"), 
						LOG_LINE_PREFIX,
						pLogItem->type,
						pLogItem->sysTime.wYear,
						pLogItem->sysTime.wMonth,
						pLogItem->sysTime.wDay,
						pLogItem->sysTime.wHour,
						pLogItem->sysTime.wMinute,
						pLogItem->sysTime.wSecond,
						pLogItem->sysTime.wMilliseconds,
						pLogItem->threadId,
						pLogItem->errCode,
						pLogItem->description,
						pLogItem->codeFile,
						pLogItem->codeLine);
		}
		else
		{
			strLog.Format(_T("%s%2d  %.4d-%.2d-%.2d %.2d:%.2d:%.2d:%.3d 0X%X              %s  {%s(%d)}"), 
						LOG_LINE_PREFIX,
						pLogItem->type,
						pLogItem->sysTime.wYear,
						pLogItem->sysTime.wMonth,
						pLogItem->sysTime.wDay,
						pLogItem->sysTime.wHour,
						pLogItem->sysTime.wMinute,
						pLogItem->sysTime.wSecond,
						pLogItem->sysTime.wMilliseconds,
						pLogItem->threadId,
						pLogItem->description,
						pLogItem->codeFile,
						pLogItem->codeLine);
		}
	}

	m_logFile.WriteString(strLog);
	m_logFile.WriteString(_T("\r\n"));
	m_logFile.Flush();
	SetEvent(m_hEvent);

	return NOERROR;
}

HRESULT	CLogWriter::LogEx(int type, DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpFmt, ...)
{
	va_list args;
	va_start (args, lpFmt);	
	_vsntprintf (m_logString, LOGSIZE_MAX, lpFmt, args);
	m_logString[LOGSIZE_MAX-1] = 0;

	return Log(type, errCode, lpFile, codeLine, m_logString);
}


HRESULT CLogWriter::Log(int type, DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpLogString)
{
	DWORD typeFlag = LogTypeIndex2Flag(type);
	if(typeFlag&m_outputFlags)
	{
		SLogItem logItem;
		logItem.type			= type;
		logItem.errCode			= errCode;
		logItem.description		= lpLogString;
		logItem.codeFile		= lpFile;
		logItem.codeLine		= codeLine;
		GetLocalTime(&logItem.sysTime);

		HRESULT hr = WriteLogItem(&logItem);

		POSITION pos = m_callbacks.GetHeadPosition();
		ILogWriterCallback *pCallback = NULL;
		while(pos)
		{
			pCallback = m_callbacks.GetNext(pos);
			ASSERT(pCallback);
			pCallback->OnWriteItem(&logItem);
		}
	}

	return	S_FALSE;
}

void CLogWriter::CheckCreateNew(SLogItem *pLogItem)
{
	ASSERT(pLogItem);
	if(	pLogItem->sysTime.wDay	 != m_day	||
		pLogItem->sysTime.wMonth != m_month	||
		pLogItem->sysTime.wYear	 != m_year)
	{
		m_day	= pLogItem->sysTime.wDay;
		m_month	= pLogItem->sysTime.wMonth;
		m_year	= pLogItem->sysTime.wYear;

		if (m_logFile.m_hFile != m_logFile.hFileNull)
		{
			m_logFile.Close();
		}

		CString strFile;
		strFile.Format(_T("_%.4d_%.2d_%.2d.txt"), m_year, m_month, m_day);
		strFile = m_pathPrefix + strFile;

		TCHAR tcPath[MAX_PATH];
		_stprintf(tcPath, _T("%s"), strFile);
		PathRemoveFileSpec(tcPath);
		if (!PathFileExists(tcPath))
		{
			if (!CreateDirectory(tcPath, NULL))
				return;
		}

		if(m_logFile.Open(strFile, CFile::modeCreate|CFile::modeNoTruncate|
			CFile::modeWrite|CFile::shareDenyWrite|CFile::typeText))
		{
			m_logFile.SeekToEnd();
		}
	}
}

void CLogWriter::RegistCallback(ILogWriterCallback *pCallback)
{
	ASSERT(pCallback);
	if(!m_callbacks.Find(pCallback))
		m_callbacks.AddTail(pCallback);
}

void CLogWriter::UnregistCallback(ILogWriterCallback *pCallback)
{
	POSITION pos = m_callbacks.Find(pCallback);
	if(pos)
		m_callbacks.RemoveAt(pos);
}

HRESULT	CLogWriter::LogSuccessEx(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpFmt, ...)
{
	va_list args;
	va_start (args, lpFmt);	
	_vsntprintf (m_logString, LOGSIZE_MAX, lpFmt, args);
	m_logString[LOGSIZE_MAX-1] = 0;

	return Log(LogType_Success, errCode, lpFile, codeLine, m_logString);
}

HRESULT	CLogWriter::LogInformationEx(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpFmt, ...)
{
	va_list args;
	va_start (args, lpFmt);	
	_vsntprintf (m_logString, LOGSIZE_MAX, lpFmt, args);
	m_logString[LOGSIZE_MAX-1] = 0;

	return Log(LogType_Information, errCode, lpFile, codeLine, m_logString);
}

HRESULT CLogWriter::LogErrorEx(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpFmt, ...)
{
	va_list args;
	va_start (args, lpFmt);	
	_vsntprintf (m_logString, LOGSIZE_MAX, lpFmt, args);
	m_logString[LOGSIZE_MAX-1] = 0;

	return Log(LogType_Error, errCode, lpFile, codeLine, m_logString);
}

HRESULT	CLogWriter::LogWarningEx(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpFmt, ...)
{
	va_list args;
	va_start (args, lpFmt);	
	_vsntprintf (m_logString, LOGSIZE_MAX, lpFmt, args);
	m_logString[LOGSIZE_MAX-1] = 0;

	return Log(LogType_Warning, errCode, lpFile, codeLine, m_logString);
}

HRESULT	CLogWriter::LogDebugEx(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpFmt, ...)
{
	va_list args;
	va_start (args, lpFmt);	
	_vsntprintf (m_logString, LOGSIZE_MAX, lpFmt, args);
	m_logString[LOGSIZE_MAX-1] = 0;

	return Log(LogType_Debug, errCode, lpFile, codeLine, m_logString);
}


