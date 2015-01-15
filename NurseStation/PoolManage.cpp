#include "StdAfx.h"
#include "PoolManage.h"

CPoolManage::CPoolManage(void)
{
}

CPoolManage::~CPoolManage(void)
{
}

int CPoolManage::CStringToChar(CString str,char c_str[])
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

int CPoolManage::CharToCstring(CString& str,char c_str[])
{
	int len = MultiByteToWideChar(CP_ACP,0,c_str,-1,str.GetBuffer(),0);
	if(c_str!=NULL)
	{
		wchar_t* w_str;
		w_str = new wchar_t[len+1];
		ZeroMemory(w_str,len+1);
		MultiByteToWideChar(CP_ACP,0,c_str,-1,w_str,len);
		str=w_str;
		delete [] w_str;
	}
	return len;
}

void CPoolManage::intToCString(int i,CString& str)
{
	char c_str[255]={0};
	itoa(i,c_str,10);
	wchar_t w_str[255]={0};
	MultiByteToWideChar(CP_ACP,0,c_str,-1,w_str,255);
	str=w_str;
}

void CPoolManage::CStringToint(int& i,CString str)
{
	char c_str[255]={0};
	CStringToChar(str,c_str);
	i=atoi(c_str);
}

CString CPoolManage::GetComputerIP()
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

CString CPoolManage::RequestBracket(const CString &str)
{
	if(str==_T(""))
	{
		return _T("");
	}
	char c_str[255]={0};
	CString strResult=_T("");
	CStringToChar(str,c_str);
	int i=0;
	while(c_str[i])
	{
		if(c_str[i]=='(')
		{
			int j=i+1;
			while(c_str[j])
			{
				if(c_str[j]!=')')
				{
					strResult+=c_str[j];
				}
				j++;
			}
			break;
		}
		i++;
	}
	return strResult;
}

CString CPoolManage::DeleRequest(const CString &str)
{
	int count = str.GetLength();
	int i=0;
	for(i;i<count;i++)
	{
		WCHAR w = str.GetAt(i);
		if(w=='(')
		{
			break;
		}
	}
	return str.Left(i);
}
