#include "StdAfx.h"
#include "BaseConfig.h"
#include "MyGetFile.h"

CBaseConfig::CBaseConfig(void)
{
	CString strPath=mygetfile.GetExeFullFilePath();
	createDirectory(strPath+_T("\\SYS"));
	m_strPath=strPath+_T("\\SYS\\sys.ini");
	m_hisPath=strPath+_T("\\SYS\\his.ini");
	m_flushTimePath=strPath+_T("\\SYS\\sysconfig.ini");
}

CBaseConfig::~CBaseConfig(void)
{
}

void CBaseConfig::Save()
{
	WritePrivateProfileString(_T("sys"),_T("DATABASE"),m_strDataDrive,m_strPath);
	WritePrivateProfileString(_T("sys"),_T("BASEIP"),m_serverIP,m_strPath);
	WritePrivateProfileString(_T("sys"),_T("BASEACOUNT"),m_serverAcount,m_strPath);
	WritePrivateProfileString(_T("sys"),_T("BASEPASSWORD"),m_serverPassword,m_strPath);
}
void CBaseConfig::SaveHisData()
{
	WritePrivateProfileString(_T("his"),_T("DATABASE"),m_hisDataDrive,m_hisPath);
	WritePrivateProfileString(_T("his"),_T("BASEIP"),m_hisIP,m_hisPath);
	WritePrivateProfileString(_T("his"),_T("BASEACOUNT"),m_hisAcount,m_hisPath);
	WritePrivateProfileString(_T("his"),_T("BASEPASSWORD"),m_hisPass,m_hisPath);
	WritePrivateProfileString(_T("his"),_T("BASEPORT"),m_hisPort,m_hisPath);
	WritePrivateProfileString(_T("his"),_T("BASENAME"),m_hisBaseName,m_hisPath);
}
CString CBaseConfig::GetDataDrive()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("sys"),_T("DATABASE"),NULL,wbuf,255,m_strPath);
	m_strDataDrive.Format(_T("%s"),wbuf);
	return m_strDataDrive;
}

CString CBaseConfig::GetServerAcount()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("sys"),_T("BASEACOUNT"),NULL,wbuf,255,m_strPath);
	m_serverAcount.Format(_T("%s"),wbuf);
	return m_serverAcount;
}

CString CBaseConfig::GetServerPassword()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("sys"),_T("BASEPASSWORD"),NULL,wbuf,255,m_strPath);
	m_serverPassword.Format(_T("%s"),wbuf);
	return m_serverPassword;
}

CString CBaseConfig::GetServerIP()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("sys"),_T("BASEIP"),NULL,wbuf,255,m_strPath);
	m_serverIP.Format(_T("%s"),wbuf);
	return m_serverIP;
}

CString CBaseConfig::GetComNum()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("sys"),_T("COM"),NULL,wbuf,255,m_strPath);
	m_comNum.Format(_T("%s"),wbuf);
	return m_comNum;
}

CString CBaseConfig::GetBaudRate()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("sys"),_T("BAUdRATE"),NULL,wbuf,255,m_strPath);
	m_baudRate.Format(_T("%s"),wbuf);
	return m_baudRate;
}

CString CBaseConfig::GetWndComNum()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("sys"),_T("WNDCOM"),NULL,wbuf,255,m_strPath);
	m_wndCom.Format(_T("%s"),wbuf);
	return m_wndCom;
}

void CBaseConfig::SaveComData()
{
	WritePrivateProfileString(_T("sys"),_T("COM"),m_comNum,m_strPath);
	WritePrivateProfileString(_T("sys"),_T("WNDCOM"),m_wndCom,m_strPath);
	WritePrivateProfileString(_T("sys"),_T("BAUDRATE"),m_baudRate,m_strPath);
}


CString CBaseConfig::GetHisDataDrive()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("his"),_T("DATABASE"),NULL,wbuf,255,m_hisPath);
	m_hisDataDrive.Format(_T("%s"),wbuf);
	return m_hisDataDrive;
}

CString CBaseConfig::GetHisAcount()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("his"),_T("BASEACOUNT"),NULL,wbuf,255,m_hisPath);
	m_hisAcount.Format(_T("%s"),wbuf);
	return m_hisAcount;
}

CString CBaseConfig::GetHisServerIP()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("his"),_T("BASEIP"),NULL,wbuf,255,m_hisPath);
	m_hisIP.Format(_T("%s"),wbuf);
	return m_hisIP;
}

CString CBaseConfig::GetHisPass()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("his"),_T("BASEPASSWORD"),NULL,wbuf,255,m_hisPath);
	m_hisPass.Format(_T("%s"),wbuf);
	return m_hisPass;
}

CString CBaseConfig::GetHisPort()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("his"),_T("BASEPORT"),NULL,wbuf,255,m_hisPath);
	m_hisPort.Format(_T("%s"),wbuf);
	return m_hisPort;
}

CString CBaseConfig::GetHisBaseName()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("his"),_T("BASENAME"),NULL,wbuf,255,m_hisPath);
	m_hisBaseName.Format(_T("%s"),wbuf);
	return m_hisBaseName;
}

CString CBaseConfig::GetFlushTimeSec()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("time"),_T("FLUSHTIMESEC"),NULL,wbuf,255,m_flushTimePath);
	m_flushTimeSec.Format(_T("%s"),wbuf);
	if(m_flushTimeSec.IsEmpty())
	{
		m_flushTimeSec = _T("30");
	}
	return m_flushTimeSec;
}

void CBaseConfig::SaveFlushTimeSec()
{
	WritePrivateProfileString(_T("time"),_T("FLUSHTIMESEC"),m_flushTimeSec,m_flushTimePath);
}