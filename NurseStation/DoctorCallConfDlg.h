#pragma once
#include "afxwin.h"


// CDoctorCallConfDlg 对话框

class CDoctorCallConfDlg : public CDialog
{
	DECLARE_DYNAMIC(CDoctorCallConfDlg)

public:
	CDoctorCallConfDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDoctorCallConfDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOCTOR_CALL_CONF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CString m_strSoundCall;
	CString m_strSoundWait;
	CString m_strDisplayCall;
	CString m_strDisplayWait;
	//CComboBox m_cbCallSpecial;
	BOOL m_bCallSpecial;
	CString m_strRecallTimes;
	CButton m_ckCallSpecial;
	BOOL m_bWaitWhenCall;
	CButton m_ckWaitWhenCall;
	CString m_strMaxWaitNum;
	int m_nRadioConfType;
	//CButton m_ckConfAll;
	afx_msg void OnBnClickedButtonSetSoundCall();
	afx_msg void OnBnClickedButtonSetSoundWait();
	afx_msg void OnBnClickedButtonSetDisplayCall();
	afx_msg void OnBnClickedButtonSetDisplayWait();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
