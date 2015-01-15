#include "StdAfx.h"
#include "ReadHisDoctor.h"

CReadHisDoctor::CReadHisDoctor(void)
{
}

CReadHisDoctor::~CReadHisDoctor(void)
{
}


void CReadHisDoctor::FlushHisDoctorToMyTable()
{
	try
	{
		if(m_connectToHis.IsHisDataBaseOpen() && m_connectToMyDataBase.IsMyDataBaseOpen())
		{
			CString hisSql=_T("select * from hisforbjlb_doctor");
			CString mySql=_T("");
			CString doctor_id,doctor_name,doctor_position,doctor_photo,office_id;
			int doctor_gender=-1;
			CString insertSql=_T("");


			if(m_connectToHis.m_pHisRecordset->Open(hisSql,CADORecordset::openQuery))
			{
				if(!m_connectToHis.m_pHisRecordset->IsBOF())
				{
					while(!m_connectToHis.m_pHisRecordset->IsEOF())
					{
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("doctor_id"),doctor_id);
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("doctor_name"),doctor_name);
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("doctor_gender"),doctor_gender);
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("doctor_position"),doctor_position);
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("doctor_photo"),doctor_photo);
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("office_id"),office_id);

						mySql.Format(_T("select * from doctor where doctor_id='%s'"),doctor_id);
						if(m_connectToMyDataBase.m_pMyRecordset->Open(mySql,CADORecordset::openQuery))
						{
							if(m_connectToMyDataBase.m_pMyRecordset->IsBOF())
							{
								insertSql.Format(_T("insert into Doctor(doctor_id,name,gender,position,photo,office_id) values('%s','%s','%d','%s','%s','%s')"),
									doctor_id,doctor_name,doctor_gender,doctor_position,doctor_photo,office_id);
								m_connectToMyDataBase.m_myDataBase.Execute(insertSql);
							}
							else
							{
								CString myDoctorName,myDoctorPosition,myDoctorPhoto,myOfficeId;
								int mygender=-1;
								m_connectToMyDataBase.m_pMyRecordset->GetFieldValue(_T("name"),myDoctorName);
								m_connectToMyDataBase.m_pMyRecordset->GetFieldValue(_T("gender"),mygender);
								m_connectToMyDataBase.m_pMyRecordset->GetFieldValue(_T("position"),myDoctorPosition);
								m_connectToMyDataBase.m_pMyRecordset->GetFieldValue(_T("photo"),myDoctorPhoto);
								m_connectToMyDataBase.m_pMyRecordset->GetFieldValue(_T("office_id"),myOfficeId);

								if(myDoctorName!=doctor_name || myDoctorPhoto!=doctor_photo
									|| myDoctorPosition!=doctor_position || mygender!=doctor_gender
									|| myOfficeId!=office_id)
								{
									CString updatesql=_T("");
									updatesql.Format(_T("update doctor set name='%s',gender='%d',position='%s',photo='%s',office_id='%s' where doctor_id='%s'"),
										doctor_name,doctor_gender,doctor_position,doctor_photo,office_id,doctor_id);
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


BOOL CReadHisDoctor::OpenHisDataBase()
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

BOOL CReadHisDoctor::OpenMyDataBase()
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

void CReadHisDoctor::CompareDeleteDoctor()
{
	try
	{
		if(m_connectToHis.IsHisDataBaseOpen() && m_connectToMyDataBase.IsMyDataBaseOpen())
		{
			CString mysql=_T("select doctor_id from doctor");
			CString hissql,deletesql;
			CString doctor_id;
			if(m_connectToMyDataBase.m_pMyRecordset->Open(mysql,CADORecordset::openQuery))
			{
				if(!m_connectToMyDataBase.m_pMyRecordset->IsBOF())
				{
					while(!m_connectToMyDataBase.m_pMyRecordset->IsEOF())
					{
						m_connectToMyDataBase.m_pMyRecordset->GetFieldValue(_T("doctor_id"),doctor_id);

						hissql.Format(_T("select * from hisforbjlb_doctor where doctor_id='%s'"),doctor_id);
						if(m_connectToHis.m_pHisRecordset->Open(hissql,CADORecordset::openQuery))
						{
							if(m_connectToHis.m_pHisRecordset->IsBOF())
							{
								deletesql.Format(_T("delete doctor where doctor_id='%s'"),doctor_id);
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


DWORD WINAPI CReadHisDoctor::ReadHisDoctorThread(LPVOID pParam)
{
	CReadHisDoctor* pThis=(CReadHisDoctor*)pParam;
	while(1)
	{
		pThis->CompareDeleteDoctor();
		pThis->FlushHisDoctorToMyTable();
		Sleep(2*1200);
	}
	return 0;
}

BOOL CReadHisDoctor::StartReadHisDoctorThread()
{
	if(OpenMyDataBase() && OpenHisDataBase())
	{
		m_hReadHisDoctor=CreateThread(NULL,0,ReadHisDoctorThread,this,0,NULL);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}