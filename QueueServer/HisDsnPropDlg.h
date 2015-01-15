#pragma once

#include "resource.h"
// CHisDsnPropDlg 对话框

class CHisDsnPropDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CHisDsnPropDlg)

public:
	CHisDsnPropDlg();
	virtual ~CHisDsnPropDlg();

// 对话框数据
	enum { IDD = IDD_PROP_HIS_DSN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

	DECLARE_MESSAGE_MAP()

public:
	CString m_strHisDsn;

	afx_msg void OnEnChangeEditDsn();
};
