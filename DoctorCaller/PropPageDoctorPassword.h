#pragma once


// CPropPageDoctorPassword 对话框

class CPropPageDoctorPassword : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPageDoctorPassword)

public:
	CPropPageDoctorPassword(CString strDoctorId, CString strDbConn);
	virtual ~CPropPageDoctorPassword();

// 对话框数据
	enum { IDD = IDD_DIALOG_MODIFY_DOCTOR_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strOldPassword;
	CString m_strNewPassword;
	CString m_strConfirm;

	CString m_strId;
	CString m_strDbConn;
	virtual BOOL OnApply();
	afx_msg void OnEnChangeEditNewPassword();

	BOOL m_bChanged;
};
