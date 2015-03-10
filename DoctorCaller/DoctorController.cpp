#include "StdAfx.h"
#include "DoctorController.h"
#include "DoctorCaller.h"
#include "DoctorCallerDlg.h"
#include "../NurseStation/WriteLog.h"

CDoctorController::CDoctorController(void* pParent)
: m_pParent(pParent)
, m_bCallSpecial(FALSE)
, m_bExpertFlag(FALSE)
, m_strNurseId(_T(""))
{
}

CDoctorController::~CDoctorController(void)
{
	//OnEndControl();
}

BOOL CDoctorController::OnStartControl()
{
	return CQueueController::OnStartControl();
}

BOOL CDoctorController::OnEndControl()
{
	return CQueueController::OnEndControl();
}

void CDoctorController::OnRecvCmdResult(LPCMDRESULT pCmdResult, LPSOCKADDR_IN psockddr)
{
	NetTypeIDEnum eNetType = pCmdResult->eTypeIDFrom;
	DoctorCmdEnum eDocCmd = pCmdResult->eDoctorCmd;
	m_DbParam.SetDbParam(&(pCmdResult->sDbParam));

	m_strConn = _T("Driver={") + m_DbParam.GetDriver() + _T("};Server=")
		+m_DbParam.GetHost()+_T(";Database=") + m_DbParam.GetDbName() 
		+ _T(";Uid=") + m_DbParam.GetUID() + _T(";Pwd=") 
		+ m_DbParam.GetPID()+_T(";");

	if(m_pParent)
	{
		((CDoctorCallerDlg*)m_pParent)->SetOptStatus(eDocCmd, pCmdResult->bSucceed);
	}

	try
	{
		CADODatabase db;
		db.SetConnectionString(m_strConn);
		db.Open();
		CADORecordset rset(&db);
		if(eNetType == ntiLogin)
		{//获取特呼权限和专家标志
			CString strSql;
			strSql.Format(_T("select D.office_id OfficeId,D.name DoctorName,D.call_special_flag CallSpecial,D.expert_flag ExpertFlag,O.name OfficeName from Doctor D,Office O where D.doctor_id='%s' and D.office_id=O.office_id;")
				, GetUserId());
			rset.Open(strSql, CADORecordset::openQuery);
			if(!rset.IsBOF())
			{
				rset.GetFieldValue(_T("CallSpecial"), m_bCallSpecial);
				rset.GetFieldValue(_T("ExpertFlag"), m_bExpertFlag);
			}
			rset.Close();
		}
		//刷新病人数据
		if(eNetType == ntiLogin 
			|| eNetType == ntiDoctorCmd && (
			eDocCmd == dcCmdCall || eDocCmd == dcCmdWait || eDocCmd == dcCmdDiscard))
		{
			CString strDoctorStr;

			//一个医生只处理一个科室
			if(m_bExpertFlag)
			{
				strDoctorStr = _T("Q.doctor_id=D.doctor_id");
			}
			else
			{
				strDoctorStr = _T("(Q.doctor_id is null or Q.doctor_id='' or Q.doctor_id=D.doctor_id)");
			}
			m_strSelect.Format(_T("Select Q.log_id,Q.queue_id_call,Q.patient_name,Q.patient_gender,Q.status from Queue Q, (select doctor_id, office_id from Doctor where doctor_id='%s') D where Q.office_id=D.office_id and %s and status in (%d,%d,%d) and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by Q.status,Q.priority desc,Q.regtime;")
				,GetUserId(), strDoctorStr, qsDoing, qsPrepare, qsInLine, m_DbParam.GetDataDays());
			m_strSelectDoneDiscard.Format(_T("Select Q.log_id,Q.queue_id_call,Q.patient_name,Q.patient_gender,Q.status from Queue Q, (select doctor_id, office_id from Doctor where doctor_id='%s') D where Q.office_id=D.office_id and status in (%d,%d) and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by Q.status desc,Q.priority desc,Q.regtime;")
				,GetUserId(), qsDone, qsDiscard, m_DbParam.GetDataDays());
			m_strSelectLeft.Format(_T("Select count(Q.log_id) LeftCount from Queue Q, (select doctor_id, office_id from Doctor where doctor_id='%s') D where Q.office_id=D.office_id and %s and status in (%d,%d,%d) and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime);")
				,GetUserId(), strDoctorStr, qsDoing, qsPrepare, qsInLine, m_DbParam.GetDataDays());

			//一个医生可处理多个科室
			//if(m_bExpertFlag)
			//{
			//	strDoctorStr.Format(_T("doctor_id='%s'"), GetUserId());
			//}
			//else
			//{
			//	strDoctorStr.Format(_T("(doctor_id is null or doctor_id='' or doctor_id='%s')"), GetUserId());
			//}
			//m_strSelect.Format(_T("Select log_id,queue_id_call,patient_name,patient_gender,status from Queue where office_id in (select office_id from Doctor_Office where doctor_id='%s') and %s and status in (%d,%d,%d) and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime)  order by status,priority desc,regtime;")
			//	,GetUserId(), strDoctorStr, qsDoing, qsPrepare, qsInLine, m_DbParam.GetDataDays());
			//m_strSelectDoneDiscard.Format(_T("Select log_id,queue_id_call,patient_name,patient_gender,status from Queue where office_id in (select office_id from Doctor_Office where doctor_id='%s') and status in (%d,%d) and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by status desc,priority desc,regtime;")
			//	,GetUserId(), qsDone, qsDiscard, m_DbParam.GetDataDays());
			//m_strSelectLeft.Format(_T("Select count(log_id) LeftCount from Queue where office_id in (select office_id from Doctor_Office where doctor_id='%s') and %s and status in (%d,%d,%d) and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime);")
			//	,GetUserId(), strDoctorStr, qsDoing, qsPrepare, qsInLine, m_DbParam.GetDataDays());

			if(m_pParent)
			{
				rset.Open(m_strSelect, CADORecordset::openQuery);
				int nLeft = ((CDoctorCallerDlg*)m_pParent)->OnShowPatients(rset);
				((CDoctorCallerDlg*)m_pParent)->SetTitle();
			}
		}
		db.Close();
	}
	catch (_com_error& e)
	{
		CString strErr;
		strErr.Format(_T("CDoctorController::OnRecvCmdResult: %s"), e.ErrorMessage());
		WriteLog::WriteDbErrLog(strErr);
	}
}

BOOL CDoctorController::SendCmd(DoctorCmdEnum eDoctorCmdEnum, CString strQueSerialId)
{
	((CDoctorCallerDlg*)m_pParent)->m_stOptStatus.SetWindowText(_T(""));
	DOCTORCMD docCmd;
	memset(&docCmd, 0, sizeof(docCmd));
	SetSendBaseInfo((LPBASEINFO)&docCmd, ntiDoctorCmd, sizeof(docCmd));
	docCmd.eDoctorCmd = eDoctorCmdEnum;
	wcscpy_s(docCmd.szQueSerialID, MAX_ID_LENGTH, strQueSerialId.GetBuffer());

	//CString strNursePort;
	//strNursePort.Format(_T("%s:%d"), NetToHostIP(m_sockNurse.sin_addr), NetToHostShort(m_sockNurse.sin_port));
	//AfxMessageBox(strNursePort);

	return SendTo((LPSTR)&docCmd, sizeof(docCmd), &m_sockNurse);
}

BOOL CDoctorController::SendQuit()
{
	return SendExitCode(m_nExitCode);
}
