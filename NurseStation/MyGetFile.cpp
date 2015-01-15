#include "StdAfx.h"
#include "MyGetFile.h"


CMyGetFile::CMyGetFile(void)
{
}

CMyGetFile::~CMyGetFile(void)
{
}

CString CMyGetFile::GetExeFullFilePath()
{
	TCHAR lpPath[MAX_PATH]=TEXT("");
	DWORD dwResult=GetModuleFileName(NULL,lpPath,MAX_PATH);
	if(dwResult==0)return _T("");
	for(int i=dwResult;i>0;i--)
	{
		if(lpPath[i]=='\\'){lpPath[i]=0;break;}
	}
	m_strPath=lpPath;
	return m_strPath;
}
