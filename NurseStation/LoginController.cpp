#include "StdAfx.h"
#include "LoginController.h"
#include "../NurseStation/MyCommon.h"
#include "../NurseStation/MyString.h"

CLoginController::CLoginController(void)
// : m_bCanLogin(TRUE)
{
	m_uNursePort = MyCommon::GetProfileInt(
		_T("sys"), _T("NURSE_PORT"), 4020, _T("SYS\\port.ini"));
}

CLoginController::~CLoginController(void)
{
}

BOOL CLoginController::OnStartControl()
{
	BOOL bRet = CController::OnStartControl();
	if(!bRet) return FALSE;

	//if(GetUserType() == utDoctor)
	//{
	//	m_uNursePort = MyCommon::GetProfileInt(
	//		_T("sys"), _T("NURSE_PORT"), 4020, _T("SYS\\port.ini"));
	//	m_strNurseIP = MyCommon::GetProfileString(
	//		_T("sys"), _T("NURSE_IP"), _T("192.168.1.100"), _T("SYS\\port.ini"));
	//}
	//else
	//{
	//	m_uNursePort = 4020;
	//}
	//m_bCanLogin = TRUE;
	m_timerSendVerify.StartTimer(
		this, 200, &CLoginController::OnSendVerify);
	//SendVerify();

	return bRet;
}

BOOL CLoginController::OnEndControl()
{
	m_timerSendVerify.KillTimer();

	return  CController::OnEndControl();
}

void CLoginController::OnSendVerify(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	SendVerify();
}

void CLoginController::SendVerify()
{

	if(GetUserType() == utDoctor)
	{
		m_uNursePort = MyCommon::GetProfileInt(
			_T("sys"), _T("NURSE_PORT"), 4020, _T("SYS\\port.ini"));
		m_strNurseIP = MyCommon::GetProfileString(
			_T("sys"), _T("NURSE_IP"), _T("192.168.1.100"), _T("SYS\\port.ini"));
	}
	else
	{
		m_uNursePort = 4020;
	}

	REQLOGIN login;
	ZeroMemory(&login, sizeof(login));
	SetSendBaseInfo((LPBASEINFO)&login, ntiReqLogin, sizeof(login));
	wcscpy_s(login.wszPassword, MAX_PASSWORD_LENGTH, m_strPassword.GetBuffer());

	if(GetUserType() == utDoctor)
	{
		//Broadcast((char*)&login, sizeof(login), m_uNursePort);
		int nLoginType = MyCommon::GetProfileInt(
			_T("sys"), _T("LOGIN_TYPE"), 0, _T("SYS\\port.ini"));
		if(nLoginType == 0)
		{
			Broadcast((char*)&login, sizeof(login), m_uNursePort);
		}
		else
		{
			SendTo((char*)&login, sizeof(login), m_strNurseIP, m_uNursePort);
		}
	}
	else
	{
		Broadcast((char*)&login, sizeof(login));
	}
}

void CLoginController::OnRecvCmdResult(LPCMDRESULT pCmdResult, LPSOCKADDR_IN psockddr)
{
	switch(pCmdResult->eTypeIDFrom)
	{
	case ntiReqLogin:
	//case ntiCmdResult:
		OnRecvLoginResult(pCmdResult, psockddr);
		break;
	default:
		break;
	}
}


