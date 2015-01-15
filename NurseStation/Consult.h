#pragma once

// CConsult 命令目标

class CConsult : public CObject
{
public:
	CConsult();
	virtual ~CConsult();
	CConsult(const CConsult& obj);
	
	//	CConsult(const CConsult& obj);
	//	Base& operator=(Base& obj);
	CConsult& operator=(CConsult& obj);
	
	///诊室名称
	CString GetConsultName(){return m_ConsultName;}
	void SetConsultName(CString name){m_ConsultName=name;}
	///呼叫名称
	CString GetCallName(){return m_CallName;}
	void SetCallName(CString name){m_CallName=name;}
	///屏地址
	int GetShowAddress(){return m_ShowAddress;}
	void SetShowAddress(int address){m_ShowAddress=address;}
	///呼叫器地址
	int GetCallAddress(){return m_CallAddress;}
	void SetCallAddress(int address){m_CallAddress=address;}
	////综合屏地址
	int GetComprehAddress(){return m_ComprehAddress;}
	void SetComprehAddress(int address){m_ComprehAddress=address;}
	////通屏地址
	int GetTroughAddress(){return m_TroughAddress;}
	void SetTroughAddress(int address){m_TroughAddress=address;}
	////通屏IP
//	CString GetThroughIP(){return m_TroughIP;}
//	void SetTroughIp(CString IP){m_TroughIP=IP;}
	////诊室ID
	CString GetConsultID(){return m_ConsultID;}
	void SetConsultID(CString consultid){m_ConsultID=consultid;}
	////机顶盒ID
	void SetstdId(CString strStbId){m_stdId=strStbId;}
	CString GetstdId(){return m_stdId;}
	////
	void SetHisFlag(const BOOL flag){m_bHisFlag = flag;}
	BOOL GetHisFlag(){return m_bHisFlag;}
private:
	CString m_ConsultName;
	int m_CallAddress;
	CString m_CallName;
	int m_ShowAddress;
	int m_ComprehAddress;
//	CString m_TroughIP;
	int m_TroughAddress;
	CString m_ConsultID;
	///机顶盒ID
	CString m_stdId;
	BOOL m_bHisFlag;//标识，是否从HIS读出的数据
};


