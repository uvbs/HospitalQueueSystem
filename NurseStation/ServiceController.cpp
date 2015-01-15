#include "StdAfx.h"
#include "ServiceController.h"
#include "MyCommon.h"
#include "MyString.h"


IMPLEMENT_DYNAMIC(CUser, CObject)
IMPLEMENT_DYNAMIC(CNurse, CUser)
IMPLEMENT_DYNAMIC(CDoctor, CUser)


CServiceController::CServiceController(void)
: m_pUpdateUserThread(NULL)
{
}

CServiceController::~CServiceController(void)
{
}

BOOL CServiceController::OnStartControl()
{
	BOOL bRet = CController::OnStartControl();

	//m_mytimerUpdateUserList.StartTimer(
	//	this, 3000, &CServiceController::OnUpdateUserList);
	if(!m_pUpdateUserThread)
		m_pUpdateUserThread = AfxBeginThread(UpdateUserThreadProc,this);

	return bRet;
}

BOOL CServiceController::OnEndControl()
{
	//BOOL bRet = CController::OnEndControl();

	//m_mytimerUpdateUserList.KillTimer();
	if(m_pUpdateUserThread)
	{
		::TerminateThread(m_pUpdateUserThread->m_hThread, 0);
	}

	POSITION pos = m_mapNurseList.GetStartPosition();
	CString strKey;
	CNurse* pNurse = NULL;
	for(; pos; )
	{
		CNurse* pTemp = pNurse;
		m_mapNurseList.GetNextAssoc(pos, strKey, pNurse);
		delete pTemp;
	}

	pos = m_mapDoctorList.GetStartPosition();
	CDoctor* pDoctor = NULL;
	for(; pos; )
	{
		CDoctor* pTemp = pDoctor;
		m_mapDoctorList.GetNextAssoc(pos, strKey, pDoctor);
		delete pTemp;
	}

	return CController::OnEndControl();
	//return bRet;
}

void CServiceController::OnRecvLogin(LPBROADCAST pLogin, LPSOCKADDR_IN psockddr)
{
	if(pLogin->header.sUserInfo.eUserType == utNurse)
	{
		AddNurse(pLogin, psockddr, TRUE);
	}
	else if(pLogin->header.sUserInfo.eUserType == utDoctor)
	{
		AddDoctor(pLogin, psockddr, TRUE);
	}
}

void CServiceController::OnRecvQuit(LPQUIT pQuit, LPSOCKADDR_IN psockddr)
{
	if(pQuit->header.sUserInfo.eUserType == utNurse)
	{
		//m_mapNurseList.RemoveKey(pQuit->header.sUserInfo.wszUserID);
		CNurse* pNurse = GetNurse(pQuit->header.sUserInfo.wszUserID);
		if(pNurse)
		{
			OnNurseOutLine(pNurse);
			m_mapNurseList.RemoveKey(pNurse->GetId());
			delete pNurse;
		}
	}
	else if(pQuit->header.sUserInfo.eUserType == utDoctor)
	{
		//m_mapDoctorList.RemoveKey(pQuit->header.sUserInfo.wszUserID);
		CDoctor* pDoctor = GetDoctor(pQuit->header.sUserInfo.wszUserID);
		if(pDoctor)
		{
			OnDoctorOutLine(pDoctor, pQuit->nExitCode);
			m_mapDoctorList.RemoveKey(pDoctor->GetId());
			delete pDoctor;
		}
	}
}

