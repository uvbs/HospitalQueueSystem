
#pragma once
#include "afxwin.h"
#include "DoctorManageDlg.h"


// CConfSoundDlg 对话框

class CConfSoundDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfSoundDlg)

public:
	CConfSoundDlg(CString& strConf, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfSoundDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SOUND_WAIT_CONF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editTextAdd;
	CListBox m_lbList;
	afx_msg void OnBnClickedButtonAddText();
	afx_msg void OnBnClickedButtonQueueId();
	afx_msg void OnBnClickedButtonPatientName();
	afx_msg void OnBnClickedButtonDoctorName();
	afx_msg void OnBnClickedButtonOfficeName();
	afx_msg void OnBnClickedButtonConsultName();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	int SplitStringToArray(CString strSrc, CArray<CString>& arrStr);
public:
	//CDoctorManageDlg* m_pParentDlg;
	//CADORecordset* m_pRset;
	CString& m_strConf;
	afx_msg void OnBnClickedButtonModify();
};

