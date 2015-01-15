#include "stdafx.h"
#include "JttsAPI.h"
#include "NurseStation.h"
//#include "DoWndScreen.h"
#include "MyString.h"
#include "MyCommon.h"
#include "SendToHardWare.h"

CJttsAPI::CJttsAPI()
: m_bCallbackFunc(FALSE)
, m_bCallbackHwnd(FALSE)
, m_nFormat(FORMAT_WAV)
, m_nFileHeadFlag(PLAYTOFILE_DEFAULT)
, m_nInitMethod(0)
, m_szInitPath(NULL)
, m_szSerialNo(NULL)
, m_szVoiceInfo(NULL)
, m_iSoundReplayTimes(1)
,m_pPlayVoiceThread(NULL)
//, m_pevtKillThread(NULL)
{
	m_szInitPath = new char[_MAX_PATH];
	m_szSerialNo = new char[_MAX_PATH];
	m_szVoiceInfo = new char[_MAX_PATH];
}

CJttsAPI::~CJttsAPI()
{
	delete[] m_szInitPath;
	delete[] m_szSerialNo;
	delete[] m_szVoiceInfo;
	if(m_pPlayVoiceThread)
	{
		delete m_pPlayVoiceThread;
		m_pPlayVoiceThread = NULL;
	}
}

BOOL CJttsAPI::InitTTS()
{
	WriteReg();

	ERRCODE err;
	err = jTTS_Init(NULL, NULL);
	if (err != ERR_NONE && err != ERR_ALREADYINIT)
	{
		AfxMessageBox(_T("jTTS语音初始化出错"));
		//ErrorMessage(err);
		return FALSE; // 初始化失败
	}
	// 预载当前音色
	/*
	JTTS_CONFIG config;
	jTTS_Get(&config);
	CString strTmp = GetProfileString(_T("Init"), _T("szLastGuid"), _T("93A47B28-FE41-4e99-9458-7A781484A467"));
	WChar2Char(config.szVoiceID, VOICEID_LEN, strTmp.GetBuffer());
	*/
	/*
	err = jTTS_PreLoad(config.szVoiceID);
	if (err != ERR_NONE)
	{
		AfxMessageBox(_T("jTTS_PreLoad error"));
		ErrorMessage(err);
		return FALSE; // 初始化失败
	}
	*/
	// 设置当前属性
	/*
	err = jTTS_Set(&config);
	if (err != ERR_NONE)
	{
		AfxMessageBox(_T("jTTS_Set error"));
		ErrorMessage(err);
		return FALSE; // 初始化失败
	}
	*/
	// 设置显示串
	//UpdateVoiceInfoString();

	LoadConf();

	m_pPlayVoiceThread = AfxBeginThread(PlayVoiceThread, this);

	return TRUE;
}

BOOL CJttsAPI::EndTTS()
{
	
	//if(!m_lstPlayStrList.IsEmpty())
	//{
	//	POSITION pos = m_lstPlayStrList.GetHeadPosition();
	//	for(;pos;)
	//	{
	//		free((VOICEDISPLAYSTR*)m_lstPlayStrList.GetNext(pos));
	//	}
	//}

	if(m_pPlayVoiceThread)
	{
		m_bPlayVoiceThread = FALSE;
		//m_pevtKillThread = new CEvent(FALSE, TRUE);
		//DWORD dwRet = ::WaitForSingleObject(m_evtKillThread->m_hObject, 5000);
		//m_pPlayVoiceThread->ExitInstance();
		//::TerminateThread(m_pPlayVoiceThread->m_hThread, 0);
		DWORD dwRet = ::WaitForSingleObject(m_pPlayVoiceThread->m_hThread, 1000);
		//HANDLE hThread[1];
		//hThread[0] = m_pPlayVoiceThread->m_hThread;
		//DWORD dwRet = ::MsgWaitForMultipleObjects(1, hThread, FALSE, 100, QS_ALLEVENTS);
		if(dwRet == WAIT_OBJECT_0)
		{
			//AfxMessageBox(_T("Jtts线程安全退出"));
		}
		//else if(dwRet == WAIT_OBJECT_0 + 1)
		//{
		//	MSG msg;
		//	if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		//	{
		//		::DispatchMessage(&msg);
		//	}
		//}
		else
		{
			//::TerminateThread(m_pPlayVoiceThread->m_hThread, 0);
		}
		//else if(dwRet == WAIT_FAILED)
		//{
		//	::TerminateThread(m_pPlayVoiceThread->m_hThread, 0);
		//	AfxMessageBox(_T("Jtts线程已于等待前结束"));
		//}
		//else if(dwRet == WAIT_TIMEOUT)
		//{
		//	::TerminateThread(m_pPlayVoiceThread->m_hThread, 0);
		//	AfxMessageBox(_T("Jtts线程等待超时"));
		//}

		//if(m_pevtKillThread->Lock(50))
		//{
		//	AfxMessageBox(_T("语音与显示同步线程安全退出"));
		//}

		//if(!m_mtThreadPlayVoice.Lock(500))
		//{
		//	m_pPlayVoiceThread->ExitInstance();
		//	Sleep(20);
		//}
		//else
		//{
		//	//AfxMessageBox(_T("Jtts线程安全退出"));
		//}
	}
	
	return ERR_NONE == jTTS_End();
}

