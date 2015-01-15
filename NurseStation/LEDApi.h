#ifndef __ledsender__
#define __ledsender__

/*
   参数说明
   
*/
#include <windows.h>

//显示数据命令
#define ROOT_PLAY_LEAF         0x27  
//插入或者更新某一页面
#define ROOT_PLAY_OBJECT       0x29  
//插入或者更新某一对象
#define ROOT_UPDATE            0x13  
//更新下位机程序
#define ROOT_FONTSET           0x14  //下载字库
#define ROOT_PLAY              0x21  //节目数据，保存到RAM，掉电丢失
#define ROOT_DOWNLOAD          0x22  //节目数据，保存到Flash
#define ROOT_PLAY_CHAPTER      0x23  //插入或者更新某一节目
#define ROOT_PLAY_REGION       0x25  
//插入或者更新某一分区
//缺省数据包大小
#define DEFAULT_PKP_LENGTH     512

//IP地址、MAC地址长度定义
#define IP_ADDRESS_LENGTH      4
#define ETHER_ADDRESS_LENGTH   6

//通讯设备类型
#define DEVICE_TYPE_COM    0  //串口通讯
#define DEVICE_TYPE_UDP    1  //网络通讯
#define DEVICE_TYPE_485    2  //485通讯

//串口或者485通讯使用得通讯速度(波特率)
#define SBR_57600          0
#define SBR_38400          1
#define SBR_19200          2
#define SBR_9600           3

//是否等待下位机应答，直接发送所有数据
#define NOTIFY_NONE        1
//是否阻塞方式；是则等到发送完成或者超时，才返回；否则立即返回
#define NOTIFY_BLOCK       2
//是否将发送结果以Windows窗体消息方式送到调用得应用
#define NOTIFY_EVENT       4
//使用一个本地端口，和多个目标IP通讯
#define NOTIFY_MULTI       8

#define R_DEVICE_READY     0
#define R_DEVICE_INVALID   -1
#define R_DEVICE_BUSY      -2
#define R_FONTSET_INVALID  -3
#define R_DLL_INIT_IVALID  -4
#define R_IGNORE_RESPOND   -5

//Chapter和Leaf中，播放时间控制
#define WAIT_USE_TIME      0  //按照指定的时间长度播放，到时间就切到下一个
#define LED_WAIT_CHILD         1  //等待子项目的播放，如果到了指定的时间长度，而子项目还没有播完，则等待播完

#define V_FALSE            0
#define V_TRUE             1

#define COLOR_MODE_FULLCOLOR         4  //全彩




#define ACTMODE_INSERT         0  //插入操作
#define ACTMODE_REPLACE        1  //替换操作

//RAM节目播放
#define ROOT_SURVIVE_ALWAYS    -1

//Windows字体类型定义
#define WFS_NONE               0x0   //普通样式
#define WFS_BOLD               0x01  //粗体
#define WFS_ITALIC             0x02  //斜体
#define WFS_UNDERLINE          0x04  
//下划线
#define WFS_STRIKEOUT          0x08  
//删除线

#define NOTIFY_BUFFER_LEN		  512

//显示屏基色类型
#define COLOR_MODE_MONO              1  //单色
#define COLOR_MODE_DOUBLE            2  //双色
#define COLOR_MODE_THREE             3  
//无灰度全彩


//流控制常量
#define FLOW_NONE          0
#define FLOW_RTS_CTS       1

//下位机应答标识
#define LM_RX_COMPLETE         1
#define LM_TX_COMPLETE         2
#define LM_RESPOND             3
#define LM_TIMEOUT             4
#define LM_NOTIFY              5
#define LM_PARAM               6
#define LM_TX_PROGRESS         7
#define LM_RX_PROGRESS         8
#define RESULT_FLASH           0xff

//电源开关状态
#define LED_POWER_ON       1
#define LED_POWER_OFF      0

//内码文字大小
#define FONT_SET_16        0      
//16点阵字符
#define FONT_SET_24        1      
//24点阵字符

//正计时、倒计时type参数
#define CT_COUNTUP         0      
//正计时
#define CT_COUNTDOWN       1      
//倒计时
//正计时、倒计时format参数
#define CF_HNS             0      
//时分秒（相对值）
#define CF_HN              1      
//时分（相对值）
#define CF_NS              2      
//分秒（相对值）
#define CF_H               3      
//时（相对值）
#define CF_N               4      
//分（相对值）
#define CF_S               5      
//秒（相对值）
#define CF_DAY             6      
//天数（绝对数量）
#define CF_HOUR            7      
//小时数（绝对数量）
#define CF_MINUTE          8      
//分钟数（绝对数量）
#define CF_SECOND          9      
//秒数（绝对数量）

