/////////////////////////////////////////////////////////////////////
//版权: 博瑞丰应用软件开发部
//作者: 邓坦周				
//内容: 对字符串进行分析，以取得参数
//日期: 02.10.26
/////////////////////////////////////////////////////////////////////

// 注意：用 #(2f) 代替 / 符号

#include "stdafx.h"
#include "Parameter.h"
#include <tlhelp32.h>

#ifndef ATOI_T

	#ifdef UNICODE
		#define PSTR_T			(unsigned short*)			
		#define STR_T			(unsigned short)
		#define STRCPY_T		wcscpy
		#define STRDUP_T		_wcsdup
		#define STRUPR_T		_wcsupr
		#define STRLEN_T		wcslen
		#define ATOI_T			_wtoi
		#define ATOF_T			_wtol
	#else // UNICODE
		#define PSTR_T			(char*)
		#define STR_T			(char)
		#define STRCPY_T		strcpy
		#define STRDUP_T		strdup
		#define STRUPR_T		strupr
		#define STRLEN_T		strlen
		#define ATOI_T			atoi
		#define ATOF_T			atof
	#endif // UNICODE

#endif //ATOI_T

int FindEnd(const CString& str, int nBegin )
{
	int nTemp = str.Find( TEXT("/"), nBegin + 1 );
	if( -1 != nTemp )
	{
		if( str.GetLength() > (nTemp+1) && str[nTemp+1] == '/' )
			return FindEnd( str, nTemp + 1 );
	}
	else
	{
		nTemp = str.GetLength();
	}
	
	return nTemp;
}

// /user tango /height 12 /end
int RequestParam( const CString &strParam, const CString &strName  )
{
	CString		str;
	int			nRes = -1;

	int	nBegin = strParam.Find( strName, 0 );

	if( nBegin != -1 )
	{
		int	nEnd = strParam.Find( TEXT("/"), nBegin + 1 );
		
		if( nEnd == -1 )
		{
			nEnd = strParam.GetLength();
		}

		str = strParam.Mid( nBegin + strName.GetLength(), nEnd - nBegin - strName.GetLength() );
		str.TrimLeft();
		str.TrimRight();
		
		nRes = _ttol(str);
	}

	return nRes;
}

#ifndef UNDER_CE
float RequestParamF( const CString &strParam, const CString &strName )
{
	CString		str;
	float		fRes = -0.0;

	int	nBegin = strParam.Find( strName, 0 );

	if( nBegin != -1 )
	{
		int	nEnd = strParam.Find( TEXT("/"), nBegin + 1 );
		if( nEnd == -1 )
		{
			nEnd = strParam.GetLength();
		}

		str = strParam.Mid( nBegin + strName.GetLength(), nEnd - nBegin - strName.GetLength() );
		str.TrimLeft();
		str.TrimRight();

		fRes = (float)ATOF_T( str ); //atof
	}

	return fRes;
}
#endif // UNDER_CE

CString toParam( const CString& param )
{
	CString str = param;
	str.Replace( TEXT("/"), TEXT("//") );
	str.Replace( TEXT(" "), TEXT("#(20)") );
	return str;
}

CString RequestParamStr( const CString &strParam, const CString &_strName )
{
	if( strParam.IsEmpty() )
		return _T("");
	
	CString strName = _strName;

	if( ' ' != strName.Right(1) )
		strName += " ";

	CString		str;

	int	nBegin = strParam.Find( strName,0 );

	if( nBegin != -1 )
	{
		//int	nEnd = strParam.Find( TEXT("/"), nBegin + 1 );

		//if( nEnd == -1 )
		//{
		//	nEnd = strParam.GetLength();
		//}
		int nEnd = FindEnd( strParam, nBegin );

		str = strParam.Mid( nBegin + strName.GetLength(), nEnd - nBegin - strName.GetLength() );
		str.TrimLeft();
		str.TrimRight();

		//str.Replace( TEXT("#(2f)"), TEXT("/") );
		//str.Replace( TEXT("#(2F)"), TEXT("/") );

		str.Replace( TEXT("#(20)"), TEXT(" ") );
		str.Replace( TEXT("//"), TEXT("/"));
	}

	return str;
}

// 12,23,23,23,34,34
int RequestParams( const CString& ini, CDWordArray &array, const CString &sep )
{
	const CString str_zero = TEXT("0");
	int		err = 0;
	array.RemoveAll();

	CString		str = ini;

	while( !str.IsEmpty() )
	{
		int n = str.Find( sep );
		
		if( -1 == n ) n = str.GetLength();

		CString d = str.Left( n );
		int value = _ttol( d );

		if( (0==value) && (d.Find(str_zero)==-1) )
		{
		}
		else
		{
			array.Add( value ); 
		}
		if((str.GetLength() - n - sep.GetLength())<=0) break;
		str = str.Right( str.GetLength() - n - sep.GetLength() );
		str.TrimLeft(); str.TrimRight();
	}
	return err;
}

