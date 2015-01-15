#include "StdAfx.h"
#include "ConnectToMyDataBase.h"

CConnectToMyDataBase::CConnectToMyDataBase(void)
{
	m_connectString=_T("");
	m_pMyRecordset=NULL;
}

CConnectToMyDataBase::~CConnectToMyDataBase(void)
{
	if(m_pMyRecordset)
	{
		delete m_pMyRecordset;
	}
}

VOID CConnectToMyDataBase::SetConnectString(int dataBaseType, CString hisIP, 
											CString hisPort, CString hisBaseName, 
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

BOOL CConnectToMyDataBase::OpenMyDataBase()
{
	if(!m_connectString.IsEmpty())
	{
		try
		{
			m_myDataBase.SetConnectionString(m_connectString);
			if(m_myDataBase.Open())
			{
				m_pMyRecordset=new CADORecordset(&m_myDataBase);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		catch (_com_error& e)
		{
			return FALSE;
		}
		catch (...)
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

BOOL CConnectToMyDataBase::IsMyDataBaseOpen()
{
	if(m_myDataBase.IsOpen())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

