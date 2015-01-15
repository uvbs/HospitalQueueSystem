#pragma once
#include "afxwin.h"


// CPropPageDoctorInfo 对话框

class CPropPageDoctorInfo : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPageDoctorInfo)

public:
	CPropPageDoctorInfo(CString strDoctorId, CString strDbConn);
	virtual ~CPropPageDoctorInfo();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_MODIFY_DOCTOR_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strId;
	CString m_strName;
	CString m_strDept;
	CString m_strTitle;
	CString m_strLoginId;
	afx_msg void OnEnChangeEditLoginId();
	virtual BOOL OnApply();

	CString m_strDbConn;
	virtual BOOL OnInitDialog();
	CButton m_btnPassword;
	afx_msg void OnBnClickedButtonPassword();
};
