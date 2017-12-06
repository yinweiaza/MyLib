#pragma once

//////////////////////////////////////////////////////////////////////////
// IP2PServerNotify�����֪ͨ�ӿ�
// ˵�����ӿ��к������������Լ���֤�̰߳�ȫ
//////////////////////////////////////////////////////////////////////////

enum ServerNotifyType
{
	snt_data,		//�յ�����
	snt_connected,		//������
	snt_disconnect		//�ѶϿ�
};

interface IP2PServerNotify
{
	virtual HRESULT OnP2PNotify(DWORD dwConnection, DWORD dwType, int nDataLen, void* lpData) PURE;
};

struct SP2PServerParam 
{
	LPCTSTR	lpszFileDir;
};

interface IP2PServer
{
	virtual HRESULT SetParam(SP2PServerParam* pParam) PURE;
	virtual HRESULT SetNotify(IP2PServerNotify* pNotify) PURE;
	virtual HRESULT Release() PURE;

	//��ʼ����
	virtual	HRESULT	Start(UINT dwPort) PURE;
	virtual	HRESULT Stop() PURE;

	virtual HRESULT Close(DWORD dwConnection) PURE;

	virtual	HRESULT SendData(DWORD dwConnection, int nDataLen, char* lpData, int nPriority) PURE;
};

extern "C" HRESULT WINAPI CreateP2PServer(IP2PServer** ppP2PServer, BOOL bDealInnerMsg=TRUE);