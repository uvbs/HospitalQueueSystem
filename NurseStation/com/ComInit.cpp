#include "StdAfx.h"
#include "ComInit.h"
#include "../DoFile.h"
#include "../CommonConvert.h"

CComInit::CComInit(void) :
m_hComReadCard(INVALID_HANDLE_VALUE)
,m_hComWndScreen(INVALID_HANDLE_VALUE)
{
	InitCom();
	for(int i=0;i<10;i++)
	{
		int result = TryCom(i);
		m_canUse[i] = result;
	}
}

CComInit::~CComInit(void)
{

}
/*
尝试打开串口，保存在数组中
*/
int CComInit::TryCom(const int nCom)
{
	if(nCom<0) return -1;
	if(m_hComWndScreen != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComWndScreen);
		m_hComWndScreen=INVALID_HANDLE_VALUE;
	}
	CString strWndCom;
	strWndCom.Format(_T("COM%d"),nCom);
	HANDLE hCom=CreateFile(strWndCom,GENERIC_READ | GENERIC_WRITE,0,
		NULL,OPEN_EXISTING,0,NULL);
	if(INVALID_HANDLE_VALUE != hCom)
	{
		CloseHandle(hCom);
		return nCom;
	}
	else
	{
		return -1;
	}
}
/*
初始化
*/
void CComInit::InitCom()
{
	CDoFile dofile;
	m_strPath = dofile.GetExeFullFilePath();
	m_strPath += _T("\\cominfo.ini");
}
/*
打开串口
*/
HANDLE CComInit::OpenComm(const int ncom)
{
	CString Com;
	Com.Format(_T("COM%d"),ncom);
	HANDLE hCom = CreateFile(Com,GENERIC_READ | GENERIC_WRITE,
		0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);

	if(INVALID_HANDLE_VALUE==hCom)
	{
		CloseHandle(hCom);
		return hCom;
	}
	//设置com事件类型，参见msdn
	SetCommMask(hCom,EV_RXCHAR | EV_TXEMPTY);
	SetupComm( hCom, 1024,512 );//设置缓冲区大小;
	PurgeComm( hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR
		| PURGE_RXCLEAR ); //清干净输入、输出缓冲区

	DCB dcb={0};
	dcb.DCBlength=sizeof(dcb);
	BOOL bres=GetCommState(hCom,&dcb);
	if(!bres)
	{
		CloseHandle(hCom);
		hCom=INVALID_HANDLE_VALUE;
		return hCom;
	}
	dcb.ByteSize=8;//后改
	dcb.fParity=0;
	dcb.BaudRate=CBR_9600;
	dcb.StopBits=ONESTOPBIT;

	bres=SetCommState(hCom,&dcb);
	if(!bres)
	{
		CloseHandle(hCom);
		hCom=INVALID_HANDLE_VALUE;
		return hCom;
	}
	COMMTIMEOUTS comm_time={0};
	comm_time.ReadIntervalTimeout=MAXDWORD;
	bres=SetCommTimeouts(hCom,&comm_time);
//	SetupComm(m_hComWndScreen, 4096, 1024);

	if(!bres)
	{
		CloseHandle(hCom);
		return hCom;
	}
	return hCom;
}
/*
打开呼叫器串口
*/
int CComInit::OpenWndScreen(const int ncom)
{
	m_cWndComm.Format(_T("%d"),ncom);
	if(m_hComWndScreen != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComWndScreen);
		m_hComWndScreen=INVALID_HANDLE_VALUE;
	}
	if(ncom== 0) return 0;
	m_hComWndScreen = OpenComm(ncom);
	if(m_hComWndScreen != INVALID_HANDLE_VALUE)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}
/*
打开刷卡器串口
*/
int CComInit::OpenCardComm(const int ncom)
{
	m_cCardComm.Format(_T("%d"),ncom);
	if(m_hComReadCard!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComReadCard);
		m_hComReadCard = INVALID_HANDLE_VALUE;
	}
	if(ncom==0)return 0;
	m_hComReadCard = OpenComm(ncom);
	if(m_hComReadCard != INVALID_HANDLE_VALUE)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}
/*
保存上次打开串口如:com1到文件
*/
void CComInit::SaveComm()
{
	WritePrivateProfileString(_T("com"),_T("CARDCOM"),m_cCardComm,m_strPath);
	WritePrivateProfileString(_T("com"),_T("WNDCOM"),m_cWndComm,m_strPath);
}
/*
读取刷卡器COM口
*/
CString CComInit::GetCardComm()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("com"),_T("CARDCOM"),NULL,wbuf,255,m_strPath);
	m_cCardComm.Format(_T("%s"),wbuf);
	return m_cCardComm;
}
/*
获取呼叫器和屏串口
*/
CString CComInit::GetWndComm()
{
	wchar_t wbuf[255];
	ZeroMemory(wbuf,255);
	GetPrivateProfileString(_T("com"),_T("WNDCOM"),NULL,wbuf,255,m_strPath);
	m_cWndComm.Format(_T("%s"),wbuf);
	return m_cWndComm; 
}

int CComInit::OpenCardComm()
{
	CString strCardCom = GetCardComm();
	int nCardCom = 0;
	CCommonConvert::CStringToint(nCardCom,strCardCom);
	return OpenCardComm(nCardCom);
}

int CComInit::OpenWndScreen()
{
	CString strWndScreen = GetWndComm();
	int nWndScreen = 0;
	CCommonConvert::CStringToint(nWndScreen,strWndScreen);
	return OpenWndScreen(nWndScreen);
}