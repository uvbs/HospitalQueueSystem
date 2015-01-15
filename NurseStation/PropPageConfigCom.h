#pragma once

#include "PoolManage.h"
#include "BaseConfig.h"
#include "com/ComInit.h"

// CPropPageConfigCom 对话框

class CPropPageConfigCom : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigCom)

public:
	CPropPageConfigCom();
	virtual ~CPropPageConfigCom();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_CONFIG_COM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	CComboBox m_com_comnum;
	CComboBox m_com_baudrate;
	CPoolManage m_pool;
	CBaseConfig m_baseconfig;
	CComboBox m_com_wndscrren;
	CString m_ed_wndaddress;
	CString m_cs_msg;
	CComInit* m_pComInit;

	afx_msg void OnCbnSelchangeComboComnum();
	afx_msg void OnCbnSelchangeComboBaudrate();
	afx_msg void OnCbnSelchangeComWndscreen();
	afx_msg void OnBnClickedBnSend();
	virtual BOOL OnApply();
};
