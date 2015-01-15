#pragma once
#include "afxwin.h"
//#include "DoctorCallerDlg.h"
#include "PoolManage.h"
#include "afxcmn.h"
#include "Parameter.h"
#include "afxdtctl.h"
#include "NurseDef.h"
#include "SLZPrinter.h"

// CNurseView 窗体视图

class CNurseView : public CFormView
{
	DECLARE_DYNCREATE(CNurseView)

public:
	CNurseView();           // 动态创建所使用的受保护的构造函数
	virtual ~CNurseView();

public:
	enum { IDD = IDD_NURSEFORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//virtual BOOL OnInitDialog();
	//virtual void OnInitialUpdate();



	DECLARE_MESSAGE_MAP()
public:
//	CStatic m_static_queinfo;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnBnClickedBnChat();
//	afx_msg void OnBnClickedFlushPait();
	void OnInitialUpdate();
	void ShowLoginNurseName();
	//CDoctorCallerDlg* m_pChatDlg;
	BOOL m_ChatDlgDoModelFlag;
	CStatic m_login_nursename;
//	afx_msg void OnBnClickedBnChangeoffice();
//	CEdit m_pait_logid;
//	CEdit m_pait_lastoffice;
//	CEdit m_pait_name;
//	CEdit m_pait_lastdoc;
//	CComboBox m_com_lastoffice;
//	CComboBox m_com_setdoctor;

//	void ShowData();
	void ShowOffice();
//	void ShowDoctor();
	CPoolManage m_pool;
//	afx_msg void OnCbnSelchangeComboLastoffice();
	
	afx_msg void OnBnClickedBnGetquenum();
	CEdit m_que_logid;
	CEdit m_que_patname;
//	CEdit m_que_officename;
//	CEdit m_que_docname;

//	CComboBoxEx m_comex_quename;
	CComboBox m_comex_quename;
	afx_msg void OnCbnSelchangeComQuename();
	CComboBox m_com_quedocname;

	//void NoHisWriteDataToQueue();
	int NoHisWriteDataToQueue(CString strSerialId);
	int GetSrand(const int &max);
	CString GetOnlyID();
	afx_msg void OnBnClickedCheckSuspend();
	afx_msg void OnBnClickedCheckQueing();
	afx_msg void OnBnClickedCheckHavecall();
	afx_msg void OnBnClickedCheckQuitque();
	CButton m_check_quit;
	CButton m_check_hacecall;
	CButton m_check_waitting;
	CButton m_check_suspend;
//	CComboBox m_com_search;
//	afx_msg void OnCbnSelchangeComSearch();
//	CStatic m_search_text;
//	afx_msg void OnBnClickedBnSearch();
//	CTime m_date;
//	CDateTimeCtrl m_date_picker;
//	afx_msg void OnDtnDatetimechangeDateSearch(NMHDR *pNMHDR, LRESULT *pResult);
//	CEdit m_ed_searchcontext;
//	afx_msg void OnBnClickedButton4();
	CComboBox m_com_calldoc;

	void SerchDoctor();

	CMap<int,int,DOCTOCOMBOINFO,DOCTOCOMBOINFO&> m_mapto_combo;
	CMap<int,int,DOCTOCOMBOINFO,DOCTOCOMBOINFO&> m_maptocall_combo;
	afx_msg void OnBnClickedBnCall();
	afx_msg void OnBnClickedBnRecall();
	afx_msg void OnBnClickedBnWait();
	afx_msg void OnBnClickedBnQuitnum();
	CString GetCallDoctorId();
//	CString m_call_doctorid;
public:
	SLZPrinter m_printer;
private:
	int GetQueNum(const CString& queID);
public:
	CButton m_btnTakingNum;
	void EnableTakingNum(BOOL bEnable);
	CString m_strQueueNum;
	CEdit m_editQueueNum;
};


