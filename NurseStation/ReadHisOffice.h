#pragma once
#include "ConnectToHis.h"
#include "ConnectToMyDataBase.h"
#include "../NurseStation/BaseConfig.h"
#include "QueueServerDef.h"
class CReadHisOffice
{
public:
	CReadHisOffice(void);
	~CReadHisOffice(void);
	void FlushHisOfficeToMyTable();

	CConnectToHis		  m_connectToHis;
	CConnectToMyDataBase  m_connectToMyDataBase;

	BOOL OpenHisDataBase();

	BOOL OpenMyDataBase();

	void CompareDeleteOffice();

	static DWORD WINAPI ReadHisOfficeThread(LPVOID pParam);
	BOOL StartReadHisOfficeThread();
	CBaseConfig m_baseConfig;
	HANDLE m_hReadHisOffice;
};
