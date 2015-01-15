#pragma once
#include "CommonConvert.h"
class CDoFile
{
public:
	CDoFile(void);
	~CDoFile(void);
	int CreateMyDirectory(const CString& path);
	BOOL IsFileExit(const CString& pathname);
	CString ParentPath(const CString& path);
	BOOL MyDeleteDirectory(const CString& path);
	CCommonConvert m_convert;
	CString GetExeFullFilePath();
private:
	CString m_strPath;
};