int RequestParamsStr(const CString&ini,CStringArray&array,const CString&sep, BOOL bWithSpace )
{
	if( ini.IsEmpty() ) return 0;
	
	int	err = 0;
		array.RemoveAll();
	try
	{			
		
		CString		str = ini;
		
		while( !str.IsEmpty() )
		{
			int	n = str.Find( sep );
			if( -1 == n ) n = str.GetLength();
			
			CString	value = str.Left( n );
			
			if( !bWithSpace )
			{
				value.TrimLeft(); value.TrimRight();
			}
			
			if( !value.IsEmpty() )
				array.Add( value );
			
 			if((str.GetLength() - n - sep.GetLength())<=0) break; 		
			str = str.Right( str.GetLength() - n - sep.GetLength() );
			
			if( !bWithSpace )
			{
				str.TrimLeft(); str.TrimRight();
			}
		}
	}
	catch (...)
	{
		
	}
	return err;
}


 int RequestParamsStrofSpace(const CString&ini,CStringArray&array,const CString&sep, BOOL bWithSpace,BOOL bSpace )
{
	if( ini.IsEmpty() ) return 0;

	int	err = 0;
	array.RemoveAll();
	
	CString		str = ini;

	while( !str.IsEmpty() )
	{
		int	n = str.Find( sep );
		if( -1 == n ) n = str.GetLength();

		CString	value = str.Left( n );

		if( !bWithSpace )
		{
			value.TrimLeft(); value.TrimRight();
		}

		if(bSpace || !value.IsEmpty() )
			array.Add( value );
		
		if((str.GetLength() - n - sep.GetLength())<=0) break;
		str = str.Right( str.GetLength() - n - sep.GetLength() );

		if( !bWithSpace )
		{
			str.TrimLeft(); str.TrimRight();
		}
	}

	return err;
}

CString logFontToString(const LOGFONT& font )
{
	CString	str;
	str.Format( TEXT("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d`|`%s"), 
	font.lfHeight, 
	font.lfWidth, 
	font.lfEscapement, 
	font.lfOrientation, 
	font.lfWeight, 
	font.lfItalic, 
	font.lfUnderline, 
	font.lfStrikeOut, 
	font.lfCharSet, 
	font.lfOutPrecision, 
	font.lfClipPrecision, 
	font.lfQuality, 
	font.lfPitchAndFamily, 
	font.lfFaceName //[LF_FACESIZE]; 
	);
	return str;
}

BOOL StringTologFont(const CString& str, LOGFONT& font )
{
	
	CStringArray	sArray;
	RequestParamsStr( str, sArray, _T("`|`") );	
	if( 2 != sArray.GetSize() ) return FALSE;
	CDWordArray dArray;
	RequestParams( sArray.GetAt(0), dArray );	
	if( 13 != dArray.GetSize() ) return FALSE;
	if(dArray.GetAt(0)<-1000) return FALSE;
	
	
#ifdef _UNICODE
	wcscpy( font.lfFaceName, sArray.GetAt(1) );
#else
	strcpy( font.lfFaceName, sArray.GetAt(1) );

#endif _UNICODE


	

	font.lfHeight = dArray[0]; 
	font.lfWidth = dArray[1]; 
	font.lfEscapement = dArray[2]; 
	font.lfOrientation = dArray[3]; 
	font.lfWeight = dArray[4]; 
	font.lfItalic = (BYTE)dArray[5]; 
	font.lfUnderline = (BYTE)dArray[6]; 
	font.lfStrikeOut = (BYTE)dArray[7]; 
	font.lfCharSet = (BYTE)dArray[8]; 
	font.lfOutPrecision = (BYTE)dArray[9]; 
	font.lfClipPrecision = (BYTE)dArray[10]; 
	font.lfQuality = (BYTE)dArray[11]; 
	font.lfPitchAndFamily = (BYTE)dArray[12]; 
		
	return TRUE;
}

// B647163E-150A-4aa8-8C15-A4FADFAD5B59

#ifndef UNDER_CE

BOOL StringToGuid( const CString& str_, GUID& guid )
{
	BOOL	bRes = FALSE;
	CString str = str_;

	if( str.Left(1) != _T('{') )
		str = _T("{") + str;
	if( str.Right(1) != _T('}') )
		str = str + _T("}");

	BSTR	lpsz = str.AllocSysString();
	HRESULT hr = CLSIDFromString( lpsz, &guid ); // CLSIDFromProgID
	::SysAllocString( lpsz );

	return hr == 0;
}

CString GuidToString( const GUID& guid )
{
	WCHAR		guid_str[MAX_PATH];
	StringFromGUID2( guid, guid_str, MAX_PATH );
	CString		str(guid_str);
	str = str.Left( 37 );
	str = str.Right( 36 );
	return str;
}

#endif // UNDER_CE

