#include "StdAfx.h"
#include "MyString.h"

MyString::MyString(void)
{
}

MyString::~MyString(void)
{
}

BOOL MyString::WChar2Char(char* dstBuf, size_t len, const wchar_t* src)
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

BOOL MyString::Char2WChar(wchar_t *dstBuf, size_t len, const char *src)
{
	if(src == NULL) return FALSE;

	size_t chlen = strlen(src);
	size_t nBytes =(size_t)MultiByteToWideChar(CP_ACP, 0,
		(const char *)src,(int)chlen,NULL,0);
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
	nBytes = MultiByteToWideChar(CP_ACP, 0, 
		(const char *)src,(int)chlen,dstBuf,(int)nBytes);
	dstBuf[iLastIndex]=0;
	if(nBytes == 0) return FALSE;
	return TRUE;
}

int MyString::Find(CString strSrc, 
							 CString strFind)
{
	int nSrcLength = strSrc.GetLength();
	int nFindLength = strFind.GetLength();
	if(nSrcLength == 0 || nFindLength == 0) return -1;
	int nAbsStart = 0;
	int nStart = -1;
	for(; (nStart = strSrc.Find(strFind.GetAt(0))) != -1;)
	{
		int i = 0, j = nStart;
		BOOL bRes = TRUE;
		for(; i < nFindLength - 1;)
		{
			if(strSrc.GetAt(++j) != strFind.GetAt(++i))
			{
				strSrc = strSrc.Mid(nStart+1);
				nAbsStart += nStart+1;
				nStart = -1;
				bRes = FALSE;
				break;
			}
		}
		if(i == nFindLength - 1 && bRes)
		{
			nAbsStart += nStart;
			return nAbsStart;
		}
	}
	return -1;
}

int MyString::FindNoCase(CString strSrc, 
								   CString strFind)
{
	int nSrcLength = strSrc.GetLength();
	int nFindLength = strFind.GetLength();
	if(nSrcLength == 0 || nFindLength == 0) return -1;
	int nAbsStart = 0;
	int nStart = -1;
	for(; (nStart = FindCharNoCase(strSrc, strFind.GetAt(0))) != -1;)
	{
		int i = 0, j = nStart;
		BOOL bRes = TRUE;
		for(; i < nFindLength - 1;)
		{
			if(!CompareCharNoCase(strSrc.GetAt(++j),strFind.GetAt(++i)))
			{
				strSrc = strSrc.Mid(nStart+1);
				nAbsStart += nStart+1;
				nStart = -1;
				bRes = FALSE;
				break;
			}
		}
		if(i == nFindLength - 1 && bRes)
		{
			nAbsStart += nStart;
			return nAbsStart;
		}
	}
	return -1;
}

int MyString::ReverseFind(CString strSrc, 
									CString strFind)
{
	int nFindLength = strFind.GetLength();
	if(strSrc.GetLength() == 0 || nFindLength == 0) return -1;
	int nIndex = - 1;
	for(; (nIndex = strSrc.ReverseFind(strFind.GetAt(nFindLength-1))) != -1;)
	{
		if(nIndex > strSrc.GetLength() - nFindLength) break;
		int i = nFindLength - 1;
		BOOL bRes = TRUE;
		for(; i > 0; )
		{
			if(strSrc.GetAt(--nIndex) != strFind.GetAt(--i))
			{
				strSrc = strSrc.Left(nIndex);
				nIndex = -1;
				bRes = FALSE;
				break;
			}
		}
		if(i == 0 && bRes)
		{
			return nIndex;
		}
	}
	return -1;
}

int MyString::ReverseFindNoCase(CString strSrc, 
										  CString strFind)
{
	//int nSrcLength = strSrc.GetLength();
	int nFindLength = strFind.GetLength();
	if(strSrc.GetLength() == 0 || nFindLength == 0) return -1;
	int nIndex = - 1;
	for(; (nIndex = ReverseFindCharNoCase(strSrc, strFind.GetAt(nFindLength-1))) != -1;)
	{
		if(nIndex > strSrc.GetLength() - nFindLength) break;
		int i = nFindLength - 1;
		BOOL bRes = TRUE;
		for(; i > 0; )
		{
			if(!CompareCharNoCase(strSrc.GetAt(--nIndex),strFind.GetAt(--i)))
			{
				strSrc = strSrc.Left(nIndex);
				nIndex = -1;
				bRes = FALSE;
				break;
			}
		}
		if(i == 0 && bRes)
		{
			return nIndex;
		}
	}

	return -1;
}

