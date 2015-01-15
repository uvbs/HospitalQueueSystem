#pragma once

class CCallerInterface
{
public:
	~CCallerInterface(void);
	static CCallerInterface* GetInstance();
private:
	CCallerInterface(void);
private:
	static DWORD WINAPI DoCallerCmd(LPVOID pParam);
	HANDLE m_hDoCallerCmd;
	BOOL Init();
	CString GetDoctorId(int callerAddress);
};
