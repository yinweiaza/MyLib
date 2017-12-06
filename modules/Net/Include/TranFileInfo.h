//================================================================================
//  Description:
//      ������붨��.
//================================================================================

#pragma once

struct STranFileInfo
{
	CString		MD5;									//��Ҫ���͵��ļ���MD5��
	CString		sourcePath;								//��Ҫ���͵��ļ���Դ·��
	CString		destPath;								//��Ҫ���͵��ļ���Ŀ��·��
	LONGLONG	fileSize;								//��Ҫ���͵��ļ����ļ���С
	BOOL		bNameAsMD5;								//�Ƿ�����MD5��������
	BOOL		bCoverSame;								//����������ͬ�ļ�ʱ���Ƿ񸲸�

	STranFileInfo()
	{
		fileSize	= 0;
		bNameAsMD5	= FALSE;
		bCoverSame  = TRUE;
	}

	STranFileInfo(const STranFileInfo& other)
	{
		*this = other;
	}

	STranFileInfo& operator = (const STranFileInfo& other)
	{
		MD5					= other.MD5;
		sourcePath			= other.sourcePath;
		destPath			= other.destPath;
		fileSize			= other.fileSize;
		bNameAsMD5			= other.bNameAsMD5;
		bCoverSame			= other.bCoverSame;

		return *this;
	}
};
typedef CArray<STranFileInfo, STranFileInfo&> STranFileInfoArray;


//////////////////////////////////////////////////////////////////////////
// IP2PClientNotify�����֪ͨ�ӿ�
// ˵�����ӿ��к������������Լ���֤�̰߳�ȫ
//////////////////////////////////////////////////////////////////////////
#define AnswerOverwrite	1
#define AnswerResume	2

struct SCNTask	//Client Notify of task
{
	GUID	dwTaskId;	//����ID
	BOOL	bFinished;	//�����Ƿ���� 
	float	fProgress;	//��ǰ���ȣ�0��1֮��
};

enum ClientNotifyType
{
	cnt_state,			//��'\0'��β�Ŀ��ַ�����
	cnt_askOverwrite,	//ͬ���ļ������ǻ���������1-���ǣ�2-������ 
	cnt_data,			//�յ����ݰ�
	cnt_connected,		//������
	cnt_disconnect,		//�ѶϿ�
	cnt_task,			//�ļ���Ŀ¼����Ŀ��������lpDataָ��SCNTask
	cnt_list			//Ŀ¼�б��أ�lpDataָ��CRemoteItemArray
};