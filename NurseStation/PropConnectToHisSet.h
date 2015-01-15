#pragma once
#include "BaseConfig.h"
#include "afxwin.h"

#define MY_DEFAULT_OPEN_PORT		8888		//缺省对外监听端口

// CPropConnectToHisSet 对话框

class CPropConnectToHisSet : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropConnectToHisSet)

public:
	CPropConnectToHisSet();
	virtual ~CPropConnectToHisSet();

// 对话框数据
	enum { IDD = IDD_PROP_LOCAL_DB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_his_port;
	CString m_his_ip;
	CString m_his_pass;
	CString m_his_account;
	virtual BOOL OnApply();
	CBaseConfig m_baseConfig;
//	CButton m_his_selectbase;
	CString m_hisDataDrive;
	int m_his_selectbase;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdAccount();
	afx_msg void OnEnChangeEdPass();
	afx_msg void OnEnChangeEdPort();
	afx_msg void OnEnChangeEdIP();
	afx_msg void OnBnClickedRadioHisoracle();
	afx_msg void OnBnClickedRadioHissql();
	afx_msg void OnBnClickedRadioHismysql();
	CString m_his_basename;
	afx_msg void OnEnChangeEditBasename();

	CString m_strLocalIp;
	CString m_strListenPort;

	//CString CPropConnectToHisSet::GetPort();
};
