#pragma once

#include <afxwin.h>
#include <afxmt.h>
#include "jTTS_ML.h"
#pragma comment(lib, "jTTS_ML.lib")
//#include "NurseDef.h"

typedef struct _tagVoiceAndDisplayStr
{
	CString		strVoiceStr;
	CString		strDisplayStr;
	int				iWndScreenId;
	int				iCompLED;
	int				iThroughLED;
	CString		strOfficeStbId;
	CString		strConsultStbId;
	CString		strCallPatientID;
	CString		strCallPatientName;
	CString		strWaitPatientID;
	CString		strWaitPatientName;
	//BOOL		bScreen;
} VOICEDISPLAYSTR, *LPVOICEDISPLAYSTR;

class CJttsAPI : public CObject
{
public:
	CJttsAPI();
	~CJttsAPI();

	void SetSoundReplayTimes(int iTimes){m_iSoundReplayTimes = iTimes;}
	void LoadConf();

	BOOL InitTTS();
	BOOL EndTTS();
	BOOL SetPlay(
		UINT uDeviceID, //如果为WAVE_MAPPER(-1)，表示缺省的放音设备，否则为指定的放音设备。 
		HWND hwnd, //一个合法的窗口句柄。当系统阅读时和完毕后，会发送一些自定义消息到此窗口。如果为NULL，则不使用窗口消息的方式通知应用程序。
		JTTS_CALLBACKPROC lpfnCallback, //一个合法的回调函数地址。当系统阅读时和完毕后，会调用此函数。
		DWORD dwUserData //用户传入的自定义数据，回调时将回传给回调函数
		);

	BOOL AddPlayText(VOICEDISPLAYSTR vdstr);
	void ErrorMessage(ERRCODE err);
private:
	CString GetModuleDir(void);
	BOOL WriteReg(void);
	//static BOOL WChar2Char(char* dstBuf, size_t len, wchar_t* srcBuf);
	static UINT PlayVoiceThread(LPVOID pParam);
private:

	//CList<char*, char*&> m_lstPlayStrList;
	CList<VOICEDISPLAYSTR> m_lstPlayStrList;
	CWinThread* m_pPlayVoiceThread;
	BOOL m_bPlayVoiceThread;
	CMutex m_mtPlayStrList;
	int m_iSoundReplayTimes;
	//CMutex m_mtThreadPlayVoice;


	//CEvent* m_pevtKillThread;

protected:
	JTTS_CONFIG m_config;
	BOOL m_bCallbackFunc,
		 m_bCallbackHwnd;
	int  m_nFormat;
	int	 m_nFileHeadFlag;
	int  m_nInitMethod;
	char* m_szInitPath;
	char* m_szSerialNo;
	char* m_szVoiceInfo;

	int m_nSpeed;	//0~9 语速
	int m_nVolume;//0~9 音量
};