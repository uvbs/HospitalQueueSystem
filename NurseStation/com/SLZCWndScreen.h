#pragma once
#include "HardWareDef.h"
#include <afxmt.h>
#include <list>
using namespace std;
class SLZCWndScreen
{
private:
	SLZCWndScreen(void);
//	static SLZCWndScreen* m_pInstance;
public:
	~SLZCWndScreen(void);
	static SLZCWndScreen* GetInstance(){
		static SLZCWndScreen instance;
		return &instance;
	}
private:
	typedef struct  
	{
		unsigned char address;//通屏地址
		unsigned char channel;//通道号
		unsigned short width;//通道宽
		unsigned short height;//通道高
		unsigned char fone;//字体大小
	}ThrScreenBasicMsg;
	typedef struct  
	{
		CString msg;
		int address;
		int channel;
		CString IP;
		USHORT port;
	}SendThrScreenMsg;
	int DoScreenMsg(CString& msg,int address,
		char* buf);
	void AssicToHex(char *param);
	void StrToHexes(char *pc,int nsize,char *puc);
	int InitScreen(char *buf,const int address);
	BOOL IsResaved(const char* buf);//判断发送给屏的数据是否接收到
	CList<WNDSCREENMSG,WNDSCREENMSG&> m_listScreenMsg;//缓冲区
	HANDLE m_hDoWndScreenMsgThread;//处理屏数据线程
	static DWORD WINAPI DoWndScreenMsgThread(LPVOID pParm);//处理屏数据线程
	HANDLE m_hDoThrWndMsgThread;//处理通屏数据线程句柄
	static DWORD WINAPI DoThrWndMsgThread(LPVOID pParam);//
	CMutex m_WndScreenMutex;//跳屏锁
	CMutex m_ThrWndMutex;//通屏锁
	void StartHardScreen();//开启处理屏数据线程
	std::list<ThrScreenBasicMsg> m_list_thrBasicMsg;//通屏通道基本信息
	std::string m_recvThroughInitStr;//接受初始化通屏数据
	std::list<string> m_list_recvString;//分解接受的通屏数据
	std::list<SendThrScreenMsg> m_list_sendThrMsg;//通屏数据缓冲区
	CString FlushCstringToFitWndScreen(const CString& str,const int length,int height);//用于通屏时格式化字符串
	BOOL SendDataToThroughScreen(const CString& str,int address,int channel,const CString& IP=_T(""),USHORT port=0);
	int  FindChannelWidth(int address,int channel,int& height);//算出该通道能显示多少个字
	void AddThrBasicMsg(ThrScreenBasicMsg msg);
public:
	void AddScreenMsg(const CString& msg,const int address);//向缓冲区里添加数据
	void InitThroughScreen(const int address);//初始化通屏串口模式
	void InitThroughScreen(const CString& IP,USHORT port,int address);//初始化通屏TCP模式
	BOOL DoThroughInitMsg();
	void AddThroughInitStr(const char* buf,const DWORD count);
	void AddThroughScreenMsg(const CString& msg,int address,int channel,const CString& IP=_T(""),USHORT port=0);//向缓冲区里添加通屏数据
};
