#pragma once

#include "BaseConfig.h"
// CSetDataBaseDlg 对话框
//#include "ConnectToBase.h"
class CSetDataBaseDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetDataBaseDlg)

public:
	CSetDataBaseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetDataBaseDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETDATABASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_set_account;
	CString m_set_pass;
	CString m_set_serverip;
	//CString m_set_database;
	CBaseConfig m_baseconfig;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
//	CConnectToBase m_connecttobase;
	//int m_selectbase;
	afx_msg void OnBnClickedButtonInitDb();
};
