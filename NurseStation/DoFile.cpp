#include "StdAfx.h"
#include "DoFile.h"

CDoFile::CDoFile(void)
{
}

CDoFile::~CDoFile(void)
{
}

int CDoFile::CreateMyDirectory(const CString& path)
{
	int nerr = -1;
	CString	_path = path;
	
	if(_path.Right(1) == '\\')
		_path=path.Left(_path.GetLength()-1);
	if(IsFileExit(_path))
		return 0;//文件夹/目录已经存在
	CString parentPath=ParentPath(_path);
	if(!parentPath.IsEmpty())//父目录
	{
		nerr = CreateMyDirectory(parentPath);//递归创建
		if(0 != nerr)
		{
			return nerr;
		}
	}
	if(CreateDirectory(_path,NULL))
		nerr=0;
	return nerr;
}

BOOL CDoFile::IsFileExit(const CString& pathname)
{
	int length=pathname.GetLength();

	if(length<=0)
		return FALSE;

	CString _pathname=pathname;

	if(_pathname.Right(1) == '\\')
		_pathname = _pathname.Left(length-1);
	
	
	WIN32_FIND_DATA find_data = {0};

	HANDLE handle =FindFirstFile(_pathname,&find_data);
	if(handle != INVALID_HANDLE_VALUE)
	{
		if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
	}
	else
	{
		return FALSE;
	}
	FindClose(handle);
}

CString CDoFile::ParentPath(const CString& path)
{
	if(path.GetLength() < 2) return _T("");

	CString		minPath = path;
	int pos = minPath.ReverseFind('\\');
	if( pos != -1 )
		minPath = minPath.Left( pos );

	return minPath;
}

BOOL CDoFile::MyDeleteDirectory(const CString& path)
{
	CFileFind tempFind;//搜索变量
	char tempFileFind[MAX_PATH]={0};//格式化变量字符
	CString DirName=path;
	if(!DirName.IsEmpty() && DirName.Right(1) == '\\')
		DirName=DirName.Left(DirName.GetLength()-1);
	sprintf_s(tempFileFind, "%s\\*.*",DirName); //匹配格式为*.*,即该目录下的所有文件  
	CString w_tempFileFind;
	m_convert.CharToCstring(w_tempFileFind,tempFileFind);
	BOOL IsFind=tempFind.FindFile(w_tempFileFind);

	while(IsFind)
	{
		IsFind=tempFind.FindNextFile();//此函数用于遍历目录或文件时，判断当前目录下是否有下一个目录或文件
		if(!tempFind.IsDots())//每个目录下都有缺省的两个目录，名称分别为 '. '和 '.. '，分别代表本层目录和上一层目录。因此，当我们在遍历目录下文件时，需要过滤掉这两个缺省目录。
		{
			CString FoundFileName=tempFind.GetFileName();//找到的目录的名字
			if(tempFind.IsDirectory())//如果是目录
			{
				CString AddFoundFileName=DirName+_T("\\")+FoundFileName;
				MyDeleteDirectory(AddFoundFileName);//递归调用,当不是目录或者没找到exit
			}
			else
			{
				//是文件直接删除
				CString AddFounFilePath=DirName+_T("\\")+FoundFileName;
				DeleteFile(AddFounFilePath);//删除目录下文件
			}
		}
	}
	tempFind.Close();
	//删除目录
	if(!RemoveDirectory(DirName))
	{
		return FALSE;
	}
	return TRUE;
}

CString CDoFile::GetExeFullFilePath()
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

