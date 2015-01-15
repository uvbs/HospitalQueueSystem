#pragma once

#include "PropPageConfigComp.h"
#include "PropPageConfigDisplay.h"
#include "PropPageConfigSound.h"
#include "PropPageConfigCom.h"
#include "PropPageConfigPrint.h"

// CPropSheetConfig

class CPropSheetConfig : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSheetConfig)

public:
	CPropSheetConfig(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSheetConfig(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPropSheetConfig();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CPropPageConfigComp m_PageComp;
	CPropPageConfigSound m_PageSound;
	CPropPageConfigDisplay m_PageDisplay;
	CPropPageConfigCom m_PageCom;
	CPropPageConfigPrint m_PagePrint;
};


