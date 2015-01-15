#pragma once

#include "SLZCallerData.h"
#include "afxmt.h"


// SLZCCaller 命令目标
/*
硬件呼叫器单例类
*/
class SLZCCaller : public CObject
{
	friend class CDoComInOut;
private:
	SLZCCaller();
public:
	//添加写data
	BOOL AddWriteCallerData(SLZCallerData callerData);
	////////////////////获取读到的数据接口
	BOOL HasData();
	SLZCallerData GetData();
	~SLZCCaller();
	////////////////////////////
	static SLZCCaller*  GetInstance();
private:
	////////////////外部获取读到的数据接口的缓冲区
	///解析以后的消息
	CList<SLZCallerData> m_callDataList;
	CMutex m_callerLock;
	///////////////
private:
	HANDLE m_hDoCallerMsgThread;//缓冲处理硬件呼叫器线程
	static DWORD WINAPI DoCallerMsgThread(LPVOID pParam);
	//缓冲处理呼叫器线程
	/////////////////解析的消息用于外部接口的锁
	CMutex m_callerReadMutex;
	//呼叫器接收到的消息缓冲
	CList<CString,CString&> m_listCallerMsg;//缓冲区
	//处理呼叫器发送的数据（解析消息)
	void DoCallerMsg(char *buf,int size);
	//警报
	int AlarmToSystem(const int CallerAddress,char* addbuf);
	////显示成功与否
	int ShowSuccFail(const int address,char* buf,const int conntext);
	//显示剩余人数
	int ShowLastPaintCount(const int CallerAddress,int WaitCount,char *addbuf);
	//分离出具体号码
	int SepaMsgNum(const char* buf,char* result,int size);
	//呼叫器接收到的消息
	CList<CString,LPCWSTR> m_ReciveMsg;
	//上次没读完的消息
	CString m_lastMsg;
	//处理上次没读完信息
	void ExTransmission();
	//////////////////写数字到呼叫器
	int ShowSystemNum(int CallerAddress,char* buf,
		const int num[],const int count);
	/////////////////显示当前呼叫号码和剩余人数
	int ShowSysCallLeftNum(int callerAddress,char* buf,int leftNum,const CString& callNum);
private:
	HANDLE m_hWriteCallerThread;//写硬件呼叫器线程
	static DWORD WINAPI WriteCallerThread(LPVOID pParam);//写硬件呼叫器线程
	//写硬件呼叫器缓冲
	CList<SLZCallerData> m_writeCallerData;
	//写呼叫器信息锁
	CMutex m_writeCallerLock;
private:
	//初始化呼叫器类
	void InitHardWareCaller();
	///处理读取消息
	void DoReadMsg(const DWORD dwReaded,char* buf);
};


