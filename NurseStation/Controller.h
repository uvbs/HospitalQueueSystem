#pragma once

#include "MySocketUDP.h"
//#include "SockDef.h"
#include "ado2.h"
#include "MyTimer.h"
//#include "NurseDef.h"

/***********定义护士站与医生呼叫器通信数据协议************/

//定义字段长度
#define MAX_ID_LENGTH				32			//用户ID长度
#define MAX_NAME_LENGTH				32			//用户姓名长度
//#define USERSIGN_LENGTH			256			//用户签名长度
#define MAX_CHATMSG_LENGTH			1024		//聊天信息长度
//#define MAX_PACKEGEDATA_LENGTH	((MAX_ID_LENGTH+MAX_NAME_LENGTH+MAX_CHATMSG_LENGTH)*3)
//数据包信息长度
#define MAX_IP_LENGTH				32			//IP地址字符串长度
#define MAX_PASSWORD_LENGTH			32			//IP地址字符串长度
#define MAX_PATIENTS_SHOW			10			//医生端显示的患者最大数
#define MAX_STR_LENGTH				256


////////////////////////////////////////////////////////////////////////////////

typedef enum _tagNetTypeIDEnum
{
	ntiNotDefined		= 0,				//未定义
	ntiReqLogin			= 1,				//登录请求
	ntiLoginVerify			= 2,				//登录验证结果
	ntiLogin					= 3,				//登录
	ntiQuit						= 4,				//退出
	ntiHeartBeat			= 5,				//心跳包
	ntiDoctorCmd			= 6,				//医生指令
	ntiCmdResult			= 7,				//医生指令执行结果
	//ntiDoctorEvaluate	= 8,				//医生请求评价
	ntiChatMsg				= 8				//聊天内容
} NetTypeIDEnum;

/*聊天消息类别*/
typedef enum _tagChatTypeIDEnum
{
	ctPublicChat		= 0,		//群聊消息
	ctPrivateChat		= 1		//私聊消息
} ChatTypeIDEnum;

/*医生指令类别枚举*/
typedef enum _tagDoctorCmdEnum
{
	dcCmdNotDefined	= 0,		//未定义
	//dcCmdLogin		= 1,		//登录
	//dcCmdQuit			= 2,		//退出
	dcCmdCall				= 1,		//呼叫
	dcCmdRecall			= 2,		//重呼
	dcCmdDiscard		= 3,		//过号
	dcCmdWait				= 4,		//呼叫下一位准备
	dcCmdEva				= 5,		//评价请求
	dcCmdPause			= 6,		//医生暂停就诊
	dcCmdResume		= 7		//医生恢复就诊
	//dcCmdChangeStatus	= 10		//暂停或恢复就诊状态
} DoctorCmdEnum;

/*评价等级枚举*/
typedef enum _tagEvaluateLevelEnum
{
	evlNone			= 0,		//未评价
	evlLevel1			= 1,		//等级1
	evlLevel2			= 2,		//等级2
	evlLevel3			= 3,		//等级3
	evlLevel4			= 4		//等级4
} EvaluateLevelEnum;

/*科室中患者的状态*/
typedef enum _tagQueueStatusEnum
{
	qsDoing				= 0,		//呼叫/就诊
	qsPrepare			= 1,		//准备
	qsInLine				= 2,		//排队
	qsSuspend			= 3,		//挂起
	qsDone				= 4,		//完成就诊
	qsDiscard			= 5,		//过号
	qsNotDefined		= 6		//未定义
} QueueStatusEnum;

/*验证结果*/
typedef enum _tagVerifyResultEnum
{
	vrNotDefine			= 0,		//未定义
	vrPass						= 1,		//验证通过
	vrNotPass				= 2,		//验证不通过
	vrLogined				= 3,		//该帐号已登录
	vrDBError				= 4,		//数据库错误
	vrNotFoundOffice	= 5,		//该护士站不处理医生所在科室
	vrFoundOffice			= 6		//申请登录的护士帐号与某已登录的护士帐号的可处理科室存在重复
} VerifyResultEnum;

/*用户类型*/
typedef enum _tagUserTypeEnum
{
	utNotDefine		= 0,		//未定义
	utDoctor				= 1,		//医生
	utNurse				= 2		//护士
} UserTypeEnum;

