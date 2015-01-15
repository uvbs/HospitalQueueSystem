// IniFile.h: interface for the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__61921152_E664_4427_80BA_9C1B580D7A4B__INCLUDED_)
#define AFX_INIFILE_H__61921152_E664_4427_80BA_9C1B580D7A4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIniFile  
{
public:
	CIniFile(CString strFileName):m_strFileName(strFileName)
	{
	}
	virtual ~CIniFile();

public:
	BOOL SetFileName(LPCTSTR lpFileName);   //设置文件名
	CString GetFileName();     //获取文件名
	BOOL SetValue(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue, bool bCreate = true);  //设置键值，bCreate是指段名及键名未存在时，是否创建键值
	CString GetValue(LPCTSTR lpSection, LPCTSTR lpKey);   //得到键值
	BOOL DelSection(LPCTSTR lpSection);    //删除段名
	BOOL DelKey(LPCTSTR lpSection, LPCTSTR lpKey);   //删除键名

	int GetSections(CStringArray& arrSection);   //枚举全部的段名
	int GetKeys(CStringArray& arrKey, LPCTSTR lpSection);      //枚举全部的键值
	int GetKeyValues(CStringArray& arrKey, CStringArray& arrValue, LPCTSTR lpSection);   //枚举出一段内的全部键名及值

	BOOL DelAllSections();  //删除所有的段名 
//	CString GetExeFullFilePath();//
private:
	CString m_strFileName;
};

#endif // !defined(AFX_INIFILE_H__61921152_E664_4427_80BA_9C1B580D7A4B__INCLUDED_)
