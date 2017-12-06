#pragma once

#include "../include/TranFileInfo.h"

namespace FTPClient
{

enum FileState
{
	FS_Wait,
	FS_Finish
};

struct SFileTask
{
	CString 	taskId;
	GUID		md5;		
	CString 	serverFile;
	CString 	clientFile;
	__int64		totalSize;
	__int64		finishSize; 
	int			curState;
	BOOL		bResume;
	BOOL		bCoverSame;
};
typedef CArray<SFileTask*, SFileTask*>	CFileTaskArray;

class CGroupFile 
{
public:
	CGroupFile(void);
	virtual ~CGroupFile(void);

	__int64			GetTotalSize();
	__int64			GetFinishSize();
	float			GetProgress();

	int				GetFileCount();

	SFileTask*		GetCurrentTask();	//当前
	SFileTask*		MoveToNextTask();
	SFileTask*		GetFirstWaitTask();

	void			SetFinished(SFileTask* pFileTask);				//重新计算整个任务完成量
	void			SetFinishSize(SFileTask* pFileTask, __int64 nSize);
	void			AddFinishSize(SFileTask* pFileTask, __int64 nSize);	//增加节目传输完成量

	HRESULT			Load(CFileTaskArray& arrFileTasks);
	HRESULT			LoadFromFileList(STranFileInfoArray &progFileInfo, LPCTSTR lpszServerDir);
	CString&		GetXML();

	void			SetTaskId(const GUID& dwTaskId);
	GUID&			GetTaskId();

	SFileTask*		Find(const CString& strFileId);

	CFileTaskArray& GetFileTasks();

	void			SetData(DWORD dwData);
	DWORD			GetData();
protected: 
	CFileTaskArray	m_fileTasks;
	__int64			m_nTotalSize;
	__int64			m_nFinishSize;
	CString			m_strXML;
	int				m_curFile;
	GUID			m_dwTaskId;
	DWORD			m_dwData;
};

inline CString&		CGroupFile::GetXML() { return m_strXML; }	
inline int			CGroupFile::GetFileCount() { return m_fileTasks.GetCount(); }	
inline void			CGroupFile::SetTaskId(const GUID& dwTaskId) { m_dwTaskId = dwTaskId; }
inline GUID&		CGroupFile::GetTaskId()		{ return m_dwTaskId; }
inline CFileTaskArray& CGroupFile::GetFileTasks() { return m_fileTasks; }

inline void			CGroupFile::SetData(DWORD dwData) { m_dwData = dwData; }
inline DWORD		CGroupFile::GetData()		{ return m_dwData; }

};
