#pragma once

#include "LogDefine.h"
#include "..\Common\StdioFileEx.h"

#define LOGSIZE_MAX		4096

interface ILogWriterCallback
{
	virtual HRESULT OnWriteItem(SLogItem *pItem) PURE;
};

class CLogWriter/*: public CUserThread*/
{
protected:
	static CLogWriter	*m_pLogWriter;

public:
	CLogWriter(void);
	virtual ~CLogWriter(void);

protected:
	HRESULT			WriteLogItem(SLogItem *pLogItem);
	HRESULT			Log(int type, DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpLogString);
	HRESULT			LogEx(int type, DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpFmt, ...);

	void			CheckCreateNew(SLogItem *pLogItem);

public:
	static CLogWriter*	GetLogWriter()	{	return m_pLogWriter;	}
	static CLogWriter*	CreateInstance();
	static void			DestroyInstance();

	HRESULT			Init(LPCTSTR lpPrefix, DWORD outputFlags);
	void			Uninit();
	void			EnableLogType(DWORD typeFlags);
	void			DisableLogType(DWORD typeFlags);
	void			RegistCallback(ILogWriterCallback *pCallback);
	void			UnregistCallback(ILogWriterCallback *pCallback);

	HRESULT			LogSuccess(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpLogString);
	HRESULT			LogInformation(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpLogString);
	HRESULT			LogError(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpLogString);
	HRESULT			LogWarning(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpLogString);
	HRESULT			LogDebug(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpLogString);

	HRESULT			LogSuccessEx(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpFmt, ...);
	HRESULT			LogInformationEx(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpFmt, ...);
	HRESULT			LogErrorEx(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpFmt, ...);
	HRESULT			LogWarningEx(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpFmt, ...);
	HRESULT			LogDebugEx(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpFmt, ...);


protected:
	HANDLE			m_hEvent;
	CString			m_pathPrefix;
	WORD			m_year;
	WORD			m_month;
	WORD			m_day;
	CStdioFileEx	m_logFile;
	DWORD			m_outputFlags;
	TCHAR			m_logString[LOGSIZE_MAX];

	CList<ILogWriterCallback*, ILogWriterCallback*>	m_callbacks;
};

inline HRESULT	CLogWriter::LogSuccess(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpLogString)
{	return Log(LogType_Success, errCode, lpFile, codeLine, lpLogString);	}
inline HRESULT	CLogWriter::LogInformation(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpLogString)
{	return Log(Log_EnableInformation, errCode, lpFile, codeLine, lpLogString);	}
inline HRESULT	CLogWriter::LogError(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpLogString)
{	return Log(LogType_Error, errCode, lpFile, codeLine, lpLogString);	}
inline HRESULT	CLogWriter::LogWarning(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpLogString)
{	return Log(LogType_Warning, errCode, lpFile, codeLine, lpLogString);	}
inline HRESULT	CLogWriter::LogDebug(DWORD errCode, LPCSTR lpFile, int codeLine, LPCTSTR lpLogString)
{	return Log(LogType_Debug, errCode, lpFile, codeLine, lpLogString);	}


HRESULT InitLogWriter(LPCTSTR lpPathPrefix, 
					  DWORD outputFlags = Log_EnableSuccess|
					  Log_EnableInformation|
					  Log_EnableWarning|
					  Log_EnableError);
void	UninitLogWriter();


