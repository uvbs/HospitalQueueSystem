#pragma once
#include "afxcmn.h"
#include "NurseOfficeQueue.h"
#include <afxmt.h>
#include "PoolManage.h"
#include "ChangeQueDialog.h"
#include "SetDoctorDialog.h"
#include "ReCallSetDlg.h"
#include "SuspendPatDlg.h"
#include "ListCtrlCl.h"
// CPaientView 窗体视图
#define  WM_MYPAITNMSG (WM_USER+100)

class CPaientView : public CFormView
{
	DECLARE_DYNCREATE(CPaientView)

protected:
	CPaientView();           // 动态创建所使用的受保护的构造函数
	virtual ~CPaientView();

public:
	enum { IDD = IDD_FORMVIEW_PAITIENT };
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

	CListCtrlCl m_view_paient;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	void ShowData(CString sql);
	HANDLE m_hWriteHand;
	
	void ShowList();
	//void ReadData();
public:
	
	
	
	CString m_officeID;
	CList<CPoolManage::CPatientQue,CPoolManage::CPatientQue&> m_paitentque;
	CPoolManage m_pool;
	afx_msg void OnLvnItemchangedViewPaitient(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickViewPaitient(NMHDR *pNMHDR, LRESULT *pResult);
	
	
	afx_msg void OnInsertqueue();
	afx_msg void OnQuitNum();
	afx_msg void OnTimeSuspend();
	afx_msg void OnNoTimeSuspend();
	afx_msg void OnPatientActive();
	
	afx_msg void OnSetDoctor();
//	afx_msg void OnRecallConsultQueue();
//	afx_msg void OnRecallConsult();
	afx_msg void OnReConsult();
public:
	
	void DeleteYesterdayData();
	
//	void WriteDataToList(CADORecordset* pRecordset);

	CString CMinute;

	//BOOL WriteDataToMyQueue(CString pait_logid);//刷卡排队
	
	

	CList<CNurseOfficeQueue,CNurseOfficeQueue&> m_list_office;
	void ReadOfficeInformation();
	
	

//	BOOL JudgeQueNull();
	int m_item;
	afx_msg void OnChangeque();

	CString m_paitname;
	CString GetSelectPaitname(){return m_paitname;}
	CString m_paitlogid;
	CString GetSelectPaitLogid(){return m_paitlogid;}
	CString m_selectdocname;
	CString GetSelectDocname(){return m_selectdocname;}
	

	BOOL m_bSelect;
	void FlushListOfficeData();
//	afx_msg void OnSearch();

	CString GetAddPaitName(){return m_AddPaitName;}
	CString GetAddPaitOfficeName(){return m_AddPaitOfficeName;}
	CString GetAddPaitDocName(){return m_AddPaitDocName;}
	CString GetAddPaitDocID(){return m_AddPaitDocID;}
	CString GetAddPaitOfficeID(){return m_AddPaitOfficeID;}
	//用于刷卡排队取号到数据库验证存在的数据
	CString m_AddPaitName;
	CString m_AddPaitOfficeName;
	CString m_AddPaitDocName;
	CString m_AddPaitOfficeID;
	CString m_AddPaitDocID;

	BOOL ReviseListCtrDocName(CString paitLogid,CString docName);
	BOOL ReviseListCtrConsultName(CString paitLogid,CString strConsultName);
	BOOL ReviseListCtrPatStatus(CString paitLogid,int status,CString DocName, CString strConsultName);
	BOOL ReviseListCtrPatStatus(CString paitLogid,int status);
	BOOL ReviseListCtrPatPriority(CString paitLogid,int priority);
	BOOL ReviseListCtrPatQueidcall(const CString& paitLogid,const CString& queidcall);
	BOOL ReviseListCtrPatName(const CString& paitLogid,const CString& newName);
	CMutex m_ShowLock;
	afx_msg void OnNMCustomdrawViewPaitient(NMHDR *pNMHDR, LRESULT *pResult);
	//CADODatabase m_dataBase;
	//BOOL ConnectToBase();
	CString GetSelectLogid();
	CString GetSelectStatus();


//	void GetSelectItemInfo();

//	CList<int,int&> m_list_selectItemInfo;

	CString GetSelectLogidFromItem(int nItem);
	CString GetSelectStatusFromItem(int nItem);

//	void SetColorToItem();
	BOOL m_bCanRefrush;
	afx_msg void OnRecallconsult();
	
	afx_msg void OnCall();
	afx_msg void OnRecall();
	afx_msg void OnWait();

public:
	CChangeQueDialog *m_pChangQueDlg;
	afx_msg void OnEditname();

	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


