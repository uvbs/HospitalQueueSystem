#pragma once


#include <vector>
class CAccount
{
public:
	CAccount(void);
	~CAccount(void);
	void SetUserPassword(CString& password){m_password=password;}
	CString GetUserPassword(){return m_password;}

	

	void SetUserRegtime();
	void SetUserRegtime(CString& time){m_regTime=time;}
	CString GetUserRegtime(){return m_regTime;}

	void SetUserPower(int power){m_nPower=power;}
	int GetUserPower(){return m_nPower;}

	CString DayTimetoString(const CTime &time);

	void SetUserID();
	void SetUserID(CString& id){m_userID=id;}
	CString GetUserID(){return m_userID;}

	CString GetNurseName(){return m_nursename;}
	void SetNurseName(CString& nursename){m_nursename=nursename;}

	void SetOfficeID(std::vector<CString> str);
	void DeleteDoOfficeVector();
	void AddOfficeID(CString str);

	int GetSrand(const int &max);
	CString GetOnlyID();
protected:
	CString m_password;
	CString m_userID;
	int m_nPower;
	CString m_regTime;
	CString m_nursename;
	
public:
	std::vector<CString> m_DoOfficeID;
};