#ifdef DEBUG

	#define  LOG_S0()					CLogWriter::GetLogWriter()->LogSuccessEx(0, __FILE__, __LINE__, _T(""))
	#define  LOG_S1(a)					CLogWriter::GetLogWriter()->LogSuccessEx(0, __FILE__, __LINE__, a)
	#define  LOG_S2(a, b)				CLogWriter::GetLogWriter()->LogSuccessEx(0, __FILE__, __LINE__, a, b)
	#define  LOG_S3(a, b, c)			CLogWriter::GetLogWriter()->LogSuccessEx(0, __FILE__, __LINE__, a, b, c)
	#define  LOG_S4(a, b, c, d)			CLogWriter::GetLogWriter()->LogSuccessEx(0, __FILE__, __LINE__, a, b, c, d)
	#define  LOG_S5(a, b, c, d, e)		CLogWriter::GetLogWriter()->LogSuccessEx(0, __FILE__, __LINE__, a, b, c, d, e)
	#define  LOG_S6(a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogSuccessEx(0, __FILE__, __LINE__, a, b, c, d, e, f)

	#define  LOG_I0()					CLogWriter::GetLogWriter()->LogInformationEx(0, __FILE__, __LINE__, _T(""))
	#define  LOG_I1(a)					CLogWriter::GetLogWriter()->LogInformationEx(0, __FILE__, __LINE__, a)
	#define  LOG_I2(a, b)				CLogWriter::GetLogWriter()->LogInformationEx(0, __FILE__, __LINE__, a, b)
	#define  LOG_I3(a, b, c)			CLogWriter::GetLogWriter()->LogInformationEx(0, __FILE__, __LINE__, a, b, c)
	#define  LOG_I4(a, b, c, d)			CLogWriter::GetLogWriter()->LogInformationEx(0, __FILE__, __LINE__, a, b, c, d)
	#define  LOG_I5(a, b, c, d, e)		CLogWriter::GetLogWriter()->LogInformationEx(0, __FILE__, __LINE__, a, b, c, d, e)
	#define  LOG_I6(a, b, c, d, e,f)	CLogWriter::GetLogWriter()->LogInformationEx(0, __FILE__, __LINE__, a, b, c, d, e, f)

	#define  LOG_W0()					CLogWriter::GetLogWriter()->LogWarningEx(0, __FILE__, __LINE__, _T(""))
	#define  LOG_W1(a)					CLogWriter::GetLogWriter()->LogWarningEx(0, __FILE__, __LINE__, a)
	#define  LOG_W2(a, b)				CLogWriter::GetLogWriter()->LogWarningEx(0, __FILE__, __LINE__, a, b)
	#define  LOG_W3(a, b, c)			CLogWriter::GetLogWriter()->LogWarningEx(0, __FILE__, __LINE__, a, b, c)
	#define  LOG_W4(a, b, c, d)			CLogWriter::GetLogWriter()->LogWarningEx(0, __FILE__, __LINE__, a, b, c, d)
	#define  LOG_W5(a, b, c, d, e)		CLogWriter::GetLogWriter()->LogWarningEx(0, __FILE__, __LINE__, a, b, c, d, e)
	#define  LOG_W6(a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogWarningEx(0, __FILE__, __LINE__, a, b, c, d, e, f)

	#define  LOG_E0()					CLogWriter::GetLogWriter()->LogErrorEx(0, __FILE__, __LINE__, _T(""))
	#define  LOG_E1(a)					CLogWriter::GetLogWriter()->LogErrorEx(0, __FILE__, __LINE__, a)
	#define  LOG_E2(a, b)				CLogWriter::GetLogWriter()->LogErrorEx(0, __FILE__, __LINE__, a, b)
	#define  LOG_E3(a, b, c)			CLogWriter::GetLogWriter()->LogErrorEx(0, __FILE__, __LINE__, a, b, c)
	#define  LOG_E4(a, b, c, d)			CLogWriter::GetLogWriter()->LogErrorEx(0, __FILE__, __LINE__, a, b, c, d)
	#define  LOG_E5(a, b, c, d, e)		CLogWriter::GetLogWriter()->LogErrorEx(0, __FILE__, __LINE__, a, b, c, d, e)
	#define  LOG_E6(a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogErrorEx(0, __FILE__, __LINE__, a, b, c, d, e, f)

	#define  LOG_D0()					CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, _T(""))
	#define  LOG_D1(a)					CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, a)
	#define  LOG_D2(a, b)				CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, a, b)
	#define  LOG_D3(a, b, c)			CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, a, b, c)
	#define  LOG_D4(a, b, c, d)			CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, a, b, c, d)
	#define  LOG_D5(a, b, c, d, e)		CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, a, b, c, d, e)
	#define  LOG_D6(a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, a, b, c, d, e, f)


	#define  LOG_S0_C(errCode)						CLogWriter::GetLogWriter()->LogSuccessEx(errCode, __FILE__, __LINE__, _T(""))
	#define  LOG_S1_C(errCode, a)					CLogWriter::GetLogWriter()->LogSuccessEx(errCode, __FILE__, __LINE__, a)
	#define  LOG_S2_C(errCode, a, b)				CLogWriter::GetLogWriter()->LogSuccessEx(errCode, __FILE__, __LINE__, a, b)
	#define  LOG_S3_C(errCode, a, b, c)				CLogWriter::GetLogWriter()->LogSuccessEx(errCode, __FILE__, __LINE__, a, b, c)
	#define  LOG_S4_C(errCode, a, b, c, d)			CLogWriter::GetLogWriter()->LogSuccessEx(errCode, __FILE__, __LINE__, a, b, c, d)
	#define  LOG_S5_C(errCode, a, b, c, d, e)		CLogWriter::GetLogWriter()->LogSuccessEx(errCode, __FILE__, __LINE__, a, b, c, d, e)
	#define  LOG_S6_C(errCode, a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogSuccessEx(errCode, __FILE__, __LINE__, a, b, c, d, e, f)

	#define  LOG_I0_C(errCode)						CLogWriter::GetLogWriter()->LogInformationEx(errCode, __FILE__, __LINE__, _T(""))
	#define  LOG_I1_C(errCode, a)					CLogWriter::GetLogWriter()->LogInformationEx(errCode, __FILE__, __LINE__, a)
	#define  LOG_I2_C(errCode, a, b)				CLogWriter::GetLogWriter()->LogInformationEx(errCode, __FILE__, __LINE__, a, b)
	#define  LOG_I3_C(errCode, a, b, c)				CLogWriter::GetLogWriter()->LogInformationEx(errCode, __FILE__, __LINE__, a, b, c)
	#define  LOG_I4_C(errCode, a, b, c, d)			CLogWriter::GetLogWriter()->LogInformationEx(errCode, __FILE__, __LINE__, a, b, c, d)
	#define  LOG_I5_C(errCode, a, b, c, d, e)		CLogWriter::GetLogWriter()->LogInformationEx(errCode, __FILE__, __LINE__, a, b, c, d, e)
	#define  LOG_I6_C(errCode, a, b, c, d, e,f)		CLogWriter::GetLogWriter()->LogInformationEx(errCode, __FILE__, __LINE__, a, b, c, d, e, f)

	#define  LOG_W0_C(errCode)						CLogWriter::GetLogWriter()->LogWarningEx(errCode, __FILE__, __LINE__, _T(""))
	#define  LOG_W1_C(errCode, a)					CLogWriter::GetLogWriter()->LogWarningEx(errCode, __FILE__, __LINE__, a)
	#define  LOG_W2_C(errCode, a, b)				CLogWriter::GetLogWriter()->LogWarningEx(errCode, __FILE__, __LINE__, a, b)
	#define  LOG_W3_C(errCode, a, b, c)				CLogWriter::GetLogWriter()->LogWarningEx(errCode, __FILE__, __LINE__, a, b, c)
	#define  LOG_W4_C(errCode, a, b, c, d)			CLogWriter::GetLogWriter()->LogWarningEx(errCode, __FILE__, __LINE__, a, b, c, d)
	#define  LOG_W5_C(errCode, a, b, c, d, e)		CLogWriter::GetLogWriter()->LogWarningEx(errCode, __FILE__, __LINE__, a, b, c, d, e)
	#define  LOG_W6_C(errCode, a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogWarningEx(errCode, __FILE__, __LINE__, a, b, c, d, e, f)

	#define  LOG_E0_C(errCode)						CLogWriter::GetLogWriter()->LogErrorEx(errCode, __FILE__, __LINE__, _T(""))
	#define  LOG_E1_C(errCode, a)					CLogWriter::GetLogWriter()->LogErrorEx(errCode, __FILE__, __LINE__, a)
	#define  LOG_E2_C(errCode, a, b)				CLogWriter::GetLogWriter()->LogErrorEx(errCode, __FILE__, __LINE__, a, b)
	#define  LOG_E3_C(errCode, a, b, c)				CLogWriter::GetLogWriter()->LogErrorEx(errCode, __FILE__, __LINE__, a, b, c)
	#define  LOG_E4_C(errCode, a, b, c, d)			CLogWriter::GetLogWriter()->LogErrorEx(errCode, __FILE__, __LINE__, a, b, c, d)
	#define  LOG_E5_C(errCode, a, b, c, d, e)		CLogWriter::GetLogWriter()->LogErrorEx(errCode, __FILE__, __LINE__, a, b, c, d, e)
	#define  LOG_E6_C(errCode, a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogErrorEx(errCode, __FILE__, __LINE__, a, b, c, d, e, f)

	#define  LOG_D0_C(errCode)						CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, _T(""))
	#define  LOG_D1_C(errCode, a)					CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, a)
	#define  LOG_D2_C(errCode, a, b)				CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, a, b)
	#define  LOG_D3_C(errCode, a, b, c)				CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, a, b, c)
	#define  LOG_D4_C(errCode, a, b, c, d)			CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, a, b, c, d)
	#define  LOG_D5_C(errCode, a, b, c, d, e)		CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, a, b, c, d, e)
	#define  LOG_D6_C(errCode, a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, a, b, c, d, e, f)

