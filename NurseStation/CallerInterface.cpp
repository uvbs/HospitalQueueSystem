#include "StdAfx.h"
#include "CallerInterface.h"
#include "./com/SLZCCaller.h"
#include "NurseStation.h"
#include "SendToHardWare.h"

CCallerInterface::CCallerInterface(void) : m_hDoCallerCmd(NULL)
{
	Init();
}

CCallerInterface::~CCallerInterface(void)
{
	if(m_hDoCallerCmd)
	{
		TerminateThread(m_hDoCallerCmd,0);
		CloseHandle(m_hDoCallerCmd);
		m_hDoCallerCmd = NULL;
	}
}

BOOL CCallerInterface::Init()
{
	if(m_hDoCallerCmd == NULL)
	{
		m_hDoCallerCmd = CreateThread(NULL,0,DoCallerCmd,this,0,NULL);
		if(m_hDoCallerCmd)
		{
			return TRUE;
		}
	}
	return FALSE;
}

DWORD WINAPI CCallerInterface::DoCallerCmd(LPVOID pParam)
{
	SLZCCaller* pSLZCaller = SLZCCaller::GetInstance();
	while(1)
	{
		if(!pSLZCaller->HasData())
		{
			Sleep(15);
			continue;
		}
		SLZCallerData data = pSLZCaller->GetData();
		BOOL flag = FALSE;
		//CString strRetData;
		switch(data.GetCmdType())
		{
		case cmdCall:
			flag = theApp.GetController()->SendCmd(data.GetCallerId(),dcCmdCall);
			break;
		case cmdRecall:
			flag = theApp.GetController()->SendCmd(data.GetCallerId(),dcCmdRecall);
			break;
		case cmdDiscard:
			flag = theApp.GetController()->SendCmd(data.GetCallerId(),dcCmdDiscard);
			break;
		case cmdWait:
			flag = theApp.GetController()->SendCmd(data.GetCallerId(),dcCmdWait);
			break;
		case cmdPause:
			flag = theApp.GetController()->SendCmd(data.GetCallerId(),dcCmdPause);
			break;
		case callerCmdShowAdd:
			CSendToHardWare::SendToHardWare(enumCallerScreen,data.GetCallerId(),_T(""),_T(""),0,-1,callerCmdShowAdd);
			break;
		default:
			break;
		}
		//if(!strRetData.IsEmpty() && flag)
		//	CSendToHardWare::SendToHardWare(enumCallerScreen,data.GetCallerId(),strRetData,_T(""),0,-1,callerCmdShowNum);
		Sleep(35);
	}
	return 0;
}

CCallerInterface* CCallerInterface::GetInstance()
{
	static CCallerInterface instance;
	return &instance;
}