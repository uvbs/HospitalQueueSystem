#pragma once
#include "DataDef.h"

class SLZCallerData
{
public:
	SLZCallerData(void) : m_iCallerId(0){}
	~SLZCallerData(void){}

	//获取呼叫器地址
	int GetCallerId() const{return m_iCallerId;}
	//设置呼叫器地址
	void SetCallerId(int iCallerId){m_iCallerId = iCallerId;}
	//获取指令类型
	CmdType GetCmdType() const{return m_cmdtype;}
	//设置指令类型
	void SetCmdType(CmdType cmdtype){m_cmdtype = cmdtype;}
	CString GetAttchMsg() const {return m_attchMsg;}
	void SetAttchMsg(const CString& msg){m_attchMsg = msg;}
private:
	int m_iCallerId;	//呼叫器地址
	CmdType m_cmdtype;		//指令类型
	CString m_attchMsg;//附加信息如呼叫：1234号
};
