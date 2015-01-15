#pragma once
#include "afxwin.h"


// CAddCommentDlg 对话框

class CAddCommentDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddCommentDlg)

public:
	CAddCommentDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddCommentDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADD_COMMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString GetComment(){return m_strComment;}
private:
	CString m_strComment;
public:
	//int m_nRdSelectComment;
	CComboBox m_cbComment;
	//CString m_strSelfComment;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
