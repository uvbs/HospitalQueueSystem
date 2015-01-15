#include "StdAfx.h"
#include "ReadHisOffice.h"

CReadHisOffice::CReadHisOffice(void)
{
}

CReadHisOffice::~CReadHisOffice(void)
{
}

void CReadHisOffice::FlushHisOfficeToMyTable()
{
	try
	{
		if(m_connectToHis.IsHisDataBaseOpen() && m_connectToMyDataBase.IsMyDataBaseOpen())
		{
			CString office_id,office_name,office_callname,have_office_id;
			CString numstart=_T("-1");
			CString numend=_T("-1");
			CString amlimit=_T("-1");
			CString pmlimit=_T("-1");
			CString daylimit=_T("-1");

			CString insertSql=_T("");
			CString hissql=_T("select * from HisForBjlb_Office");
			CString mysql=_T("");


			if(m_connectToHis.m_pHisRecordset->Open(hissql,CADORecordset::openQuery))
			{
				if(!m_connectToHis.m_pHisRecordset->IsBOF())
				{
					while(!m_connectToHis.m_pHisRecordset->IsEOF())
					{
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("office_id"),office_id);
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("office_name"),office_name);
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("office_callname"),office_callname);

						mysql.Format(_T("select * from office where office_id='%s'"),office_id);

						if(m_connectToMyDataBase.m_pMyRecordset->Open(mysql,CADORecordset::openQuery))
						{
							if(m_connectToMyDataBase.m_pMyRecordset->IsBOF())
							{
								insertSql.Format(_T("insert into office(office_id,name,call_name,numstart,numend,amlimit,pmlimit,daylimit) values('%s','%s','%s','%s','%s','%s','%s','%s')"),
									office_id,office_name,office_callname,numstart,numend,amlimit,pmlimit,daylimit);
								m_connectToMyDataBase.m_myDataBase.Execute(insertSql);
							}
							else
							{
								CString myTableOfficeName,myTableCallName;
								m_connectToMyDataBase.m_pMyRecordset->
									GetFieldValue(_T("name"),myTableOfficeName);
								m_connectToMyDataBase.m_pMyRecordset->
									GetFieldValue(_T("call_name"),myTableCallName);
								if(myTableCallName!=office_callname || myTableOfficeName!=office_name)
								{
									CString updatesql=_T("");
									updatesql.Format(_T("update office set name='%s',call_name='%s'"),
										office_name,office_callname);
									m_connectToMyDataBase.m_myDataBase.Execute(updatesql);
								}
							}
							m_connectToMyDataBase.m_pMyRecordset->Close();
						}
						m_connectToHis.m_pHisRecordset->MoveNext();
					}
				}
				m_connectToHis.m_pHisRecordset->Close();
			}
		}
	}
	catch (_com_error& e)
	{

	}
	catch (...)
	{

	}
}


BOOL CReadHisOffice::OpenHisDataBase()
{
	CString hisDataDrive=m_baseConfig.GetHisDataDrive();
	CString hisBaseAccount=m_baseConfig.GetHisAcount();
	CString hisBaseIP=m_baseConfig.GetHisServerIP();
	CString hisPort=m_baseConfig.GetHisPort();
	CString hisBaseName=m_baseConfig.GetHisBaseName();
	CString hisPass=m_baseConfig.GetHisPass();
	
	if(hisDataDrive==_T("Oracle"))
	{
		m_connectToHis.SetConnectString(typeOracle,hisBaseIP,hisPort,hisBaseName,
			hisBaseAccount,hisPass);
	}
	else if(hisDataDrive==_T("SQL Server"))
	{
		m_connectToHis.SetConnectString(typeSqlServer,hisBaseIP,hisPort,hisBaseName,
			hisBaseAccount,hisPass);
	}

	if(m_connectToHis.OpenHisDataBase())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CReadHisOffice::OpenMyDataBase()
{
	CString dataDrive=m_baseConfig.GetDataDrive();
	CString baseIP=m_baseConfig.GetServerIP();
	CString baseAccount=m_baseConfig.GetServerAcount();
	CString basePass=m_baseConfig.GetServerPassword();
	
	m_connectToMyDataBase.SetConnectString(typeSqlServer,baseIP,_T(""),
		_T("Nurse"),baseAccount,basePass);

	if(m_connectToMyDataBase.OpenMyDataBase())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CReadHisOffice::CompareDeleteOffice()
{
	try
	{
		if(m_connectToMyDataBase.IsMyDataBaseOpen() && m_connectToHis.IsHisDataBaseOpen())
		{
			CString mysql=_T("select office_id from office");
			CString hissql,deletesql;
			CString office_id;

			if(m_connectToMyDataBase.m_pMyRecordset->Open(mysql,CADORecordset::openQuery))
			{
				if(!m_connectToMyDataBase.m_pMyRecordset->IsBOF())
				{
					while(!m_connectToMyDataBase.m_pMyRecordset->IsEOF())
					{
						m_connectToMyDataBase.m_pMyRecordset->GetFieldValue(_T("office_id"),office_id);

						hissql.Format(_T("select * from hisforbjlb_office where office_id='%s'"),office_id);
						if(m_connectToHis.m_pHisRecordset->Open(hissql,CADORecordset::openQuery))
						{
							if(m_connectToHis.m_pHisRecordset->IsBOF())
							{
								deletesql.Format(_T("delete from office where office_id='%s'"),office_id);
								m_connectToMyDataBase.m_myDataBase.Execute(deletesql);
								deletesql.Format(_T("delete from nurse_office where office_id='%s'"),office_id);
								m_connectToMyDataBase.m_myDataBase.Execute(deletesql);
								deletesql.Format(_T("delete from consult where office_id='%s'"),office_id);
								m_connectToMyDataBase.m_myDataBase.Execute(deletesql);
							}
							m_connectToHis.m_pHisRecordset->Close();
						}
						m_connectToMyDataBase.m_pMyRecordset->MoveNext();
					}
				}
				m_connectToMyDataBase.m_pMyRecordset->Close();
			}
		}
	}
	catch (_com_error& e)
	{

	}
	catch (...)
	{

	}
}

DWORD WINAPI CReadHisOffice::ReadHisOfficeThread(LPVOID pParam)
{
	CReadHisOffice* pThis=(CReadHisOffice*)pParam;
	while(1)
	{
		pThis->CompareDeleteOffice();
		pThis->FlushHisOfficeToMyTable();
		Sleep(2*1300);
	}
	return 0;
}

BOOL CReadHisOffice::StartReadHisOfficeThread()
{
	if(OpenMyDataBase() && OpenHisDataBase())
	{
		m_hReadHisOffice=CreateThread(NULL,0,ReadHisOfficeThread,this,NULL,0);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}