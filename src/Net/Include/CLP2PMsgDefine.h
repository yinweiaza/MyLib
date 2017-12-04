
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
	BOOL	fileExist;	//最终文件已存在
	__int64	finishSize;	//已完成大小，如果最终文件存在，返回整个文件大小
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

//C->S, S->C, 单个文件传输完成
#define NETMSG_TRANSFILE_END	0x0004

//S->C, 收到文件数据
#define NETMSG_TRANSFILE_ACK	0x0005
typedef struct tagTRANSFILE_ACK 
{
	HRESULT hr;			//错误代码
	int		recvLen;	//本次接收到文件数据长
}TRANSFILE_ACK, *LPTRANSFILE_ACK;

//C->S, S->C, 传输任务完成
#define NETMSG_TRANSTASK_END	0x0006

#pragma pack()