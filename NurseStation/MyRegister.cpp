#include "StdAfx.h"
#include "MyRegister.h"
#include "GetDeviceSerialId.h"

MyRegister::MyRegister(void)
{
}

MyRegister::~MyRegister(void)
{
}

BOOL MyRegister::GetSerialNo(CString& strSerialNo)
{
	GetDeviceSerialId getId;
	strSerialNo = getId.GetHdSnHex2();
	if(strSerialNo.IsEmpty())
	{
		return FALSE;
	}
	strSerialNo = strSerialNo.Mid(2, 10);
	if(strSerialNo.GetLength() != 10)
	{
		return FALSE;
	}
	strSerialNo = _T("2014-") + strSerialNo;
	return TRUE;
}

BOOL MyRegister::CalcSerialNo(CString strSrc, CString& strSerialno)
{
	if(strSrc.Left(4) != _T("2014"))
	{
		//MessageBox(_T("机器码与注册器版本不一致！"), _T("错误"));
		return FALSE;
	}
	strSrc = strSrc.Mid(5);
	//MessageBox(strCpuId);
	if(strSrc.GetLength() != 10)
	{
		//MessageBox(_T("机器码个数不正确！"), _T("错误"));
		return FALSE;
	}
	//strSrc.Mid(3, 10);
	CHAR szKey[10];
	szKey[0] = (CHAR)(strSrc.GetAt(0) / 2 + 100);
	szKey[1] = (CHAR)(strSrc.GetAt(1) / 2 + 50);
	szKey[2] = (CHAR)(strSrc.GetAt(2) * 3 - 42);
	szKey[3] = (CHAR)(strSrc.GetAt(3) - 10);
	szKey[4] = (CHAR)(strSrc.GetAt(4) / 3 + 7);
	szKey[5] = (CHAR)(strSrc.GetAt(5) - 25);
	szKey[6] = (CHAR)(strSrc.GetAt(6) + 89);
	szKey[7] = (CHAR)(strSrc.GetAt(7) * 2 + 8);
	szKey[8] = (CHAR)(strSrc.GetAt(8) + 90);
	szKey[9] = (CHAR)((strSrc.GetAt(9) - 23) /3 + 9);

	strSerialno.Empty();
	CString str4Hexs;
	//int iKey;
	for(int i = 0; i < 10; i++)
	{
		//str4Hexs.Format(_T("%X"), szKey[i]);
		//iKey = (i * 725) % str4Hexs.GetLength();
		//strSerialno.AppendFormat(_T("%c"), str4Hexs.GetAt(iKey));
		//iKey = (i * 402) % str4Hexs.GetLength();
		//strSerialno.AppendFormat(_T("%c"), str4Hexs.GetAt(iKey));
		//strSerialno.AppendFormat(_T("%02X"), szKey[i]);
		str4Hexs.Format(_T("%02X"), szKey[i]);
		strSerialno += str4Hexs.Right(2);
	}
	strSerialno = strSerialno.Mid(7, 12);

	return TRUE;
}
/*
BOOL MyRegister::VerifySerialNo()
{
	CString strSerialNoFromReg, strSerialNoCalc;
	if(!GetSerialNo(strSerialNoCalc))
	{
		return FALSE;
	}
	if(!CalcSerialNo(strSerialNoCalc, strSerialNoCalc))
	{
		return FALSE;
	}
	if(!ReadSerialNo(strSerialNoFromReg))
	{
		return FALSE;
	}
	if(strSerialNoCalc.Compare(strSerialNoFromReg) != 0)
	{
		return FALSE;
	}
	return TRUE;

}

BOOL MyRegister::ReadSerialNo(CString& strSerialNo)
{
	HKEY hKey;
	LONG ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		_T("SOFTWARE\\NurseSystem\\"), 0, KEY_READ, &hKey);
	if(ERROR_SUCCESS != ret)
	{
		return FALSE;
	}
	WCHAR wszSerialNo[128] = {0};
	DWORD dType = REG_SZ;
	DWORD dLength = 128;
	ret = ::RegQueryValueEx(hKey, _T("SerialNo"), NULL, &dType, (LPBYTE)wszSerialNo, &dLength);
	//ret = ::RegGetValue(hKey, NULL, _T("SerialNo"), REG_SZ, NULL, wszSerialNo, NULL);
	::RegCloseKey(hKey);
	//AfxMessageBox(wszSerialNo);
	if(ERROR_SUCCESS != ret)
	{
		return FALSE;
	}
	strSerialNo.Format(_T("%s"), wszSerialNo);

	return TRUE;
}
*/
