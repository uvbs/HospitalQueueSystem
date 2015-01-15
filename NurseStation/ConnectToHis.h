#pragma once
#include "../NurseStation/ado2.h"
class CConnectToHis
{
public:
	CConnectToHis(void);
	~CConnectToHis(void);
public:
	CADODatabase m_hisDataBase;
	CString m_connectString;
	void SetConnectString(int dataBaseType/*数据库类型0为Orcle,1为SQL Server*/,
		CString hisIP,CString hisPort,CString hisBaseName/*数据库名*/,
		CString hisAccount/*用户名*/,CString hisPass/*密码*/);
	BOOL OpenHisDataBase();
	BOOL IsHisDataBaseOpen();
	CADORecordset* m_pHisRecordset;
};
