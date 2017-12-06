#pragma once

enum HttpFileStatus
{
	en_http_prepared=0,		//准备传输
	en_http_translate= 1,	//正在传输
	en_http_break = 2,		//中断状态
	en_http_end  = 3,		//传输完成
};


struct sHttpTranFileInfo
{
	CString		MD5;									//需要传送的文件的MD5码
	CString		filePath;								//需要传送的文件的路径
	CString		sourceName;								//需要传送的文件名,包括后缀名
	CString     destName;								//服务器上的显示名字，包括后缀名
	CString     destTitle;								//服务器上的显示名字,不包括后缀名
	CString		fileExtension;							//文件扩展名
	LONGLONG	fileSize;								//需要传送的文件的文件大小
	int			status;									//文件上传状态


	sHttpTranFileInfo()
	{
		fileSize	= 0;
		status      = en_http_prepared;
	}

	sHttpTranFileInfo(const sHttpTranFileInfo& other)
	{
		*this = other;
	}

	sHttpTranFileInfo& operator = (const sHttpTranFileInfo& other)
	{
		MD5					= other.MD5;
		filePath			= other.filePath;
		sourceName			= other.sourceName;
		fileSize			= other.fileSize;
		destName			= other.destName;
		fileExtension		= other.fileExtension;
		destTitle			= other.destTitle;
		status				= other.status;

		return *this;
	}
};
typedef CArray<sHttpTranFileInfo, sHttpTranFileInfo&> sHttpTranFileInfoArray;