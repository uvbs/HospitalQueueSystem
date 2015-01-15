#pragma once

#include "Controller.h"

class CLoginController : public CController
{
public:
	CLoginController(void);
	virtual ~CLoginController(void);

	//BOOL CanLogin()
	//{
	//	return m_bCanLogin;
	//}
	//CString GetUserName()
	//{
	//	return m_strUserName;
	//}
	CString GetPassword()
	{
		return m_strPassword;
	}
	void SetPassword(CString strPassword)
	{
		m_strPassword = strPassword;
	}
	BOOL GetCallSpecial()
	{
		return m_bCallSpecial;
	}
	void SetCallSpecial(BOOL bCallSpecial)
	{
		m_bCallSpecial = bCallSpecial;
	}
	VerifyResultEnum GetVerifyResult()
	{
		return m_eVerifyResult;
	}
	LPSOCKADDR_IN GetPNurseSock()
	{
		return &m_sockNurse;
	}
	void SetNurseSock(LPSOCKADDR_IN pSock)
	{
		m_sockNurse = *pSock;
	}

protected:

	virtual BOOL OnStartControl();
	virtual BOOL OnEndControl();

	virtual void OnRecvLoginResult(LPCMDRESULT pLoginResult, LPSOCKADDR_IN psockddr) = 0;

private:
	///////		CMD dispatch function	/////////////////////////////
	virtual void OnRecvReqLogin(LPREQLOGIN pReqLogin, LPSOCKADDR_IN psockddr){}
	virtual void OnRecvLogin(LPBROADCAST pLogin, LPSOCKADDR_IN psockddr){}
	virtual void OnRecvQuit(LPQUIT pQuit, LPSOCKADDR_IN psockddr){}
	virtual void OnRecvHeartBeat(LPBROADCAST pHeartbeat, LPSOCKADDR_IN psockddr){}
	virtual void OnRecvDoctorCmd(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockddr){}
	virtual void OnRecvCmdResult(LPCMDRESULT pCmdResult, LPSOCKADDR_IN psockddr);
	virtual void OnRecvChatMsg(LPCHATMSG pChatMsg, LPSOCKADDR_IN psockddr){}
	///////		End	CMD dispatch function	/////////////////////////

	virtual void OnSendVerify(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	virtual void SendVerify();
private:
	//BOOL m_bCanLogin;
	MyTimer<CLoginController> m_timerSendVerify;
	CString m_strPassword;
	BOOL m_bCallSpecial;
	//BOOL m_bExpertFlag;
	VerifyResultEnum m_eVerifyResult;
	SOCKADDR_IN m_sockNurse;

protected:
	USHORT m_uNursePort;
	CString m_strNurseIP;
};