void CServiceController::OnRecvHeartBeat(LPBROADCAST pHeartbeat, LPSOCKADDR_IN psockddr)
{
	//BOOL bExist = FALSE;
	if(pHeartbeat->header.sUserInfo.eUserType == utNurse)
	{
		NurseMap::CPair *pPair;
		pPair = m_mapNurseList.PLookup(pHeartbeat->header.sUserInfo.wszUserID);
		if(pPair)
		{
			pPair->value->SetOnline(TRUE);
			if(pPair->value->GetOffline())
			{
				OnNurseLogin(pPair->value, TRUE);
				pPair->value->SetOffline(FALSE);
			}
			//bExist = TRUE;
		}
		else
		{
			AddNurse(pHeartbeat, psockddr, FALSE);
		}
	}
	else if(pHeartbeat->header.sUserInfo.eUserType == utDoctor)
	{
		DoctorMap::CPair *pPair;
		pPair = m_mapDoctorList.PLookup(pHeartbeat->header.sUserInfo.wszUserID);
		if(pPair)
		{
			pPair->value->SetOnline(TRUE);
			if(pPair->value->GetOffline())
			{
				OnDoctorLogin(pPair->value, TRUE);
				pPair->value->SetOffline(FALSE);
			}
			//bExist = TRUE;
		}
		else
		{
			AddDoctor(pHeartbeat, psockddr, FALSE);
		}
	}
	//if(!bExist)
	//{
	//	OnRecvLogin(pHeartbeat, psockddr);
	//}
}

//void CServiceController::OnUpdateUserList(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
void CServiceController::UpdateUserList()
{
	if(!GetOnline()) return;

	NurseMap::CPair* pNursePair;
	pNursePair = m_mapNurseList.PGetFirstAssoc();
	for(; pNursePair; pNursePair = m_mapNurseList.PGetNextAssoc(pNursePair))
	{
		if(pNursePair->value->GetOnline())
		{
			pNursePair->value->SetOnline(FALSE);
			pNursePair->value->SetOffline(FALSE);
			//pNursePair = m_mapNurseList.PGetNextAssoc(pNursePair);
		}
		else
		{
			if(pNursePair->value->GetId() == GetUserId() && utNurse == GetUserType())
			{
				SetOnline(FALSE);
				return;
			}
			if(!pNursePair->value->GetOffline())
			{
				OnNurseOutLine(pNursePair->value);
				pNursePair->value->SetOffline(TRUE);
			}

			//NurseMap::CPair* pTemp = pNursePair;
			//pNursePair = m_mapNurseList.PGetNextAssoc(pNursePair);
			//delete pTemp->value;
			//m_mapNurseList.RemoveKey(pTemp->key);
		}
	}
	DoctorMap::CPair* pDoctorPair;
	pDoctorPair = m_mapDoctorList.PGetFirstAssoc();
	for(; pDoctorPair; pDoctorPair = m_mapDoctorList.PGetNextAssoc(pDoctorPair))
	{
		if(pDoctorPair->value->GetOnline())
		{
			pDoctorPair->value->SetOnline(FALSE);
			pDoctorPair->value->SetOffline(FALSE);
		}
		else
		{
			if(pDoctorPair->value->GetId() == GetUserId() && utDoctor == GetUserType())
			{
				SetOnline(FALSE);
				return;
			}
			if(!pDoctorPair->value->GetOffline())
			{
				OnDoctorOutLine(pDoctorPair->value, EXITCODE_OFFLINE);
				pDoctorPair->value->SetOffline(TRUE);
			}
			
			//DoctorMap::CPair* pTemp = pDoctorPair;
			//pDoctorPair = m_mapDoctorList.PGetNextAssoc(pDoctorPair);
			//delete pTemp->value;
			//m_mapDoctorList.RemoveKey(pTemp->key);
		}
	}
}

