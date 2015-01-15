#include "StdAfx.h"
#include "MyPort.h"
#include "MyCommon.h"
#include "MyString.h"

MyPort::MyPort(void)
{
}

MyPort::~MyPort(void)
{
}

USHORT MyPort::GetLocalPort(USHORT uDefaultPort)
{
	return GetPort(_T("LOCAL_PORT"), uDefaultPort);
}

USHORT MyPort::GetOpenPort(USHORT uDefaultPort)
{
	return GetPort(_T("OPEN_PORT"), uDefaultPort);
}

BOOL MyPort::SetLocalPort(CString strPort)
{
	return SetPort(_T("LOCAL_PORT"), MyString::Str2Int(strPort));
}

BOOL MyPort::SetOpenPort(CString strPort)
{
	return SetPort(_T("OPEN_PORT"), MyString::Str2Int(strPort));
}




//////////////////////////////	private		//////////////////////////////////



USHORT MyPort::GetPort(CString strKeyName, USHORT uDefaultPort)
{
	CString strAbsName = MyCommon::GetModuleDir() + _T("SYS\\port.ini");

	return MyString::Str2Int(
		MyCommon::GetProfileString(
		_T("sys"), strKeyName, 
		MyString::Int2Str(uDefaultPort),
		strAbsName));
}

BOOL MyPort::SetPort(CString strKeyName, USHORT uPort)
{
	CString strAbsName = MyCommon::GetModuleDir() + _T("SYS\\port.ini");

	return MyCommon::WriteProfileString(
		_T("sys"), strKeyName,
		MyString::Int2Str(uPort), strAbsName);
}