int GetDaysInMonth(const CTime& time )
{
	int		nDays1[] = {31,29,31,30,31,30,31,31,30,31,30,31};
	int		nDays2[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	
	int		nYear = time.GetYear();
	int		nMonth =  time.GetMonth();
	
	if( ( !(nYear%4) && (nYear%100) ) || !(nYear%400) )
	{
		return nDays1[ nMonth-1 ];
	}
	else
	{
		return nDays2[ nMonth - 1 ];
	}
}

CString DayTimeToString(const CTime& time )
{
	CString	str;
	try
	{
		str.Format(TEXT("%04d,%02d,%02d,%02d,%02d,%02d"),
			time.GetYear(), time.GetMonth(),time.GetDay(),
		time.GetHour(), time.GetMinute(), time.GetSecond() );
	}
	catch(...)
	{
		return _T("");
	}
	
	return str;
}
CString DayToString(const CTime& time)
{
	CString str;
	str.Format( TEXT("%04d,%02d,%02d"), 
		time.GetYear(), time.GetMonth(), time.GetDay() ); // = time.Format("%Y,%m,%d");
	return str;
}
CString TimeToString( const CTime& time ,const CString & Symbol)
{
	CString	str;
	try
	{
		str.Format( TEXT("%02d%s%02d%s%02d"), 
		time.GetHour(),Symbol, time.GetMinute(),Symbol, time.GetSecond() ); //= time.Format("%H,%M,%S");
	}	
	catch (...)
	{
		return _T("");
	}
	
	return str;
}
void	SetSysTime(const CString& time)
{
	COleDateTime tm;
	SYSTEMTIME st;
	tm.ParseDateTime(time);		
	tm.GetAsSystemTime(st);
	SetLocalTime(&st);
}

BOOL	StringToDayTime( const CString& str, CTime& time )
{
	BOOL	bres = FALSE;
	CDWordArray	array;
	RequestParams( str, array );
	if( 6 == array.GetSize() )
	{
		bres = TRUE;

		try
		{
			time = CTime( array[0], array[1], array[2], array[3], array[4], array[5] );
		}
		catch(...)
		{
			bres = FALSE;
		}
		
	}
	
	return bres;
}
BOOL	StringToDay(const CString& str, CTime& time)
{
	BOOL	bres = FALSE;
	CDWordArray	array;
	RequestParams( str, array );
	if( 3 == array.GetSize() )
	{
		bres = TRUE;
		try
		{
			time = CTime( array[0], array[1], array[2], 7, 7, 7 );
		}
		catch(...)
		{
			bres = FALSE;
		}
	}
	
	return bres;
}
BOOL	StringToDay(const CString& str, CTime& time,const CString & Symbol)
{
	BOOL	bres = FALSE;
	CStringArray a;
	CDWordArray	array;
	RequestParamsStr(str,a,_T(" "));
	if(a.GetSize()<=0) return FALSE;
	RequestParams( a.GetAt(0), array ,Symbol);
	if( 3 == array.GetSize() )
	{
		bres = TRUE;
		try
		{
			if(array[0]<2010) array[0]=2010;
			time = CTime( array[0], array[1], array[2], 7, 7, 7 );
		}
		catch(...)
		{
			bres = FALSE;
		}
	}
	
	return bres;
}
BOOL	StringToStandStr(const CString& str,CTime& time)
{
	BOOL bres=FALSE;
	// 2012-01-01 01:01:01
	CStringArray A1;
	RequestParamsStr(str,A1,_T(" "));
	if(A1.GetSize()==2)
	{
		CStringArray B1,B2;
		RequestParamsStr(A1.GetAt(0),B1,_T("-"));
		RequestParamsStr(A1.GetAt(1),B2,_T(":"));
		if(B1.GetSize()==3 && B2.GetSize()==3)
		{
			try
			{
				CString b1_str0=B1.GetAt(0);
				CString b1_str1=B1.GetAt(1);
				CString b1_str2=B1.GetAt(2);
				CString b2_str0=B2.GetAt(0);
				CString b2_str1=B2.GetAt(1);
				CString b2_str2=B2.GetAt(2);
				char b1_char0[256]={0};
				char b1_char1[256]={0};
				char b1_char2[256]={0};

				char b2_char0[256]={0};
				char b2_char1[256]={0};
				char b2_char2[256]={0};
				
				WideCharToMultiByte(CP_ACP,0,b1_str0,-1,b1_char0,0,NULL,FALSE);
				WideCharToMultiByte(CP_ACP,0,b1_str1,-1,b1_char1,0,NULL,FALSE);
				WideCharToMultiByte(CP_ACP,0,b1_str2,-1,b1_char2,0,NULL,FALSE);

				WideCharToMultiByte(CP_ACP,0,b2_str0,-1,b2_char0,0,NULL,FALSE);
				WideCharToMultiByte(CP_ACP,0,b2_str1,-1,b2_char1,0,NULL,FALSE);
				WideCharToMultiByte(CP_ACP,0,b2_str2,-1,b2_char2,0,NULL,FALSE);

				time = CTime(atoi(b1_char0), atoi(b1_char1),atoi(b1_char2), 
					atoi(b2_char0), atoi(b2_char1), atoi(b2_char2));
				bres = TRUE;
			}
			catch(...)
			{
				bres = FALSE;
			}
		}
	}
	return bres;
}

BOOL	StringToTime(const CString& str, CTime& time, const CTime& t,const CString & Symbol)
{
	BOOL	bres = FALSE;
	CDWordArray	array;
	RequestParams( str, array ,Symbol);
	if( 3 == array.GetSize() )
	{
		bres = TRUE;
		try
		{
			time = CTime( t.GetYear(), t.GetMonth(), t.GetDay(), 
				array[0], array[1], array[2] );
		}
		catch(...)
		{
			bres = FALSE;
		}
	}
	
	return bres;
}

BOOL IsSameDay(const CTime& t1, const CTime& t2 )
{
	return t1.GetYear()==t2.GetYear() && 
		t1.GetMonth()==t2.GetMonth() && 
		t1.GetDay()==t2.GetDay();
}
int		TimeCompareTime(const CTime& t1, const CTime& t2 )	//t1>t2=1,t1=t2=0,t1<t2=-1
{
	if(t1<0 || t2<0) return -2;
	if(t1.GetHour()>t2.GetHour() || (t1.GetHour()==t2.GetHour() && t1.GetMinute()>t2.GetMinute())
		||(t1.GetHour()==t2.GetHour() && t1.GetMinute()==t2.GetMinute() && t1.GetSecond()>t2.GetSecond()))
	{
		return 1;
	}
	else if(t1.GetHour()==t2.GetHour() && t1.GetMinute()==t2.GetMinute() && t1.GetSecond()==t2.GetSecond())
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
inline BYTE toHex(const BYTE &x)

{
	return x > 9 ? x + 55: x + 48;
}

CString NewUrlEncode( const CString& strIn)
{
	/*
	static 	HMODULE		ghUrlEncode = NULL;
	static  LPCTSTR		(WINAPI* UrlEncode)(LPCTSTR) = NULL; 

	CString strOut(_T(""));
	
	if( NULL == ghUrlEncode )
	{
		ghUrlEncode = LoadLibrary("urlencodedll.dll");
	}

	if( NULL == ghUrlEncode ) return strOut;
	
	if( NULL == UrlEncode )
	{
		UrlEncode = (LPCTSTR (WINAPI*)(LPCTSTR))GetProcAddress( ghUrlEncode,"UrlEncode");
	}
	
	if( NULL == UrlEncode ) return strOut;
	*/

	CString	sIn = strIn;
	CString sOut;
	
	const int nLen = sIn.GetLength() + 1;
	
	
	
	register LPBYTE pOutTmp = NULL;
	
	LPBYTE pOutBuf = NULL;
	
	register LPBYTE pInTmp = NULL;
	
	LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
	
	BYTE b = 0;
	
	
	
	//alloc out buffer
	
	pOutBuf = (LPBYTE)sOut.GetBuffer(nLen*3 - 2);//new BYTE [nLen  * 3];
	
	
	
	if(pOutBuf)
		
	{
		
		pInTmp   = pInBuf;
		
		pOutTmp = pOutBuf;
		
		
		
		// do encoding
		
		while (*pInTmp)
			
		{
			
			if(isalnum(*pInTmp))
				
				*pOutTmp++ = *pInTmp;
			
			else
				
				if(isspace(*pInTmp))
					
					*pOutTmp++ = '+';
				
				else
					
				{
					
					*pOutTmp++ = '%';
					
					*pOutTmp++ = toHex(*pInTmp>>4);
					
					*pOutTmp++ = toHex(*pInTmp%16);
					
				}
				
				pInTmp++;
				
		}
		
		*pOutTmp = '\0';
		
		//sOut=pOutBuf;
		
		//delete [] pOutBuf;
		
		sOut.ReleaseBuffer();
		
	}
	
	sIn.ReleaseBuffer();
	
	return sOut;
	
	
	
	/*
	for(int i=0;i<strIn.GetLength();i++)
	{
		TCHAR chr = strIn.GetAt(i);
		TCHAR strHex[7] = "";
		CString temp;
		
		if(chr>127 || chr<0)
		{
			// UTF-8 三字节编码: 1110xxxx 10xxxxxx 10xxxxxx
			// UTF-8 双字节编码: 110xxxxx 10xxxxxx
			_itot((chr>>12)+224,strHex,16);
			//temp.Format( "%x", (chr>>12)+224 );
			strOut+=_T("%");
			strOut+=strHex;

			_itot((chr>>6)%64+128,strHex,16);
			//temp.Format("%x", (chr>>6)%64+128 );
			strOut+=_T("%");
			strOut+=strHex;

			_itot((chr%64)+128,strHex,16);
			//temp.Format("%x", (chr%64)+128 );
			strOut+=_T("%");
			strOut+=strHex;
		}
		else
		{
			if(_istalnum(chr))
			{
				strOut+=chr;
			}
			else if(_istspace(chr))
			{
				strOut+=_T("+");
			}
			else
			{
				_itot(chr,strHex,16);
				//temp.Format("%x", chr);
				strOut+=_T("%");
				strOut+=strHex;
			}
		}
	}
	*/
	
	//return strOut;
}

DWORD IpStringToDWORD( const CString& strAddr )
{
	BYTE lpIP[4];

	CStringArray	array;
	RequestParamsStr( strAddr, array, _T(".") );

	if( array.GetSize() == 4 )
	{
		lpIP[0] = (BYTE)ATOI_T( array[0] );
		lpIP[1] = (BYTE)ATOI_T( array[1] );
		lpIP[2] = (BYTE)ATOI_T( array[2] );
		lpIP[3] = (BYTE)ATOI_T( array[3] );
	}

	return *(DWORD *)lpIP;
}
void	OleDateTimeToTime(const COleDateTime& oleTime, CTime& Time1)
{
	
	Time1=	CTime (oleTime.GetYear(),oleTime.GetMonth(),oleTime.GetDay(),oleTime.GetHour(),oleTime.GetMinute(),oleTime.GetSecond());
	//time = CTime( array[0], array[1], array[2], array[3], array[4], array[5] );
	
}
void	TimeToOleDateTime(const CTime& Time,COleDateTime & OleTime)
{
	
	OleTime =COleDateTime(Time.GetYear(),Time.GetMonth(),Time.GetDay(),Time.GetHour(),Time.GetMinute(),Time.GetSecond());
}
void	OleDateTimeToStandStr(const COleDateTime& time, CString & str)
{
	str.Format(TEXT("%04d-%02d-%02d %02d:%02d:%02d"), 
		time.GetYear(), time.GetMonth(), time.GetDay(),
		time.GetHour(), time.GetMinute(), time.GetSecond() );	
}

void TimetoOleDateTime(const CTime& Time1,COleDateTime & time2)
{
COleDateTime oletime(Time1.GetYear(),Time1.GetMonth(),Time1.GetDay(),Time1.GetHour(),Time1.GetMinute(),Time1.GetSecond());
	time2=oletime;
}
/*
void	StandToDayTime(const CString& strtime,CTime & time)
{
	CStringArray a1;
	RequestParamsStr(strtime,a1," ");
	if(a1.GetSize()==2)
	{

	}

	if(str.GetLength()==8)
	{
		strtime=str.Mid(0,4)+"-"+str.Mid(4,2)+"-"+str.Mid(6,2);
	}
	else if(str.GetLength()==14)
	{
		strtime=str.Mid(0,4)+"-"+str.Mid(4,2)+"-"+str.Mid(6,2)+" "+str.Mid(8,2)+":"+str.Mid(10,2)+str.Mid(12,2);
	}

}
*/
void DayTimeToStandStr( const CTime& time, CString& str )
{
	str.Format(TEXT("%04d-%02d-%02d %02d:%02d:%02d"), 
		time.GetYear(), time.GetMonth(), time.GetDay(),
		time.GetHour(), time.GetMinute(), time.GetSecond() );
}

CString	TimeStrToStandStr(const CString& str)	//20110101 转 2011-01-01
{
	CString strtime;
	if(str.GetLength()==8)
	{
		strtime=str.Mid(0,4)+_T("-")+str.Mid(4,2)+_T("-")+str.Mid(6,2);
	}
	else if(str.GetLength()==14)
	{
		strtime=str.Mid(0,4)+_T("-")+str.Mid(4,2)+_T("-")+str.Mid(6,2)+_T(" ")+str.Mid(8,2)+_T(":")+str.Mid(10,2)+str.Mid(12,2);
	}
	return strtime;
}

CString  inttoCString(  int   a) 
{
	CString str;
	if(a<0) a=-a;
	str.Format(_T("%d"),a);
	return str;
}

void CStringtobyte(const CString & str,BYTE * pb)		//转网络字节
{	
	
	CString str1=str;
	char pstr[256]={0};
	WideCharToMultiByte(CP_ACP,0,str1,-1,pstr,0,NULL,FALSE);
	char *p=pstr;
	int len=str.GetLength();
	for(int i=0;i<len;i++)
	{
		pb[i]=(BYTE)p[i];			
		
	}
	
}

byte* intToByte(int i) {			//转网络字节
	
	byte *abyte0 = new byte[4];
	
	abyte0[0] = (byte) (0xff & i);
	
	abyte0[1] = (byte) ((0xff00 & i) >> 8);
	
	abyte0[2] = (byte) ((0xff0000 & i) >> 16);
	
	abyte0[3] = (byte) ((0xff000000 & i) >> 24);
	
	return abyte0;
	
  }

int bytesToInt(byte* bytes) {
	
	int addr = bytes[0] & 0xFF;
	
	addr |= ((bytes[1] << 8) & 0xFF00);
	
	addr |= ((bytes[2] << 16) & 0xFF0000);
	
	addr |= ((bytes[3] << 24) & 0xFF000000);
	
	return addr;
	
	   }

//////////////////////////////////////////////////////////////////////////
//函数功能：返回指定传
//函数参数：源串, 起始标记，结束标记
//函数返回：源串中从起始标记开始到结束标记中间的串
//编 写 人：LiuPC
//编写日期: 2009/07/06
//////////////////////////////////////////////////////////////////////////
CString  ReturnStringW(const CString& _strSrc, const CString& _strStartFlag, const CString& _strEndFlag)
{
	CString   retStr   = _T("");
	CString   strObj   = _strSrc;
	CString   strStart = _strStartFlag;
	CString   strEnd   = _strEndFlag;
	
	//起始位置索引
	int nStartSel = strObj.Find( strStart );
	if ( nStartSel == -1 )
	{
		TRACE0("Start Flag Values Is NULL!");
		return _T("");
	}
	
	//终止位置索引
	int nEndSel   = strObj.Find( strEnd );
	if ( nEndSel == -1 )
	{
		TRACE0("End Flag Values Is NULL");
		return _T("");
	}
	
	//当终止索引大于起始索引取中间信息
	if ( nEndSel > nStartSel )
	{
		retStr = strObj.Left(nEndSel);
		retStr = retStr.Right( retStr.GetLength() - nStartSel - 2);
	}
	else //否则 (如果出现起始位置大于终止位置的情况例如 请A001号到01号窗口办理)
	{
		//当取到和号的时候  即01窗口的时候做如下处理
		//取起始号向右的串。再在子串中找终止标识
		retStr  = strObj.Right(strObj.GetLength() - nStartSel -2);
		nEndSel = retStr.Find(strEnd);
		
		retStr  = retStr.Left(nEndSel);
	}
	
	return retStr;
}

//////////////////////////////////////////////////////////////////////////
//函数功能：返回指定传
//函数参数：源串, 起始标记，结束标记
//函数返回：源串中从起始标记开始到结束标记中间的串
//编 写 人：LiuPC
//编写日期: 2009/07/06
//////////////////////////////////////////////////////////////////////////
CString  ReturnStringA(const CString& _strSrc, const CString& _strStartFlag, const CString& _strEndFlag)
{
	CString   retStr   = _T("");
	CString   strObj   = _strSrc;
	CString   strStart = _strStartFlag;
	CString   strEnd   = _strEndFlag;
	
	//起始位置索引
	int nStartSel = strObj.Find( strStart );
	if ( nStartSel == -1 )
	{
		TRACE0("Start Flag Values Is NULL!");
		return _T("");
	}
	
	//终止位置索引
	int nEndSel   = strObj.Find( strEnd );
	if ( nEndSel == -1 )
	{
		TRACE0("End Flag Values Is NULL");
		return _T("");
	}
	
	//当终止索引大于起始索引取中间信息
	if ( nEndSel > nStartSel )
	{
		retStr = strObj.Left(nEndSel);
		retStr = retStr.Right( retStr.GetLength() - nStartSel - 1);
	}
	else //否则 (如果出现起始位置大于终止位置的情况例如 请A001号到01号窗口办理)
	{
		//当取到和号的时候  即01窗口的时候做如下处理
		//取起始号向右的串。再在子串中找终止标识
		retStr  = strObj.Right(strObj.GetLength() - nStartSel -1);
		nEndSel = retStr.Find(strEnd);
		
		retStr  = retStr.Left(nEndSel);
	}
	
	return retStr;
}

CSize GetStringSize(const CString& str, const LOGFONT& font ,CDC* Pdc)
{
	CSize	size(0,0);		
	
	CFont	f;
	f.CreateFontIndirect( &font );
	
	CFont* pOldFont = Pdc->SelectObject( &f );
	
	size = Pdc->GetOutputTextExtent( str, str.GetLength() );
	
	Pdc->SelectObject( pOldFont );
	f.DeleteObject();
	return size;
}

int		GetHeightFontSize(CSize & size1,const int& nwidth)
{
	
		int nHs=size1.cx/nwidth;
		return (nHs+1)*size1.cy;
}

int		GetSrand(const int& Max)		//取0 到 100 随便数
{			
	time_t t;	
	srand((unsigned) time(&t));	
	return	rand() % Max;
}

CString		GetOnlyID()
{
	CString m_id;
	CTime now=CTime::GetCurrentTime();
	static int nCount;
	m_id.Format(_T("%04d%02d%02d%02d%02d%02d%04d%04d"),now.GetYear(),now.GetMonth(),now.GetDay(),now.GetHour(),now.GetSecond(),now.GetMinute(),GetSrand(9999),++nCount);
	
	return m_id;
}

int			GetDWORDCount(const CString& str)		//获取汉字的个数
{
	
    int count = 0;  
    int i=0;  
    for(i=0;i<str.GetLength();i++)   
    {   
        if((BYTE)str[i]>0x80)//if((BYTE)str[i]>= 0xa1 && (BYTE)str[i]<= 0xfe)   
        {  
            count++;  
            i++;  
        }         
    }   
	return count;
}

void ConvertUnicode(char* pUnicode) 
{
	char *pWinId = NULL;
	DWORD dwNumNum = 0;
	dwNumNum = WideCharToMultiByte(CP_OEMCP,NULL, (LPCWSTR)pUnicode, -1, NULL, 0, NULL, FALSE);	//返回号码长度
	dwNumNum++;
	pWinId = new char[dwNumNum];
	
	WideCharToMultiByte(CP_OEMCP, NULL, (LPCWSTR)pUnicode, -1, pWinId, dwNumNum, NULL, FALSE);
	memcpy(pUnicode, pWinId, dwNumNum);
	delete[] pWinId;
}

void ConvertAsni(char* pAsni, CString strInfo)
{
	char pstrInfo[256]={0};
	WideCharToMultiByte(CP_ACP,0,strInfo,-1,pstrInfo,0,NULL,FALSE);
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pstrInfo, -1, NULL, 0);
	unsigned short *wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);

	char pwszGBK[255]={0};
	memcpy(pwszGBK,wszGBK,sizeof(wszGBK));
	wchar_t wpszGBK[255]={0};
	MultiByteToWideChar(CP_ACP,0,pwszGBK,-1,wpszGBK,0);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pstrInfo, -1, wpszGBK, len);
	WideCharToMultiByte(CP_ACP,0,wpszGBK,-1,pwszGBK,0,NULL,FALSE);
	memcpy(pAsni, pwszGBK,255);
	delete[] wszGBK;
}


