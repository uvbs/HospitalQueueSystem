#pragma once
//#include "../NurseStation/BaseConfig.h"
#include "afxwin.h"
#include "resource.h"

// CPropConnectToHisDbDlg 对话框

class CPropConnectToHisDbDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropConnectToHisDbDlg)

public:
	CPropConnectToHisDbDlg();
	virtual ~CPropConnectToHisDbDlg();

// 对话框数据
	enum { IDD = IDD_PROP_HIS_DB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnApply();

	virtual BOOL OnInitDialog();

	CString m_strUID;
	CString m_strPWD;
	CString m_strHostIP;
	afx_msg void OnEnChangeEditMyaccount();
	afx_msg void OnEnChangeEditMypass();
	afx_msg void OnEnChangeEditMyip();
	CString m_strPort;
	CString m_strServiceName;
	CString m_strDataDays;
	afx_msg void OnEnChangeEditPort();
	afx_msg void OnEnChangeEditServiceName();
	afx_msg void OnEnChangeEditDataDays();
};
