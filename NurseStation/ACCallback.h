#pragma once

/**************************************************************************
*   ACCallback.h
*   Helper class of Member function callback mechanism
**************************************************************************/
#include "stdafx.h"
#include "windows.h"
#pragma pack(push, 1)
struct _ACCallbackOpCodes
{
	unsigned char tag;  // CALL e8
	LONG_PTR offset;  // offset (dest - src - 5, 5=sizeof(tag + offset))
	LONG_PTR _this;   // a this pointer
	LONG_PTR _func;   // pointer to real member function address
};
#pragma pack(pop)
static __declspec( naked ) int STDACJMPProc()
{
	_asm
	{
		POP ECX  
			MOV EAX, DWORD PTR [ECX + 4] // func
		MOV ECX, [ECX]     // this  
		JMP EAX
	}
}
static LONG_PTR CalcJmpOffset(LONG_PTR Src, LONG_PTR Dest)
{
	return Dest - (Src + 5);
}
/*
* NOTE: _TPStdFunc: a type of function pointer to API or Callbacks, *MUST* be _stdcall
_TPMemberFunc: a type of function pointer to class member function, 
*MUST* be the *DEFAULT* calling conversation, *NO* prefix should be added,
that is, using ECX for "this" pointer, pushing parameters from right to left, 
and the callee cleans the stack.
_TClass: the class who owns the callback function. The caller should only own the _stdcall function pointer
LIFE TIME:  It is important to keep the ACCallback object alive until the CALLBACK is not required!!!
*/
template<typename _TPStdFunc, class _TClass, typename _TPMemberFunc>
class ACCallback
{
public:
	_TClass *m_pThis;
	_TPMemberFunc m_pFunc;
private:
	_TPStdFunc m_pStdFunc;
	void MakeCode()
	{
		if (m_pStdFunc) ::VirtualFree(m_pStdFunc, 0, MEM_RELEASE);
		m_pStdFunc = (_TPStdFunc)::VirtualAlloc(NULL, sizeof(_ACCallbackOpCodes), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		_ACCallbackOpCodes *p = (_ACCallbackOpCodes *)m_pStdFunc;
		p->_func = *(LONG_PTR *)&m_pFunc;
		p->_this = (LONG_PTR)m_pThis;
		p->tag = 0xE8;
		p->offset = CalcJmpOffset((LONG_PTR)p, (LONG_PTR)STDACJMPProc);
	}
public:
	ACCallback<_TPStdFunc, _TClass, _TPMemberFunc>()
	{
	}
	ACCallback<_TPStdFunc, _TClass, _TPMemberFunc>(_TClass* pThis,
		_TPMemberFunc pFunc
		)
	{
		m_pFunc = pFunc;
		m_pThis = pThis;
		m_pStdFunc = NULL;
		MakeCode();
	}
	~ACCallback<_TPStdFunc, _TClass, _TPMemberFunc>()
	{
		::VirtualFree(m_pStdFunc, 0, MEM_RELEASE);
	}
	void Assign(_TClass* pThis,
		_TPMemberFunc pFunc
		)
	{
		m_pFunc = pFunc;
		m_pThis = pThis;
		m_pStdFunc = NULL;
		MakeCode();
	}
	operator _TPStdFunc()
	{
		return m_pStdFunc;
	}
};
/********************************** EXAMPLE **********************************
class CClass1
{
public:
TCHAR m_Buf[255];
BOOL EnumWindowProc(HWND hwnd, LPARAM lp)
{
GetWindowText(hwnd, m_Buf, 255);
printf("Enum window=%s\n", m_Buf);
return TRUE;
}
typedef BOOL (CClass1::*CLASSWNDENUMPROC)(HWND, LPARAM);
};
TO USE:
CClass1 c1;
ACCallback<WNDENUMPROC, CClass1, CClass1::CLASSWNDENUMPROC> cb(&c1, &CClass1::EnumWindowProc);
EnumWindows(cb, 0);
************************* END OF EXAMPLE *********************************/
