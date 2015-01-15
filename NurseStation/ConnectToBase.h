#pragma once


#include "ado2.h"
class CConnectToBase  
{
public:
	CConnectToBase();
	virtual ~CConnectToBase();
public:
	BOOL ConnectToDb(const int& type);
	BOOL TestConnectToDb(const CString& strIP,const CString& strAcount,const CString& strPassword,int &type);
	void SetBaseName(const CString& basename){m_baseName=basename;}
	CString GetBaseName(){return m_baseName;}
	void SetBaseIP(const CString& ip){m_baseIP=ip;}
	CString GetBaseIP(){return m_baseIP;}

	void SetBaseAcount(const CString& acount){m_baseAcount=acount;}
	CString GetBaseAcount(){return m_baseAcount;}

	void SetBasePassword(const CString& password){m_basePassword=password;}
	CString GetBasePasswor(){return m_basePassword;}

	CADODatabase* GetADODatabase(){return m_pDatabase;}
	CADORecordset* GetADORecordset(const int& index);
	CADORecordset* GetADORecordset(){return m_pRecordset;}
	void CloseRecordset();


protected:
	CString m_baseIP;
	CString m_baseAcount;
	CString m_basePassword;
	CString m_baseName;
	CADODatabase *m_pDatabase;
	CADORecordset* m_pRecordset;
};