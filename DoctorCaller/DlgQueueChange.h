#pragma once
#include "afxwin.h"


// CDlgQueueChange 对话框

class CDlgQueueChange : public CDialog
{
	DECLARE_DYNAMIC(CDlgQueueChange)

public:
	CDlgQueueChange(CString strConn, CString strDoctorId, CString strNurseId, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgQueueChange();

// 对话框数据
	enum { IDD = IDD_DIALOG_QUEUE_CHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strConn;
	CString m_strDoctorId;
	CString m_strNurseId;

	void RefreshQueue();
	CListBox m_lbSelectQueue;
	CListBox m_lbAllQueue;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();

	CString m_strSqlSelQueue;
	CString m_strSqlAllQueue;
	CMap<int,int,CString,LPCTSTR> m_mapSelQueue;
	CMap<int,int,CString,LPCTSTR> m_mapAllQueue;
};
