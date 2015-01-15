#pragma once
#include "ConnectToHis.h"
#include "ConnectToMyDataBase.h"
#include "../NurseStation/BaseConfig.h"
#include "QueueServerDef.h"
class CReadHisData
{
public:
	CReadHisData(void);
	~CReadHisData(void);
public:
	CConnectToHis          m_connectToHis;
	CConnectToMyDataBase   m_connectToMyDataBase;
	void FlushHisDataToMyTable(int typeDataDrive);

	void SetHisDataBaseString(int dataBaseType/*数据库类型0为Orcle,1为SQL Server*/,
		CString hisIP,CString hisPort,CString hisBaseName/*数据库名*/,
		CString hisAccount/*用户名*/,CString hisPass/*密码*/);

	void SetMyDataBaseString(int dataBaseType/*数据库类型0为Orcle,1为SQL Server*/,
		CString hisIP,CString hisPort,CString hisBaseName/*数据库名*/,
		CString hisAccount/*用户名*/,CString hisPass/*密码*/);
	
	BOOL OpenHisDataBase();
	BOOL OpenMyDataBase();
	BOOL IsHisDataBaseOpen();
	BOOL IsMyDataBaseOpen();

	HANDLE m_hReadHisData;
	static DWORD WINAPI ThreadReadData(LPVOID pParam);

	BOOL StartReadDataThread();
	CBaseConfig m_baseConfig;
	int m_typeDataDrive;
};
