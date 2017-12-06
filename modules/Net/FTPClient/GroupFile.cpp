#include "StdAfx.h"
#include ".\GroupFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace FTPClient
{

CGroupFile::CGroupFile(void)
{
	m_nTotalSize = 0;
	m_nFinishSize = 0;
	m_curFile = -1;
}

CGroupFile::~CGroupFile(void)
{
	DeleteArray(m_fileTasks);
}

__int64			CGroupFile::GetTotalSize()
{
	return m_nTotalSize;
}

__int64			CGroupFile::GetFinishSize()
{
	return m_nFinishSize;
}

float			CGroupFile::GetProgress()
{
	return m_nFinishSize*1.0/m_nTotalSize;
}

SFileTask*		CGroupFile::GetCurrentTask()
{
	if (m_curFile<0 || m_curFile>=m_fileTasks.GetCount())
		return NULL;

	return m_fileTasks[m_curFile];
}

SFileTask*		CGroupFile::MoveToNextTask()
{
	if (m_curFile<0 || m_curFile>=m_fileTasks.GetCount())
		return NULL;

	m_curFile = (m_curFile+1)%(m_fileTasks.GetCount());
	return m_fileTasks[m_curFile];
}

SFileTask*		CGroupFile::GetFirstWaitTask()
{
	if (m_curFile<0 || m_curFile>=m_fileTasks.GetCount())
		return NULL;

	int idx = m_curFile;

	do 
	{
		if (m_fileTasks[idx]->curState==FS_Wait)
		{
			m_curFile = idx;
			return m_fileTasks[idx];
		}
		idx = (idx+1)%(m_fileTasks.GetCount());

	} while (m_curFile != idx);

	return NULL;
}

//重新计算整个任务完成量
void			CGroupFile::SetFinished(SFileTask* pFileTask) 
{	
	__int64 dSize = pFileTask->totalSize - pFileTask->finishSize;
	m_nFinishSize += dSize;
	pFileTask->finishSize = pFileTask->totalSize;
	pFileTask->curState	= FS_Finish;
}

void			CGroupFile::SetFinishSize(SFileTask* pFileTask, __int64 nSize)
{
	__int64 dSize = nSize - pFileTask->finishSize;
	m_nFinishSize += dSize;
	pFileTask->finishSize = nSize;
}

//增加节目传输完成量
void			CGroupFile::AddFinishSize(SFileTask* pFileTask, __int64 nSize)
{
	m_nFinishSize += nSize;
	pFileTask->finishSize += nSize;
}

HRESULT	CGroupFile::LoadFromFileList(STranFileInfoArray &progFileInfo, LPCTSTR lpszServerDir)
{
	for (int i=0; i<progFileInfo.GetCount(); i++)
	{
		STranFileInfo& tfi = progFileInfo[i];
/*
		CString str = tfi.destPath;
		if (str.GetLength()>0 && str.GetAt(0)==_T('\\'))
			str = str.Right(str.GetLength()-1);

		str = UrlCanonicalize2(str);

		TCHAR szPath[MAX_PATH];
		PathCombine(szPath, lpszServerDir, str);
*/
		CString strServerFile = UrlCombine2(lpszServerDir, tfi.destPath);

		GUID guid;
		::CoCreateGuid(&guid);

		SFileTask* p = new SFileTask();
		p->taskId = Guid2String(guid);
		p->clientFile = tfi.sourcePath;
		p->serverFile = strServerFile;
		p->totalSize = tfi.fileSize;
		p->finishSize = 0;
		p->curState = FS_Wait;
		p->bCoverSame = tfi.bCoverSame;
		if (tfi.bNameAsMD5)	
			p->bResume = TRUE;
		else
			p->bResume = FALSE;
		m_fileTasks.Add(p);

		m_nTotalSize += p->totalSize;
	}

	m_curFile = 0;
	return NOERROR;
}


SFileTask*		CGroupFile::Find(const CString& strFileId)
{
	for (int i=0; i<m_fileTasks.GetCount(); i++)
	{
		SFileTask* p = m_fileTasks[i];
		if (strFileId.CompareNoCase(p->taskId)==0)
			return p;
	}
	return NULL;
}

};
