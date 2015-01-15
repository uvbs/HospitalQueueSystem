// MySocketUDP.cpp : 实现文件
//

#include "stdafx.h"
//#include "NurseStation.h"
#include "MySocketUDP.h"
#include "MyCommon.h"
#include "MyString.h"


// MySocketUDP

MySocketUDP::MySocketUDP()
: m_uPort(0)
, m_bIsCreate(FALSE)
, m_bPeekRecvThreadProc(FALSE)
,m_pPeekRecvThreadProc(NULL)
{
	m_strLogFilePath = MyCommon::GetModuleDir() + _T("log/");
}

MySocketUDP::~MySocketUDP()
{
	CloseSocket();
}

// MySocketUDP 成员函数

BOOL MySocketUDP::StartSocket(USHORT uPort)
{
	if(m_bIsCreate)
	{
		return FALSE;
	}
	if(!Startup())
	{
		AfxMessageBox(_T("初始化Socket失败"));
		//ShowSocketError();
		return FALSE;
	}

	if(!CreateSocket())
	{
		AfxMessageBox(_T("创建Socket套接字失败"));
		return FALSE;
	}

	if(uPort > 0)
	{
		m_uPort = uPort;
		if(!Bind())
		{
			CString str;
			str.Format(_T("%d端口绑定失败,请配置另一个端口号"), m_uPort);
			AfxMessageBox(str);
			CloseSocket();
			return FALSE;
		}

		if(!StartPeekRecvThread())
		{
			CloseSocket();
			return FALSE;
		}
	}

	m_bIsCreate = TRUE;

	return TRUE;
}

BOOL MySocketUDP::ReBind(USHORT uPort)
{
	if(!_CloseSocket())
	{
		return FALSE;
	}
	if(!CreateSocket())
	{
		return FALSE;
	}
	USHORT uPortTemp = m_uPort;
	m_uPort = uPort;
	if(!Bind())
	{
		m_uPort = uPortTemp;
		return FALSE;
	}
	return TRUE;
}

BOOL MySocketUDP::CloseSocket()
{
	if(!m_bIsCreate)
	{
		return FALSE;
	}

	KillPeekRecvThread();

	if(!_CloseSocket())
	{
		return FALSE;
	}
	if(!Cleanup())
	{
		return FALSE;
	}
	m_bIsCreate = FALSE;

	return TRUE;
}

BOOL MySocketUDP::SendTo( CHAR * pData, UINT nLen, CString wszIP)
{
	return SendTo(pData, nLen, wszIP, m_uPort);
}

BOOL MySocketUDP::SendTo( CHAR * pData, UINT nLen, CString wszIP, USHORT uPort)
{
	CHAR szIP[_MY_MAX_IP_LENGTH] = {0};
	MyString::WChar2Char(szIP, _MY_MAX_IP_LENGTH, wszIP);
	SOCKADDR_IN addrin = {0};
	addrin.sin_family = AF_INET;
	addrin.sin_port = htons(uPort);
	addrin.sin_addr.S_un.S_addr = inet_addr(szIP);

	return SendTo(pData, nLen, &addrin);
}