BOOL CJttsAPI::SetPlay(
			 UINT uDeviceID,
			 HWND hwnd,
			 JTTS_CALLBACKPROC lpfnCallback,
			 DWORD dwUserData
			 )
{
	return ERR_NONE == jTTS_SetPlay(0xFFFFFFFF, NULL, NULL, 0);
}

BOOL CJttsAPI::AddPlayText(VOICEDISPLAYSTR vdstr)
{
	if(vdstr.strVoiceStr.IsEmpty())
	{
		return FALSE;
	}
	m_mtPlayStrList.Lock();
	//if(!m_lstPlayStrList.IsEmpty())
	//{
	//	if(/*vdstr.strDoctorID.Compare(m_lstPlayStrList.GetTail().strDoctorID) == 0 && */
	//		vdstr.strVoiceStr.Compare(m_lstPlayStrList.GetTail().strVoiceStr) == 0)
	//	{
	//		m_mtPlayStrList.Unlock();
	//		return FALSE;
	//	}
	//}
	m_lstPlayStrList.AddTail(vdstr);
	m_mtPlayStrList.Unlock();
	return TRUE;
}

UINT CJttsAPI::PlayVoiceThread(LPVOID pParam)
{
	CJttsAPI* pThis = (CJttsAPI*)pParam;

	pThis->m_bPlayVoiceThread = TRUE;
	
	while(pThis->m_bPlayVoiceThread)
	//while(1)
	{
		if(pThis->m_lstPlayStrList.IsEmpty())
		{
			Sleep(300);
			continue;
		}
		pThis->m_mtPlayStrList.Lock();
		VOICEDISPLAYSTR vdstr = pThis->m_lstPlayStrList.GetHead();
		pThis->m_lstPlayStrList.RemoveHead();
		pThis->m_mtPlayStrList.Unlock();
		//液晶
		((CNurseStationApp*)AfxGetApp())->GetController()->StbUpdateCallInfo(
			vdstr.strDisplayStr, vdstr.strOfficeStbId);
		//LED
		CSendToHardWare::SendToHardWare(
			enumComprehScreen, vdstr.iCompLED, vdstr.strDisplayStr);
		CSendToHardWare::SendToHardWare(
			enumSamllScreen, vdstr.iWndScreenId, vdstr.strDisplayStr);
		((CNurseStationApp*)AfxGetApp())->GetController()->SendToThroughLED(
			vdstr.iThroughLED, vdstr.strDisplayStr);

		//声音
		int nLength = vdstr.strVoiceStr.GetLength() * 2 + 1;
		char* buf = (char*)malloc(nLength);
		memset(buf, 0, nLength);
		MyString::WChar2Char(buf, nLength, vdstr.strVoiceStr.GetBuffer());
		ERRCODE errcode;
		for(int i = 0; i < pThis->m_iSoundReplayTimes; i++)
		{
			errcode = jTTS_Play(buf, PLAYMODE_SYNC);
		}
		free(buf);
		//if(errcode == ERR_NONE)
		//{
			((CNurseStationApp*)AfxGetApp())->GetController()->ResetStbShowMsgFlag(vdstr.strOfficeStbId);
		//}
	}
	::ExitThread(0);
	return 0;
}

