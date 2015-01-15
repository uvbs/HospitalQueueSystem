#pragma once

// CPropSetting
#include "HisDsnPropDlg.h"
#include "MyDbPropDlg.h"
#include "PropSocket.h"
#include "PropConnectToHisDbDlg.h"

class CPropSetting : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSetting)

public:
	CPropSetting(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSetting(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPropSetting();
	
protected:
	CPropConnectToHisDbDlg m_PropConnectToHisDbDlg;
	//CHisDsnPropDlg m_HisDsnPropDlg;
	CMyDbPropDlg m_MyDbPropDlg;
	//CPropSocket m_PropSocket;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	HICON    m_hIcon;
};


