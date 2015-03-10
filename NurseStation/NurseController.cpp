#include "StdAfx.h"
#include "NurseController.h"
#include "OpenDatabase.h"
#include "MyString.h"
#include "MyCommon.h"
#include "NurseStation.h"
#include "WriteLog.h"
#include "SendToHardWare.h"


//#ifdef _DEBUG
//extern void MyWriteConsole(CString str);
//#endif

CNurseController::CNurseController(void)
: m_strWebRootDir(_T(""))
, m_iHtmlPageItemCount(8)
, m_iHtmlPageSwitchTime(8)
, m_iHtmlShowCallTime(8)
, m_nCurIndex(0)
, m_pJttsAPI(NULL)
, m_strHtmlTitleBegin(_T(""))
, m_strHtmlTitleEnd(_T(""))
, m_strHtmlNoticeBegin(_T(""))
, m_strHtmlNoticeEnd(_T(""))
, m_strHtmlInstMsgBegin(_T(""))
, m_strHtmlInstMsgEnd(_T(""))
, m_strHtmlQueueInfoBegin(_T(""))
, m_strHtmlQueueInfoEnd(_T(""))
//, m_strHtmlTitle(_T(""))
, m_pDisplayThread(NULL)
, m_strQueHtml(_T(""))
, m_pUpdateQueListThread(NULL)
, m_strModuleDir(_T(""))
{
	m_strModuleDir = MyCommon::GetModuleDir();

	m_strWebRootDir = m_strModuleDir + _T("webRoot\\");

	CBaseConfig config;
	m_DbParam.SetDriver(config.GetDataDrive());
	m_DbParam.SetHost(config.GetServerIP());
	m_DbParam.SetUID(config.GetServerAcount());
	m_DbParam.SetPID(config.GetServerPassword());
	m_DbParam.SetDbName(_T("Nurse"));
	SetDataDays(MyCommon::GetProfileInt(
		_T("sys"), _T("DATA_DAYS"), 0, _T("\\SYS\\sysconfig.ini")));
	SetCallSeconds(MyCommon::GetProfileInt(
		_T("sys"), _T("CALL_SECONDS"), 0, _T("\\SYS\\sysconfig.ini")));

	m_strSqlConn = _T("Driver={")+m_DbParam.GetDriver()+_T("};Server=")
		+m_DbParam.GetHost()+_T(";Database=Nurse;Uid=")
		+m_DbParam.GetUID()+_T(";Pwd=")+m_DbParam.GetPID()+_T(";");
}

CNurseController::~CNurseController(void)
{
	if(m_pDisplayThread)
	{
		delete m_pDisplayThread;
		m_pDisplayThread = NULL;
	}
}

BOOL CNurseController::OnStartControl()
{
	InitDisplay();

	BeginDisplayThread();

	InitNetServer();

	if(!SqlGetNurseFromDB())
	{
		//return FALSE;
	}
	SetUserType(utNurse);

	m_pJttsAPI = new CJttsAPI;
	if(!m_pJttsAPI->InitTTS())
	{
		AfxMessageBox(_T("声音服务启动失败!"));
	}

	DeleteDbData();

	return CQueueController::OnStartControl();
}

BOOL CNurseController::OnEndControl()
{
	CloseSocket();
	KillDisplayThread();
	TerminateProcess(m_piNetServer.hProcess, 0);

	if(m_pJttsAPI)
	{
		m_pJttsAPI->EndTTS();
		delete m_pJttsAPI;
		m_pJttsAPI = NULL;
	}

	return CQueueController::OnEndControl();
}

void CNurseController::OnRecvDoctorCmd(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockaddr)
{
	//检验该医生是否在护士站可处理科室里
	//int nCount = ((CNurseStationApp*)AfxGetApp())->m_canDoOfficeIDList.GetCount();
	//CString strOfficeId(pDoctorCmd->header.sUserInfo.wszOffices);
	//BOOL bHave = FALSE;
	//for(int i = 0; i < nCount; i++)
	//{
	//	if(((CNurseStationApp*)AfxGetApp())->m_canDoOfficeIDList[i].Compare(strOfficeId) == 0)
	//	{
	//		bHave = TRUE;
	//		break;
	//	}
	//}
	//if(!bHave)
	//{
	//	return;
	//}
	////////////////////////////////////////////////////////////////

	CString strDoctorID(pDoctorCmd->header.sUserInfo.wszUserID);
	if(strDoctorID.IsEmpty())
	{
		return;
	}

	//CDoctor* pDoctor = GetDoctor(strDoctorID);
	//if(!pDoctor)
	//{
	//	BROADCAST user;
	//	memset(&user, 0, sizeof(user));
	//	user.header = pDoctorCmd->header;
	//	user.header.eTypeID = ntiLogin;
	//	user.header.nDataLength = sizeof(user);
	//	DOCTORBASICINFO docInfo;
	//	SqlQueryDoctorBaseInfo(&docInfo, strDoctorID);
	//	wcscpy_s(user.header.sUserInfo.wszUserName, docInfo.strDoctorName);
	//	wcscpy_s(user.header.sUserInfo.wszOffices, docInfo.strOfficeID);
	//	SendTo( (LPSTR)&user, sizeof( user ), _T("127.0.0.1"));
	//	wcscpy_s(pDoctorCmd->header.sUserInfo.wszUserName, docInfo.strDoctorName);
	//	wcscpy_s(pDoctorCmd->header.sUserInfo.wszOffices, docInfo.strOfficeID);
	//	SendTo( (LPSTR)pDoctorCmd, sizeof( *pDoctorCmd ), _T("127.0.0.1"));
	//	return;
	//}

	//CDoctor* pDoctor = GetDoctor(strDoctorID);
	//if(!pDoctor)
	//{
	//	return;
	//}
	LPDOCTORBASICINFO pDocInfo = GetDocBasInfo(strDoctorID);
	if(!pDocInfo)
	{
		return;
	}
	
	//
	switch(pDoctorCmd->eDoctorCmd)
	{
	case dcCmdCall:
		OnCall(pDoctorCmd, psockaddr);
		break;
	case dcCmdRecall:
		OnRecall(pDoctorCmd, psockaddr);
		break;
	case dcCmdDiscard:
		OnDiscard(pDoctorCmd, psockaddr);
		break;
	case dcCmdWait:
		OnWait(pDoctorCmd, psockaddr);
		break;
	case dcCmdEva:
		OnEva(pDoctorCmd, psockaddr);
		break;
	case dcCmdPause:
		OnPause(pDoctorCmd, psockaddr);
		break;
	case dcCmdResume:
		OnResume(pDoctorCmd, psockaddr);
		break;
	}
}

void CNurseController::OnRecvReqLogin(LPREQLOGIN pReqLogin, LPSOCKADDR_IN psockddr)
{
	CMDRESULT cmdRes;
	ZeroMemory(&cmdRes, sizeof(cmdRes));
	SetSendBaseInfo((LPBASEINFO)&cmdRes, ntiCmdResult, sizeof(cmdRes));
	cmdRes.eTypeIDFrom = pReqLogin->header.eTypeID;
	cmdRes.sUserInfoFrom = pReqLogin->header.sUserInfo;
	DBPARAM dbParam;
	cmdRes.sDbParam = *(m_DbParam.ToDBPARAM(&dbParam));

	if(pReqLogin->header.sUserInfo.eUserType == utNurse)
	{
		if(GetUserId().Compare(pReqLogin->header.sUserInfo.wszUserID) == 0)
		{
			cmdRes.eRes = vrLogined;
			SendTo((char*)&cmdRes, sizeof(cmdRes), psockddr);
		}
	}
	else if(pReqLogin->header.sUserInfo.eUserType == utDoctor)
	{
		VerifyResultEnum eRes = VerifyDoctor(pReqLogin->header.sUserInfo.wszUserID, 
			pReqLogin->wszPassword, &cmdRes);
		if(eRes != vrNotFoundOffice)
		{
			cmdRes.eRes = eRes;
			SendTo((char*)&cmdRes, sizeof(cmdRes), psockddr);
		}
	}
}

VerifyResultEnum CNurseController::VerifyDoctor(CString strLoginID, 
												CString strPassword, 
												LPCMDRESULT pCmdResult)
{
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset set(&db);
		CString strSql;
		//strSql.Format(_T("select D.office_id OfficeId,D.name DoctorName,D.call_special_flag CallSpecial,D.expert_flag ExpertFlag,O.name OfficeName from Doctor D,Office O where D.doctor_id='%s' and D.password='%s' and D.office_id=O.office_id;")
		//	, strDoctorID, strPassword);
		strSql.Format(_T("select D.doctor_id,D.name DoctorName,D.call_special_flag CallSpecial,D.expert_flag ExpertFlag,D.office_id OfficeId,O.name OfficeName from Doctor D LEFT JOIN Office O ON D.office_id=O.office_id where D.login_id='%s' and D.password='%s';")
			, strLoginID, strPassword);
		try
		{
			set.Open(strSql);
		}catch(_com_error&)
		{
			WriteLog::WriteDbErrLog(_T("CNurseController::VerifyDoctor"));
			return vrDBError; 
		}

		if(set.IsBOF())
		{
			return vrNotPass;
		}
		CString strOfficeID;
		set.GetFieldValue(_T("OfficeId"), strOfficeID);

		BOOL bExist = FALSE;
		for(int i = 0; i < m_arrstrOfficeId.GetCount(); i++)
		{
			if(strOfficeID.Compare(m_arrstrOfficeId[i]) == 0)
			{
				bExist = TRUE;
				break;
			}
		}
		if(!bExist)
		{
			return vrNotFoundOffice;
		}

		CString strDoctorID;
		set.GetFieldValue(_T("doctor_id"), strDoctorID);

		CUser* pDoctor = GetUser(strDoctorID, utDoctor);
		if(pDoctor)
		{
			if(pDoctor->GetOnline())
			{
				return vrLogined;
			}
		}

		CString strUserName;
		set.GetFieldValue(_T("DoctorName"), strUserName);

		BOOL bCallSpecial;
		set.GetFieldValue(_T("CallSpecial"), bCallSpecial);

		CString strOfficeName;
		set.GetFieldValue(_T("OfficeName"), strOfficeName);

		BOOL bExpertFlag;
		set.GetFieldValue(_T("ExpertFlag"), bExpertFlag);

		set.Close();
		db.Close();
		CString strOffices = strOfficeID + _T(",") + strOfficeName;

		wcscpy_s(pCmdResult->sUserInfoFrom.wszUserID, MAX_ID_LENGTH, strDoctorID.GetBuffer());
		wcscpy_s(pCmdResult->sUserInfoFrom.wszOffices, MAX_NAME_LENGTH, strOffices.GetBuffer());
		wcscpy_s(pCmdResult->sUserInfoFrom.wszUserName, MAX_NAME_LENGTH, strUserName.GetBuffer());
		pCmdResult->bCallSpecial = bCallSpecial;
		pCmdResult->bExpertFlag = bExpertFlag;
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::VerifyDoctor"));
	}
	catch (...)
	{
	}

	return vrPass;
}

BOOL CNurseController::SendCmd(CString strDoctorID, CString strSerialID, DoctorCmdEnum eDoctorCmdEnum)
{
	//CDoctor* pDoctor = GetDoctor(strDoctorID);
	LPDOCTORBASICINFO pDocInfo = GetDocBasInfo(strDoctorID);
	if(!pDocInfo)
	{
		DOCTORBASICINFO dinfo;
		SqlQueryDoctorBaseInfo(dinfo, strDoctorID);
		m_mapDocIdDocInfo[strDoctorID] = dinfo;
		pDocInfo = GetDocBasInfo(strDoctorID);
	}
	if(!pDocInfo) return FALSE;
	DOCTORCMD docCmd;
	docCmd.header.eTypeID = ntiDoctorCmd;

	docCmd.header.nDataLength = sizeof(docCmd);
	docCmd.eDoctorCmd = eDoctorCmdEnum;
	wcscpy_s(docCmd.header.sUserInfo.wszUserID, MAX_ID_LENGTH, strDoctorID.GetBuffer());
	wcscpy_s(docCmd.szQueSerialID, MAX_ID_LENGTH, strSerialID.GetBuffer());
	wcscpy_s(docCmd.header.sUserInfo.wszUserName, pDocInfo->strDoctorName);
	wcscpy_s(docCmd.header.sUserInfo.wszOffices, pDocInfo->strOfficeID);
	docCmd.header.sUserInfo.eUserType = utDoctor;

	return SendTo( (LPSTR)&docCmd, 
		sizeof( docCmd ), _T("127.0.0.1"));
}

void CNurseController::OnDoctorLogin(CDoctor* pDoctor, BOOL bLogin)
{
	CQueueController::OnDoctorLogin(pDoctor, TRUE);

	LOGIN login;
	ZeroMemory(&login, sizeof(login));
	login.header.eTypeID = ntiLogin;
	login.header.nDataLength = sizeof(login);
	login.header.sSockAddrIn = pDoctor->GetSockAddrIn();
	login.header.sUserInfo.eUserType = utDoctor;
	CString strUserId = pDoctor->GetId();
	wcscpy_s(login.header.sUserInfo.wszUserID, 
		MAX_ID_LENGTH, strUserId.GetBuffer());

	SendCmdResult((LPBASEINFO)&login, &login.header.sSockAddrIn, TRUE);
	DOCTORBASICINFO dinfo;
	SqlQueryDoctorBaseInfo(dinfo, strUserId);

	m_mapDocIdDocInfo[pDoctor->GetId()] = dinfo;

	VIEWUPDATEINFO sViewInfo;
	sViewInfo.strDoctorID = strUserId;
	sViewInfo.strDoctorName = dinfo.strDoctorName;
	sViewInfo.strOfficeID = dinfo.strOfficeID;
	sViewInfo.strPatientName = _T("");
	sViewInfo.strSerialID = _T("");
	UpdateDocView(&sViewInfo, dsWorking);
}

void CNurseController::OnDoctorOutLine(CDoctor* pDoctor, int nExitCode)
{
	CString strUserId = pDoctor->GetId();

	switch(nExitCode)
	{
	case EXITCODE_OFFLINE:
		{

		}
		break;
	case EXITCODE_UPDATE_NONE:
		{

		}
		break;
	case EXITCODE_UPDATE_CALL:
		{
			SqlUpdateCallToDone(strUserId);
		}
		break;
	case EXITCODE_UPDATE_WAIT:
		{
			SqlUpdateWaitToInLine(strUserId);
		}
		break;
	case EXITCODE_UPDATE_ALL:
		{
			SqlUpdateCallToDone(strUserId);
			SqlUpdateWaitToInLine(strUserId);
		}
		break;
	}

	if(nExitCode != EXITCODE_OFFLINE)
	{
		QUIT quit;
		ZeroMemory(&quit, sizeof(quit));
		quit.header.eTypeID = ntiQuit;
		quit.header.nDataLength = sizeof(quit);
		quit.header.sSockAddrIn = pDoctor->GetSockAddrIn();
		quit.header.sUserInfo.eUserType = utDoctor;
		wcscpy_s(quit.header.sUserInfo.wszUserID, 
			MAX_ID_LENGTH, strUserId.GetBuffer());

		SendCmdResult((LPBASEINFO)&quit, &quit.header.sSockAddrIn, TRUE);

		VIEWUPDATEINFO viewInfo;
		viewInfo.strDoctorID = m_mapDocIdDocInfo[strUserId].strDoctorID;
		viewInfo.strDoctorName = m_mapDocIdDocInfo[strUserId].strDoctorName;
		viewInfo.strOfficeID = m_mapDocIdDocInfo[strUserId].strOfficeID;

		UpdateDocView(&viewInfo, usOutLine);
	}

	CQueueController::OnDoctorOutLine(pDoctor, nExitCode);
}

