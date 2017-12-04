#pragma once

enum HttpFileStatus
{
	en_http_prepared=0,		//׼������
	en_http_translate= 1,	//���ڴ���
	en_http_break = 2,		//�ж�״̬
	en_http_end  = 3,		//�������
};


struct sHttpTranFileInfo
{
	CString		MD5;									//��Ҫ���͵��ļ���MD5��
	CString		filePath;								//��Ҫ���͵��ļ���·��
	CString		sourceName;								//��Ҫ���͵��ļ���,������׺��
	CString     destName;								//�������ϵ���ʾ���֣�������׺��
	CString     destTitle;								//�������ϵ���ʾ����,��������׺��
	CString		fileExtension;							//�ļ���չ��
	LONGLONG	fileSize;								//��Ҫ���͵��ļ����ļ���С
	int			status;									//�ļ��ϴ�״̬


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