#ifndef _JTTS_ML_H_
#define _JTTS_ML_H_

#pragma pack( push, jtts_ml_h )
#pragma pack( 8 )

#ifdef __cplusplus
extern "C"
{
#endif

#define TTSAPI __stdcall

//ERRCODE
typedef enum 
{
	ERR_NONE,			//	0
	ERR_ALREADYINIT,	//	1
	ERR_NOTINIT,		//	2 
	ERR_MEMORY,			//	3
	ERR_INVALIDHWND,	//	4
	ERR_INVALIDFUNC,	//	5
	ERR_OPENLIB,		//	6
	ERR_READLIB,		//	7
	ERR_PLAYING,		//	8
	ERR_DONOTHING,		//	9
	ERR_INVALIDTEXT,	//	10
	ERR_CREATEFILE,		//	11
	ERR_WRITEFILE,		//	12
	ERR_FORMAT,			//	13
	ERR_INVALIDSESSION,	//	14
	ERR_TOOMANYSESSION,	//	15
	ERR_MORETEXT,		//	16
	ERR_CONFIG,			//	17
	ERR_OPENDEVICE,		//	18
	ERR_RESETDEVICE,	//	19
	ERR_PAUSEDEVICE,	//	20
	ERR_RESTARTDEVICE,	//	21
	ERR_STARTTHREAD,	//	22
	ERR_BEGINOLE,		//	23
	ERR_NOTSUPPORT,		//	24
	ERR_SECURITY,		//	25
	ERR_CONVERT,		//	26
	ERR_PARAM,			//	27
	ERR_INPROGRESS,		//  28
	ERR_INITSOCK,		//	29
	ERR_CREATESOCK,		//	30
	ERR_CONNECTSOCK,	//	31
	ERR_TOOMANYCON,		//	32
	ERR_CONREFUSED,		//	33
	ERR_SEND,			//	34
	ERR_RECEIVE,		//	35
	ERR_SERVERSHUTDOWN,	//	36
	ERR_OUTOFTIME,		//	37
	ERR_CONFIGTTS,		//	38
	ERR_SYNTHTEXT,		//	39
	ERR_CONFIGVERSION,	//	40
	ERR_EXPIRED,		//	41
	ERR_NEEDRESTART,	//	42
	ERR_CODEPAGE,		//	43
	ERR_ENGINE,			//	44
	ERR_CREATEEVENT,	//	45
	ERR_PLAYMODE,		//	46
	ERR_OPENFILE,		//	47
	ERR_USERABORT,		//	48
	ERR_LICENSEFULL,	//	49
	ERR_LICENSEFILEINVALID,	//	50
	ERR_LICENSE,            //	51 
	ERR_LICENSEEXPIRED,		//	52

	ERR_ENOUGHSPACE,       //53 ------add by binfeng 2006-12-28
}ERRCODE;

//---------------------------------------------------------------------------
// 系统的设置选项

//支持多语种
//
//这里列出的是系统内建的语言定义，需要安装相应音库才能真正支持, 
//但目前并非所有语言都有相应的音库
//
//对于这里没有列出的语言，将来也可能会发布相应的音库，同时会分配一个数值，
//只要安装此音库后，就可以使用。对于没有列出的语言，如果想使用，可以直接使用数值
//
//可以通过Lang系列函数得到所有系统中定义的（包括将来扩展的）语言数值及其描述的信息
//
//对于系统中真正支持的语言，可以通过jTTS_GetVoiceCount, jTTS_GetVoiceAttribute函数
//得到所有安装的音库，并从其属性中知道其语言
//
#define LANGUAGE_MANDARIN				0	// 汉语普通话
#define LANGUAGE_CANTONESE				1	// 广东话
#define LANGUAGE_CHINESE				LANGUAGE_MANDARIN

#define LANGUAGE_US_ENGLISH				10	// 美国英语
#define LANGUAGE_BRITISH_ENGLISH		11	// 英国英语
#define LANGUAGE_ENGLISH				LANGUAGE_US_ENGLISH

#define LANGUAGE_FRENCH					20	// 法语
#define LANGUAGE_CANADIAN_FRENCH		21	// 加拿大法语

#define LANGUAGE_SPANISH				30	// 西班牙语
#define LANGUAGE_LATINAMERICAN_SPANISH	31	// 拉丁美洲西班牙语

#define LANGUAGE_PORTUGUESE				40	// 葡萄牙语
#define LANGUAGE_BRAZILIAN_PORTUGUESE	41	// 巴西葡萄牙语

#define LANGUAGE_DUTCH					50	// 荷兰语
#define LANGUAGE_BELGIAN_DUTCH			51	// 比利时荷兰语

#define LANGUAGE_GERMAN					60	// 德语
#define LANGUAGE_ITALIAN				70	// 意大利语
#define LANGUAGE_SWEDISH				80	// 瑞典语
#define LANGUAGE_NORWEGIAN				90	// 挪威语
#define LANGUAGE_DANISH					100	// 丹麦语
#define LANGUAGE_POLISH					110 // 波兰语
#define LANGUAGE_GREEK					120	// 希腊语
#define LANGUAGE_HUNGARIAN				130 // 匈牙利语
#define LANGUAGE_CZECH					140 // 捷克语
#define LANGUAGE_TURKISH				150 // 土耳其语

#define LANGUAGE_RUSSIAN				500	// 俄语

#define LANGUAGE_ARABIC					600	// 阿拉伯语

#define LANGUAGE_JAPANESE				700	// 日语
#define LANGUAGE_KOREAN					710 // 韩语

#define LANGUAGE_VIETNAMESE				720 // 越南语
#define LANGUAGE_MALAY					730	// 马来语
#define LANGUAGE_THAI					740 // 泰语

//支持多领域
// 
//这里列出的是系统内建的领域定义，需要安装相应音库的资源包才能真正支持。
//
//对于这里没有列出的领域，将来也可能会发布相应的资源包，同时会分配一个数值，
//只要安装此资源包后，就可以使用。对于没有列出的领域，如果想使用，可以直接使用数值
//
//可以通过Domain系列函数得到所有系统中定义的（包括将来扩展的）领域数值及其描述的信息
//
//对于系统中真正支持的领域，可以通过jTTS_GetDomainCount, jTTS_GetDomainAttribute函数
//得到所有安装的领域
#define DOMAIN_COMMON			0		// 通用领域，新闻
#define DOMAIN_FINANCE			1		// 金融证券
#define DOMAIN_WEATHER			2		// 天气预报
#define DOMAIN_SPORTS			3		// 体育赛事
#define DOMAIN_TRAFFIC			4		// 公交信息
#define DOMAIN_TRAVEL			5		// 旅游餐饮
#define DOMAIN_CARBORNE			6		//汽运
#define DOMAIN_QUEUE			7		//排队
#define DOMAIN_SONG				8		//点歌
#define DOMAIN_INSURANCE		9		//保险
#define DOMAIN_VOYAGE			10		//航空
#define DOMAIN_REVENUE			11		//税务
#define DOMAIN_ELECPOWER		12		//电力
#define DOMAIN_MESSAGE			13		//短信


#define DOMAIN_CUSTOM			30		// 一般定制领域

#define DOMAIN_MIN				0
#define DOMAIN_MAX				31

//支持的CODEPAGE
#define CODEPAGE_GB		        936		// 包括GB18030, GBK, GB2312
#define CODEPAGE_BIG5		    950
#define CODEPAGE_SHIFTJIS	    932
#define CODEPAGE_KCS			949
#define CODEPAGE_ISO8859_1		1252
#define CODEPAGE_UNICODE		1200
#define CODEPAGE_UNICODE_BIGE	1201	// BIG Endian
#define CODEPAGE_UTF8 			65001

//支持TAG
#define TAG_AUTO				0x00	// 自动判断
#define TAG_JTTS				0x01	// 仅处理含有jTTS 3.0支持的TAG: \read=\  
#define TAG_SSML				0x02	// 仅处理含有SSML 的TAG: <voice gender="female" />
#define TAG_NONE				0x03	// 没有TAG

//DigitMode
#define DIGIT_AUTO_NUMBER		0
#define DIGIT_TELEGRAM			1
#define DIGIT_NUMBER			2
#define DIGIT_AUTO_TELEGRAM		3

#define DIGIT_AUTO				DIGIT_AUTO_NUMBER

// PuncMode
#define PUNC_OFF				0	/* 不读符号，自动判断回车换行是否分隔符*/
#define PUNC_ON					1	/* 读符号，  自动判断回车换行是否分隔符*/
#define PUNC_OFF_RTN			2	/* 不读符号，强制将回车换行作为分隔符*/
#define PUNC_ON_RTN				3	/* 读符号，  强制将回车换行作为分隔符*/

// EngMode
#define ENG_AUTO				0	/* 自动方式 */
#define ENG_SAPI				1	/* 强制采用英文读法 */
#define ENG_LETTER				2	/* 强制单字母方式 */
#define ENG_LETTER_PHRASE		3	/* 强制采用字母＋自录音词汇的方式 */

//Gender
#define GENDER_FEMALE			0
#define GENDER_MALE				1
#define GENDER_NEUTRAL			2

//AGE
#define AGE_BABY				0		//0 - 3
#define AGE_CHILD				1		//3 - 12
#define AGE_YOUNG				2		//12 - 18
#define AGE_ADULT				3		//18 - 60
#define AGE_OLD					4		//60 -

//PITCH
#define PITCH_MIN				0
#define PITCH_MAX				9

//VOLUME
#define VOLUME_MIN				0
#define VOLUME_MAX				9

//SPEED
#define SPEED_MIN				0
#define SPEED_MAX				9

//AUDIOVOLUME
#define AUDIOVOLUME_MIN			0
#define AUDIOVOLUME_MAX			100

//BACKAUDIOFLAG
#define BACKAUDIOFLAG_REPEAT	0x01
#define BACKAUDIOFLAG_DEFAULT	0x00

//BACKAUDIO
#define BACKAUDIO_NONE			0		// 没有背景音乐
#define BACKAUDIO_MIN			1		// 最小的背景音乐序号
#define BACKAUDIO_MAX			10000	// 最大的背景音乐序号

//VOICESYTLE
#define VOICESTYLE_CADENCE		0		// 抑扬顿挫
#define VOICESTYLE_FLAT			1		// 平稳庄重

//VOICEBUFSIZE
#define VOICEBUFSIZE_DEFAULT	0		// Default: 128K
#define VOICEBUFSIZE_MIN		16		// 16K
#define VOICEBUFSIZE_MAX		8192	// 8M

//INSERTINFOSIZE
#define INSERTINFOSIZE_DEFAULT	0		// Default: 100
#define INSERTINFOSIZE_MIN		1
#define INSERTINFOSIZE_MAX		1000

//---------------------------------------------------------------------------

//jTTS_Play状态	
#define STATUS_NOTINIT	0
#define STATUS_READING	1
#define STATUS_PAUSE	2
#define STATUS_IDLE		3

//jTTS_PlayToFile的文件格式
#define FORMAT_WAV			0	// PCM Native (和音库一致，目前为16KHz, 16Bit)
#define FORMAT_VOX_6K		1	// OKI ADPCM, 6KHz, 4bit (Dialogic Vox)
#define FORMAT_VOX_8K		2	// OKI ADPCM, 8KHz, 4bit (Dialogic Vox)
#define FORMAT_ALAW_8K		3	// A律, 8KHz, 8Bit
#define FORMAT_uLAW_8K		4	// u律, 8KHz, 8Bit
#define FORMAT_WAV_8K8B		5	// PCM, 8KHz, 8Bit
#define FORMAT_WAV_8K16B	6	// PCM, 8KHz, 16Bit
#define FORMAT_WAV_16K8B	7	// PCM, 16KHz, 8Bit
#define FORMAT_WAV_16K16B	8	// PCM, 16KHz, 16Bit
#define FORMAT_WAV_11K8B	9	// PCM, 11.025KHz, 8Bit
#define FORMAT_WAV_11K16B	10	// PCM, 11.025KHz, 16Bit

#define FORMAT_FIRST		0
#define FORMAT_LAST			10

// jTTS_Play / jTTS_PlayToFile / jTTS_SessionStart 函数支持的dwFlag定义

// 此项仅对jTTS_PlayToFile适用
#define PLAYTOFILE_DEFAULT		0x0000	//默认值,写文件时只增加FORMAT_WAV_...格式的文件头
#define PLAYTOFILE_NOHEAD		0x0001	//所有的格式都不增加文件头
#define PLAYTOFILE_ADDHEAD		0x0002	//增加FORMAT_WAV_...格式和FORMAT_ALAW_8K,FORMAT_uLAW_8K格式的文件头

#define PLAYTOFILE_MASK			0x000F

// 此项仅对jTTS_Play适用
#define PLAY_RETURN				0x0000	// 如果正在播放，返回错误
#define PLAY_INTERRUPT			0x0010	// 如果正在播放，打断原来的播放，立即播放新的内容

#define PLAY_MASK				0x00F0

// 播放的内容
#define PLAYCONTENT_TEXT		0x0000	// 播放内容为文本
#define PLAYCONTENT_TEXTFILE	0x0100	// 播放内容为文本文件
#define PLAYCONTENT_AUTOFILE	0x0200	// 播放内容为文件，根据后缀名采用外界Filter DLL抽取
										// 无法判断的当作文本文件

#define PLAYCONTENT_MASK		0x0F00

// 播放的模式，同时用于SessionStart
#define PLAYMODE_DEFAULT		0x0000	// 在jTTS_Play下缺省异步，在jTTS_PlayToFile下缺省同步
										// jTTS_SessionStart下为主动获取数据方式
#define PLAYMODE_ASYNC			0x1000	// 异步播放，函数立即退出
#define PLAYMODE_SYNC			0x2000	// 同步播放，播放完成后退出

#define PLAYMODE_MASK			0xF000

// jTTS_FindVoice返回的匹配级别
#define MATCH_LANGUAGE			0	// 满足LANGUAGE，
#define MATCH_GENDER			1	// 满足LANGUAGE, GENDER
#define MATCH_AGE				2	// 满足LANGUAGE, GENDER, AGE
#define MATCH_NAME				3	// 满足LANGUAGE, GENDER，AGE，NAME
#define MATCH_DOMAIN			4	// 满足LANGUAGE, GENDER，AGE，NAME, DOMAIN，也即满足所有条件
#define MATCH_ALL				4	// 满足所有条件

// InsertInfo信息
#define INFO_MARK				0
#define INFO_VISEME				1

//各种信息串的长度
#define VOICENAME_LEN			32
#define VOICEID_LEN				40
#define VENDOR_LEN				32
#define DLLNAME_LEN				256

#define ATTRNAME_LEN			32
#define XMLLANG_LEN				256

//JTTS_PARAM
typedef enum 
{
	PARAM_CODEPAGE,			// CODEPAGE_xxx
	PARAM_VOICEID,			// Voice ID
	PARAM_PITCH,			// PITCH_MIN - PITCH_MAX
	PARAM_VOLUME,		    // VOLUME_MIN - VOLUME_MAX
	PARAM_SPEED,			// SPEED_MIN - SPEED_MAX
	PARAM_PUNCMODE,			// PUNC_xxx
	PARAM_DIGITMODE,		// DIGIT_xxx
	PARAM_ENGMODE,			// ENG_xxx
	PARAM_TAGMODE,			// TAG_xxx
	PARAM_DOMAIN,		    // DOMAIN_xxx
	PARAM_TRYTIMES,         // 0-100
	PARAM_IDLETIME,         // 0-60 (s), default 10, 0 means 无限等待
	PARAM_LOADBALANCE,		// TRUE, FALSE
	PARAM_VOICESTYLE,		// VOICESTYLE_CADENCE, VOICESTYLE_FLAT
	PARAM_BACKAUDIO,		// BACKAUDIO_NONE, BACKAUDIO_MIN - BACKAUDIO_MAX
	PARAM_BACKAUDIOVOLUME,	// AUDIOVOLUME_MIN - AUDIOVOLUME_MAX
	PARAM_BACKAUDIOFLAG,	// BACKAUDIOFLAG_xxx
	PARAM_VOICEBUFSIZE,		// VOICEBUFSIZE_DEFAULT, or n (K)
	PARAM_INSERTINFOSIZE,	// INSERTINFOSIZE_DEFAULT, or n
}
JTTS_PARAM;


//JTTS_CONFIG
#define JTTS_VERSION4		0x0004	// version 4.0
typedef struct JTTS_CONFIG_T
{
	WORD	wVersion;				// JTTS_VERSION4
	WORD	nCodePage;
	char	szVoiceID[VOICEID_LEN];	// 使用的音色
	short	nDomain;	
	short	nPitch;
	short	nVolume;
	short	nSpeed;
	short	nPuncMode;
	short	nDigitMode;
	short	nEngMode;
	short	nTagMode;
	short   nTryTimes;			// 重试次数,此成员仅用于远程合成
	short	nIdleTime;		//////////////////////////////////////////	
	BOOL    bLoadBalance;		// 负载平衡,此成员仅用于远程合成
	short	nVoiceStyle;		// 朗读风格
	short	nBackAudio;			// BACKAUDIO_NONE: 无背景音乐，BACKAUDIO_MIN-BACKAUDIO_MAX: 背景音乐序号，在资源管理中心"录音管理器"中定义
	short	nBackAudioVolume;	// 背景音乐的音量
	WORD	wBackAudioFlag;		// 背景音乐的属性
	short	nVoiceBufSize;		// 设置内部缓冲区的大小
	short	nInsertInfoSize;	// 缓冲区中插入信息的个数
	short	nReserved[6];		// 保留
//	long    nIdleTime;/////////////////////////////////////////////////
}
JTTS_CONFIG;

//JTTS_VOICEATTRIBUTE
typedef struct JTTS_VOICEATTRIBUTE_T
{
	char		szName[VOICENAME_LEN];	// 只能为英文名称
	char		szVoiceID[VOICEID_LEN];	// 音色的唯一标识
	short		nGender;				// GENDER_xxx
	short		nAge;					// AGE_xx
	DWORD		dwDomainArray;			// 由低位向高位，分别表示DOMAIN_xxx
	DWORD		nLanguage;				// 支持的语言, LANGUAGE_xxx
	char		szVendor[VENDOR_LEN];	// 提供厂商
	char		szDLLName[DLLNAME_LEN];	// 对应的DLL
	DWORD		dwVersionMS;			// 引擎的版本号，对应"3.75.0.31"的前两节
										// e.g. 0x00030075 = "3.75"
    DWORD		dwVersionLS;			// e.g. 0x00000031 = "0.31"
}
JTTS_VOICEATTRIBUTE;

// 插入信息
typedef struct INSERTINFO_T
{
	int nTag;		// 有二种：INFO_MARK, INFO_VISEME
	DWORD dwValue; 	// 具体信息：
					// MARK时，高24位mark文本偏移，低8位文本长度
					// VISEME时，表示唇型
	DWORD dwBytes;	// 在语音流的什么地方插入，必须按顺序增加
}
INSERTINFO;

typedef struct JTTS_LANGATTRIBUTE_T
{
	int	  nValue;
	char  szName[ATTRNAME_LEN];
	char  szEngName[ATTRNAME_LEN];
	char  szXmlLang[XMLLANG_LEN];
}
JTTS_LANGATTRIBUTE;

typedef struct JTTS_DOMAINATTRIBUTE_T
{
	int   nValue;
	char  szName[ATTRNAME_LEN];
	char  szEngName[ATTRNAME_LEN];
}
JTTS_DOMAINATTRIBUTE;

typedef struct JTTS_COUNTERS_T
{	
	int nSession;
}
JTTS_COUNTERS;

// 系统通知消息及回调函数部分

// 系统通知消息
#define WM_JTTS_NOTIFY			(WM_USER + 0x4999)

// 回调函数
typedef BOOL (TTSAPI * JTTS_CALLBACKPROC)(WORD wParam, LONG lParam, DWORD dwUserData);
typedef BOOL (TTSAPI * JTTS_DATACALLBACKPROC)(DWORD dwSessionID, WORD wParam, LONG lParam, 
									   unsigned char * pVoiceData, DWORD dwLen, 
									   INSERTINFO* pInsertInfo, int nInsertInfo, 
									   DWORD dwUserData);

// WM_JTTS_NOTIFY / JTTS_CALLBACKPROC / JTTS_DATACALLBACKPROC中wParam的定义
#define NOTIFY_BEGIN			0	// lParam: 没有使用
#define NOTIFY_END				1	// lParam: 没有使用
#define NOTIFY_SENTEND			2	// lParam: 已经读完的字节数
#define NOTIFY_SENTBEGIN		3	// lParam: 将要读到的位置（以字节数计算）
#define NOTIFY_MARK				4	// lParam: 标记Mark串的位置和长度, 参考宏GETMARKOFFSET和GETMARKLEN
#define NOTIFY_VISEME			5	// lParam: 标记唇型信息，现版本中未使用
#define NOTIFY_CHANGECONFIG		6	// 系统主动使用与传入Config不同的配置
#define NOTIFY_DATA				7	// 仅用于 JTTS_DATACALLBACKPROC，传递合成数据和InsertInfo信息 
									// lParam not use; dwLen是pVoiceData长度; pVoiceData语音数据缓冲; 
									// nInsertInfo, pInsertInfo是插入数据的数目和内容

// 为和老版本兼容而保留
#define NOTIFY_PROGRESS			2	
#define NOTIFY_PROGRESS2		3	

// 通过NOTIFY_MARK的lParam得到具体的Mark String的偏移量和长度
#define GETMARKOFFSET(lParam)	(lParam >> 8)
#define GETMARKLEN(lParam)		(lParam & 0xFF)

//------------------------------------------------------------------------
// 系统函数
ERRCODE TTSAPI jTTS_Init(const char * pcszLibPath, const char * pcszSerialNo);
ERRCODE TTSAPI jTTS_End();

int		TTSAPI jTTS_GetLangCount();
ERRCODE TTSAPI jTTS_GetLangAttribute(int nIndex, JTTS_LANGATTRIBUTE * pAttribute);
ERRCODE TTSAPI jTTS_GetLangAttributeByValue(int nValue, JTTS_LANGATTRIBUTE * pAttribute);

int		TTSAPI jTTS_GetDomainCount();
ERRCODE TTSAPI jTTS_GetDomainAttribute(int nIndex, JTTS_DOMAINATTRIBUTE * pAttribute);
ERRCODE TTSAPI jTTS_GetDomainAttributeByValue(int nValue, JTTS_DOMAINATTRIBUTE * pAttribute);

//-------------------------------------------------------------
// 音库信息函数
int		TTSAPI jTTS_GetVoiceCount();
ERRCODE TTSAPI jTTS_GetVoiceAttribute(int nIndex, JTTS_VOICEATTRIBUTE * pAttribute);
ERRCODE TTSAPI jTTS_GetVoiceAttributeByID(const char *pszVoiceID, JTTS_VOICEATTRIBUTE * pAttribute);
BOOL	TTSAPI jTTS_IsVoiceSupported(const char *pszVoiceID);
ERRCODE TTSAPI jTTS_FindVoice(int nLanguage, int nGender, int nAge, char * szName, int nDomain, 
					   char * pszVoiceID, WORD * pwMatchFlag);

ERRCODE TTSAPI jTTS_PreLoad (const char *pszVoiceID);

//------------------------------------------------------------------------
// 设置函数 
ERRCODE TTSAPI jTTS_Set(const JTTS_CONFIG * pConfig);
ERRCODE TTSAPI jTTS_Get(JTTS_CONFIG * pConfig);
ERRCODE TTSAPI jTTS_SetParam(JTTS_PARAM nParam, DWORD dwValue);
ERRCODE TTSAPI jTTS_GetParam(JTTS_PARAM nParam, DWORD *pdwValue);

//------------------------------------------------------------------------
// 播放函数 
ERRCODE TTSAPI jTTS_SetPlay(UINT uDeviceID, HWND hwnd, JTTS_CALLBACKPROC lpfnCallback, DWORD dwUserData);
ERRCODE TTSAPI jTTS_Play(const char * pcszText, DWORD dwFlag);
ERRCODE TTSAPI jTTS_Stop();
ERRCODE TTSAPI jTTS_Pause();
ERRCODE TTSAPI jTTS_Resume();
int     TTSAPI jTTS_GetStatus();

//------------------------------------------------------------------------
// 播放到文件函数 
ERRCODE TTSAPI jTTS_PlayToFile(const char *pcszText, const char * pcszFileName, 
							   UINT nFormat, const JTTS_CONFIG * pConfig, 
							   DWORD dwFlag, JTTS_CALLBACKPROC lpfnCallback, 
							   DWORD dwUserData);

//-------------------------------------------------------------
// 合成过程底层函数
ERRCODE TTSAPI jTTS_SessionStart(const char * pcszText, DWORD * pdwSessionID, 
								 UINT nFormat, const JTTS_CONFIG *pConfig, DWORD dwFlag, 
								 int * pnBitsPerSample, int * pnSamplesPerSec);
ERRCODE TTSAPI jTTS_SessionGetData(DWORD dwSessionID, int nBufIndex, unsigned char ** ppVoiceData, 
								   DWORD * pdwLen, int nReserveLen, INSERTINFO** ppInsertInfo,
								   int* pnInsertInfo);
ERRCODE TTSAPI jTTS_SessionStop(DWORD dwSessionID);
ERRCODE TTSAPI jTTS_SessionGetReadBytes(DWORD dwSessionID, DWORD * pdwBytes);
ERRCODE TTSAPI jTTS_SessionExec(DWORD dwSessionID, JTTS_DATACALLBACKPROC lpfnDataCallback, DWORD dwUserData);


#ifdef __cplusplus
}
#endif

#pragma pack( pop, jtts_ml_h )

#endif //_JTTS_ML_H_


