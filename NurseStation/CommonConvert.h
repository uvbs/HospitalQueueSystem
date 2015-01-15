#pragma once
////用于转换字符串的类
#include <string>
using namespace std;
class CCommonConvert
{
public:
	CCommonConvert(void);
	~CCommonConvert(void);
	static int CStringToChar(CString str,char c_str[]);
	static int CharToCstring(CString& str,char c_str[]);
	static void intToCString(int i,CString& str);
	static void CStringToint(int& i,CString str);
	static BOOL WChar2Char(char* dstBuf, size_t len, const wchar_t* src);
	//获取本机IP
	CString GetComputerIP();
	//获取程序执行路径
	CString GetExeFullFilePath();
	////UTF8转std::string
	static std::string UTF8_To_string(const std::string & str);
	////std::string转utf8字符串
	static std::string string_To_UTF8(const std::string & str); 
private:
	CString m_strPath;
};
