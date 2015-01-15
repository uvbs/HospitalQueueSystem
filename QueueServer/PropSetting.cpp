// PropSetting.cpp : 实现文件
//

#include "stdafx.h"
//#include "NurseStation.h"
#include "PropSetting.h"


// CPropSetting

IMPLEMENT_DYNAMIC(CPropSetting, CPropertySheet)

CPropSetting::CPropSetting(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->m_psh.dwFlags &= ~(PSH_HASHELP);

	m_PropConnectToHisDbDlg.m_psp.dwFlags &=~(PSP_HASHELP);
	//m_HisDsnPropDlg.m_psp.dwFlags &=~(PSP_HASHELP);
	m_MyDbPropDlg.m_psp.dwFlags &=~(PSP_HASHELP);
	//m_PropSocket.m_psp.dwFlags &=~(PSP_HASHELP);

	AddPage(&m_PropConnectToHisDbDlg);
	//AddPage(&m_HisDsnPropDlg);
	AddPage(&m_MyDbPropDlg);
	//AddPage(&m_PropSocket);
}

CPropSetting::CPropSetting(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->m_psh.dwFlags &= ~(PSH_HASHELP);

	m_PropConnectToHisDbDlg.m_psp.dwFlags &=~(PSP_HASHELP);
	//m_HisDsnPropDlg.m_psp.dwFlags &=~(PSP_HASHELP);
	m_MyDbPropDlg.m_psp.dwFlags &=~(PSP_HASHELP);
	//m_PropSocket.m_psp.dwFlags &=~(PSP_HASHELP);

	AddPage(&m_PropConnectToHisDbDlg);
	//AddPage(&m_HisDsnPropDlg);
	AddPage(&m_MyDbPropDlg);
	//AddPage(&m_PropSocket);
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