CString FindExtension( const CString& name )
{
	int len = name.GetLength();
	int i;
	for ( i = len-1; i >= 0; i-- )
	{
		if (name[i] == '.')
			return name.Mid(i+1);
	}
	return CString("");
}
/*
int FindFormat( const CString& ext )
{
	int type = 0;
	if (ext == "bmp")					type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
	else if (ext=="jpg"||ext=="jpeg")	type = CXIMAGE_FORMAT_JPG;
#endif
	else type = CXIMAGE_FORMAT_UNKNOWN;
	
	return type;
}
*/

BOOL IsNumber( LPCTSTR pszText )
{
//	ASSERT_VALID_STRING( pszText );	
	for( int i = 0; i < lstrlen( pszText ); i++ )
		if( !_istdigit( pszText[ i ] ) )
			return false;
		
		return true;
}
BOOL	OpenShowProcess(const CString& path)
{
	if( path.IsEmpty() ) return FALSE;
	CString path_ = path;	

		SHELLEXECUTEINFO	exeInfo = {0};		
		exeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		exeInfo.fMask = SEE_MASK_FLAG_NO_UI|SEE_MASK_NOCLOSEPROCESS;
		exeInfo.hwnd = NULL;
		exeInfo.lpVerb = _T("open");
		exeInfo.lpFile = path_.GetBuffer(0);
		exeInfo.lpParameters = NULL; 
		exeInfo.lpDirectory = NULL;
		exeInfo.nShow = SW_HIDE;
		
		ShellExecuteEx( &exeInfo );						
	
	return true;
}
HWND OpenShowProcess(const CString& path, const CString& wndName )
{
	if( path.IsEmpty() || wndName.IsEmpty() ) return NULL;
	CString path_ = path;
	
	HWND hWnd = ::FindWindow( NULL, wndName );
	if( NULL == hWnd )
	{
		SHELLEXECUTEINFO	exeInfo = {0};
		
		exeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		exeInfo.fMask = SEE_MASK_FLAG_NO_UI|SEE_MASK_NOCLOSEPROCESS;
		exeInfo.hwnd = NULL;
		exeInfo.lpVerb = _T("open");
		exeInfo.lpFile = path_.GetBuffer(0);
		exeInfo.lpParameters = NULL; 
		exeInfo.lpDirectory = NULL;
		exeInfo.nShow = SW_HIDE;
		
		ShellExecuteEx( &exeInfo );
		
		int	nRecycle = 0;
		while( 1 )
		{
			if( hWnd = ::FindWindow( NULL, wndName ) )
			{
//#ifndef _DEBUG
				Sleep( 1000 );
//#endif
				break;
			}
			
			nRecycle ++;
			if( nRecycle > 20 ) break;
			
			Sleep( 30 );
		}
		
		
	}
	return hWnd;
}

