#pragma once

#include "NurseView.h"
// CMainFrame 框架
#include "PoolManage.h"
#include "OfficeQueView.h"
#include "DocInfoView.h"
#include "PaientView.h"
#include "ado2.h"
#include "SearchDataDlg.h"
#include "BaseConfig.h"
#include "ReadCardDlg.h"
//#include "DoctorCallerDlg.h" 
#include <afxmt.h>
#include <afxpriv.h>
//#include "SocketUDPClient.h"
//#include "SocketTCPClient.h"
//#include "SocketClient.h"
#include "ComputeFuncationTime.h"
#include "LCDShow.h"
#include "VisonShowLED.h"

#define ID_CALLNUM  0xE720
#define ID_QUITNUM  0xE721
#define ID_SUSPENDNUM 0xE722
#define ID_ALLNUM   0xE723
#define ID_WAITNUM  0xE724
#define ID_FLUSHTIME 0xE725
#define ID_LOGIN     0xE726
#define MYFLUSHTIME  201411

#define TIMERID_INITLED	0x0402

class CMainFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CMainFrame)
protected:
	CMainFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CMainFrame();
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
//	virtual BOOL OnInitDialog();
	
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()


protected:
	
	CMenu m_menu;
	
	CStatusBar m_StatBar;
	CRect m_Rect; 
public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	CDocInfoView *m_pDocview;
	CPaientView *m_pPatientview;
	COfficeQueView *m_pOfficeview;
	CNurseView *m_nurse;
	CSplitterWnd m_wndSplit;
	CSplitterWnd m_wndSplit_2;
	//CString m_baseDrive;
	//CString m_baseIP;
	//CString m_baseAccount;
	//CString m_basePassWord;
	CBaseConfig m_BaseConfig;
public:
	
	
	CDocInfoView* GetDocInfoView(){return m_pDocview;}
	CPaientView* GetPatientView(){return m_pPatientview;}
	COfficeQueView* GetOfficeQueView(){return m_pOfficeview;}
	CNurseView* GetNurseView(){return m_nurse;}
	afx_msg void OnFrShowsuspend();
	afx_msg void OnFrShowquit();
	afx_msg void OnFrFlush();
	afx_msg void OnFrShowwaitting();
	afx_msg void OnFrShowcall();

	afx_msg void OnFrCardQue();
	afx_msg void OnFrAutoQue();
//	afx_msg void OnUpdateFrAutoQue(CCmdUI* pCmdUI);

	
	
	CMap<UINT,UINT,UINT,UINT&> m_menuCheckStates;

	//CADODatabase* pbase;
	CString m_strConn;
	//CDoctorCallerDlg* m_pChatDlg;
	
	
	HANDLE m_hSuspendTime;//时间挂起线程
	HANDLE m_hAutoQue;//自动排队线程
//	HANDLE m_hComReadCard;//读卡资源
//	HANDLE m_hReadCardThread;//读磁卡线程
//	HANDLE m_hReadCallerThread;//硬件呼叫器线程
//	HANDLE m_hDoCallerMsgThread;//缓冲处理硬件呼叫器线程
//	HANDLE m_hDoWndScreenMsgThread;//处理屏数据线程
	static DWORD WINAPI SuspendTimeThread(LPVOID pParam);//时间挂起线程
	static DWORD WINAPI WriteDataToMyQueueThread(LPVOID pParam);//自动排队线程
//	static DWORD WINAPI ReadCardThread(LPVOID pParam);//读卡线程
//	static DWORD WINAPI ReadCallerThread(LPVOID pParam);//硬件呼叫器线程
//	static DWORD WINAPI DoWndScreenMsgThread(LPVOID pParm);//处理屏数据线程
//	static DWORD WINAPI DoCallerMsgThread(LPVOID pParam);//缓冲处理硬件呼叫器线程函数

//	HANDLE m_hComWndScreen;//条屏串口(呼叫器)

//	BOOL SendMsgToWndScreen(CString docid,CString msg);
//	BOOL SendMsgToWndScreen(int wndaddress,CString msg);
	
	//BOOL ConnectToBase();
//	long InitReadCard(const int ncom,const int baudRate);//读卡串口
//	long InitWndScreen(const int ncom);//窗口屏串口(呼叫器)
	afx_msg void OnFrReadcard();
	
//	int m_comNum;//刷卡com口
//	int m_WndCom;//窗口屏串口
//	BOOL m_WndComNum;
//	int m_baudRate;//波特率
	CPoolManage m_pool;
//	BOOL m_count;//刷卡串口数量
	afx_msg void OnFrSearch();

//	void DoCardMsg(char str[],DWORD data_length);
	afx_msg void OnFrOutputdoc();
	afx_msg void OnFrOutputpait();
	CString m_strFileFilter;
	int WriteFileFromPat(CString strFilePath);
	int WriteFileFromDoc(CString strFilePath);

//	void AssicToHex(char *param);
//	void StrToHexes(char *pc,int nsize,char *puc);
//	void DoCallerMsg(char* buf,int size,CADODatabase &threadBase);
//	OVERLAPPED m_ReadOverlapped;

	/*处理屏数据*/
	typedef struct _tagWndScreenMsg
	{
		CString		docid;
		CString		strmsg;
	}WNDSCREENMSG,*LPWNDSCREENMSG;

//	CList<WNDSCREENMSG,WNDSCREENMSG&> m_list_wndscreenmsg;

//	CMutex m_wndscreenmutex;//窗口屏原子锁
//	CMutex m_callermutex;

	//硬件呼叫器发出警报
