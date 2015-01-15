#include "StdAfx.h"
#include "ConnectToBase.h"

CConnectToBase::CConnectToBase(void)
{
	m_pRecordset=NULL;
	m_pDatabase=new CADODatabase();
}

CConnectToBase::~CConnectToBase(void)
{
	if(m_pRecordset)
	{
		delete m_pRecordset;
		m_pRecordset=NULL;
	}
	if(m_pDatabase)
	{
		delete m_pDatabase;
		m_pDatabase=NULL;
	}
}

void CConnectToBase::CloseRecordset()
{
	m_pRecordset->Close();
	m_pRecordset=NULL;
}
BOOL CConnectToBase::TestConnectToDb(const CString& strIP,const CString& strAcount,const CString& strPassword,int &type)
{
	if(type==0)
	{
		m_pDatabase->SetConnectionString(_T("Driver={Oracle};Server="+strIP+";Database=Nurse;Uid="+strAcount+";Pwd="+strPassword+";"));
	}
	else if(type==1)
	{
		m_pDatabase->SetConnectionString(_T("Driver={SQL Server};Server="+strIP+";Database=Nurse;Uid="+strAcount+";Pwd="+strPassword+";"));
	}
	else if(type==2)
	{
		m_pDatabase->SetConnectionString(_T("Driver={My SQL};Server="+strIP+";Database=Nurse;Uid="+strAcount+";Pwd="+strPassword+";"));
	}
	else 
	{
		AfxMessageBox(_T("请选择数据库"));
		return FALSE;
	}
	m_pDatabase->SetConnectionMode(CADODatabase::connectModeShareDenyNone);
	m_pDatabase->SetConnectionTimeout();
	try
	{
	if(m_pDatabase->Open())
	{
	//	m_pRecordset=new CADORecordset(m_pDatabase);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	}
	catch(_com_error &e)
	{
		return FALSE;
	}
	return TRUE;
}

CADORecordset* CConnectToBase::GetADORecordset(const int& index)
{
	ConnectToDb(index);
	return m_pRecordset;
}

BOOL CConnectToBase::ConnectToDb(const int& type)
{
	if(m_pRecordset)
	{
		if(m_pRecordset->IsOpen())
			m_pRecordset->Close();
		delete m_pRecordset;
		m_pRecordset=NULL;
	}
	if(m_pDatabase->IsOpen())
		m_pDatabase->Close();
	if(type==0)
	{
		m_pDatabase->SetConnectionString(_T("Driver={Oracle};Server="+m_baseIP+";Database=Nurse;Uid="+m_baseAcount+";Pwd="+m_basePassword+";"));
	}
	else if(type==1)
	{
		m_pDatabase->SetConnectionString(_T("Driver={SQL Server};Server="+m_baseIP+";Database=Nurse;Uid="+m_baseAcount+";Pwd="+m_basePassword+";"));

	}
	else if(type==2)
	{
		m_pDatabase->SetConnectionString(_T("Driver={My SQL};Server="+m_baseIP+";Database=Nurse;Uid="+m_baseAcount+";Pwd="+m_basePassword+";"));
	}
	m_pDatabase->SetConnectionMode(CADODatabase::connectModeShareDenyNone);
	try
	{
	if(m_pDatabase->Open())
	{
		m_pRecordset=new CADORecordset(m_pDatabase);
	}
	}
	catch(_com_error &e)
	{
		AfxMessageBox(_T("连接数据库失败,请确认账号和密码或者检查数据库网络"));
		return FALSE;
	}
	return TRUE;
} 
