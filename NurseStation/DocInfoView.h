#pragma once
#include "afxcmn.h"
#include "BaseConfig.h"
#include "ado2.h"
#include "PoolManage.h"
#include <afxmt.h>
#include "ListCtrlCl.h"
// CDocInfoView 窗体视图

class CDocInfoView : public CFormView
{
	DECLARE_DYNCREATE(CDocInfoView)

protected:
	CDocInfoView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDocInfoView();

public:
	enum { IDD = IDD_FORMVIEW_DOCINFO };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlCl m_view_docinfo;
	void ShowList();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual void OnInitialUpdate();
//	void ReadData();
	

//	CList<CPoolManage::CDoctorInfo,CPoolManage::CDoctorInfo&> m_docInfoList;
	void FlushDoctorData(USERSTATUS& userstatus);
	void ShowDoctorData(USERSTATUS& userstatus);
protected:
	CRect m_rect;
public:
	afx_msg void OnNMRClickViewDocinfo(NMHDR* pNMHDR, LRESULT *pResult);
	afx_msg void OnDoctorCall();
	afx_msg void OnDoctorRecall();
//	CList<USERSTATUS,USERSTATUS&> m_listUserStatus;
	CMap<CString,LPCWSTR,USERSTATUS,USERSTATUS&> m_listUserStatus;
	void ShowAllInlineDoctorInfo();
	CMap<int,int,CString, LPCWSTR> m_itemToDocID;
	CMutex m_mutexDoc;
	afx_msg void OnLvnItemchangedViewDocinfo(NMHDR *pNMHDR, LRESULT *pResult);
	int m_item;
	afx_msg void OnNMCustomdrawViewDocinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDoctorWait();
};