int GetCutString(CString strSource, int setLength)
{
	int len = 0;
	if(strSource.GetLength() <= setLength) //字符串长度比设定截取值小时，直接返回字符串的长度 
		return strSource.GetLength();
	
	for(int i=0; i<setLength; i++)
	{
		if(strSource[len]<0 || strSource[len]>255)
		{
			if((len+1)==setLength) break;
			len = len + 2;
		}
		else
			len = len + 1;
		if(len >= setLength)
			break;
	}
	return len;
}

//GBK转化为UTF8
void ConvertGBKToUtf8_(CString& strGBK) 
{
	char pstrGBK[256]={0};
	WideCharToMultiByte(CP_ACP,0,strGBK,-1,pstrGBK,0,NULL,FALSE);
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pstrGBK, -1, NULL,0);
	unsigned short * wszUtf8 = new unsigned short[len+1];
	memset(wszUtf8, 0, len * 2 + 2);
	char tempwszUtf8[255]={0};
	memcpy(tempwszUtf8,wszUtf8,len*2+2);

	wchar_t wpwszUtf8[255]={0};

	MultiByteToWideChar(CP_ACP,0,tempwszUtf8,-1,wpwszUtf8,0);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pstrGBK, -1, wpwszUtf8, len);
	
	len = WideCharToMultiByte(CP_UTF8, 0, wpwszUtf8, -1, NULL, 0, NULL, NULL); 
	char *szUtf8 = new char[len + 1];
	memset(szUtf8, 0, len + 1);
	wchar_t pwszUtf8[255]={0};
	MultiByteToWideChar(CP_ACP,0,tempwszUtf8,-1,pwszUtf8,0);
	WideCharToMultiByte (CP_UTF8, 0, pwszUtf8, -1, szUtf8, len, NULL,NULL);
	
	strGBK = szUtf8;
	delete[] szUtf8;
	delete[] wszUtf8;
}

