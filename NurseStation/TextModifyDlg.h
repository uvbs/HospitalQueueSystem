#pragma once


// CTextModifyDlg 对话框

class CTextModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CTextModifyDlg)

public:
	CTextModifyDlg(CString& strTextRef, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTextModifyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEXT_MODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strText;
	CString& m_rstrText;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
