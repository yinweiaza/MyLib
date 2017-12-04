#pragma once

interface ILock
{
	virtual	BOOL Lock() PURE;
	virtual	void Unlock() PURE;
};

class CAutoLock
{
public:
	CAutoLock(ILock* pT9Lock, BOOL bLock = FALSE)
	{
		ASSERT(pT9Lock);
		m_pT9Lock = pT9Lock;

		m_bLcoked = FALSE;
		if(bLock)
			m_bLcoked = m_pT9Lock->Lock();
	}

	~CAutoLock()
	{
		Unlock();
	}

	BOOL Lock()
	{
		if(m_bLcoked)
			return TRUE;

		m_bLcoked = m_pT9Lock->Lock();
	}

	void Unlock()
	{
		if(m_bLcoked)
			m_pT9Lock->Unlock();
	}

private:
	ILock			*m_pT9Lock;
	BOOL			m_bLcoked;
};