//UTF8转化为GBK
void ConvertUtf8ToGBK_(CString& strUtf8) 
{
	char pc_strUtf8[255]={0};
	WideCharToMultiByte(CP_ACP,0,strUtf8,-1,pc_strUtf8,0,NULL,FALSE);
	int len=MultiByteToWideChar(CP_UTF8, 0, pc_strUtf8, -1, NULL,0);

	unsigned short * wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);

	char pc_wszGBK[255]={0};
	memcpy(pc_wszGBK,wszGBK,len*2+2);
	wchar_t wpszGBK[255]={0};

	MultiByteToWideChar(CP_UTF8, 0, pc_strUtf8, -1, wpszGBK, len);
	MultiByteToWideChar(CP_ACP,0,pc_wszGBK,-1,wpszGBK,0);

	len = WideCharToMultiByte(CP_ACP, 0, wpszGBK, -1, NULL, 0, NULL, NULL); 
	char *szGBK=new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, wpszGBK, -1, szGBK, len, NULL,NULL);
	
	strUtf8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
}

char*	UnicodeToHexString(char *pSrc,int size)
{
	if(pSrc ==NULL)
		return "";
	int nSrclength = size;
	int  nDstlength;
	char buf0[128];
	char buf1[128];
	char pDst[128];
	char t[1024];
	memset(t,0,sizeof(t));
	wchar_t wchar[128];
	memset(wchar,0,sizeof(wchar));
	nDstlength = MultiByteToWideChar(CP_ACP,0,pSrc,nSrclength,wchar,128);
	
	static char p[128];
	memset(p,0,128);
	memset(buf0,0,128);
	memset(buf1,0,128);
	memset(pDst,0,128);
	for(int i =0;i<nDstlength;i++)
	{

		sprintf(buf0,"%02x",wchar[i]>>8);
		sprintf(buf1,"%02x",wchar[i]&0xff);
		strcat(buf0,buf1);
		strcat(p,buf0);
	}
	return p;
}

