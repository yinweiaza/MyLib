//================================================================================
//  Description:
//      错误代码定义.
//================================================================================

#pragma once

struct STranFileInfo
{
	CString		MD5;									//需要传送的文件的MD5码
	CString		sourcePath;								//需要传送的文件的源路径
	CString		destPath;								//需要传送的文件的目的路径
	LONGLONG	fileSize;								//需要传送的文件的文件大小
	BOOL		bNameAsMD5;								//是否是用MD5码命名的
	BOOL		bCoverSame;								//服务器有相同文件时，是否覆盖

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
// IP2PClientNotify，组件通知接口
// 说明：接口中函数由派生类自己保证线程安全
//////////////////////////////////////////////////////////////////////////
#define AnswerOverwrite	1
#define AnswerResume	2

struct SCNTask	//Client Notify of task
{
	GUID	dwTaskId;	//任务ID
	BOOL	bFinished;	//任务是否完成 
	float	fProgress;	//当前进度，0到1之间
};

enum ClientNotifyType
{
	cnt_state,			//以'\0'结尾的宽字符串；
	cnt_askOverwrite,	//同名文件，覆盖还是续传，1-覆盖，2-续传； 
	cnt_data,			//收到数据包
	cnt_connected,		//已连接
	cnt_disconnect,		//已断开
	cnt_task,			//文件、目录、节目传输任务，lpData指向SCNTask
	cnt_list			//目录列表返回，lpData指向CRemoteItemArray
};