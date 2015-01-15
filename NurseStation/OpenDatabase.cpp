#include "StdAfx.h"
#include "OpenDatabase.h"
#include "BaseConfig.h"

OpenDatabase::OpenDatabase(void)
{
}

OpenDatabase::~OpenDatabase(void)
{
}

BOOL OpenDatabase::OpenDB(CADODatabase &db)
{
	CBaseConfig config;
	CString strSqlConn = _T("Driver={")+config.GetDataDrive()+_T("};Server=")
		+config.GetServerIP()+_T(";Database=Nurse;Uid=")
		+config.GetServerAcount()+_T(";Pwd=")+config.GetServerPassword()+_T(";");
	db.SetConnectionString(strSqlConn);
	try
	{
		db.Open();
	}catch(_com_error& e)
	{
		AfxMessageBox(_T("数据库连接失败,可能是以下原因：\n\t1.本机与数据库的网络连接已断开；\n\t2.数据库服务未启动；\n\t3.数据库配置文件不存在或配置错误;\n\t4.数据库用户名密码错误。")
			,MB_OK|MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}
