#pragma once
#include "afxcmn.h"

#include "ListCtrlCl.h"
#include "PoolManage.h"

#include "Account.h"
// COfficeQueView 窗体视图
#define  WM_USEROFFICECLICK WM_USER+100

class COfficeQueView : public CFormView
{
	DECLARE_DYNCREATE(COfficeQueView)

protected:
	COfficeQueView();           // 动态创建所使用的受保护的构造函数
	virtual ~COfficeQueView();

public:
	enum { IDD = IDD_FORMVIEW_OFFICEQUE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CRect m_Rect;
	DECLARE_MESSAGE_MAP()
public:
	CListCtrlCl m_view_officeque;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ShowList();
	virtual void OnInitialUpdate();
	void ReadData();
	
	CString m_officeID;
	
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnLvnItemchangedListOfficeque(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListOfficeque(NMHDR *pNMHDR, LRESULT *pResult);
	CString GetOfficeID(){return m_officeID;}
	
	BOOL SearchData(CString officeid);
	BOOL SearchData(CString &date,CString& contex,int index);
	CPoolManage m_pool;
//	CMainFrame *pWnd;
	
	
	CString m_officename;

	CString GetSelectOfficename(){return m_officename;}

//	void SearchDocData();
	CADORecordset* SearchNurseContrlOffice();

	std::vector<CString> m_CandoOfficeID;
	void GetNurseContrlOfficeID();	

	//void SetDataDays(int nDays);
	//BOOL m_bReadInPatient;
private:
	int m_item;
	//CString m_strSqlDate;
};


