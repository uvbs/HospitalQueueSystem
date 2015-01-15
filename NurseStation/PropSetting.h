#pragma once



// CPropSetting
#include "PropQueSettting.h"

#include "UserSetting.h"

#include "DoctorManageDlg.h"
#include "PropConnectToHisSet.h"
//#include "CompConfPropDlg.h"
#include "WaitingRoomConfPropDlg.h"
#include "StbPropDlg.h"
#include "PropDlgThroughLED.h"


class CPropSetting : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSetting)

public:
	CPropSetting(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSetting(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPropSetting();

	//	CPrintSetting m_propprint;//打印
	CUserSetting m_propuser;//用户
	CPropQueSettting m_propque;//科室设置
	//	CPropConsulSetting m_propconsult;//诊室
	CDoctorManageDlg m_DoctorManageDlg;//医生设置
	//CPropConnectToHisSet m_propConnectToHis;
	//CCompConfPropDlg m_CompConfPropDlg;
	//	CPropWaittingRoom m_propWait;
	//CWaitingRoomConfPropDlg m_WaitingRoomConfPropDlg;
	CStbPropDlg m_StbPropDlg;//机顶盒设置
//	CPropThroughWnd m_propThroughWnd;//同频设置
	CPropDlgThroughLED m_PropDlgThroughLED;
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	HICON    m_hIcon;
};


