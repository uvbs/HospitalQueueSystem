#pragma once
#include "afxwin.h"
#include "ado2.h"
#include "PoolManage.h"
#include "BaseConfig.h"
#include "afxcmn.h"

#include "NurseOfficeQueue.h"
#include "Consult.h"
#include <map>
using namespace std;
// CPropQueSettting 对话框
//#define  WM_MYCHANGEMSG (WM_USER+10001)
class CPropQueSettting : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropQueSettting)

public:
	CPropQueSettting();
	virtual ~CPropQueSettting();
	BOOL OnInitDialog();
	void WriteDataToList();
// 对话框数据
	enum { IDD = IDD_PROPQUESETTING };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBnAddque();
	afx_msg void OnBnClickedBnDelque();
	CListBox m_list_nofficeque;
	CList<CNurseOfficeQueue,CNurseOfficeQueue&> m_listNuOfQue;
	CADODatabase *m_pbase;
	CBaseConfig m_baseConfig;
	CString m_dataDrive;
	CString m_baseIP;
	CString m_baseAccount;
	CString m_basePassWord;
	unsigned int m_listquenum;
	CString m_edit_QueName;
	CString m_edit_QueCallName;
	CString m_edit_QueID;
	CString m_edit_QueCallID;
	CString m_edit_QueStartNum;
	CString m_edit_QueEndNum;
	CString m_edit_AmLimit;
	CString m_edit_PmLimit;
	CString m_edit_DayLimit;
	CPoolManage m_poolManage;
	afx_msg void OnLbnSelchangeListNuresqueue();
	afx_msg void OnEnChangeEditNursequename();
	afx_msg void OnEnChangeEditNursecallname();
	afx_msg void OnEnChangeEditNursequeid();
	BOOL OnApply();
protected:
	int m_index;
public:
	afx_msg void OnEnChangeEditNursecallid();
	afx_msg void OnEnChangeEditNursequestartnum();
	afx_msg void OnEnChangeEditNursequeendnum();
	afx_msg void OnEnChangeEditAmlimit();
	afx_msg void OnEnChangeEditPmLimit();
	afx_msg void OnEnChangeEditDaylimit();

	afx_msg void OnEnChangeEdConsultName();
	afx_msg void OnEnChangeEdConsultCallname();
	afx_msg void OnEnChangeEdWndScrrenid();
	afx_msg void OnEnChangeEdComprehwndid();
	

	void LoadConsultData(CString SelectOfficeID);
	CListCtrl m_list_consult;
	CADORecordset* m_pre;
	afx_msg void OnLvnItemchangedOfficeConsult(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_consult_name;
	CString m_consult_callname;
	CString m_wnd_scrrenid;
	CString m_evaluator_id;
	CString m_caller_id;
	CString m_comprehwnd_id;
//	afx_msg void OnBnClickedBnRevise();
	afx_msg void OnBnClickedBnDelconsult();

	int m_selectConsultitem;
	int m_listconsult_count;
	
	afx_msg void OnNMCustomdrawOfficeConsult(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_selectOfficeID;
	afx_msg void OnBnClickedBnAddconsult();
	CString GetSelectOfficeID(){return m_selectOfficeID;}
	int GetListConsultCount(){return m_listconsult_count;}
	/**********************从his中读office数据********************/
	void ReadOfficeInfoFromHis();
	/*************************************************************/
	/**********************连接HIS数据库**************************/
	BOOL ConnectToHis();
	/*************************************************************/
	CADODatabase* m_phisDataBase;
	//连自己的数据库
	BOOL ConnectToBase();

	void OnBnClickedBnFlushhis();
	//void ReadOfficeInfoFromHis();
	//void WriteDataToList();
	void RemoveAllListBoxData();
	void AddDataToListBox();

//	CString m_ed_thruscreenip;
//	CString m_ed_thruscreenid;
//	afx_msg void OnEnChangeEdWndGip();
//	afx_msg void OnEnChangeEdGadd();

//	BOOL ReadConsultToList();
	int GetMaxConsultID();

	//int m_consultID;
	int m_consultItem;

	CString m_selectConsultID;

	CList<CConsult,CConsult&> m_list_delConsult;

	CList<CNurseOfficeQueue,CNurseOfficeQueue&> m_list_delNurseOffQue;

	typedef struct _tagStdInfo
	{
		CString m_stdName;
		CString m_stdId;
	}STDINFO,*LPSTDINFO;
	//科室液晶机顶盒MAP
	CMap<int,int,STDINFO,STDINFO&> m_mapto_stdinfo;
	//科室液晶机顶盒com
	CComboBox m_com_stdinfo;
	/////////////////////////////
	afx_msg void OnCbnSelchangeComboRtb();

	void FlushMapToStbInfo();
	//诊室液晶机顶盒com
	CComboBox m_com_consultRtb;
	//诊室液晶机顶盒map
	CMap<int,int,STDINFO,STDINFO&> m_mapto_consultStdInfo;
	afx_msg void OnCbnSelchangeComConrtb();
private:
	void SetQueEditStatus(const BOOL hisflag);
	void SetConsultEditStatus(const BOOL hisFlag);
public:
	typedef struct
	{
		int serialID;//唯一编号
		CString throughName;//通屏编号
	}ThroughWndInfo;
	//诊室通屏com
	CComboBox m_com_throughWnd;
	std::map<int,ThroughWndInfo> m_mapto_throughWnd;
	void FlushMapToThroughWnd();
	afx_msg void OnCbnSelchangeComThroughid();
};