//排队系统数据库参数
typedef struct _tagDbParam
{
	WCHAR				wszDriver[32];
	WCHAR				wszHost[32];
	USHORT				m_uPort;
	WCHAR				wszUID[32];
	WCHAR				wszPID[32];
	WCHAR				wszDbName[32];
	INT						m_nDataDays;
	INT						m_nCallSeconds;
} DBPARAM, *LPDBPARAM;

////////////////////////////////////////////////////////////////////////
//////////////////////////	Socket 数据包定义	////////////////////////

typedef struct _tagUserInfo
{
	UserTypeEnum		eUserType;
	WCHAR				wszUserID[MAX_ID_LENGTH];
	WCHAR				wszUserName[MAX_NAME_LENGTH];
	WCHAR				wszOffices[MAX_STR_LENGTH];// _T("OfficeId1,OfficeName1|OfficeId2,OfficeName2|OfficeId3,OfficeName3")
} USERINFO, *LPUSERINFO;

/*Socket数据包包头*/
typedef struct _tagNetHeader
{
	NetTypeIDEnum		eTypeID;		//通信数据类别
	DWORD					nDataLength;	//数据长度
	SOCKADDR_IN		sSockAddrIn;	//数据包中转时保存源地址
	USERINFO				sUserInfo;
} NETHEADER, *LPNETHEADER;

typedef struct _tagBaseInfo
{
	NETHEADER				header;
} *LPBASEINFO;

//客户端登录请求数据
typedef struct _tagReqLogin
{
	NETHEADER			header;
	//USERINFO			sUserInfo;
	WCHAR					wszPassword[MAX_PASSWORD_LENGTH];
} REQLOGIN, *LPREQLOGIN;

//服务器响应客户端登录请求数据
//typedef struct _tagLoginResult
//{
//	NETHEADER				header;
//	//USERINFO				sUserInfo;
//	BOOL						bCallSpecial;
//	VerifyResultEnum		eRes;
//	DBPARAM				dbParam;
//} LOGINRESULT, *LPLOGINRESULT;

//用户广播数据
typedef struct _tagBroadcast
{
	NETHEADER			header;
	//USERINFO			sUserInfo;
} BROADCAST, *LPBROADCAST, LOGIN, *LPLOGIN, HEARTBEAT, *LPHEARTBEAT;

//用户退出数据
typedef struct _tagQuit
{
	NETHEADER			header;
	//USERINFO			sUserInfo;
	int							nExitCode;
} QUIT, *LPQUIT;

/*医生呼叫器发送到护士站的指令的数据结构*/
typedef struct _tagDoctorCmd
{
	NETHEADER				header;
	//USERINFO				sUserInfo;
	DoctorCmdEnum		eDoctorCmd;
	WCHAR						szQueSerialID[MAX_ID_LENGTH];
	int								iCode;
} DOCTORCMD, *LPDOCTORCMD;

/*护士站发送给医生端的患者信息结构*/
typedef struct _tagPatientToDoctor
{
	WCHAR						wszSerialID[MAX_ID_LENGTH];
	WCHAR						wszQueueID[MAX_ID_LENGTH];
	WCHAR						wszPatientName[MAX_NAME_LENGTH];
	WCHAR						wszPatientGender[8];
	WCHAR						wszPatientAge[16];
	QueueStatusEnum		eStatus;			
}PATIENTTODOCTOR, *LPPATIENTTODOCTOR;

/*护士站发送给医生端的患者信息结构*/
typedef struct _tagCmdResult
{
	NETHEADER				header;
	NetTypeIDEnum			eTypeIDFrom;
	DoctorCmdEnum		eDoctorCmd;
	USERINFO					sUserInfoFrom;
	BOOL							bSucceed;
	BOOL							bCallSpecial;
	VerifyResultEnum		eRes;
	int								nPatientCount;
	//int								nSentCount;
	//PATIENTTODOCTOR		patientsArray[MAX_PATIENTS_SHOW];
	BOOL							bExpertFlag;
	DBPARAM					sDbParam;
}CMDRESULT, *LPCMDRESULT;


/*聊天内容的数据结构*/
typedef struct _tagChatMsg
{
	NETHEADER				header;
	//USERINFO				sUserInfo;
	ChatTypeIDEnum		eChatTypeID;
	WCHAR						wszMsg[MAX_CHATMSG_LENGTH + 2];
} CHATMSG, *LPCHATMSG;