//模拟时钟边框形状
#define SHAPE_RECTANGLE    0      
//方形
#define SHAPE_ROUNDRECT    1      
//圆角方形
#define SHAPE_CIRCLE       2      
//圆形

//命令代码定义
#define PKC_RESPOND               3
#define PKC_QUERY                 4
#define PKC_OVERFLOW              5
#define PKC_ADJUST_TIME           6
#define PKC_GET_PARAM             7
#define PKC_SET_PARAM             8
#define PKC_GET_POWER             9
#define PKC_SET_POWER             10
#define PKC_GET_BRIGHT            11
#define PKC_SET_BRIGHT            12
#define PKC_COM_TRANSFER          21
#define PKC_GET_POWER_SCHEDULE    60
#define PKC_SET_POWER_SCHEDULE    61
#define PKC_GET_BRIGHT_SCHEDULE   62
#define PKC_SET_BRIGHT_SCHEDULE   63
#define PKC_SET_CURRENT_CHAPTER   66
#define PKC_NOTIFY                100
#define PKC_MODIFY_IP             7654
#define PKC_MODIFY_MAC            7655

#define NOTIFY_SET_PARAM          0x00010004  
//设置参数

typedef struct TIMESTAMP{
	long   date;
	long   time;
}TTimeStamp, *PTimeStamp;

typedef struct DEVICE_PARAM{
	WORD  devType;
	WORD  comSpeed;
	WORD  comPort;
	WORD  comFlow;
	WORD  locPort;
	WORD  rmtPort;
	WORD  srcAddr;
	WORD  dstAddr;
	char  rmtHost[16];
	DWORD txTimeo;   //发送后等待应答时间 ====超时时间应为txTimeo*txRepeat
	DWORD txRepeat;  //失败重发次数
	DWORD txMovewin; //划动窗口
	DWORD key;
	long  pkpLength; //数据包大小
}TDeviceParam, *PDeviceParam;

typedef struct SENDER_PARAM{
	TDeviceParam devParam;
	long  wmHandle;
	long  wmMessage;
	long  wmLParam;
	long  notifyMode;
}TSenderParam, *PSenderParam;

typedef struct NOTIFY_PARAM{
	WORD  Notify;
	WORD  Command;
	long  Result;
	long  Status;
	TSenderParam Param;
	BYTE  Buffer[NOTIFY_BUFFER_LEN];
	DWORD Size;
}TNotifyParam, *PNotifyParam;

typedef struct POWER_SCHEDULE{
	DWORD Enabled;
	DWORD Mode;
	TTimeStamp OpenTime[21];
	TTimeStamp CloseTime[21];
	DWORD Checksum;
}TPowerSchedule, *PPowerSchedule;

typedef struct BRIGHT_SCHEDULE{
	DWORD Enabled;
	BYTE  Bright[24];
	DWORD Checksum;
}TBrightSchedule, *PBrightSchedule;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* 在线控制卡列表相关接口函数                                           */
/************************************************************************/

typedef struct DEVINFO{
	char  dev_name[32];  //设备名称
	DWORD dev_id;        //设备标识
	DWORD dev_ip;        //设备IP地址
	WORD  dev_addr;      //设备地址
	WORD  dev_port;      //设备端口
	DWORD reserved[5];   //系统保留
}TDevInfo, *PDevInfo;

typedef struct DEVICEREPORT{
	TDevInfo devinfo;    //设备信息
	double   timeupdate; //最后刷新时间
}TDeviceReport, *PDeviceReport;









