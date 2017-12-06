#pragma once
#include "ServerSocket.h"
// CThrdServer
class CP2PServer;

struct SDataItem 
{
	int		nDataLen;
	char*	pData;
	int		nPriority;
};

struct SThrdServerParam
{
	CString			strFileDir;
	CP2PServer*		pP2PServer;
	BOOL			bDealInnerMsg;
};

class CThrdServer : public CWinThread
{
	friend class CP2PServer;
	DECLARE_DYNCREATE(CThrdServer)
public:
	void			SetParam(SThrdServerParam* pParam);
	void			SendData(int nDataLen, char* lpData, int nPriority);

	void			OnSocketClose();
	void			OnSocketReceivedMessage(void* lpBuf, int nBufLen);

protected:
	CThrdServer();           // protected constructor used by dynamic creation
	virtual ~CThrdServer();

	CP2PServer*		m_pP2PServer;
	CString			m_strFileDir;
	CServerSocket*	m_pServerSocket;
	CFile			m_transFile;
	GUID			m_fileMD5;
	BOOL			m_bDealInnerMsg;

	CList<SDataItem*, SDataItem*>m_datas;

	CCriticalSection m_csData;
	static	CCriticalSection m_csFile;

	void			DeleteDataItem(SDataItem* pItem);
	void			AddDataItem(WORD wMsgId, CString& strXML);

	void			OnPumpSendData(WPARAM wParam, LPARAM lParam);
	BOOL			OnInnerNetMessage(DWORD dwConnection, int nDataLen, void* lpData);			

	HRESULT			ProcGroupFileReq(int nDataLen, void* lpData);
	HRESULT			ProcTransFileReq(int nDataLen, void* lpData);
	HRESULT			ProcTransFileBuf(int nDataLen, void* lpData);
	HRESULT			ProcTransFileEnd(int nDataLen, void* lpData);
	HRESULT			ProcTransTaskEnd(int nDataLen, void* lpData);
public:
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();

protected:
	void	OnAttachSocket(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


