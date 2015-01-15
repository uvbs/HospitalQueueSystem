#pragma once


class CMyGetFile
{
public:
	CMyGetFile(void);
	~CMyGetFile(void);
	CString GetExeFullFilePath();
private:
	CString m_strPath;
};
