#include "StdAfx.h"
#include "SLZCWndScreen.h"
#include "ComInit.h"
#include "../CommonConvert.h"
#include "ComplSocketClient.h"
#include "../ComputeFuncationTime.h"
#include "DoComInOut.h"
#include "../MySocketUDP.h"
#include "../MyCommon.h"
#include "../MyString.h"
#include "../NurseStation.h"


//SLZCWndScreen* SLZCWndScreen::m_pInstance=NULL;//new SLZCWndScreen;
//extern void MyWriteConsole(CString str);

SLZCWndScreen::SLZCWndScreen(void) : 
m_hDoWndScreenMsgThread(NULL)
, m_ThrWndMutex(NULL)
{
	StartHardScreen();
	////////////////////////窗口屏
	///初始化通屏通道，获取通道的信息最大255块通屏
	
//   	for(int i=0;i<255;i++)
//   	{
//  		InitThroughScreen(1);
//  	}
//	InitThroughScreen(_T("192.168.1.8"),1024);
	////////////////////////////////////////////
}

SLZCWndScreen::~SLZCWndScreen(void)
{
	TerminateThread(m_hDoWndScreenMsgThread,0);
	if(m_hDoWndScreenMsgThread)
	{
		CloseHandle(m_hDoWndScreenMsgThread);
		m_hDoWndScreenMsgThread = NULL;
	}
	TerminateThread(m_hDoThrWndMsgThread,0);
	if(m_hDoThrWndMsgThread)
	{
		CloseHandle(m_hDoThrWndMsgThread);
		m_hDoThrWndMsgThread = NULL;
	}
}

int SLZCWndScreen::DoScreenMsg(CString& msg,
								 int address, char* buf)
{
	char h,l;//高位，低位
	char color=0;//颜色默认为0红色
	int count=msg.GetLength();
	if(count==0)
	{
		return 0;
	}

	buf[0]=(char)WNDSCREEN_HEAD_FIRST;
	buf[1]=(char)WNDSCREEN_HEAD_SCEND;
	/*屏地址*/
	buf[2]=address;
	buf[3]=(char)WNDSCREEN_SHOW_INFOR;

	int m=6;
	unsigned char mydatacount=0;
	for(int i=0;i<count;i++)
	{
		BOOL flag=FALSE;
		WCHAR w=msg.GetAt(i);
		if(w<=255)
		{
			char ch=w;
			if(ch=='#')
			{
				char mark[4]={0};
				mark[0]=ch;
				int temp=i;
				WCHAR secd=msg.GetAt(temp+1);
				if(secd<=255 && secd>='0' && secd<='2')
				{
					mark[1]=secd;
					WCHAR three=msg.GetAt(temp+2);
					if(three<=255 && three=='#')
					{
						flag=TRUE;
						mark[2]=three;

						mark[3]='\0';
						if(mark[1]=='0')
						{
							color=0;
						}
						else if(mark[1]=='1')
						{
							color=1;
						}
						else if(mark[1]=='2')
						{
							color=2;
						}
						i+=2;
					}
				}
			}
			if(!flag)
			{
				buf[m]=w;
				m++;
				buf[m]=color;//颜色
				m++;
				mydatacount+=2;//数据大小
			}
		}
		else
		{	
			//第一个字节
			char c_w[2]={0};
			WideCharToMultiByte(CP_ACP,0,&w,-1,c_w,2,NULL,NULL);
			h=(c_w[0]&0xf0)>>4;if(h>9) h+=0x57;else h+=0x30;
			l=(c_w[0]&0x0f);if(l>9)  l+=0x57;else l+=0x30;

			char arr[2];
			arr[0]=h;
			arr[1]=l;

			StrToHexes(arr,2,&buf[m]);//数据
			m++;

			//第二个字节
			h=(c_w[1]&0xf0)>>4;if(h>9) h+=0x57;else h+=0x30;
			l=(c_w[1]&0x0f);if(l>9)  l+=0x57;else l+=0x30;

			arr[0]=h;
			arr[1]=l;

			StrToHexes(arr,2,&buf[m]);//数据
			m++;
			buf[m]=color;//颜色
			m++;
			mydatacount+=3;
		}
	}
	buf[4]=mydatacount/255;//数据大小高位
	buf[5]=mydatacount%255;//数据大小低位
	for(int i=2;i<m;i++)
	{
		buf[m]^=buf[i];
	}
	buf[m+1]='\0';

	return m+1;
}

