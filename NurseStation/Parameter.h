/////////////////////////////////////////////////////////////////////
//版权: 博瑞丰应用软件开发部
//作者: 邓坦周				
//内容: 对字符串进行分析，以取得参数
//日期: 02.10.26
/////////////////////////////////////////////////////////////////////

#ifndef _PARAMETER__H_
#define _PARAMETER__H_

// 注意：用 // 代替 / 内容里面的符号
//#include "D:\\work\\cximage\\CxImage\\ximage.h"
//#pragma comment(lib, "D:\\work\\cximage\\CxImage\\CxImageDLL\\Release\\cximage.lib" )

CString NewUrlEncode( const CString& strIn);

CString toParam( const CString& param );

// 格式：/id 12
int RequestParam( const CString  &strParam, const CString  &name );
// 格式 /name abc 
CString RequestParamStr( const CString &strParam, const CString &name ); 

#ifndef UNDER_CE
// 格式 /width 3.3
float RequestParamF( const CString &strParam, const CString &name );
#endif // UNDER_CE

// 返回 0 表示正确，其他的错误， 格式 12,45,32,-1,23, 
int RequestParams( const CString& ini, CDWordArray& array, const CString &sep=_T(","));
// 返回 0 表示正确，其他的错误， 格式 abc|def|dir adf|adsfasdf //bWithSpace TRUE 删除空格 //bSpace True 分格符空的也计算
int RequestParamsStr( const CString& ini,CStringArray& array, const CString &sep=_T("``"),
					 BOOL bWithSpace = FALSE ); 
int RequestParamsStrofSpace( const CString& ini,CStringArray& array, const CString &sep=_T("``"),
					 BOOL bWithSpace = FALSE,BOOL bSpace=FALSE ); 
CString	logFontToString(const LOGFONT& font );
BOOL	StringTologFont(const CString& str, LOGFONT& font );

// guid to string
BOOL		StringToGuid( const CString& str, GUID &guid );
CString		GuidToString( const GUID& guid );


int GetDaysInMonth(const CTime& time );


CString DayTimeToString(const CTime& time );
CString DayTimeToStandString(const CTime& time );
CString DayToString(const CTime& time);
CString TimeToString( const CTime& time,const CString & Symbol=_T("," ));
void	DayTimeToStandStr( const CTime& time, CString& str );
//void	StandToDayTime(const CString& strtime,CTime & time);
CString	TimeStrToStandStr(const CString& str);	//20110101 转 1011-01-01
void	OleDateTimeToStandStr(const COleDateTime& time, CString & str);
void	TimeToOleDateTime(const CTime& Time,COleDateTime & OleTime);
void	OleDateTimeToTime(const COleDateTime& oleTime, CTime& Time);
void	 TimetoOleDateTime(const CTime& Time,COleDateTime & time2);

void	SetSysTime(const CString& time);

BOOL	StringToDayTime( const CString& str, CTime& time );
BOOL	StringToDay(const CString& str, CTime& time);
BOOL	StringToDay(const CString& str, CTime& time,const CString & Symbol);
BOOL	StringToTime(const CString& str, CTime& time, const CTime& day=CTime(1977,7,7,7,7,7),const CString & Symbol=_T(","));
BOOL	StringToStandStr(const CString& str,CTime& time);

BOOL	IsSameDay( const CTime& t1, const CTime& t2 );
int		TimeCompareTime(const CTime& t1, const CTime& t2 );	//t1>t2=1,t1=t2=0,t1<t2=-1
DWORD	IpStringToDWORD( const CString& strAddr );

CString inttoCString( int a) ;

void CStringtobyte(const CString & str,BYTE * pb);
byte* intToByte(int i);			//整型转成网络字节
int bytesToInt(byte* bytes);	//byte 网络字节转成整型

//小刘增加的函数
CString  ReturnStringW(const CString& _strSrc, const CString& _strStartFlag, const CString& _strEndFlag);
CString  ReturnStringA(const CString& _strSrc, const CString& _strStartFlag, const CString& _strEndFlag);

CSize GetStringSize(const CString& str, const LOGFONT& font ,CDC* Pdc); // xj add 
int		GetHeightFontSize(CSize & size1,const int& nwidth);

int		GetSrand(const int& Max);		//取0 到 Max 随机数
CString		GetOnlyID();		//取维一的ID号
int			GetDWORDCount(const CString& str);		//获取汉字的个数
void ConvertUnicode(char* pUnicode);
 void ConvertAsni(char* pAsni, CString strInfo);
 //GBK转化为UTF8
void ConvertGBKToUtf8_(CString& strGBK);
//UTF8转化为GBK
void ConvertUtf8ToGBK_(CString& strUtf8) ;
///把字符串转成unicode 16进制形式
char* UnicodeToHexString(char *pSrc,int size);


/*************************************************************
*函数名称：Calc_CRC16CCNET
*函数功能：计算CRC校验值
*输入参数：DataBuf 数据缓冲区首地址
BufLen   数据长度                             
*输出参数：
*备    注:
**************************************************************/
int Calc_CRC16CCNET( unsigned char *DataBuf, unsigned int BufLen );

void calc_crc(unsigned char byte,  unsigned char *inter_crc_h,  unsigned char *inter_crc_l);
 //int FindFormat( const CString& ext );
CString FindExtension( const CString& name );

BOOL IsNumber( LPCTSTR pszText );
HWND	OpenShowProcess(const CString& path, const CString& wndName );
BOOL	OpenShowProcess(const CString& path);

/*-------------------------------------------------------
//    开发人员：G.Yin
//    编写时间：2006-1-6
//    函数名称：GetCutString(CString strSource, int setLength)
//    返回类型：int
//    功能说明：截取包含汉字的字符串，其中字符串的长度不超过接近预设的setLength长度；
//    如果strSource的长度小于setLength值，直接返回strSource的长度
//    参数说明：
//        --参数名称----输入输出----参数类型----参数说明-------
//        --strSource            输入        CString                待截取的字符串        
//        --setLength            输入        int                    欲截取的字符串的长度  
//-------------------------------------------------------
*/


int GetCutString(CString strSource, int setLength);

unsigned char Char8ToByte(CString str);

//xj 将字符转二进制byte 
byte	char16toByte(char cdata);


//半角转换为全角
CString DbcToSbc(CString stringText);

BOOL SetPrivilege(
                  HANDLE hToken,          // access token handle
                  LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
                  BOOL bEnablePrivilege   // to enable or disable privilege
                  ) ;

HANDLE GetProcessHandle(int nID);

BOOL IsProcessExist(CString ProcessName, int& dwProcID,BOOL bKill /*= FALSE*/);
BOOL ShutDownOS_(BOOL Reboot);
#endif // _PARAMETER__H_
