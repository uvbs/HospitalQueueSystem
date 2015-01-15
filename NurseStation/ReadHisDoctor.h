#pragma once
#include "ConnectToHis.h"
#include "ConnectToMyDataBase.h"
#include "QueueServerDef.h"
#include "../NurseStation/BaseConfig.h"
class CReadHisDoctor
{
public:
	CReadHisDoctor(void);
	~CReadHisDoctor(void);

	void FlushHisDoctorToMyTable();

	CConnectToHis          m_connectToHis;
	CConnectToMyDataBase   m_connectToMyDataBase;


	BOOL OpenHisDataBase();

	BOOL OpenMyDataBase();

	void CompareDeleteDoctor();
	CBaseConfig m_baseConfig;
	HANDLE m_hReadHisDoctor;
	static DWORD WINAPI ReadHisDoctorThread(LPVOID pParam);

	BOOL StartReadHisDoctorThread();
};
