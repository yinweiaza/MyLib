#pragma once

#define MsgIdLen	(sizeof(WORD))

// CThrdBase
class CClientSocket;

struct SDataItem 
{
	int		nDataLen;
	char*	pData;
	int		nPriority;
};

class CThrdBase : public CWinThread
{
	DECLARE_DYNCREATE(CThrdBase)
public:

protected:
	CThrdBase();           // protected constructor used by dynamic creation
	virtual ~CThrdBase();

	CList<SDataItem*, SDataItem*>m_datas;

	CCriticalSection m_csData;
	CClientSocket*	m_pClientSocket;
	BOOL			m_bConnected;

	void			DeleteDataItem(SDataItem* pItem);
	void			OnSendDataPump(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();

	virtual void	OnSocketConnect(int nErrorCode);
	virtual void	OnSocketReceivedMessage(void* lpBuf, int nBufLen);
	virtual void	OnSocketClose(int nErrorCode);
protected:
	DECLARE_MESSAGE_MAP()
};


