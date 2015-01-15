#include "StdAfx.h"
#include "Account.h"


void CAccount::SetUserRegtime()
{
	CTime time=CTime::GetCurrentTime();
	m_regTime=DayTimetoString(time);
}

CString CAccount::DayTimetoString(const CTime &time)
{
	CString str;
	try
	{
		str.Format(TEXT("%04d-%02d-%02d %02d:%02d:%02d"),
			time.GetYear(), time.GetMonth(),time.GetDay(),
			time.GetHour(), time.GetMinute(), time.GetSecond() );
	}
	catch(...)
	{
		return _T("");
	}
	return str;
}


CAccount::CAccount()
{
	SetUserRegtime();
}
CAccount::~CAccount()
{

}

void CAccount::SetUserID()
{
	m_userID=GetOnlyID();
}


void CAccount::SetOfficeID(::std::vector<CString> str)
{
	int size=str.size();
	m_DoOfficeID.clear();
	
	for(int i=0;i<size;i++)
	{		
		m_DoOfficeID.push_back(str[i]);
	}
}

void CAccount::DeleteDoOfficeVector()
{
	m_DoOfficeID.clear();
}

void CAccount::AddOfficeID(CString str)
{
	m_DoOfficeID.push_back(str);
}

int CAccount::GetSrand(const int &max)
{
	time_t t;	
	srand((unsigned) time(&t));	
	return	rand() % max;
}

CString CAccount::GetOnlyID()
{
	CString m_id;
	CTime now=CTime::GetCurrentTime();
	static int nCount;
	m_id.Format(_T("%04d%02d%02d%02d%02d%02d%04d%04d"),
		now.GetYear(),now.GetMonth(),now.GetDay(),now.GetHour(),
		now.GetSecond(),now.GetMinute(),GetSrand(9999),++nCount);
	return m_id;
}