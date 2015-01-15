#pragma once
#include "ACCallback.h"

template <class _TClass>
class MyTimer
{
public:
	typedef void (_TClass::*MYTIMERPROC)(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	MyTimer(void)
		: m_pThis(NULL)
		, m_idEvent(0)
		, m_uElapse(0)
		, m_pTimerProc(NULL)
	{

	}

	MyTimer(_TClass* pThis, MYTIMERPROC pClassTimerProc)
		: m_pThis(pThis)
		, m_pTimerProc(pClassTimerProc)
		, m_idEvent(0)
		, m_uElapse(0)
	{
		//m_callback.Assign(pThis, pClassTimerProc);
		//m_bAsigned = TRUE;
	}

	MyTimer(_TClass* pThis, MYTIMERPROC pClassTimerProc, UINT uElapse)
		: m_pThis(pThis)
		, m_pTimerProc(pClassTimerProc)
		, m_uElapse(uElapse)
		, m_idEvent(0)
	{

	}

	~MyTimer(void)
	{
		KillTimer();
	}

	void SetTemplateInstance(_TClass* pThis)
	{
		m_pThis = pThis;
	}

	void SetElapse(UINT elapse)
	{
		m_uElapse = elapse;
	}

	void SetTimerProc(MYTIMERPROC pTimerProc)
	{
		m_pTimerProc = pTimerProc;
	}

	void SetTimer(_TClass* pThis, UINT uElapse, MYTIMERPROC pTimerProc)
	{
		m_pThis = pThis;
		m_uElapse = uElapse;
		m_pTimerProc = pTimerProc;
	}

	BOOL IsActive()
	{
		return m_idEvent != 0;
	}
	
public:

	BOOL StartTimer(_TClass* pThis, UINT uElapse, MYTIMERPROC pTimerProc)
	{
		m_pThis = pThis;
		m_uElapse = uElapse;
		m_pTimerProc = pTimerProc;
		return SetTimer();
	}

	BOOL StartTimer(UINT uElapse)
	{
		m_uElapse = uElapse;
		return SetTimer();
	}

	BOOL StartTimer()
	{
		return SetTimer();
	}

	BOOL ReStartTimer()
	{
		return SetTimer();
	}

	BOOL KillTimer()
	{
		BOOL bRet = TRUE;
		if(m_idEvent)
		{
			bRet = ::KillTimer(NULL, m_idEvent);
			m_idEvent = 0;
		}
		
		return bRet;
	}

private:

	BOOL SetTimer()
	{
		if(!KillTimer())
		{
			return FALSE;
		}
		if(!m_pThis || !m_pTimerProc || !m_uElapse)
		{
			return FALSE;
		}
		m_callback.Assign(m_pThis, m_pTimerProc);
		m_idEvent = ::SetTimer(NULL, 0, m_uElapse, m_callback);
		return m_idEvent != 0;
	}

private:

	ACCallback<TIMERPROC, _TClass, MYTIMERPROC> m_callback;

	_TClass* m_pThis;
	UINT_PTR m_idEvent;
	UINT m_uElapse;
	MYTIMERPROC m_pTimerProc;
};