CString MyString::Replace(CString strSrc, 
									CString strFrom, 
									CString strTo)
{
	int nLength = strFrom.GetLength();
	CString strSrcTemp = strSrc;
	int nAbsStart = 0;
	//BOOL bFound = TRUE;
	BOOL bFound = FALSE;
	int nStart = -1;
	for(; (nStart = strSrcTemp.Find(strFrom.GetAt(0))) != -1;)
	{
		BOOL b = TRUE;
		int i = 0, j = nStart;
		for(; i < nLength-1;)
		{
			if(strSrcTemp.GetAt(++j) != strFrom.GetAt(++i))
			{
				strSrcTemp = strSrcTemp.Mid(nStart+1);
				nAbsStart += nStart+1;
				nStart = -1;
				b = FALSE;
				break;
			}
		}
		//if(i == nLength-1)
		if(b)
		{
			bFound = TRUE;
			nAbsStart += nStart;
			break;
		}
		//if(bFound)
		//{
		//	nAbsStart += nStart;
		//	break;
		//}
	}
	if(bFound == FALSE)
	{
		return strSrc;
	}
	CString strDest = strSrc.Left(nAbsStart);
	strDest += strTo;
	strDest += strSrc.Mid(nAbsStart + nLength);

	return Replace(strDest, strFrom, strTo);
}

BOOL MyString::Split(CString strSrc, 
							   CStringArray& arrStrDest, 
							   CString strDivisionSet)
{
	if(strSrc.IsEmpty() || strDivisionSet.IsEmpty())
	{
		return FALSE;
	}
	int nIndex = -1;
	for(; (nIndex = strSrc.FindOneOf(strDivisionSet)) != -1;)
	{
		CString str = strSrc.Left(nIndex);
		arrStrDest.Add(str);
		strSrc = strSrc.Mid(nIndex+1);
	}
	if(!strSrc.IsEmpty())
	{
		arrStrDest.Add(strSrc);
	}
	return TRUE;
}

int MyString::FindCharNoCase(CString& strSrc, wchar_t chFind)
{
	int nRes = -1;
	nRes = strSrc.Find(chFind);
	if(nRes == -1)
	{
		if(chFind >= 'A' && chFind <= 'Z')
			chFind = 'a' + chFind - 'A';
		else if(chFind >= 'a' && chFind <= 'z')
			chFind = 'A' + chFind - 'a';
		nRes = strSrc.Find(chFind);
	}
	return nRes;
}

int MyString::ReverseFindCharNoCase(CString& strSrc, wchar_t chFind)
{
	int nRes = -1;
	nRes = strSrc.ReverseFind(chFind);
	if(nRes == -1)
	{
		if(chFind >= 'A' && chFind <= 'Z')
			chFind = 'a' + chFind - 'A';
		else if(chFind >= 'a' && chFind <= 'z')
			chFind = 'A' + chFind - 'a';
		nRes = strSrc.Find(chFind);
	}
	return nRes;
}

BOOL MyString::CompareCharNoCase(wchar_t ch1, wchar_t ch2)
{
	BOOL bRes = FALSE;
	bRes = ch1 == ch2;
	if(!bRes)
	{
		if(ch2 >= 'A' && ch2 <= 'Z')
		{
			ch2 = 'a' + ch2 - 'A';
		}
		else if(ch2 >= 'a' && ch2 <= 'z')
		{
			ch2 = 'A' + ch2 - 'a';
		}
		bRes = ch1 == ch2;
	}
	return bRes;
}

BOOL MyString::GetProfileFromString(CString strSrc, 
									   CString strProfileName, 
									   CString& strProfileValue,
									   wchar_t wchSplitNameValue, 
									   wchar_t wchSplitProfile)
{
	strProfileName.AppendFormat(_T("%c"), wchSplitNameValue);
	int nStartIndex = Find(strSrc, strProfileName);
	if(nStartIndex == -1)
	{
		strProfileValue = _T("");
		return FALSE;
	}
	nStartIndex += strProfileName.GetLength();
	int nEndIndex = strSrc.Find(wchSplitProfile, nStartIndex);
	if(nEndIndex == -1)
	{
		nEndIndex = strSrc.GetLength();
	}
	strProfileValue = strSrc.Mid(nStartIndex, nEndIndex-nStartIndex);
	return TRUE;
}

BOOL MyString::GetProfileFromStringNoCase(CString strSrc, 
											 CString strProfileName, 
											 CString& strProfileValue,
											 wchar_t wchSplitNameValue, 
											 wchar_t wchSplitProfile)
{
	strProfileName.AppendFormat(_T("%c"), wchSplitNameValue);
	int nStartIndex = FindNoCase(strSrc, strProfileName);
	if(nStartIndex == -1)
	{
		strProfileValue = _T("");
		return FALSE;
	}
	nStartIndex += strProfileName.GetLength();
	int nEndIndex = strSrc.Find(wchSplitProfile, nStartIndex);
	if(nEndIndex == -1)
	{
		nEndIndex = strSrc.GetLength();
	}
	strProfileValue = strSrc.Mid(nStartIndex, nEndIndex-nStartIndex);
	return TRUE;
}

int MyString::Str2Int(CString strNum)
{
#ifdef UNICODE
	char buf[32] = {0};
	MyString::WChar2Char(buf, 32, strNum.GetBuffer());
	return atoi(buf);
#else
	return atoi(strNum.GetBuffer());
#endif
}

CString MyString::Int2Str(int iNum)
{
	CString strNum;
	strNum.Format(_T("%d"), iNum);
	return strNum;
}
