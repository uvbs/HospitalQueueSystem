#include "StdAfx.h"
#include "ComplSocketClient.h"
#include "../DoFile.h"
#include "../NurseStation.h"

//extern void MyWriteConsole(CString str);

CComplSocketClient::CComplSocketClient(void) : m_romotePort(9527)
, m_nTimeOut(1000)
{
	CDoFile doFile;
	CString exePath = doFile.GetExeFullFilePath();
	m_caChePath = exePath + _T("\\Cache");
	doFile.CreateMyDirectory(m_caChePath);
	m_caChePath += _T("\\Cache.dat");
	InitClient();
}

CComplSocketClient::~CComplSocketClient(void)
{
	StopClient();
}

BOOL CComplSocketClient::InitClient()
{
	WSADATA wsaData;
	//初始化
	if(SOCKET_ERROR==WSAStartup(MAKEWORD(2,2),&wsaData))
	{
#ifdef _DEBUG
		MyWriteConsole(_T("WSAStartup error"));
#endif
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

void CComplSocketClient::StopClient()
{
#ifdef _DEBUG
	MyWriteConsole(_T("WSACleanup"));
#endif
	WSACleanup();
}
/*
BOOL CComplSocketClient::SendData(const CString& msg,std::string& recvMsg,int& actRecvSize)
{
	if(msg.IsEmpty())
	{
		return FALSE;
	}
	BOOL flag = SendData(m_romotePort,m_romoteIP,msg,recvMsg,actRecvSize);
	if(!flag)
	{
		//缓存机制
		DealCache(msg);
	}
	else
	{
		//重发机制
		ReSendMsg();
	}
	return flag;
}
*/
BOOL CComplSocketClient::SendData(USHORT port,CString IP,const CString& msg,std::string& recvMsg,int& actRecvSize)
{
	CCommonConvert convert;
	int len = convert.CStringToChar(msg,NULL);
	char* data = new char[len+1];
	convert.CStringToChar(msg,data);
	std::string sendData(data);
	delete [] data;
	BOOL flag = SendData(port,IP,sendData,len+1,recvMsg,actRecvSize);
	return flag;
}

BOOL CComplSocketClient::SendData(USHORT port,CString IP,const std::string& msg,int size,std::string& recvMsg,int& actRecvSize)
{
	return SendData(port,IP,msg.c_str(),size,recvMsg,actRecvSize);
}

BOOL CComplSocketClient::SendData(USHORT port,CString IP,const char* msg,int size,std::string& recvMsg,int& actRecvSize )
{
	WSADATA wsaData;
	//初始化
	if(SOCKET_ERROR==WSAStartup(MAKEWORD(2,2),&wsaData))
	{
#ifdef _DEBUG
		MyWriteConsole(_T("WSAStartup error"));
#endif
		return FALSE;
	}
#ifdef _DEBUG
	CString sendMsg = _T("Send Code:");
	CString temp(msg);
	sendMsg+=temp;
	MyWriteConsole(sendMsg);
#endif
	m_sClient = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET == m_sClient)
	{
		WSACleanup();
#ifdef _DEBUG
		MyWriteConsole(_T("socket error"));
#endif
		return FALSE;
	}
	SOCKADDR_IN ServerAddr;
	ServerAddr.sin_port = htons(port);
	ServerAddr.sin_family = AF_INET;
	///转换
	CCommonConvert convert;
	int len = convert.CStringToChar(IP,NULL);

	char* buf = new char[len+1]; 
	ZeroMemory(buf,len+1);
	convert.CStringToChar(IP,buf);
	////////////////////////////////////////////
	//ServerAddr.sin_addr.S_un.S_addr = inet_addr(buf);
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(buf);
	delete []buf;

	if(SOCKET_ERROR==connect(m_sClient,(SOCKADDR*)&ServerAddr,
		sizeof(ServerAddr)))
	{
#ifdef _DEBUG
		MyWriteConsole(_T("connect error"));
#endif
		closesocket(m_sClient);
		WSACleanup();
		return FALSE;
	}
	setsockopt(m_sClient,SOL_SOCKET,SO_SNDTIMEO,(char *)&m_nTimeOut,sizeof(UINT));
	if(SOCKET_ERROR == send(m_sClient,msg,size,0))
	{
#ifdef _DEBUG
		MyWriteConsole(_T("send failed"));
#endif
		closesocket(m_sClient);
		WSACleanup();
		return FALSE;
	}
	char recvBuf[MAXRECVBUF]={0};
	//setsockopt(m_sClient,SOL_SOCKET,SO_RCVTIMEO,(char *)&m_nTimeOut,sizeof(UINT));
	while(true)
	{
		setsockopt(m_sClient,SOL_SOCKET,SO_RCVTIMEO,(char *)&m_nTimeOut,sizeof(UINT));
		int tempSize = recv(m_sClient,recvBuf,MAXRECVBUF,0);
		if(tempSize==SOCKET_ERROR || tempSize==0)
		{
#ifdef _DEBUG
			DWORD errCode = GetLastError();
			CString str = _T("recv error:");
			str.AppendFormat(_T("%d"),errCode);
			MyWriteConsole(str);
#endif
			break;
		}
		actRecvSize+=tempSize;
		std::string recvTemp(recvBuf);
		recvMsg += recvTemp;
	}
#ifdef _DEBUG
	MyWriteConsole(_T("send ok"));
#endif
	closesocket(m_sClient);
	WSACleanup();
	return TRUE;
}
/*
void CComplSocketClient::DealCache(const CString& msg)
{
	if(msg.IsEmpty())return;
	m_list_caCheMsg.push_back(msg);
#ifdef _DEBUG
	CString str=_T("cache size:");
	str.AppendFormat(_T("%d"),m_list_caCheMsg.size());
	MyWriteConsole(str);
#endif
	if(m_list_caCheMsg.size()>=200)
	{
		CFile file;
		if(file.Open(m_caChePath,CFile::modeCreate | CFile::modeWrite))
		{
			std::list<CString>::const_iterator itera = m_list_caCheMsg.begin();
			for(itera;itera!=m_list_caCheMsg.end();itera++)
			{
				CString temp = *itera;
				char tempBuf[MAXRECVBUF] ={0};
				CCommonConvert::CStringToChar(temp,tempBuf);
				file.Write(tempBuf,MAXRECVBUF);
			}
			m_list_caCheMsg.clear();//清除缓存
			file.Close();
		}
	}
}

void CComplSocketClient::ReSendMsg()
{
	AppendListMsg();//从文件读出没发送成功的数据
	///文件清空
	CFile clearFile;
	if(clearFile.Open(m_caChePath,CFile::modeWrite))
	{
		clearFile.SetLength(0);
		clearFile.Close();
	}
	/////////////////
	if(m_list_caCheMsg.size()==0)return;
	std::list<CString>::const_iterator itera = m_list_caCheMsg.begin();
	for(itera;itera!=m_list_caCheMsg.end();itera++)
	{
		CString msg = *itera;
		std::string recvMsg;
		int actRecvSize = 0;
		BOOL flag = SendData(m_romotePort,m_romoteIP,msg,recvMsg,actRecvSize);
		if(!flag)//重发时发送不成功
		{
			//直接写文件
			CFile file;
			if(file.Open(m_caChePath,CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
			{
				file.SeekToEnd();
				CString temp = *itera;
				char tempBuf[MAXRECVBUF] ={0};
				CCommonConvert::CStringToChar(temp,tempBuf);
				file.Write(tempBuf,MAXRECVBUF);
				file.Close();
			}
		}
	}
	m_list_caCheMsg.clear();
}
*/
/*
BOOL CComplSocketClient::AppendListMsg()
{
	CFile file;
	if(file.Open(m_caChePath,CFile::modeRead))
	{
		DWORD length = (DWORD)file.GetLength()/MAXRECVBUF;
		for(DWORD i=0;i<length;i++)
		{
			char msg[MAXRECVBUF] = {0};
			if(file.Read(msg,MAXRECVBUF))
			{
				CString wStrMsg;
				CCommonConvert::CharToCstring(wStrMsg,msg);
				m_list_caCheMsg.push_back(wStrMsg);
			}
		}
		file.Close();
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
*/