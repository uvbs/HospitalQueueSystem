#include "StdAfx.h"
#include "CommonConvert.h"

CCommonConvert::CCommonConvert(void)
{
}

CCommonConvert::~CCommonConvert(void)
{
}

int CCommonConvert::CStringToChar(CString str,char c_str[])
{
	///参数传NULL，返回长度
	int len=WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);
	if(c_str!=NULL)
	{
		char* tmpch = NULL;
		tmpch=new char[len+1];
		memset(tmpch,0,len+1);
		WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),tmpch,len,NULL,NULL);
		strncpy(c_str,tmpch,len+1);
		c_str[len]='\0';
		delete [] tmpch;
	}
	return len;
}

int CCommonConvert::CharToCstring(CString& str,char c_str[])
{
	int len = MultiByteToWideChar(CP_ACP,0,c_str,-1,str.GetBuffer(),0);
	if(c_str!=NULL)
	{
		wchar_t* w_str = NULL;
		w_str = new wchar_t[len+1];
		ZeroMemory(w_str,len+1);
		MultiByteToWideChar(CP_ACP,0,c_str,-1,w_str,len);
		str=w_str;
		delete [] w_str;
	}
	return len;
}

void CCommonConvert::intToCString(int i,CString& str)
{
	char c_str[255]={0};
	itoa(i,c_str,10);
	wchar_t w_str[255]={0};
	MultiByteToWideChar(CP_ACP,0,c_str,-1,w_str,255);
	str=w_str;
}

void CCommonConvert::CStringToint(int& i,CString str)
{
	char c_str[255]={0};
	CStringToChar(str,c_str);
	i=atoi(c_str);
}
/*
获取本机的IP地址
*/
CString CCommonConvert::GetComputerIP()
{
	WSADATA wsaData;
	char name[255];
	char *IP;
	PHOSTENT hostinfo; 
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) == 0 ) 
	{  
		if( gethostname ( name, sizeof(name)) == 0) 
		{ 
			if((hostinfo = gethostbyname(name)) != NULL) 
			{ //这些就是获得IP的函数
				IP= inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			}
		}
		WSACleanup();
	}

	int len=MultiByteToWideChar(CP_ACP,0,IP,-1,NULL,0);
	wchar_t *w_strIP;
	w_strIP=new wchar_t[len+1];
	memset(w_strIP,0,(len+1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP,0,IP,-1,w_strIP,len);
	CString c_strIP=w_strIP;

	delete w_strIP;
	return c_strIP;
}

CString CCommonConvert::GetExeFullFilePath()
{
	TCHAR lpPath[MAX_PATH]=TEXT("");
	DWORD dwResult=GetModuleFileName(NULL,lpPath,MAX_PATH);
	if(dwResult==0)return _T("");
	for(int i=dwResult;i>0;i--)
	{
		if(lpPath[i]=='\\'){lpPath[i]=0;break;}
	}
	m_strPath=lpPath;
	return m_strPath;
}

BOOL CCommonConvert::WChar2Char(char* dstBuf, size_t len, const wchar_t* src)
{
	if(src == NULL) return FALSE;

	size_t chlen = wcslen(src);
	size_t nBytes =(size_t)WideCharToMultiByte(0, 0,src, chlen, 
		NULL, 0, NULL, NULL);
	int iLastIndex;
	if(nBytes>=len)
	{
		nBytes=len-1;
		iLastIndex = len;
	}
	else
	{
		iLastIndex = nBytes;
	}
	nBytes = WideCharToMultiByte(0, 0, src, chlen, 
		dstBuf, nBytes, NULL, NULL);
	dstBuf[iLastIndex]=0;
	if(nBytes == 0) return FALSE;
	return TRUE;
}

std::string CCommonConvert::string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);
	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr(pBuf);
	delete []pwBuf;
	delete []pBuf;
	pwBuf = NULL;
	pBuf  = NULL;
	return retStr; 
}

std::string CCommonConvert::UTF8_To_string(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr = pBuf;
	delete []pBuf;
	delete []pwBuf;
	pBuf = NULL;
	pwBuf = NULL;
	return retStr; 
}