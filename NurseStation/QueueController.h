#pragma once

#include "ServiceController.h"

class DbParam
{
public:
	DbParam()
		: m_strDriver(_T(""))
		, m_strHost(_T(""))
		, m_strUID(_T(""))
		, m_strPID(_T(""))
		, m_strDbName(_T(""))
		, m_uPort(0)
		, m_nDataDays(0)
		, m_nCallSeconds(0)
	{}
	DbParam(LPDBPARAM pDBPARAM)
		: m_strDriver(pDBPARAM->wszDriver)
		, m_strHost(pDBPARAM->wszHost)
		, m_strUID(pDBPARAM->wszUID)
		, m_strPID(pDBPARAM->wszPID)
		, m_strDbName(pDBPARAM->wszDbName)
		, m_uPort(pDBPARAM->m_uPort)
		, m_nDataDays(pDBPARAM->m_nDataDays)
		, m_nCallSeconds(pDBPARAM->m_nCallSeconds)
	{}

	void SetDbParam(LPDBPARAM pDBPARAM)
	{
		m_strDriver = pDBPARAM->wszDriver;
		m_strHost = pDBPARAM->wszHost;
		m_strUID = pDBPARAM->wszUID;
		m_strPID = pDBPARAM->wszPID;
		m_strDbName = pDBPARAM->wszDbName;
		m_uPort = pDBPARAM->m_uPort;
		m_nDataDays = pDBPARAM->m_nDataDays;
		m_nCallSeconds = pDBPARAM->m_nCallSeconds;
	}

	LPDBPARAM ToDBPARAM(LPDBPARAM pDBPARAM)
	{
		if(!pDBPARAM) return pDBPARAM;
		wcscpy_s(pDBPARAM->wszDriver, 32, m_strDriver);
		wcscpy_s(pDBPARAM->wszHost, 32, m_strHost);
		wcscpy_s(pDBPARAM->wszUID, 32, m_strUID);
		wcscpy_s(pDBPARAM->wszPID, 32, m_strPID);
		wcscpy_s(pDBPARAM->wszDbName, 32, m_strDbName);
		pDBPARAM->m_uPort = m_uPort;
		pDBPARAM->m_nDataDays = m_nDataDays;
		pDBPARAM->m_nCallSeconds = m_nCallSeconds;
		return pDBPARAM;
	}

	CString GetDriver() const {return m_strDriver;}
	void SetDriver(CString strDriver) {m_strDriver = strDriver;}
	CString GetHost() const {return m_strHost;}
	void SetHost(CString strHost) {m_strHost = strHost;}
	USHORT GetPort() const {return m_uPort;}
	void SetPort(USHORT uPort) {m_uPort = uPort;}
	CString GetUID() const {return m_strUID;}
	void SetUID(CString strUID) {m_strUID = strUID;}
	CString GetPID() const {return m_strPID;}
	void SetPID(CString strPID) {m_strPID = strPID;}
	CString GetDbName() const {return m_strDbName;}
	void SetDbName(CString strDbName) {m_strDbName = strDbName;}
	int GetDataDays() const {return m_nDataDays;}
	void SetDataDays(int nDataDays) {m_nDataDays = nDataDays;}
	//BOOL GetExpertFlag() const {return m_bExpertFlag;}
	//void SetExpertFlag(BOOL bExpertFlag) {m_bExpertFlag = bExpertFlag;}
	int GetCallSeconds() const {return m_nCallSeconds;}
	void SetCallSeconds(int nCallSeconds) {m_nCallSeconds = nCallSeconds;} 

private:
	CString m_strDriver;
	CString m_strHost;
	USHORT m_uPort;
	CString m_strUID;
	CString m_strPID;
	CString m_strDbName;
	int m_nDataDays;
	int m_nCallSeconds;
	//BOOL m_bExpertFlag;
};

class CQueueController : public CServiceController
{
public:
	CQueueController(void);
	virtual ~CQueueController(void);

public:
	void ShowChat();
	void HideChat();

	void ShowOrHideChat();

	void MoveChatControls();
	
	//void MoveChat();
	CDialog* GetChat()
	{
		return m_pChatDlg;
	}

protected:
	virtual BOOL OnStartControl();
	virtual BOOL OnEndControl();

	//virtual void OnRecvDoctorCmd(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockddr) = 0;
	//virtual void OnRecvCmdResult(LPCMDRESULT pCmdResult, LPSOCKADDR_IN psockddr) = 0;
	virtual void OnRecvChatMsg(LPCHATMSG pChatMsg, LPSOCKADDR_IN psockddr);

	virtual void OnNurseLogin(CNurse* pNurse, BOOL bLogin);
	virtual void OnDoctorLogin(CDoctor* pDoctor, BOOL bLogin);
	virtual void OnDoctorOutLine(CDoctor* pDoctor, int nExitCode);
	virtual void OnNurseOutLine(CNurse* pNurse);
	virtual BOOL SendQuit();

public:
	BOOL SendChat(CString strData, LPSOCKADDR_IN psockddr = NULL);
protected:
	BOOL SendLogin();
	BOOL SendExitCode(int ExitCode);
	//void OnSendHeartBeat(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	CWinThread* m_pSendHeartBeatThread;
	static UINT SendHeartBeatThreadProc(LPVOID pParam);
	void SendHeartBeat();

private:
	//MyTimer<CQueueController> m_timerSendHeartBeat;
	CDialog* m_pChatDlg;
	BOOL m_bChatShow;

protected:
	USHORT m_uBroadPort;
public:
	USHORT GetBroadPort(){return m_uBroadPort;}

protected:
	DbParam m_DbParam;
public:
	DbParam* GetDbParam() {return &m_DbParam;}
};
