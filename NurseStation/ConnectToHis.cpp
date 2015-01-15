#include "StdAfx.h"
#include "ConnectToHis.h"

CConnectToHis::CConnectToHis(void)
{
	m_pHisRecordset=NULL;
}

CConnectToHis::~CConnectToHis(void)
{
	if(m_pHisRecordset)
	{
		delete m_pHisRecordset;
	}
}

void CConnectToHis::SetConnectString(int dataBaseType, CString hisIP, 
									 CString hisPort,CString hisBaseName,
									 CString hisAccount, CString hisPass)
{
	switch(dataBaseType)
	{
	case 0:
		//m_connectString = _T("Driver={SQL Server};Server="+hisIP+";Database="+hisBaseName+";
		//Uid="+hisAccount+";Pwd="+hisPass+";");
		m_connectString = _T("Provider=MSDAORA.1;hos=");
		m_connectString += hisIP + _T(";port=");
		m_connectString += hisPort + _T(";Data Source=");//;User ID=");
		m_connectString += hisBaseName + _T(";User ID=");
		m_connectString += hisAccount + _T(";Password=");
		m_connectString += hisPass + _T(";PersistSecurityInfo=False;");
		break;
	case 1:
		m_connectString = _T("Driver={SQL Server};Server=");
		m_connectString += hisIP + _T(";Database=");
		m_connectString += hisBaseName + _T(";Uid=");
		m_connectString += hisAccount + _T(";Pwd=");
		m_connectString += hisPass + _T(";");
		break;
	default:
		break;
	}
}


BOOL CConnectToHis::OpenHisDataBase()
{
	if(!m_connectString.IsEmpty())
	{
		try
		{
			m_hisDataBase.SetConnectionString(m_connectString);
			if(m_hisDataBase.Open())
			{
				m_pHisRecordset=new CADORecordset(&m_hisDataBase);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		catch (_com_error& e)
		{
			
		}
		catch (...)
		{
		}
	}

	return FALSE;
}

BOOL CConnectToHis::IsHisDataBaseOpen()
{
	if(m_hisDataBase.IsOpen())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}