BOOL MySocketUDP::SendTo( CHAR * pData, UINT nLen, LPSOCKADDR_IN pSockaddr_in)
{
	StartSocket(0);
	try
	{
		int iRet = ::sendto(m_uSocket, pData, nLen, 0, (LPSOCKADDR)pSockaddr_in, sizeof(SOCKADDR_IN));
		//int iRet = ::sendto(m_uSocket, pData, nLen, MSG_DONTWAIT, (LPSOCKADDR)pSockaddr_in, sizeof(SOCKADDR_IN));
		if(iRet == SOCKET_ERROR)
		{
			return FALSE;
		}
	}
	catch (CException* e)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL MySocketUDP::Broadcast(CHAR * pData, UINT nLen)
{
	return Broadcast(pData, nLen, m_uPort);
}

BOOL MySocketUDP::Broadcast(CHAR * pData, UINT nLen, USHORT uPort)
{
	SOCKADDR_IN addrin = {0};
	addrin.sin_family = AF_INET;
	addrin.sin_port = htons(uPort);
	addrin.sin_addr.S_un.S_addr = ::htonl( INADDR_BROADCAST );

	//设置广播模式
	BOOL bBroadcast = TRUE;
	int iRet = ::setsockopt( m_uSocket, SOL_SOCKET, SO_BROADCAST,
		(char *)&bBroadcast,sizeof( bBroadcast ) );
	if(SOCKET_ERROR == iRet)
	{
		return FALSE;
	}

	BOOL bRet = SendTo(pData, nLen, &addrin);

	//设置非广播模式
	bBroadcast = FALSE;
	iRet = ::setsockopt( m_uSocket, SOL_SOCKET, SO_BROADCAST,
		(char *)&bBroadcast,sizeof( bBroadcast ) );
	if(SOCKET_ERROR == iRet)
	{
		//Create();
	}

	return bRet;
}

BOOL MySocketUDP::RecvFrom(CHAR * buf, int iLen, LPSOCKADDR_IN from)
{
	int iAddrinLen = sizeof(SOCKADDR_IN);
	if(SOCKET_ERROR == ::recvfrom(m_uSocket, buf, iLen, 0, (LPSOCKADDR)from, &iAddrinLen))
	{
		Sleep(100);
		return FALSE;
	}

	return TRUE;
}

int MySocketUDP::GetLastError()
{
	return ::WSAGetLastError();
}

void MySocketUDP::ShowSocketError()
{
	int errCode = ::WSAGetLastError();
	switch(errCode)
	{
	case WSANOTINITIALISED:	//在使用此API之前应首先成功地调用WSAStartup()。
		AfxMessageBox(_T("在使用此API之前应首先成功地调用WSAStartup"));
		break;
	case WSAENETDOWN:		//WINDOWS套接口实现检测到网络子系统失效。
		AfxMessageBox(_T("WINDOWS套接口实现检测到网络子系统失效"));
		break;
	case WSAEFAULT:			//fromlen参数非法；from缓冲区大小无法装入端地址。
		AfxMessageBox(_T("fromlen参数非法；from缓冲区大小无法装入端地址"));
		break;
	case WSAEINTR:			//阻塞进程被WSACancelBlockingCall()取消。
		AfxMessageBox(_T("阻塞进程被WSACancelBlockingCall()取消"));
		break;
	case WSAEINPROGRESS:	//一个阻塞的WINDOWS套接口调用正在运行中。
		AfxMessageBox(_T("一个阻塞的WINDOWS套接口调用正在运行中"));
		break;
	case WSAEINVAL:			//套接口未用bind()进行捆绑。
		AfxMessageBox(_T("套接口未用bind()进行捆绑"));
		break;
	case WSAENOTCONN:		//套接口未连接（仅适用于SOCK_STREAM类型）。
		AfxMessageBox(_T("套接口未连接（仅适用于SOCK_STREAM类型）"));
		break;
	case WSAENOTSOCK:		//描述字不是一个套接口。
		AfxMessageBox(_T("描述字不是一个套接口"));
		break;
	case WSAEOPNOTSUPP:		//指定了MSG_OOB，但套接口不是SOCK_STREAM类型的。
		AfxMessageBox(_T("指定了MSG_OOB，但套接口不是SOCK_STREAM类型的"));
		break;
	case WSAESHUTDOWN:		//套接口已被关闭。当一个套接口以0或2的how参数调用shutdown()关闭后，无法再用recv()接收数据
		AfxMessageBox(_T("套接口已被关闭。当一个套接口以0或2的how参数调用shutdown()关闭后，无法再用recv()接收数据"));
		break;
	default:
		AfxMessageBox(_T("套接口未知错误"));
	}
}

USHORT MySocketUDP::NetToHostShort(USHORT uNetShort)
{
	return ::ntohs(uNetShort);
}

CString MySocketUDP::NetToHostIP(IN_ADDR& in_addrNet)
{
	CHAR * pszIP = ::inet_ntoa(in_addrNet);
	WCHAR wszIP[_MY_MAX_IP_LENGTH] = {0};
	MyString::Char2WChar(wszIP, _MY_MAX_IP_LENGTH, pszIP);
	//CString strIP(wszIP);
	//return strIP;

	return wszIP;
}

void MySocketUDP::WriteLogWithTime(CString strSockLog)
{
	CTime time = CTime::GetCurrentTime();
	CString str = time.Format(_T("%Y-%m-%d %H:%M:%S")) + _T("  \t") + strSockLog + _T("\r\n\r\n");
	CString strLogFile = time.Format(_T("log_%Y%m%d.log"));
	if(!MyCommon::PathFileExist(m_strLogFilePath))
	{
		if(!MyCommon::CreatePath(m_strLogFilePath))
		{
			return;
		}
	}
	AppendWriteFile(str, m_strLogFilePath + strLogFile);
}

void MySocketUDP::WriteErrLog(CString strSockLog)
{
	CString str = _T("Error: ") + strSockLog;
	WriteLogWithTime(str);
}

BOOL MySocketUDP::AppendWriteFile(CString strText, CString strFileName)
{
	CFile file;
	if(!file.Open(strFileName, 
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		//CString str;
		//str.Format(_T("创建或打开日志文件失败,您所使用的计算机帐号没有相应的磁盘写权限:\r\n\t%s"), g_pControl->m_strLogFilePath);
		//AfxMessageBox(str);
		return FALSE;
	}
	CHAR* szBuf = (CHAR*)malloc(strText.GetLength()*2 + 1);
	memset(szBuf, 0, strText.GetLength()*2 + 1);
	MyString::WChar2Char(szBuf, 
		strText.GetLength()*2 + 1, strText.GetBuffer());
	file.SeekToEnd();
	file.Write(szBuf, strlen(szBuf));
	file.Close();

	return TRUE;
}
/*
void MySocketUDP::OnRecvData()
{

}
*/

////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////	private	/////////////////////////////////////////

BOOL MySocketUDP::Startup()
{
	WSADATA wsaData;
	if(::WSAStartup(WINSOCK_VERSION, &wsaData) == SOCKET_ERROR)
	{
		return FALSE;
	}
	m_uMaxUdpDg = wsaData.iMaxUdpDg;

	return TRUE;
}

BOOL MySocketUDP::CreateSocket()
{
	m_uSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//m_uSocketBroadcast = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//设置非阻塞模式
	ULONG ulMode = 1;
	::ioctlsocket(m_uSocket, FIONBIO, &ulMode);

	if(m_uSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL MySocketUDP::Bind()
{
	SOCKADDR_IN addrin = {0};
	addrin.sin_family = AF_INET;
	addrin.sin_port = htons(m_uPort);
	addrin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if(SOCKET_ERROR == ::bind(m_uSocket, (SOCKADDR*)&addrin, sizeof(SOCKADDR_IN)))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL MySocketUDP::_CloseSocket()
{
	if(::closesocket(m_uSocket) != SOCKET_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL MySocketUDP::Cleanup()
{
	if(::WSACleanup() != SOCKET_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL MySocketUDP::StartPeekRecvThread()
{
	if(m_pPeekRecvThreadProc) return TRUE;
	m_pPeekRecvThreadProc = AfxBeginThread(PeekRecvThreadProc, this);
	return m_pPeekRecvThreadProc != NULL;
}

BOOL MySocketUDP::KillPeekRecvThread()
{
	m_bPeekRecvThreadProc = FALSE;
	if(m_pPeekRecvThreadProc)
	{
		::WaitForSingleObject(m_pPeekRecvThreadProc->m_hThread, 100);
		delete m_pPeekRecvThreadProc;
		m_pPeekRecvThreadProc = NULL;
	}
	return TRUE;
}

UINT MySocketUDP::PeekRecvThreadProc(LPVOID pParam)
{
	MySocketUDP* pThis = (MySocketUDP*)pParam;
	TIMEVAL timeout;
	//timeout.tv_sec = 6;
	//timeout.tv_usec = 0;
	timeout.tv_sec = 0;
	timeout.tv_usec = 600;
	FD_SET fdset;
	int nRet;
	pThis->m_bPeekRecvThreadProc = TRUE;
	while(pThis->m_bPeekRecvThreadProc)
	{
		FD_ZERO(&fdset);
		FD_SET(pThis->m_uSocket, &fdset);
		nRet = ::select(0, &fdset, NULL, NULL, &timeout);
		//nRet = ::select(0, &fdset, NULL, NULL, NULL);
		//if(nRet == SOCKET_ERROR || nRet == 0)
		if(nRet <= 0)
		{
			Sleep(100);
			continue;
		}
		else
		{
			if(FD_ISSET(pThis->m_uSocket, &fdset))
			{
				pThis->OnRecvData();
			}
			else
			{
				Sleep(100);
			}
		}
	}
	::ExitThread(0);

	return 0;
}
