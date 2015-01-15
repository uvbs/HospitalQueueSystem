// PropSheetConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PropSheetConfig.h"


// CPropSheetConfig

IMPLEMENT_DYNAMIC(CPropSheetConfig, CPropertySheet)

CPropSheetConfig::CPropSheetConfig(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_PageComp);
	AddPage(&m_PageSound);
	AddPage(&m_PageDisplay);
	AddPage(&m_PageCom);
	AddPage(&m_PagePrint);
}

CPropSheetConfig::CPropSheetConfig(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_PageComp);
	AddPage(&m_PageSound);
	AddPage(&m_PageDisplay);
	AddPage(&m_PageCom);
	AddPage(&m_PagePrint);
}

CPropSheetConfig::~CPropSheetConfig()
{
}


BEGIN_MESSAGE_MAP(CPropSheetConfig, CPropertySheet)
END_MESSAGE_MAP()


// CPropSheetConfig 消息处理程序
