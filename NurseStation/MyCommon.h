#pragma once

class MyCommon
{
public:
	MyCommon(void);
	~MyCommon(void);

	enum DirFileGetType{
		gettypePath = (int)0x0000,
		gettypeName = (int)0x0001,
		gettypeTitle = (int)0x0002,
		gettypeURL = (int)0x0003
	};

	/**
	desc:		获取当前程序所在路径,最后包含'\'

	return:		返回 当前程序所在路径
	*/
	static CString GetModuleDir(void);

	static CString GetModuleFileName(void);

	/**
	desc:			获取某文件夹下的文件全路径名

	strPath:		文件夹路径
	strarrFileList: 保存文件列表的字符串数组
	strSuffix:		文件后缀名，多后缀以"|"分隔

	return:			文件数
	*/
	static int GetFilesAbsNameInDir(CString strPath, CStringArray& strarrFileList, CString strSuffix);

	/**
	desc:			获取某文件夹下的文件名

	strPath:		文件夹路径
	strarrFileList: 保存文件列表的字符串数组
	strSuffix:		文件后缀名，多后缀以"|"分隔

	return:			文件数
	*/
	static int GetFilesNameInDir(CString strPath, CStringArray& strarrFileList, CString strSuffix);

	/**
	desc:			获取某文件夹下的文件标题(不包含后缀)

	strPath:		文件夹路径
	strarrFileList: 保存文件列表的字符串数组
	strSuffix:		文件后缀名，多后缀以"|"分隔

	return:			文件数
	*/
	static int GetFilesTitleInDir(CString strPath, CStringArray& strarrFileList, CString strSuffix);

	static BOOL GetLocalHostNameAndIP(CString& strHostName, CString& strHostIP);

	static CString GetLocalHostName();

	static CString GetLocalIP();

	static BOOL IsNumbers(CString strNum);

	static BOOL PathFileExist(CString strPath);

	static BOOL CreatePath(CString strPath);

	static BOOL CreateFile(CString strFile);

	static CString GetPathFromAbsName(CString strAbsName);

	static CString GetNameFromAbsName(CString strAbsName);

	static BOOL IsAbsPath(CString strAbsPath);

	static CString GetProfileString(CString strAppName, CString strKeyName, 
		CString strDefault, CString strFileName);

	static int GetProfileInt(CString strAppName, CString strKeyName, 
		int nDefault, CString strFileName);

	static BOOL WriteProfileString(CString strAppName, CString strKeyName, 
		CString strProfile, CString strFileName);

	static CString GetAbsName(CString strName);

	static BOOL WriteRegString(HKEY hKeyRoot, CString strSubKey, 
		CString strValueName, CString strData);

	static BOOL GetRegString(HKEY hKeyRoot, CString strSubKey, 
		CString strValueName, CString& strData);

	static CString GetRegString(HKEY hKeyRoot, CString strSubKey, 
		CString strValueName);

	static BOOL AddAutoRun(CString strValueName);

	static BOOL FindProcessByName(CString strName, DWORD* pdwProcessId);

	static BOOL KillProcessByName(CString strName);

	//static void ListProcess();

	///////////////////////////////		private method		/////////////////////////////////
private:
	static BOOL _CreatePath(CString strPath);
	static int GetFilesInDir(CString strPath, CStringArray& strarrFileList, CString strSuffix, int nGetType);
};
