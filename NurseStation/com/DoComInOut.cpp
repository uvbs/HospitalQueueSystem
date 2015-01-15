#include "StdAfx.h"
#include "DoComInOut.h"
#include "ComInit.h"
#include "../CommonConvert.h"
#include "../NurseStation.h"

//extern void MyWriteConsole(CString str); 

CDoComInOut::CDoComInOut(void) :
m_hReadCallerThread(NULL)
, m_isDoneThroughInit(FALSE)
, m_hWriteComThread(NULL)
{
	m_pSlzCaller = SLZCCaller::GetInstance();
//	m_pSlzEvaluator = SLZCEvaluator::GetInstance();
	m_pSlzWndScreen = SLZCWndScreen::GetInstance();
	Start();
}

CDoComInOut::~CDoComInOut(void)
{
	if(m_hReadCallerThread)
	{
		TerminateThread(m_hReadCallerThread,0);
		CloseHandle(m_hReadCallerThread);
		m_hReadCallerThread = NULL;
	}
	if(m_hWriteComThread)
	{
		TerminateThread(m_hWriteComThread,0);
		CloseHandle(m_hWriteComThread);
		m_hWriteComThread = NULL;
	}
}

DWORD WINAPI CDoComInOut::ReadCallerThread(LPVOID pParam)
{
	while(TRUE)
	{
		CDoComInOut* pThis=(CDoComInOut*)pParam;
		char buf[1024]={0};
		DWORD dwReaded=0;//读到的大小
		BOOL bres=FALSE;
		DWORD dwErrorFlags;
		COMSTAT ComStat;
		OVERLAPPED m_osRead;
		memset(&m_osRead,0,sizeof(OVERLAPPED));
		CComInit* pComInit = CComInit::GetInstance();
		if(pComInit->m_hComWndScreen
			!=INVALID_HANDLE_VALUE)
		{	
			//////考虑枷锁
			ClearCommError(pComInit->m_hComWndScreen,&dwErrorFlags,&ComStat);
//			if(!ComStat.cbInQue) continue;
//			dwReaded=min(dwReaded,(DWORD)ComStat.cbInQue);
//			if(!dwReaded) continue;
			Sleep(100);
			bres=ReadFile(pComInit->m_hComWndScreen,buf,1024,&dwReaded,&m_osRead);
			if(!bres)
			{
				if(GetLastError()==ERROR_IO_PENDING)
				{
					GetOverlappedResult(pComInit->m_hComWndScreen, &m_osRead,&dwReaded,TRUE);
				}
			}
			if(dwReaded>0)//读到了
			{
#ifdef _DEBUG
				unsigned char uBuf[1024]={0};
				memcpy(uBuf,buf,dwReaded);
				CString temp;
				for(UINT i=0;i<dwReaded;i++)
				{
					temp+=_T("0x");
					temp.AppendFormat(_T("%08x"),uBuf[i]);
					temp+=_T(" ");
				}
				MyWriteConsole(temp);
				MyWriteConsole(_T("------------------------------------------"));
				CString strBuf;
				CCommonConvert convert;
				convert.CharToCstring(strBuf,buf);
				MyWriteConsole(strBuf);
				MyWriteConsole(_T("------------------------------------------"));
				CString strDwread;
				MyWriteConsole(strDwread);
#endif
				//////不是呼叫器和评价的数据,注：呼叫器评价器数据以0xff,0x68开始,0x16结束
				if(buf[0]==(char)0xA0 && buf[1]==(char)0x90 && buf[dwReaded-1]==(char)0xAA && 
					buf[dwReaded-2]==(char)0xA5)
				{
					int wndID = buf[2];//屏地址
					//条屏，综合屏数据，发送成功
				}
				else if((buf[0]!=(char)0xff&&buf[2]!=(char)0x68) && buf[dwReaded-1]!=(char)0x16)
				{
					//通屏数据
					if(!pThis->m_isDoneThroughInit)
					{
#ifdef _DEBUG
						MyWriteConsole(_T("同频数据"));
#endif
						SLZCWndScreen* pWindowScreen = SLZCWndScreen::GetInstance();
						pWindowScreen->AddThroughInitStr(buf,dwReaded);
					}
				}
				else
				{
				//////////呼叫器消息
				//呼叫器消息
					WriteComMsg* pMsg = new WriteComMsg;
//					pMsg->buf = new char[dwReaded+1];
					memset(pMsg->buf,0,textNum);
					memcpy(pMsg->buf,buf,dwReaded);
					pMsg->length = dwReaded;
					pThis->AddWriteComMsg(pMsg);
					WaitForSingleObject(pThis->m_hWriteComThread,40);
					pThis->m_pSlzCaller->DoReadMsg(dwReaded,buf);
				}
			}
			////////////
		}
		else
		{
			Sleep(20);
		}
	}
	return 0;
}

BOOL CDoComInOut::Start()
{
	BOOL flag = FALSE;
	if(m_hReadCallerThread==NULL)
	{
		m_hReadCallerThread = CreateThread(NULL,0,ReadCallerThread,this,0,NULL);
		if(m_hReadCallerThread)
		{
			flag = TRUE;
		}
	}
	if(m_hWriteComThread==NULL)
	{
		m_hWriteComThread = CreateThread(NULL,0,WriteComThread,this,0,NULL);
		if(m_hWriteComThread)
		{
			flag = TRUE;
		}
	}
	return flag;
}

DWORD WINAPI CDoComInOut::WriteComThread(LPVOID pParam)
{
	while(TRUE)
	{
		CDoComInOut* pThis = (CDoComInOut*)pParam;
		if(pThis->m_list_writeComMsg.size()==0)
		{
			Sleep(10);
		}
		else
		{
			pThis->m_readComLock.Lock();
			list<WriteComMsg*>::const_iterator itera = pThis->m_list_writeComMsg.begin();
			WriteComMsg* pMsg = *itera;
//			pThis->m_list_writeComMsg.erase(itera);
			pThis->m_list_writeComMsg.pop_front();
			pThis->m_readComLock.Unlock();
			CComInit* pComInit = CComInit::GetInstance();
			DWORD dwWrited = 0;
#ifdef _DEBUG
			CString sendMsg=_T("second msg:");
			for(int i=0;i<pMsg->length;i++)
			{
				sendMsg.AppendFormat(_T("%x "),*(pMsg->buf+i));
			}
			sendMsg.AppendFormat(_T("count:%d"),pMsg->length);
			MyWriteConsole(sendMsg);
#endif
//			DWORD dwErrorFlags;
//			COMSTAT ComStat;
			OVERLAPPED m_osWrite;
			memset(&m_osWrite,0,sizeof(m_osWrite));
			BOOL bWriteStat = FALSE;
//			WriteFile(hCom,buffer,dwBytesWritten, &dwBytesWritten,&m_OsWrite); 
			bWriteStat = WriteFile(pComInit->m_hComWndScreen,pMsg->buf,
				pMsg->length,&dwWrited,&m_osWrite);
			if(!bWriteStat)
			{
				if(GetLastError()==ERROR_IO_PENDING)
				{ 
					WaitForSingleObject(m_osWrite.hEvent,1000);
				}
			}
#ifdef _DEBUG
			 CString strCount;
			 strCount.Format(_T("send count:%d"),dwWrited);
			 MyWriteConsole(strCount);
#endif
//			delete [] pMsg->buf;
			delete pMsg;
		}
	}
	return 0;
}

void CDoComInOut::AddWriteComMsg(WriteComMsg* pMsg)
{
	m_readComLock.Lock();
	m_list_writeComMsg.push_back(pMsg);
	m_readComLock.Unlock();
}