void CNurseController::OnCall(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockddr)
{
	////将呼叫列表中该医生数据移除，并更新数据库，将患者状态改为"完成就诊"/////////
	///// modefiy by luo 11.11改cpaitview中数据
	CString strDoctorID(pDoctorCmd->header.sUserInfo.wszUserID);
	DOCTORBASICINFO docInfo;
	SqlQueryDoctorBaseInfo(docInfo, strDoctorID);

	BOOL bSuc = TRUE;

	CString strQueSerialID(pDoctorCmd->szQueSerialID);

	VIEWUPDATEINFO sViewInfo;
	SqlQueryCallingPatient(strDoctorID, &sViewInfo);
	if(!sViewInfo.strSerialID.IsEmpty())
	{
		((CNurseStationApp*)AfxGetApp())->m_pNurseWnd->GetPatientView()->
			ReviseListCtrPatStatus(sViewInfo.strSerialID, qsDone, 
				sViewInfo.strDoctorName,sViewInfo.strConsultName);
	}

	SqlUpdateCallToDone(strDoctorID);
	////////////////////////////////////////////////////////////////////////////////////
	if(strQueSerialID.IsEmpty())
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		CString strSql;
		//strSql.Format(_T("select top 1 log_id,queue_id_call,patient_name,patient_gender,patient_month patient_birth,status,doctor_id from Queue where doctor_id='%s' and status=%d and %s;")
		//	,strDoctorID , qsPrepare, m_strSqlDate);
		strSql.Format(_T("select log_id,status,doctor_id from Queue where doctor_id='%s' and status=%d and %s order by regtime;")
			,strDoctorID , qsPrepare, m_strSqlDate);
		try
		{
			rset.Open(strSql);
			if(!rset.IsBOF())
			{
				rset.Edit();
				rset.SetFieldValue(_T("status"), qsDoing);
				rset.Update();
				rset.GetFieldValue(_T("log_id"), strQueSerialID); 
				rset.Close();
			}
			else
			{
				rset.Close();
				//strSql.Format(_T("Select top 1 Q.log_id,Q.status,Q.doctor_id from Queue Q, (select doctor_id, office_id from Doctor where doctor_id='%s') D where Q.office_id=D.office_id and %s and status=%d and %s order by Q.priority desc, Q.regtime;")
				//	,strDoctorID ,docInfo.bExpertFlag ? _T("Q.doctor_id=D.doctor_id") : _T("(Q.doctor_id is null or Q.doctor_id='' or Q.doctor_id=D.doctor_id)"), qsInLine, m_strSqlDate);
				//rset.Open(strSql);
				//CString str = GetFirstInLinePatientSelectString(strDoctorID, docInfo.bExpertFlag);
				//AfxMessageBox(str);
				rset.Open(GetFirstInLinePatientSelectString(strDoctorID, docInfo.bExpertFlag));
				//if(!rset.IsBOF())
				//{
				//	rset.Edit();
				//	rset.SetFieldValue(_T("status"), qsDoing);
				//	rset.SetFieldValue(_T("doctor_id"), strDoctorID);
				//	rset.Update();
				//	rset.GetFieldValue(_T("log_id"), strQueSerialID);  
				//}
				CADORecordset rset2(&db);
				CString strSelExist;
				for(; !rset.IsBOF(); rset.MoveNext())
				{
					CString strPatientId;
					rset.GetFieldValue(_T("patient_id"), strPatientId);
					strSelExist.Format(_T("select log_id from Queue where patient_id='%s' and (status=%d or status=%d) and %s;"),
						strPatientId, qsDoing, qsPrepare, m_strSqlDate);
					rset2.Open(strSelExist, CADORecordset::openQuery);
					if(!rset2.IsBOF())
					{
						rset2.Close();
						continue;
					}
					rset.Edit();
					rset.SetFieldValue(_T("status"), qsDoing);
					rset.SetFieldValue(_T("doctor_id"), strDoctorID);
					rset.Update();
					rset.GetFieldValue(_T("log_id"), strQueSerialID);

					rset2.Close();
					break;
				}
			}
			rset.Close();
			db.Close();
		}
		catch (_com_error& e)
		{
			bSuc = FALSE;
			WriteLog::WriteDbErrLog(_T("CNurseController::OnCall"));
		}
	}
	else
	{
		bSuc = SqlUpdateToCall(strDoctorID, strQueSerialID);
	}
	//
	//if(!bOurCaller)
	//{
	//	ReturnDataToOtherCaller(wszDoctorID, dcCmdCall, )
	//}
	//else
	//{
	//播放声音及诊室窗口屏显示

	if(bSuc)
	{
		if(!strQueSerialID.IsEmpty())
		{
			AddToSoundAndDisplay(docInfo, strQueSerialID, TRUE);
		}
		else
		{
			bSuc = FALSE;
		}

		SqlQueryCallingPatient(strDoctorID, &sViewInfo);

		//刷新CPaintView中的患者状态
		((CNurseStationApp*)AfxGetApp())->m_pNurseWnd->GetPatientView()->
			ReviseListCtrPatStatus(sViewInfo.strSerialID, qsDoing, 
			sViewInfo.strDoctorName,sViewInfo.strConsultName);
		//刷新CDocInfoView中医生列表相应医生的患者姓名
		UpdateDocView(&sViewInfo, dsWorking);

		if(docInfo.bWaitWhenCall)
		{
			CallWait(docInfo);
		}

		CONSULTSTB consultStb;
		consultStb.strDepartName = docInfo.strOfficeName;
		consultStb.strConsultName = docInfo.strConsultName;
		consultStb.strDoctorId = docInfo.strDoctorID;
		consultStb.strDoctorName = docInfo.strDoctorName;
		consultStb.strDoctorTitle = docInfo.strTitle;
		consultStb.strStbId = docInfo.strConsultStbId;
		//
		consultStb.strCallPatient = SqlQueryCallPatient(strDoctorID);
		consultStb.strWaitPatient = SqlQueryWaitPatient(strDoctorID);
		//
		UpdateConsultStb(consultStb);

		//}

		//////////////////////////////////////////////////////////////

		//更新护士站液晶屏、综合屏
		//UpdateDisplayList();

		//CDoctor* pDoctor = GetDoctor(strDoctorID);
		//LPDOCTORBASICINFO pDocInfo = GetDocBasInfo(strDoctorID);

		UpdateQueListData(docInfo.strOfficeID);
		//UpdateQueListDataRowType();
	}

	SendCmdResult((LPBASEINFO)pDoctorCmd, psockddr, bSuc);
//	((CNurseStationApp*)AfxGetApp())->m_pNurseWnd->ReturnToHCaller(strDoctorID);
}

/*在OnCall()中调用*/
BOOL CNurseController::CallWait(DOCTORBASICINFO& docInfo)
{
	BOOL bSuc = TRUE;

	CString strSerialId;
	CString strSql;
	strSql.Format(_T("Select log_id,status,doctor_id from Queue where doctor_id='%s' and status=%d and %s;")
		,docInfo.strDoctorID , qsPrepare, m_strSqlDate);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSql);
		if(rset.IsBOF())
		{
			rset.Close();

			//CString strSql;
			//strSql.Format(_T("Select top 1 Q.log_id,Q.status,Q.doctor_id from Queue Q, (select doctor_id, office_id from Doctor where doctor_id='%s') D where Q.office_id=D.office_id and %s and status=%d and %s order by Q.priority desc, Q.regtime;")
			//	,docInfo.strDoctorID ,docInfo.bExpertFlag ? _T("Q.doctor_id=D.doctor_id") : _T("(Q.doctor_id is null or Q.doctor_id='' or Q.doctor_id=D.doctor_id)"), qsInLine, m_strSqlDate);
			//rset.Open(strSql);
			rset.Open(GetFirstInLinePatientSelectString(docInfo.strDoctorID, docInfo.bExpertFlag));
			//if(!rset.IsBOF())
			//{
			//	rset.Edit();
			//	rset.SetFieldValue(_T("status"), qsPrepare);
			//	rset.SetFieldValue(_T("doctor_id"), docInfo.strDoctorID);
			//	rset.Update();
			//	rset.GetFieldValue(_T("log_id"), strSerialId);
			//}
			CADORecordset rset2(&db);
			CString strSelExist;
			for(; !rset.IsBOF(); rset.MoveNext())
			{
				CString strPatientId;
				rset.GetFieldValue(_T("patient_id"), strPatientId);
				strSelExist.Format(_T("select log_id from Queue where patient_id='%s' and (status=%d or status=%d) and %s;"),
					strPatientId, qsDoing, qsPrepare, m_strSqlDate);
				rset2.Open(strSelExist, CADORecordset::openQuery);
				if(!rset2.IsBOF())
				{
					continue;
				}
				rset.Edit();
				rset.SetFieldValue(_T("status"), qsPrepare);
				rset.SetFieldValue(_T("doctor_id"), docInfo.strDoctorID);
				rset.Update();
				rset.GetFieldValue(_T("log_id"), strSerialId);
				break;
			}

			rset.Close();

			if(!strSerialId.IsEmpty())
			{
				AddToSoundAndDisplay(docInfo, strSerialId, FALSE);

				//刷新CPaintView中的患者状态
				((CNurseStationApp*)AfxGetApp())->m_pNurseWnd->GetPatientView()->
					ReviseListCtrPatStatus(strSerialId, qsPrepare, 
					m_mapDocIdDocInfo[docInfo.strDoctorID].strDoctorName,
					m_mapDocIdDocInfo[docInfo.strDoctorID].strConsultName);
			}
			else
			{
				bSuc = FALSE;
			}
			//更新护士站液晶屏、综合屏
			UpdateQueListData(docInfo.strOfficeID);

		}
		else
		{
			//rset.GetFieldValue(_T("log_id"), strSerialId);
			//rset.Close();
		}
		db.Close();
	}
	catch (_com_error& e)
	{
		bSuc = FALSE;
		WriteLog::WriteDbErrLog(_T("CNurseController::OnWait"));
	}

	return bSuc;
}

void CNurseController::OnRecall(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockddr)
{
	CString strDoctorID(pDoctorCmd->header.sUserInfo.wszUserID);
	DOCTORBASICINFO docInfo;
	SqlQueryDoctorBaseInfo(docInfo, strDoctorID);

	BOOL bSuc = TRUE;

	CString strSqlSel;
	//strSqlSel.Format(_T("Select log_id,queue_id_call,patient_name,status,doctor_id from Queue where doctor_id='%s' and status=%d;"),wszDoctorID , qsDoing);
	//strSqlSel.Format(_T("Select log_id,his_queue_id queue_id_call,patient_name,status,doctor_id from Queue where doctor_id='%s' and status=%d;"),wszDoctorID , qsDoing);
	strSqlSel.Format(_T("Select  top 1 log_id from Queue where doctor_id='%s' and status=%d and %s;")
		,strDoctorID , qsDoing, m_strSqlDate);
	CString strSerialID;
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSqlSel);
		if(!rset.IsBOF())
		{
			rset.GetFieldValue(0, strSerialID);
		}
		rset.Close();
		db.Close();
	}
	catch (_com_error& e)
	{
		bSuc = FALSE;
		WriteLog::WriteDbErrLog(_T("CNurseController::OnRecall"));
	}

	if(!strSerialID.IsEmpty())
	{
		AddToSoundAndDisplay(docInfo, strSerialID, TRUE);
	}
	else
	{
		bSuc = FALSE;
	}

	SendCmdResult((LPBASEINFO)pDoctorCmd, psockddr, bSuc);
}

void CNurseController::OnDiscard(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockddr)
{
	CString strDoctorID(pDoctorCmd->header.sUserInfo.wszUserID);

	DOCTORBASICINFO docInfo;
	SqlQueryDoctorBaseInfo(docInfo, strDoctorID);

	BOOL bSuc = TRUE;

	CString strSql;
	strSql.Format(_T("Select log_id from Queue where doctor_id='%s' and status=%d and %s;")
		,strDoctorID , qsDoing, m_strSqlDate);
	CString strSerialID;
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSql, CADORecordset::openQuery);
		if(!rset.IsBOF())
		{
			rset.GetFieldValue(_T("log_id"), strSerialID);
			//rset.SetFieldValue(_T("status"), qsDiscard);
			//rset.Update();
			SqlUpdateCallToDiscard(strDoctorID);
		}
		rset.Close();
		db.Close();
	}
	catch (_com_error& e)
	{
		bSuc = FALSE;
		WriteLog::WriteDbErrLog(_T("CNurseController::OnDiscard"));
	}

	//SqlUpdateCallToDiscard(strDoctorID);
	
	if(!strSerialID.IsEmpty())
	{
		//CALLPATIENTINFO callinfo;
		//SqlQueryPatientBySerialID(strSerialID, &callinfo);

		//刷新CPaintView中的患者状态
		((CNurseStationApp*)AfxGetApp())->m_pNurseWnd->GetPatientView()->
			ReviseListCtrPatStatus(strSerialID, qsDiscard, 
			m_mapDocIdDocInfo[strDoctorID].strDoctorName,
			m_mapDocIdDocInfo[strDoctorID].strConsultName);

		//SqlQueryDoctorBaseInfo(&(callinfo.sDoctorBasicInfo),strDoctorID);

		VIEWUPDATEINFO viewInfo;
		viewInfo.strDoctorID = strDoctorID;
		viewInfo.strOfficeID = m_mapDocIdDocInfo[strDoctorID].strOfficeID;
		viewInfo.strPatientName.Empty();
		viewInfo.strSerialID = strSerialID;
		viewInfo.strDoctorName = m_mapDocIdDocInfo[strDoctorID].strDoctorName;
		UpdateDocView(&viewInfo, dsWorking);
	}
	else
	{
		bSuc = FALSE;
	}
	SendCmdResult((LPBASEINFO)pDoctorCmd, psockddr, bSuc);
	//CDoctor* pDoctor = GetDoctor(strDoctorID);
	LPDOCTORBASICINFO pDocInfo = GetDocBasInfo(strDoctorID);
	if(pDocInfo)
	{
		UpdateQueListData(pDocInfo->strOfficeID);
		//UpdateQueListDataRowType();
	}

	CONSULTSTB consultStb;
	consultStb.strDepartName = docInfo.strOfficeName;
	consultStb.strConsultName = docInfo.strConsultName;
	consultStb.strDoctorId = docInfo.strDoctorID;
	consultStb.strDoctorName = docInfo.strDoctorName;
	consultStb.strDoctorTitle = docInfo.strTitle;
	consultStb.strStbId = docInfo.strConsultStbId;
	//
	consultStb.strCallPatient = _T("");
	consultStb.strWaitPatient = SqlQueryWaitPatient(strDoctorID);
	//
	UpdateConsultStb(consultStb);
}