void CServiceController::AddNurse(LPBROADCAST pLogin, LPSOCKADDR_IN psockddr, BOOL bLogin)
{
	//CNurse nurse;
	CNurse* pNurse = new CNurse;
	pNurse->SetId(pLogin->header.sUserInfo.wszUserID);
	pNurse->SetName(pLogin->header.sUserInfo.wszUserName);
	pNurse->SetSockAddrIn(psockddr);
	pNurse->SetOnline(TRUE);
	pNurse->SetOffline(FALSE);

	//try
	//{
	//	CADORecordset rset(&m_db);
	//	CString strSql;
	//	strSql.Format(_T("select N.office_id,O.name from Nurse_Office N,Office O where N.office_id=O.office_id and N.nurse_id='%s';"),
	//		pLogin->header.sUserInfo.wszUserID);
	//	if(m_db.IsOpen())
	//	{
	//		if(rset.Open(strSql, CADORecordset::openQuery))
	//		{
	//			if(!rset.IsBOF())
	//			{
	//				for(; !rset.IsEOF(); rset.MoveNext())
	//				{
	//					CString strOfficeId, strOfficeName;
	//					rset.GetFieldValue(0, strOfficeId);
	//					rset.GetFieldValue(1, strOfficeName);
	//					nurse.AddOffice(strOfficeId, strOfficeName);
	//				}
	//			}
	//		}
	//	}
	//}
	//catch (_com_error& e)
	//{

	//}

	CStringArray arrStrOffices;
	MyString::Split(pLogin->header.sUserInfo.wszOffices, arrStrOffices, _T("|"));
	if(!arrStrOffices.IsEmpty())
	{
		for(int i = 0; i < arrStrOffices.GetCount(); i++)
		{
			CStringArray arrStrOffice;
			MyString::Split(arrStrOffices[i], arrStrOffice, _T(","));
			if(!arrStrOffice.IsEmpty())
			{
				Office office;
				office.SetOfficeId(arrStrOffice[0]);
				if(arrStrOffice.GetCount() > 1)
				{
					office.SetOfficeName(arrStrOffice[1]);
				}
				pNurse->AddOffice(office);
			}
		}
	}

	m_mapNurseList[pLogin->header.sUserInfo.wszUserID] = pNurse;
	OnNurseLogin(pNurse, bLogin);
}

void CServiceController::AddDoctor(LPBROADCAST pLogin, LPSOCKADDR_IN psockddr, BOOL bLogin)
{
	//CDoctor doctor;
	CDoctor* pDoctor = new CDoctor;
	pDoctor->SetId(pLogin->header.sUserInfo.wszUserID);
	pDoctor->SetName(pLogin->header.sUserInfo.wszUserName);
	pDoctor->SetSockAddrIn(psockddr);
	pDoctor->SetOnline(TRUE);
	pDoctor->SetOffline(FALSE);

	//try
	//{
	//	CADORecordset rset(&m_db);
	//	CString strSql;
	//	strSql.Format(_T("select D.office_id,O.name from CDoctor D,Office O where D.office_id=O.office_id and D.doctor_id='%s';") 
	//		,pLogin->header.sUserInfo.wszUserID);
	//	if(m_db.IsOpen())
	//	{
	//		if(rset.Open(strSql, CADORecordset::openQuery))
	//		{
	//			if(!rset.IsBOF())
	//			{
	//				CString strOfficeId, strOfficeName;
	//				rset.GetFieldValue(0, strOfficeId);
	//				rset.GetFieldValue(1, strOfficeName);
	//				doctor.SetOffice(strOfficeId, strOfficeName);
	//			}
	//		}
	//	}
	//}
	//catch (_com_error& e)
	//{

	//}

	CStringArray arrStrOffices;
	MyString::Split(pLogin->header.sUserInfo.wszOffices, arrStrOffices, _T("|"));
	if(!arrStrOffices.IsEmpty())
	{
		CStringArray arrStrOffice;
		MyString::Split(arrStrOffices[0], arrStrOffice, _T(","));
		if(!arrStrOffice.IsEmpty())
		{
			Office office;
			office.SetOfficeId(arrStrOffice[0]);
			if(arrStrOffice.GetCount() > 1)
			{
				office.SetOfficeName(arrStrOffice[1]);
			}
			pDoctor->SetOffice(office);
		}
	}

	m_mapDoctorList[pLogin->header.sUserInfo.wszUserID] = pDoctor;
	OnDoctorLogin(pDoctor, bLogin);
}

