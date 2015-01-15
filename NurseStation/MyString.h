#pragma once

class MyString
{
public:
	MyString(void);
	~MyString(void);

	/**
	desc:		宽字节字符串 转 单字节字符串

	dstBuf:		目标字符串缓冲区
	src:		源字符串

	return:		成功，返回 TRUE;否则，返回 FALSE
	*/
	static BOOL WChar2Char(char* dstBuf, size_t len, const wchar_t* src);

	/**
	desc:		单字节字符串 转 宽字节字符串

	dstBuf:		目标字符串缓冲区
	src:		源字符串

	return:		成功，返回 TRUE;否则，返回 FALSE
	*/
	static BOOL Char2WChar(wchar_t* dstBuf,size_t len,const char *src);

	/**
	desc:		在字符串中查找子字符串第一次出现的下标
	strSrc:		源字符串
	strFind:	要查找的字符串

	return:		查找成功，返回所查找字符串第一次出现的下标；
	否则，返回 -1
	*/
	static int Find(CString strSrc, CString strFind);

	/**
	desc:		在字符串中查找子字符串第一次出现的下标, 不区分大小写
	strSrc:		源字符串
	strFind:	要查找的字符串

	return:		查找成功，返回所查找字符串第一次出现的下标；
	否则，返回 -1
	*/
	static int FindNoCase(CString strSrc, CString strFind);

	static int ReverseFind(CString strSrc, CString strFind);

	static int ReverseFindNoCase(CString strSrc, CString strFind);

	/**
	desc:		将源字符串中的字符串替换成新字符串

	strSrc:		源字符串
	strFrom:	要替换的字符串
	strTo:		用来替换的新字符串

	return:		返回替换后的新字符串
	*/
	static CString Replace(CString strSrc, CString strFrom, CString strTo);

	/**
	desc:			将字符串根据分隔符分割成字符串数组, 
	分隔符可以是一个字符，也可以是多个字符。
	例如，_T("\n") 或 _T(" ") 或 _T(" \n,.。")

	strSrc:			源字符串
	arrStrDest:		字符串数组，保存分割好的字符串
	strDivisionSet:	分隔符集

	return:			成功，返回 TRUE；否则，返回 FALSE
	*/
	static BOOL Split(CString strSrc, CStringArray& arrStrDest, CString strDivisionSet);

	static int FindCharNoCase(CString& strSrc, wchar_t chFind);

	static int ReverseFindCharNoCase(CString& strSrc, wchar_t chFind);

	/**
	desc:		比较两个宽字符是否相等, 不区分大小写
	return:		相等，返回 TRUE；否则，返回 FALSE
	*/
	static BOOL CompareCharNoCase(wchar_t ch1, wchar_t ch2);

	/**
	desc:				在字符串中获取属性值

	strSrc:				源字符串
	strProfileName:		要获取的属性名
	strProfileValue:	保存获取到的属性值

	return:				成功，返回 TRUE；否则，返回 FALSE
	*/
	static BOOL GetProfileFromString(CString strSrc, CString strProfileName, 
		CString& strProfileValue, wchar_t wchSplitNameValue, wchar_t wchSplitProfile);

	/**
	desc:				在字符串中获取属性值，不区分属性名大小写

	strSrc:				源字符串
	strProfileName:		要获取的属性名
	strProfileValue:	保存获取到的属性值

	return:				成功，返回 TRUE；否则，返回 FALSE
	*/
	static BOOL GetProfileFromStringNoCase(CString strSrc, CString strProfileName, 
		CString& strProfileValue, wchar_t wchSplitNameValue, wchar_t wchSplitProfile);

	/**
	desc:		整型数字字符串 转成 整型数字

	strNum:		数字字符串

	return:		返回 转换好的整型数字
	*/
	static int Str2Int(CString strNum);

	/**
	desc:		整型数字 转成 整型数字字符串 

	iNum:		整型数字

	return:		返回 转换好的整型数字字符串
	*/
	static CString Int2Str(int iNum);
};