void CNurseController::OnWait(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockddr)
{
	CString strDoctorID(pDoctorCmd->header.sUserInfo.wszUserID);
	DOCTORBASICINFO docInfo;
	SqlQueryDoctorBaseInfo(docInfo, strDoctorID);
	if(docInfo.iWaitNum < 1) docInfo.iWaitNum = 1;

	BOOL bSuc = TRUE;

	CString strSql;
	strSql.Format(_T("Select log_id,status,doctor_id from Queue where doctor_id='%s' and status=%d and %s;")
		,strDoctorID , qsPrepare, m_strSqlDate);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSql);
		if(rset.GetRecordCount() < docInfo.iWaitNum)
		{
			rset.Close();

			CString strSerialID(pDoctorCmd->szQueSerialID);
			if(strSerialID.IsEmpty())
			{
				//CString strSql;
				//strSql.Format(_T("Select top 1 Q.log_id,Q.status,Q.doctor_id from Queue Q, (select doctor_id, office_id from Doctor where doctor_id='%s') D where Q.office_id=D.office_id and %s and status=%d and %s order by Q.priority desc, Q.regtime;")
				//	,strDoctorID ,docInfo.bExpertFlag ? _T("Q.doctor_id=D.doctor_id") : _T("(Q.doctor_id is null or Q.doctor_id='' or Q.doctor_id=D.doctor_id)"), qsInLine, m_strSqlDate);
				rset.Open(GetFirstInLinePatientSelectString(strDoctorID, docInfo.bExpertFlag));
				//if(!rset.IsBOF())
				//{
				//	rset.Edit();
				//	rset.SetFieldValue(_T("status"), qsPrepare);
				//	rset.SetFieldValue(_T("doctor_id"), strDoctorID);
				//	rset.Update();
				//	rset.GetFieldValue(_T("log_id"), strSerialID);
				//}
				CADORecordset rset2(&db);
				CString strSelExist;
				for(; !rset.IsBOF(); rset.MoveNext())
				{
					CString strPatientId;
					rset.GetFieldValue(_T("patient_id"), strPatientId);
					strSelExist.Format(_T("select log_id from Queue where patient_id='%s' and (status=%d or status=%d) and %s;"),
						strPatientId, qsDoing, qsPrepare, m_strSqlDate);
					rset2.Open(strSelExist, CADORecordset::openQuery);
					if(!rset2.IsBOF())
					{
						continue;
					}
					rset.Edit();
					rset.SetFieldValue(_T("status"), qsPrepare);
					rset.SetFieldValue(_T("doctor_id"), strDoctorID);
					rset.Update();
					rset.GetFieldValue(_T("log_id"), strSerialID);
					break;
				}

				rset.Close();
				db.Close();
			}
			else
			{
				bSuc = SqlUpdateToWait(strDoctorID, strSerialID);
			}

			if(bSuc)
			{
				if(!strSerialID.IsEmpty())
				{
					AddToSoundAndDisplay(docInfo, strSerialID, FALSE);

					//CALLPATIENTINFO callInfo;
					//SqlQueryDoctorBaseInfo(&callInfo.sDoctorBasicInfo, strDoctorID);
					//SqlQueryPatientBySerialID(strSerialID, &callInfo);

					//刷新CPaintView中的患者状态
					((CNurseStationApp*)AfxGetApp())->m_pNurseWnd->GetPatientView()->
						ReviseListCtrPatStatus(strSerialID, qsPrepare, 
						m_mapDocIdDocInfo[strDoctorID].strDoctorName,
						m_mapDocIdDocInfo[strDoctorID].strConsultName);

					//更新护士站液晶屏、综合屏
					//UpdateDisplayList();
					//CDoctor* pDoctor = GetDoctor(strDoctorID);
					//LPDOCTORBASICINFO pDocInfo = GetDocBasInfo(strDoctorID);
					UpdateQueListData(docInfo.strOfficeID);
					//UpdateQueListDataRowType();

					CONSULTSTB consultStb;
					consultStb.strDepartName = docInfo.strOfficeName;
					consultStb.strConsultName = docInfo.strConsultName;
					consultStb.strDoctorId = docInfo.strDoctorID;
					consultStb.strDoctorName = docInfo.strDoctorName;
					consultStb.strDoctorTitle = docInfo.strTitle;
					consultStb.strStbId = docInfo.strConsultStbId;
					//
					consultStb.strCallPatient = SqlQueryCallPatient(strDoctorID);
					consultStb.strWaitPatient = SqlQueryWaitPatient(strDoctorID);
					//
					UpdateConsultStb(consultStb);
				}
				else
				{
					bSuc = FALSE;
				}
			}
		}
		else
		{
			//rset.GetFieldValue(_T("log_id"), strSerialID);
			rset.Close();
			db.Close();
			AddToSoundAndDisplay(docInfo, _T(""), FALSE);
		}
	}
	catch (_com_error& e)
	{
		bSuc = FALSE;
		WriteLog::WriteDbErrLog(_T("CNurseController::OnWait"));
	}

	SendCmdResult((LPBASEINFO)pDoctorCmd, psockddr, bSuc);
}

void CNurseController::OnEva(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockddr)
{
	CString strDoctorID(pDoctorCmd->header.sUserInfo.wszUserID);
	//查询该医生所对应评价器
	CString strSql;
	strSql.Format(_T("select evaluator_id from Doctor where doctor_id=%s;"), strDoctorID);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSql, CADORecordset::openQuery);
		if(!rset.IsBOF())
		{
			int nEvaltorID;
			rset.GetFieldValue(_T("evaluator_id"), nEvaltorID);
			if(nEvaltorID > 0)
			{
				//REQEVALUATE evltRes;
				//evltRes.header.eTypeID = ntiEvaluateResult;
				//evltRes.header.nDataLength = sizeof(evltRes);
				//srand((UINT)time(NULL));
				//evltRes.eEvaluateLevel = (EvaluateLevelEnum)(rand()%4+1);
				//wcscpy_s(evltRes.wszDoctorID, MAX_ID_LENGTH, strDoctorID.GetBuffer());
				//SendTo( (LPSTR)&evltRes, 
				//	sizeof( evltRes ), _T("127.0.0.1"));

				CString str;
				ReSetEvaluator(str);
			}
		}
		rset.Close();
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::OnEva"));
	}

	SendCmdResult((LPBASEINFO)pDoctorCmd, psockddr, TRUE);
}

void CNurseController::ReSetEvaluator(CString strEvaltorID)
{

}

void CNurseController::OnPause(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockddr)
{
	BOOL bExist = FALSE;
	CString strDoctorID(pDoctorCmd->header.sUserInfo.wszUserID);

	CDoctor* pDoctor = (CDoctor*)GetUser(strDoctorID, utDoctor);
	if(pDoctor)
	{
		USERSTATUS usbuf;
		usbuf.eUserStatus = dsPause;
		((CNurseStationApp*)AfxGetApp())->m_pDocInfoView->FlushDoctorData(usbuf);

		//CALLPATIENTINFO callinfo;
		//SqlQueryDoctorBaseInfo(&(callinfo.sDoctorBasicInfo),strDoctorID);
		VIEWUPDATEINFO viewInfo;
		viewInfo.strDoctorID = m_mapDocIdDocInfo[strDoctorID].strDoctorID;
		viewInfo.strOfficeID = m_mapDocIdDocInfo[strDoctorID].strOfficeID;
		//viewInfo.strPatientName = callinfo.strPatientName;
		//viewInfo.strSerialID = callinfo.strSerialID;
		viewInfo.strDoctorName = m_mapDocIdDocInfo[strDoctorID].strDoctorName;
		UpdateDocView(&viewInfo, dsPause);

		bExist = TRUE;
	}

	SendCmdResult((LPBASEINFO)pDoctorCmd, psockddr, bExist);
}

void CNurseController::OnResume(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockddr)
{
	BOOL bExist = FALSE;
	CString strDoctorID(pDoctorCmd->header.sUserInfo.wszUserID);
	CDoctor* pDoctor = (CDoctor*)GetUser(strDoctorID, utDoctor);
	if(pDoctor)
	{
		USERSTATUS usbuf;
		usbuf.eUserStatus = dsWorking;
		((CNurseStationApp*)AfxGetApp())->m_pDocInfoView->FlushDoctorData(usbuf);

		//CALLPATIENTINFO callinfo;
		//SqlQueryDoctorBaseInfo(&(callinfo.sDoctorBasicInfo),strDoctorID);
		VIEWUPDATEINFO viewInfo;
		viewInfo.strDoctorID = m_mapDocIdDocInfo[strDoctorID].strDoctorID;
		viewInfo.strOfficeID = m_mapDocIdDocInfo[strDoctorID].strOfficeID;
		//viewInfo.strPatientName = callinfo.strPatientName;
		//viewInfo.strSerialID = callinfo.strSerialID;
		viewInfo.strDoctorName = m_mapDocIdDocInfo[strDoctorID].strDoctorName;
		UpdateDocView(&viewInfo, dsWorking);

		bExist = TRUE;
	}

	SendCmdResult((LPBASEINFO)pDoctorCmd, psockddr, bExist);
}

//void CNurseController::AddToSoundAndDisplay(CString& strDoctorID, CString& strQueSerialID, BOOL bCall)
void CNurseController::AddToSoundAndDisplay(DOCTORBASICINFO& rDoctorBasicInfo, CString strQueSerialID, BOOL bCall)
{
	CALLPATIENTINFO cpiCall = {0};
	//SqlQueryDoctorBaseInfo(&cpiCall.sDoctorBasicInfo, strDoctorID);
	//cpiCall.sDoctorBasicInfo = m_mapDocIdDocInfo[strDoctorID];
	cpiCall.sDoctorBasicInfo = rDoctorBasicInfo;
	if(!bCall && strQueSerialID.IsEmpty())
	{
		SqlQueryWaitPatientByDoctorId(rDoctorBasicInfo.strDoctorID, &cpiCall);
	}
	else
	{
		SqlQueryPatientBySerialID(strQueSerialID, &cpiCall);
	}

	VOICEDISPLAYSTR vdstr;
	vdstr.strVoiceStr = CovertVarStr(cpiCall, bCall ? covtSoundCall : covtSoundWait);
	vdstr.strDisplayStr = CovertVarStr(cpiCall, bCall ? covtDisplayCall : covtDisplayWait);
	//vdstr.strDoctorID = strDoctorID;
	vdstr.iWndScreenId = rDoctorBasicInfo.iWndScreenId;
	vdstr.strConsultStbId = rDoctorBasicInfo.strConsultStbId;
	//vdstr.bScreen = TRUE;
	vdstr.strOfficeStbId = m_mapOffice[rDoctorBasicInfo.strOfficeID].GetStbId();
	vdstr.iCompLED = rDoctorBasicInfo.iCompLED;
	vdstr.iThroughLED = rDoctorBasicInfo.iThroughLED;
	PlayVoice(vdstr);
}

BOOL CNurseController::AddVoiceDisplay(CString strVoice, CString strStbId)
{
	VOICEDISPLAYSTR vdstr;
	vdstr.strVoiceStr = strVoice;
	vdstr.strDisplayStr = strVoice;
	vdstr.strOfficeStbId = strStbId;
	PlayVoice(vdstr);
	return TRUE;
}
/*
void CNurseController::OnUpdateQueListRset(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	//UpdateQueListData();
	if(m_pUpdateQueListThread == NULL)
		m_pUpdateQueListThread = AfxBeginThread(UpdateQueListThreadProc,this);
}
*/
UINT CNurseController::UpdateQueListThreadProc(LPVOID pParam)
{
	CNurseController* pThis = (CNurseController*)pParam;
	pThis->m_bUpdateQueListThread = TRUE;

	while(pThis->m_bUpdateQueListThread)
	{
		//for(int i = 0; i < pThis->m_arrOffice.GetCount(); i++)
		//{
		//	pThis->UpdateQueListData(pThis->m_arrOffice[i].GetOfficeId());
		//}
		POSITION pos = pThis->m_mapOffice.GetStartPosition();
		CString strKey;
		Office office;
		for(;pos;)
		{
			pThis->m_mapOffice.GetNextAssoc(pos, strKey, office);
			pThis->UpdateQueListData(office.GetOfficeId());
		}
		//pThis->UpdateQueListDataRowType();

		Sleep(2000);
	}
	//delete pThis->m_pUpdateQueListThread;
	//pThis->m_pUpdateQueListThread = NULL;
	::ExitThread(0);
	return 0;
}

BOOL CNurseController::UpdateQueListData(CString strOfficeId)
{
	CString strSql(_T(""));
	strSql.Format(_T("select Q.queue_id_call QueueID,Q.patient_name PatientName,Q.comment Comment,O.call_name OfficeName,C.call_name ConsultName,D.name DoctorName,Q.status Status from Queue Q left join Office O on O.office_id=Q.office_id left join Doctor D on D.doctor_id=Q.doctor_id left join Consult C on C.consult_id=D.consult_id where %s and O.office_id='%s' and Status in (%d,%d,%d) order by Q.doctor_id, Status,priority desc,regtime;"), 
		m_strSqlDate, strOfficeId, qsDoing, qsPrepare, qsInLine);

	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSql, CADORecordset::openQuery);
		CStbPatientList* pList = NULL;
		//for(int i = 0; i < m_arrOffice.GetCount(); i++)
		//{
		//	if(m_arrOffice[i].GetOfficeId() == strOfficeId)
		//	{
		//		pList = m_arrOffice[i].GetStbPatientList();
		//		m_arrOffice[i].SetQueueNum(rset.GetRecordCount());
		//		break;
		//	}
		//}
		m_mapOffice[strOfficeId].SetQueueNum(rset.GetRecordCount());
		pList = m_mapOffice[strOfficeId].GetStbPatientList();

		if(!pList) return TRUE;

		//for(int i = 0; i < pList->GetCount(); i++)
		//{
		//	delete (*pList)[i];
		//}
		pList->RemoveAll();

		if(!rset.IsBOF())
		{
			for(; !rset.IsEOF(); rset.MoveNext())
			{
				StbPatient patient;
				rset.GetFieldValue(_T("QueueID"), patient.strQueueId);
				rset.GetFieldValue(_T("PatientName"), patient.strPatientName);
				rset.GetFieldValue(_T("OfficeName"), patient.strOfficeName);
				rset.GetFieldValue(_T("ConsultName"), patient.strConsultName);
				rset.GetFieldValue(_T("DoctorName"), patient.strDoctorName);
				int iStatus;
				rset.GetFieldValue(_T("Status"), iStatus);
				patient.eStatus = (QueueStatusEnum)iStatus;
				rset.GetFieldValue(_T("Comment"), patient.strComment);

				pList->Add(patient);
			}
		}
		rset.Close();
		db.Close();
	}
	catch (_com_error& e)
	{
		m_mapOffice[strOfficeId].SetQueueNum(0);
		WriteLog::WriteDbErrLog(_T("CNurseController::UpdateQueListData"));
		return FALSE;
	}
	catch (...)
	{
		m_mapOffice[strOfficeId].SetQueueNum(0);
		return FALSE;
	}
	return TRUE;
}

BOOL CNurseController::SqlQueryCallingPatient(CString strDoctorID, LPVIEWUPDATEINFO pViewUpdateInfo)
{
	CString strSql;
	strSql.Format(_T("select Q.log_id SerialID, Q.patient_name PatientName, Q.office_id, D.name DoctorName,C.call_name ConsultName from Queue Q left join Doctor D on Q.doctor_id=D.doctor_id left join Consult C on D.consult_id=C.consult_id where Q.status=%d and Q.doctor_id='%s' and %s;"),
		qsDoing, strDoctorID, m_strSqlDate);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSql, CADORecordset::openQuery);
		if(rset.IsBOF())
		{
			pViewUpdateInfo->strSerialID.Empty();
			pViewUpdateInfo->strPatientName.Empty();
			pViewUpdateInfo->strOfficeID.Empty();
			pViewUpdateInfo->strDoctorID.Empty();
			pViewUpdateInfo->strDoctorName.Empty();
			pViewUpdateInfo->strConsultName.Empty();
			return FALSE;
		}
		pViewUpdateInfo->strDoctorID = strDoctorID;
		rset.GetFieldValue(_T("SerialID"), pViewUpdateInfo->strSerialID);
		rset.GetFieldValue(_T("PatientName"), pViewUpdateInfo->strPatientName);
		rset.GetFieldValue(_T("office_id"), pViewUpdateInfo->strOfficeID);
		rset.GetFieldValue(_T("DoctorName"), pViewUpdateInfo->strDoctorName);
		rset.GetFieldValue(_T("ConsultName"), pViewUpdateInfo->strConsultName);

		rset.Close();
		db.Close();

		return TRUE;
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlQueryCallingPatient"));
	}
	catch (...)
	{
	}

	return FALSE;
}

BOOL CNurseController::SqlQueryPatientBySerialID(CString strSerialID, LPCALLPATIENTINFO pCallPatientInfo)
{
	CString strSql;
	strSql.Format(_T("select queue_id_call, patient_name, patient_gender, patient_birth, status from Queue where log_id='%s';")
		, strSerialID);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		if(rset.Open(strSql, CADORecordset::openQuery))
		{
			if(rset.IsBOF())
			{
				pCallPatientInfo->strSerialID.Empty();
				pCallPatientInfo->strQueueID.Empty();
				pCallPatientInfo->strPatientName.Empty();
				pCallPatientInfo->eGender = genderNone;
				pCallPatientInfo->strBirth.Empty();
				pCallPatientInfo->iStatus = qsNotDefined;

				return FALSE;
			}
			rset.GetFieldValue(_T("queue_id_call"), pCallPatientInfo->strQueueID);
			rset.GetFieldValue(_T("patient_name"), pCallPatientInfo->strPatientName);
			int iGender;
			rset.GetFieldValue(_T("patient_gender"), iGender);
			pCallPatientInfo->eGender = (GenderEnum)iGender;
			rset.GetFieldValue(_T("patient_birth"), pCallPatientInfo->strBirth);
			rset.GetFieldValue(_T("status"), pCallPatientInfo->iStatus);
			pCallPatientInfo->strSerialID = strSerialID;

			rset.Close();
		}
		db.Close();
		return TRUE;
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlQueryPatientBySerialID"));
	}
	catch (...)
	{
	}

	return FALSE;
}

