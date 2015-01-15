#pragma once
#include "afxmt.h"

class WriteLog
{
public:
	WriteLog(void);
	~WriteLog(void);

	static void Write(CString strLog);
	static void WriteLogWithTime(CString strSockLog);
	static void WriteErrLog(CString strErr);
	static void WriteTimeErrLog(CString strErr);
	static void WriteDbErrLog(CString strErr);
};