//#define MAX_PACKEGEDATA_LENGTH	(sizeof(CHATMSG) > sizeof(CMDRESULT) ? sizeof(CHATMSG) : sizeof(CMDRESULT))
#define MAX_PACKEGEDATA_LENGTH	sizeof(CHATMSG)

/*通信的UDP数据包,用于Socket服务器(护士站)接收数据，然后再进行数据类型判断*/
typedef struct _tagUDPPackege
{
	NETHEADER		header;
	CHAR					szData[MAX_PACKEGEDATA_LENGTH - sizeof(NETHEADER)];
} UDPPACKEGE, *LPUDPPACKEGE;


//////////////////////////	End Socket 数据包定义		////////////////////////

#define MY_DEFAULT_LOCAL_PORT		4020		//缺省系统内端口

class CController : public MySocketUDP
{
public:
	CController();
	virtual ~CController();

	BOOL StartControl();
	BOOL EndControl();

	CString GetUserId() const
	{
		return m_strCtlUserId;
	}
	void SetUserId(CString strUserId)
	{
		m_strCtlUserId = strUserId;
	}
	CString GetUserName() const
	{
		return m_strCtlUserName;
	}
	void SetUserName(CString strUserName)
	{
		m_strCtlUserName = strUserName;
	}
	UserTypeEnum GetUserType() const
	{
		return m_eCtlUserType;
	}
	void SetUserType(UserTypeEnum eUserType)
	{
		m_eCtlUserType = eUserType;
	}
	CString GetOffices() const
	{
		return m_strCtlOffices;
	}
	void SetOffices(CString strOffices)
	{
		m_strCtlOffices = strOffices;
	}
	BOOL GetOnline() const
	{
		return m_bOnline;
	}
	void SetOnline(BOOL bOnline)
	{
		m_bOnline = bOnline;
	}
	void SetSendBaseInfo(LPBASEINFO pBaseInfo, NetTypeIDEnum eNetType, int iStructSize)
	{
		pBaseInfo->header.eTypeID = eNetType;
		pBaseInfo->header.nDataLength = iStructSize;
		wcscpy_s(pBaseInfo->header.sUserInfo.wszUserID, MAX_ID_LENGTH, GetUserId().GetBuffer());
		wcscpy_s(pBaseInfo->header.sUserInfo.wszUserName, MAX_NAME_LENGTH, GetUserName().GetBuffer());
		wcscpy_s(pBaseInfo->header.sUserInfo.wszOffices, MAX_STR_LENGTH, GetOffices().GetBuffer());
		pBaseInfo->header.sUserInfo.eUserType = GetUserType();
	}
	USHORT GetPort(){return m_uPort;}

protected:

	virtual BOOL OnStartControl();
	virtual BOOL OnEndControl();

	virtual void DispatchCmd(LPUDPPACKEGE pUDPPackege, LPSOCKADDR_IN psockddr);

	///////		CMD dispatch function	/////////////////////////////
	virtual void OnRecvReqLogin(LPREQLOGIN pReqLogin, LPSOCKADDR_IN psockddr) = 0;
	virtual void OnRecvLogin(LPBROADCAST pLogin, LPSOCKADDR_IN psockddr) = 0;
	virtual void OnRecvQuit(LPQUIT pQuit, LPSOCKADDR_IN psockddr) = 0;
	virtual void OnRecvHeartBeat(LPBROADCAST pHeartbeat, LPSOCKADDR_IN psockddr) = 0;
	virtual void OnRecvDoctorCmd(LPDOCTORCMD pDoctorCmd, LPSOCKADDR_IN psockddr) = 0;
	virtual void OnRecvCmdResult(LPCMDRESULT pCmdResult, LPSOCKADDR_IN psockddr) = 0;
	virtual void OnRecvChatMsg(LPCHATMSG pChatMsg, LPSOCKADDR_IN psockddr) = 0;
	///////		End	CMD dispatch function	/////////////////////////

	virtual void OnSetPort();

protected:

	void SetPort(USHORT uPort);

protected:
	virtual void OnRecvData();
	//BOOL DispatchCmd(LPUDPPACKEGE pUDPPackege, LPSOCKADDR_IN psockddr);

protected:

	USHORT m_uPort;

	CString m_strCtlUserId;
	CString m_strCtlUserName;
	UserTypeEnum m_eCtlUserType;
	CString m_strCtlOffices;
	BOOL m_bOnline;
};