#else

	#define  LOG_S0()					CLogWriter::GetLogWriter()->LogSuccessEx(0, NULL, 0, _T(""))
	#define  LOG_S1(a)					CLogWriter::GetLogWriter()->LogSuccessEx(0, NULL, 0, a)
	#define  LOG_S2(a, b)				CLogWriter::GetLogWriter()->LogSuccessEx(0, NULL, 0, a, b)
	#define  LOG_S3(a, b, c)			CLogWriter::GetLogWriter()->LogSuccessEx(0, NULL, 0, a, b, c)
	#define  LOG_S4(a, b, c, d)			CLogWriter::GetLogWriter()->LogSuccessEx(0, NULL, 0, a, b, c, d)
	#define  LOG_S5(a, b, c, d, e)		CLogWriter::GetLogWriter()->LogSuccessEx(0, NULL, 0, a, b, c, d, e)
	#define  LOG_S6(a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogSuccessEx(0, NULL, 0, a, b, c, d, e, f)

	#define  LOG_I0()					CLogWriter::GetLogWriter()->LogInformationEx(0, NULL, 0, _T(""))
	#define  LOG_I1(a)					CLogWriter::GetLogWriter()->LogInformationEx(0, NULL, 0, a)
	#define  LOG_I2(a, b)				CLogWriter::GetLogWriter()->LogInformationEx(0, NULL, 0, a, b)
	#define  LOG_I3(a, b, c)			CLogWriter::GetLogWriter()->LogInformationEx(0, NULL, 0, a, b, c)
	#define  LOG_I4(a, b, c, d)			CLogWriter::GetLogWriter()->LogInformationEx(0, NULL, 0, a, b, c, d)
	#define  LOG_I5(a, b, c, d, e)		CLogWriter::GetLogWriter()->LogInformationEx(0, NULL, 0, a, b, c, d, e)
	#define  LOG_I6(a, b, c, d, e,f)	CLogWriter::GetLogWriter()->LogInformationEx(0, NULL, 0, a, b, c, d, e, f)

	#define  LOG_W0()					CLogWriter::GetLogWriter()->LogWarningEx(0, NULL, 0, _T(""))
	#define  LOG_W1(a)					CLogWriter::GetLogWriter()->LogWarningEx(0, NULL, 0, a)
	#define  LOG_W2(a, b)				CLogWriter::GetLogWriter()->LogWarningEx(0, NULL, 0, a, b)
	#define  LOG_W3(a, b, c)			CLogWriter::GetLogWriter()->LogWarningEx(0, NULL, 0, a, b, c)
	#define  LOG_W4(a, b, c, d)			CLogWriter::GetLogWriter()->LogWarningEx(0, NULL, 0, a, b, c, d)
	#define  LOG_W5(a, b, c, d, e)		CLogWriter::GetLogWriter()->LogWarningEx(0, NULL, 0, a, b, c, d, e)
	#define  LOG_W6(a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogWarningEx(0, NULL, 0, a, b, c, d, e, f)

	#define  LOG_E0()					CLogWriter::GetLogWriter()->LogErrorEx(0, NULL, 0, _T(""))
	#define  LOG_E1(a)					CLogWriter::GetLogWriter()->LogErrorEx(0, NULL, 0, a)
	#define  LOG_E2(a, b)				CLogWriter::GetLogWriter()->LogErrorEx(0, NULL, 0, a, b)
	#define  LOG_E3(a, b, c)			CLogWriter::GetLogWriter()->LogErrorEx(0, NULL, 0, a, b, c)
	#define  LOG_E4(a, b, c, d)			CLogWriter::GetLogWriter()->LogErrorEx(0, NULL, 0, a, b, c, d)
	#define  LOG_E5(a, b, c, d, e)		CLogWriter::GetLogWriter()->LogErrorEx(0, NULL, 0, a, b, c, d, e)
	#define  LOG_E6(a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogErrorEx(0, NULL, 0, a, b, c, d, e, f)

	#define  LOG_D0()					CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, _T(""))
	#define  LOG_D1(a)					CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, a)
	#define  LOG_D2(a, b)				CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, a, b)
	#define  LOG_D3(a, b, c)			CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, a, b, c)
	#define  LOG_D4(a, b, c, d)			CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, a, b, c, d)
	#define  LOG_D5(a, b, c, d, e)		CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, a, b, c, d, e)
	#define  LOG_D6(a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogDebugEx(0, __FILE__, __LINE__, a, b, c, d, e, f)


	#define  LOG_S0_C(errCode)						CLogWriter::GetLogWriter()->LogSuccessEx(errCode, NULL, 0, _T(""))
	#define  LOG_S1_C(errCode, a)					CLogWriter::GetLogWriter()->LogSuccessEx(errCode, NULL, 0, a)
	#define  LOG_S2_C(errCode, a, b)				CLogWriter::GetLogWriter()->LogSuccessEx(errCode, NULL, 0, a, b)
	#define  LOG_S3_C(errCode, a, b, c)				CLogWriter::GetLogWriter()->LogSuccessEx(errCode, NULL, 0, a, b, c)
	#define  LOG_S4_C(errCode, a, b, c, d)			CLogWriter::GetLogWriter()->LogSuccessEx(errCode, NULL, 0, a, b, c, d)
	#define  LOG_S5_C(errCode, a, b, c, d, e)		CLogWriter::GetLogWriter()->LogSuccessEx(errCode, NULL, 0, a, b, c, d, e)
	#define  LOG_S6_C(errCode, a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogSuccessEx(errCode, NULL, 0, a, b, c, d, e, f)

	#define  LOG_I0_C(errCode)						CLogWriter::GetLogWriter()->LogInformationEx(errCode, NULL, 0, _T(""))
	#define  LOG_I1_C(errCode, a)					CLogWriter::GetLogWriter()->LogInformationEx(errCode, NULL, 0, a)
	#define  LOG_I2_C(errCode, a, b)				CLogWriter::GetLogWriter()->LogInformationEx(errCode, NULL, 0, a, b)
	#define  LOG_I3_C(errCode, a, b, c)				CLogWriter::GetLogWriter()->LogInformationEx(errCode, NULL, 0, a, b, c)
	#define  LOG_I4_C(errCode, a, b, c, d)			CLogWriter::GetLogWriter()->LogInformationEx(errCode, NULL, 0, a, b, c, d)
	#define  LOG_I5_C(errCode, a, b, c, d, e)		CLogWriter::GetLogWriter()->LogInformationEx(errCode, NULL, 0, a, b, c, d, e)
	#define  LOG_I6_C(errCode, a, b, c, d, e,f)		CLogWriter::GetLogWriter()->LogInformationEx(errCode, NULL, 0, a, b, c, d, e, f)

	#define  LOG_W0_C(errCode)						CLogWriter::GetLogWriter()->LogWarningEx(errCode, NULL, 0, _T(""))
	#define  LOG_W1_C(errCode, a)					CLogWriter::GetLogWriter()->LogWarningEx(errCode, NULL, 0, a)
	#define  LOG_W2_C(errCode, a, b)				CLogWriter::GetLogWriter()->LogWarningEx(errCode, NULL, 0, a, b)
	#define  LOG_W3_C(errCode, a, b, c)				CLogWriter::GetLogWriter()->LogWarningEx(errCode, NULL, 0, a, b, c)
	#define  LOG_W4_C(errCode, a, b, c, d)			CLogWriter::GetLogWriter()->LogWarningEx(errCode, NULL, 0, a, b, c, d)
	#define  LOG_W5_C(errCode, a, b, c, d, e)		CLogWriter::GetLogWriter()->LogWarningEx(errCode, NULL, 0, a, b, c, d, e)
	#define  LOG_W6_C(errCode, a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogWarningEx(errCode, NULL, 0, a, b, c, d, e, f)

	#define  LOG_E0_C(errCode)						CLogWriter::GetLogWriter()->LogErrorEx(errCode, NULL, 0, _T(""))
	#define  LOG_E1_C(errCode, a)					CLogWriter::GetLogWriter()->LogErrorEx(errCode, NULL, 0, a)
	#define  LOG_E2_C(errCode, a, b)				CLogWriter::GetLogWriter()->LogErrorEx(errCode, NULL, 0, a, b)
	#define  LOG_E3_C(errCode, a, b, c)				CLogWriter::GetLogWriter()->LogErrorEx(errCode, NULL, 0, a, b, c)
	#define  LOG_E4_C(errCode, a, b, c, d)			CLogWriter::GetLogWriter()->LogErrorEx(errCode, NULL, 0, a, b, c, d)
	#define  LOG_E5_C(errCode, a, b, c, d, e)		CLogWriter::GetLogWriter()->LogErrorEx(errCode, NULL, 0, a, b, c, d, e)
	#define  LOG_E6_C(errCode, a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogErrorEx(errCode, NULL, 0, a, b, c, d, e, f)

	#define  LOG_D0_C(errCode)						CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, _T(""))
	#define  LOG_D1_C(errCode, a)					CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, a)
	#define  LOG_D2_C(errCode, a, b)				CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, a, b)
	#define  LOG_D3_C(errCode, a, b, c)				CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, a, b, c)
	#define  LOG_D4_C(errCode, a, b, c, d)			CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, a, b, c, d)
	#define  LOG_D5_C(errCode, a, b, c, d, e)		CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, a, b, c, d, e)
	#define  LOG_D6_C(errCode, a, b, c, d, e, f)	CLogWriter::GetLogWriter()->LogDebugEx(errCode, __FILE__, __LINE__, a, b, c, d, e, f)