int Calc_CRC16CCNET( unsigned char *DataBuf, unsigned int BufLen )
{
	int i;
	char crc_h, crc_l;
	
	crc_h = crc_l = 0;
	for( i=0; i<(int)BufLen; i++ )
	{
		calc_crc( DataBuf[i], (unsigned char*)&crc_h, (unsigned char*)&crc_l );
	}
	
	i = (unsigned char)(crc_h);
	i <<= 8;
	i += (unsigned char)(crc_l);
  return i;
}

void calc_crc(unsigned char byte,  unsigned char *inter_crc_h,  unsigned char *inter_crc_l)
{
	char i,c;
	int temp_crc;
	
	(*inter_crc_h) ^= byte;
	temp_crc = *inter_crc_l;
	temp_crc <<= 8;
	temp_crc |= *inter_crc_h;
	for( i=0; i<8; i++ )
	{
		c = temp_crc & 0x01;
		temp_crc >>= 1;
		if( c )
		{
			temp_crc ^= 0X08408;
		}
	}
	
	*inter_crc_l = temp_crc >> 8;
  *inter_crc_h = temp_crc;
}

//将字符转byte 如:"11111111" 转 byte
unsigned char Char8ToByte(CString str)
{
	unsigned char i;
	unsigned char ch;
	unsigned char retCh;
	
	retCh = 0;
	if( 8 != str.GetLength()  )
	{
		return retCh;
	}
	
	for( i=0; i<8; i++ )
	{
//		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)str.GetAt(i),-1,ch,0,NULL,FALSE);
		
//		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)str.GetAt(i),-1,(LPSTR)ch,1,NULL,NULL);
		ch=str.GetAt(i);
		if( '1' == ch )
		{
			retCh |= 1<<(7-i);
		}
	}
  return retCh;
}

