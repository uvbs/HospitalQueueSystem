#include "StdAfx.h"
#include "ReadHisData.h"

CReadHisData::CReadHisData(void)
{
	m_typeDataDrive=-1;
}

CReadHisData::~CReadHisData(void)
{
}

void CReadHisData::FlushHisDataToMyTable(int typeDataDrive)
{
	try
	{
		CString office_id=_T("4");
		if(IsHisDataBaseOpen() && IsMyDataBaseOpen())
		{
			CString serial_id,patient_name,gender,register_time;
			CString queue_id=_T("");
			int month=0;
			BOOL flag=FALSE;
			CString insertSql=_T("");

			CString Hissql=_T("");
			CString Searchyessql=_T("");
			CString Delyessql=_T("");

			switch(typeDataDrive)
			{
			case typeOracle:

				break;
			case typeSqlServer:
				Hissql=_T("select 姓名,rtrim(ltrim(性别))性别,月份,输液号,就诊号,登记时间 from emr_outpatient_patient where convert(varchar(10),getdate(),120)=substring(登记时间,1,10)");
				Searchyessql=_T("select * from hisforbjlb_patient where convert(varchar(10),getdate(),120)<>convert(varchar(10),register_time,120)");
				Delyessql=_T("delete from hisforbjlb_patient where convert(varchar(10),getdate(),120)<>convert(varchar(10),register_time,120)");
				break;
			default:
				break;
			}


			if(m_connectToMyDataBase.m_pMyRecordset->Open(Searchyessql,CADORecordset::openQuery))
			{
				if(!m_connectToMyDataBase.m_pMyRecordset->IsBOF())
				{
					m_connectToMyDataBase.m_myDataBase.Execute(Delyessql);
				}
				m_connectToMyDataBase.m_pMyRecordset->Close();
			}
			CString mysql=_T("");
			if(m_connectToHis.m_pHisRecordset->Open(Hissql,CADORecordset::openQuery))
			{
				if(!m_connectToHis.m_pHisRecordset->IsBOF())
				{
					while(!m_connectToHis.m_pHisRecordset->IsEOF())
					{
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("姓名"),patient_name);
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("性别"),gender);
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("月份"),month);
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("输液号"),queue_id);
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("就诊号"),serial_id);
						m_connectToHis.m_pHisRecordset->GetFieldValue(_T("登记时间"),register_time);

						int length=queue_id.GetLength();
						CString temp=queue_id.Right(1);
						if(temp==_T("."))
							queue_id=queue_id.Left(length-1);
						mysql.Format(_T("select serial_id from hisforbjlb_patient where queue_id='%s'"),queue_id);
						if(m_connectToMyDataBase.m_pMyRecordset->Open(mysql,CADORecordset::openQuery))
						{
							if(m_connectToMyDataBase.m_pMyRecordset->IsBOF())
							{
								insertSql.Format(_T("insert into HisForBjlb_Patient(serial_id,queue_id,patient_name,gender,office_id,register_time,month) values('%s','%s','%s','%s','%s','%s','%d')"),
									serial_id,queue_id,patient_name,gender,office_id,register_time,month);
								m_connectToMyDataBase.m_myDataBase.Execute(insertSql);
							}
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

void CReadHisData::SetHisDataBaseString(int dataBaseType, CString hisIP,
								   CString hisPort,CString hisBaseName,
								   CString hisAccount,CString hisPass)
{
	m_connectToHis.SetConnectString(dataBaseType,hisIP,hisPort,
		hisBaseName,hisAccount,hisPass);
}

void CReadHisData::SetMyDataBaseString(int dataBaseType, CString hisIP,
								  CString hisPort,CString hisBaseName,
								  CString hisAccount,CString hisPass)
{
	m_connectToMyDataBase.SetConnectString(dataBaseType,hisIP,hisPort,
		hisBaseName,hisAccount,hisPass);
}

BOOL CReadHisData::OpenHisDataBase()
{
	CString hisDataDrive=m_baseConfig.GetHisDataDrive();
	CString hisBaseAccount=m_baseConfig.GetHisAcount();
	CString hisBaseIP=m_baseConfig.GetHisServerIP();
	CString hisPort=m_baseConfig.GetHisPort();
	CString hisBaseName=m_baseConfig.GetHisBaseName();
	CString hisPass=m_baseConfig.GetHisPass();

	
	if(hisDataDrive==_T("Oracle"))
	{
		m_typeDataDrive=typeOracle;
		SetHisDataBaseString(typeOracle,hisBaseIP,hisPort,hisBaseName,
			hisBaseAccount,hisPass);
	}
	else if(hisDataDrive==_T("SQL Server"))
	{
		m_typeDataDrive=typeSqlServer;
		SetHisDataBaseString(typeSqlServer,hisBaseIP,hisPort,hisBaseName,
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

BOOL CReadHisData::OpenMyDataBase()
{
	CString dataDrive=m_baseConfig.GetDataDrive();
	CString baseIP=m_baseConfig.GetServerIP();
	CString baseAccount=m_baseConfig.GetServerAcount();
	CString basePass=m_baseConfig.GetServerPassword();
	SetMyDataBaseString(typeSqlServer,baseIP,_T(""),
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

BOOL CReadHisData::IsHisDataBaseOpen()
{
	if(m_connectToHis.IsHisDataBaseOpen())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CReadHisData::IsMyDataBaseOpen()
{
	if(m_connectToMyDataBase.IsMyDataBaseOpen())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

DWORD WINAPI CReadHisData::ThreadReadData(LPVOID pParam)
{
	CReadHisData* pReadHis=(CReadHisData*)pParam;
	while(1)
	{
		
		pReadHis->FlushHisDataToMyTable(pReadHis->m_typeDataDrive);
		Sleep(1000);
	}
	return 0;
}

BOOL CReadHisData::StartReadDataThread()
{
	if(OpenHisDataBase() && OpenMyDataBase())
	{
		m_hReadHisData=CreateThread(NULL,0,ThreadReadData,this,NULL,0);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