#endif


#ifdef DEBUG

	//输出日志，添加文件路径和行号
	#define  LOG_SUCCESS(strLog)				CLogWriter::GetLogWriter()->LogSuccess(0, __FILE__, __LINE__, strLog)
	#define  LOG_INFORMATION(strLog)			CLogWriter::GetLogWriter()->LogInformation(0, __FILE__, __LINE__, strLog)
	#define  LOG_ERROR(strLog)					CLogWriter::GetLogWriter()->LogSuccess(0, __FILE__, __LINE__, strLog)
	#define  LOG_WARNING(strLog)				CLogWriter::GetLogWriter()->LogWarning(0, __FILE__, __LINE__, strLog)
	#define  LOG_DEBUG(strLog)					CLogWriter::GetLogWriter()->LogDebug(0, __FILE__, __LINE__, strLog)

	#define  LOG_SUCCESS_C(strLog, code)		CLogWriter::GetLogWriter()->LogSuccess(code, __FILE__, __LINE__, strLog)
	#define  LOG_INFORMATION_C(strLog, code)	CLogWriter::GetLogWriter()->LogInformation(code, __FILE__, __LINE__, strLog)	
	#define  LOG_ERROR_C(strLog, code)			CLogWriter::GetLogWriter()->LogSuccess(code, __FILE__, __LINE__, strLog)	
	#define  LOG_WARNING_C(strLog, code)		CLogWriter::GetLogWriter()->LogWarning(code, __FILE__, __LINE__, strLog)
	#define  LOG_DEBUG_C(strLog, code)			CLogWriter::GetLogWriter()->LogDebug(code, __FILE__, __LINE__, strLog)