BOOL CNurseController::SqlQueryWaitPatientByDoctorId(CString strDoctorId, LPCALLPATIENTINFO pCallPatientInfo)
{
	BOOL bRet = TRUE;
	CString strSql;
	strSql.Format(_T("select queue_id_call,patient_name,patient_gender,patient_birth,status from Queue where status=%d and  doctor_id='%s' and %s;"),
		qsPrepare, strDoctorId,  m_strSqlDate);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		if(rset.Open(strSql, CADORecordset::openQuery))
		{
			if(rset.IsBOF())
			{
				pCallPatientInfo->strSerialID.Empty();
				pCallPatientInfo->strQueueID.Empty();
				pCallPatientInfo->strPatientName.Empty();
				pCallPatientInfo->eGender = genderNone;
				pCallPatientInfo->strBirth.Empty();
				pCallPatientInfo->iStatus = qsNotDefined;

				bRet = FALSE;
			}
			else
			{
				CString strValue;
				for( ; !rset.IsEOF(); rset.MoveNext())
				{
					rset.GetFieldValue(_T("queue_id_call"), strValue);
					pCallPatientInfo->strQueueID += strValue + _T("号");
					rset.GetFieldValue(_T("patient_name"), strValue);
					pCallPatientInfo->strPatientName += strValue + _T("，");
				}
				pCallPatientInfo->strQueueID.Delete(pCallPatientInfo->strQueueID.GetLength() - 1);
				pCallPatientInfo->strPatientName.Delete(pCallPatientInfo->strPatientName.GetLength() - 1);
			}
			rset.Close();
		}
		db.Close();
	}
	catch (_com_error& e)
	{
		bRet = FALSE;
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlQueryPatientBySerialID"));
	}
	catch (...)
	{
		bRet = FALSE;
	}

	return bRet;
}

BOOL CNurseController::SqlGetNurseFromDB()
{
	BOOL bRet = FALSE;

	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		CString strSql;

		strSql.Format(_T("select NO.office_id,O.name,N.nurse_name from Nurse_Office NO,Office O,Nurse N where NO.office_id=O.office_id and NO.nurse_id=N.nurse_id and NO.nurse_id='%s';"),
			GetUserId());
		if(rset.Open(strSql, CADORecordset::openQuery))
		{
			if(!rset.IsBOF())
			{
				CString strOffices;
				for(; !rset.IsEOF(); rset.MoveNext())
				{
					CString strOfficeId, strOfficeName;
					rset.GetFieldValue(0, strOfficeId);
					rset.GetFieldValue(1, strOfficeName);
					m_arrstrOfficeId.Add(strOfficeId);
					strOffices += strOfficeId + _T(",") + strOfficeName + _T("|");
				}
				SetOffices(strOffices);

				rset.MoveFirst();
				CString strNurseName;
				rset.GetFieldValue(2, strNurseName);
				SetUserName(strNurseName);

				bRet = TRUE;
			}
			rset.Close();
		}
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlGetNurseFromDB"));
	}
	catch (...)
	{
	}

	return bRet;
}

//获取医生呼叫时信息///////////////////////////////////////////////
BOOL CNurseController::SqlQueryDoctorBaseInfo(DOCTORBASICINFO& rDoctorBasicInfo,
									  CString strDoctorID)
{
	BOOL bRet = FALSE;

	CString strSql;
	strSql.Format(_T("select D.name DoctorName,D.sound_call,D.sound_wait,D.display_call,D.display_wait,D.max_recall_times,D.expert_flag,D.title,D.photo,D.wait_num,D.wait_when_call_flag,D.evaluator_id,D.caller_id,O.office_id ,O.call_name OfficeName,C.consult_id,C.call_name ConsultName,C.wnd_screen_id,C.stb_id ConsultStbId,C.compreh_screen_id CompLED,C.through_screen_id ThroughLED from (Select * from Doctor where doctor_id='%s') D left join Office O on O.office_id=D.office_id left join Consult C on C.Consult_ID=D.consult_id;") 
		,strDoctorID);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		if(rset.Open(strSql, CADORecordset::openQuery))
		{
			if(!rset.IsBOF())
			{
				rDoctorBasicInfo.strDoctorID = strDoctorID;
				rset.GetFieldValue(_T("OfficeName"), rDoctorBasicInfo.strOfficeName);
				rset.GetFieldValue(_T("ConsultName"), rDoctorBasicInfo.strConsultName);
				rset.GetFieldValue(_T("DoctorName"), rDoctorBasicInfo.strDoctorName);
				rset.GetFieldValue(_T("sound_call"), rDoctorBasicInfo.strSoundCall);
				rset.GetFieldValue(_T("sound_wait"), rDoctorBasicInfo.strSoundWait);
				rset.GetFieldValue(_T("display_call"), rDoctorBasicInfo.strDisplayCall);
				rset.GetFieldValue(_T("display_wait"), rDoctorBasicInfo.strDisplayWait);
				rset.GetFieldValue(_T("office_id"), rDoctorBasicInfo.strOfficeID);
				rset.GetFieldValue(_T("consult_id"), rDoctorBasicInfo.strConsultID);
				rset.GetFieldValue(_T("max_recall_times"), rDoctorBasicInfo.iMaxRecallTimes);
				rset.GetFieldValue(_T("wnd_screen_id"), rDoctorBasicInfo.iWndScreenId);
				rset.GetFieldValue(_T("evaluator_id"), rDoctorBasicInfo.iEvaluatorId);
				rset.GetFieldValue(_T("evaluator_id"), rDoctorBasicInfo.iCallerId);
				rset.GetFieldValue(_T("ConsultStbId"), rDoctorBasicInfo.strConsultStbId);
				rset.GetFieldValue(_T("expert_flag"), rDoctorBasicInfo.bExpertFlag);
				rset.GetFieldValue(_T("title"), rDoctorBasicInfo.strTitle);
				rset.GetFieldValue(_T("wait_num"), rDoctorBasicInfo.iWaitNum);
				rset.GetFieldValue(_T("wait_when_call_flag"), rDoctorBasicInfo.bWaitWhenCall);
				rset.GetFieldValue(_T("CompLED"), rDoctorBasicInfo.iCompLED);
				rset.GetFieldValue(_T("ThroughLED"), rDoctorBasicInfo.iThroughLED);

				if(!rset.IsFieldNull(_T("photo")))
				{
					CADOFieldInfo fi;
					rset.GetFieldInfo(_T("photo"), &fi);
					if(fi.m_lSize > 0)
					{
						_variant_t var;
						rset.GetFieldValue(_T("photo"), var);
						if(var.vt == VT_BYREF|VT_UI1 && var.vt != VT_NULL && var.vt != VT_EMPTY)
						{
							//m_strPhotoPath= m_strPhotoDir + m_strId + _T(".jpg");
							CFile imageFile;
							if(imageFile.Open(
								m_strWebRootDir + _T("photoTemp/" + strDoctorID + _T(".jpg")), 
								CFile::modeCreate|CFile::modeWrite))
							{
								BYTE* pbuf = new BYTE[fi.m_lSize];
								if(pbuf != NULL)
								{
									BYTE* pbufTemp = pbuf;
									SafeArrayAccessData(var.parray, (void**)&pbuf);
									imageFile.Write(pbuf, fi.m_lSize);
									imageFile.Close();
									SafeArrayUnaccessData(var.parray);
									delete[] pbufTemp;
								}
							}
						}
					}
				}

				bRet = TRUE;
			}
			rset.Close();
		}
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlQueryDoctorBaseInfo"));
	}
	catch (...)
	{
	}

	return bRet;
}

BOOL CNurseController::SqlUpdateCallToDone(CString strDoctorID)
{
	CString strSql;
	strSql.Format(_T("Update Queue set status=%d where status=%d and doctor_id='%s';"), 
		qsDone, qsDoing, strDoctorID);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		db.Execute(strSql);
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlUpdateCallToDone"));
		return FALSE;
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CNurseController::SqlUpdateCallToDiscard(CString strDoctorID)
{
	CString strSql;
	strSql.Format(_T("Update Queue set status=%d where status=%d and doctor_id='%s';"), 
		qsDiscard, qsDoing, strDoctorID);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		db.Execute(strSql);
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlUpdateCallToDiscard"));
		return FALSE;
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CNurseController::SqlUpdateWaitToInLine(CString strDoctorID)
{
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		CString strSql;
		strSql.Format(_T("select expert_flag from doctor where doctor_id='%s';"), 
			strDoctorID);
		rset.Open(strSql);
		if(rset.IsBOF()) return FALSE;
		int nExpertFlag;
		rset.GetFieldValue(0, nExpertFlag);
		if(nExpertFlag == 1)
		{
			strSql.Format(_T("Update Queue set status=%d where status=%d and doctor_id='%s';"), 
				qsInLine, qsPrepare, strDoctorID);
		}
		else
		{
			strSql.Format(_T("Update Queue set status=%d,doctor_id=null where status=%d and doctor_id='%s';"), 
				qsInLine, qsPrepare, strDoctorID);
		}
		db.Execute(strSql);
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlUpdateWaitToInLine"));
		return FALSE;
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}

/*
BOOL CNurseController::SqlUpdateToCall(CString strDoctorID, CString strQueSerialID)
{
	CString strSql;
	strSql.Format(_T("Update Queue set status=%d, doctor_id='%s' where log_id='%s';"), 
		qsDoing, strDoctorID, strQueSerialID);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		db.Execute(strSql);
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlUpdateToCall"));
		return FALSE;
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}
*/

BOOL CNurseController::SqlUpdateToCall(CString strDoctorID, CString strQueSerialID)
{
	BOOL bSuc = TRUE;
	CString strSql;
	strSql.Format(_T("select status,doctor_id from Queue where log_id='%s';"), 
		strQueSerialID);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSql, CADORecordset::openQuery);
		if(!rset.IsBOF())
		{
			int nStatus;
			CString strDoctor;
			rset.GetFieldValue(_T("status"), nStatus);
			rset.GetFieldValue(_T("doctor_id"), strDoctor);
			if((nStatus == qsInLine && (strDoctor == strDoctorID || strDoctor.IsEmpty()))
				|| (nStatus == qsPrepare && strDoctor == strDoctorID))
			{
				rset.Edit();
				rset.SetFieldValue(_T("status"), qsDoing);
				rset.SetFieldValue(_T("doctor_id"), strDoctorID);
				rset.Update();
			}
			else
			{
				bSuc = FALSE;
			}
		}
		else
		{
			bSuc = FALSE;
		}
		rset.Close();
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlUpdateToCall"));
		bSuc = FALSE;
	}
	catch (...)
	{
		bSuc = FALSE;
	}

	return bSuc;
}

/*
BOOL CNurseController::SqlUpdateToWait(CString strDoctorID, CString strQueSerialID)
{
	CString strSql;
	strSql.Format(_T("Update Queue set status=%d, doctor_id='%s' where log_id='%s';"), 
		qsPrepare, strDoctorID, strQueSerialID);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		db.Execute(strSql);
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlUpdateToWait"));
		return FALSE;
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}
*/

BOOL CNurseController::SqlUpdateToWait(CString strDoctorID, CString strQueSerialID)
{
	BOOL bSuc = TRUE;
	CString strSql;
	strSql.Format(_T("select status,doctor_id from Queue where log_id='%s';"), 
		strQueSerialID);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSql, CADORecordset::openQuery);
		if(!rset.IsBOF())
		{
			int nStatus;
			CString strDoctor;
			rset.GetFieldValue(_T("status"), nStatus);
			rset.GetFieldValue(_T("doctor_id"), strDoctor);
			if(nStatus == qsInLine && (strDoctor == strDoctorID || strDoctor.IsEmpty()))
			{
				rset.Edit();
				rset.SetFieldValue(_T("status"), qsPrepare);
				rset.SetFieldValue(_T("doctor_id"), strDoctorID);
				rset.Update();
			}
			else
			{
				bSuc = FALSE;
			}
		}
		else
		{
			bSuc = FALSE;
		}
		rset.Close();
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlUpdateToCall"));
		bSuc = FALSE;
	}
	catch (...)
	{
		bSuc = FALSE;
	}

	return bSuc;
}

//生成患者列表发给医生端///////////////////////////////////////////////
BOOL CNurseController::SendCmdResult(LPBASEINFO lpBaseInfo, const LPSOCKADDR_IN psockddr, BOOL bSucceed)
{
	CMDRESULT cmdres;
	memset(&cmdres, 0, sizeof(cmdres));
	SetSendBaseInfo((LPBASEINFO)&cmdres, ntiCmdResult, sizeof(cmdres));
	cmdres.sUserInfoFrom = lpBaseInfo->header.sUserInfo;
	cmdres.eTypeIDFrom = lpBaseInfo->header.eTypeID;
	cmdres.bSucceed = bSucceed;
	DBPARAM dbParam;
	cmdres.sDbParam = *(m_DbParam.ToDBPARAM(&dbParam));
	//////////////////////////////////////////////////////////////////////
	CString strDoctorID(lpBaseInfo->header.sUserInfo.wszUserID);

	
	//if(lpBaseInfo->header.eTypeID == ntiDoctorCmd)
	//{
	//	DoctorCmdEnum eCmd = ((LPDOCTORCMD)lpBaseInfo)->eDoctorCmd;
	//	cmdres.eDoctorCmd = eCmd;
	//	if(eCmd == dcCmdCall 
	//		|| eCmd == dcCmdWait 
	//		|| eCmd == dcCmdDiscard
	//		|| lpBaseInfo->header.eTypeID == ntiLogin 
	//		&& lpBaseInfo->header.sUserInfo.eUserType == utDoctor)
	//	{
	//		SqlQueryPatients(strDoctorID, &cmdres);
	//	}
	//}
	//else if(lpBaseInfo->header.eTypeID == ntiLogin 
	//	&& lpBaseInfo->header.sUserInfo.eUserType == utDoctor)
	//{
	//	SqlQueryPatients(strDoctorID, &cmdres);
	//}
	

	if(lpBaseInfo->header.eTypeID == ntiDoctorCmd)
	{
		cmdres.eDoctorCmd = ((LPDOCTORCMD)lpBaseInfo)->eDoctorCmd;
	}

	return SendTo((CHAR*)&cmdres, sizeof(cmdres), psockddr);
}

BOOL CNurseController::SqlQueryPatients(CString strDoctorId, LPCMDRESULT pCmdResult)
{
	BOOL bRet = TRUE;

	INT uCount = 0;
	int nIndex = 0;
	try
	{
		CString strSql;
		//strSql.Format(_T("Select log_id,his_queue_id queue_id_call,patient_name,patient_gender,patient_month patient_age,status,doctor_id from Queue where doctor_id='%s' and status=%d;"),strDoctorID , qsDoing);
		strSql.Format(_T("Select count(log_id) from Queue where doctor_id='%s' and status in (%d,%d,%d) and %s;"), 
			strDoctorId , qsDoing, qsPrepare, qsInLine, m_strSqlDate);
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSql);
		rset.GetFieldValue(0, uCount);
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlQueryPatients"));
		bRet = FALSE;
	}
	catch (...)
	{
		bRet = FALSE;
	}
	pCmdResult->nPatientCount = uCount;
	//pCmdResult->nSentCount = nIndex;

	return bRet;
}

void CNurseController::PlayVoice(VOICEDISPLAYSTR vdstr)
{
	if(m_pJttsAPI)
	{
		m_pJttsAPI->AddPlayText(vdstr);
	}
}

