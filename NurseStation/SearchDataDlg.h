#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CSearchDataDlg 对话框

class CSearchDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CSearchDataDlg)

public:
	CSearchDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSearchDataDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_search_item;
	CComboBox m_com_searchway;
	CDateTimeCtrl m_date_picker;
	CTime m_date;
	afx_msg void OnDtnDatetimechangeDatetimePicker(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboSearchway();
	CEdit m_ed_searchcontex;
	afx_msg void OnBnClickedOk();
};
