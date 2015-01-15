// PropPrintSet.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PropPrintSet.h"


// CPropPrintSet

IMPLEMENT_DYNAMIC(CPropPrintSet, CPropertySheet)

CPropPrintSet::CPropPrintSet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	this->m_psh.dwFlags &= ~PSP_HASHELP;
	this->m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_printText.m_psp.dwFlags &= ~PSP_HASHELP;
	m_printPic.m_psp.dwFlags &= ~PSP_HASHELP;
	m_printShowTime.m_psp.dwFlags &= ~PSP_HASHELP;
	
	//m_pPrSetDlg = (CPrintSetDlg*) pParentWnd;
	m_pPrSetDlg = (CPropPageConfigPrint*) pParentWnd;

	if(m_pPrSetDlg->m_bIsForEdit)
	{
		switch(m_pPrSetDlg->m_trackContrl.m_pRightBnSelect->GetWindowType())
		{
		case enmStatic:
			if(!m_pPrSetDlg->m_trackContrl.m_pRightBnSelect->
				m_pTransStatic->IsForImage() && m_pPrSetDlg->
				m_trackContrl.m_pRightBnSelect->
				m_pTransStatic->GetIsShowTime())
			{
				AddPage(&m_printShowTime);
			}
			if(!m_pPrSetDlg->m_trackContrl.m_pRightBnSelect->
				m_pTransStatic->IsForImage() && !m_pPrSetDlg->
				m_trackContrl.m_pRightBnSelect->m_pTransStatic->
				GetIsShowQueNum() && !m_pPrSetDlg->m_trackContrl.
				m_pRightBnSelect->m_pTransStatic->GetIsShowTime())
			{
				AddPage(&m_printText);
			}
			if(m_pPrSetDlg->m_trackContrl.m_pRightBnSelect->
				m_pTransStatic->IsForImage())
			{
				AddPage(&m_printPic);
			}
			break;
		}
	}
	if(m_pPrSetDlg->m_bIsAddText)
	{
		AddPage(&m_printText);
	}
	if(m_pPrSetDlg->m_bIsAddPicture)
	{
		AddPage(&m_printPic);
	}
	if(m_pPrSetDlg->m_bIsShowTime)
	{
		AddPage(&m_printShowTime);
	}
	/*
	if(m_pView->m_isShowTime)
	{
		AddPage(&m_propShowTime);
	}
	*/
}

CPropPrintSet::CPropPrintSet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	this->m_psh.dwFlags &= ~PSP_HASHELP;
	this->m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_printText.m_psp.dwFlags &= ~PSP_HASHELP;
	m_printPic.m_psp.dwFlags &= ~PSP_HASHELP;
	m_printShowTime.m_psp.dwFlags &= ~PSP_HASHELP;
	
	//m_pPrSetDlg = (CPrintSetDlg*) pParentWnd;
	m_pPrSetDlg = (CPropPageConfigPrint*) pParentWnd;
	
	if(m_pPrSetDlg->m_bIsForEdit)
	{
		switch(m_pPrSetDlg->m_trackContrl.m_pRightBnSelect->GetWindowType())
		{
		case enmStatic:
			if(!m_pPrSetDlg->m_trackContrl.m_pRightBnSelect->
				m_pTransStatic->IsForImage() && m_pPrSetDlg->
				m_trackContrl.m_pRightBnSelect->
				m_pTransStatic->GetIsShowTime())
			{
				 AddPage(&m_printShowTime);
			}
			if(!m_pPrSetDlg->m_trackContrl.m_pRightBnSelect->
				m_pTransStatic->IsForImage() && !m_pPrSetDlg->
				m_trackContrl.m_pRightBnSelect->m_pTransStatic->
				GetIsShowQueNum() && !m_pPrSetDlg->m_trackContrl.
				m_pRightBnSelect->m_pTransStatic->GetIsShowTime())
			{
				AddPage(&m_printText);
			}
			if(m_pPrSetDlg->m_trackContrl.m_pRightBnSelect->
				m_pTransStatic->IsForImage())
			{
				AddPage(&m_printPic);
			}
			break;
		}
	}
	if(m_pPrSetDlg->m_bIsAddText)
	{
		AddPage(&m_printText);
	}
	if(m_pPrSetDlg->m_bIsAddPicture)
	{
		AddPage(&m_printPic);
	}
	if(m_pPrSetDlg->m_bIsShowTime)
	{
		AddPage(&m_printShowTime);
	}
}

CPropPrintSet::~CPropPrintSet()
{
}


BEGIN_MESSAGE_MAP(CPropPrintSet, CPropertySheet)
END_MESSAGE_MAP()

// CPropPrintSet 消息处理程序