//	BOOL AlarmToSystem(int CallerAddress);
	//硬件呼叫器显示剩余人数
//	BOOL ShowLastPaintCount(int CallerAddress,int WaitCount);
//	BOOL ShowCallerNum(int CallerAddress,const CString& msg);
	void SqlQueryWait(CADORecordset& rset, WCHAR* wszDoctorID);
	void SqlQueryInLine(CADORecordset& rset, WCHAR* wszDoctorID);

	//显示硬件呼叫器地址
//	void ShowCallerAddress(int CallerAddress);

//	CList<CString,CString&> m_callerMsg;

	BOOL WriteSuspendTimeData();
	CMyGetFile m_getfullfile;
	BOOL ReadSuspendTimeData();
public:
	CList<SUSPENDTIME,SUSPENDTIME&> m_list_suspendtime;
	CMutex m_timeMenutex;

public:
	void AddTrayIcon();
	void RemoveTrayIcon();
	BOOL TaskBarAddIcon(HWND hwnd, UINT uID, HICON hicon, CString strTip);
	BOOL TaskBarDeleteIcon(HWND hwnd, UINT uID);
	afx_msg LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSubmenuDisplay();
	afx_msg void OnSubmenuQuit();


	void SetWndStausText();
	void FlushStaDataToMemory(CString allnum,
	CString callnum,
	CString quitnum,
	CString suspendnum,
	CString waitnum);

	typedef struct _tagStatusData
	{
		CString allString;
		CString waitString;
		CString suspendString;
		CString callString;
		CString quitString;   
	}STATUSDATA,*LPSTATUSDATA;

	STATUSDATA m_statusData;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	int m_fushsec;

	CToolBar m_wndToolBar;
	CImageList m_imageList;
	afx_msg void OnToolbarDropDown(NMHDR* pnmtb, LRESULT* plRes);
	afx_msg void OnIconexportDoc();
	afx_msg void OnIconexportPat();
	afx_msg void OnIconqueCard();
	afx_msg void OnIconqueAuto();
	BOOL m_iconCardSelect;
	BOOL m_iconAutoSelect;
	afx_msg void OnIconsusTime();
	afx_msg void OnIconsusNotime();
	afx_msg void OnRecalliconReconsult();
//	afx_msg void OnRecalliconToque();
	afx_msg void OnBnFlush();
	afx_msg void OnBnQue();
	afx_msg void OnBnExport();
	afx_msg void OnBnActive();
	afx_msg void OnBnSuspend();
	afx_msg void OnBnInsert();
	afx_msg void OnBnRecall();
	afx_msg void OnBnQuit();
	afx_msg void OnBnRepeat();
	afx_msg void OnBnSetdoc();
	afx_msg void OnBnSearch();
	afx_msg void OnBnMessage();
	afx_msg void OnBnExchange();
	afx_msg void OnBnShow();
	BOOL m_iconQueCheck;
	BOOL m_iconExportCheck;
	BOOL m_iconSuspendCheck;
	BOOL m_iconRecall;
	void SearchDoNum();


	/***********************排队逻辑********************/
	BOOL WriteDataToMyQueue();
//	CADORecordset* m_pre;
	/***************************************************/
	
	//连接HIS数据库对象
//	CADODatabase* m_phisDataBase;
	/***********************连接HIS数据库******************/
//	BOOL ConnectToHis();
	/******************************************************/
	/*******从his数据库的HisForBjlb_Patient中读数据放入HisForBjlb_Patient****/
//	HANDLE m_hFlushHis;
//	static DWORD WINAPI FlushHisToMyTable(LPVOID pParam);
	/***************************************************************************/

	afx_msg void OnItemAbout();

//	CDoWndScreen m_doWndScreen;
	//发送通屏udp
	//SocketClient *m_pDoThroughScreenUDP;
	//SocketTCPClient *m_pDoThroughScreenTCP;

	typedef struct _tagDocToAdd
	{
		int address;
		int comprehAddress;
		CString through_screen_ip;
		int through_screen_id;
	}DOCTOADD,*LPDOCTOADD;

	CMap<CString,LPCWSTR,DOCTOADD,DOCTOADD&> m_map_doctoadd;
	void SearchDocToadd();

	CMutex m_prePreLock;
	//对接没用office_id只针对某处
	CString m_officeId;
	void SearChFirstOffice();

public:
	afx_msg LRESULT OnRevReRunMsg(WPARAM wParam, LPARAM lParam);

	CComputeFuncationTime m_computeTime;

//	CList<CString,LPCWSTR> m_ReciveMsg;

//	CString m_lastMsg;//呼叫器上次没读完的信息保存
	afx_msg void OnFrUiflush();
	int m_originFlushTime;

	CLCDShow *m_pLCDShowDlg;
	afx_msg void OnFrPrint();
	afx_msg void OnFrCompset();
	afx_msg void OnItemExit();
	afx_msg void OnIconMenuShow();
	afx_msg void OnIconMenuExit();
	afx_msg void OnTakeNumConf();

//	BOOL ReturnToHCaller(CString strDoctorId);
	void InitThroughWnd();//初始化通屏，获取通道宽、高
	afx_msg void OnConfig();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL DestroyWindow();
public:
	CVisonShowLED* m_pVisonShowLED;
	afx_msg LRESULT OnLedNotfiyMessage(WPARAM wParam,LPARAM lParam);

	static UINT InitLEDThreadProc(LPVOID pParam);
};

//


