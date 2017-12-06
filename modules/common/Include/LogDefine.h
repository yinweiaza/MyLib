#pragma once

#define LogType_Success				0
#define LogType_Information			1
#define LogType_Warning				2
#define LogType_Error				3
#define LogType_Debug				4
#define LogType_Count				5

#define Log_EnableSuccess		0x00000001
#define Log_EnableInformation	0x00000002
#define Log_EnableWarning		0x00000004
#define Log_EnableError			0x00000008
#define Log_EnableDebug			0x00000010

#define LOG_LINE_PREFIX			_T("-->>")

struct SLogItemBase
{
	int			type;
	SYSTEMTIME	sysTime;
	DWORD		errCode;
	CString		description;
	DWORD		threadId;
	SLogItemBase()
	{
		type		= LogType_Error;
		GetLocalTime(&sysTime);
		errCode		= 0;
		threadId	= 0;
	}
};

struct SLogItem: public SLogItemBase
{
	CString		codeFile;
	int			codeLine;

	SLogItem()
	{
		codeLine = 0;
	}
};
typedef CArray<SLogItem*, SLogItem*> CLogItemArray;
typedef CList<SLogItem*, SLogItem*>	 CLogItemList;

