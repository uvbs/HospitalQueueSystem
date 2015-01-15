#include "StdAfx.h"
#include "MyCommon.h"
#include "MyString.h"
#include <shlwapi.h>
#include <tlhelp32.h>

MyCommon::MyCommon(void)
{
}

MyCommon::~MyCommon(void)
{
}

CString MyCommon::GetModuleDir(void)
{
	TCHAR tszModulePath[MAX_PATH] = {0};
	::GetModuleFileName(NULL, tszModulePath, MAX_PATH);
	CString strModulePath(tszModulePath);
	strModulePath.Replace('/', '\\');
	int nPos = strModulePath.ReverseFind(_T('\\'));
	if(nPos < 0)
	{
		return _T("");
	}
	else
	{
		return strModulePath.Left(nPos+1);
	}
}

CString MyCommon::GetModuleFileName(void)
{
	TCHAR tszModulePath[MAX_PATH] = {0};
	::GetModuleFileName(NULL, tszModulePath, MAX_PATH);

	return tszModulePath;
}

int MyCommon::GetFilesAbsNameInDir(CString strPath, CStringArray& strarrFileList, CString strSuffix)
{
	return GetFilesInDir(strPath, strarrFileList, strSuffix, gettypePath);
}

int MyCommon::GetFilesNameInDir(CString strPath, CStringArray& strarrFileList, CString strSuffix)
{
	return GetFilesInDir(strPath, strarrFileList, strSuffix, gettypeName);
}

int MyCommon::GetFilesTitleInDir(CString strPath, CStringArray& strarrFileList, CString strSuffix)
{
	return GetFilesInDir(strPath, strarrFileList, strSuffix, gettypeTitle);
}

BOOL MyCommon::GetLocalHostNameAndIP(CString& strHostName, CString& strHostIP)
{
	BOOL bRes = TRUE;
	WSADATA wsaData;
	CHAR szHostName[256] = {0};
	CHAR* szIP = NULL;
	PHOSTENT phostinfo; 
	if ( WSAStartup( MAKEWORD(2,2), &wsaData ) == 0 ) 
	{  
		if( gethostname( szHostName, 256) == 0) 
		{ 
			if((phostinfo = gethostbyname(szHostName)) != NULL) 
			{ //这些就是获得IP的函数
				szIP= inet_ntoa (*(struct in_addr *)*phostinfo->h_addr_list);
			}
			else
			{
				bRes = FALSE;
			}
		}
		else
		{
			bRes = FALSE;
		}
		WSACleanup();
	}
	else
	{
		bRes = FALSE;
	}
	//WCHAR* wszHostName = new WCHAR[strlen(szHostName) + 2];
	//Char2WChar(wszHostName, strlen(szHostName)+2, szHostName);
	//strHostName = wszHostName;
	//delete[] wszHostName;
	strHostName.Format(_T("%s"), szHostName);
	if(szIP)
	{
		//WCHAR* wszIP = new WCHAR[strlen(szIP) + 2];
		//Char2WChar(wszIP, strlen(szIP)+2, szIP);
		//strHostIP = wszIP;
		//delete[] wszIP;
		strHostIP.Format(_T("%s"), szIP);
	}
	else
	{
		strHostIP.Empty();
	}

	return bRes;
}

CString MyCommon::GetLocalIP()
{
	CString strHostName(_T("")), strIP(_T(""));
	if(!GetLocalHostNameAndIP(strHostName, strIP))
	{
		strIP.Empty();
	}

	return strIP;
}

CString MyCommon::GetLocalHostName()
{
	CString strHostName(_T("")), strIP(_T(""));
	GetLocalHostNameAndIP(strHostName, strIP);

	return strHostName;
}

