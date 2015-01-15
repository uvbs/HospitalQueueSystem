#ifndef HARDWAREDEF_H
#define HARDWAREDEF_H

/*条屏数据发送*/
#define WNDSCREEN_HEAD_FIRST      0xA0//数据头
#define WNDSCREEN_HEAD_SCEND      0x90//数据头
#define WNDSCREEN_SHOW_ROM        0xA1//显示条屏ROM数据
#define WNDSCREEN_SET_ROM         0xA2//设置ROM
#define WNDSCREEN_SHOW_INFOR      0xA5//显示信息

/*硬件呼叫器数据*/
#define HARDWARE_CALLER_START     0x0A//开始
#define HARDWARE_CALLER_WAIT	  0x0B//等候
#define HARDWARE_CALLER_CALL      0x0C//呼叫
#define HARDWARE_CALLER_RECALL    0x0D//重呼
#define HARDWARE_CALLER_CANCEL    0x0E//取消
#define HARDWARE_CALLER_FUNCATION 0x0F//功能
#define HARDWARE_CALLER_SHOW      0x02//功能码02
#define HARDWARE_CALLER_SYSTEM    0x03//系统呼叫呼叫器
#define HARDWARE_CALLER_HEAD	  0xFF
#define HARDWARE_CALLER_CONST     0x68
#define HARDWARE_CALLER_TAIL      0x16

/*硬件评价器数据*/
#define HARDWARE_EVALTOR_HEAD       0xFF//头
#define HARDWARE_EVALTOR_CONST      0x68
#define HARDWARE_EVALTOR_TAIL       0x16//尾
#define HARDWARE_EVALTOR_TOSYS   0x11//评价起发数据到系统
#define HARDWARE_EVALTOR_TOEVA      0x01//系统发数据到评价器
#define HARDWARE_EVALTOR_TOSTAR     0x02//显示星级
#define HARDWARE_EVALTOR_TOSTART    0x01//呼叫器向评价器发数据
#define HARDWARE_EVALTOR_NOLEVEL    0x00//0星
#define HARDWARE_EVALTOR_LEVELONE   0x01//1星
#define HARDWARE_EVALTOR_LEVELTWO   0x02//2星
#define HARDWARE_EVALTOR_LEVELTHREE 0x03//3星
#define HARDWARE_EVALTOR_LEVELFOUR  0x04//4星
#define HARDWARE_EVALTOR_LEVELFIVE  0x05//5星
#define HARDWARE_EVALTOR_GRADEZERO  0x00//差
#define HARDWARE_EVALTOR_GRADEONE   0x01//一般
#define HARDWARE_EVALTOR_GRADETWO   0x02//较好
#define HARDWARE_EVALTOR_GRADETHREE 0x03//好
#define HARDWARE_EVALTOR_GRADFOUR   0x04//很好

#define HARDWARE_THROUGH_FLAG1 0xAA
#define HARDWARE_THROUGH_FLAG2 0xBB
//////评价超时数据结构
typedef struct _tagEvaltOutTime
{
	int address;
	BOOL IsEvalt;
	UINT time;
}EvaltOutTime,*LPEVALTOUTTIME;

/****** 评价等级 ***********/
typedef enum _tagEvaLevelEum
{
	elNone				= 0,		//未评价
	elGood				= 3,		//好
	elMid				= 2,		//中
	elBad				= 1,		//差
	elBest              = 4         //非常满意
} EvaLevelEum;

typedef struct _tagWndScreenMsg
{
	int         address;
	CString		strmsg;
	int         color;
}WNDSCREENMSG,*LPWNDSCREENMSG;

/*******通屏数据包头**************/
typedef struct _tagThroughScreenHead
{
	unsigned char flagFirst;	//0xAA
	unsigned char address;	//地址	（0-255），其中255为广播地址
	//TCP通讯时采用广播地址通讯
	unsigned char flagSecond;	//0xBB
	unsigned char type;		//数据包类型	高4位：0-命令类，1-数据类；
	//低4位：0-无校验，1-CRC校验
	unsigned char index;	//序号	保留0
	unsigned short int length;	//数据长度	注：(长度 = 数据+[CRC校验字节])
	//长度高字节在前 长度低字节在后
}ThroughScreenHead,*pThroughScreenHead;

/******通屏对应通道基本信息List**********/
typedef struct//	(34字节)
{
	unsigned char   TongDaoHao;     //通道号
	unsigned char   PlayType;       //类型 0-文本,1图片
	unsigned char   ShowTitle;      //是否设置标题行
	unsigned char   ShowFlag;       //更新后单屏立即显示
	unsigned short  Width;          //窗口宽
	unsigned short  Height;         //窗口高
	unsigned char   Font;           //字体	16点,24点
	unsigned char   IsSameColor;    //是否颜色相同
	unsigned int    Color;          //颜色
	unsigned char   Name[20];       //标识名称
}TQtList,*pTQtList;

/*****通屏基本信息************/
typedef struct//		(32字节)
{
	unsigned char   Flag1;          //是否有效标志   0xE9
	unsigned char   Flag2;          //是否有效标志   0x5A
	unsigned char   LedType;        //类型0-单红,1-单绿,2-双色
	unsigned char   GrayScale;      //灰度等级 设置为0
	unsigned short  Width;          //屏宽
	unsigned short  Height;         //屏高
	unsigned char   TXSelect;       //通讯类型 0-RS232,1-TCPIP,2-UDP
	unsigned char   IP[16];         //IP
	unsigned short  Port;           //端口
	unsigned char   Com;            //串口
	unsigned char   Baud;           //波特率0~7(1200,2400,4800,9600,19200,38400,57600,115200)
	unsigned char   LedAddr;        //地址	0~255,广播地址:255
	unsigned char   CheckLevel;     //校验等级0-无校验,1-CRC校验
	unsigned char   TXDelay;        //通讯延时
}TLedInfo,*pTLedInfo;
#endif