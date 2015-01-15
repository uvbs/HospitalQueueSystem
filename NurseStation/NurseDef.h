#ifndef _NURSESYSTEMDEF_H
#define _NURSESYSTEMDEF_H


////////////////////////////////////////////////////////////////////////////////
/////////////////////////	跨类文件使用的数据定义	////////////////////////


//用户类别
#define USERTYPE_SYSADMIN			0			//系统管理员
#define USERTYPE_MANAGER			1			//后台管理员
#define USERTYPE_NURSE				2			//前台护士
#define USERTYPE_DOCTOR				-1			//医生

/*声音和显示设置的变量定义*/
#define STRING_QUEUEID_CALL			(_T("[排队号码]"))			//呼叫排队号码
#define STRING_PATIENTNAME_CALL		(_T("[患者姓名]"))			//呼叫患者姓名
#define STRING_QUEUEID_WAIT			(_T("[准备排队号码]"))		//准备排队号码
#define STRING_PATIENTNAME_WAIT		(_T("[准备患者姓名]"))		//准备患者姓名
#define STRING_DOCTORNAME			(_T("[医生姓名]"))			//医生姓名
#define STRING_OFFICENAME			(_T("[科室名称]"))			//科室名称
#define STRING_CONSULTNAME			(_T("[诊室名称]"))			//诊室名称
#define STRING_PATIENT_GENDER		(_T("[患者性别]"))			//患者性别
#define STRING_PATIENT_AGE			(_T("[患者年龄]"))			//患者年龄

#define COLORREF_RED				0x0000FF			//红色
#define COLORREF_GREEN				0x00FF00			//绿色
#define COLORREF_YELLOW				0x00FFFF			//黄色

#define COLORCODE_RED				(_T("#0#"))			//红色
#define COLORCODE_GREEN				(_T("#1#"))			//绿色
#define COLORCODE_YELLOW			(_T("#2#"))			//黄色



////////////////////////////////////////////////////////////////////////////
////////////////////	

/*在线医生的状态*/
typedef enum _tagDoctorStatusEnum
{
	dsWorking		= 0,		//工作状态
	dsPause			= 1,		//暂停状态
	usOutLine		= 2			//离线状态
} DoctorStatusEnum;

/*科室中患者优先级*/
typedef enum _tagPatientPriorityEnum
{
	priMinusTwo		= -2,		//优先级-2		[最低]	//预留值
	priMinusOne		= -1,		//优先级-1		[较低]	//预留值
	priZero			= 0,		//优先级0，默认	[普通]
	priOne			= 1,		//优先级1		[中等]
	priTwo			= 2,		//优先级2		[较高]
	priThree		= 3,		//优先级3		[最高]
	priFour			= 4,		//优先级4				//预留值
	priFive			= 5			//优先级5				//预留值
} PatientPriorityEnum;

/*聊天消息类别*/
typedef enum _tagGenderEnum
{
	genderFemale	= 0,		//女性
	genderMale		= 1,		//男性
	genderNeuter	= 2,		//中性
	genderNone		= -1		//不指定
} GenderEnum;

/*聊天消息类别*/
typedef enum _tagExpertEnum
{
	expertNo		= 0,		//非专家
	expertYes		= 1,		//专家
} ExpertEnum;


/////////////////////////////////////////////////////////////////////////////
////////////////////////


/*在线用户状态信息*/
/*
typedef struct _tagUserStatus
{
	WCHAR				wszUserID[MAX_ID_LENGTH];
	WCHAR				wszUserName[MAX_NAME_LENGTH];
	WCHAR				wszUserIP[MAX_IP_LENGTH];
	USHORT				iPort;
	SOCKADDR_IN			sSockAddrIn;
	DoctorStatusEnum		eUserStatus;
	short				nOnlineFlag;	//心跳包判断是否在线依据
	WCHAR               wszPatientName[MAX_NAME_LENGTH];//用户为医生时，保存呼叫/就诊患者
	WCHAR				wszOfficeID[MAX_ID_LENGTH];
}USERSTATUS, *LPUSERSTATUS;
*/
typedef struct _tagUserStatus
{
	CString				strUserID;
	CString				strUserName;
	CString				strUserIP;
	USHORT				iPort;
	SOCKADDR_IN			sSockAddrIn;
	DoctorStatusEnum	eUserStatus;
	short				nOnlineFlag;	//心跳包判断是否在线依据
	CString             strPatientName;//用户为医生时，保存呼叫/就诊患者
	CString				strOfficeID;
}USERSTATUS, *LPUSERSTATUS;

/*时间挂起信息*/
typedef struct _tagSuspendTime
{
	WCHAR		logid[255];
	CTime       suspendtime;
}SUSPENDTIME,*LPSUSPENDTIME;


#define  statusWaitLine   _T("排队")
#define  statusQuitQue    _T("过号")
#define  statusSuspend    _T("挂起")
#define  statusDone       _T("完成就诊")
#define  statusDoing      _T("呼叫/就诊")
#define  statusPre        _T("准备")


#define  priMinTwoText    _T("最低")
#define  priMinOneText    _T("较低")
#define	 priZeroText      _T("普通")
#define  priOneText       _T("中等")
#define  priTwoText       _T("较高")
#define  priThreeText     _T("最高")

#define  callNumText      _T("完成就诊:")
#define  quitNumText      _T("过号:")
#define  suspendNumText   _T("挂起:")
#define  allNumText       _T("总人数:")
#define  waitNumText      _T("排队:")
#define  flushTimeText    _T("刷新时间:")
#define  loginText        _T("当前登录:")


#define  toolFulshText		_T("刷新")
#define  toolQueText		_T("排队方式")
#define  toolExportText		_T("导出")
#define  toolQuitText		_T("过号")
#define  toolInsertText		_T("重排")
#define  toolSuspendText	_T("挂起")
#define  toolActiveText		_T("激活")
#define  toolReconsultText  _T("复诊")
#define  toolRecallText     _T("召回")
#define  toolSetDocText     _T("指定医生")
#define  toolSearchText     _T("查找")
#define  toolMessageText    _T("消息")
#define  toolShowText       _T("液晶显示")
#define  toolExchangeText   _T("转移科室")


#define  docStaOnlineText   _T("工作")
#define  docStaOutlineText  _T("离线")
#define  docStaSuspendText  _T("暂停")


typedef struct _tagDocToComBoInfo
{
	CString docid;
	CString docname;
}DOCTOCOMBOINFO,*LPDOCTOCOMBOINFO;

/************************************************/


#endif		//_NURSESYSTEMDEF_H