BOOL CNurseController::StbNoticeHtmlExist(CString strStbId)
{
	CString strPath = m_strWebRootDir + strStbId + _T("/") + _T("webinfo2.html");
	return MyCommon::PathFileExist(strPath);
}

BOOL CNurseController::StbUpdateTitleHtml(CString strTitle, CString strStbId)
{
	CString str(m_strHtmlTitleBegin);
	str += strTitle;
	str += m_strHtmlTitleEnd;

	CString strExePath = m_strModuleDir + _T("sys\\stb_keshi.ini");
	::WritePrivateProfileString(_T("sys"),_T("TITLE"), strTitle, strExePath);

	return WriteToStbHtmlFile(str, strStbId, _T("webinfo1.html"));
}

BOOL CNurseController::StbUpdateNoticeHtml(CString strNotice, CString strStbId)
{
	CString str(m_strHtmlNoticeBegin);
	str += strNotice;
	str += m_strHtmlNoticeEnd;
	return WriteToStbHtmlFile(str, strStbId, _T("webinfo2.html"));
}

/*
BOOL CNurseController::StbUpdateCallInfo(CString strCallInfo)
{
	if(m_strNurseStbId.IsEmpty())
	{
		return FALSE;
	}

	strCallInfo = CovertColorString(strCallInfo);

	return StbUpdateInstantMsg(strCallInfo, m_strNurseStbId);
}
*/
BOOL CNurseController::StbUpdateCallInfo(CString strCallInfo, CString strStbId)
{
	//if(m_strNurseStbId.IsEmpty())
	//{
	//	return FALSE;
	//}

	strCallInfo = CovertColorString(strCallInfo);

	return StbUpdateInstantMsg(strCallInfo, strStbId);
}


BOOL CNurseController::StbUpdateInstantMsg(CString strIMsg, CString strStbId)
{
	CString str(m_strHtmlInstMsgBegin);
	str += strIMsg;
	str += m_strHtmlInstMsgEnd;
	//if(strStbId == m_strNurseStbId)
	//{
	//	if(m_pDisplayThread)
	//	{
	//		//m_pDisplayThread->SuspendThread();
	//		//::SetTimer(this->m_hWnd, TIMERID_DISPLAY_CALL, m_iHtmlShowCallTime * 1000, NULL);
	//		//m_ptimerDisplayCall->KillTimer();
	//		//m_ptimerDisplayCall->SetTimer();
	//		//m_callbackResumeQue.Assign(this, &CNurseController::OnResumeDisplay);
	//		//m_idEventResumeQue = ::SetTimer(NULL, 0, m_iHtmlShowCallTime * 1000, m_callbackResumeQue);
	//	}
	//	m_bSleepDisplayThread = TRUE;
	//}
	if(strStbId != _T("0"))
	{
		m_mapStb[strStbId].nShowMsg = m_iHtmlShowCallTime;
	}
	else
	{
		CMap<CString, LPCTSTR, StbData, StbData&>::CPair* pPair = NULL;
		pPair = m_mapStb.PGetFirstAssoc();
		for(;pPair;pPair = m_mapStb.PGetNextAssoc(pPair))
		{
			if(pPair->key.IsEmpty()) continue;
			pPair->value.nShowMsg = m_iHtmlShowCallTime;
		}
	}

	return WriteToStbHtmlFile(str, strStbId, _T("webinfo.html"));
}

BOOL CNurseController::StbUpdateQueueInfo(CString strStbId)
{
	if(strStbId.IsEmpty()) return FALSE;
	
	switch(m_nShowType)
	{
	case 0:
		{
			if(m_mapStb[strStbId].arrOffice.GetCount() == 0)
			{
				m_mapStb[strStbId].strHtmlTemp = _T("");
				return TRUE;
			}
			else if(m_mapStb[strStbId].arrOffice.GetCount() == 1)
			{
				return StbUpdateQueueInfoHtmlTypeRow(strStbId);
			}
			else
			{
				if(m_mapStb[strStbId].arrOffice.GetCount() <= m_nPageColCount)
				{
					return StbUpdateQueueInfoHtmlTypeCol(strStbId);
				}
				else
				{
					return StbUpdateQueueInfoHtmlTypeSwitchCol(strStbId);
				}
			}
		}
		break;
	case 1:
		{
			return StbUpdateQueueInfoHtmlTypeRow(strStbId);
			
		}
		break;
	case 2:
		{
			return StbUpdateQueueInfoHtmlTypeCol(strStbId);
		}
		break;
	case 3:
		{
			return StbUpdateQueueInfoHtmlTypeSwitchCol(strStbId);
		}
		break;
	default:
		{
			if(m_mapStb[strStbId].arrOffice.GetCount() == 0)
			{
				m_mapStb[strStbId].strHtmlTemp = _T("");
				return TRUE;
			}
			else if(m_mapStb[strStbId].arrOffice.GetCount() == 1)
			{
				return StbUpdateQueueInfoHtmlTypeRow(strStbId);
			}
			else
			{
				if(m_mapStb[strStbId].arrOffice.GetCount() <= m_nPageColCount)
				{
					return StbUpdateQueueInfoHtmlTypeCol(strStbId);
				}
				else
				{
					return StbUpdateQueueInfoHtmlTypeSwitchCol(strStbId);
				}
			}
		}
	}
}

BOOL CNurseController::StbUpdateQueueInfoHtmlTypeRow(CString strStbId)
{
	CArray<StbPatient> rowList;
	//for(int i = 0; i < m_arrOffice.GetCount(); i++)
	//{
	//	for(int j = 0; j < m_arrOffice[i].GetStbPatientList()->GetCount(); j++)
	//	{
	//		if(m_arrOffice[i].GetStbPatientList()->GetAt(j).eStatus == qsDoing)
	//		{
	//			rowList.Add(m_arrOffice[i].GetStbPatientList()->GetAt(j));
	//		}
	//	}
	//}
	for(int i = 0; i < m_mapStb[strStbId].arrOffice.GetCount(); i++)
	{
		for(int j = 0; j < m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetStbPatientList()->GetCount(); j++)
		{
			try
			{
				if(m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetStbPatientList()->GetAt(j).eStatus == qsDoing)
				{
					rowList.Add(m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetStbPatientList()->GetAt(j));
				}
			}
			catch (CException* e)
			{
			}
		}
	}

	for(int i = 0; i < m_mapStb[strStbId].arrOffice.GetCount(); i++)
	{
		for(int j = 0; j < m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetStbPatientList()->GetCount(); j++)
		{
			try
			{
				if(m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetStbPatientList()->GetAt(j).eStatus == qsPrepare)
				{
					rowList.Add(m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetStbPatientList()->GetAt(j));
				}
			}
			catch (CException* e)
			{
			}
		}
	}

	for(int i = 0; i < m_mapStb[strStbId].arrOffice.GetCount(); i++)
	{
		for(int j = 0; j < m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetStbPatientList()->GetCount(); j++)
		{
			try
			{
				if(m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetStbPatientList()->GetAt(j).eStatus == qsInLine)
				{
					rowList.Add(m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetStbPatientList()->GetAt(j));
				}
			}
			catch (CException* e)
			{
			}
		}
	}

	CString strColorHead;
	strColorHead.Format(_T("<font color=\"%s\">"), m_strColorHead);

	CString strTableHead(_T("<tr>"));
	if(m_bShowLineNum)
	{
		strTableHead += _T("<th></th>");
	}
	BOOL bMultiOffice = FALSE;
	if(m_mapStb[strStbId].arrOffice.GetCount() > 1)
	{
		bMultiOffice = TRUE;
	}
	if(bMultiOffice && m_bShowOffice)
	{
		strTableHead += _T("<th>") + strColorHead + _T("科室</font></th>");
	}
	if(m_bShowQueueNum)
	{
		strTableHead += _T("<th>") + strColorHead + _T("号序</font></th>");
	}
	strTableHead += _T("<th>") + strColorHead + _T("患者</font></th>");
	if(m_bShowDoctor)
	{
		strTableHead += _T("<th>") + strColorHead + _T("医生</font></th>");
	}
	if(m_bShowConsult)
	{
		strTableHead += _T("<th>") + strColorHead + _T("诊室</font></th>");
	}
	
	strTableHead += _T("<th>") + strColorHead + _T("状态</font></th></tr>");

	CString strHtml(m_strHtmlQueueInfoBegin + strTableHead);

	CString strHtmlRow1(_T("<tr class=\"ord\">"));
	CString strHtmlRow2(_T("<tr class=\"alt\">"));

	//static int nIndex = 0;
	if(m_mapStb[strStbId].nIndex > rowList.GetCount() - 1)
	{
		m_mapStb[strStbId].nIndex = 0;
	}

	CString strColorLineNum;
	strColorLineNum.Format(_T("<font color=\"%s\">"), m_strColorLineNum);

	for(int i = 0; i < m_iHtmlPageItemCount && m_mapStb[strStbId].nIndex < rowList.GetCount(); i++, m_mapStb[strStbId].nIndex++)
	{
		if(!m_bSwitchPage)
		{
			m_mapStb[strStbId].nIndex = i;
		}
		CString strRow(i%2 == 0 ? strHtmlRow1 : strHtmlRow2);
		CString strRowColor;
		CString strStatus;
		if(rowList[m_mapStb[strStbId].nIndex].eStatus == qsDoing)
		{
			strRowColor = _T("<font color=\"") + m_strColorCall + _T("\">");
			strStatus = _T("呼叫");
		}
		else if(rowList[m_mapStb[strStbId].nIndex].eStatus == qsPrepare)
		{
			strRowColor = _T("<font color=\"") + m_strColorWait + _T("\">");
			strStatus = _T("准备");
		}
		else
		{
			strRowColor = _T("<font color=\"") + m_strColorInLine + _T("\">");
			strStatus = _T("排队");
		}
		if(!rowList[m_mapStb[strStbId].nIndex].strComment.IsEmpty())
		{
			strStatus += _T("(");
			strStatus += rowList[m_mapStb[strStbId].nIndex].strComment;
			strStatus += _T(")");
		}
		
		if(m_bShowLineNum)
		{
			strRow += _T("<td>");
			strRow += strColorLineNum;
			strRow += MyString::Int2Str(m_mapStb[strStbId].nIndex + 1);
			strRow += _T("</font></td>");
		}
		if(bMultiOffice && m_bShowOffice)
		{
			strRow += _T("<td>") + strRowColor;
			strRow += rowList[m_mapStb[strStbId].nIndex].strOfficeName;
			strRow += _T("</font></td>");
		}
		if(m_bShowQueueNum)
		{
			strRow += _T("<td>") + strRowColor;
			strRow += rowList[m_mapStb[strStbId].nIndex].strQueueId;
			strRow += _T("</font></td>");
		}
		strRow += _T("<td>") + strRowColor;
		if(!rowList[m_mapStb[strStbId].nIndex].strPatientName.IsEmpty())
		{
			strRow += rowList[m_mapStb[strStbId].nIndex].strPatientName;
		}
		else
		{
			strRow += rowList[m_mapStb[strStbId].nIndex].strQueueId;
		}
		strRow += _T("</font></td>");
		if(m_bShowDoctor)
		{
			strRow += _T("<td>") + strRowColor;
			strRow += rowList[m_mapStb[strStbId].nIndex].strDoctorName;
			strRow += _T("</font></td>");
		}
		if(m_bShowConsult)
		{
			strRow += _T("<td>") + strRowColor;
			strRow += rowList[m_mapStb[strStbId].nIndex].strConsultName;
			strRow += _T("</font></td>");
		}

		strRow += _T("<td>") + strRowColor;
		strRow += strStatus;
		strRow += _T("</font></td>");
		strRow += _T("</tr>");
		strHtml += strRow;
	}
	strHtml += m_strHtmlQueueInfoEnd;
	m_mapStb[strStbId].strHtmlTemp = strHtml;

	return TRUE;
}

BOOL CNurseController::StbUpdateQueueInfoHtmlTypeCol(CString strStbId)
{
	CString strColorHead;
	strColorHead.Format(_T("<font color=\"%s\">"), m_strColorHead);

	CString strTableHead(_T("<tr>"));
	if(m_bShowLineNum)
	{
		strTableHead += _T("<th></th>");
	}
	int nMaxCount = 0;

	for(int i = 0; i < m_mapStb[strStbId].arrOffice.GetCount(); i++)
	{
		if(m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetStbPatientList()->GetCount() > nMaxCount)
		{
			nMaxCount = m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetStbPatientList()->GetCount();
		}
		strTableHead +=  _T("<th>") + strColorHead + m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetOfficeName();
		if(m_bShowSum)
		{
			strTableHead +=   _T("(") + MyString::Int2Str(m_mapOffice[m_mapStb[strStbId].arrOffice[i]].GetQueueNum())  + _T("人)");
		}
		strTableHead +=  _T("</font></th>");
	}
	strTableHead += _T("</tr>");
	CString strHtml(m_strHtmlQueueInfoBegin + strTableHead);

	CString strHtmlRow1(_T("<tr class=\"ord\">"));
	CString strHtmlRow2(_T("<tr class=\"alt\">"));

	//static int nIndex = 0;
	if(m_mapStb[strStbId].nIndex > nMaxCount - 1)
	{
		m_mapStb[strStbId].nIndex = 0;
	}

	CString strColorLineNum;
	strColorLineNum.Format(_T("<font color=\"%s\">"), m_strColorLineNum);

	for(int i = 0; i < m_iHtmlPageItemCount && m_mapStb[strStbId].nIndex < nMaxCount; i++,m_mapStb[strStbId].nIndex++)
	{
		if(!m_bSwitchPage)
		{
			m_mapStb[strStbId].nIndex = i;
		}
		CString strRow(m_mapStb[strStbId].nIndex % 2 == 0 ? strHtmlRow1 : strHtmlRow2);

		if(m_bShowLineNum)
		{
			strRow += _T("<td>");
			strRow += strColorLineNum;
			strRow += MyString::Int2Str(m_mapStb[strStbId].nIndex + 1);
			strRow += _T("</font></td>");
		}

		for(int j = 0; j < m_mapStb[strStbId].arrOffice.GetCount(); j++)
		{
			strRow += _T("<td>");
			if(m_mapStb[strStbId].nIndex < m_mapOffice[m_mapStb[strStbId].arrOffice[j]].GetStbPatientList()->GetCount())
			{
				CString strShowName;
				if(m_bShowQueueNum)
				{
					strShowName = _T("[") + m_mapOffice[m_mapStb[strStbId].arrOffice[j]].GetStbPatientList()->GetAt(m_mapStb[strStbId].nIndex).strQueueId + _T("]") ;
				}
				strShowName += m_mapOffice[m_mapStb[strStbId].arrOffice[j]].GetStbPatientList()->GetAt(m_mapStb[strStbId].nIndex).strPatientName;
				if(strShowName.IsEmpty())
				{
					strShowName = m_mapOffice[m_mapStb[strStbId].arrOffice[j]].GetStbPatientList()->GetAt(m_mapStb[strStbId].nIndex).strQueueId;
				}
				if(!m_mapOffice[m_mapStb[strStbId].arrOffice[j]].GetStbPatientList()->GetAt(m_mapStb[strStbId].nIndex).strComment.IsEmpty())
				{
					strShowName += _T("(");
					strShowName += m_mapOffice[m_mapStb[strStbId].arrOffice[j]].GetStbPatientList()->GetAt(m_mapStb[strStbId].nIndex).strComment;
					strShowName += _T(")");
				}
				CString strRowColor;
				if(m_mapOffice[m_mapStb[strStbId].arrOffice[j]].GetStbPatientList()->GetAt(m_mapStb[strStbId].nIndex).eStatus == qsDoing)
				{
					strRowColor = _T("<font color=\"") + m_strColorCall + _T("\">");
				}
				else if(m_mapOffice[m_mapStb[strStbId].arrOffice[j]].GetStbPatientList()->GetAt(m_mapStb[strStbId].nIndex).eStatus == qsPrepare)
				{
					strRowColor = _T("<font color=\"") + m_strColorWait + _T("\">");
				}
				else
				{
					strRowColor = _T("<font color=\"") + m_strColorInLine + _T("\">");
				}
				strRow += strRowColor + strShowName + _T("</font>");
			}
			strRow += _T("</td>");
		}
		strRow += _T("</tr>");
		strHtml += strRow;
	}
	strHtml += m_strHtmlQueueInfoEnd;
	m_mapStb[strStbId].strHtmlTemp = strHtml;

	return TRUE;
}

