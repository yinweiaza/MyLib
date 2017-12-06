
#pragma once

#pragma pack(1)

//C->S
#define NETMSG_TRANSFILE_REQ	0x0001
typedef struct tagTRANSFILE_REQ
{
	WCHAR	serverFile[260];
	__int64	fileSize;
	GUID	fileMD5;
}TRANSFILE_REQ, *LPTRANSFILE_REQ;

//S->
#define NETMSG_TRANSFILE_RES	0x0002
typedef struct tagTRANSFILE_RES
{
	BOOL	fileExist;	//�����ļ��Ѵ���
	__int64	finishSize;	//����ɴ�С����������ļ����ڣ����������ļ���С
	HRESULT	hr;
}TRANSFILE_RES, *LPTRANSFILE_RES;

//C->S
#define NETMSG_TRANSFILE_BUF	0x0003
typedef struct tagTRANSFILE_BUF
{
	__int64	offset;
	int		length;
	char	pBuf[0];
}TRANSFILE_BUF, *LPTRANSFILE_BUF;

//C->S, S->C, �����ļ��������
#define NETMSG_TRANSFILE_END	0x0004

//S->C, �յ��ļ�����
#define NETMSG_TRANSFILE_ACK	0x0005
typedef struct tagTRANSFILE_ACK 
{
	HRESULT hr;			//�������
	int		recvLen;	//���ν��յ��ļ����ݳ�
}TRANSFILE_ACK, *LPTRANSFILE_ACK;

//C->S, S->C, �����������
#define NETMSG_TRANSTASK_END	0x0006

#pragma pack()