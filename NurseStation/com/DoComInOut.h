#pragma once
#include "SLZCCaller.h"
#include "SLZCWndScreen.h"
#include "afxmt.h"
#include <list>
#include "DataDef.h"
using namespace std;
/*
处理串口读取类
*/
class CDoComInOut
{
private:
	CDoComInOut(void);
//	CDoComInOut(const CDoComInOut& obj);
//	CDoComInOut& operator=(const CDoComInOut& obj);
public:
	//提供获取唯一实例接口
	static CDoComInOut* GetInstance()
	{
		static CDoComInOut instance;
		return &instance;
	}
	~CDoComInOut(void);
	HANDLE m_hReadCallerThread;//读硬件呼叫器线程
	HANDLE m_hWriteComThread;//写串口线程
	void SetThroughInitDone(BOOL isDone) 
	{
		m_isDoneThroughInit = isDone;
	}
private:
	static DWORD WINAPI ReadCallerThread(LPVOID pParam);//读硬件呼叫器线程
	static DWORD WINAPI WriteComThread(LPVOID pParam);//写串口线程
private:
	SLZCCaller* m_pSlzCaller;//呼叫器唯一变量
//	SLZCEvaluator* m_pSlzEvaluator;//评价起唯一变量
	SLZCWndScreen* m_pSlzWndScreen;//屏唯一变量
private:
	CMutex m_readComLock;
	BOOL Start();
	BOOL m_isDoneThroughInit;//通屏是否初始话
private:
	list<WriteComMsg*> m_list_writeComMsg;
public:
	void AddWriteComMsg(WriteComMsg* pMsg);
};