BOOL CNurseController::StbUpdateQueueInfoHtmlTypeSwitchCol(CString strStbId)
{
	CString strColorHead;
	strColorHead.Format(_T("<font color=\"%s\">"), m_strColorHead);

	CString strTableHead(_T("<tr>"));
	if(m_bShowLineNum)
	{
		strTableHead += _T("<th></th>");
	}
	static int nCol = 0;

	if(nCol > m_mapStb[strStbId].arrOffice.GetCount() - 1)
	{
		nCol = 0;
	}

	for(int i = 0, nColTemp = nCol; i < m_nPageColCount && nColTemp < m_mapStb[strStbId].arrOffice.GetCount(); i++, nColTemp++)
	{
		strTableHead += _T("<th>") + strColorHead + m_mapOffice[m_mapStb[strStbId].arrOffice[nColTemp]].GetOfficeName();
		if(m_bShowSum)
		{
			strTableHead += _T("(") + MyString::Int2Str(m_mapOffice[m_mapStb[strStbId].arrOffice[nColTemp]].GetQueueNum())  + _T("人)");
		}
		strTableHead += _T("</font></th>");

	}
	strTableHead += _T("</tr>");

	CString strHtml(m_strHtmlQueueInfoBegin + strTableHead);

	CString strHtmlRow1(_T("<tr class=\"ord\">"));
	CString strHtmlRow2(_T("<tr class=\"alt\">"));

	CString strColorLineNum;
	strColorLineNum.Format(_T("<font color=\"%s\">"), m_strColorLineNum);

	for(int i = 0; i < m_iHtmlPageItemCount; i++)
	{
		CString strRow(i%2 == 0 ? strHtmlRow1 : strHtmlRow2);
		if(m_bShowLineNum)
		{
			strRow += _T("<td>");
			strRow += strColorLineNum;
			strRow += MyString::Int2Str(i + 1);
			strRow += _T("</font></td>");
		}
		for(int j = 0, nColTemp = nCol; j < m_nPageColCount && nColTemp < m_mapStb[strStbId].arrOffice.GetCount(); j++, nColTemp++)
		{
			strRow += _T("<td>");
			if(i < m_mapOffice[m_mapStb[strStbId].arrOffice[nColTemp]].GetStbPatientList()->GetCount())
			{
				CString strShowName;
				if(m_bShowQueueNum)
				{
					strShowName = _T("[") + m_mapOffice[m_mapStb[strStbId].arrOffice[nColTemp]].GetStbPatientList()->GetAt(i).strQueueId + _T("]") ;
				}
				strShowName += m_mapOffice[m_mapStb[strStbId].arrOffice[nColTemp]].GetStbPatientList()->GetAt(i).strPatientName;
				if(strShowName.IsEmpty())
				{
					strShowName = m_mapOffice[m_mapStb[strStbId].arrOffice[nColTemp]].GetStbPatientList()->GetAt(i).strQueueId;
				}
				if(!m_mapOffice[m_mapStb[strStbId].arrOffice[nColTemp]].GetStbPatientList()->GetAt(i).strComment.IsEmpty())
				{
					strShowName += _T("(");
					strShowName += m_mapOffice[m_mapStb[strStbId].arrOffice[nColTemp]].GetStbPatientList()->GetAt(i).strComment;
					strShowName += _T(")");
				}
				CString strRowColor;
				if(m_mapOffice[m_mapStb[strStbId].arrOffice[nColTemp]].GetStbPatientList()->GetAt(i).eStatus == qsDoing)
				{
					strRowColor = _T("<font color=\"") + m_strColorCall + _T("\">");
				}
				else if(m_mapOffice[m_mapStb[strStbId].arrOffice[nColTemp]].GetStbPatientList()->GetAt(i).eStatus == qsPrepare)
				{
					strRowColor = _T("<font color=\"") + m_strColorWait + _T("\">");
				}
				else
				{
					strRowColor = _T("<font color=\"") + m_strColorInLine + _T("\">");
				}
				strRow += strRowColor + strShowName + _T("</font>");
			}
			strRow += _T("</td>");
		}
		strRow += _T("</tr>");
		strHtml += strRow;
	}

	nCol += m_nPageColCount;
	strHtml += m_strHtmlQueueInfoEnd;
	m_mapStb[strStbId].strHtmlTemp = strHtml;

	return TRUE;
}

BOOL CNurseController::StbUpdateQueueInfoHtml(CString strStbId)
{
	//return ::CopyFile(m_strWebRootDir + _T("/webinfo.html"), 
	//	m_strWebRootDir + _T("/") + strStbId + _T("/webinfo.html"), FALSE);
	//if(m_strQueHtml.IsEmpty())
	//{
	//	StbUpdateQueueInfoHtml();
	//}
	return WriteToStbHtmlFile(m_mapStb[strStbId].strHtmlTemp, strStbId, _T("webinfo.html"));
}
/*
BOOL CNurseController::StbUpdateQueueInfoHtmlThreadProc()
{
	if(m_strNurseStbId.IsEmpty())
	{
		return FALSE;
	}

	return StbUpdateQueueInfoHtml(m_strNurseStbId);
}
*/
BOOL CNurseController::WriteToStbHtmlFile(CString& strHtmlStr, CString strStbId, CString strHtmlFileName)
{
	if(strStbId.IsEmpty()) return FALSE;
	BOOL bRet = TRUE;
	if(strStbId != _T("0"))
	{
		CString strPath = m_strWebRootDir + strStbId;
		if(MyCommon::CreatePath(strPath))
		{
			strPath = strPath + _T("/") + strHtmlFileName;
			bRet = WriteToFile(strHtmlStr, strPath);
		}
		else
		{
			bRet = FALSE;
		}
	}
	else
	{
		CMap<CString, LPCTSTR, StbData, StbData&>::CPair* pPair = NULL;
		pPair = m_mapStb.PGetFirstAssoc();
		for(;pPair;pPair = m_mapStb.PGetNextAssoc(pPair))
		{
			if(pPair->key.IsEmpty()) continue;
			CString strPath = m_strWebRootDir + pPair->key;
			if(MyCommon::CreatePath(strPath))
			{
				strPath = strPath + _T("/") + strHtmlFileName;
				bRet = WriteToFile(strHtmlStr, strPath);
			}
			else
			{
				bRet = FALSE;
			}
		}
	}
	return bRet;
}


BOOL CNurseController::WriteToFile(CString& strText, CString& strFilePath)
{
	CFile htmlFile;
	if(!htmlFile.Open(strFilePath, CFile::modeWrite | CFile::modeCreate))
	{
		return FALSE;
	}
#ifdef _DEBUG
	//MyWriteConsole(_T("Write To: ") + strFilePath + _T("\nWriting: ") + strText);
	//int iOldLen = strText.GetLength();
#endif

	CT2CA htmlStr(strText, CP_UTF8);

	htmlFile.Write(htmlStr, ::strlen(htmlStr));

#ifdef _DEBUG
	//int iLength = ::strlen(htmlStr);
	//CString strWrite(htmlStr);
	//CString str;
	//str.Format(_T("Wrote: %s\r\nText Length: %d , Wrote Length: %d"), 
	//	strWrite, iOldLen, iLength);
	//MyWriteConsole(str);
#endif

	htmlFile.Close();

	return TRUE;
}

CString CNurseController::CovertVarStr(CALLPATIENTINFO& cpiCall, 
							   CoverTypeEnum nType)
{
	CString strDest;
	switch(nType)
	{
	case covtSoundCall:
		strDest = CovertVarStr2(cpiCall.sDoctorBasicInfo.strSoundCall, cpiCall);
		break;
	case covtSoundWait:
		strDest = CovertVarStr2(cpiCall.sDoctorBasicInfo.strSoundWait, cpiCall);
		break;
	case covtDisplayCall:
		strDest = CovertVarStr2(cpiCall.sDoctorBasicInfo.strDisplayCall, cpiCall);
		break;
	case covtDisplayWait:
		strDest = CovertVarStr2(cpiCall.sDoctorBasicInfo.strDisplayWait, cpiCall);
		break;
	}
	return strDest;
}

CString CNurseController::CovertVarStr2(CString strSrc, 
								CALLPATIENTINFO& cpiCall)
{
	CString strDest;
	if(strSrc.IsEmpty())
	{
		return strSrc;
	}
	strDest = MyString::Replace(
		strSrc, STRING_QUEUEID_CALL, cpiCall.strQueueID);
	strDest = MyString::Replace(
		strDest, STRING_PATIENTNAME_CALL, cpiCall.strPatientName);
	strDest = MyString::Replace(
		strDest, STRING_DOCTORNAME, cpiCall.sDoctorBasicInfo.strDoctorName);
	strDest = MyString::Replace(
		strDest, STRING_OFFICENAME, cpiCall.sDoctorBasicInfo.strOfficeName);
	strDest = MyString::Replace(
		strDest, STRING_CONSULTNAME, cpiCall.sDoctorBasicInfo.strConsultName);
	strDest = MyString::Replace(
		strDest, STRING_PATIENT_GENDER, GetGenderStr(cpiCall.eGender));
	strDest = MyString::Replace(
		strDest, STRING_PATIENT_AGE, GetAgeStr(cpiCall.strBirth, TRUE));//该行代码GetAgeStr()可能产生异常
	return strDest;
}

