#include "StdAfx.h"
#include "WriteLog.h"
#include "../NurseStation/MyCommon.h"
#include "../NurseStation/MyString.h"

WriteLog::WriteLog(void)
{

}

WriteLog::~WriteLog(void)
{
}

void WriteLog::WriteErrLog(CString strErr)
{
	CString str("����: ");
	str = str + strErr + _T("\r\n");
	Write(str);
}

void WriteLog::WriteTimeErrLog(CString strErr)
{
	CString str("����: ");
	str = str + strErr;
	WriteLogWithTime(str);
}

void WriteLog::WriteDbErrLog(CString strErr)
{
	CString str(_T("����: ���ݿ����"));
	str = str + strErr;
	WriteLogWithTime(str);
}

void WriteLog::WriteLogWithTime(CString strSockLog)
{
	CTime time = CTime::GetCurrentTime();
	CString str = time.Format(_T("%Y-%m-%d %H:%M:%S")) + _T("  \t") + strSockLog + _T("\r\n");
	Write(str);
}

void WriteLog::Write(CString strLog)
{
	static int nWriteLog = MyCommon::GetProfileInt(
		_T("sys"), _T("WRITE_LOG"), 0, _T("\\SYS\\sysconfig.ini"));
	if(!nWriteLog) return;
	static CMutex mtLogFile;
	strLog += _T("\r\n");
	CHAR* szBuf = (CHAR*)malloc(strLog.GetLength()*2 + 1);
	memset(szBuf, 0, strLog.GetLength()*2 + 1);
	MyString::WChar2Char(szBuf, strLog.GetLength()*2 + 1, strLog.GetBuffer());
	CTime time = CTime::GetCurrentTime();
	static CString strLogPath = MyCommon::GetModuleDir() + _T("log");
	if(!MyCommon::PathFileExist(strLogPath))
	{
		MyCommon::CreatePath(strLogPath);
	}

	mtLogFile.Lock(20);
	CFile file;
	if(!file.Open(strLogPath + time.Format(_T("\\%Y%m%d.log")), 
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		return;
	}
	file.SeekToEnd();
	file.Write(szBuf, strlen(szBuf));
	file.Close();
	mtLogFile.Unlock();
	free(szBuf);
}
