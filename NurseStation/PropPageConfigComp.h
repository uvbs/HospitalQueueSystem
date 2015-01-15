#pragma once


// CPropPageConfigComp 对话框

class CPropPageConfigComp : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigComp)

public:
	CPropPageConfigComp();
	virtual ~CPropPageConfigComp();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_CONFIG_COMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int m_nRadioDisableTakingNum;
	CString m_strRefreshTime;
	CString m_strDataDays;
	int m_nReadIn;

	void Load();
	void Save();
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnEnChangeEditRefreshTime();
	afx_msg void OnEnChangeEditDataDays();
	afx_msg void OnBnClickedRadioReadIn();
	afx_msg void OnBnClickedRadioNotReadIn();
	afx_msg void OnBnClickedRadioDisableTaking();
	afx_msg void OnBnClickedRadioEnableTaking();

	CString m_strFile;
	CString m_strTimeCall;
	afx_msg void OnEnChangeEditTimeCall();
};
