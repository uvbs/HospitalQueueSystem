// Controller.cpp : 实现文件
//

#include "stdafx.h"
#include "Controller.h"
#include "MyCommon.h"
#include "MyString.h"
#include "MyPort.h"


// CController

CController::CController()
: m_uPort(0)
, m_strCtlUserId(_T(""))
, m_strCtlUserName(_T(""))
, m_eCtlUserType(utNotDefine)
, m_strCtlOffices(_T(""))
{
}

CController::~CController()
{
}

BOOL CController::StartControl()
{
	return OnStartControl();
}

BOOL CController::EndControl()
{
	return OnEndControl();
}

BOOL CController::OnStartControl()
{
	OnSetPort();
	return StartSocket(m_uPort);
	//return TRUE;
}

void CController::OnSetPort()
{
	MyPort myPort;
	m_uPort = myPort.GetLocalPort(MY_DEFAULT_LOCAL_PORT);
}

BOOL CController::OnEndControl()
{
	return CloseSocket();
}

void CController::SetPort(USHORT uPort)
{
	m_uPort = uPort;
}

void CController::DispatchCmd(LPUDPPACKEGE pUDPPackege,
							   LPSOCKADDR_IN psockaddr)
{
	switch( ((LPUDPPACKEGE)pUDPPackege)->header.eTypeID )
	{
	case ntiReqLogin:
		OnRecvReqLogin((LPREQLOGIN)pUDPPackege, psockaddr);
		break;
	//case ntiLoginVerify:
	//	OnRecvLoginResult((LPLOGINRESULT)pUDPPackege, psockaddr);
	//	break;
	case ntiLogin:
		OnRecvLogin((LPBROADCAST)pUDPPackege, psockaddr);
		break;
	case ntiQuit:
		OnRecvQuit((LPQUIT)pUDPPackege, psockaddr);
		break;
	case ntiHeartBeat:
		OnRecvHeartBeat((LPBROADCAST)pUDPPackege, psockaddr);
		break;
	case ntiDoctorCmd:
		OnRecvDoctorCmd((LPDOCTORCMD)pUDPPackege, psockaddr);
		break;
	case ntiCmdResult:
		OnRecvCmdResult((LPCMDRESULT)pUDPPackege, psockaddr);
		break;
	case ntiChatMsg:
		OnRecvChatMsg((LPCHATMSG)pUDPPackege, psockaddr);
		break;
	default:
		break;
	}
}

void CController::OnRecvData()
{
	CHAR szDataBuf[MAX_PACKEGEDATA_LENGTH] = {0};
	SOCKADDR_IN addrinClient;
	int iLen = sizeof(addrinClient);
	if(RecvFrom(szDataBuf, MAX_PACKEGEDATA_LENGTH, &addrinClient))
	{
		DispatchCmd((LPUDPPACKEGE)szDataBuf, &addrinClient);
	}
	else
	{
		Sleep(100);
	}
}


// CController 成员函数
