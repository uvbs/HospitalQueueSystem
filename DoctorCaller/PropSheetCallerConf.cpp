// PropSheetCallerConf.cpp : 实现文件
//

#include "stdafx.h"
#include "DoctorCaller.h"
#include "PropSheetCallerConf.h"
#include "DoctorCallerDlg.h"


// CPropSheetCallerConf

IMPLEMENT_DYNAMIC(CPropSheetCallerConf, CPropertySheet)

CPropSheetCallerConf::CPropSheetCallerConf(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	this->m_psh.dwFlags &= ~(PSH_HASHELP);

	m_pPropPageQueueChange = new CPropPageQueueChange(
		((CDoctorCallerDlg*)pParentWnd)->GetController()->GetUserId(),
		((CDoctorCallerDlg*)pParentWnd)->GetController()->m_strConn,
		((CDoctorCallerDlg*)pParentWnd)->GetController()->GetNurseId());
	m_pPropPageQueueChange->m_psp.dwFlags &= ~(PSP_HASHELP);

	AddPage(m_pPropPageQueueChange);

	m_pPropPageDoctorInfo = new CPropPageDoctorInfo(
		((CDoctorCallerDlg*)pParentWnd)->GetController()->GetUserId(),
		((CDoctorCallerDlg*)pParentWnd)->GetController()->m_strConn);
	this->m_psh.dwFlags &= ~(PSH_HASHELP);
	m_pPropPageDoctorInfo->m_psp.dwFlags &= ~(PSP_HASHELP);

	AddPage(m_pPropPageDoctorInfo);

	m_pPropPageDoctorPassword = new CPropPageDoctorPassword(
		((CDoctorCallerDlg*)pParentWnd)->GetController()->GetUserId(),
		((CDoctorCallerDlg*)pParentWnd)->GetController()->m_strConn);
	m_pPropPageDoctorPassword->m_psp.dwFlags &= ~(PSP_HASHELP);

	AddPage(m_pPropPageDoctorPassword);
}

CPropSheetCallerConf::CPropSheetCallerConf(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	this->m_psh.dwFlags &= ~(PSH_HASHELP);

	m_pPropPageQueueChange = new CPropPageQueueChange(
		((CDoctorCallerDlg*)pParentWnd)->GetController()->GetUserId(),
		((CDoctorCallerDlg*)pParentWnd)->GetController()->m_strConn,
		((CDoctorCallerDlg*)pParentWnd)->GetController()->GetNurseId());
	m_pPropPageQueueChange->m_psp.dwFlags &= ~(PSP_HASHELP);

	AddPage(m_pPropPageQueueChange);

	m_pPropPageDoctorInfo = new CPropPageDoctorInfo(
		((CDoctorCallerDlg*)pParentWnd)->GetController()->GetUserId(),
		((CDoctorCallerDlg*)pParentWnd)->GetController()->m_strConn);
	m_pPropPageDoctorInfo->m_psp.dwFlags &= ~(PSP_HASHELP);

	AddPage(m_pPropPageDoctorInfo);

	m_pPropPageDoctorPassword = new CPropPageDoctorPassword(
		((CDoctorCallerDlg*)pParentWnd)->GetController()->GetUserId(),
		((CDoctorCallerDlg*)pParentWnd)->GetController()->m_strConn);
	m_pPropPageDoctorPassword->m_psp.dwFlags &= ~(PSP_HASHELP);

	AddPage(m_pPropPageDoctorPassword);
}

CPropSheetCallerConf::~CPropSheetCallerConf()
{
}


BEGIN_MESSAGE_MAP(CPropSheetCallerConf, CPropertySheet)
END_MESSAGE_MAP()


// CPropSheetCallerConf 消息处理程序