void CNurseController::LoadDisplayConf()
{
	CString strStbKeshiPath = m_strModuleDir + _T("sys\\stb_keshi.ini");

	m_strHtmlTitleBegin = _T("<!DOCTYPE HTML><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">body{text-align: left; color: #ffffcc; float:left; font:  bold 80px/80px Verdana, Geneva, sans-serif;}#center{ margin-left: auto; margin-right: auto; vertical-align: middle; }</style><title>医院标题</title></head><body><div style=\"textalign:left\"><img src=\"../img/hspt.jpg\" width=\"80\" height=\"80\" style=\"float:left\">");
	m_strHtmlTitleBegin = MyCommon::GetProfileString(
		_T("sys"),_T("TITLEBEGIN"), m_strHtmlTitleBegin, strStbKeshiPath);
	m_strHtmlTitleEnd = _T("</div></body></html>");
	m_strHtmlTitleEnd = MyCommon::GetProfileString(
		_T("sys"),_T("TITLEEND"), m_strHtmlTitleEnd, strStbKeshiPath);
	m_strHtmlNoticeBegin = _T("<!DOCTYPE HTML><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">body{text-align: center; color: #ff0000; font:  bold 50px/50px Verdana, Geneva, sans-serif;}#center{ margin-left: auto; margin-right: auto; vertical-align: middle; }</style><title>通知</title></head><body><div><marquee scrollamount=\"5\">");
	m_strHtmlNoticeBegin = MyCommon::GetProfileString(
		_T("sys"),_T("NOTICEBEGIN"), m_strHtmlNoticeBegin, strStbKeshiPath);
	m_strHtmlNoticeEnd = _T("</marquee></div></body></html>");
	m_strHtmlNoticeEnd = MyCommon::GetProfileString(
		_T("sys"),_T("NOTICEEND"), m_strHtmlNoticeEnd, strStbKeshiPath);
	m_strHtmlInstMsgBegin = _T("<!DOCTYPE HTML><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">body{text-align: center; color: #ff0000; font:bold 100px/110px Verdana, Geneva, sans-serif;}#center{ margin-left: auto; margin-right: auto; vertical-align: middle; }#customers{font-family:\"Trebuchet MS\", Arial, Helvetica, sans-serif;width:100%%;border-collapse:collapse;}</style><title>呼叫信息</title></head><body><br/><div style=\"width: 1200px; margin-left: 0%%; margin-right: auto; margin-top: 0px;\">");
	m_strHtmlInstMsgBegin = MyCommon::GetProfileString(
		_T("sys"),_T("INSTMSGBEGIN"), m_strHtmlInstMsgBegin, strStbKeshiPath);
	m_strHtmlInstMsgEnd = _T("</div></body></html>");
	m_strHtmlInstMsgEnd = MyCommon::GetProfileString(
		_T("sys"),_T("INSTMSGEND"), m_strHtmlInstMsgEnd, strStbKeshiPath);
	m_strHtmlQueueInfoBegin = _T("<!DOCTYPE HTML><html><head><meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\" /><style type=\"text/css\">body{text-align: center; color: #000000; font:bold 50px/50px Verdana, Geneva, sans-serif;}#center{ margin-left: auto; margin-right: auto; vertical-align: middle; }#customers{font-family:\"Trebuchet MS\", Arial, Helvetica, sans-serif;width:100%;border-collapse:collapse;}#customers td, #customers th {font-size:1em;border:1px solid #98bf21;padding:3px 7px 2px 7px;}#customers th {font-size:1.1em;padding-top:5px;padding-bottom:4px;color:#008B00;}#customers tr.ord td {color:#000000;}#customers tr.alt td {color:#000000;}</style><title>排队信息</title></head><body><div style=\"width: 1280px; margin-left: 0%; margin-right: auto; margin-top: 15px;\"><div><table id=\"customers\"; width=\"1280px\"; border=\"1\" cellpadding=\"0\" cellspacing=\"0\">");
	m_strHtmlQueueInfoBegin = MyCommon::GetProfileString(
		_T("sys"),_T("QUEUEINFOBEGIN"), m_strHtmlQueueInfoBegin, strStbKeshiPath);
	m_strHtmlQueueInfoEnd = _T("</table></div></div></body></html>");
	m_strHtmlQueueInfoEnd = MyCommon::GetProfileString(
		_T("sys"),_T("QUEUEINFOEND"), m_strHtmlQueueInfoEnd, strStbKeshiPath);
	//m_strHtmlTitle = MyCommon::GetProfileString(
	//	_T("sys"),_T("TITLE"), _T("XXX医院XX科"), strStbPath);
	m_iHtmlPageItemCount = MyCommon::GetProfileInt(
		_T("sys"),_T("PAGE_ITEM_COUNT"), 8, strStbKeshiPath);
	m_iHtmlPageSwitchTime = MyCommon::GetProfileInt(
		_T("sys"),_T("PAGE_SWITCH_TIME"), 8, strStbKeshiPath);
	//m_iHtmlShowCallTime = MyCommon::GetProfileInt(
	//	_T("sys"),_T("SHOW_CALL_TIME"), 8, strStbKeshiPath);
	m_bShowQueueNum = MyCommon::GetProfileInt(
		_T("sys"), _T("SHOW_QUEUE_NUM"), 1, strStbKeshiPath);
	m_bSwitchPage = MyCommon::GetProfileInt(
		_T("sys"), _T("SWITCH_PAGE"), 1, strStbKeshiPath);
	m_nPageColCount = MyCommon::GetProfileInt(
		_T("sys"), _T("PAGE_COL_COUNT"), 3, strStbKeshiPath);
	m_bShowLineNum = MyCommon::GetProfileInt(
		_T("sys"), _T("SHOW_LINE_NUM"), 1, strStbKeshiPath);
	m_strColorCall = MyCommon::GetProfileString(
		_T("sys"),_T("COLOR_CALL"), _T("#FF0000"), strStbKeshiPath);
	m_strColorWait = MyCommon::GetProfileString(
		_T("sys"),_T("COLOR_WAIT"), _T("#EEB422"), strStbKeshiPath);
	m_strColorInLine = MyCommon::GetProfileString(
		_T("sys"),_T("COLOR_INLINE"), _T("#000000"), strStbKeshiPath);
	m_strColorHead = MyCommon::GetProfileString(
		_T("sys"),_T("COLOR_HEAD"), _T("#008B00"), strStbKeshiPath);
	m_strColorLineNum = MyCommon::GetProfileString(
		_T("sys"),_T("COLOR_LINENUM"), _T("#707070"), strStbKeshiPath);
	m_bShowSum = MyCommon::GetProfileInt(
		_T("sys"), _T("SHOW_SUM"), 1, strStbKeshiPath);
	m_nShowType = MyCommon::GetProfileInt(
		_T("sys"), _T("SHOW_TYPE"), 0, strStbKeshiPath);

	m_bShowOffice = MyCommon::GetProfileInt(
		_T("sys"), _T("SHOW_OFFICE"), 1, strStbKeshiPath);
	m_bShowDoctor = MyCommon::GetProfileInt(
		_T("sys"), _T("SHOW_DOCTOR"), 0, strStbKeshiPath);
	m_bShowConsult = MyCommon::GetProfileInt(
		_T("sys"), _T("SHOW_CONSULT"), 1, strStbKeshiPath);

	CString strStbZhenshiPath = m_strModuleDir + _T("sys\\stb_zhenshi.ini");

	m_strHospitalName = MyCommon::GetProfileString(
		_T("sys"), _T("CONSULT_STB_TITLE"), _T(""), strStbZhenshiPath);
	m_strSeparator = MyCommon::GetProfileString(
		_T("sys"), _T("CONSULT_WAIT_SEPARATOR"), _T(","), strStbZhenshiPath);
	m_bConsultShowNum = MyCommon::GetProfileInt(
		_T("sys"), _T("CONSULT_SHOW_QUEUE_NUM"), 1, strStbZhenshiPath);
}

BOOL CNurseController::InitDisplay()
{
	LoadDisplayConf();

	MyCommon::CreatePath(m_strWebRootDir);
	MyCommon::CreatePath(m_strWebRootDir + _T("photoTemp"));
	MyCommon::CreatePath(m_strWebRootDir + _T("img"));
	MyCommon::CreateFile(m_strWebRootDir + _T("img/hspt.jpg"));
	MyCommon::CreatePath(m_strWebRootDir + _T("zhenshi"));

	//读取诊室液晶模版
	CStdioFile file;
	CString strFilePath = m_strModuleDir + _T("zhenshi.html");

	::CopyFile(m_strModuleDir + _T("/zhenshi/zhenshi.html"), 
			strFilePath, TRUE);
	::CopyFile(m_strModuleDir + _T("/zhenshi/bkgd.jpg"), 
		m_strWebRootDir + _T("zhenshi/bkgd.jpg"), TRUE);
	::CopyFile(m_strModuleDir + _T("/zhenshi/main.css"), 
		m_strWebRootDir + _T("zhenshi/main.css"), TRUE);


	m_strConsultHtml = _T("");
	if(file.Open(strFilePath, CFile::modeRead|CFile::typeText))
	{
		CString strRead = _T("");
		for(; file.ReadString(strRead); strRead.Empty())
		{
			m_strConsultHtml += strRead + _T("\n");
		}
		file.Close();
	}
	//AfxMessageBox(m_strConsultHtml);
	//更新文件fdfile.xml
	strFilePath = m_strModuleDir + _T("fdfile_tp.xml");
	CString strFileStr = _T("");
	if(file.Open(strFilePath, CFile::modeRead|CFile::typeText))
	{
		CString strRead = _T("");
		for(; file.ReadString(strRead); strRead.Empty())
		{
			strFileStr += strRead + _T("\n");
		}
		file.Close();
	}
	else
	{
		AfxMessageBox(_T("fdfile_tp.xml文件缺失！"));
	}
	strFileStr = MyString::Replace(
		strFileStr, _T("%webRoot%"), m_strWebRootDir);

	strFilePath = m_strModuleDir + _T("fdfile.xml");
	if(file.Open(strFilePath, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		file.WriteString(strFileStr);
		file.Close();
	}

	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		CString strSql;

		//strSql = _T("select stb_id, stb_init_text from SetTopBox where 1=1;");
		//try
		//{
		//	if(rset.Open(strSql, CADORecordset::openQuery))
		//	{
		//		if(!rset.IsBOF())
		//		{
		//			for(int i = 0; !rset.IsEOF(); i++, rset.MoveNext())
		//			{
		//				CString strStbId, strInitText;
		//				rset.GetFieldValue(0, strStbId);
		//				m_arrstrStbIdList.Add(strStbId);
		//				rset.GetFieldValue(1, strInitText);
		//				//StbUpdateInstantMsg(strInitText, strStbId);
		//				WriteToStbHtmlFile(strInitText, strStbId, _T("webinfo.html"));
		//				//StbUpdateTitleHtml(m_strHtmlTitle, strStbId);
		//				//if(!StbNoticeHtmlExist(strStbId))
		//				//{
		//				//	StbUpdateNoticeHtml(_T(""), strStbId);
		//				//}
		//			}
		//		}
		//		rset.Close();
		//	}

		//}
		//catch (_com_error& e)
		//{
		//	WriteLog::WriteDbErrLog(_T("CNurseController::InitDisplay"));
		//}
		//catch (...)
		//{
		//}

		strSql.Format(_T("select office_id, call_name, stb_id from Office where office_id in (select office_id from Nurse_Office where nurse_id='%s') order by call_name"), GetUserId());


		if(rset.Open(strSql, CADORecordset::openQuery))
		{
			if(!rset.IsBOF())
			{
				for(; !rset.IsEOF(); rset.MoveNext())
				{
					CString strOfficeId, strOfficeName, strStbId;
					rset.GetFieldValue(_T("office_id"), strOfficeId);
					rset.GetFieldValue(_T("call_name"), strOfficeName);
					rset.GetFieldValue(_T("stb_id"), strStbId);
					Office office;
					office.SetOfficeId(strOfficeId);
					office.SetOfficeName(strOfficeName);
					office.SetStbId(strStbId);
					//m_arrOffice.Add(office);
					m_mapOffice[strOfficeId] = office;

					UpdateQueListData(strOfficeId);
				}
			}

			rset.Close();
		}
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::InitDisplay"));
	}
	catch (...)
	{
	}

	for(int i = 0; i < m_arrstrStbIdList.GetCount(); i++)
	{
		CString strPath = m_strWebRootDir + m_arrstrStbIdList[i];
		MyCommon::CreatePath(strPath);
	}


	POSITION pos = m_mapOffice.GetStartPosition();
	CString strOfficeId;
	Office office;
	for(;pos;)
	{
		m_mapOffice.GetNextAssoc(pos, strOfficeId, office);
		CString strStbId = office.GetStbId();
		if(!strStbId.IsEmpty())
		{
			StbData stbData;
			if(m_mapStb.Lookup(strStbId, stbData))
			{		
				m_mapStb[strStbId].arrOffice.Add(office.GetOfficeId());
			}
			else
			{
				stbData.strStbId = strStbId;
				stbData.arrOffice.Add(office.GetOfficeId());
				m_mapStb[strStbId] = stbData;
			}
		}
	}

	return TRUE;
}

BOOL CNurseController::BeginDisplayThread()
{
	m_pUpdateQueListThread = AfxBeginThread(UpdateQueListThreadProc,this);
	m_pDisplayThread = AfxBeginThread(DisplayThreadProc, this);
	return m_pDisplayThread == NULL ? TRUE : FALSE;
}

void CNurseController::KillDisplayThread()
{
	if(m_pDisplayThread)
	{
		m_bDisplayThread = FALSE;

		DWORD dwRet = ::WaitForSingleObject(m_pDisplayThread->m_hThread, 20);

		if(dwRet == WAIT_OBJECT_0)
		{
			//AfxMessageBox(_T("科室液晶队列显示线程安全退出"));
		}
		//else if(dwRet == WAIT_OBJECT_0 + 1)
		//{
		//	MSG msg;
		//	if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		//	{
		//		::DispatchMessage(&msg);
		//	}
		//}
		else
		{
			::TerminateThread(m_pDisplayThread->m_hThread, 0);
		}
		//else if(dwRet == WAIT_FAILED)
		//{
		//	::TerminateThread(m_pDisplayThread->m_hThread, 1);
		//	AfxMessageBox(_T("科室液晶队列显示线程已于等待前结束"));
		//}
		//else if(dwRet == WAIT_TIMEOUT)
		//{
		//	::TerminateThread(m_pDisplayThread->m_hThread, 1);
		//	AfxMessageBox(_T("科室液晶队列显示线程等待超时"));
		//}

		//if(m_pevtKillDisplayThread->Lock(50))
		//{
		//	//AfxMessageBox(_T("科室液晶队列显示线程安全退出"));
		//}

		//if(!m_mutexThreadDisplay.Lock(500))
		//{
		//	m_pDisplayThread->ExitInstance();
		//	Sleep(20);
		//}
		//else
		//{
		//	//AfxMessageBox(_T("科室液晶队列显示线程安全退出"));
		//}
		delete m_pDisplayThread;
		m_pDisplayThread = NULL;
	}
	if(m_pUpdateQueListThread)
	{
		m_bUpdateQueListThread = FALSE;
		DWORD dwRet = ::WaitForSingleObject(m_pUpdateQueListThread->m_hThread, 20);
		if(dwRet != WAIT_OBJECT_0)
		{
			::TerminateThread(m_pUpdateQueListThread->m_hThread, 0);
		}
		delete m_pUpdateQueListThread;
		m_pUpdateQueListThread = NULL;
	}
}

UINT CNurseController::DisplayThreadProc(LPVOID pParam)
{
	CNurseController* pThis = (CNurseController*)pParam;
	pThis->m_bDisplayThread = TRUE;
	pThis->m_bSleepDisplayThread = FALSE;

	while(pThis->m_bDisplayThread)
	{
		CMap<CString, LPCTSTR, StbData, StbData&>::CPair* pPair = NULL;
		pPair = pThis->m_mapStb.PGetFirstAssoc();
		//POSITION pos = pThis->m_mapStb.GetStartPosition();
		//CString strStbId;
		//StbData stbData;
		for(;pPair;pPair = pThis->m_mapStb.PGetNextAssoc(pPair))
		{
			//pThis->m_mapStb.GetNextAssoc(pos, strStbId, stbData);
			if(pPair->key != _T("0"))
			{
				if(pPair->value.nShowMsg <= 0)
				{
					if(pPair->value.nSwitch <= 0)
					{
						pThis->StbUpdateQueueInfo(pPair->key);
						pThis->StbUpdateQueueInfoHtml(pPair->key);
						pPair->value.nSwitch = pThis->m_iHtmlPageSwitchTime;
					}
				}

				//if(pPair->value.nShowMsg > 0)
				//{
				//	--pPair->value.nShowMsg;
				//}

				if(pPair->value.nSwitch > 0)
				{
					--pPair->value.nSwitch;
				}
			}
		}

		Sleep(1000);
	}
	::ExitThread(0);
	return 0;
}
/*
void CNurseController::OnResumeDisplay(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	//KillTimer(TIMERID_DISPLAY_CALL);
	//m_ptimerDisplayCall->KillTimer();
	//::KillTimer(NULL, m_idEventResumeQue);
	//if(m_pDisplayThread)
	//{
	//	for(;m_pDisplayThread->ResumeThread(););
	//}
}
*/
void CNurseController::UpdateDocView(LPVIEWUPDATEINFO pViewUpdateInfo, DoctorStatusEnum eUS)
{
	USERSTATUS us;
	us.strUserID = pViewUpdateInfo->strDoctorID;
	us.strUserName = pViewUpdateInfo->strDoctorName;
	us.strOfficeID = pViewUpdateInfo->strOfficeID;
	us.strPatientName = pViewUpdateInfo->strPatientName;
	us.eUserStatus = eUS;
	((CNurseStationApp*)AfxGetApp())->m_pDocInfoView->FlushDoctorData(us);
}

CString CNurseController::GetGenderStr(int iGender)
{
	if(iGender == genderFemale)
	{
		return _T("女");
	}
	else if(iGender == genderMale)
	{
		return _T("男");
	}
	else
	{
		return _T("");
	}
}


CString CNurseController::GetAgeStr(CString strBirth, BOOL bShowMonth)
{
	CString strPatientAge = _T("0");
	if(strBirth.IsEmpty()) return strPatientAge;
	CTime tNow = CTime::GetCurrentTime();
	COleDateTime odt;
	try
	{
		odt.ParseDateTime(strBirth);//该行可能产生异常
	}
	catch(CException* e)
	{
		return strPatientAge;
	}
	odt.Format(_T("%Y-%m-%d"));
	SYSTEMTIME st;
	odt.GetAsSystemTime(st);
	CTime tBirth(st);
#ifdef _DEBUG
	//CString str = tBirth.Format(_T("%Y-%m-%d"));
	//AfxMessageBox(str);
#endif
	int nYear = tNow.GetYear() - tBirth.GetYear();
	int nMonth = tNow.GetMonth() - tBirth.GetMonth();
	int nDay = tNow.GetDay() - tBirth.GetDay();

	if(nDay < 0)
	{
		--nMonth;
	}
	if(nMonth < 0)
	{
		--nYear;
		nMonth += 12;
	}
	if(nYear < 0) return strPatientAge;
	if(nYear == 0 && nMonth == 0)
	{
		CTimeSpan ts = tNow - tBirth;
		nDay = ts.GetDays();
		if(nDay < 0) return strPatientAge;
		strPatientAge.Format(_T("%d天"), nDay);
	}
	else
	{
		if(nYear > 0)
		{
			strPatientAge.Format(_T("%d岁"), nYear);
		}
		if(bShowMonth && nMonth > 0 || nYear < 1)
		{
			strPatientAge.AppendFormat(_T("%d月"), nMonth);
		}
	}

#ifdef _DEBUG
	//AfxMessageBox(strPatientAge);
#endif

	return strPatientAge;
}

BOOL CNurseController::InitNetServer()
{
	//获取系统版本信息，windows XP 及以下版本则启动CassiniDev.exe；否则不启动。
	OSVERSIONINFOEX osinfoex;
	osinfoex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(!GetVersionEx((LPOSVERSIONINFO)&osinfoex))
	{
		return FALSE;
	}
	//CString strVersion;
	//strVersion.Format(_T("系统版本：%d.%d"), osinfoex.dwMajorVersion, osinfoex.dwMinorVersion);
	//AfxMessageBox(strVersion);
	if(osinfoex.dwMajorVersion > 5)//Windows VISTA 以上版本
	{
		return FALSE;
	}

	HWND hWndNetServer = ::FindWindow(NULL, _T("多媒体信息发布服务端"));
	if(!hWndNetServer)
	{
		//SHELLEXECUTEINFO ShExecInfo = {0};
		//ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		//ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		//ShExecInfo.hwnd = NULL;
		//ShExecInfo.lpVerb = NULL;
		//ShExecInfo.lpFile =_T("CassiniDev.exe");
		//ShExecInfo.lpParameters = _T("");
		//ShExecInfo.lpDirectory = MyCommon::GetModuleDir();
		//ShExecInfo.nShow = SW_SHOW;
		//ShExecInfo.hInstApp = NULL;
		//ShellExecuteEx(&ShExecInfo);
		////::WaitForSingleObject(ShExecInfo.hProcess,INFINITE);

		//ShellExecute(NULL,_T("open"),MyCommon::GetModuleDir() + _T("CassiniDev.exe"),NULL,NULL, SW_SHOW );

		STARTUPINFO si = {0};
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		memset(&m_piNetServer, 0, sizeof(PROCESS_INFORMATION));

		CString strCmdLine;
		//strCmdLine.Format(_T("\"%sCassiniDev.exe\""), MyCommon::GetModuleDir());
		strCmdLine = _T("CassiniDev.exe");

		//if(!::CreateProcess(NULL, strCmdLine.GetBuffer(), NULL, NULL, TRUE, NULL, NULL, MyCommon::GetModuleDir(), &si, &m_piNetServer))
		if(!::CreateProcess(NULL, strCmdLine.GetBuffer(), NULL, NULL, FALSE, NULL, NULL, m_strModuleDir, &si, &m_piNetServer))
		{
			AfxMessageBox(_T("液晶显示服务端无法启动，请手动启动!"));
			return FALSE;
		}
		//else
		//{
		//	::CloseHandle(m_piNetServer.hProcess);
		//	::CloseHandle(m_piNetServer.hThread);
		//}
	}

	return TRUE;
}

void CNurseController::ReLoadConf()
{
	LoadDisplayConf();
	SetDataDays(MyCommon::GetProfileInt(
		_T("sys"), _T("DATA_DAYS"), 0, _T("\\SYS\\sysconfig.ini")));
	m_bReadInPatient = MyCommon::GetProfileInt(
		_T("sys"), _T("READ_INPATIENT"), 0, _T("\\SYS\\sysconfig.ini"));
	m_pJttsAPI->LoadConf();
	SetCallSeconds(MyCommon::GetProfileInt(
		_T("sys"), _T("CALL_SECONDS"), 0, _T("\\SYS\\sysconfig.ini")));
}

LPDOCTORBASICINFO CNurseController::GetDocBasInfo(CString strDoctorId)
{
	CMap<CString, LPCTSTR, DOCTORBASICINFO, DOCTORBASICINFO&>::CPair* pPair;
	pPair = m_mapDocIdDocInfo.PLookup(strDoctorId);
	if(pPair)
	{
		return &(pPair->value);
	}
	else
	{
		return NULL;
	}
}

CString CNurseController::CovertColorString(CString strSrc)
{
	int iIndex = strSrc.Find('#');
	if(iIndex != -1)
	{
		int iType;
		if(strSrc.GetAt(iIndex +1) == '0' && strSrc.GetAt(iIndex +2) == '#') 
		{
			iType = 0;
		}
		else if(strSrc.GetAt(iIndex +1) == '1' && strSrc.GetAt(iIndex +2) == '#')
		{
			iType = 1;
		}
		else if(strSrc.GetAt(iIndex +1) == '2' && strSrc.GetAt(iIndex +2) == '#')
		{
			iType = 2;
		}
		else
		{
			iType = -1;
		}

		int iIndex2;
		if(iIndex < strSrc.GetLength() - 3)
		{
			iIndex2 = strSrc.Find('#', iIndex + 3);
			if(iIndex2 == -1)
			{
				iIndex2 = strSrc.GetLength() -1;
			}
		}
		else
		{
			iIndex2 = strSrc.GetLength() -1;
		}
		CString strLeft = strSrc.Left(iIndex);
		CString strMid = strSrc.Mid(iIndex + 3, iIndex2 - (iIndex + 3));
		CString strRight = strSrc.Mid(iIndex2);

		CString strColor;

		switch(iType)
		{
		case 0:
			{
				strColor = _T("<font color='Red'>");
			}
			break;
		case 1:
			{
				strColor = _T("<font color='Green'>");
			}
			break;
		case 2:
			{
				strColor = _T("<font color='Yellow'>");
			}
			break;
		default:
			{
				return strSrc;
			}
			break;
		}

		strSrc = strLeft + strColor + strMid + _T("</font>") + strRight;

		return CovertColorString(strSrc);
	}
	else
	{
		return strSrc;
	}
}

void CNurseController::SetDataDays(int nDays)
{
	if(nDays < 0)
	{
		nDays = 0;
	}
	m_DbParam.SetDataDays(nDays);
	m_strSqlDate.Format(_T("regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime)")
		, GetDataDays());
}

void CNurseController::SetCallSeconds(int nSecond)
{
	if(nSecond < 0)
	{
		nSecond = 0;
	}
	//m_nTimeCall = nSecond;
	m_DbParam.SetCallSeconds(nSecond);
}

BOOL CNurseController::ResetStbShowMsgFlag(CString strStbId)
{
	if(strStbId.IsEmpty()) return FALSE;

	if(strStbId != _T("0"))
	{
		m_mapStb[strStbId].nShowMsg = 0;
	}
	else
	{
		CMap<CString, LPCTSTR, StbData, StbData&>::CPair* pPair = NULL;
		pPair = m_mapStb.PGetFirstAssoc();
		for(;pPair;pPair = m_mapStb.PGetNextAssoc(pPair))
		{
			pPair->value.nShowMsg = 0;
		}
	}

	return TRUE;
}

BOOL CNurseController::UpdateConsultStb(CONSULTSTB& consultStb)
{
	CString strHtml = m_strConsultHtml;
	strHtml = MyString::Replace(
		strHtml, _T("%CONSULTNAME%"), consultStb.strConsultName);
	strHtml = MyString::Replace(
		strHtml, _T("%DOCTORNAME%"), consultStb.strDoctorName);
	strHtml = MyString::Replace(
		strHtml, _T("%DEPARTNAME%"), consultStb.strDepartName);
	strHtml = MyString::Replace(
		strHtml, _T("%DOCTORTITLE%"), consultStb.strDoctorTitle);
	strHtml = MyString::Replace(
		strHtml, _T("%CALLPATIENT%"), consultStb.strCallPatient);
	strHtml = MyString::Replace(
		strHtml, _T("%WAITPATIENT%"), consultStb.strWaitPatient);
	strHtml = MyString::Replace(
		strHtml, _T("%DOCTORIMG%"), 
		_T("../photoTemp/") + consultStb.strDoctorId + _T(".jpg"));
	strHtml = MyString::Replace(
		strHtml, _T("%TITLE%"), m_strHospitalName);

	//AfxMessageBox(strHtml);
	CString strPath = m_strWebRootDir + consultStb.strStbId;
	if(!MyCommon::PathFileExist(strPath))
	{
		MyCommon::CreatePath(strPath);
	}

	return WriteToFile(strHtml, strPath + _T("/webinfo.html"));
}

CString CNurseController::SqlQueryWaitPatient(CString& strDoctorID)
{
	CString strPatientName = _T("");
	CString strSql;
	strSql.Format(_T("select Q.log_id SerialID, Q.queue_id_call QueueNum,Q.patient_name PatientName, Q.office_id, D.name DoctorName from Queue Q left join Doctor D on Q.doctor_id=D.doctor_id where Q.status=%d and Q.doctor_id='%s' and %s order by Q.regtime;"),
		qsPrepare, strDoctorID, m_strSqlDate);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSql, CADORecordset::openQuery);
		if(!rset.IsBOF())
		{
			for(; !rset.IsEOF(); rset.MoveNext())
			{
				CString strValue;
				rset.GetFieldValue(_T("PatientName"), strValue);
				strPatientName += strValue;
				if(m_bConsultShowNum)
				{
					rset.GetFieldValue(_T("QueueNum"), strValue);
					strPatientName += _T("(") + strValue + _T(")");
				}
				strPatientName += m_strSeparator;
			}
			strPatientName = strPatientName.Left(
				strPatientName.GetLength() - m_strSeparator.GetLength());
		}

		rset.Close();
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlQueryCallingPatient"));
	}
	catch (...)
	{
	}

	return strPatientName;
}

CString CNurseController::SqlQueryCallPatient(CString& strDoctorID)
{
	CString strPatientName = _T("");
	CString strSql;
	strSql.Format(_T("select Q.log_id SerialID, Q.queue_id_call QueueNum,Q.patient_name PatientName, Q.office_id, D.name DoctorName from Queue Q left join Doctor D on Q.doctor_id=D.doctor_id where Q.status=%d and Q.doctor_id='%s' and %s;"),
		qsDoing, strDoctorID, m_strSqlDate);
	try
	{
		CADODatabase db;
		db.Open(m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSql, CADORecordset::openQuery);
		if(!rset.IsBOF())
		{
			rset.GetFieldValue(_T("PatientName"), strPatientName);
			if(m_bConsultShowNum)
			{
				CString strQueueNum;
				rset.GetFieldValue(_T("QueueNum"), strQueueNum);
				strPatientName += _T("(") + strQueueNum + _T(")");
			}
		}
		rset.Close();
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::SqlQueryCallingPatient"));
	}
	catch (...)
	{
	}

	return strPatientName;
}

BOOL CNurseController::DeleteDbData()
{
	BOOL bRet = TRUE;

	CADODatabase db;
	db.SetConnectionString(m_strSqlConn);
	try
	{
		db.Open();
		CString strSql;
		strSql.Format(_T("delete from Queue where regtime<cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) and office_id in(select office_id from Nurse_Office where Nurse_id='%s')"),
			GetDataDays(), GetUserId());
		db.Execute(strSql);
	}
	catch (_com_error& e)
	{
		bRet = FALSE;
		WriteLog::WriteDbErrLog(_T("CNurseController::DeleteDbData()"));
	}

	return bRet;
}

BOOL CNurseController::SendToThroughLED(int nId, CString strMsg)
{
	if(nId < 1) return FALSE;
	CString strIP = _T("");
	int uPort = 0;
	int nAddress = 0;
	int nChannelNum = 0;

	CADODatabase db;
	db.SetConnectionString(m_strSqlConn);
	try
	{
		db.Open();
		CString strSql;
		strSql.Format(_T("select ip,port,address,channel_num from ThroughLED where serial_id=%d"),nId);
		CADORecordset rset(&db);
		rset.Open(strSql, CADORecordset::openQuery);
		if(!rset.IsBOF())
		{
			rset.GetFieldValue(_T("ip"), strIP);
			rset.GetFieldValue(_T("port"), uPort);
			rset.GetFieldValue(_T("address"), nAddress);
			rset.GetFieldValue(_T("channel_num"), nChannelNum);
		}
	}
	catch (_com_error& e)
	{
		return FALSE;
	}

	return CSendToHardWare::SendToHardWare(
		enumThroughScreen, nAddress, strMsg, strIP, uPort, nChannelNum);
}

CString CNurseController::GetFirstInLinePatientSelectString(CString strDoctorID, BOOL isExpert)
{
	CString strSql;
	//一个医生只处理一个科室
	////strSql.Format(_T("Select top 10 Q.log_id,Q.patient_id,Q.status,Q.doctor_id from Queue Q, (select doctor_id, office_id from Doctor where doctor_id='%s') D where Q.office_id=D.office_id and %s and status=%d and %s order by Q.priority desc, Q.regtime;")
	////	,strDoctorID, isExpert ? _T("Q.doctor_id=D.doctor_id") : _T("(Q.doctor_id is null or Q.doctor_id='' or Q.doctor_id=D.doctor_id)"), qsInLine, m_strSqlDate);

	strSql.Format(_T("Select top 10 log_id,patient_id,status,doctor_id from Queue where office_id in (select office_id from Doctor where doctor_id='%s') and %s and status=%d and %s order by priority desc, regtime;"),
		strDoctorID,
		isExpert ? _T("doctor_id='")+strDoctorID+_T("'") : _T("(doctor_id is null or doctor_id='' or doctor_id='")+strDoctorID+_T("')"), 
		qsInLine,
		m_strSqlDate);

	//一个医生可处理多个科室
	////CString strDoctorStr;
	////if(isExpert)
	////{
	////	strDoctorStr.Format(_T("doctor_id='%s'"), strDoctorID);
	////}
	////else
	////{
	////	strDoctorStr.Format(_T("(doctor_id is null or doctor_id='' or doctor_id='%s')"), strDoctorID);
	////}
	////strSql.Format(_T("Select top 10 log_id,patient_id,status,doctor_id from Queue where office_id in (select office_id from Doctor_Office where doctor_id='%s') and %s and status=%d and %s order by priority desc, regtime;")
	////	,strDoctorID ,strDoctorStr, qsInLine, m_strSqlDate);

	//strSql.Format(_T("Select top 10 log_id,patient_id,status,doctor_id from Queue where office_id in (select office_id from Doctor_Office where doctor_id='%s') and %s and status=%d and %s order by priority desc, regtime;"),
	//	strDoctorID,
	//	isExpert ? _T("doctor_id='")+strDoctorID+_T("'") : _T("(doctor_id is null or doctor_id='' or doctor_id='")+strDoctorID+_T("')"), 
	//	qsInLine,
	//	m_strSqlDate);

	//AfxMessageBox(strSql);
	//WriteLog::Write(strSql);

	return strSql;
}

BOOL CNurseController::SendCmd(const int nCallerId, 
							   const DoctorCmdEnum eDocCmd)
{
	if(nCallerId < 1) return FALSE;
	CString strDoctorId;
	CADODatabase db;
	db.SetConnectionString(m_strSqlConn);
	CADORecordset rset(&db);
	CString strSel;
	strSel.Format(_T("select doctor_id from Doctor where caller_id=%d;"), nCallerId);
	try
	{
		db.Open();
		rset.Open(strSel, CADORecordset::openQuery);
		if(rset.IsBOF()) return FALSE;
		rset.GetFieldValue(0, strDoctorId);
	}
	catch (_com_error&)
	{
		return FALSE;
	}
	return SendCmd(strDoctorId, _T(""), eDocCmd);
}

void CNurseController::OnRecvCmdResult(LPCMDRESULT pCmdResult, LPSOCKADDR_IN psockddr)
{
	CString strDoctorId(pCmdResult->sUserInfoFrom.wszUserID);
	LPDOCTORBASICINFO pDocInfo = GetDocBasInfo(strDoctorId);
	if(pDocInfo)
	{
		DOCTORBASICINFO dinfo;
		SqlQueryDoctorBaseInfo(dinfo, strDoctorId);
		m_mapDocIdDocInfo[strDoctorId] = dinfo;
		pDocInfo = GetDocBasInfo(strDoctorId);
	}
	if(!pDocInfo) return;
	BOOL bSuc = pCmdResult->bSucceed;
	CString strCount = _T("0");
	CString strCurNum = _T("");
	if(bSuc)
	{
		CADODatabase db;
		db.SetConnectionString(m_strSqlConn);
		CADORecordset rset(&db);
		CString strSel;
		//Select count(Q.log_id) LeftCount from Queue Q, (select doctor_id, office_id from Doctor where doctor_id='%s') D where Q.office_id=D.office_id and %s and status in (%d,%d,%d) and
		//strSel.Format(_T("SELECT * FROM (Select count(log_id) LeftCount from Queue where doctor_id='%s' and status in (%d,%d,%d) and %s) Q1 LEFT JOIN (SELECT queue_id_call CurNum FROM Queue WHERE doctor_id='%s' and status=%d and %s) Q2 ON 1=1;"), 
		//	strDoctorId, qsDoing, qsPrepare, qsInLine, m_strSqlDate, strDoctorId, qsDoing, m_strSqlDate);
		CString strDoctorStr;



		if(pDocInfo->bExpertFlag)
		{
			strDoctorStr = _T("Q.doctor_id=D.doctor_id");
		}
		else
		{
			strDoctorStr = _T("(Q.doctor_id is null or Q.doctor_id='' or Q.doctor_id=D.doctor_id)");
		}
		strSel.Format(_T("SELECT * FROM (Select count(Q.log_id) LeftCount from Queue Q, (select doctor_id, office_id from Doctor where doctor_id='%s') D where Q.office_id=D.office_id and %s and status in (%d,%d) and %s) Q1 LEFT JOIN (SELECT queue_id_call CurNum FROM Queue WHERE doctor_id='%s' and status=%d and %s) Q2 ON 1=1;"), 
			strDoctorId,strDoctorStr,qsPrepare, qsInLine, m_strSqlDate, strDoctorId, qsDoing, m_strSqlDate);
		try
		{
			db.Open();
			rset.Open(strSel, CADORecordset::openQuery);
			if(rset.IsBOF())
			{
				bSuc = FALSE;
			}
			else
			{
				rset.GetFieldValue(_T("LeftCount"), strCount);
				rset.GetFieldValue(_T("CurNum"), strCurNum);
			}
			
			rset.Close();
		}
		catch (_com_error&)
		{
			bSuc = FALSE;
		}
	}
	if(bSuc)
	{
		CSendToHardWare::SendToHardWare(
			enumCallerScreen,pDocInfo->iCallerId,
			strCurNum + _T(" ") + strCount,
			_T(""),0,-1,callerCmdShowNum);
	}
	else
	{
		CSendToHardWare::SendToHardWare(
			enumCallerScreen,pDocInfo->iCallerId,
			_T("0"),
			_T(""),0,-1,callerCmdShowAlarm);
	}
}

