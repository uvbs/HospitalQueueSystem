#pragma once
#ifndef _MY_MAX_IP_LENGTH
#define _MY_MAX_IP_LENGTH	64
#endif
// MySocketUDP ÃüÁîÄ¿±ê
//typedef void ( *PCALLBACKFUNC ) (void* pData, sockaddr_in* pAddr);
#include "afxwin.h"
#include "WinSock2.h"

class MySocketUDP
{
public:
	MySocketUDP();
	virtual ~MySocketUDP();

	BOOL StartSocket(USHORT uPort = 0);
	BOOL ReBind(USHORT uPort);
	BOOL CloseSocket();
	BOOL SendTo( CHAR * pData, UINT nLen, CString szIP);
	BOOL SendTo( CHAR * pData, UINT nLen, CString szIP, USHORT uPort);
	BOOL SendTo( CHAR * pData, UINT nLen, LPSOCKADDR_IN pSockaddr_in);
	BOOL Broadcast(CHAR * pData, UINT nLen);
	BOOL Broadcast(CHAR * pData, UINT nLen, USHORT uPort);
	BOOL RecvFrom(CHAR * buf, int iLen, LPSOCKADDR_IN from = NULL);

	USHORT NetToHostShort(USHORT uNetShort);
	CString NetToHostIP(IN_ADDR& in_addrNet);

	int GetLastError();
	void ShowSocketError();

	//virtual void OnRecvData() = 0;
	virtual void OnRecvData(){}

	void WriteLogWithTime(CString strSockLog);
	void WriteErrLog(CString strErr);
	BOOL AppendWriteFile(CString strText, CString strFileName);

	USHORT GetPort(){return m_uPort;}

private:
	BOOL Startup();
	BOOL CreateSocket();
	BOOL Bind();
	BOOL _CloseSocket();
	BOOL Cleanup();
	BOOL StartPeekRecvThread();
	BOOL KillPeekRecvThread();

private:
	USHORT m_uPort;
	SOCKET m_uSocket;
	//SOCKET m_uSocketBroadcast;
	USHORT m_uMaxUdpDg;
	BOOL m_bIsCreate;

	CString m_strLogFilePath;

	static UINT PeekRecvThreadProc(LPVOID pParam);
	CWinThread* m_pPeekRecvThreadProc;
	BOOL m_bPeekRecvThreadProc;
};


