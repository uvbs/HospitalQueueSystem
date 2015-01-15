// PropSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PropSetting.h"


// CPropSetting

IMPLEMENT_DYNAMIC(CPropSetting, CPropertySheet)

CPropSetting::CPropSetting(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->m_psh.dwFlags &= ~(PSH_HASHELP);

	m_propuser.m_psp.dwFlags &= ~(PSP_HASHELP);
	m_propque.m_psp.dwFlags &=~(PSP_HASHELP);

	m_DoctorManageDlg.m_psp.dwFlags &=~(PSP_HASHELP);
	//m_propConnectToHis.m_psp.dwFlags &=~(PSP_HASHELP);
	//m_CompConfPropDlg.m_psp.dwFlags &=~(PSP_HASHELP);
	//m_WaitingRoomConfPropDlg.m_psp.dwFlags &=~(PSP_HASHELP);
	m_StbPropDlg.m_psp.dwFlags &=~(PSP_HASHELP);
//	m_propThroughWnd.m_psp.dwFlags &= ~(PSP_HASHELP);
//	m_propWait.m_psp.dwFlags &=~(PSP_HASHELP);
	m_PropDlgThroughLED.m_psp.dwFlags &=~(PSP_HASHELP);
	
	
	AddPage(&m_propuser);
	AddPage(&m_propque);
	
	AddPage(&m_DoctorManageDlg);
	//AddPage(&m_CompConfPropDlg);
	AddPage(&m_StbPropDlg);
//	AddPage(&m_propThroughWnd);
	//AddPage(&m_propConnectToHis);
	//AddPage(&m_WaitingRoomConfPropDlg);
//	AddPage(&m_propWait);

	AddPage(&m_PropDlgThroughLED);

	theApp.m_pUserSetting=&m_propuser;
	theApp.m_pPropQueSetting=&m_propque;
}

CPropSetting::CPropSetting(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->m_psh.dwFlags &= ~(PSH_HASHELP);

	m_propuser.m_psp.dwFlags &= ~(PSP_HASHELP);
	m_propque.m_psp.dwFlags &= ~(PSP_HASHELP);

	m_DoctorManageDlg.m_psp.dwFlags &=~(PSP_HASHELP);
	//m_propConnectToHis.m_psp.dwFlags &=~(PSP_HASHELP);
	//m_CompConfPropDlg.m_psp.dwFlags &=~(PSP_HASHELP);
	//m_WaitingRoomConfPropDlg.m_psp.dwFlags &=~(PSP_HASHELP);
	m_StbPropDlg.m_psp.dwFlags &=~(PSP_HASHELP);
//	m_propThroughWnd.m_psp.dwFlags &= ~(PSP_HASHELP);
//	m_propWait.m_psp.dwFlags &=~(PSP_HASHELP);

	m_PropDlgThroughLED.m_psp.dwFlags &=~(PSP_HASHELP);

	AddPage(&m_propuser);
	AddPage(&m_propque);
	
	AddPage(&m_DoctorManageDlg);
	//AddPage(&m_CompConfPropDlg);
	AddPage(&m_StbPropDlg);
//	AddPage(&m_propThroughWnd);
	//AddPage(&m_propConnectToHis);
	//AddPage(&m_WaitingRoomConfPropDlg);
//	AddPage(&m_propWait);

	AddPage(&m_PropDlgThroughLED);

	theApp.m_pUserSetting=&m_propuser;
	theApp.m_pPropQueSetting=&m_propque;
}

CPropSetting::~CPropSetting()
{

}


BEGIN_MESSAGE_MAP(CPropSetting, CPropertySheet)
END_MESSAGE_MAP()


// CPropSetting 消息处理程序

BOOL CPropSetting::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	ModifyStyle(0, WS_MINIMIZEBOX);
	
	return bResult;
}
