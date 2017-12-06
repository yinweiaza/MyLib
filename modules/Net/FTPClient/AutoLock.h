#pragma once

#include <afxmt.h>

class CAutoLock
{
public:
	CAutoLock(CSyncObject* pSyncObject)
	{
		ASSERT(pSyncObject != NULL);

		m_pSyncObject = pSyncObject;
		m_pSyncObject->Lock();
	}

	virtual ~CAutoLock(void)
	{
		ASSERT(m_pSyncObject != NULL);

		m_pSyncObject->Unlock();
	}

private:
	CSyncObject* m_pSyncObject;
};
