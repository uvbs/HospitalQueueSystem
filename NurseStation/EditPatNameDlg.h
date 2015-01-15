#pragma once
#include "afxwin.h"


// CEditPatNameDlg 对话框

class CEditPatNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditPatNameDlg)

public:
	CEditPatNameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditPatNameDlg();

// 对话框数据
	enum { IDD = IDD_EDITNAME_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_ed_oldName;
	CEdit m_ed_newName;
	afx_msg void OnBnClickedOk();
};
