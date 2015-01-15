#include "StdAfx.h"
#include "QueueController.h"
#include "ChatDlg.h"
#include "MyCommon.h"
#include "MyString.h"
//#include "WriteLog.h"

CQueueController::CQueueController(void)
: m_pChatDlg(NULL)
, m_pSendHeartBeatThread(NULL)
, m_bChatShow(FALSE)
{
	m_uBroadPort = MyCommon::GetProfileInt(
		_T("sys"), _T("NURSE_PORT"), 4020, _T("SYS\\port.ini"));
}

CQueueController::~CQueueController(void)
{
}

BOOL CQueueController::OnStartControl()
{
	BOOL bRet =  CServiceController::OnStartControl();
	if(!bRet) return FALSE;

	m_pChatDlg = new CChatDlg(this);
	m_pChatDlg->Create(CChatDlg::IDD);
	m_pChatDlg->ShowWindow(SW_HIDE);

	SendLogin();
	//m_timerSendHeartBeat.StartTimer(
	//	this, 1000, &CQueueController::OnSendHeartBeat);

	if(!m_pSendHeartBeatThread)
	{
		m_pSendHeartBeatThread = AfxBeginThread(SendHeartBeatThreadProc, this);
	}

	return bRet;
}

BOOL CQueueController::OnEndControl()
{
	if(m_pChatDlg)
	{
		((CChatDlg*)m_pChatDlg)->DestroyWindow();
		delete m_pChatDlg;
		m_pChatDlg = NULL;
	}
	//m_timerSendHeartBeat.KillTimer();
	if(m_pSendHeartBeatThread)
	{
		::TerminateThread(m_pSendHeartBeatThread->m_hThread, 0);
	}
	SendQuit();

	return CServiceController::OnEndControl();
}

void CQueueController::ShowChat()
{
	if(m_pChatDlg)
	{
		//m_pChatDlg->ShowWindow(SW_SHOW);
		//m_pChatDlg->ShowWindow(SW_NORMAL);
		m_pChatDlg->ShowWindow(SW_SHOWNORMAL);
		m_pChatDlg->UpdateWindow();
		m_bChatShow = TRUE;
	}
}

void CQueueController::HideChat()
{
	if(m_pChatDlg)
	{
		m_pChatDlg->ShowWindow(SW_HIDE);
		//m_pChatDlg->UpdateWindow();
		m_bChatShow = FALSE;
	}
}

void CQueueController::ShowOrHideChat()
{
	if(!m_pChatDlg) return;

	//WINDOWPLACEMENT wpm;
	//ZeroMemory(&wpm, sizeof(wpm));
	//wpm.length = sizeof(wpm);
	//if(!m_pChatDlg->GetWindowPlacement(&wpm)) return;
	//if(wpm.showCmd != SW_HIDE)
	if(m_bChatShow)
	{
		HideChat();
	}
	else
	{
		ShowChat();
	}
}

void CQueueController::MoveChatControls()
{
	if(!m_pChatDlg) return;

	((CChatDlg*)m_pChatDlg)->MoveControls();
}

void CQueueController::OnRecvChatMsg(LPCHATMSG pChatMsg, LPSOCKADDR_IN psockddr)
{
	if(!m_pChatDlg) return;
	CUser* pUser = GetUser(pChatMsg->header.sUserInfo.wszUserID,
		pChatMsg->header.sUserInfo.eUserType);
	if(pUser)
	{
		((CChatDlg*)m_pChatDlg)->AddChat(
			pUser, pChatMsg->wszMsg, pChatMsg->eChatTypeID == ctPublicChat);
	}
}

void CQueueController::OnNurseLogin(CNurse* pNurse, BOOL bLogin)
{
	if(m_pChatDlg)
	{
		((CChatDlg*)m_pChatDlg)->AddUser(pNurse, bLogin);
	}

	//CServiceController::OnNurseLogin(pNurse, bLogin);
}

void CQueueController::OnNurseOutLine(CNurse* pNurse)
{
	if(m_pChatDlg)
	{
		((CChatDlg*)m_pChatDlg)->DelUser(pNurse);
	}

	//CServiceController::OnNurseOutLine(pNurse);
}

