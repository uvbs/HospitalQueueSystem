// Consult.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "Consult.h"


// CConsult

CConsult::CConsult()
{
	m_ConsultName=_T("");
	m_CallAddress=-1;
	m_CallName=_T("");
	m_ShowAddress=-1;
	m_ComprehAddress=-1;
//	m_TroughIP=_T("");
	m_TroughAddress=-1;
	m_ConsultID=_T("0");
	m_bHisFlag = FALSE;
}

CConsult::~CConsult()
{
}

CConsult::CConsult(const CConsult& obj)
{
	
	m_ConsultName=obj.m_ConsultName;
	m_CallAddress=obj.m_CallAddress;
	m_CallName=obj.m_CallName;

	m_ShowAddress=obj.m_ShowAddress;
	m_ComprehAddress=obj.m_ComprehAddress;
//	m_TroughIP=obj.m_TroughIP;
	m_TroughAddress=obj.m_TroughAddress;
	m_ConsultID=obj.m_ConsultID;
	m_stdId = obj.m_stdId;
	m_bHisFlag = obj.m_bHisFlag;
}

CConsult& CConsult::operator=(CConsult& obj)
{
	m_ConsultName=obj.m_ConsultName;
	m_CallAddress=obj.m_CallAddress;
	m_CallName=obj.m_CallName;

	m_ShowAddress=obj.m_ShowAddress;
	m_ComprehAddress=obj.m_ComprehAddress;
//	m_TroughIP=obj.m_TroughIP;
	m_TroughAddress=obj.m_TroughAddress;
	m_ConsultID=obj.m_ConsultID;
	m_stdId = obj.m_stdId;
	m_bHisFlag = obj.m_bHisFlag;
	return *this;
}


// CConsult 成员函数
