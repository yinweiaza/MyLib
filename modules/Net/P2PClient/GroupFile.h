#pragma once

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

	SFileTask*		GetCurrentTask();	//��ǰ
	SFileTask*		MoveToNextTask();
	SFileTask*		GetFirstWaitTask();

	void			SetFinished(SFileTask* pFileTask);				//���¼����������������
	void			SetFinishSize(SFileTask* pFileTask, int nSize);
	void			AddFinishSize(SFileTask* pFileTask, int nSize);	//���ӽ�Ŀ���������

	HRESULT			LoadFromFileList(CStringList& strFiles, LPCTSTR lpszServerDir);
	HRESULT			LoadFromDir(LPCTSTR lpszDir, LPCTSTR lpszServerDir);
	HRESULT			LoadFromProgram(STranFileInfoArray &progFileInfo, LPCTSTR lpszServerDir);
	CString&		GetXML();

	void			LoadDirFile(LPCTSTR lpBaseDir, LPCTSTR lpFindDir, LPCTSTR lpszServerDir);
protected:
	CFileTaskArray	m_fileTasks;
	__int64			m_nTotalSize;
	__int64			m_nFinishSize;
	CString			m_strXML;
	int				m_curFile;
};

inline CString&		CGroupFile::GetXML() { return m_strXML; }	
inline int			CGroupFile::GetFileCount() { return m_fileTasks.GetCount(); }