void CQueueController::OnDoctorLogin(CDoctor* pDoctor, BOOL bLogin)
{
	if(m_pChatDlg)
	{
		((CChatDlg*)m_pChatDlg)->AddUser(pDoctor, bLogin);
	}

	//CServiceController::OnDoctorLogin(pDoctor, bLogin);
}

void CQueueController::OnDoctorOutLine(CDoctor* pDoctor, int nExitCode)
{
	if(m_pChatDlg)
	{
		((CChatDlg*)m_pChatDlg)->DelUser(pDoctor);
	}

	//CServiceController::OnDoctorOutLine(pDoctor);
}

BOOL CQueueController::SendLogin()
{
	//构造数据
	BROADCAST user;
	memset(&user, 0, sizeof(user));
	SetSendBaseInfo((LPBASEINFO)&user, ntiLogin, sizeof(user));

	if(GetUserType() == utDoctor)
	{
		int nLoginType = MyCommon::GetProfileInt(
			_T("sys"), _T("LOGIN_TYPE"), 0, _T("SYS\\port.ini"));
		if(nLoginType != 0)
		{
			USHORT uNursePort = MyCommon::GetProfileInt(
				_T("sys"), _T("NURSE_PORT"), 4020, _T("SYS\\port.ini"));
			CString strNurseIP = MyCommon::GetProfileString(
				_T("sys"), _T("NURSE_IP"), _T("192.168.1.100"), _T("SYS\\port.ini"));
			SendTo((char*)&user, sizeof(user), strNurseIP, uNursePort);
		}
		return Broadcast((char*)&user, sizeof(user), m_uBroadPort);
	}
	else
	{
		return Broadcast((char*)&user, sizeof(user));
	}
}

//void CQueueController::OnSendHeartBeat(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
void CQueueController::SendHeartBeat()
{
	BROADCAST user;
	memset(&user, 0, sizeof(user));
	SetSendBaseInfo((LPBASEINFO)&user, ntiHeartBeat, sizeof(user));

	if(GetUserType() == utDoctor)
	{
		Broadcast((char*)&user, sizeof(user), m_uBroadPort);
	}
	else
	{
		Broadcast((char*)&user, sizeof(user));
	}
}

BOOL CQueueController::SendChat(CString strData, LPSOCKADDR_IN psockddr)
{
	CHATMSG chat;
	memset(&chat, 0, sizeof(chat));
	SetSendBaseInfo((LPBASEINFO)&chat, ntiChatMsg, sizeof(chat));

	for(; !strData.IsEmpty(); )
	{
		CString strSend = strData.Left(MAX_CHATMSG_LENGTH-2);
		strData = strData.Mid(MAX_CHATMSG_LENGTH - 1);

		wcscpy_s(chat.wszMsg, MAX_CHATMSG_LENGTH, strSend.GetBuffer());

		if(NULL == psockddr)
		{   //广播
			chat.eChatTypeID = ctPublicChat;
			if(GetUserType() == utDoctor)
			{
				if(!Broadcast((char*)&chat, sizeof(chat), m_uBroadPort))
				{
					return FALSE;
				}
			}
			else
			{
				if(!Broadcast((char*)&chat, sizeof(chat)))
				{
					return FALSE;
				}
			}
		}else
		{   //发送到指定IP
			chat.eChatTypeID = ctPrivateChat;
			if(!SendTo( (LPSTR)&chat, sizeof( chat ), psockddr))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CQueueController::SendQuit()
{
	return SendExitCode(EXITCODE_UPDATE_ALL);
}

BOOL CQueueController::SendExitCode(int nExitCode)
{
	QUIT quit;
	memset(&quit, 0, sizeof(quit));
	SetSendBaseInfo((LPBASEINFO)&quit, ntiQuit, sizeof(quit));
	quit.nExitCode = nExitCode;

	if(GetUserType() == utDoctor)
	{
		return Broadcast((char*)&quit, sizeof(quit), m_uBroadPort);
	}
	else
	{
		return Broadcast((char*)&quit, sizeof(quit));
	}
}

UINT CQueueController::SendHeartBeatThreadProc(LPVOID pParam)
{
	CQueueController* pThis = (CQueueController*)pParam;

	while(1)
	{
		pThis->SendHeartBeat();
		Sleep(1000);
	}
	::ExitThread(0);
	return 0;
}



