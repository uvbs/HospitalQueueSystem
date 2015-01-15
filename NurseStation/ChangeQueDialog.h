#pragma once

#include "afxwin.h"
#include "PoolManage.h"
#include "ado2.h"
#include "BaseConfig.h"
// CChangeQueDialog 对话框


class CChangeQueDialog : public CDialog
{
	DECLARE_DYNAMIC(CChangeQueDialog)

public:
	CChangeQueDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChangeQueDialog();

// 对话框数据
	enum { IDD = IDD_CHANGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	CEdit m_rd_patname;
//	CEdit m_rd_lastque;
//	CComboBox m_com_toque;
	virtual BOOL OnInitDialog();
	afx_msg HRESULT OnReadMsg(WPARAM wParam,LPARAM lParam);

//	CString m_cs_patname;
//	CString m_cs_lastque;
	CString m_paitentlogid;
	CADODatabase* m_pbase;
	
	//BOOL ConnectToBase();

	CString m_baseDrive;
	CString m_baseIP;
	CString m_baseAccount;
	CString m_basePassWord;
	CBaseConfig m_BaseConfig;
	void SearchOffice();
	afx_msg void OnBnClickedOk();
	CPoolManage m_pool;

	int GetMaxQuenumber();
	CString GetCallID();

	CString m_selectofficeid;
//	CComboBox m_ch_setdoctor;
	CComboBox m_com_toque;

	//void SearchDoctor();
	afx_msg void OnCbnSelchangeComboToque();

//	void SearchWaitRoom();

// 	typedef struct _tagWaitRoomInfo
// 	{
// 		CString waitid;
// 		CString waitName;
// 		CString stb_id;
// 	}WAITROOMINFO,*LPWAITROOMINFO;
// 
// 	CMap<int,int,WAITROOMINFO,WAITROOMINFO&> m_map_waitinfo;
	HICON m_hIcon;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCancel();
};