byte	char16toByte(char cdata)
{
	byte	bdata;
	if(cdata<='F' && cdata>='A')
	{
		bdata=cdata-55;
	}
	else if(cdata<='f' && cdata>='a')
	{
		bdata=cdata-87;
	}
	else if(cdata<='9' && cdata>='0')
	{
		bdata=cdata-48;
	}
	return bdata;

}


CString DbcToSbc(CString stringText)
{
	if(stringText.IsEmpty()) return _T("");
	CString STR1=stringText;		
	byte buf[3];
	if((byte)STR1[0]<0X80)	
	{
		
			buf[0]=(byte)0xA3;
			buf[1]=(byte)STR1[0]+0x80;
			buf[2]='\0';
			
		
	}
	wchar_t wpbuf[6]={0};
	char pbuf[255]={0};
	memcpy(pbuf,buf,sizeof(pbuf));
	MultiByteToWideChar(CP_ACP,0,pbuf,-1,wpbuf,0);
	STR1.Format(_T("%s"),wpbuf);
	return STR1;
}





BOOL SetPrivilege(
                  HANDLE hToken,          // access token handle
                  LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
                  BOOL bEnablePrivilege   // to enable or disable privilege
                  ) 
{
    TOKEN_PRIVILEGES tp;
    LUID luid;
	
    if ( !LookupPrivilegeValue( 
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid ) )        // receives LUID of privilege
    {
        printf("LookupPrivilegeValue error: %u/n", GetLastError() ); 
        return FALSE; 
    }
	
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if ( bEnablePrivilege )
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;
	
    // Enable the privilege or disable all privileges.
	
    if ( !AdjustTokenPrivileges(
        hToken, 
        FALSE, 
        &tp, 
        sizeof(TOKEN_PRIVILEGES), 
        (PTOKEN_PRIVILEGES) NULL, 
        (PDWORD) NULL) )
    { 
        printf("AdjustTokenPrivileges error: %u/n", GetLastError() ); 
        return FALSE; 
    } 
	
    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		
    {
        printf("The token does not have the specified privilege. /n");
        return FALSE;
    } 
	
    return TRUE;
}


HANDLE GetProcessHandle(int nID)
{
    HANDLE hToken;
    BOOL flag = OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken );
    if( !flag )
    {
        DWORD err = GetLastError();
        printf( "OpenProcessToken error:%d", err );
    }
    SetPrivilege( hToken, SE_DEBUG_NAME, true );
    CloseHandle(hToken);
    return OpenProcess(PROCESS_ALL_ACCESS, FALSE, nID);
}
HANDLE GetProcessHandle(LPCTSTR pName)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        return NULL;
    }
    PROCESSENTRY32 pe = { sizeof(pe) };
    BOOL fOk;
    for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
    {
        if (!_tcscmp(pe.szExeFile, pName))
        {
            CloseHandle(hSnapshot);
            return GetProcessHandle(pe.th32ProcessID);
        }
    }
    return NULL;
}


BOOL IsProcessExist(CString ProcessName, int& dwProcID,BOOL bKill /*= FALSE*/)
{
	
	BOOL ProcessExist=false;
	PROCESSENTRY32 my;  //用来存放快照进程信息的一个结构体
	HANDLE l = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //创建进程快照
	if (((int)l) != -1)
	{
		
		my.dwSize = sizeof(my); //必须这样初始化，否则调用Process32First失败
		if (Process32First(l, &my)) //获得进程快照中的第一个进程
		{
			do
			{
				//CharLowerBuff(my.szExeFile, MAX_PATH);//转换成小写字母
				if(_tcscmp(ProcessName ,my.szExeFile) == 0)  //查找对应进程
				{
					
					if (bKill)
					{
						HANDLE   hProcess; 
						hProcess=GetProcessHandle(my.th32ProcessID); 
						if   (hProcess) 
						{ 
							dwProcID = 0;
							TerminateProcess(hProcess,0);//关闭进程 
							
							ProcessExist = TRUE;
						} 
					}
					else
					{
						dwProcID = my.th32ProcessID;//获取进程ID
						// ProcHWND = GetHWND(dwProcID);//获取窗口句柄
						ProcessExist=TRUE;
					}
					
					break;
				}
				else
				{
					ProcessExist=FALSE;
				}
			}while (Process32Next(l, &my)); //获取下一个进程
			
		}
		CloseHandle(l);
	}
	return ProcessExist;
}


BOOL ShutDownOS_(BOOL Reboot)
{
	// 给所有程序发送关闭的消息
	//	::PostMessage( HWND_BROADCAST, WM_CLOSE, 0, 0 );
	//	Sleep( 200 );
	//关闭系统
	
	// windows 
	
	
		if(Reboot)
		{
			return ::ExitWindowsEx(EWX_REBOOT,0);    
		}
		else
		{
			return ::ExitWindowsEx( EWX_SHUTDOWN , 0 );
		}
	
	return TRUE;
}