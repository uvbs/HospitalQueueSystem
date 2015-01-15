#pragma once

#include "./com/DataDef.h"
typedef enum{
	enumSamllScreen = 1,
	enumComprehScreen = 2,
	enumThroughScreen = 3,
	enumCallerScreen = 4
}ScrrenType;

class CSendToHardWare
{
public:
	CSendToHardWare(void);
	~CSendToHardWare(void);
public:
	static BOOL SendToHardWare(ScrrenType scrrenType,int address,const CString msg,const CString IP=_T(""),USHORT port=0, int channel=-1,CmdType cmdType=callerCmdShowFail);
};

