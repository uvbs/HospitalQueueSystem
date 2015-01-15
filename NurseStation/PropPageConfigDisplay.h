#pragma once
#include "afxwin.h"


// CPropPageConfigDisplay 对话框

class CPropPageConfigDisplay : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigDisplay)

public:
	CPropPageConfigDisplay();
	virtual ~CPropPageConfigDisplay();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_CONFIG_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strPageItemCount;
	CString m_strSwitchPageTime;
	CButton m_ckShowQueueNum;
	CButton m_ckSwitchPage;
	CString m_strPageColCount;
	CButton m_ckShowLineNum;

	CString m_strConsultStbTitle;
	CButton m_ckConsultShowNum;
	CString m_strSeparator;

	void Load();
	void Save();
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnBnClickedCheckShowLineNum();
	afx_msg void OnBnClickedCheckSwitchPage();
	afx_msg void OnBnClickedCheckShowNum();
	afx_msg void OnEnChangeEditPageColCount();
	afx_msg void OnEnChangeEditItemcount();
	afx_msg void OnEnChangeEditSwitchMinute();
	afx_msg void OnEnChangeEditConsultStbTitle();
	afx_msg void OnBnClickedCheckConsultShowNum();
	afx_msg void OnEnChangeEditSeparator();

	CString m_strKeshiFile;
	CString m_strZhenshiFile;

	CButton m_ckShowSum;
	afx_msg void OnBnClickedCheckShowSum();
	int m_nShowType;
	afx_msg void OnBnClickedRadioShowType();
	afx_msg void OnBnClickedRadioShowType2();
	afx_msg void OnBnClickedRadioShowType3();
	afx_msg void OnBnClickedRadioShowType4();
};
