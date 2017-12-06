#include "StdAfx.h"
#include ".\GroupFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

void			CGroupFile::SetFinishSize(SFileTask* pFileTask, int nSize)
{
	__int64 dSize = nSize - pFileTask->finishSize;
	m_nFinishSize += dSize;
	pFileTask->finishSize = nSize;
}

//增加节目传输完成量
void			CGroupFile::AddFinishSize(SFileTask* pFileTask, int nSize)
{
	m_nFinishSize += nSize;
	pFileTask->finishSize += nSize;
}

HRESULT			CGroupFile::LoadFromFileList(CStringList& strFiles, LPCTSTR lpszServerDir)
{
	POSITION pos = strFiles.GetHeadPosition();
	while (pos != NULL)
	{
		CString strFile = strFiles.GetNext(pos);
		CString str = PathFindFileName(strFile);
		
		TCHAR szPath[MAX_PATH];
		PathCombine(szPath, lpszServerDir, str);

		GUID md5;
		DWORD t1, t2;
		t1 = GetTickCount();
//		CreateMD5FromFile2(strFile, md5);
		t2 = GetTickCount();
//		TRACE(_T("CreateMD5FromFile2:%d\n"), t2-t1);
		t1 = t2;

		CreateMD5Quickly(strFile, md5);
		t2 = GetTickCount();
//		TRACE(_T("CreateMD5Quick:%d\n"), t2-t1);
		t1 = t2;

		CFile file(strFile, CFile::modeRead|CFile::shareDenyNone);
		__int64 totalSize = file.GetLength();
		file.Close();

		SFileTask* p = new SFileTask();
		p->md5 = md5;
		p->clientFile = strFile;
		p->serverFile = szPath;
		p->totalSize = totalSize;
		p->finishSize = 0;
		p->curState = FS_Wait;
		m_fileTasks.Add(p);

		m_nTotalSize += totalSize;
	}

	m_curFile = 0;
	return NOERROR;
}

void	CGroupFile::LoadDirFile(LPCTSTR lpBaseDir, LPCTSTR lpFindDir, LPCTSTR lpszServerDir)
{
	CString strPath(lpFindDir),strBaseDir(lpBaseDir);

	if (strPath.ReverseFind('\\') != (strPath.GetLength() - 1))
		strPath += _T("\\");

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(strPath + _T("*.*"), &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		while (::FindNextFile(hFind, &fd))
		{
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				CString strDir = fd.cFileName;
				if ((strDir != _T(".")) && (strDir != _T("..")))
				{
					LoadDirFile(lpBaseDir, strPath + fd.cFileName + _T("\\"), lpszServerDir);
				}
			}
			else
			{
				CString strFile = strPath + fd.cFileName;
				CString	strTemp = strFile.Right(strFile.GetLength()-strBaseDir.GetLength());
				TCHAR szServerFile[MAX_PATH];
				PathCombine(szServerFile, lpszServerDir, strTemp.Mid(1));//去掉strTemp最前面的斜杠

				GUID md5;

				DWORD t1, t2;
				t1 = GetTickCount();
//				CreateMD5FromFile2(strFile, md5);
				t2 = GetTickCount();
//				TRACE(_T("CreateMD5FromFile2:%d\n"), t2-t1);
				t1 = t2;

				CreateMD5Quickly(strFile, md5);
				t2 = GetTickCount();
//				TRACE(_T("CreateMD5Quick:%d\n"), t2-t1);
				t1 = t2;

				CFile file(strFile, CFile::modeRead|CFile::shareDenyNone);
				__int64 totalSize = file.GetLength();
				file.Close();

				SFileTask* p = new SFileTask();
				p->md5 = md5;
				p->clientFile = strFile;
				p->serverFile = szServerFile;
				p->totalSize = totalSize;
				p->finishSize = 0;
				p->curState = FS_Wait;
				m_fileTasks.Add(p);

				m_nTotalSize += totalSize;
			}
		}

		::FindClose(hFind);
	}
}


HRESULT			CGroupFile::LoadFromDir(LPCTSTR lpszDir, LPCTSTR lpszServerDir)
{
	TCHAR szPath[MAX_PATH];
	PathCombine(szPath, lpszDir, _T(".."));
	CString strBaseDir = szPath;

	CString strFindDir = lpszDir;
	CString strServerDir = lpszServerDir;
	LoadDirFile(strBaseDir, strFindDir, strServerDir);

	m_curFile = 0;
	return NOERROR;
}

HRESULT			CGroupFile::LoadFromProgram(STranFileInfoArray &progFileInfo, LPCTSTR lpszServerDir)
{
	for (int i=0; i<progFileInfo.GetCount(); i++)
	{
		STranFileInfo&	pfi = progFileInfo[i];

		TCHAR szPath[MAX_PATH];
		PathCombine(szPath, lpszServerDir, pfi.destPath);

		GUID md5 = String2Guid(pfi.MD5);

		SFileTask* p = new SFileTask();
		p->md5 = md5;
		p->clientFile = pfi.sourcePath;
		p->serverFile = szPath;
		p->totalSize = pfi.fileSize;
		p->finishSize = 0;
		p->curState = FS_Wait;
		m_fileTasks.Add(p);

		m_nTotalSize += pfi.fileSize;
	}

	m_curFile = 0;
	return NOERROR;
}