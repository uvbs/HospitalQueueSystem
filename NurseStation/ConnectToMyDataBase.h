#pragma once
#include "../NurseStation/ado2.h"
class CConnectToMyDataBase
{
public:
	CConnectToMyDataBase(void);
	~CConnectToMyDataBase(void);
public:
	CString m_connectString;
	CADODatabase m_myDataBase;
	void SetConnectString(int dataBaseType/*数据库类型0为Orcle,1为SQL Server*/,
		CString hisIP,CString hisPort,CString hisBaseName/*数据库名*/,
		CString hisAccount/*用户名*/,CString hisPass/*密码*/);
	BOOL OpenMyDataBase();
	BOOL IsMyDataBaseOpen();
	CADORecordset* m_pMyRecordset;
};