BOOL CJttsAPI::WriteReg(void)
{
	CString strModulePath = MyCommon::GetModuleDir();
	//CString strSerialNo = GetProfileString(_T("Config"), _T("SerialNo"), _T("4C3E77924B936234"));
	CString strJttsSerialNo(_T("4C3E77924B936234"));

	HKEY hKey;
	LONG ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		_T("Software\\SinoVoice\\jTTS4_Professional\\"), 0, KEY_WRITE, &hKey);
	if(ERROR_SUCCESS != ret)
	{
		LPCTSTR lpSubKey = _T("Software\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegCreateKey:HKEY_LOCAL_MACHINE\\Software\\"));
			return FALSE;
		}
		lpSubKey = _T("SinoVoice\\jTTS4_Professional\\");
		ret = ::RegCreateKey(hKey, lpSubKey, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegCreateKey:SinoVoice\\jTTS4_Professional\\"));
			return FALSE;
		}
		ret = ::RegOpenKeyEx(hKey, NULL, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			//AfxMessageBox(_T("Error:RegOpenKeyEx:HKEY_LOCAL_MACHINE\\Software\\SinoVoice\\jTTS4_Professional\\"));
			return FALSE;
		}
	}

	ret = ::RegSetValueEx(hKey, _T("LibPath4"), 0, REG_SZ,
		(const BYTE*)strModulePath.GetBuffer(), strModulePath.GetLength()*2+1);
	if(ERROR_SUCCESS != ret)
	{
		//AfxMessageBox(_T("Error:RegSetValue:Write LibPath4"));
		return FALSE;
	}
	ret = ::RegSetValueEx(hKey, _T("SerialNo"), 0, REG_SZ,
		(const BYTE*)strJttsSerialNo.GetBuffer(), strJttsSerialNo.GetLength()*2);
	if(ERROR_SUCCESS != ret)
	{
		//AfxMessageBox(_T("Error:RegSetValue:Write SerialNo"));
		return FALSE;
	}

	return TRUE;
}

/*
BOOL TTSAPI JTTSCallBackProc(WORD wParam, LONG lParam, DWORD dwUserData)
{
CString str;
CMainFrame * pFrame  = (CMainFrame *) AfxGetMainWnd();
CStatusBar * pBar = &pFrame->m_wndStatusBar;
switch(wParam)
{
case NOTIFY_BEGIN:
{
g_lPrevPos = 0;
str.Format("Notify - Begin - Total Size: %ld", lParam);
break;
}
case NOTIFY_END:
{
str.Format("Notify - End - Return Code: %ld", lParam);
break;
}
case NOTIFY_SENTEND:
{
g_lPrevPos = lParam;	// 因为一句的结束和下一句开始几乎同时，所以为显示清晰，先保留此值，等到发送下句开始时再统一显示
break;
}
case NOTIFY_SENTBEGIN:
{
str.Format("Notify - Reading From %ld To %ld", g_lPrevPos, lParam);
break;
}
case NOTIFY_MARK:
{
DWORD dwOffset = GETMARKOFFSET(lParam);
WORD wLen = GETMARKLEN(lParam);
str.Format("Notify - Mark Offset: %ld Len: %d", dwOffset, wLen);
break;
}
default:
str.Format("Notify - Unknow, lParam: %ld", lParam);
break;
}

SendMessage(pBar->m_hWnd, (UINT) SB_SETTEXT, 0, (LPARAM)(LPCTSTR)str);

return TRUE;
}
*/

void CJttsAPI::LoadConf()
{
	m_iSoundReplayTimes = MyCommon::GetProfileInt(
		_T("sys"),_T("SOUND_REPLAY_TIMES"), 1, _T("\\SYS\\sound.ini"));

	m_nSpeed = MyCommon::GetProfileInt(
		_T("sys"),_T("SOUND_SPEED"), 5, _T("\\SYS\\sound.ini"));

	m_nVolume = MyCommon::GetProfileInt(
		_T("sys"),_T("SOUND_VOLUME"), 5, _T("\\SYS\\sound.ini"));

	jTTS_SetParam(PARAM_SPEED, m_nSpeed);
	jTTS_SetParam(PARAM_VOLUME, m_nVolume);
}

