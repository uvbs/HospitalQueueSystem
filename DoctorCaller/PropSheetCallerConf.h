#pragma once
#include "PropPageDoctorInfo.h"
#include "PropPageDoctorPassword.h"
#include "PropPageQueueChange.h"


// CPropSheetCallerConf

class CPropSheetCallerConf : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSheetCallerConf)

public:
	CPropSheetCallerConf(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSheetCallerConf(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPropSheetCallerConf();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CPropPageDoctorInfo* m_pPropPageDoctorInfo;
	CPropPageDoctorPassword* m_pPropPageDoctorPassword;
	//CPropPageQueueChange* m_pPropPageQueueChange;
};


