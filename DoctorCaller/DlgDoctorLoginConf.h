#pragma once
#include "afxwin.h"


// CDlgDoctorLoginConf 对话框

class CDlgDoctorLoginConf : public CDialog
{
	DECLARE_DYNAMIC(CDlgDoctorLoginConf)

public:
	CDlgDoctorLoginConf(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDoctorLoginConf();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOGIN_CONF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNurseIP;
	CString m_strNursePort;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	int m_nLoginType;
	CEdit m_editNurseIP;
	CEdit m_editNursePort;
	afx_msg void OnBnClickedRadioLoginType2();
	CButton m_ckLoginType2;
	CButton m_ckLoginType1;
	afx_msg void OnBnClickedRadioLoginType1();
};