#else
	//输出日志，不添加文件路径和行号
	#define  LOG_SUCCESS(strLog)				CLogWriter::GetLogWriter()->LogSuccess(0, NULL, 0, strLog)
	#define  LOG_INFORMATION(strLog)			CLogWriter::GetLogWriter()->LogInformation(0, NULL, 0, strLog)
	#define  LOG_ERROR(strLog)					CLogWriter::GetLogWriter()->LogSuccess(0, NULL, 0, strLog)
	#define  LOG_WARNING(strLog)				CLogWriter::GetLogWriter()->LogWarning(0, NULL, 0, strLog)
	#define  LOG_DEBUG(strLog)					CLogWriter::GetLogWriter()->LogDebug(0, __FILE__, __LINE__, strLog)

	#define  LOG_SUCCESS_C(strLog, code)		CLogWriter::GetLogWriter()->LogSuccess(code, NULL, 0, strLog)
	#define  LOG_INFORMATION_C(strLog, code)	CLogWriter::GetLogWriter()->LogInformation(code, NULL, 0, strLog)
	#define  LOG_ERROR_C(strLog, code)			CLogWriter::GetLogWriter()->LogError(code, NULL, 0, strLog)
	#define  LOG_WARNING_C(strLog, code)		CLogWriter::GetLogWriter()->LogWarning(code, NULL, 0, strLog)
	#define  LOG_DEBUG_C(strLog, code)			CLogWriter::GetLogWriter()->LogDebug(code, __FILE__, __LINE__, strLog)
#endif




