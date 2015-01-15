#pragma once
#include "afxdtctl.h"
#include "afxwin.h"


// CSuspendPatDlg 对话框

class CSuspendPatDlg : public CDialog
{
	DECLARE_DYNAMIC(CSuspendPatDlg)

public:
	CSuspendPatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSuspendPatDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SUSPEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CDateTimeCtrl m_last_datetime;
	CEdit m_ed_currenttime;
	CTime m_settime;
	CTime m_current_time;
	CDateTimeCtrl m_suspend_time;
	afx_msg void OnBnClickedOk();
	void WriteFile();
};
