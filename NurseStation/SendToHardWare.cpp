#include "StdAfx.h"
#include "SendToHardWare.h"
#include "com/SLZCWndScreen.h"
#include "com/SLZCCaller.h"
#include "NurseStation.h"
#include "MyString.h"
#include "MyCommon.h"


//CSendToHardWare::m_left=MyCommon::GetProfileInt(_T("sys"),_T("LEFT"),0,_T("sys\\VisonShow.ini"));
//CSendToHardWare::m_right=MyCommon::GetProfileInt(_T("sys"),_T("RIGHT"),256,_T("sys\\VisonShow.ini"));
//CSendToHardWare::m_top = MyCommon::GetProfileInt(_T("sys"),_T("TOP"),0,_T("sys\\VisonShow.ini"));
//CSendToHardWare::m_bottom = MyCommon::GetProfileInt(_T("sys"),_T("BOTTOM"),64,_T("sys\\VisonShow.ini"));

CSendToHardWare::CSendToHardWare(void)
{
	
}

CSendToHardWare::~CSendToHardWare(void)
{
}


BOOL CSendToHardWare::SendToHardWare(ScrrenType scrrenType,int address,const CString msg,const CString IP,USHORT port,int channel,CmdType cmdType)
{
	SLZCWndScreen* pSlzWndScreen = SLZCWndScreen::GetInstance();
	SLZCCaller* pSlzCaller = SLZCCaller::GetInstance();
	switch(scrrenType)
	{
	case enumSamllScreen:
		pSlzWndScreen->AddScreenMsg(msg,address);
		break;
	case enumComprehScreen:
		pSlzWndScreen->AddScreenMsg(msg,address);
		break;
	case enumThroughScreen:
		{
			CString strTemp = msg;
			strTemp.Replace(_T("#0#"),_T(""));
			strTemp.Replace(_T("#1#"),_T(""));
			strTemp.Replace(_T("#2#"),_T(""));
			pSlzWndScreen->AddThroughScreenMsg(strTemp,address,channel,IP,port);
			CRect rect;
			static UINT left=MyCommon::GetProfileInt(_T("sys"),_T("LEFT"),0,_T("sys\\VisonShow.ini"));
			static UINT right=MyCommon::GetProfileInt(_T("sys"),_T("RIGHT"),256,_T("sys\\VisonShow.ini"));
			static UINT top = MyCommon::GetProfileInt(_T("sys"),_T("TOP"),0,_T("sys\\VisonShow.ini"));
			static UINT bottom = MyCommon::GetProfileInt(_T("sys"),_T("BOTTOM"),64,_T("sys\\VisonShow.ini"));
			static UINT nRegion = MyCommon::GetProfileInt(_T("sys"),_T("REGION"),0,_T("sys\\VisonShow.ini"));
			rect.left = left; rect.right = right; rect.top = top; rect.bottom = bottom;
			theApp.m_pNurseWnd->m_pVisonShowLED->SendDataToScreen(strTemp,IP,rect,10*1000,nRegion);
		}
		break;
	case enumCallerScreen:
		{
			SLZCallerData data;
			data.SetAttchMsg(msg);
			data.SetCallerId(address);
			data.SetCmdType(cmdType);
			pSlzCaller->AddWriteCallerData(data);
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}