#pragma once

#include "../NurseStation/QueueController.h"

class CDoctorController : public CQueueController
{
public:
	CDoctorController(void* pParent);
	~CDoctorController(void);

	virtual BOOL OnStartControl();
	virtual BOOL OnEndControl();

	LPSOCKADDR_IN GetPNurseSock()
	{
		return &m_sockNurse;
	}
	void SetNurseSock(LPSOCKADDR_IN pSock)
	{
		m_sockNurse = *pSock;
	}
	BOOL GetCallSpecial()
	{
		return m_bCallSpecial;
	}
	void SetCallSpecial(BOOL bCallSpecial)
	{
		m_bCallSpecial = bCallSpecial;
	}
	void SetExpertFlag(BOOL bExpertFlag)
	{
		m_bExpertFlag = bExpertFlag;
	}
	void SetExitCode(int nCode)
	{
		m_nExitCode = nCode;
	}

	BOOL SendCmd(DoctorCmdEnum eDoctorCmdEnum, CString strQueSerialId = _T(""));
	virtual BOOL SendQuit();

protected:
	
	virtual void OnRecvCmdResult(LPCMDRESULT pCmdResult, LPSOCKADDR_IN psockddr);

private:

	virtual void OnRecvDoctorCmd(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockddr){}

private:

	SOCKADDR_IN m_sockNurse;
	BOOL m_bCallSpecial;
	BOOL m_bExpertFlag;

	void* m_pParent;
	int m_nExitCode;
public:
	CString m_strConn;
	CString m_strSelect;
	CString m_strSelectDoneDiscard;
	CString m_strSelectLeft;

	int GetDataDays(){return m_DbParam.GetDataDays();}

	CString m_strNurseId;
	CString GetNurseId(){return m_strNurseId;}
	void SetNurseId(CString strNurseId){m_strNurseId = strNurseId;}
};
