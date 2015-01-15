#pragma once

#include "MyFile.h"
#include "MyGetFile.h"
class CBaseConfig
{
public:
	CBaseConfig(void);
	~CBaseConfig(void);
public:
	CString GetServerIP();
	void SetServerIP(const CString& ip){m_serverIP=ip;}
	CString GetServerAcount();
	void SetServerAcount(const CString& acount){m_serverAcount=acount;}
	CString GetServerPassword();
	void SetServerPassword(const CString& password){m_serverPassword=password;}
	void Save();
	void SetDataDrive(CString dataDrive){m_strDataDrive=dataDrive;}
	CString GetDataDrive();
	void SetComNum(CString num){m_comNum=num;}
	
	void SetBaudRate(CString baudrate){m_baudRate=baudrate;}
	CString GetBaudRate();
	CString GetComNum();
	
	void SetWndComNum(CString num){m_wndCom=num;}
	CString GetWndComNum();
	void SaveComData();

	CString GetHisServerIP();
	void SetHisServerIP(const CString& ip){m_hisIP=ip;}
	CString GetHisPass();
	void SetHisPass(const CString& pass){m_hisPass=pass;}
	CString GetHisAcount();
	void SetHisAcount(const CString& account){m_hisAcount=account;}
	CString GetHisDataDrive();
	void SetHisDataDrive(const CString& dataDrive){m_hisDataDrive=dataDrive;}
	void SaveHisData();
	void SetHisPort(const CString& port){m_hisPort=port;}
	CString GetHisPort();
	void SetHisBaseName(const CString& basename){m_hisBaseName=basename;}//连接的数据库名
	CString GetHisBaseName();
	CString GetFlushTimeSec();
	void SetFlushTimeSec(CString timesec){m_flushTimeSec=timesec;}

	void SaveFlushTimeSec();
private:
	CString m_serverIP;
	CString m_serverAcount;
	CString m_serverPassword;
	CString m_strPath;
	CString m_strDataDrive;
	CMyGetFile mygetfile;
	CString m_comNum;
	CString m_baudRate;
	CString m_wndCom;
	CString m_hisIP;
	CString m_hisAcount;
	CString m_hisPass;
	CString m_hisDataDrive;
	CString m_hisPath;
	CString m_hisPort;
	CString m_hisBaseName;
	CString m_flushTimePath;
	CString m_flushTimeSec;
};