BOOL MyCommon::IsNumbers(CString strNum)
{
	if(strNum.IsEmpty()) return FALSE;
	for(int i = 0; i < strNum.GetLength(); i++)
	{
		if(strNum.GetAt(i) < '0' || strNum.GetAt(i) > '9')
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL MyCommon::PathFileExist(CString strPath)
{
	if(!IsAbsPath(strPath))
	{
		if(strPath.GetLength() > 0)
		{
			if(strPath.GetAt(0) == _T('/'))
			{
				strPath.Delete(0);
			}
		}
		strPath = GetModuleDir() + strPath;
	}
	return ::PathFileExists(strPath);
}

CString MyCommon::GetAbsName(CString strName)
{
	if(!IsAbsPath(strName))
	{
		if(strName.GetLength() > 0)
		{
			if(strName.GetAt(0) == _T('/'))
			{
				strName.Delete(0);
			}
		}
		strName = GetModuleDir() + strName;
	}
	return strName;
}

BOOL MyCommon::CreatePath(CString strPath)
{
	strPath.Replace('\\', '/');
	return _CreatePath(strPath);
}

BOOL MyCommon::CreateFile(CString strFile)
{
	CString strFileName = GetNameFromAbsName(strFile);
	CString strPath = GetPathFromAbsName(strFile);
	if(!IsAbsPath(strPath))
	{
		if(strPath.GetLength() > 0)
		{
			if(strPath.GetAt(0) == _T('/'))
			{
				strPath.Delete(0);
			}
		}
		strPath = GetModuleDir() + strPath;
	}
	if(!MyCommon::CreatePath(strPath))
	{
		return FALSE;
	}
	HANDLE hFile = ::CreateFile(strPath + strFileName, GENERIC_ALL, FILE_SHARE_READ,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	::CloseHandle(hFile);

	return TRUE;
}

CString MyCommon::GetPathFromAbsName(CString strAbsName)
{
	strAbsName.Replace('\\', '/');
	int nPos = strAbsName.ReverseFind(_T('/'));
	if(nPos < 0)
	{
		return CString();
	}
	else
	{
		return strAbsName.Left(nPos+1);
	}
}

CString MyCommon::GetNameFromAbsName(CString strAbsName)
{
	strAbsName.Replace('\\', '/');
	int nPos = strAbsName.ReverseFind(_T('/'));
	if(nPos < 0)
	{
		return strAbsName;
	}
	else
	{
		return strAbsName.Mid(nPos+1);
	}
}

BOOL MyCommon::IsAbsPath(CString strAbsPath)
{
	if(strAbsPath.GetLength() < 2) return FALSE;
	if(strAbsPath.GetAt(1) != ':') return FALSE;
	if(strAbsPath.GetLength() < 3)
	{
		strAbsPath += _T("/");
	}
	return ::PathFileExists(strAbsPath.Left(3));
}

CString MyCommon::GetProfileString(CString strAppName, 
												   CString strKeyName, 
												   CString strDefault, 
												   CString strFileName)
{
	strFileName = GetAbsName(strFileName);
	if(!PathFileExist(strFileName))
	{
		if(!MyCommon::CreateFile(strFileName))
		{
			return strDefault;
		}
	}
	TCHAR wszPort[4096] = {0};
	DWORD dwRet;
	dwRet = ::GetPrivateProfileString(
		strAppName, strKeyName, NULL, wszPort, 4096, strFileName);
	if(dwRet == 0)
	{
		//::wcscpy_s(wszPort, 11, Int2Str(MY_DEFAULT_SOCKET_PORT));
		::WritePrivateProfileString(strAppName, strKeyName, strDefault, strFileName);

		return strDefault;
	}

	return wszPort;
}

int MyCommon::GetProfileInt(CString strAppName, 
											CString strKeyName, 
											int nDefault, 
											CString strFileName)
{
	//int nRet = nDefault;
	strFileName = GetAbsName(strFileName);
	if(!PathFileExist(strFileName))
	{
		if(!MyCommon::CreateFile(strFileName))
		{
			return nDefault;
		}
	}

	TCHAR wszPort[32] = {0};
	DWORD dwRet = ::GetPrivateProfileString(
		strAppName, strKeyName, NULL, wszPort, 32, strFileName);
	if(dwRet == 0)
	{
		//::wcscpy_s(wszPort, 11, Int2Str(MY_DEFAULT_SOCKET_PORT));
		CString strNum;
		strNum.Format(_T("%d"), nDefault);
		::WritePrivateProfileString(
			strAppName, strKeyName, strNum, strFileName);

		return nDefault;
	}
	else
	{
		return ::GetPrivateProfileInt(
			strAppName, strKeyName, nDefault, strFileName);
	}
}

BOOL MyCommon::WriteProfileString(CString strAppName,
												  CString strKeyName,
												  CString strProfile,
												  CString strFileName)
{
	strFileName = GetAbsName(strFileName);
	if(!PathFileExist(strFileName))
	{
		if(!MyCommon::CreateFile(strFileName))
		{
			return FALSE;
		}
	}
	return ::WritePrivateProfileString(
		strAppName, strKeyName, strProfile, strFileName);
}

BOOL MyCommon::WriteRegString(HKEY hKeyRoot, CString strSubKey, 
				 CString strValueName, CString strData)
{
	HKEY hKey;
	LONG ret = ::RegOpenKeyEx(hKeyRoot, 
		strSubKey, 0, KEY_WRITE, &hKey);
	if(ERROR_SUCCESS != ret)
	{
		ret = ::RegCreateKey(hKeyRoot, strSubKey, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			return FALSE;
		}
		else
		{
			ret = ::RegOpenKeyEx(hKeyRoot, 
				strSubKey, 0, KEY_WRITE, &hKey);
		}
	}

	ret = ::RegSetValueEx(hKey, strValueName, 0, REG_SZ,
		(const BYTE*)strData.GetBuffer(), strData.GetLength()*2+1);
	if(ERROR_SUCCESS != ret)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL MyCommon::GetRegString(HKEY hKeyRoot, CString strSubKey, 
							   CString strValueName, CString& strData)
{
	BOOL bRet = FALSE;
	HKEY hKey;
	LONG ret = ::RegOpenKeyEx(hKeyRoot, 
		strSubKey, 0, KEY_READ, &hKey);
	if(ERROR_SUCCESS == ret)
	{
		WCHAR wszTemp[1024] = {0};
		DWORD dType = REG_SZ;
		DWORD dLength = 1024;
		ret = ::RegQueryValueEx(hKey, strValueName, NULL, &dType, (LPBYTE)wszTemp, &dLength);
		::RegCloseKey(hKey);
		if(ERROR_SUCCESS == ret)
		{
			strData.Format(_T("%s"), wszTemp);
			bRet = TRUE;
		}
	}

	return bRet;
}

CString MyCommon::GetRegString(HKEY hKeyRoot, CString strSubKey, 
				  CString strValueName)
{
	CString strData = _T("");

	HKEY hKey;
	LONG ret = ::RegOpenKeyEx(hKeyRoot, 
		strSubKey, 0, KEY_READ, &hKey);
	if(ERROR_SUCCESS == ret)
	{
		WCHAR wszTemp[1024] = {0};
		DWORD dType = REG_SZ;
		DWORD dLength = 1024;
		ret = ::RegQueryValueEx(hKey, strValueName, NULL, &dType, (LPBYTE)wszTemp, &dLength);
		::RegCloseKey(hKey);
		if(ERROR_SUCCESS == ret)
		{
			strData.Format(_T("%s"), wszTemp);
		}
	}

	return strData;
}

BOOL MyCommon::AddAutoRun(CString strValueName)
{
	CString strPath = GetModuleFileName();
	CString strPathGet = GetRegString(HKEY_LOCAL_MACHINE, 
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 
		strValueName);

	if(strPathGet.CompareNoCase(strPath) == 0)
	{
		return TRUE;
	}

	return WriteRegString(HKEY_LOCAL_MACHINE,
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 
		strValueName, strPath);
}

BOOL MyCommon::FindProcessByName(CString strName, DWORD* pdwProcessId)
{
	BOOL bSuc = FALSE;
	HANDLE hSnapshot;
	hSnapshot=::CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS,0);// 创建系统快照
	if(INVALID_HANDLE_VALUE != hSnapshot)
	{
		PROCESSENTRY32 pe = {0};
		pe.dwSize = sizeof(PROCESSENTRY32);
		if(::Process32First(hSnapshot,&pe))
		{
			do 
			{
				if(strName.CompareNoCase(pe.szExeFile) == 0)
				{
					if(pdwProcessId)
					{
						*pdwProcessId = pe.th32ProcessID;
					}
					bSuc = TRUE;
				}
			} while (::Process32Next(hSnapshot, &pe));
		}
		::CloseHandle(hSnapshot);
	}

	return bSuc;
}

BOOL MyCommon::KillProcessByName(CString strName)
{
	BOOL bRet = FALSE;
	HANDLE hSnapshot;
	hSnapshot=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);// 创建系统快照
	if(INVALID_HANDLE_VALUE != hSnapshot)
	{
		PROCESSENTRY32 pe = {0};
		pe.dwSize = sizeof(PROCESSENTRY32);
		if(::Process32First(hSnapshot,&pe))
		{
			do 
			{
				if(strName.CompareNoCase(pe.szExeFile) == 0)
				{
					//if(::TerminateProcess(::OpenProcess(
					//	PROCESS_ALL_ACCESS, 0, pe.th32ProcessID), 0))
					if(::TerminateProcess(::OpenProcess(
						PROCESS_TERMINATE, 0, pe.th32ProcessID), 0))
					{
						bRet = TRUE;
						continue;
					}
				}
			} while (::Process32Next(hSnapshot, &pe));
		}
		::CloseHandle(hSnapshot);
	}

	return bRet;
}
/*
void MyCommon::ListProcess()
{
	HANDLE hSnapshot;
	hSnapshot=::CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS,0);// 创建系统快照
	if(INVALID_HANDLE_VALUE != hSnapshot)
	{
		PROCESSENTRY32 pe = {0};
		pe.dwSize = sizeof(PROCESSENTRY32);
		if(::Process32First(hSnapshot,&pe))
		{
			do 
			{
				::wprintf(L"ID: %4d    ParentID: %4d    Priority: %2d    Name: %s\n",
					pe.th32ProcessID, pe.th32ParentProcessID,
					pe.pcPriClassBase, pe.szExeFile);
			} while (::Process32Next(hSnapshot, &pe));
			printf("\n");
		}
		::CloseHandle(hSnapshot);
	}
}
*/




///////////////////////////////		private method		/////////////////////////////////
BOOL MyCommon::_CreatePath(CString strPath)
{
	if(::PathFileExists(strPath))
	{
		return TRUE;
	}
	else
	{
		int iIndex = strPath.ReverseFind('/');
		if(iIndex < 2)
		{
			return FALSE;
		}
		CString strParent = strPath.Left(iIndex);
		if(::PathFileExists(strParent))
		{
			return ::CreateDirectory(strPath, NULL);
		}
		else
		{
			if(_CreatePath(strParent))
			{
				return _CreatePath(strPath);
			}
			else
			{
				return FALSE;
			}
		}
	}
}

int MyCommon::GetFilesInDir(CString strPath, CStringArray& strarrFileList, CString strSuffix, int nGetType)
{
	int nCount = 0;
	BOOL bAll = FALSE;
	CStringArray arrstrSuffix;
	if(strSuffix.IsEmpty() || strSuffix == _T(".*") || strSuffix == _T("*"))
	{
		bAll = TRUE;
	}
	else
	{
		if(!MyString::Split(strSuffix, arrstrSuffix, _T("|")))
		{
			bAll = TRUE;
		}
		else
		{
			for(int i = 0; i < arrstrSuffix.GetCount(); i++)
			{
				if(arrstrSuffix[i] == _T(".*") || arrstrSuffix[i] == _T("*"))
				{
					bAll = TRUE;
					break;
				}
			}
		}
	}

	CFileFind finder;
	BOOL bFound = finder.FindFile(strPath + _T("\\*.*"));
	for(; bFound; )
	{
		bFound = finder.FindNextFile();
		if(finder.IsDots()) continue;
		BOOL bAdd = FALSE;
		if(bAll)
		{
			bAdd = TRUE;
		}
		else
		{
			//CString strFilePath = finder.GetFilePath();
			CString strFileSuffix = finder.GetFilePath().Mid(
				finder.GetFilePath().ReverseFind('.'));
			for(int i = 0; i < arrstrSuffix.GetCount(); i++)
			{
				if(arrstrSuffix[i].CompareNoCase(strFileSuffix) == 0 || 
					arrstrSuffix[i].CompareNoCase(strFileSuffix.Mid(1)) == 0)
				{
					bAdd = TRUE;
					break;
				}
			}
		}
		if(bAdd)
		{
			++nCount;
			CString strAdd;
			switch(nGetType)
			{
			case gettypePath:
				{
					strAdd = finder.GetFilePath();
				}
				break;
			case gettypeName:
				{
					strAdd = finder.GetFileName();
				}
				break;
			case gettypeTitle:
				{
					strAdd = finder.GetFileTitle();
				}
				break;
			case gettypeURL:
				{
					strAdd = finder.GetFileURL();
				}
				break;
			default:
				{
					strAdd = finder.GetFilePath();
				}
			}
			strarrFileList.Add(strAdd);
		}
	}
	finder.Close();

	return nCount;
}

