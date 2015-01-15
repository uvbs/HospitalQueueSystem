#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CStbPropDlg 对话框

class CStbPropDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CStbPropDlg)

public:
	CStbPropDlg();
	virtual ~CStbPropDlg();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_STB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void RefreshList(void);

private:
	CComboBox m_cbStbList;
	CListCtrl m_lcRoomList;
	CString m_strStbName;

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
	afx_msg void OnEnChangeEdWaitname();
	afx_msg void OnBnClickedBnAddwaitroom();
	afx_msg void OnBnClickedBnDeletewaitroom();
	virtual BOOL OnApply();
	CString m_strStbIp;
	CString m_strStbPort;
	CString m_strStbInitText;
	afx_msg void OnEnChangeEdStbIp();
	afx_msg void OnEnChangeEdStbPort();
	afx_msg void OnEnChangeEditStbInitText();
	CString m_strStbId;
	afx_msg void OnEnChangeEdStbId();
	CComboBox m_cbStbType;
	//CMap<int, int, CString, LPCTSTR> m_mapStbType;
	CStringArray m_strarrStbType;
	afx_msg void OnCbnSelchangeComboStbType();
	virtual void OnCancel();
	virtual void OnOK();
};