/*
HINSTANCE hInstance;

void LED_Initialize(void)
{
  if ((hInstance=LoadLibrary("LEDSender2010.dll"))!=NULL)
  {
    LED_Startup=(long (_stdcall *)(void))GetProcAddress(hInstance,"LED_Startup");
    LED_Cleanup=(long (_stdcall *)(void))GetProcAddress(hInstance,"LED_Cleanup");

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //在线控制卡列表相关接口函数
    LED_Report_CreateServer=(long (_stdcall *)(long, long))GetProcAddress(hInstance,"LED_Report_CreateServer");
	LED_Report_RemoveServer=(void (_stdcall *)(long))GetProcAddress(hInstance,"LED_Report_RemoveServer");
	LED_Report_RemoveAllServer=(void (_stdcall *)(void))GetProcAddress(hInstance,"LED_Report_RemoveAllServer");
	LED_Report_GetOnlineList=(long (_stdcall *)(long, void*, long))GetProcAddress(hInstance,"LED_Report_GetOnlineList");
	LED_Report_GetOnlineItemName=(char* (_stdcall *)(long, long))GetProcAddress(hInstance,"LED_Report_GetOnlineItemName");
	LED_Report_GetOnlineItemHost=(char* (_stdcall *)(long, long))GetProcAddress(hInstance,"LED_Report_GetOnlineItemHost");
	LED_Report_GetOnlineItemPort=(long (_stdcall *)(long, long))GetProcAddress(hInstance,"LED_Report_GetOnlineItemPort");
	LED_Report_GetOnlineItemAddr=(long (_stdcall *)(long, long))GetProcAddress(hInstance,"LED_Report_GetOnlineItemAddr");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    LED_ResetDisplay=(long (_stdcall *)(PSenderParam))GetProcAddress(hInstance,"LED_ResetDisplay");
    LED_AdjustTime=(long (_stdcall *)(PSenderParam))GetProcAddress(hInstance,"LED_AdjustTime");
    LED_AdjustTimeEx=(long (_stdcall *)(PSenderParam, LPSYSTEMTIME))GetProcAddress(hInstance,"LED_AdjustTimeEx");
    LED_SetCurChapter=(long (_stdcall *)(PSenderParam, long))GetProcAddress(hInstance,"LED_SetCurChapter");
    LED_SetPower=(long (_stdcall *)(PSenderParam, long))GetProcAddress(hInstance,"LED_SetPower");
    LED_GetPower=(long (_stdcall *)(PSenderParam))GetProcAddress(hInstance,"LED_GetPower");
    LED_SetBright=(long (_stdcall *)(PSenderParam, long))GetProcAddress(hInstance,"LED_SetBright");
    LED_GetBright=(long (_stdcall *)(PSenderParam))GetProcAddress(hInstance,"LED_GetBright");
    LED_SetPowerSchedule=(long (_stdcall *)(PSenderParam, PPowerSchedule))GetProcAddress(hInstance,"LED_SetPowerSchedule");
    LED_GetPowerSchedule=(long (_stdcall *)(PSenderParam))GetProcAddress(hInstance,"LED_GetPowerSchedule");
    LED_SetBrightSchedule=(long (_stdcall *)(PSenderParam, PBrightSchedule))GetProcAddress(hInstance,"LED_SetBrightSchedule");
    LED_GetBrightSchedule=(long (_stdcall *)(PSenderParam))GetProcAddress(hInstance,"LED_GetBrightSchedule");
    LED_SendToScreen=(long (_stdcall *)(PSenderParam, long))GetProcAddress(hInstance,"LED_SendToScreen");
    LED_ComTransfer=(long (_stdcall *)(PSenderParam, BYTE*, DWORD))GetProcAddress(hInstance,"LED_ComTransfer");

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //此处与之前同名函数功能相同，只是将结构体指针参数拆分成多个变量参数，方便PB、java等开发语言调用
    LED_UDP_SenderParam=(long (_stdcall *)(long, long, char*, long, long, long, long, long))GetProcAddress(hInstance,"LED_UDP_SenderParam");
    LED_COM_SenderParam=(long (_stdcall *)(long, long, long, long, long, long, long))GetProcAddress(hInstance,"LED_COM_SenderParam");
	LED_UDP_SenderParam_ByReportName=(long (_stdcall *)(long, long, long, char*, long, long, long))GetProcAddress(hInstance,"LED_UDP_SenderParam_ByReportName");
    LED_ResetDisplay2=(long (_stdcall *)(long))GetProcAddress(hInstance,"LED_ResetDisplay2");
    LED_AdjustTime2=(long (_stdcall *)(long))GetProcAddress(hInstance,"LED_AdjustTime2");
    LED_AdjustTimeEx2=(long (_stdcall *)(long, LPSYSTEMTIME))GetProcAddress(hInstance,"LED_AdjustTimeEx2");
    LED_SetCurChapter2=(long (_stdcall *)(long, long))GetProcAddress(hInstance,"LED_SetCurChapter2");
    LED_SetPower2=(long (_stdcall *)(long, long))GetProcAddress(hInstance,"LED_SetPower2");
    LED_GetPower2=(long (_stdcall *)(long))GetProcAddress(hInstance,"LED_GetPower2");
    LED_SetBright2=(long (_stdcall *)(long, long))GetProcAddress(hInstance,"LED_SetBright2");
    LED_GetBright2=(long (_stdcall *)(long))GetProcAddress(hInstance,"LED_GetBright2");
    LED_SetPowerSchedule2=(long (_stdcall *)(long, PPowerSchedule))GetProcAddress(hInstance,"LED_SetPowerSchedule2");
    LED_GetPowerSchedule2=(long (_stdcall *)(long))GetProcAddress(hInstance,"LED_GetPowerSchedule2");
    LED_SetBrightSchedule2=(long (_stdcall *)(long, PBrightSchedule))GetProcAddress(hInstance,"LED_SetBrightSchedule2");
    LED_GetBrightSchedule2=(long (_stdcall *)(long))GetProcAddress(hInstance,"LED_GetBrightSchedule2");
    LED_SendToScreen2=(long (_stdcall *)(long, long))GetProcAddress(hInstance,"LED_SendToScreen2");
    LED_ComTransfer2=(long (_stdcall *)(long, BYTE*, DWORD))GetProcAddress(hInstance,"LED_ComTransfer2");

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//读取控制卡参数
    LED_Cache_GetBoardParam=(long (_stdcall *)(PSenderParam))GetProcAddress(hInstance,"LED_Cache_GetBoardParam");
    LED_Cache_GetBoardParam2=(long (_stdcall *)(long))GetProcAddress(hInstance,"LED_Cache_GetBoardParam2");
	//提取各个参数
    LED_Cache_GetBoardParam_SaveToFile=(long (_stdcall *)(char*))GetProcAddress(hInstance,"LED_Cache_GetBoardParam_SaveToFile");
    LED_Cache_GetBoardParam_IP=(char* (_stdcall *)(void))GetProcAddress(hInstance,"LED_Cache_GetBoardParam_IP");
    LED_Cache_GetBoardParam_Mac=(char* (_stdcall *)(void))GetProcAddress(hInstance,"LED_Cache_GetBoardParam_Mac");
    LED_Cache_GetBoardParam_Addr=(long (_stdcall *)(void))GetProcAddress(hInstance,"LED_Cache_GetBoardParam_Addr");
    LED_Cache_GetBoardParam_Width=(long (_stdcall *)(void))GetProcAddress(hInstance,"LED_Cache_GetBoardParam_Width");
    LED_Cache_GetBoardParam_Height=(long (_stdcall *)(void))GetProcAddress(hInstance,"LED_Cache_GetBoardParam_Height");
    LED_Cache_GetBoardParam_Brightness=(long (_stdcall *)(void))GetProcAddress(hInstance,"LED_Cache_GetBoardParam_Brightness");
	//修改各个参数
    LED_Cache_SetBoardParam_LoadFromFile=(long (_stdcall *)(char*))GetProcAddress(hInstance,"LED_Cache_SetBoardParam_LoadFromFile");
    LED_Cache_SetBoardParam_IP=(void (_stdcall *)(char*))GetProcAddress(hInstance,"LED_Cache_SetBoardParam_IP");
    LED_Cache_SetBoardParam_Mac=(void (_stdcall *)(char*))GetProcAddress(hInstance,"LED_Cache_SetBoardParam_Mac");
    LED_Cache_SetBoardParam_Addr=(void (_stdcall *)(long))GetProcAddress(hInstance,"LED_Cache_SetBoardParam_Addr");
    LED_Cache_SetBoardParam_Width=(void (_stdcall *)(long))GetProcAddress(hInstance,"LED_Cache_SetBoardParam_Width");
    LED_Cache_SetBoardParam_Height=(void (_stdcall *)(long))GetProcAddress(hInstance,"LED_Cache_SetBoardParam_Height");
    LED_Cache_SetBoardParam_Brightness=(void (_stdcall *)(long))GetProcAddress(hInstance,"LED_Cache_SetBoardParam_Brightness");
	//将修改后的参数接入控制卡
    LED_Cache_SetBoardParam=(long (_stdcall *)(PSenderParam))GetProcAddress(hInstance,"LED_Cache_SetBoardParam");
    LED_Cache_SetBoardParam2=(long (_stdcall *)(long))GetProcAddress(hInstance,"LED_Cache_SetBoardParam2");

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LED_GetNotifyParam=(long (_stdcall *)(PNotifyParam, long))GetProcAddress(hInstance,"LED_GetNotifyParam");

    MakeFromVsqFile=(long (_stdcall *)(char*, long, long, long))GetProcAddress(hInstance,"MakeFromVsqFile");
    MakeRoot=(long (_stdcall *)(long, long, long))GetProcAddress(hInstance,"MakeRoot");
    MakeChapter=(long (_stdcall *)(long, long, long, long, DWORD, WORD))GetProcAddress(hInstance,"MakeChapter");
    MakeRegion=(long (_stdcall *)(long, long, long, long, long, long, long, long, long, long))GetProcAddress(hInstance,"MakeRegion");
    MakeLeaf=(long (_stdcall *)(long, long, long, long, long, long, DWORD, WORD))GetProcAddress(hInstance,"MakeLeaf");
    MakeObject=(long (_stdcall *)(long, long, long, long, long, long, long))GetProcAddress(hInstance,"MakeObject");

    AddChapter=(long (_stdcall *)(WORD, DWORD, WORD))GetProcAddress(hInstance,"AddChapter");
    AddRegion=(long (_stdcall *)(WORD, long, long, long, long, long))GetProcAddress(hInstance,"AddRegion");
    AddLeaf=(long (_stdcall *)(WORD, DWORD, WORD))GetProcAddress(hInstance,"AddLeaf");

    AddDateTime=(long (_stdcall *)(WORD, long, long, long, long, long, long, char*, long, long, long, long, long, long, long, char*))GetProcAddress(hInstance,"AddDateTime");
	AddCampaignEx=(long (_stdcall *)(WORD, long, long, long, long, long, long, char*, long, long, long, char*, PTimeStamp, PTimeStamp, PTimeStamp, long))GetProcAddress(hInstance,"AddCampaignEx");
    AddClock=(long (_stdcall *)(WORD, long, long, long, long, long, long, long, DWORD, DWORD, DWORD, long, long, long, DWORD, long, DWORD, long, DWORD, long, DWORD, long, DWORD))GetProcAddress(hInstance,"AddClock");
    AddMovie=(long (_stdcall *)(WORD, long, long, long, long, long, long, char*, long))GetProcAddress(hInstance,"AddMovie");

    AddWindows=(long (_stdcall *)(WORD, long, long, long, long, long, long))GetProcAddress(hInstance,"AddWindows");
    AddChildWindow=(long (_stdcall *)(WORD, HDC, long, long, long, long, long, long, long, long, long, long))GetProcAddress(hInstance,"AddChildWindow");
    AddChildPicture=(long (_stdcall *)(WORD, char*, long, long, long, long, long, long, long, long))GetProcAddress(hInstance,"AddChildPicture");
    AddChildText=(long (_stdcall *)(WORD, char*, char*, long, long, long, long, long, long, long, long, long, long, long, long))GetProcAddress(hInstance,"AddChildWindow");
    AddStrings=(long (_stdcall *)(WORD, long, long, long, long, long, long))GetProcAddress(hInstance,"AddStrings");
    AddChildString=(long (_stdcall *)(WORD, char*, long, long, long, long, long, long, long, long, long))GetProcAddress(hInstance,"AddChildString");

    AddWindow=(long (_stdcall *)(WORD, long, long, long, long, long, long, HDC, long, long, long, long, long, long, long, long, long, long))GetProcAddress(hInstance,"AddWindow");
    AddPicture=(long (_stdcall *)(WORD, long, long, long, long, long, long, char*, long, long, long, long, long, long, long, long))GetProcAddress(hInstance,"AddPicture");
    AddText=(long (_stdcall *)(WORD, long, long, long, long, long, long, char*, char*, long, long, long, long, long, long, long, long, long, long, long, long))GetProcAddress(hInstance,"AddText");
    AddString=(long (_stdcall *)(WORD, long, long, long, long, long, long, char*, long, long, long, long, long, long, long, long, long))GetProcAddress(hInstance,"AddString");

	if (LED_Startup) LED_Startup();
  }
}

void LED_Destroy(void)
{
	if (hInstance!=NULL) 
	{
		LED_Cleanup();
		FreeLibrary(hInstance);
	}
}
*/
#endif