#pragma once

class CComInit
{
private:
	CComInit(void);
public:
	~CComInit(void);
	//打开窗口屏、呼叫器串口
	int OpenWndScreen(const int ncom);
	int m_canUse[10];//保存本机上所有的可用串口号
	HANDLE m_hComWndScreen;//条屏串口(呼叫器)
	HANDLE m_hComReadCard;//刷卡器串口
	HANDLE OpenComm(const int ncom);//打开串口
	///try本机所有的可用的串口
	int TryCom(const int nCom);
	//打开刷卡器串口
	int OpenCardComm(const int ncom);
	//保存到文件
	void SaveComm();
	//读文件
	CString GetCardComm();
	CString GetWndComm();
	int OpenWndScreen();
	int OpenCardComm();
private:
	//初始化串口
	void InitCom();
	CString m_cCardComm;
	CString m_cWndComm;
	CString m_strPath;
public:
	//获取唯一实例
	static CComInit* GetInstance(){
		static CComInit instance;
		return &instance;
	}
};
