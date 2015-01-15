#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CWaitingRoomConfPropDlg 对话框

class CWaitingRoomConfPropDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CWaitingRoomConfPropDlg)

public:
	CWaitingRoomConfPropDlg();
	virtual ~CWaitingRoomConfPropDlg();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_WAITINGROOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void RefreshList(void);
	void UpdateStbList();

	void UpdateFromDB();

private:
	CComboBox m_cbStbList;
	CListCtrl m_lcRoomList;
	CString m_strRoomName;

	BOOL QueryDB(void);
	CADODatabase m_db;
	CADORecordset m_set;
	CString m_strSqlQuerry;
	LONG m_nCurSel;

	CMap<int, int, CString, LPCTSTR> m_mapStbId_Name;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListRoomlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListDocList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboStblist();
	afx_msg void OnEnChangeEdWaitname();
	afx_msg void OnBnClickedBnAddwaitroom();
	afx_msg void OnBnClickedBnDeletewaitroom();
	virtual BOOL OnApply();
};
