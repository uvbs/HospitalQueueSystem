// DoctorCaller.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "DoctorCallerDlg.h"
#include "LoginDlg.h"


// CDoctorCallerApp:
// 有关此类的实现，请参阅 DoctorCaller.cpp
//

class CDoctorCallerApp : public CWinApp
{
public:
	CDoctorCallerApp();

	CDoctorCallerDlg* GetDoctorCallerDlg()
	{
		return m_pDoctorCallerDlg;
	}
	CLoginDlg* GetLoginDlg()
	{
		return m_pLoginDlg;
	}

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
public:

private:
	//CDoctorController* m_pDoctorController;
	CDoctorCallerDlg* m_pDoctorCallerDlg;
	CLoginDlg* m_pLoginDlg;

public:
	virtual int ExitInstance();
};

extern CDoctorCallerApp theApp;