void SLZCWndScreen::AssicToHex(char *param)
{
	if(*param >= '0' && *param <= '9')
		*param -= '0';
	else if(*param >= 'a' && *param <= 'f')
		*param -= ('a'-10);
	else if(*param >= 'A' && *param <= 'F')
		*param -= ('A'-10);
}

void SLZCWndScreen::StrToHexes(char *pc,int nsize,char *puc)
{
	int i=0;
	char uctemp=0;
	for(;i<nsize;i+=2)
	{
		*puc=*pc++;
		AssicToHex(puc);
		*puc <<= 4;//高位
		uctemp=*pc++;//低位
		AssicToHex(&uctemp);
		*puc+=uctemp;
		puc++;
	}
}

int SLZCWndScreen::InitScreen(char *buf,const int address)
{
	buf[0]=(char)WNDSCREEN_HEAD_FIRST;
	buf[1]=(char)WNDSCREEN_HEAD_SCEND;
	buf[2]=address;
	buf[3]=(char)WNDSCREEN_SHOW_ROM;
	buf[4]=0x00;
	buf[5]=0x00;
	buf[6]=buf[2]^buf[3]^buf[4]^buf[5];
	buf[7]='\0';
	return 8;
}

BOOL SLZCWndScreen::IsResaved(const char* buf)
{
	int size=strlen(buf);
	if(buf[size]==0xAA)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

DWORD WINAPI SLZCWndScreen::DoWndScreenMsgThread(LPVOID pParm)
{
	while(1)
	{
		SLZCWndScreen* pDlg=(SLZCWndScreen*)pParm;
		if(pDlg->m_listScreenMsg.GetCount()==0)
		{
			Sleep(100);
		}
		else
		{
			BOOL res = FALSE;
//			DWORD dwWrite;
			POSITION pos=pDlg->m_listScreenMsg.GetHeadPosition();
			while(pos!=NULL)
			{
				pDlg->m_WndScreenMutex.Lock();
				WNDSCREENMSG wndmsg=pDlg->m_listScreenMsg.GetNext(pos);
				pDlg->m_listScreenMsg.RemoveHead();
				pDlg->m_WndScreenMutex.Unlock();
				char buf[1024]={0};
#ifdef _DEBUG
				CString str;
				str.Format(_T("add:%d"),wndmsg.address);
				str.AppendFormat(_T("msg:%s"),wndmsg.strmsg);
				MyWriteConsole(str);
#endif
				int size = pDlg->DoScreenMsg(wndmsg.strmsg,wndmsg.address,
					buf);
// 				Sleep(10);
// 				CComInit* pComInit = CComInit::GetInstance();
// 				res=WriteFile(pComInit->m_hComWndScreen,
// 					buf,size,&dwWrite,NULL);
				WriteComMsg *pMsg = new WriteComMsg;
//				pMsg->buf = new char(size+1);
				memset(pMsg->buf,0,textNum);
				memcpy(pMsg->buf,buf,size);
				pMsg->length = size;
				CDoComInOut* pComInOut = CDoComInOut::GetInstance();
				pComInOut->AddWriteComMsg(pMsg);
			}
		}
	}
	return 0;
}

void SLZCWndScreen::AddScreenMsg(const CString& msg,const int address)
{
	if(msg!=_T(""))
	{
		WNDSCREENMSG wndmsg;
		wndmsg.address=address;
		wndmsg.strmsg=msg;
		wndmsg.color=0;
		m_WndScreenMutex.Lock();
		m_listScreenMsg.AddTail(wndmsg);
		m_WndScreenMutex.Unlock();
	}
}

void SLZCWndScreen::StartHardScreen()
{
	if(m_hDoWndScreenMsgThread == NULL)
	{
		//缓冲处理窗口屏线程
		m_hDoWndScreenMsgThread=CreateThread(NULL,0,
			DoWndScreenMsgThread,this,NULL,0);
	}
	if(m_hDoThrWndMsgThread == NULL)
	{
		m_hDoThrWndMsgThread = CreateThread(NULL,0,DoThrWndMsgThread,
			this,NULL,0);
	}
}

void SLZCWndScreen::InitThroughScreen(const int address)
{
	ThroughScreenHead head;
	memset(&head,0,sizeof(head));
	head.flagFirst = HARDWARE_THROUGH_FLAG1;
	head.flagSecond = HARDWARE_THROUGH_FLAG2;
	head.address = address;
	char cmd[15]="get qtlist.lst";

	head.length = 14;
	unsigned char headAddCmd[22]={0};
	memcpy(headAddCmd,&head,sizeof(head));
	memcpy(&headAddCmd[7],cmd,14);
	headAddCmd[21]='\0';
	DWORD dwWrite = 0;
	CComInit* pComInit = CComInit::GetInstance();
// 	Sleep(1);
// 	int res=WriteFile(pComInit->m_hComWndScreen,
// 		headAddCmd,22,&dwWrite,NULL);
	WriteComMsg *pMsg = new WriteComMsg;
//	pMsg->buf = new char(22);
	memset(pMsg->buf,0,textNum);
	memcpy(pMsg->buf,headAddCmd,22);
	pMsg->length = 22;
	CDoComInOut* pComInOut = CDoComInOut::GetInstance();
	pComInOut->AddWriteComMsg(pMsg);
}

void SLZCWndScreen::InitThroughScreen(const CString& IP,USHORT port,int address)
{
	ThroughScreenHead head;
	memset(&head,0,sizeof(head));
	head.flagFirst = HARDWARE_THROUGH_FLAG1;
	head.flagSecond = HARDWARE_THROUGH_FLAG2;
	head.address = address;
	char cmd[15]="get qtlist.lst";

	head.length = 14;
	unsigned char headAddCmd[22]={0};
	memcpy(headAddCmd,&head,sizeof(head));
	memcpy(&headAddCmd[7],cmd,14);
	headAddCmd[21]='\0';

	std::string recvBuf;
 	CComplSocketClient client;
 	int actRecvSize = 0;
	char* pMsg = new char[22];
	memset(pMsg,0,22);
	memcpy(pMsg,headAddCmd,22);
 	if(client.SendData(port,IP,pMsg,22,recvBuf,actRecvSize))
 	{
 		if(actRecvSize>0)
 			AddThroughInitStr(recvBuf.c_str(),actRecvSize);
 	}
	delete [] pMsg;
}

CString SLZCWndScreen::FlushCstringToFitWndScreen(const CString& str,const int length,int height)
{
	if(height==1)return str;
	int num=0;
	CString temp=_T("");
	int count=str.GetLength();
	for(int i=0;i<count;i++)
	{
		BOOL flag=FALSE;
		WCHAR w=str.GetAt(i);
		WCHAR secd;WCHAR three;
		if(w<=255)
		{
			if(w=='#')
			{
				int i_temp=i;
				secd=str.GetAt(i_temp+1);
				if(secd<=255 && secd>='0' && secd<='2')
				{
					three=str.GetAt(i_temp+2);
					if(three<=255 && three=='#')
					{
						flag=TRUE;
						i+=2;
					}
				}
			}
			else
			{
				num++;
			}
		}
		else if(w>255)
		{
			num+=2;
		}
		
		if(num<length)
		{
			temp+=w;
			if(flag)
			{
				temp+=secd;
				temp+=three;
			}
		}
		else if(num>length)
		{
			temp+=_T(" ");
			temp+=w;
			if(w>255)
			{
				num=2;
			}
			else
			{
				num=1;
			}
		}
		else if(num==length)
		{
			temp+=w;
			num = 0;
		}
	}

	if(num<length && num!=0)
	{
		for(int j=0;j<length-num;j++)
		{
			temp+=_T(" ");
		}
	}
	return temp;
}

void SLZCWndScreen::AddThroughInitStr(const char* buf,const DWORD count)
{
	m_recvThroughInitStr.append(buf,count);
}

BOOL SLZCWndScreen::DoThroughInitMsg()
{
	CDoComInOut* pComInOut = CDoComInOut::GetInstance();
	pComInOut->SetThroughInitDone(TRUE);
	if(m_recvThroughInitStr.empty())return FALSE;
	string::size_type position,oldPosition;
	position = m_recvThroughInitStr.find("Get_Data");
	while(position!=m_recvThroughInitStr.npos)
	{
		oldPosition = position;
		position = m_recvThroughInitStr.find("Get_Data",position+1);
		string temp = m_recvThroughInitStr.substr(oldPosition,position-oldPosition-1);
		m_list_recvString.push_back(temp);
	}
	string recvMsg;
	if(!m_list_recvString.empty())
	{
		std::list<string>::const_iterator itera = m_list_recvString.begin();
		for(itera;itera!=m_list_recvString.end();itera++)
		{
			recvMsg= *itera;
			if(!recvMsg.empty())
			{
				string::size_type first_pos = recvMsg.find_first_of('#');
				if(first_pos!=recvMsg.npos)//找到了
				{
					/////////////////////////////通屏基本信息如:屏地址
					ThrScreenBasicMsg ThrScreenMsg;
					memset(&ThrScreenMsg,0,sizeof(ThrScreenMsg));
					string throughMsg = recvMsg.substr(first_pos+1,32);
					ThrScreenMsg.address = throughMsg[29];
					///////////////////////////////////////////
					string temp = recvMsg.substr(first_pos+33);
					for(int i=0;i<(int)temp.size();i+=34)
					{
						/////////////////通道信息
						string channel = temp.substr(i,34);
						ThrScreenMsg.channel = channel[0];//通道号
						ThrScreenMsg.fone = channel[8];//字体
						memcpy(&ThrScreenMsg.width,&channel[4],2);
						memcpy(&ThrScreenMsg.height,&channel[6],2);
						/////////////////////
						AddThrBasicMsg(ThrScreenMsg);
					}
					////////////
				}
			}
//			m_list_recvString.pop_front();
		}
		m_list_recvString.clear();//删除
	}
	m_recvThroughInitStr.clear();
	return TRUE;
}

void SLZCWndScreen::AddThrBasicMsg(ThrScreenBasicMsg msg)
{
	BOOL flag = FALSE;
	std::list<ThrScreenBasicMsg>::iterator itera = m_list_thrBasicMsg.begin();
	for(itera;itera!=m_list_thrBasicMsg.end();itera++)
	{
		if(msg.address == itera->address && msg.channel==itera->channel)
		{
			flag = TRUE;
			itera->fone = msg.fone;
			itera->height = msg.height;
			itera->width = msg.width;
			break;
		}
	}
	if(!flag)
		m_list_thrBasicMsg.push_back(msg);
}

BOOL SLZCWndScreen::SendDataToThroughScreen(const CString& str,int address,int channel,const CString& IP,USHORT port)
{
	BOOL bRet = TRUE;
//	CString str =_T("123");
	/*
	ThroughScreenHead head;memset(&head,0,sizeof(head));
	head.address=1;head.flagFirst=HARDWARE_THROUGH_FLAG1;
	head.flagSecond = HARDWARE_THROUGH_FLAG2;
	head.type = 0;//命令
	head.length = 11;//第一包长度
	
	
	char firstPacket[19]={0};
	memcpy(firstPacket,&head,sizeof(head));
	const char cmd[8] = "qtdata ";
	memcpy(&firstPacket[7],cmd,7);
	firstPacket[14] = 0;
	firstPacket[15] = ' ';
	short int length = 29;
	memcpy(&firstPacket[16],&length,sizeof(length));
	DWORD dwWrite = 0;
	CComInit* pComInit = CComInit::GetInstance();
	Sleep(3);
	int res=WriteFile(pComInit->m_hComWndScreen,
		firstPacket,19,&dwWrite,NULL);//发送第一包协议头 + qtdata┗┛实时数据窗口号┗┛数据文件总长度
	char secondPacket[512]={0};
	head.type=0x10;
	head.length = 3;
	memcpy(secondPacket,&head,sizeof(head));
	secondPacket[7]='1';
	secondPacket[8]='2';
	secondPacket[9]='3';
	secondPacket[10]='\0';
	res = WriteFile(pComInit->m_hComWndScreen,
		secondPacket,11,&dwWrite,NULL);
		*/
	/*
	//格式化字符串
	int width = FindChannelWidth(address,channel);
	if(width==-1)return FALSE;
	CString FitStr = FlushCstringToFitWndScreen(str,width);
#ifdef _DEBUG
	MyWriteConsole(FitStr);
#endif
	///////////////
	CCommonConvert convert;
	int length = convert.CStringToChar(FitStr,NULL);
	short int dataLength = (short int)length;//强转
	///数据文件总长度
	memcpy(&firstPacket[16],&dataLength,sizeof(dataLength));
	DWORD dwWrite = 0;
	CComInit* pComInit = CComInit::GetInstance();
	Sleep(3);
	int res=WriteFile(pComInit->m_hComWndScreen,
		firstPacket,19,&dwWrite,NULL);//发送第一包协议头 + qtdata┗┛实时数据窗口号┗┛数据文件总长度
	///////////////////////////////////////////////
	char* buf = new char[length+1];//数据
	memset(buf,0,sizeof(buf));
	length = convert.CStringToChar(FitStr,buf);
	if(length<512)
	{
		head.type = 0x10;//数据类
		head.length = length;
		char seconedPacket[512]={0};
		memcpy(seconedPacket,&head,sizeof(head));
		memcpy(&seconedPacket[7],buf,length+1);
		Sleep(3);
		int res=WriteFile(pComInit->m_hComWndScreen,
			seconedPacket,8+length,&dwWrite,NULL);
		///发送第二包协议头 + 512字节数据 +[CRC];返回send@
	}
	else
	{
		for(int i=0;i<length/512;i++)
		{
			
		}
	}
//	head.length = length;
	///////////////////
	delete [] buf;
	*/
	///自己公司协议
//	int height = 0;
//	int width = FindChannelWidth(address,channel,height);
	CString msg = str;
// #ifdef _DEBUG
// 	CString test;
// 	test.Format(_T("width:%d"),width);
// 	MyWriteConsole(test);
// #endif
//	if(width!=-1)
//	{
//		msg = FlushCstringToFitWndScreen(msg,width,height);
//	}
	char buf[textNum]={0};
	int length = DoScreenMsg(msg,address+channel,buf);
// #ifdef _DEBUG
// 	CComputeFuncationTime time;
// 	time.SetStartTime(clock());
// #endif
//	CComInit* pComInit = CComInit::GetInstance();
// 	Sleep(1);
// 	int res=WriteFile(pComInit->m_hComWndScreen,
// 		buf,length,&dwWrite,NULL);
	if(IP.IsEmpty())
	{
		WriteComMsg *pMsg = new WriteComMsg;
//	pMsg->buf = new char(length+1);
		memset(pMsg->buf,0,textNum);
		memcpy(pMsg->buf,buf,length);
		pMsg->length = length;
		CDoComInOut* pComInOut = CDoComInOut::GetInstance();
		pComInOut->AddWriteComMsg(pMsg);
// #ifdef _DEBUG
// 	time.SetFinshTime(clock());
// 	CString strTime;
// 	strTime.Format(_T("%f"),time.GetDuration());
// 	MyWriteConsole(strTime);
// #endif
	}
	else
	{
		std::string recvBuf;
		int actRecvSize = 0;
//		CComplSocketClient client;
//		return client.SendData(port,IP,buf,length,recvBuf,actRecvSize);
		MySocketUDP myUdp;
		bRet = myUdp.SendTo(buf, length, IP, port);
	}
	return bRet;
}

int SLZCWndScreen::FindChannelWidth(int address,int channel,int& height)
{
	int width = -1;
	std::list<ThrScreenBasicMsg>::const_iterator itera = m_list_thrBasicMsg.begin();
	for(itera;itera!=m_list_thrBasicMsg.end();itera++)
	{
		ThrScreenBasicMsg msg = *itera;
		if(address == msg.address && channel == msg.channel)
		{
			width = msg.width/msg.fone*2;
			if(msg.width%msg.fone>=msg.fone/2)
			{
				width++;
			}
			height = msg.height/msg.fone;
			break;
		}
	}
	return width;
}

void SLZCWndScreen::AddThroughScreenMsg(const CString& msg,int address,int channel,const CString& IP/* =_T */,USHORT port)
{
	if(address<=0||msg.IsEmpty())return;
	SendThrScreenMsg sendMsg;
	sendMsg.msg = msg;
	sendMsg.address = address;
	sendMsg.channel = channel;
	sendMsg.IP = IP;
	sendMsg.port = port;
	m_ThrWndMutex.Lock();
	m_list_sendThrMsg.push_back(sendMsg);
	m_ThrWndMutex.Unlock();
}

DWORD WINAPI SLZCWndScreen::DoThrWndMsgThread(LPVOID pParam)
{
	SLZCWndScreen* pThis = (SLZCWndScreen*)pParam;
	while(TRUE)
	{
		if(pThis->m_list_sendThrMsg.empty())
		{
			Sleep(10);
		}
		else
		{
			SendThrScreenMsg msg;
			pThis->m_ThrWndMutex.Lock();
			std::list<SendThrScreenMsg>::const_iterator itera = pThis->m_list_sendThrMsg.begin();
			msg = *itera;
			pThis->m_list_sendThrMsg.pop_front();
			pThis->m_ThrWndMutex.Unlock();
//			WaitForSingleObject(pThis->m_hDoWndScreenMsgThread,3);
			pThis->SendDataToThroughScreen(msg.msg,msg.address,msg.channel,msg.IP,msg.port);

// 			CRect rect;
// 			static UINT left=MyCommon::GetProfileInt(_T("sys"),_T("LEFT"),0,_T("sys\\VisonShow.ini"));
// 			static UINT right=MyCommon::GetProfileInt(_T("sys"),_T("RIGHT"),256,_T("sys\\VisonShow.ini"));
// 			static UINT top = MyCommon::GetProfileInt(_T("sys"),_T("TOP"),0,_T("sys\\VisonShow.ini"));
// 			static UINT bottom = MyCommon::GetProfileInt(_T("sys"),_T("BOTTOM"),64,_T("sys\\VisonShow.ini"));
// 			rect.left = left; rect.right = right; rect.top = top; rect.bottom = bottom;
// 			//theApp.m_pNurseWnd->m_pVisonShowLED->SendDataToScreen(strTemp,IP,rect,10*1000);
// 			((CNurseStationApp*)AfxGetApp())->m_pNurseWnd->m_pVisonShowLED->SendDataToScreen(msg.msg,msg.IP,rect,10*1000);
		}
	}
	return 0;
}