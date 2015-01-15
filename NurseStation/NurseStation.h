// NurseStation.h : NurseStation 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "stdafx.h"
#include "resource.h"		// 主符号
#include "ConnectToBase.h"
#include "MainFrm.h"
#include "Doc_NurseDlg.h"
#include "DocInfoView.h"
#include "JttsAPI.h"
#include "PoolManage.h"
#include "UserSetting.h"
#include "OfficeQueView.h"
#include "PaientView.h"
#include "PropQueSettting.h"
#include "NurseController.h"


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

// CNurseStationApp:
// 有关此类的实现，请参阅 NurseStation.cpp
//

class CNurseStationApp : public CWinApp
{
public:
	CNurseStationApp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:

	CNurseController* GetController()
	{
		return m_pNurseController;
	}

public:
	CMainFrame* m_pNurseWnd;
	CDocInfoView* m_pDocInfoView;
	COfficeQueView* m_pOfficeQueView;
	CPaientView*   m_pPaientView;
	CNurseView*    m_pNurseView;
	//CJttsAPI* m_pJttsAPI;

//	BOOL m_IsFront;
	CString m_loadNurseID;//登录护士id
	CString m_loadNursePassword;//密码
	CString m_loadNurseName;//护士名字
	CArray<CString> m_canDoOfficeIDList;
	int m_power;//权限
	CString m_strIP;//登录ip
	int GetUserPower(){return m_power;}
	CString GetLoginID(){return m_loadNurseID;}
	CString GetLoginNursename(){return m_loadNurseName;}

	//CDoctorCallerDlg* m_pChatDlg;
	CUserSetting* m_pUserSetting;
	CPropQueSettting* m_pPropQueSetting;
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	CPropSetting* m_pPropSetting;

	//BOOL VerifySerialNo();
	//BOOL ReadSerialNo(CString& strSerialNo);
	BOOL m_backSet;

	virtual BOOL PreTranslateMessage(MSG* pMsg);


public:
	//BOOL OpenDB(CADODatabase &db);
	//BOOL AddAutoRun(CString strValueName);

private:
	CNurseController* m_pNurseController;
};

extern CNurseStationApp theApp;

#ifdef _DEBUG

extern void MyWriteConsole(CString str);

#endif
//extern int g_nDataDays;