#ifndef _HALLQUEFRONT_DATADEF_H
#define _HALLQUEFRONT_DATADEF_H


#define EVA_MSG WM_USER+1019
#define CALL_MSG WM_USER+1117
#define  textNum 2048 //最大处理文字限制
/*指令类别枚举*/
typedef enum _tagCmdType
{
	cmdNotDefined		= 0,		//未定义
	cmdLogin			= 1,		//登录
	cmdQuit				= 2,		//退出
	cmdCall				= 3,		//呼叫
	cmdRecall			= 4,		//重呼
	cmdDiscard			= 5,		//过号
	cmdWait				= 6,		//呼叫下一位准备
	cmdEvaReq			= 7,		//评价请求
	cmdPause			= 8,		//暂停
	cmdResume			= 9,		//恢复
	cmdCallNum			= 10,		//呼叫特定号码
	cmdCallSec          = 11,		//呼叫保安
	cmdCallMana			= 12,		//呼叫大堂经理
	cmdCallBusc			= 13,		//呼叫业务顾问
	cmdExChange			= 14,		//转移队列
	//呼叫器显示以及其他命令
	callerCmdShowAdd	= 15,		//显示地址
	callerCmdShowSuc	= 16,		//显示成功(即处理成功)
	callerCmdShowFail	= 17,		//显示失败
	callerCmdShowNum	= 18,		//显示剩余人数
	callerCmdShowAlarm	= 19		//发出滴滴声
} CmdType;

//typedef enum _tagCtlCmdType
//{
//	//呼叫器显示以及其他命令
//	callerCmdNotDefine	= 0,		//未定义
//	callerCmdShowAdd	= 1,		//显示地址
//	callerCmdShowSuc	= 2,		//显示成功(即处理成功)
//	callerCmdShowFail	= 3,		//显示失败
//	callerCmdShowNum	= 4,		//显示剩余人数
//	callerCmdShowAlarm	= 5			//发出滴滴声
//} CtlCmdType;

/*评价等级枚举*/
typedef enum _tagEvaLevel
{
	evaNone			= -1,		//未评价
	evaBad				= 0,		//不满意
	evaNormal		= 1,		//一般
	evaGood			= 2,		//满意
	evaPefect			= 3		//非常满意
} EvaLevel;

/*卡类型*/
typedef enum _tagCardTypeEnum
{
	cardNotDefine		= 0,		//未定义
	cardIDCard			= 1,		//身份证
	cardMagCard			= 2			//银行卡
} CardType;



/*
刷卡的信息
*/
typedef struct _tagCardInfo
{
public:
	_tagCardInfo(){
		nAttchPageID = -1;
		iCustLevel = 0;
	}
	~_tagCardInfo(){}
public:
	CardType iCardType;//卡类型
	CString strCardNumber;//卡号,身份证号
	UINT iCustLevel;//用户级别,用于对接远程服务器(有待完善)
	CString strCustName;//用户姓名，用于刷身份证
	CString strAttchQueID;//属于那个队列的队列ID
	int nAttchPageID;//链接哪个界面
}CARDINFO,*PCARDINFO;
/*
typedef struct _tagLogicVariables//系统综合逻辑变量
{
	BOOL IsAutoCloseComputer;//自动关机
	CTime CloseComputerTime;//关机时间
	BOOL IsAutoClearData;//系统自动清除数据
	CTime ClearDataTime;//自动清除数据时间
	BOOL IsLackPaperAlarm;//缺纸报警
	BOOL ExchangeQue;//转移队列
	BOOL ExchangeWindow;//转移窗口
	BOOL IsAutoSendToServer;//是否发送生成数据到服务端
	BOOL IsUseJtts;			//是否使用jtts语音库
	WCHAR strOrganNmae[addNum];//机构名称
	WCHAR strOrganID[addNum];//机构代码//支行代码
	WCHAR strParOrgName[addNum];//父级机构名称
	WCHAR strParOrgID[addNum];//父级机构ID
	int playSpeed;			//JTTS 播放速度
	UINT iNumberCallType;	//数字播放格式 
	BOOL IsAutoChangePage;
}LogicVariables,*pLogicVariables;
*/
/*
显示缺纸还是显示等候
*/
typedef struct _tagShowVariables
{
	BOOL bShowNoPage;
	BOOL bShowWait;
}ShowVariables,*pShowVariables;

///写串口数据
typedef struct _tagWriteComMsg
{
	int length;
	char buf[textNum];
}WriteComMsg;
/*
#define STRING_QUEUEID_CALL			(_T("[排队号码]"))			//呼叫排队号码
#define STRING_CUSTNAME_CALL		(_T("[客户姓名]"))			//呼叫客户姓名
#define STRING_WINDOWNAME			(_T("[窗口名称]"))			//窗口名称

#define COLORCODE_RED				(_T("#0#"))			//红色
#define COLORCODE_GREEN				(_T("#1#"))			//绿色
#define COLORCODE_YELLOW			(_T("#2#"))			//黄色
*/
//typedef enum _tagGenderEnum
//{
//	gdNotDefined		= 0,	//未定义
//	gdMale				= 1,	//男
//	gdFemale			= 2		//女
//} Gender;
/*
#define MYGENDER_NONE	 2014		//未定义
#define MYGENDER_MALE		 2015		//男
#define MYGENDER_FEMALE	 2016		//女

typedef USHORT GENDER;

#define COLORREF_RED				0x0000FF			//红色
#define COLORREF_GREEN				0x00FF00			//绿色
#define COLORREF_YELLOW				0x00FFFF			//黄色
*/
#endif //_HALLQUEFRONT_DATADEF_H