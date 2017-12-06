#pragma once

#include ".\LogDefine.h"

class CLogReader
{
public:
	CLogReader(void);
	virtual ~CLogReader(void);

	BOOL			Read(LPCTSTR lpFile);
	void			Clear();

	void			AddNew(CString &strLine);
	void			AddToLast(CString &strLine);

public:
	CLogItemList	m_loglist;	
	

protected:
	
	SLogItem		*m_pLast;
	int				m_prefixLen;
	//temp
	int				m_type;
	int				m_year, m_month, m_day, m_h, m_m, m_s, m_ms;
	DWORD			m_errCode;
	DWORD			m_threadId;
	int				m_codeLine;
	CString			m_description;
	CString			m_codeFile;
};
