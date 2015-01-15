#pragma once


// CInitLocalDbDlg 对话框

class CInitLocalDbDlg : public CDialog
{
	DECLARE_DYNAMIC(CInitLocalDbDlg)

public:
	CInitLocalDbDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInitLocalDbDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INIT_LOCAL_DB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUID;
	CString m_strPWD;
	CString m_strHostIP;
	afx_msg void OnEnChangeEditUid();
	afx_msg void OnEnChangeEditPwd();
	afx_msg void OnEnChangeEditHost();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