CDoctor* CServiceController::GetDoctor(CString strDoctorId)
{
	return (CDoctor*)GetUser(strDoctorId, utDoctor);
}

CNurse* CServiceController::GetNurse(CString strNurseId)
{
	return (CNurse*)GetUser(strNurseId, utNurse);
}

CUser* CServiceController::GetUser(CString strUserId, UserTypeEnum eUserType)
{
	CUser* pUser = NULL;
	switch(eUserType)
	{
	case utNurse:
		{
			NurseMap::CPair* pPair = m_mapNurseList.PLookup(strUserId);
			if(pPair)
			{
				pUser = pPair->value;
			}
		}
		break;
	case utDoctor:
		{
			DoctorMap::CPair* pPair = m_mapDoctorList.PLookup(strUserId);
			if(pPair)
			{
				pUser = pPair->value;
			}
		}
		break;
	default:
		break;
	}

	return pUser;
}

UINT CServiceController::UpdateUserThreadProc(LPVOID pParam)
{
	CServiceController* pThis = (CServiceController*)pParam;

	while(1)
	{
		pThis->UpdateUserList();
		Sleep(3000);
	}
	::ExitThread(0);
	return 0;
}

void CServiceController::OnRecvData()
{
	SetOnline(TRUE);
	CController::OnRecvData();
}


/*
void CServiceController::AddDoctor(CString strId, CString strName, LPSOCKADDR_IN psockddr)
{
if(!m_db.IsOpen()) return;

CADORecordset rset(&m_db);
CString strSql;
try
{
CDoctor doctor;
doctor.SetId(strId);
doctor.SetName(strName);
strSql.Format(_T("select D.name DoctorName,D.sound_call,D.sound_wait,D.display_call,D.display_wait,D.max_recall_times,O.office_id ,O.call_name OfficeName,C.consult_id,C.call_name ConsultName,C.wnd_screen_id,D.evaluator_id,C.stb_id ConsultStbId from (Select * from CDoctor where doctor_id='%s') D left join Office O on O.office_id=D.office_id left join Consult C on C.Consult_ID=D.consult_id;") 
,strId);
if(rset.Open(strSql, CADORecordset::openQuery))
{
if(!rset.IsBOF())
{
CString strValue;
int iValue;
rset.GetFieldValue(_T("DoctorName"), strValue);
doctor.SetName(strValue);
rset.GetFieldValue(_T("office_id"), strValue);
doctor.SetOfficeId(strValue);
rset.GetFieldValue(_T("OfficeName"), strValue);
doctor.SetOfficeName(strValue);
rset.GetFieldValue(_T("consult_id"), strValue);
doctor.SetConsultId(strValue);
rset.GetFieldValue(_T("ConsultName"), strValue);
doctor.SetConsultName(strValue);
rset.GetFieldValue(_T("sound_call"), strValue);
doctor.SetSoundCall(strValue);
rset.GetFieldValue(_T("sound_wait"), strValue);
doctor.SetSoundWait(strValue);
rset.GetFieldValue(_T("display_call"), strValue);
doctor.SetDisplayCall(strValue);
rset.GetFieldValue(_T("display_wait"), strValue);
doctor.SetDisplayWait(strValue);
rset.GetFieldValue(_T("ConsultStbId"), strValue);
doctor.SetConsultStbId(strValue);
rset.GetFieldValue(_T("max_recall_times"), iValue);
doctor.SetMaxRecallTimes(iValue);
rset.GetFieldValue(_T("wnd_screen_id"), iValue);
doctor.SetWndScreenId(iValue)
rset.GetFieldValue(_T("evaluator_id"), iValue);
doctor.SetEvaluatorId(iValue)
}
}
m_mapDoctorList[strId] = doctor;
}
catch (_com_error& e)
{

}
}
*/
