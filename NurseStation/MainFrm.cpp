// NurseFrameWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "MainFrm.h"
#include <shlwapi.h>
#include "NurseOfficeQueue.h"
#include "NurseDef.h"
#include "MyString.h"
#include "MyCommon.h"
#include "WriteLog.h"
#include "CommonConvert.h"
#include "com/ComInit.h"
#include "com/SLZCWndScreen.h"
#include "PropSheetConfig.h"
#include "SendToHardWare.h"
#include "CallerInterface.h"



//
HHOOK g_hKeyHook = NULL;
LRESULT CALLBACK MyKeyHookProc(int code, WPARAM wParam, LPARAM lParam);

//////////////保存背景内存DC
CDC*         g_pBackDC = new CDC;
/////////////////////////////////////
//////	消息定义	//////////////////////
#define WM_MY_TRAYICON		(WM_USER+1)
#define MY_MESSAGE_RERUN	(WM_USER + 2)
///////////////////////////////////////////

/*硬件呼叫器数据*/
#define HARDWARE_CALLER_START		0x0A//开始
#define HARDWARE_CALLER_WAIT		0x0B//等候
#define HARDWARE_CALLER_CALL		0x0C//呼叫
#define HARDWARE_CALLER_RECALL		0x0D//重呼
#define HARDWARE_CALLER_CANCEL		0x0E//取消
#define HARDWARE_CALLER_FUNCATION	0x0F//功能
#define HARDWARE_CALLER_SHOW		0x02//功能码02
#define HARDWARE_CALLER_SYSTEM		0x03//系统呼叫呼叫器

//extern void MyWriteConsole(CString str);

// CMainFrame
static UINT indicators[] ={
	ID_SEPARATOR,
	//	ID_INDICATOR_CAPS,
	//	ID_INDICATOR_NUM,
	//	ID_INDICATOR_SCRL
	ID_LOGIN,
	ID_ALLNUM,
	ID_WAITNUM,
	ID_CALLNUM,
	ID_SUSPENDNUM,
	ID_QUITNUM,
	ID_FLUSHTIME
};

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	///////////////////////////////////////////////
	m_menu.LoadMenu(IDR_FRAME_MENU);//加载菜单
	SetMenu(&m_menu);

	m_menuCheckStates[ID_FR_SHOWCALL]=MF_UNCHECKED;
	m_menuCheckStates[ID_FR_SHOWQUIT]=MF_UNCHECKED;
	m_menuCheckStates[ID_FR_SHOWSUSPEND]=MF_UNCHECKED;
	m_menuCheckStates[ID_FR_SHOWWAITTING]=MF_CHECKED;

	m_menuCheckStates[ID_FR_CARDQUE]=MF_UNCHECKED;
	m_menuCheckStates[ID_FR_AUTOQUE]=MF_CHECKED;
	///////////////////////////////////////////////
	HMENU hMenu=::GetMenu(this->m_hWnd);
	CheckMenuItem(hMenu,ID_FR_AUTOQUE,MF_CHECKED|MF_BYCOMMAND);
	///////////////////////////////////////////////

	///////////////////////////////////////状态栏初始化
	m_StatBar.Create(this);
	m_StatBar.SetIndicators(indicators,8);
	CString callString=callNumText;
	CString quitString=quitNumText;
	CString suspendString=suspendNumText;
	CString allString=allNumText;
	CString waitString=waitNumText;
	CString flushTime=flushTimeText;
	CString loginString=loginText+theApp.m_loadNurseName;


	m_StatBar.SetPaneText(1,allString);
	m_StatBar.SetPaneText(2,waitString);
	m_StatBar.SetPaneText(3,callString);
	m_StatBar.SetPaneText(4,suspendString);
	m_StatBar.SetPaneText(5,quitString);
	m_StatBar.SetPaneText(7,loginString);
	//	m_StatBar.SetPaneInfo(0,ID_SEPARATOR,SBPS_NORMAL,m_Rect.Width()/7);
	m_StatBar.SetPaneInfo(1,ID_ALLNUM,SBPS_NORMAL,m_Rect.Width()/10);
	m_StatBar.SetPaneInfo(2,ID_WAITNUM,SBPS_NORMAL,m_Rect.Width()/10);
	m_StatBar.SetPaneInfo(3,ID_CALLNUM,SBPS_NORMAL,m_Rect.Width()/10);
	m_StatBar.SetPaneInfo(4,ID_SUSPENDNUM,SBPS_NORMAL,m_Rect.Width()/10);
	m_StatBar.SetPaneInfo(5,ID_QUITNUM,SBPS_NORMAL,m_Rect.Width()/10);
	m_StatBar.SetPaneInfo(6,ID_FLUSHTIME,SBPS_NORMAL,m_Rect.Width()/10);
	m_StatBar.SetPaneInfo(7,ID_LOGIN,SBPS_NORMAL,m_Rect.Width()/10);
	/////////////////////////////////////////////
	


	/////////////////////////////////////////工具栏创建

	m_imageList.Create(32,32,ILC_COLOR32|ILC_MASK,0,0);
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_FLUSH)); //0
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_QUE));//1
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_EXPORT));//2
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_QUIT));//3
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_INSERT));//4
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SUSPEND));//5
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_ACTIVE));//6
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_REPEAT));//7
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_RECALL));//8
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SETDOC));//9
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_EXCHANGE));//10
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SEARCH));//11
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_MESSAGE));//12
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SHOW));//13

	UINT btnIDs[15]; 
	btnIDs[0]=IDI_ICON_FLUSH; 
	btnIDs[1]=IDI_ICON_QUE; 

	btnIDs[2]=IDI_ICON_EXPORT;
	btnIDs[3]=ID_SEPARATOR;
	btnIDs[4]=IDI_ICON_QUIT;
	btnIDs[5]=IDI_ICON_INSERT;
	btnIDs[6]=IDI_ICON_SUSPEND;
	btnIDs[7]=IDI_ICON_ACTIVE;
	btnIDs[8]=IDI_ICON_REPEAT;
	btnIDs[9]=IDI_ICON_RECALL;
	btnIDs[10]=IDI_ICON_SETDOC;
	btnIDs[11]=IDI_ICON_EXCHANGE;
	btnIDs[12]=IDI_ICON_SEARCH;
	btnIDs[13]=IDI_ICON_MESSAGE;
	btnIDs[14]=IDI_ICON_SHOW;

	m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC | CBRS_FLYBY | CBRS_GRIPPER);
	m_wndToolBar.SetButtons(btnIDs,15);
	m_wndToolBar.SetButtonText(0,toolFulshText);
	m_wndToolBar.SetButtonText(1,toolQueText);
	m_wndToolBar.SetButtonText(2,toolExportText);
	m_wndToolBar.SetButtonText(4,toolQuitText);
	m_wndToolBar.SetButtonText(5,toolInsertText);
	m_wndToolBar.SetButtonText(6,toolSuspendText);
	m_wndToolBar.SetButtonText(7,toolActiveText);
	m_wndToolBar.SetButtonText(8,toolReconsultText);
	m_wndToolBar.SetButtonText(9,toolRecallText);
	m_wndToolBar.SetButtonText(10,toolSetDocText);
	m_wndToolBar.SetButtonText(11,toolExchangeText);
	m_wndToolBar.SetButtonText(12,toolSearchText);
	m_wndToolBar.SetButtonText(13,toolMessageText);
	m_wndToolBar.SetButtonText(14,toolShowText);

	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	TBMETRICS tbmer;
	tbmer.cbSize=sizeof(TBMETRICS);
	tbmer.dwMask=TBMF_BUTTONSPACING;
	tbmer.cxButtonSpacing=20;
	m_wndToolBar.GetToolBarCtrl().SetMetrics(&tbmer);

	DWORD dwStyle=m_wndToolBar.GetButtonStyle(2);
	dwStyle|=TBSTYLE_DROPDOWN | TBBS_CHECKBOX;
	m_wndToolBar.SetButtonStyle(2,dwStyle);

	dwStyle=m_wndToolBar.GetButtonStyle(1);
	dwStyle|=TBBS_DROPDOWN | TBSTYLE_AUTOSIZE | TBBS_CHECKBOX;
	m_wndToolBar.SetButtonStyle(1,dwStyle);

	dwStyle=m_wndToolBar.GetButtonStyle(6);
	dwStyle|=TBSTYLE_DROPDOWN | TBBS_CHECKBOX;
	m_wndToolBar.SetButtonStyle(6,dwStyle);
	
	/*
	dwStyle=m_wndToolBar.GetButtonStyle(9);
	dwStyle|=TBSTYLE_DROPDOWN | TBBS_CHECKBOX;
	m_wndToolBar.SetButtonStyle(9,dwStyle);
	*/

	dwStyle=m_wndToolBar.GetButtonStyle(10);
	dwStyle|=TBSTYLE_AUTOSIZE;
	m_wndToolBar.SetButtonStyle(10,dwStyle);

	dwStyle=m_wndToolBar.GetButtonStyle(11);
	dwStyle|=TBSTYLE_AUTOSIZE;
	m_wndToolBar.SetButtonStyle(11,dwStyle);

	dwStyle=m_wndToolBar.GetButtonStyle(14);
	dwStyle|=TBSTYLE_AUTOSIZE;
	m_wndToolBar.SetButtonStyle(14,dwStyle);

	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imageList);
	m_wndToolBar.SetSizes(CSize(50,50),CSize(32,32));   
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);


	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);
	//////////////////////////////////////////////////////	

	//获取系统刷新时间
	//CString time=m_BaseConfig.GetFlushTimeSec();
	CString time = MyCommon::GetProfileString(
		_T("time"),_T("FLUSHTIMESEC"),_T("30"),_T("\\SYS\\sysconfig.ini"));
	m_pool.CStringToint(m_fushsec,time);
	m_pool.CStringToint(m_originFlushTime,time);
	//系统刷新数据时间
	SetTimer(MYFLUSHTIME,1000,NULL);

	//读取上次关闭后挂起的患者
	//ReadSuspendTimeData();
	//m_hSuspendTime=CreateThread(NULL,0,SuspendTimeThread,this,NULL,0);//时间挂起线程
	/////////////////////////////////////////////////
	m_pVisonShowLED = CVisonShowLED::GetInstance(this->m_hWnd);
	////////////////////////////////////////////////实例化呼叫器接口
	
	CCallerInterface::GetInstance();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CWnd::OnSysCommand(nID, lParam);
	}
}

CMainFrame::CMainFrame()
{
	m_nurse=NULL;
	m_pDocview=NULL;
	m_pOfficeview=NULL;
	m_pPatientview=NULL;
//	m_pre=NULL;
	
//	m_count=FALSE;
//	m_WndComNum=FALSE;

// 	m_hReadCardThread=NULL;
// 	m_hReadCallerThread=NULL;
// 	m_hDoWndScreenMsgThread=NULL;
	//m_pChatDlg = NULL;
	
	
	//pbase= new CADODatabase;
//	m_phisDataBase=new CADODatabase;
	m_strFileFilter = _T("Microsoft Excel CSV file(*.csv)|*.csv||");
//	m_hComWndScreen=INVALID_HANDLE_VALUE;
	
	m_iconCardSelect=FALSE;
	m_iconAutoSelect=TRUE;
	m_iconQueCheck=FALSE;
	m_iconExportCheck=FALSE;
	m_iconSuspendCheck=FALSE;
	m_iconRecall=FALSE;
	m_pLCDShowDlg = NULL;

	//m_baseAccount=m_BaseConfig.GetServerAcount();
	//m_basePassWord=m_BaseConfig.GetServerPassword();
	//m_baseIP=m_BaseConfig.GetServerIP();
	//m_baseDrive=m_BaseConfig.GetDataDrive();

	m_strConn.Format(_T("Driver={%s};Server=%s;Database=Nurse;Uid=%s;Pwd=%s;"),
		m_BaseConfig.GetDataDrive(),m_BaseConfig.GetServerIP(),
		m_BaseConfig.GetServerAcount(), m_BaseConfig.GetServerPassword());

	////////////////////////处理串口
	CComInit* pComInit = CComInit::GetInstance();
	pComInit->OpenWndScreen();
	pComInit->OpenCardComm();
}

CMainFrame::~CMainFrame()
{
	if(m_list_suspendtime.GetCount()>0)
	{
		WriteSuspendTimeData();
	}
	//if(pbase)
	//{
	//	//delete pbase;
	//	//pbase=NULL;
	//}
	/*
	if(m_phisDataBase)
	{
		delete m_phisDataBase;
		m_phisDataBase=NULL;
	}
	*/
// 	if(m_pre)
// 	{
// 		delete m_pre;
// 		m_pre=NULL;
// 	}
// 	
	if(m_pLCDShowDlg)
	{
		delete m_pLCDShowDlg;
		m_pLCDShowDlg = NULL;
	}
	//if(m_pDoThroughScreenUDP)
	//{
	//	delete m_pDoThroughScreenUDP;
	//	m_pDoThroughScreenUDP=NULL;
	//}
	//if(m_pDoThroughScreenTCP)
	//{
	//	delete m_pDoThroughScreenTCP;
	//	m_pDoThroughScreenTCP=NULL;
	//}
	if(g_pBackDC)
	{
		DeleteObject(g_pBackDC->GetSafeHdc());
		delete g_pBackDC;
		g_pBackDC = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FR_SHOWSUSPEND, &CMainFrame::OnFrShowsuspend)
	ON_COMMAND(ID_FR_SHOWQUIT, &CMainFrame::OnFrShowquit)
	ON_COMMAND(ID_FR_FLUSH, &CMainFrame::OnFrFlush)
	
	ON_COMMAND(ID_FR_SHOWWAITTING, &CMainFrame::OnFrShowwaitting)
	ON_COMMAND(ID_FR_SHOWCALL, &CMainFrame::OnFrShowcall)
//	ON_UPDATE_COMMAND_UI(ID_FR_SHOWWAITTING, &CMainFrame::OnUpdateFrShowwaitting)
//	ON_UPDATE_COMMAND_UI(ID_FR_SHOWSUSPEND, &CMainFrame::OnUpdateFrShowsuspend)
//	ON_UPDATE_COMMAND_UI(ID_FR_SHOWQUIT, &CMainFrame::OnUpdateFrShowquit)
//	ON_UPDATE_COMMAND_UI(ID_FR_SHOWCALL, &CMainFrame::OnUpdateFrShowcall)
//	ON_COMMAND(ID_FR_SEARCH, &CMainFrame::OnFrSearch)
	ON_COMMAND(ID_FR_CARDQUE,&CMainFrame::OnFrCardQue)
	ON_COMMAND(ID_FR_AUTOQUE,&CMainFrame::OnFrAutoQue)
//	ON_UPDATE_COMMAND_UI(ID_FR_CARDQUE, &CMainFrame::OnUpdateFrCardQue)
//	ON_UPDATE_COMMAND_UI(ID_FR_AUTOQUE, &CMainFrame::OnUpdateFrAutoQue)
	ON_COMMAND(ID_FR_READCARD, &CMainFrame::OnFrReadcard)
	ON_COMMAND(ID_FR_SEARCH, &CMainFrame::OnFrSearch)
	ON_COMMAND(ID_FR_OUTPUTDOC, &CMainFrame::OnFrOutputdoc)
	ON_COMMAND(ID_FR_OUTPUTPAIT, &CMainFrame::OnFrOutputpait)
	ON_MESSAGE(WM_MY_TRAYICON, &CMainFrame::OnTrayIcon)
	ON_COMMAND(ID_SUBMENU_DISPLAY, &CMainFrame::OnSubmenuDisplay)
	ON_COMMAND(ID_SUBMENU_QUIT, &CMainFrame::OnSubmenuQuit)
	ON_NOTIFY(TBN_DROPDOWN,AFX_IDW_TOOLBAR,&CMainFrame::OnToolbarDropDown)
	ON_WM_TIMER()
	ON_COMMAND(ID_ICONEXPORT_DOC, &CMainFrame::OnIconexportDoc)
	ON_COMMAND(ID_ICONEXPORT_PAT, &CMainFrame::OnIconexportPat)
	ON_COMMAND(ID_ICONQUE_CARD, &CMainFrame::OnIconqueCard)
	ON_COMMAND(ID_ICONQUE_AUTO, &CMainFrame::OnIconqueAuto)
	ON_COMMAND(ID_ICONSUS_TIME, &CMainFrame::OnIconsusTime)
	ON_COMMAND(ID_ICONSUS_NOTIME, &CMainFrame::OnIconsusNotime)
//	ON_COMMAND(ID_RECALLICON_RECONSULT, &CMainFrame::OnRecalliconReconsult)
//	ON_COMMAND(ID_RECALLICON_TOQUE, &CMainFrame::OnRecalliconToque)
	ON_COMMAND(IDI_ICON_FLUSH,&CMainFrame::OnBnFlush)
	ON_COMMAND(IDI_ICON_QUE,&CMainFrame::OnBnQue)
	ON_COMMAND(IDI_ICON_EXPORT,&CMainFrame::OnBnExport)
	ON_COMMAND(IDI_ICON_ACTIVE,&CMainFrame::OnBnActive)
	ON_COMMAND(IDI_ICON_SUSPEND,&CMainFrame::OnBnSuspend)
	ON_COMMAND(IDI_ICON_INSERT,&CMainFrame::OnBnInsert)
	ON_COMMAND(IDI_ICON_RECALL,&CMainFrame::OnBnRecall)
	ON_COMMAND(IDI_ICON_QUIT,&CMainFrame::OnBnQuit)
	ON_COMMAND(IDI_ICON_REPEAT,&CMainFrame::OnBnRepeat)
	ON_COMMAND(IDI_ICON_SETDOC,&CMainFrame::OnBnSetdoc)
	ON_COMMAND(IDI_ICON_SEARCH,&CMainFrame::OnBnSearch)
	ON_COMMAND(IDI_ICON_MESSAGE,&CMainFrame::OnBnMessage)
	ON_COMMAND(IDI_ICON_EXCHANGE,&CMainFrame::OnBnExchange)
	ON_COMMAND(IDI_ICON_SHOW,&CMainFrame::OnBnShow)
	ON_COMMAND(ID_ITEM_ABOUT, &CMainFrame::OnItemAbout)
	ON_MESSAGE(MY_MESSAGE_RERUN, &CMainFrame::OnRevReRunMsg)
	ON_COMMAND(ID_FR_UIFLUSH, &CMainFrame::OnFrUiflush)
	ON_COMMAND(ID_FR_PRINT, &CMainFrame::OnFrPrint)
	ON_COMMAND(ID_FR_COMPSET, &CMainFrame::OnFrCompset)
	ON_COMMAND(ID_ITEM_EXIT, &CMainFrame::OnItemExit)
	ON_COMMAND(ID_ICON_MENU_SHOW, &CMainFrame::OnIconMenuShow)
	ON_COMMAND(ID_ICON_MENU_EXIT, &CMainFrame::OnIconMenuExit)
	ON_COMMAND(ID_TAKE_NUM_CONF, &CMainFrame::OnTakeNumConf)
	ON_COMMAND(ID_CONFIG, &CMainFrame::OnConfig)
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CMainFrame 消息处理程序

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	theApp.m_pMainWnd=FromHandle(m_hWnd);
	theApp.m_pNurseWnd=(CMainFrame*)theApp.m_pMainWnd;

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),TRUE);

	AddTrayIcon();

	//ConnectToBase();


	//////// BEGIN modified by songyz  2013-10-23 /////////////
	//m_pChatDlg = new CDoctorCallerDlg;
	////g_pDoctorCallerDlg = m_pChatDlg;
	//((CNurseStationApp*)AfxGetApp())->m_pChatDlg = m_pChatDlg;
	///*******************************小宋注意这儿调试出错********************/
	//m_pChatDlg->CDialog::Create(IDD_CHAT_DIALOG, this);
	//m_pChatDlg->ShowWindow(SW_HIDE);
	//	m_pChatDlg->SocketInit();
	///////// END //////////////////////////////////////

	///////////////////////////////////////////////////
	m_wndSplit.CreateStatic(this,1,3);
	m_wndSplit.SetColumnInfo(1,m_Rect.Width()/5,0);

	m_wndSplit_2.CreateStatic(&m_wndSplit,2,1,WS_CHILD|WS_VISIBLE,
		m_wndSplit.IdFromRowCol(0,1));
	m_wndSplit.CreateView(0,0,RUNTIME_CLASS(CNurseView),
		CSize(m_Rect.Width()/5,m_Rect.Height()),pContext);
	m_wndSplit.CreateView(0,2,RUNTIME_CLASS(CPaientView),
		CSize(3*m_Rect.Width()/5,m_Rect.Height()),pContext);

	m_wndSplit_2.CreateView(0,0,RUNTIME_CLASS(COfficeQueView),
		CSize(m_Rect.Width()/5,m_Rect.Height()/2+80),pContext);
	m_wndSplit_2.CreateView(1,0,RUNTIME_CLASS(CDocInfoView),
		CSize(m_Rect.Width()/5,m_Rect.Height()/2-80),pContext);

	m_pDocview=(CDocInfoView*)m_wndSplit_2.GetPane(1,0);
	m_pOfficeview=(COfficeQueView*)m_wndSplit_2.GetPane(0,0);
	m_pPatientview=(CPaientView*)m_wndSplit.GetPane(0,2);
	m_nurse=(CNurseView*)m_wndSplit.GetPane(0,0);

	////// Modified by songyz 2013.10.30 //////////////
	((CNurseStationApp*)AfxGetApp())->m_pDocInfoView = m_pDocview;
	((CNurseStationApp*)AfxGetApp())->m_pNurseView=m_nurse;
	((CNurseStationApp*)AfxGetApp())->m_pPaientView=m_pPatientview;
	((CNurseStationApp*)AfxGetApp())->m_pOfficeQueView=m_pOfficeview;
	///////////////////////////////////////////////////

// 	CString c_baudRate=m_BaseConfig.GetBaudRate();
// 	CString c_comNum=m_BaseConfig.GetComNum();

//	m_pool.CStringToint(m_baudRate,c_baudRate);
//	m_pool.CStringToint(m_comNum,c_comNum);


// 	CString strWndCom=m_BaseConfig.GetWndComNum();
// 	if(strWndCom!=_T(""))
// 	{
// 		int index;
// 		m_pool.CStringToint(index,strWndCom);
// 		m_WndCom=index;
// 		if(InitWndScreen(m_WndCom)!=1)
// 		{
// 			//MessageBox(_T("窗口屏串口打开失败"),_T("注意"),MB_OK|MB_ICONINFORMATION);
// 		}
// 		else
// 		{
// 			m_hReadCallerThread=CreateThread(NULL,0,ReadCallerThread,this,NULL,0);//读硬件呼叫器线程
// 		}
// 	}

//	m_hDoWndScreenMsgThread=CreateThread(NULL,0,DoWndScreenMsgThread,this,NULL,0);//缓冲处理屏数据线程
//	m_hDoCallerMsgThread=CreateThread(NULL,0,DoCallerMsgThread,this,NULL,0);

	

	::PostMessage(m_pOfficeview->m_hWnd,WM_SIZE,0,0);
	::PostMessage(m_pPatientview->m_hWnd,WM_SIZE,0,0);
	::PostMessage(m_pDocview->m_hWnd,WM_SIZE,0,0);


	

	//////////////////////////////////////////////////////////
	SearchDocToadd();
	/////////////////////////////////////////////////////////
	SearChFirstOffice();
	////////////////////启动自动排队线程

	//g_nDataDays = MyCommon::GetProfileInt(_T("sys"), _T("DATA_DAYS"), 0, _T("\\SYS\\sysconfig.ini"));

	m_hAutoQue=CreateThread(NULL,0,WriteDataToMyQueueThread,this,NULL,0);
	/////////////////////////初始化通屏数据
	//InitThroughWnd();

	SetTimer(TIMERID_INITLED, 1000, NULL);

	//
	//g_hKeyHook = ::SetWindowsHookEx(WH_KEYBOARD, 
	//	MyKeyHookProc, NULL, NULL);
	return CFrameWnd::OnCreateClient(lpcs,pContext);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	GetClientRect(&m_Rect);
}

void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//OnDestroy();
	//DestroyWindow();
	if(MessageBox(_T("确定退出吗？"), _T("警告"), MB_OKCANCEL|MB_ICONINFORMATION) == IDCANCEL)
	{
		return;
	}

	CFrameWnd::OnClose();
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	TerminateThread(m_hSuspendTime,0);
	TerminateThread(m_hAutoQue,0);
	//TerminateThread(m_hReadCardThread,0);
// 	TerminateThread(m_hReadCallerThread,0);
// 	TerminateThread(m_hDoCallerMsgThread,0);
// 	
// 	TerminateThread(m_hDoWndScreenMsgThread,0);
	RemoveTrayIcon();
}

void CMainFrame::OnFrShowsuspend()
{
	// TODO: 在此添加命令处理程序代码

	HMENU hMenu=::GetMenu(this->m_hWnd);
	UINT states=GetMenuState(hMenu,ID_FR_SHOWSUSPEND,MF_BYCOMMAND);
	ASSERT(states != 0xFFFFFFFF);
	//	m_menuCheckStates[ID_FR_SHOWWAITTING]=states;
	if(states & MF_CHECKED)
	{
		m_menuCheckStates[ID_FR_SHOWSUSPEND]=MF_UNCHECKED;
		CheckMenuItem(hMenu,ID_FR_SHOWSUSPEND,MF_UNCHECKED|MF_BYCOMMAND);
	}
	else
	{
		m_menuCheckStates[ID_FR_SHOWSUSPEND]=MF_CHECKED;
		CheckMenuItem(hMenu,ID_FR_SHOWSUSPEND,MF_CHECKED|MF_BYCOMMAND);
	}
	if(m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_CHECKED)
	{
		GetNurseView()->m_check_suspend.SetCheck(1);
	}
	else
	{
		GetNurseView()->m_check_suspend.SetCheck(0);
	}
	OnFrFlush();
}

void CMainFrame::OnFrShowquit()
{
	// TODO: 在此添加命令处理程序代码
	
	HMENU hMenu=::GetMenu(this->m_hWnd);
	UINT states=GetMenuState(hMenu,ID_FR_SHOWQUIT,MF_BYCOMMAND);
	ASSERT(states != 0xFFFFFFFF);
	
	if(states & MF_CHECKED)
	{
		m_menuCheckStates[ID_FR_SHOWQUIT]=MF_UNCHECKED;
		CheckMenuItem(hMenu,ID_FR_SHOWQUIT,MF_UNCHECKED|MF_BYCOMMAND);
	}
	else
	{
		m_menuCheckStates[ID_FR_SHOWQUIT]=MF_CHECKED;
		CheckMenuItem(hMenu,ID_FR_SHOWQUIT,MF_CHECKED|MF_BYCOMMAND);
	}
	if(m_menuCheckStates[ID_FR_SHOWQUIT]==MF_CHECKED)
	{
		GetNurseView()->m_check_quit.SetCheck(1);
	}
	else
	{
		GetNurseView()->m_check_quit.SetCheck(0);
	}
	OnFrFlush();
}

void CMainFrame::OnFrFlush()
{
	if(!m_pPatientview->m_bCanRefrush)
	{
		return;
	}
//	m_wndscreenmutex.Lock();
	CString officeid=GetOfficeQueView()->GetOfficeID();
	GetOfficeQueView()->SearchData(officeid);
	GetDocInfoView()->ShowAllInlineDoctorInfo();
	SearchDoNum();
//	m_wndscreenmutex.Unlock();
	//GetPatientView()->SetFocus();
}

void CMainFrame::OnFrShowwaitting()
{
	// TODO: 在此添加命令处理程序代码

	HMENU hMenu=::GetMenu(this->m_hWnd);
	UINT states=GetMenuState(hMenu,ID_FR_SHOWWAITTING,MF_BYCOMMAND);
	ASSERT(states != 0xFFFFFFFF);
//	m_menuCheckStates[ID_FR_SHOWWAITTING]=states;
	if(states & MF_CHECKED)
	{
		m_menuCheckStates[ID_FR_SHOWWAITTING]=MF_UNCHECKED;
		CheckMenuItem(hMenu,ID_FR_SHOWWAITTING,MF_UNCHECKED|MF_BYCOMMAND);
	}
	else
	{
		m_menuCheckStates[ID_FR_SHOWWAITTING]=MF_CHECKED;
		CheckMenuItem(hMenu,ID_FR_SHOWWAITTING,MF_CHECKED|MF_BYCOMMAND);
	}
	if(m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_CHECKED)
	{
		GetNurseView()->m_check_waitting.SetCheck(1);
	}
	else
	{
		GetNurseView()->m_check_waitting.SetCheck(0);
	}
	OnFrFlush();
}

void CMainFrame::OnFrShowcall()
{
	// TODO: 在此添加命令处理程序代码
	
	HMENU hMenu=::GetMenu(this->m_hWnd);
	UINT states=GetMenuState(hMenu,ID_FR_SHOWCALL,MF_BYCOMMAND);
	ASSERT(states != 0xFFFFFFFF);
	//	m_menuCheckStates[ID_FR_SHOWWAITTING]=states;
	if(states & MF_CHECKED)
	{
		m_menuCheckStates[ID_FR_SHOWCALL]=MF_UNCHECKED;
		CheckMenuItem(hMenu,ID_FR_SHOWCALL,MF_UNCHECKED|MF_BYCOMMAND);
	}
	else
	{
		m_menuCheckStates[ID_FR_SHOWCALL]=MF_CHECKED;
		CheckMenuItem(hMenu,ID_FR_SHOWCALL,MF_CHECKED|MF_BYCOMMAND);
	}
	if(m_menuCheckStates[ID_FR_SHOWCALL]==MF_CHECKED)
	{
		GetNurseView()->m_check_hacecall.SetCheck(1);
	}
	else
	{
		GetNurseView()->m_check_hacecall.SetCheck(0);
	}
	OnFrFlush();
}

/*
void CMainFrame::OnFrSearch()
{
	// TODO: 在此添加命令处理程序代码
	CSearchDataDlg dlg(this);
	dlg.DoModal();
}
*/

DWORD WINAPI CMainFrame::SuspendTimeThread(LPVOID pParam)
{
	CMainFrame* pMainFrame=(CMainFrame*)pParam;

	while(1)
	{
		try
		{
			CTime time=CTime::GetCurrentTime();
			int count=pMainFrame->m_list_suspendtime.GetCount();
			if(count==0)
			{
				Sleep(50);
			}

			POSITION pos;
			SUSPENDTIME suspendtime;
			CString strUpdate;
			pMainFrame->m_timeMenutex.Lock();

			CADODatabase database;
			database.SetConnectionString(pMainFrame->m_strConn);
			database.Open();

			for(int i=0;i<count;i++)
			{
				pos=pMainFrame->m_list_suspendtime.FindIndex(i);
				suspendtime=pMainFrame->m_list_suspendtime.GetAt(pos);
				if(suspendtime.suspendtime<=time)
				{
					CString sql=_T("");
					sql.Format(_T("update queue set status='%d' where log_id='%s'"),
						qsInLine,suspendtime.logid);
					if(database.IsOpen())
					{
						database.Execute(sql);

						if(pMainFrame->m_pPatientview->m_view_paient.m_hWnd)
						{
							pMainFrame->GetPatientView()->ReviseListCtrPatStatus(suspendtime.logid,qsInLine);
							pMainFrame->m_list_suspendtime.RemoveAt(pos);
						}
						break;
					}
				}
			}
			database.Close();
		}
		catch(_com_error& e)
		{
			WriteLog::WriteDbErrLog(_T("CMainFrame::SuspendTimeThread"));
		}
		Sleep(200);
	}

	return 0;
}

void CMainFrame::OnFrAutoQue()
{
	HMENU hMenu=::GetMenu(this->m_hWnd);
	UINT states=GetMenuState(hMenu,ID_FR_AUTOQUE,MF_BYCOMMAND);
	ASSERT(states != 0xFFFFFFFF);

	if(states & MF_CHECKED)//被check
	{
//		m_menuCheckStates[ID_FR_AUTOQUE]=MF_UNCHECKED;
//		CheckMenuItem(hMenu,ID_FR_AUTOQUE,MF_UNCHECKED|MF_BYCOMMAND);
		CheckMenuItem(hMenu,ID_FR_AUTOQUE,MF_CHECKED|MF_BYCOMMAND);
		return;
	}
	else//没有被check
	{
		m_menuCheckStates[ID_FR_AUTOQUE]=MF_CHECKED;//
		m_menuCheckStates[ID_FR_CARDQUE]=MF_UNCHECKED;
		CheckMenuItem(hMenu,ID_FR_CARDQUE,MF_BYCOMMAND|MF_UNCHECKED);
		CheckMenuItem(hMenu,ID_FR_AUTOQUE,MF_CHECKED|MF_BYCOMMAND);
	}
}

DWORD WINAPI CMainFrame::WriteDataToMyQueueThread(LPVOID pParam)
{
	CMainFrame* pMainFrame=(CMainFrame*)pParam;

	while(1)
	{
		//pMainFrame->m_wndscreenmutex.Lock();
		if(pMainFrame->m_menuCheckStates[ID_FR_AUTOQUE]==MF_UNCHECKED)
		{
			//threadDtaBase.Close();
			Sleep(10000);
			continue;
		}
		else
		{
			try
			{
				pMainFrame->WriteDataToMyQueue();
			}
			catch(CException& e)
			{

			}
		}
		//pMainFrame->m_wndscreenmutex.Unlock();
		Sleep(3000);
	}
	return 0;
}

void CMainFrame::OnFrCardQue()
{
	HMENU hMenu=::GetMenu(this->m_hWnd);
	UINT states=GetMenuState(hMenu,ID_FR_CARDQUE,MF_BYCOMMAND);
	
	ASSERT(states != 0xFFFFFFFF);

	if(states & MF_CHECKED)
	{
//		m_menuCheckStates[ID_FR_CARDQUE]=MF_UNCHECKED;
		CheckMenuItem(hMenu,ID_FR_CARDQUE,MF_CHECKED|MF_BYCOMMAND);
		return;
	}
	else
	{
		m_menuCheckStates[ID_FR_CARDQUE]=MF_CHECKED;
		m_menuCheckStates[ID_FR_AUTOQUE]=MF_UNCHECKED;
		CheckMenuItem(hMenu,ID_FR_AUTOQUE,MF_BYCOMMAND|MF_UNCHECKED);
		CheckMenuItem(hMenu,ID_FR_CARDQUE,MF_CHECKED|MF_BYCOMMAND);
//  		if(InitReadCard(m_comNum,m_baudRate)==1)
//  		{
//  			m_hReadCardThread=CreateThread(NULL,0,ReadCardThread,this,NULL,0);
//  		}
	}
}


/*
BOOL CMainFrame::WriteDataToMyQueue()
{
	BOOL bRet = TRUE;
	//对应HIS
	CString log_ID=_T("");//流水号
	CString	deptName=_T("");//患者姓名
	CString	Queue=_T("");//科室科室ID
	CString docName=_T("");//医生姓名
	CString	docID=_T("");//医生ID
	CString regTime=_T("");//挂号时间
	//用于对接的his过来的排队号
	CString queue_id_his=_T("");
	CString gender=_T("");
	int myGender=genderNone;

	//排队
	CString Queue_id=_T("");//系统内部排队号码
	CString call_id=_T("");//id前缀
	int NumStart;//开始排队号码
	int NumEnd;//结束号码
	int AmLimit;//上午限制人数
	int PmLimit;//下午限制人数
	int DayLimit;//整天限制人数
	CString C_NumStart=_T("");
	int queNumber;//实际排队人数

	CString strIdPay, strPayDesc;
	int nInFlag;
	
	CTime CurrentTime=CTime::GetCurrentTime();
	int year=CurrentTime.GetYear();
	int month=CurrentTime.GetMonth();
	int day=CurrentTime.GetDay();
	CTime AmTime(year,month,day,12,0,0);
	CTime PmTime(year,month,day,23,59,59);
	
	CString patientBirth;
	CNurseOfficeQueue office;

	try
	{
		CADODatabase db;
		db.SetConnectionString(m_strConn);
		CString strUpdate;
		CADORecordset* pre=new CADORecordset(&db);
		//排重查找在HisForBjlb_Patient中没有加入Queue科室排队的数据
		CString sql=_T("");
		db.Open();
		sql.Format(_T("select h.serial_id,h.patient_name,h.queue_num,h.id_depart,h.id_doctor,h.time,h.patient_gender,h.id_item,h.item_desc,h.in_flag,h.read_flag,o.name officename,o.call_id from HisForBjlb_Patient h left join Office o on o.office_id=h.id_depart left join Doctor d on h.id_doctor=d.doctor_id where h.time>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) and (h.read_flag is null or h.read_flag=0) and h.serial_id not in (select log_id from Queue) and h.id_depart in(select office_id from Nurse_Office where Nurse_id='%s') order by h.time"),
			theApp.GetController()->GetDataDays(), theApp.GetLoginID());
		CString insertSql=_T("");
		if(!pre->Open(sql,CADORecordset::openQuery))
		{
			bRet = FALSE;
		}
		else
		{
			WriteLog::WriteLogWithTime(_T("Got data,beginning Writing to DB..."));
			while(!pre->IsEOF())
			{
				try
				{
					WriteLog::WriteLogWithTime(_T("Getting one records..."));
					pre->GetFieldValue(_T("serial_id"),log_ID);
					CString strSerialId = log_ID;

					//SYSTEMTIME t;
					//::GetLocalTime(&t);
					//log_ID.AppendFormat(_T("_%04d%02d%02d%02d%02d%02d%03d"),
					//	t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);

					pre->GetFieldValue(_T("patient_name"),deptName);
					pre->GetFieldValue(_T("id_depart"),Queue);
					pre->GetFieldValue(_T("id_doctor"),docID);
					pre->GetFieldValue(_T("call_id"),call_id);
					pre->GetFieldValue(_T("time"),regTime);
					pre->GetFieldValue(_T("queue_num"),queue_id_his);
					pre->GetFieldValue(_T("patient_birth"),patientBirth);
					pre->GetFieldValue(_T("patient_gender"),gender);

					pre->GetFieldValue(_T("id_item"), strIdPay);
					pre->GetFieldValue(_T("item_desc"), strPayDesc);

					pre->GetFieldValue(_T("in_flag"), nInFlag);

					if(pre->IsFieldNull(_T("patient_gender")))
					{
						myGender = genderNone;
					}
					else
					{
						if(gender==_T("1"))
						{
							myGender=genderMale;
						}
						if(gender==_T("0"))
						{
							myGender=genderFemale;
						}
					}

					POSITION pos=this->GetPatientView()->m_list_office.GetHeadPosition();
					while(pos!=NULL)
					{
						POSITION posLast=pos;
						office=this->GetPatientView()->m_list_office.GetNext(pos);
						if(office.GetQueID()==Queue)
						{	
							NumStart=office.GetQuenumStart();
							//NumStart = nTodayCount;
							NumEnd=office.GetQuenumEnd();
							if(NumEnd <= 0)
							{
								NumEnd = 100000;
							}
							AmLimit=office.GetAmLimitPatient();
							PmLimit=office.GetPmLimitPatient();
							DayLimit=office.GetDayLimitPatient();
							queNumber=office.GetQueNumber();//实际排队人数
							if(CurrentTime<=AmTime)
							{
								BOOL judge=FALSE;
								if(AmLimit<=0)
								{
									judge=NumStart<=NumEnd && queNumber<DayLimit && 
										NumEnd>=NumStart;
								}
								else
								{
									judge=NumStart<=NumEnd && NumEnd>=NumStart &&
										queNumber<AmLimit;
								}
								if(judge)
								{
									if(NumStart<1000&&NumStart>0)
									{
										C_NumStart.Format(_T("%03d"), NumStart);
									}
									else if(NumStart<10000&&NumStart>=1000)
									{
										C_NumStart.Format(_T("%04d"), NumStart);
									}
									else
									{
										C_NumStart.Format(_T("%d"), NumStart);
									}
									call_id+=C_NumStart;

									if(docID!=_T(""))
									{
										insertSql.Format(_T("insert into Queue(log_id,queue_id,patient_name,office_id,doctor_id,regtime,status,priority,queue_id_call,his_queue_id,patient_birth,patient_gender,id_item,item_desc,in_flag) values('%s',%d,'%s','%s','%s','%s',%d,%d,'%s','%s','%s',%d,'%s','%s',%d)"),
											log_ID,NumStart,deptName,Queue,docID,regTime,qsInLine,priZero,queue_id_his,queue_id_his,patientBirth,myGender,strIdPay,strPayDesc,nInFlag);
									}
									else
									{
										insertSql.Format(_T("insert into Queue(log_id,queue_id,patient_name,office_id,regtime,status,priority,queue_id_call,his_queue_id,patient_birth,patient_gender,id_item,item_desc,in_flag) values('%s',%d,'%s','%s','%s',%d,%d,'%s','%s','%s',%d,'%s','%s',%d)"),
											log_ID,NumStart,deptName,Queue,regTime,qsInLine,priZero,queue_id_his,queue_id_his,patientBirth,myGender,strIdPay,strPayDesc,nInFlag);
									}
									db.Execute(insertSql);
									NumStart++;
									queNumber++;
									office.SetQuenumStart(NumStart);
									office.SetQueNumber(queNumber);

									this->GetPatientView()->m_list_office.SetAt(posLast,office);
									break;
								}
							}
							else if(CurrentTime<=PmTime)
							{
								BOOL judge=FALSE;
								if(PmLimit<=0)
								{
									judge=NumStart<=NumEnd && queNumber<DayLimit && 
										NumEnd>=NumStart;
								}
								else
								{
									judge=NumStart<=NumEnd && NumEnd>=NumStart &&
										queNumber<PmLimit;
								}
								if(judge)
								{
									if(NumStart<1000&&NumStart>0)
									{
										C_NumStart.Format(_T("%03d"), NumStart);
									}
									else if(NumStart<10000&&NumStart>=1000)
									{
										C_NumStart.Format(_T("%04d"), NumStart);
									}
									else
									{
										C_NumStart.Format(_T("%d"), NumStart);
									}
									call_id+=C_NumStart;

									if(docID!=_T(""))
									{
										insertSql.Format(_T("insert into Queue(log_id,queue_id,patient_name,office_id,doctor_id,regtime,status,priority,queue_id_call,his_queue_id,patient_birth,patient_gender,id_item,item_desc,in_flag) values('%s',%d,'%s','%s','%s','%s',%d,%d,'%s','%s','%s',%d,'%s','%s',%d)"),
											log_ID,NumStart,deptName,Queue,docID,regTime,qsInLine,priZero,queue_id_his,queue_id_his,patientBirth,myGender,strIdPay,strPayDesc, nInFlag);
									}
									else
									{
										insertSql.Format(_T("insert into Queue(log_id,queue_id,patient_name,office_id,regtime,status,priority,queue_id_call,his_queue_id,patient_birth,patient_gender,id_item,item_desc,in_flag) values('%s',%d,'%s','%s','%s',%d,%d,'%s','%s','%s',%d,'%s','%s',%d)"),
											log_ID,NumStart,deptName,Queue,regTime,qsInLine,priZero,queue_id_his,queue_id_his,patientBirth,myGender,strIdPay,strPayDesc, nInFlag);
									}
									//WriteLog::Write(insertSql);
									db.Execute(insertSql);

									NumStart++;
									queNumber++;
									office.SetQuenumStart(NumStart);
									office.SetQueNumber(queNumber);
									this->GetPatientView()->m_list_office.SetAt(posLast,office);
									break;
								}
							}
						}
					}
					//strUpdate.AppendFormat(_T("update hisforbjlb_patient set read_flag=1 where serial_id='%s';"),
					//	strSerialId);
					strUpdate.Format(_T("update hisforbjlb_patient set read_flag=1 where serial_id='%s';"),
						strSerialId);
					db.Execute(strUpdate);

				}
				catch (_com_error& e)
				{
					WriteLog::WriteDbErrLog(_T("CMainFrame::WriteDataToMyQueue"));
					pre->MoveNext();
					continue;
				}
				pre->MoveNext();
			}
			pre->Close();
		}

		//pre->Update();
		delete pre;
		pre=NULL;
		db.Close();
		//database.Execute(strUpdate);
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CMainFrame::WriteDataToMyQueue"));
		//Sleep(2000);
		return FALSE;
	}
	return TRUE;
}
*/


BOOL CMainFrame::WriteDataToMyQueue()
{
	BOOL bRet = TRUE;

	try
	{
		CADODatabase db;
		db.SetConnectionString(m_strConn);
		db.Open();

		CADORecordset rset(&db);
		CString sql=_T("");
		//sql.Format(_T("select h.serial_id,h.patient_name,h.queue_num,h.id_depart,h.id_doctor,h.time,h.patient_gender,h.id_item,h.item_desc,h.in_flag,h.read_flag,o.name officename,o.call_id from HisForBjlb_Patient h left join Office o on o.office_id=h.id_depart left join Doctor d on h.id_doctor=d.doctor_id where h.time>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) and (h.read_flag is null or h.read_flag=0) and h.serial_id not in (select log_id from Queue) and h.id_depart in(select office_id from Nurse_Office where Nurse_id='%s') order by h.time"),
		//	theApp.GetController()->GetDataDays(), theApp.GetLoginID());
		sql.Format(_T("select P.serial_id,P.reg_id,P.queue_num,P.patient_id,P.patient_name,P.id_depart,P.id_doctor,P.time,P.patient_gender,P.patient_birth,P.id_item,I.name item_desc,P.in_flag from HisForBjlb_Patient P left join Item I on P.id_item=I.id where P.time>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) and (P.read_flag is null or P.read_flag=0) and P.serial_id not in (select log_id from Queue) and P.id_depart in(select office_id from Nurse_Office where Nurse_id='%s') %s order by P.time"),
			theApp.GetController()->GetDataDays(), theApp.GetLoginID(),  theApp.GetController()->GetReadInFlag() ? _T("") : _T("and (in_flag<>1 or in_flag is null)"));
		try
		{
			rset.Open(sql,CADORecordset::openQuery);
			CString strSql=_T("");
			CString strSerialId, strRegId,strPatientId,strPatientName,strIdDepart,strIdDoctor,
				strTime,strQueueNum,strPatientBirth,strIdItem,strItemDesc;
			int nPatientGender = genderNone,nInflag = 0;

			//WriteLog::WriteLogWithTime(_T("Recordset Opened,beginning Writing to DB..."));
			while(!rset.IsEOF())
			{
				try
				{
					//WriteLog::WriteLogWithTime(_T("Getting one records..."));
					rset.GetFieldValue(_T("serial_id"),strSerialId);
					rset.GetFieldValue(_T("reg_id"),strRegId);
					rset.GetFieldValue(_T("queue_num"),strQueueNum);
					rset.GetFieldValue(_T("patient_id"),strPatientId);
					rset.GetFieldValue(_T("patient_name"),strPatientName);
					if(rset.IsFieldNull(_T("patient_gender")))
					{
						nPatientGender = genderNone;
					}
					else
					{
						rset.GetFieldValue(_T("patient_gender"),nPatientGender);
					}
					rset.GetFieldValue(_T("patient_birth"),strPatientBirth);
					rset.GetFieldValue(_T("id_depart"),strIdDepart);
					rset.GetFieldValue(_T("id_doctor"),strIdDoctor);
					rset.GetFieldValue(_T("time"),strTime);
					rset.GetFieldValue(_T("id_item"), strIdItem);
					rset.GetFieldValue(_T("item_desc"), strItemDesc);

					if(rset.IsFieldNull(_T("in_flag")))
					{
						nInflag = 0;
					}
					else
					{
						rset.GetFieldValue(_T("in_flag"), nInflag);
					}
				}
				catch (_com_error& e)
				{
					WriteLog::WriteDbErrLog(_T("CMainFrame::WriteDataToMyQueue: GetFieldValue"));
				}

				strSql.Format(_T("insert into Queue(log_id,reg_id,queue_id_call,patient_id,patient_name,office_id,doctor_id,regtime,patient_birth,id_item,item_desc,patient_gender,status,priority,in_flag) values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,%d,%d,%d)"),
					strSerialId,strRegId,strQueueNum,strPatientId,strPatientName,strIdDepart,strIdDoctor,strTime,strPatientBirth,strIdItem,strItemDesc,nPatientGender,qsInLine,priZero,nInflag);
				try
				{
					db.Execute(strSql);
					strSql.Format(_T("update hisforbjlb_patient set read_flag=1 where serial_id='%s';"),
						strSerialId);
					try
					{
						db.Execute(strSql);
					}
					catch (_com_error& e)
					{
						WriteLog::WriteDbErrLog(_T("CMainFrame::WriteDataToMyQueue: Execute update read_flag"));
						//WriteLog::WriteLogWithTime(strSql);
					}
				}
				catch (_com_error& e)
				{
					WriteLog::WriteDbErrLog(_T("CMainFrame::WriteDataToMyQueue: Execute insert"));
					//WriteLog::WriteLogWithTime(strSql);
				}

				rset.MoveNext();
			}
			rset.Close();
		}
		catch (_com_error& e)
		{
			bRet = FALSE;
			WriteLog::WriteDbErrLog(_T("CMainFrame::WriteDataToMyQueue: Open recordset"));
		}

		db.Close();
	}
	catch (_com_error& e)
	{
		bRet = FALSE;
		WriteLog::WriteDbErrLog(_T("CMainFrame::WriteDataToMyQueue: Open db"));
	}
	return bRet;
}


// long CMainFrame::InitReadCard(const int ncom,const int baudRate)
// {
// 	if(ncom<0) return 2;
// 	if(m_count==TRUE)
// 	{
// 		if(m_hComReadCard)
// 		{
// 			CloseHandle(m_hComReadCard);
// 			m_hComReadCard=INVALID_HANDLE_VALUE;
// 		}
// 	}
// 
// 	CString strcom;
// 	strcom.Format(_T("COM%d"),ncom);
// 	HANDLE hCom=CreateFile(strcom,GENERIC_READ | GENERIC_WRITE,0,
// 		NULL,OPEN_EXISTING,0,NULL);//打开串口
// 	if(INVALID_HANDLE_VALUE==hCom)
// 	{
// 		CloseHandle(hCom);
// 		return 4;
// 	}
// 	DCB dcb={0};
// 	dcb.DCBlength=sizeof(dcb);
// 	BOOL bres=GetCommState(hCom,&dcb);
// 	if(!bres)
// 	{
// 		CloseHandle(hCom);
// 		hCom=INVALID_HANDLE_VALUE;
// 		return 9;
// 	}
// 	switch (baudRate)
// 	{
// 	case 0:
// 		dcb.BaudRate=CBR_110;
// 		break;
// 	case 1:
// 		dcb.BaudRate=CBR_300;
// 		break;
// 	case 2:
// 		dcb.BaudRate=CBR_600;
// 		break;
// 	case 3:
// 		dcb.BaudRate=CBR_1200;
// 		break;
// 	case 4:
// 		dcb.BaudRate=CBR_2400;
// 		break;
// 	case 5:
// 		dcb.BaudRate=CBR_4800;
// 		break;
// 	case 6:
// 		dcb.BaudRate=CBR_9600;
// 		break;
// 	case 7:
// 		dcb.BaudRate=CBR_14400;
// 		break;
// 	case 8:
// 		dcb.BaudRate=CBR_19200;
// 		break;
// 	case 9:
// 		dcb.BaudRate=CBR_38400;
// 		break;
// 	case 10:
// 		dcb.BaudRate=CBR_57600;
// 		break;
// 	case 11:
// 		dcb.BaudRate=CBR_115200;
// 		break;
// 	case 12:
// 		dcb.BaudRate=CBR_128000;
// 		break;
// 	case 13:
// 		dcb.BaudRate=CBR_256000;
// 		break;
// 	}
// 	dcb.ByteSize=8;//后改
// 	dcb.fParity=0;
// 
// 	bres=SetCommState(hCom,&dcb);
// 	if(!bres)
// 	{
// 		CloseHandle(hCom);
// 		hCom=INVALID_HANDLE_VALUE;
// 		return 5;
// 	}
// 	COMMTIMEOUTS comm_time={0};
// 	comm_time.ReadIntervalTimeout=MAXDWORD;
// 	bres=SetCommTimeouts(hCom,&comm_time);
// 	if(!bres)
// 	{
// 		CloseHandle(hCom);
// 		return 6;
// 	}
// 	m_hComReadCard=hCom;
// 	m_count=TRUE;
// 	return 1;
// }
void CMainFrame::OnFrReadcard()
{
	// TODO: 在此添加命令处理程序代码
	CReadCardDlg dlg(this);
	if(IDOK==dlg.DoModal())
	{
		CString c_baudRate=m_BaseConfig.GetBaudRate();
		CString c_comNum=m_BaseConfig.GetComNum();

//		m_pool.CStringToint(m_baudRate,c_baudRate);
//		m_pool.CStringToint(m_comNum,c_comNum);
		//刷卡
//  		if(InitReadCard(m_comNum,m_baudRate)==1)
//  		{
//  			if(m_hReadCardThread==NULL && m_menuCheckStates[ID_FR_CARDQUE]==MF_CHECKED)
//  			{
//  				m_hReadCardThread=CreateThread(NULL,0,ReadCardThread,this,NULL,0);
//  			}
//  		}
//  		else
//  		{
//  			if(m_hReadCardThread!=NULL)
//  			{
//  				TerminateThread(m_hReadCardThread,0);
//  				m_hReadCardThread=NULL;
//  			}
//  		}
		//呼叫器
// 		CString c_wndCom=m_BaseConfig.GetWndComNum();
// 		m_pool.CStringToint(m_WndCom,c_wndCom);
// 		if(InitWndScreen(m_WndCom)==1)
// 		{
// 			if(m_hReadCallerThread==NULL)
// 			{
// 				m_hReadCallerThread=CreateThread(NULL,0,ReadCallerThread,this,NULL,0);
// 			}
// 		}
// 		else
// 		{
// 			if(m_hReadCallerThread!=NULL)
// 			{
// 				TerminateThread(m_hReadCallerThread,0);
// 				m_hReadCallerThread=NULL;
// 			}
// 		}
	}
}
/*
DWORD WINAPI CMainFrame::ReadCardThread(LPVOID pParam)
{
	CMainFrame* pMainFrame=(CMainFrame*)pParam;
	//	unsigned long now_len=0;
	while(1)
	{
		char buf[255]={0};
		if(pMainFrame->m_menuCheckStates[ID_FR_CARDQUE]==MF_UNCHECKED)
		{
			return 0;
		}
		//		if(pCard->m_ShutThread)return 0;
		DWORD dwReaded=0;//读到的大小
		BOOL bres=FALSE;
		if(pMainFrame->m_hComReadCard!=INVALID_HANDLE_VALUE)
		{	
			Sleep(200);
			bres=ReadFile(pMainFrame->m_hComReadCard,buf,255,&dwReaded,NULL);
			
			if(strlen(buf)!=0)
			{
				//::SendMessage(pMainFrame->GetNurseView()->m_hWnd,WM_READCARD,(WPARAM)buf,(LPARAM)dwReaded);
				pMainFrame->DoCardMsg(buf,dwReaded);
			}
		}
	}
	return 0;
}
*/
void CMainFrame::OnFrSearch()
{
	// TODO: 在此添加命令处理程序代码
	CSearchDataDlg dlg(this);
	dlg.DoModal();
}
/*
void CMainFrame::DoCardMsg(char str[],DWORD data_length)
{
	DWORD j=0;
	DWORD k=0;
	DWORD i=0;
	CString CS_all_msg=_T("");
	char* strmsg=(char*)str;//刷卡读出信息
	m_pool.CharToCstring(CS_all_msg,strmsg);
	if(CS_all_msg==_T(""))
	{
		return;
	}
	
	char card_allmsg[255]={0};//所有卡信息
	char cardNo[255]={0};//卡号
	char lastMsg[255]={0};//卡号对应信息
	strcpy(card_allmsg,strmsg);//转换
	DWORD length=data_length;//读出信息实际长度
	if(length==0)
	{
		return;
	}
	for(i=1;i<length-1;i++)//分离卡号
	{
		if(card_allmsg[i] != '=')
		{
			cardNo[j]=card_allmsg[i];
			j++;
		}
		else
		{
			break;
		}
	}

	for(i=i+1;i<length-1;i++)//分离信息
	{
		if(card_allmsg[i] != '?')
		{
			lastMsg[k]=card_allmsg[i];
			k++;
		}
		else
		{
			break;
		}
	}
	CString CS_CardNo=_T("");//卡号
	CString CS_lastMsg=_T("");//磁卡对应信息

	m_pool.CharToCstring(CS_CardNo,cardNo);
	m_pool.CharToCstring(CS_lastMsg,lastMsg);

	if(CS_CardNo!=_T("") && CS_lastMsg!=_T(""))
	{
		this->GetNurseView()->m_que_logid.SetWindowText(CS_lastMsg);
		//OnBnClickedBnGetquenum();//排队取号
	}

}
*/
void CMainFrame::OnFrOutputdoc()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(FALSE,_T(".csv"),NULL,OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,m_strFileFilter);
	if(IDOK == dlg.DoModal())
	{
		WriteFileFromDoc(dlg.GetPathName());
	}
}

void CMainFrame::OnFrOutputpait()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(FALSE,_T(".csv"),NULL,OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,m_strFileFilter);
	if(IDOK == dlg.DoModal())
	{
		WriteFileFromPat(dlg.GetPathName());
	}
}

int CMainFrame::WriteFileFromPat(CString strFilePath)
{
	int datacount=GetPatientView()->m_view_paient.GetItemCount();
	int column=GetPatientView()->m_view_paient.GetHeaderCtrl()->GetItemCount();
	CFile file(strFilePath, CFile::modeCreate|CFile::modeWrite);
	CString strLines(_T("行号,排队号,患者姓名,就诊号,挂号时间,医生姓名,当前状态,等侯时间,优先级\r\n"));
	CString context=_T("");
	for(int i=0;i<datacount;i++)
	{
		for(int j=0;j<column;j++)
		{
			context=GetPatientView()->m_view_paient.GetItemText(i,j);
			strLines+=context;
			strLines+=",";
		}
		strLines += "\r\n";
	}
	int nBytes = WideCharToMultiByte(0, 0,strLines, strLines.GetLength(), 
		NULL, 0, NULL, NULL);
	CHAR *buf = (CHAR*)malloc((sizeof(CHAR))*2*(nBytes + 1));
	memset(buf, 0, (sizeof(CHAR))*2*(nBytes + 1));
	WideCharToMultiByte(0, 0, strLines, strLines.GetLength(), 
		buf, nBytes, NULL, NULL);
	file.Write(buf, strlen(buf));
	free(buf);
	file.Close();
	return 0;
}

int CMainFrame::WriteFileFromDoc(CString strFilePath)
{
	int datacount=GetDocInfoView()->m_view_docinfo.GetItemCount();
	int column=GetDocInfoView()->m_view_docinfo.GetHeaderCtrl()->GetItemCount();
	CFile file(strFilePath, CFile::modeCreate|CFile::modeWrite);
	CString strLines(_T("状态,医生姓名,当前患者\r\n"));
	CString context=_T("");
	for(int i=0;i<datacount;i++)
	{
		for(int j=0;j<column;j++)
		{
			context=GetDocInfoView()->m_view_docinfo.GetItemText(i,j);
			strLines+=context;
			strLines+=",";
		}
		strLines += "\r\n";
	}
	int nBytes = WideCharToMultiByte(0, 0,strLines, strLines.GetLength(), 
		NULL, 0, NULL, NULL);
	CHAR *buf = (CHAR*)malloc((sizeof(CHAR))*2*(nBytes + 1));
	memset(buf, 0, (sizeof(CHAR))*2*(nBytes + 1));
	WideCharToMultiByte(0, 0, strLines, strLines.GetLength(), 
		buf, nBytes, NULL, NULL);
	file.Write(buf, strlen(buf));
	free(buf);
	file.Close();
	return 0;
}
/*
BOOL CMainFrame::SendMsgToWndScreen(CString docid,CString msg)
{
	WNDSCREENMSG wndscreenmsg;
	wndscreenmsg.docid=docid;
	wndscreenmsg.strmsg=msg;
	m_prePreLock.Lock();
//	m_list_wndscreenmsg.AddTail(wndscreenmsg);
	m_prePreLock.Unlock();
////////////////////////////////////
//	SendMsgToPre(docid,msg);
	/////////////////////////////////
	return TRUE;
}
*/
/*
long CMainFrame::InitWndScreen(const int ncom)
{
	if(ncom<0) return 2;
	if(m_WndComNum==TRUE)
	{
		if(m_hComWndScreen)
		{
			CloseHandle(m_hComWndScreen);
			m_hComWndScreen=INVALID_HANDLE_VALUE;
		}
	}
	CString strcom;
	strcom.Format(_T("COM%d"),ncom);
	HANDLE hCom=CreateFile(strcom,GENERIC_READ | GENERIC_WRITE,0,
		NULL,OPEN_EXISTING,0,NULL);//打开串口

	if(INVALID_HANDLE_VALUE==hCom)
	{
		CloseHandle(hCom);
		return 4;
	}
	memset(&m_ReadOverlapped,0,sizeof(OVERLAPPED));

	DCB dcb={0};
	dcb.DCBlength=sizeof(dcb);
	BOOL bres=GetCommState(hCom,&dcb);
	if(!bres)
	{
		CloseHandle(hCom);
		hCom=INVALID_HANDLE_VALUE;
		return 9;
	}
	dcb.ByteSize=8;//后改
	dcb.fParity=0;
	dcb.BaudRate=CBR_9600;
	dcb.StopBits=ONESTOPBIT;
	
	bres=SetCommState(hCom,&dcb);
	if(!bres)
	{
		CloseHandle(hCom);
		hCom=INVALID_HANDLE_VALUE;
		return 5;
	}
	COMMTIMEOUTS comm_time={0};
	comm_time.ReadIntervalTimeout=MAXDWORD;
	bres=SetCommTimeouts(hCom,&comm_time);

	m_ReadOverlapped.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);//读事件

	if(!bres)
	{
		CloseHandle(hCom);
		return 6;
	}
	m_hComWndScreen=hCom;
	m_WndComNum=TRUE;
	return 1;

}
*/
/*
DWORD WINAPI CMainFrame::ReadCallerThread(LPVOID pParam)
{
	CMainFrame* pMainFrame=(CMainFrame*)pParam;
	while(1)
	{
		char buf[1024]={0};
		//		if(pCard->m_ShutThread)return 0;
		DWORD dwReaded=0;//读到的大小
		BOOL bres=FALSE;
		
		if(pMainFrame->m_hComWndScreen!=INVALID_HANDLE_VALUE)
		{	
			Sleep(100);
			pMainFrame->m_callermutex.Lock();
			//Sleep(100);
			bres=ReadFile(pMainFrame->m_hComWndScreen,buf,1024,&dwReaded,NULL);
			if(GetLastError()==ERROR_IO_PENDING)
			{
				WaitForSingleObject(pMainFrame->m_ReadOverlapped.hEvent,100);
			}
			if(dwReaded>0)
			{
				WriteFile(pMainFrame->m_hComWndScreen,buf,dwReaded,&dwReaded,NULL);
				DWORD j=0;
				BOOL f=FALSE;
				CString first_str;
				for(j=0;j<dwReaded&&j<1024;j++)
				{
					CString temp;
					temp.Format(_T("%08x"),buf[j]);
					if(temp==_T("ffffffff") && j!=0)
					{
						f=TRUE;
					}
					if(f)
					{
						pMainFrame->m_ReciveMsg.AddTail(first_str);
						first_str=_T("0x");
						first_str+=temp;
						f=FALSE;
					}
					else
					{
						first_str+=_T("0x");
						first_str+=temp;
					}
				}
				if(!first_str.IsEmpty() && 
					first_str.Right(10)==_T("0x00000016")
					&& first_str.Left(10)==_T("0xffffffff")
					)
				{
					pMainFrame->m_ReciveMsg.AddTail(first_str);
				}
				else if(!first_str.IsEmpty())
				{
					pMainFrame->m_lastMsg+=first_str;
					if(!pMainFrame->m_lastMsg.IsEmpty() && 
						pMainFrame->m_lastMsg.Right(10)==_T("0x00000016")
						&& pMainFrame->m_lastMsg.Left(10)==_T("0xffffffff")
						)
					{
						int flag=0;
						int count=pMainFrame->m_lastMsg.GetLength();
						for(int i=0;i<count;i+=10)
						{
							CString mid=pMainFrame->m_lastMsg.Mid(i,10);
							if(mid==_T("0xffffffff"))
							{
								flag++;
							}
#ifdef _DEBUG								
//							MyWriteConsole(_T("midmsg:")+mid);
#endif
						}
						if(flag==1)
						{
							pMainFrame->m_ReciveMsg.AddTail(pMainFrame->m_lastMsg);
						}
						pMainFrame->m_lastMsg=_T("");
					}
				}

#ifdef _DEBUG
				CString allMsg;
				for(j=0;j<dwReaded&&j<1024;j++)
				{
					CString temp;
					temp.Format(_T("%08x"),buf[j]);
					allMsg+=_T("0x");
					allMsg+=temp;
				}
				MyWriteConsole(_T("allmsg")+allMsg);


				POSITION pos=pMainFrame->m_ReciveMsg.GetHeadPosition();
				while(pos)
				{
					CString msg=pMainFrame->m_ReciveMsg.GetNext(pos);
					if(msg!=_T("") && msg.Left(10)==_T("0xffffffff")
						&& msg.Right(10)==_T("0x00000016"))
					{
						int length=msg.GetLength();
						CString str=msg.Mid(70,length-90);
						length=str.GetLength();
						if(str.Right(20)!=_T("0x0000000c0x0000000c"))
							MyWriteConsole(_T("first")+msg);
					}
				}

#endif				
				//过滤
				POSITION posRe=pMainFrame->m_ReciveMsg.GetHeadPosition();
				while(posRe)
				{
					POSITION posDel=posRe;
					CString msg=pMainFrame->m_ReciveMsg.GetNext(posRe);
					if(msg!=_T("") && msg.Left(10)==_T("0xffffffff")
						&& msg.Right(10)==_T("0x00000016"))
					{
						int length=msg.GetLength();
						CString str=msg.Mid(70,length-90);
						length=str.GetLength();
						if(str.Right(20)!=_T("0x0000000c0x0000000c"))
							pMainFrame->m_callerMsg.AddTail(msg);
					}
					pMainFrame->m_ReciveMsg.RemoveAt(posDel);
				}
			}
			pMainFrame->m_callermutex.Unlock();
		}
		else
		{
			Sleep(20);
		}
	}
	return 0;
}
*/

/*
DWORD WINAPI CMainFrame::DoWndScreenMsgThread(LPVOID pParm)
{
	WNDSCREENMSG wndscreenmsg;
	CMainFrame* pMainFrame=(CMainFrame*)pParm;
	while(1)
	{
#ifdef _DEBUG
	pMainFrame->m_computeTime.SetStartTime(clock());
#endif
		CString docid=_T("");
		CString msg=_T("");

		if(pMainFrame->m_list_wndscreenmsg.GetCount()==0)
		{
			Sleep(50);
		}
		else
		{
			POSITION pos=pMainFrame->m_list_wndscreenmsg.GetHeadPosition();
			while(pos!=NULL)
			{
				pMainFrame->m_prePreLock.Lock();
				wndscreenmsg=pMainFrame->m_list_wndscreenmsg.GetNext(pos);
				pMainFrame->m_list_wndscreenmsg.RemoveHead();
				pMainFrame->m_prePreLock.Unlock();
				docid=wndscreenmsg.docid;
				msg=wndscreenmsg.strmsg;
				
				int address=pMainFrame->m_map_doctoadd[docid].address;
				
//				int comprehAddress=0;
//				CString through_screen_ip=_T("");
				int through_screen_id=pMainFrame->m_map_doctoadd[docid].through_screen_id;
				
				CString his_queue_id,patient_name;
				CString prepare=_T("");
				CString queinline=_T("");
				
				msg=pMainFrame->m_doWndScreen.FlushCstringToFitWndScreen(msg);
				char buf[1024]={0};//条屏字符串
				
				int size=pMainFrame->m_doWndScreen.DoWndScreenMsg(address,buf,msg);
//				char comprehBuf[1024]={0};//综合屏
//				pMainFrame->m_doWndScreen.DoWndScreenMsg(comprehAddress,comprehBuf,msg);
				char throbuf_first[1024]={0};//通屏1
				
#ifdef _DEBUG
				MyWriteConsole(msg);
#endif
				pMainFrame->m_doWndScreen.DoWndScreenMsg(through_screen_id,throbuf_first,msg);
//				char throbuf_secd[1024]={0};//通屏2
//				pMainFrame->m_doWndScreen.DoWndScreenMsg(through_screen_id+1,throbuf_secd,prepare);
//				char throbuf_three[1024]={0};//通屏3
//				pMainFrame->m_doWndScreen.DoWndScreenMsg(through_screen_id+2,throbuf_three,queinline);
				//写串口

				if(pMainFrame->m_hComWndScreen!=INVALID_HANDLE_VALUE)
				{

					BOOL bres=FALSE;
					DWORD dwWrite=0;
					//通屏卡信息
					bres=WriteFile(pMainFrame->m_hComWndScreen,throbuf_first,size,&dwWrite,NULL);
					

					//综合屏信息
//					bres=WriteFile(pMainFrame->m_hComWndScreen,comprehBuf,1024,&dwWrite,NULL);
//					dwWrite=0;
					
					
					bres=WriteFile(pMainFrame->m_hComWndScreen,buf,size,&dwWrite,NULL);
//					size-=dwWrite;
//					Sleep(300);
//					bres=WriteFile(pMainFrame->m_hComWndScreen,throbuf_secd,1024,&dwWrite,NULL);
//					Sleep(300);
//					bres=WriteFile(pMainFrame->m_hComWndScreen,throbuf_three,1024,&dwWrite,NULL);
#ifdef _DEBUG
					pMainFrame->m_computeTime.SetFinshTime(clock());
					CString myclock;
					myclock.Format(_T("%f"),pMainFrame->m_computeTime.GetDuration());
					MyWriteConsole(myclock);

					CString c_dwWrite;
					c_dwWrite.Format(_T("%d"),dwWrite);
					MyWriteConsole(c_dwWrite);
#endif
				}



				//发通屏网络协议
				
//				if(pMainFrame->m_pDoThroughScreenUDP)
//				{
//					buf[2]=through_screen_id;
//					UINT uLen = 1024;
//					USHORT port = 1024;
//					UINT nLength=1024;
					//UDP
//					pMainFrame->m_pDoThroughScreenUDP->
//						SendData(throbuf_first,uLen,through_screen_ip.GetBuffer(0),port);
//					pMainFrame->m_pDoThroughScreenUDP->
//						SendData(throbuf_secd,nLength,through_screen_ip.GetBuffer(0),port);
//					pMainFrame->m_pDoThroughScreenUDP->
//						SendData(throbuf_three,nLength,through_screen_ip.GetBuffer(0),port);
					//TCP
//					pMainFrame->m_pDoThroughScreenTCP->
//						SendData(throbuf_first,uLen,through_screen_ip.GetBuffer(0)str.GetBuffer(0),port);

//					pMainFrame->m_pDoThroughScreenTCP->
//						SendData(throbuf_secd,nLength,through_screen_ip.GetBuffer(0)str.GetBuffer(0),port);
					
//					pMainFrame->m_pDoThroughScreenTCP->
//						SendData(throbuf_three,nLength,through_screen_ip.GetBuffer(0)str.GetBuffer(0),port);
//				}
			}
		}
	}
	return 0;
}
*/
/*
BOOL CMainFrame::SendMsgToWndScreen(int wndaddress,CString msg)
{
	if(msg==_T(""))
	{
		return FALSE;
	}
	else
	{
		//写串口
		char buf[1024]={0};
		int size=m_doWndScreen.DoWndScreenMsg(wndaddress,buf,msg);
		if(InitWndScreen(m_WndCom)==1)
		{
			BOOL bres=FALSE;
			DWORD dwWrite=0;
			
			bres=WriteFile(m_hComWndScreen,buf,size,&dwWrite,NULL);
//			size-=dwWrite;
		}
	}
	return TRUE;
}
*/
/*
BOOL CMainFrame::AlarmToSystem(int CallerAddress)
{
	DWORD dwWrited;
	BOOL res;
	char addbuf[10]={0};
	addbuf[0]=0xFF;
	addbuf[1]=0x68;
	addbuf[2]=0x02;
	addbuf[3]=0x02;
	addbuf[4]=0x68;
	addbuf[5]=CallerAddress;//地址
	addbuf[6]=HARDWARE_CALLER_SYSTEM;
	addbuf[7]=addbuf[5]+addbuf[6];
	addbuf[8]=0x16;
	addbuf[9]='\0';
	
	res=WriteFile(m_hComWndScreen,addbuf,10,&dwWrited,NULL);
	
	return res;
}
*/
/*
BOOL CMainFrame::ShowLastPaintCount(int CallerAddress,int WaitCount)
{
	
	char addbuf[256] = {0};

	addbuf[0]=(char)0xFF;
	addbuf[1]=0x68;

	addbuf[4]=0x68;
	addbuf[5]=CallerAddress;//地址
	addbuf[6]=HARDWARE_CALLER_SHOW;
	int i=7;

	int divisor = 1;
	int temp = 0;
	int length=0;
	////////////////////////////////
	while(WaitCount/divisor)
	{
		length++;
		divisor*=10;
	}
	////////////////////////////////
	divisor=10;
	if(length==0)return 0;
	int *num = new int[length];
	int k = length-1;
	memset(num,0,length*sizeof(int));
	do
	{
		int temp1=temp;
		temp = (WaitCount % divisor );
		if(temp>=0)
		{
			if(temp>=10)
			{
				int result=(temp - temp1)/(divisor/10); 
				num[k--] = result;
			}
			else
			{
				num[k--] = temp-temp1;
			}
		}
		divisor*=10;
	}while(temp != WaitCount);
	////////////////////////////////////
	for(int m=0;m<length;m++)
	{
		addbuf[i++] = num[m];
	}
	delete [] num;
	//////////////////////////////////
	for(int j=5;j<i;j++)
	{
		addbuf[i]+=addbuf[j];
	}
	addbuf[i+1]='\0';

	addbuf[2]=i-5;
	addbuf[3]=i-5;
	
	DWORD dwWrited;
	BOOL bres=WriteFile(m_hComWndScreen,addbuf,i+2,&dwWrited,NULL);
	return bres;
}
*/

void CMainFrame::SqlQueryWait(CADORecordset& rset, 
									WCHAR* wszDoctorID)
{
	CString strSqlSel;
	strSqlSel.Format(_T("Select log_id,queue_id_call,patient_name,status,doctor_id from Queue where doctor_id='%s' and status=%d;"),wszDoctorID , qsPrepare);
	rset.Open(strSqlSel,CADORecordset::openQuery);
}

void CMainFrame::SqlQueryInLine(CADORecordset& rset, WCHAR* wszDoctorID)
{
	CString strSql;
	//strSql.Format(_T("Select Q.log_id,Q.queue_id_call,Q.patient_name,status,Q.doctor_id,Q.priority,Q.regtime from Queue Q, (select doctor_id, office_id from Doctor where doctor_id='%s') D where Q.office_id=D.office_id and (Q.doctor_id is null or Q.doctor_id='' or Q.doctor_id=D.doctor_id) and status=%d and convert(varchar(10),getdate(),120)=convert(varchar(10),Q.regtime,120) order by Q.priority, Q.regtime;"),wszDoctorID , qsInLine);
	strSql.Format(_T("Select Q.log_id,Q.queue_id_call,Q.patient_name,status,Q.doctor_id from Queue Q, (select doctor_id, office_id from Doctor where doctor_id='%s') D where Q.office_id=D.office_id and (Q.doctor_id is null or Q.doctor_id='' or Q.doctor_id=D.doctor_id) and status=%d and convert(varchar(10),getdate(),120)=convert(varchar(10),Q.regtime,120) order by Q.priority, Q.regtime;"),wszDoctorID , qsInLine);
	rset.Open(strSql,CADORecordset::openQuery);
}
/*
void CMainFrame::ShowCallerAddress(int CallerAddress)
{
	DWORD dwWrited;
	char addbuf[255]={0};
	addbuf[0]=0xFF;
	addbuf[1]=0x68;

	addbuf[4]=0x68;
	addbuf[5]=CallerAddress;//地址
	addbuf[6]=HARDWARE_CALLER_SHOW;
	int i=7;

	int temp=CallerAddress%100;
	CallerAddress/=100;
	if(CallerAddress!=0)
	{
		addbuf[i]=CallerAddress;
	}

	addbuf[++i]=temp/10;
	addbuf[++i]=temp%10;

	for(int j=5;j<=i;j++)
	{
		addbuf[i+1]+=addbuf[j];
	}
	addbuf[i+2]='\0';

	addbuf[2]=i-4;
	addbuf[3]=i-4;
	BOOL bres=WriteFile(m_hComWndScreen,addbuf,255,&dwWrited,NULL);
}
*/
/*
DWORD WINAPI CMainFrame::DoCallerMsgThread(LPVOID pParam)
{
	CMainFrame* pMainFrame=(CMainFrame*)pParam;

	while(1)
	{
		if(pMainFrame->m_callerMsg.GetCount()==0)
		{
			Sleep(50);
		}
		else
		{

			try
			{
				CADODatabase threadBase;
				threadBase.SetConnectionString(pMainFrame->m_strConn);
				threadBase.Open();

				POSITION pos=pMainFrame->m_callerMsg.GetHeadPosition();
				while(pos!=NULL)
				{
					pMainFrame->m_callermutex.Lock();
					CString msg=pMainFrame->m_callerMsg.GetNext(pos);
					char temp[1024]={0};
					int size=msg.GetLength();
					if(size%10==0)
					{
						for(int i=0;i<size/10;i++)
						{
							CString c_temp=msg.Left(10);
							temp[i]=wcstol(c_temp,NULL,16);
							msg=msg.Mid(10);
						}
					}
					pMainFrame->m_callerMsg.RemoveHead();
					pMainFrame->m_callermutex.Unlock();
					pMainFrame->DoCallerMsg(temp,size/10,threadBase);

					threadBase.Close();
				}
			}
			catch (_com_error& e)
			{
				WriteLog::WriteDbErrLog(_T("CMainFrame::DoCallerMsgThread"));
			}
		}
	}
	return 0;
}
*/
BOOL CMainFrame::WriteSuspendTimeData()
{
	CString strFilePath=m_getfullfile.GetExeFullFilePath();
	strFilePath+=_T("suspendtime.dat");
	CFile file;
	SUSPENDTIME suspendtime;
	if(file.Open(strFilePath,CFile::modeWrite | CFile::modeCreate))
	{
		POSITION pos=m_list_suspendtime.GetHeadPosition();
		while(pos)
		{
			suspendtime=m_list_suspendtime.GetNext(pos);
			file.Write(&suspendtime,sizeof(SUSPENDTIME));
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

BOOL CMainFrame::ReadSuspendTimeData()
{
	CString strFilePath=m_getfullfile.GetExeFullFilePath();
	strFilePath+=_T("suspendtime.dat");
	CFile file;
	if(file.Open(strFilePath,CFile::modeRead))
	{
		ULONGLONG count=file.GetLength()/sizeof(SUSPENDTIME);
		for(ULONGLONG i=0;i<count;i++)
		{
			SUSPENDTIME suspendtime;
			if(file.Read(&suspendtime,sizeof(SUSPENDTIME))>0)
			{
				m_list_suspendtime.AddTail(suspendtime);
			}
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void CMainFrame::AddTrayIcon(void)
{
	HICON hIcon = LoadIcon(AfxGetApp()->m_hInstance, 
		MAKEINTRESOURCE(IDR_MAINFRAME));
	CString strTip =  _T("排队叫号系统护士站");
	TaskBarAddIcon(this->m_hWnd, IDR_MAINFRAME, hIcon, strTip);
}

void CMainFrame::RemoveTrayIcon()
{
	TaskBarDeleteIcon(this->m_hWnd, IDR_MAINFRAME);
}

BOOL CMainFrame::TaskBarAddIcon(HWND hwnd, 
									 UINT uID, 
									 HICON hIcon, 
									 CString strTip)
{
	BOOL res;
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = uID;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
	nid.uCallbackMessage = WM_MY_TRAYICON;
	nid.hIcon = hIcon;
	if (!strTip.IsEmpty())
	{
		lstrcpyn(nid.szTip, strTip.GetBuffer(), sizeof(nid.szTip));
	}
	else
	{
		nid.szTip[0] = '\0';
	}
	res = Shell_NotifyIcon(NIM_ADD, &nid);

	if (hIcon)
	{
		DestroyIcon(hIcon);
	}
	return res;
}

BOOL CMainFrame::TaskBarDeleteIcon(HWND hwnd, UINT uID)
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = uID;
	return Shell_NotifyIcon(NIM_DELETE, &nid);
}

LRESULT CMainFrame::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME)
	{
		return 1;
	}
	switch(lParam)
	{
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		{
			this->ShowWindow(SW_NORMAL);
			//this->ShowWindow(SW_SHOW);
		}
		break;
	case WM_RBUTTONUP:
		{
			SetForegroundWindow();//点击界面其他位置右下角弹出菜单消失
			CPoint pt;
			::GetCursorPos(&pt);
			CMenu menu;
			menu.LoadMenu(IDR_MENU_ICON_RCLICK);
			CMenu* subMenu = menu.GetSubMenu(0);
			subMenu->TrackPopupMenu(TPM_CENTERALIGN, pt.x, pt.y, this);
			subMenu->Detach();
			subMenu->DestroyMenu();
			menu.Detach();
			menu.DestroyMenu();
		}
		break;
	}
	return 0;
}

void CMainFrame::OnSubmenuDisplay()
{
	ShowWindow(SW_MAXIMIZE);
	//ShowWindow(SW_SHOW);
	//UpdateWindow();
	SetForegroundWindow();
}

void CMainFrame::OnSubmenuQuit()
{
	OnClose();
}

void CMainFrame::SetWndStausText()
{
	m_StatBar.SetPaneText(1,m_statusData.allString);
	m_StatBar.SetPaneText(2,m_statusData.waitString);
	m_StatBar.SetPaneText(3,m_statusData.callString);
	m_StatBar.SetPaneText(4,m_statusData.suspendString);
	m_StatBar.SetPaneText(5,m_statusData.quitString);
}

void CMainFrame::FlushStaDataToMemory(CString allnum, CString callnum, 
										  CString quitnum, CString suspendnum, 
										  CString waitnum)
{
	////////////////////////////////
	CString allString=allNumText+allnum;
	CString waitString=waitNumText+waitnum;
	CString suspendString=suspendNumText+suspendnum;
	CString callString=callNumText+callnum;
	CString quitString=quitNumText+quitnum;
	m_statusData.allString=allString;
	m_statusData.callString=callString;
	m_statusData.quitString=quitString;
	m_statusData.suspendString=suspendString;
	m_statusData.waitString=waitString;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case MYFLUSHTIME:
		{
			CString flushTime=_T("");
			m_pool.intToCString(m_fushsec,flushTime);
			CString str(flushTimeText);
			str += flushTime;
			str += _T("秒");

			m_StatBar.SetPaneText(6,str);
			if(m_fushsec>0)
			{
				m_fushsec--;
				CString flushTime=_T("");
				m_pool.intToCString(m_fushsec,flushTime);
				CString str(flushTimeText);
				str += flushTime;
				str += _T("秒");
			
				m_StatBar.SetPaneText(6,str);
				if(!m_fushsec)
				{
					OnFrFlush();
					m_fushsec=m_originFlushTime;
				}
			}
		}
		break;
	case TIMERID_INITLED:
		{
			KillTimer(TIMERID_INITLED);
			AfxBeginThread(InitLEDThreadProc, this);
		}
		break;
	default:
		{

		}
	}
	CFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnToolbarDropDown(NMHDR* pnmtb, LRESULT* plRes)
{	
	CWnd *pWnd;
	UINT nID;
	NMTOOLBAR* pToolBar=(NMTOOLBAR*)pnmtb;
	switch (pToolBar->iItem)
	{
	case IDI_ICON_QUE:
		pWnd = &m_wndToolBar;
		nID = IDR_MENU_ICON_QUE;
		break;
	case IDI_ICON_EXPORT:
		pWnd = &m_wndToolBar;
		nID=IDR_MENU_ICON_EXPORT;
		break;
	case IDI_ICON_SUSPEND:
		pWnd = &m_wndToolBar;
		nID=IDR_MENU_ICON_SUSPEND;
		break;
		/*
	case IDI_ICON_RECALL:
		pWnd=&m_wndToolBar;
		nID=IDR_MENU_ICON_RECALL;
		break;
		*/
	default:
		return;
	}
	CMenu menu;
	menu.LoadMenu(nID);
	CMenu* pPopup = menu.GetSubMenu(0);
	if(pToolBar->iItem==IDI_ICON_QUE)
	{
		if(m_iconAutoSelect)
		{
			CheckMenuItem(menu.m_hMenu,ID_ICONQUE_AUTO,MF_CHECKED|MF_BYCOMMAND);
	
		}
		else
		{
			CheckMenuItem(menu.m_hMenu,ID_ICONQUE_AUTO,MF_UNCHECKED|MF_BYCOMMAND);
		
		}
		if(m_iconCardSelect)
		{
			CheckMenuItem(menu.m_hMenu,ID_ICONQUE_CARD,MF_CHECKED|MF_BYCOMMAND);
			
		}
		else
		{
			CheckMenuItem(menu.m_hMenu,ID_ICONQUE_CARD,MF_UNCHECKED|MF_BYCOMMAND);
			
		}
	}
	ASSERT(pPopup);
	CRect rc;
	pWnd->SendMessage(TB_GETRECT, pToolBar->iItem, (LPARAM)&rc);
	pWnd->ClientToScreen(&rc);
	pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		rc.left, rc.bottom, this, &rc);
}
void CMainFrame::OnIconexportDoc()
{
	// TODO: 在此添加命令处理程序代码
	OnFrOutputdoc();
	int exportState=
		m_wndToolBar.GetToolBarCtrl().GetState(IDI_ICON_EXPORT);
	m_wndToolBar.GetToolBarCtrl().SetState(IDI_ICON_EXPORT,
		exportState &= ~TBSTATE_CHECKED);
	m_iconExportCheck=FALSE;
}

void CMainFrame::OnIconexportPat()
{
	// TODO: 在此添加命令处理程序代码
	OnFrOutputpait();
	int exportState=
		m_wndToolBar.GetToolBarCtrl().GetState(IDI_ICON_EXPORT);
	m_wndToolBar.GetToolBarCtrl().SetState(IDI_ICON_EXPORT,
		exportState &= ~TBSTATE_CHECKED);

	m_iconExportCheck=FALSE;
}

void CMainFrame::OnIconqueCard()
{
	// TODO: 在此添加命令处理程序代码
	OnFrCardQue();
	m_iconCardSelect=TRUE;
	m_iconAutoSelect=FALSE;

	int queState=
		m_wndToolBar.GetToolBarCtrl().GetState(IDI_ICON_QUE);
	m_wndToolBar.GetToolBarCtrl().SetState(IDI_ICON_QUE,
		queState &= ~TBSTATE_CHECKED);
	m_iconQueCheck=FALSE;
}

void CMainFrame::OnIconqueAuto()
{
	// TODO: 在此添加命令处理程序代码
	
	OnFrAutoQue();
	m_iconAutoSelect=TRUE;
	m_iconCardSelect=FALSE;

	int queState=
		m_wndToolBar.GetToolBarCtrl().GetState(IDI_ICON_QUE);
	m_wndToolBar.GetToolBarCtrl().SetState(IDI_ICON_QUE,
		queState &= ~TBSTATE_CHECKED);
	m_iconQueCheck=FALSE;
}

void CMainFrame::OnIconsusTime()
{
	// TODO: 在此添加命令处理程序代码
	/*
	GetPatientView()->OnTimeSuspend();
	int suspendState=
		m_wndToolBar.GetToolBarCtrl().GetState(IDI_ICON_SUSPEND);
	m_wndToolBar.GetToolBarCtrl().SetState(IDI_ICON_SUSPEND,
		suspendState &= ~TBSTATE_CHECKED);
	m_iconSuspendCheck=FALSE;
	*/
}

void CMainFrame::OnIconsusNotime()
{

	GetPatientView()->OnNoTimeSuspend();
	int suspendState=
		m_wndToolBar.GetToolBarCtrl().GetState(IDI_ICON_SUSPEND);
	m_wndToolBar.GetToolBarCtrl().SetState(IDI_ICON_SUSPEND,
		suspendState &= ~TBSTATE_CHECKED);
	m_iconSuspendCheck=FALSE;
}
/*
void CMainFrame::OnRecalliconReconsult()
{
	// TODO: 在此添加命令处理程序代码
//	GetPatientView()->OnRecallConsult();
	int recallState=
		m_wndToolBar.GetToolBarCtrl().GetState(IDI_ICON_RECALL);
	m_wndToolBar.GetToolBarCtrl().SetState(IDI_ICON_RECALL,
		recallState &= ~TBSTATE_CHECKED);
	m_iconRecall=FALSE;
}
*/
/*
void CMainFrame::OnRecalliconToque()
{
	// TODO: 在此添加命令处理程序代码
//	GetPatientView()->OnRecallConsultQueue();
	
}
*/
void CMainFrame::OnBnFlush()
{
	OnFrFlush();
}

void CMainFrame::OnBnQue()
{
	m_iconQueCheck=!m_iconQueCheck;
	if(m_iconQueCheck)
	{
		CWnd* pWnd=&m_wndToolBar;
		CMenu menu;
		if(menu.LoadMenu(IDR_MENU_ICON_QUE)==NULL)
		{
			return;
		}
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup);
		if(m_iconAutoSelect)
		{
			CheckMenuItem(menu.m_hMenu,ID_ICONQUE_AUTO,MF_CHECKED|MF_BYCOMMAND);
		}
		else
		{
			CheckMenuItem(menu.m_hMenu,ID_ICONQUE_AUTO,MF_UNCHECKED|MF_BYCOMMAND);
		}
		if(m_iconCardSelect)
		{
			CheckMenuItem(menu.m_hMenu,ID_ICONQUE_CARD,MF_CHECKED|MF_BYCOMMAND);
		}
		else
		{
			CheckMenuItem(menu.m_hMenu,ID_ICONQUE_CARD,MF_UNCHECKED|MF_BYCOMMAND);
		}
		CRect rc;
		pWnd->SendMessage(TB_GETRECT, IDI_ICON_QUE, (LPARAM)&rc);
		pWnd->ClientToScreen(&rc);
		pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			rc.left, rc.bottom, this, &rc);
	}
}

void CMainFrame::OnBnExport()
{
	m_iconExportCheck=!m_iconExportCheck;
	if(m_iconExportCheck)
	{
		CWnd* pWnd=&m_wndToolBar;
		CMenu menu;
		menu.LoadMenu(IDR_MENU_ICON_EXPORT);
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup);
		CRect rc;
		pWnd->SendMessage(TB_GETRECT, IDI_ICON_EXPORT, (LPARAM)&rc);
		pWnd->ClientToScreen(&rc);
		pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			rc.left, rc.bottom, this, &rc);
	}
}

void CMainFrame::OnBnActive()
{
	GetPatientView()->OnPatientActive();
}

void CMainFrame::OnBnSuspend()
{
	m_iconSuspendCheck=!m_iconSuspendCheck;
	if(m_iconSuspendCheck)
	{
		CWnd* pWnd=&m_wndToolBar;
		CMenu menu;
		menu.LoadMenu(IDR_MENU_ICON_SUSPEND);
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup);
		CRect rc;
		pWnd->SendMessage(TB_GETRECT, IDI_ICON_SUSPEND, (LPARAM)&rc);
		pWnd->ClientToScreen(&rc);
		pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			rc.left, rc.bottom, this, &rc);
	}
}

void CMainFrame::OnBnInsert()
{
	GetPatientView()->OnInsertqueue();
}

void CMainFrame::OnBnRecall()
{
	GetPatientView()->OnRecallconsult();
}

void CMainFrame::OnBnQuit()
{
	GetPatientView()->OnQuitNum();
}

void CMainFrame::OnBnRepeat()
{
	GetPatientView()->OnReConsult();
}

void CMainFrame::OnBnSetdoc()
{
	GetPatientView()->OnSetDoctor();
}

void CMainFrame::OnBnSearch()
{
	OnFrSearch();
}

void CMainFrame::OnBnMessage()
{
	GetNurseView()->OnBnClickedBnChat();
}

void CMainFrame::SearchDoNum()
{
	try
	{

		CADODatabase db;
		db.SetConnectionString(m_strConn);
		db.Open();
		CADORecordset *pre=new CADORecordset(&db);
		CString sql=_T("");
		CString waitNum,allNum,suspendNum,doneNum,quitNum;
		sql.Format(_T("select count(status)number from queue where office_id='%s' and status in('%d','%d','%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime)")
			,m_pOfficeview->GetOfficeID(),qsDoing,qsPrepare,qsInLine, theApp.GetController()->GetDataDays());
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
				pre->GetFieldValue(_T("number"),waitNum);
		}


		sql.Format(_T("select count(status)number from queue where office_id='%s' and status='%d' and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime)")
			,m_pOfficeview->GetOfficeID(),qsSuspend, theApp.GetController()->GetDataDays());
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
				pre->GetFieldValue(_T("number"),suspendNum);
		}

		sql.Format(_T("select count(status)number from queue where office_id='%s' and status='%d' and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime)")
			,m_pOfficeview->GetOfficeID(),qsDiscard, theApp.GetController()->GetDataDays());
		if(pre->Open(sql,CADORecordset::openQuery))
		{	
			if(!pre->IsBOF())
				pre->GetFieldValue(_T("number"),quitNum);
		}


		sql.Format(_T("select count(status)number from queue where office_id='%s' and status='%d' and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime)")
			,m_pOfficeview->GetOfficeID(),qsDone, theApp.GetController()->GetDataDays());
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
				pre->GetFieldValue(_T("number"),doneNum);
		}

		sql.Format(_T("select count(status)number from queue where office_id='%s' and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime)")
			,m_pOfficeview->GetOfficeID(), theApp.GetController()->GetDataDays());
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
				pre->GetFieldValue(_T("number"),allNum);
		}
		pre->Close();
		delete pre;
		pre=NULL;
		db.Close();

		FlushStaDataToMemory(allNum,doneNum,quitNum,suspendNum,waitNum);
		SetWndStausText();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CMainFrame::SearchDoNum"));
	}
}


/*
BOOL CMainFrame::ConnectToHis()
{
	CString hisBaseDrive=m_BaseConfig.GetHisDataDrive();
	CString hisIP=m_BaseConfig.GetHisServerIP();
	CString hisAcount=m_BaseConfig.GetHisAcount();
	CString hisPass=m_BaseConfig.GetHisPass();
	//小宋改连接MySql时用
	CString hisPort=m_BaseConfig.GetHisPort();
	CString hisBaseName=m_BaseConfig.GetHisBaseName();
	CString hisDataBase=m_BaseConfig.GetHisBaseName();
	////////////////////////////////////
	CString conStr;
//	m_phisDataBase->SetConnectionString(_T("Driver={"+hisBaseDrive+"};Server="+hisIP+";Database=HisData;Uid="+hisAcount+";Pwd="+hisPass+";"));
	
	if(hisBaseDrive==_T("SQL Server"))
	{
		//conStr=_T("Driver={"+hisBaseDrive+"};Server="+hisIP+";Database=HisData;Uid="+hisAcount+";Pwd="+hisPass+";");
		m_phisDataBase->SetConnectionString(_T("Driver={"+hisBaseDrive+"};Server="+hisIP+";Database=" + hisDataBase +";Uid="+hisAcount+";Pwd="+hisPass+";"));
	}
	if(hisBaseDrive==_T("Oracle"))
	{
		CString conStr;
		conStr = _T("Provider=MSDAORA.1;hos=");
		conStr += hisIP + _T(";port=");
		conStr += hisPort + _T(";Data Source=");//;User ID=");
		conStr += hisBaseName + _T(";User ID=");
		conStr += hisAcount + _T(";Password=");
		conStr += hisPass + _T(";PersistSecurityInfo=False;");
		m_phisDataBase->SetConnectionString(conStr);
		//"Provider=MSDAORA;host=192.168.1.129;port=1521;Data Source=orcl;User Id=Scott;Password=123456;";
	}
	if(m_phisDataBase->IsOpen())
	{
		m_phisDataBase->Close();
	}
	try
	{
		if(m_phisDataBase->GetConnectionString().IsEmpty())
		{
			return FALSE;
		}
		if(m_phisDataBase->Open())
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch(...)
	{
		return FALSE;
	}
	
}
*/
/*
DWORD WINAPI CMainFrame::FlushHisToMyTable(LPVOID pParam)
{
	CMainFrame* pMainFrame=(CMainFrame*)pParam;
	CADODatabase threadDtaBase;
	threadDtaBase.SetConnectionString(_T("Driver={"+pMainFrame->m_baseDrive+"};Server="+pMainFrame->m_baseIP+";Database=Nurse;Uid="+pMainFrame->m_baseAccount+";Pwd="+pMainFrame->m_basePassWord+";"));
	if(threadDtaBase.IsOpen())
	{
		threadDtaBase.Close();
	}
	if(threadDtaBase.Open())
	{
	while(1)
	{
		if(pMainFrame->m_menuCheckStates[ID_FR_AUTOQUE]==MF_UNCHECKED)
		{
			return 0;
		}
		CString serial_id,patient_name,gender,register_time;
		CString queue_id=_T("");
		int month=0;
		BOOL flag=FALSE;
		CString insertSql=_T("");
		CString updateSql=_T("");
		//可以考虑删除昨天数据

		if(threadDtaBase.IsOpen() && pMainFrame->m_phisDataBase->IsOpen())
		{
			CADORecordset* pHisrecord=new CADORecordset(pMainFrame->m_phisDataBase);
			CADORecordset* pMyrecord=new CADORecordset(&threadDtaBase);
			CADORecordset* pDelYesRecord=new CADORecordset(&threadDtaBase);

			CString Hissql=_T("select 姓名,rtrim(ltrim(性别))性别,月份,输液号,就诊号,登记时间 from emr_outpatient_patient where convert(varchar(10),getdate(),120)=substring(登记时间,1,10)");
			CString Delyessql=_T("select * from hisforbjlb_patient where convert(varchar(10),getdate(),120)<>convert(varchar(10),register_time,120)");
			CString DelQuesql=_T("select * from queue where convert(varchar(10),getdate(),120)<>convert(varchar(10),regtime,120)");
			if(pDelYesRecord->Open(Delyessql,CADORecordset::openQuery))
			{
				if(!pDelYesRecord->IsBOF())
				{
					Delyessql=_T("delete from hisforbjlb_patient where convert(varchar(10),getdate(),120)<>convert(varchar(10),register_time,120)");
					threadDtaBase.Execute(Delyessql);
				}
				pDelYesRecord->Close();
			}
			if(pDelYesRecord->Open(DelQuesql,CADORecordset::openQuery))
			{
				if(!pDelYesRecord->IsBOF())
				{
					DelQuesql=_T("delete from queue where convert(varchar(10),getdate(),120)<>convert(varchar(10),regtime,120)");
					threadDtaBase.Execute(DelQuesql);
				}
				pDelYesRecord->Close();
			}
			CString mysql=_T("");
			if(pHisrecord->Open(Hissql,CADORecordset::openQuery))
			{
				if(!pHisrecord->IsBOF())
				{
					while(!pHisrecord->IsEOF())
					{
						pHisrecord->GetFieldValue(_T("姓名"),patient_name);
						pHisrecord->GetFieldValue(_T("性别"),gender);
						pHisrecord->GetFieldValue(_T("月份"),month);
						pHisrecord->GetFieldValue(_T("输液号"),queue_id);
						pHisrecord->GetFieldValue(_T("就诊号"),serial_id);
						pHisrecord->GetFieldValue(_T("登记时间"),register_time);

						int length=queue_id.GetLength();
						CString temp=queue_id.Right(1);
						if(temp==_T("."))
							queue_id=queue_id.Left(length-1);
						mysql.Format(_T("select serial_id from hisforbjlb_patient where queue_id='%s'"),queue_id);
						if(pMyrecord->Open(mysql,CADORecordset::openQuery))
						{
							if(pMyrecord->IsBOF())
							{
								insertSql.Format(_T("insert into HisForBjlb_Patient(serial_id,queue_id,patient_name,gender,office_id,register_time,month) values('%s','%s','%s','%s','%s','%s','%d')"),
									serial_id,queue_id,patient_name,gender,pMainFrame->m_officeId,register_time,month);
								threadDtaBase.Execute(insertSql);
							}
						}				
						pHisrecord->MoveNext();
					}
				}
				pHisrecord->Close();
			}
			
			delete pHisrecord;
			pHisrecord=NULL;
			pMyrecord->Close();
			delete pMyrecord;
			pMyrecord=NULL;
			pDelYesRecord->Close();
			delete pDelYesRecord;
			pDelYesRecord=NULL;
		}
		Sleep(1500);
	}
	}
	return 0;
}
*/
void CMainFrame::OnItemAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}


void CMainFrame::SearchDocToadd()
{
	try
	{
		CADODatabase db;
		db.SetConnectionString(m_strConn);
		if(db.Open())
		{
			CADORecordset *pre=new CADORecordset(&db);
			DOCTOADD doctoadd;
			CString docid;
			CString sql=_T("select distinct c.wnd_screen_id,c.compreh_screen_id,c.through_screen_id,c.through_screen_ip,d.doctor_id from Doctor d,Consult c where d.consult_id=c.consult_id");
			if(pre->Open(sql,CADORecordset::openQuery))
			{
				if(!pre->IsBOF())
				{
					while(!pre->IsEOF())
					{
						pre->GetFieldValue(_T("wnd_screen_id"),doctoadd.address);
						pre->GetFieldValue(_T("compreh_screen_id"),doctoadd.comprehAddress);
						pre->GetFieldValue(_T("through_screen_id"),doctoadd.through_screen_id);
						pre->GetFieldValue(_T("through_screen_ip"),doctoadd.through_screen_ip);
						pre->GetFieldValue(_T("doctor_id"),docid);

						m_map_doctoadd[docid]=doctoadd;
						pre->MoveNext();
					}
				}
				pre->Close();
			}
			delete pre;
			pre=NULL;
			db.Close();
		}
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CMainFrame::SearchDocToadd"));
	}
}

void CMainFrame::SearChFirstOffice()
{
	try
	{

		CADODatabase db;
		db.SetConnectionString(m_strConn);
		if(db.Open())
		{
			CADORecordset *pre=new CADORecordset(&db);
			CString sql=_T("");
			sql.Format(_T("select office_id from nurse_office where nurse_id='%s'"),theApp.m_loadNurseID);
			if(pre->Open(sql,CADORecordset::openQuery))
			{
				if(!pre->IsBOF())
				{
					while(!pre->IsEOF())
					{
						pre->GetFieldValue(_T("office_id"),m_officeId);
						break;
					}
				}
				pre->Close();
			}
			delete pre;
			pre=NULL;
			db.Close();
		}
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CMainFrame::SearChFirstOffice"));
	}
}

LRESULT CMainFrame::OnRevReRunMsg(WPARAM wParam, LPARAM lParam)
{
	//ShowWindow(SW_SHOW);
	ShowWindow(SW_MAXIMIZE);
	return 0;
}

void CMainFrame::OnFrUiflush()
{
	// TODO: 在此添加命令处理程序代码
	//CUiFlushTimeDialog dlg(this);
	//dlg.DoModal();
}

void CMainFrame::OnBnShow()
{
	m_pLCDShowDlg=new CLCDShow();
	m_pLCDShowDlg->Create(IDD_DIALOG_SHOWMSG,this);
	

	CRect rectfirst;
	m_pLCDShowDlg->GetWindowRect(&rectfirst);

	CRect rect;
	GetWindowRect(&rect);
//	ScreenToClient(&rect);

	m_pLCDShowDlg->MoveWindow(rect.left+rect.Width()*2/5,rect.top+rect.Height()/3,rectfirst.Width(),rectfirst.Height());
	m_pLCDShowDlg->ShowWindow(SW_SHOWNORMAL);
}

void CMainFrame::OnBnExchange()
{
	GetPatientView()->OnChangeque();
}
void CMainFrame::OnFrPrint()
{
	// TODO: 在此添加命令处理程序代码
	//CPrintSetDlg printDlg(this);
	//if(IDOK==printDlg.DoModal())
	//{
	//	m_nurse->m_printer.ReFreshPrintInfo();
	//}
}

void CMainFrame::OnFrCompset()
{
	//CCompConfPropDlg dlg;
	//dlg.DoModal();
}

void CMainFrame::OnItemExit()
{
	OnClose();
}


void CMainFrame::OnIconMenuShow()
{
	OnSubmenuDisplay();
}

void CMainFrame::OnIconMenuExit()
{
	OnClose();
}

void CMainFrame::OnTakeNumConf()
{
	//CTakingNumConfDlg dlg;
	//dlg.DoModal();
}
/*
void CMainFrame::SetDataDays(int nDays)
{
	if(nDays < 0)
	{
		nDays = 0;
	}
	m_nDataDays = nDays;
	m_strSqlDate.Format(_T("regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime)")
		, m_nDataDays);
}
*/
/*
BOOL CMainFrame::ReturnToHCaller(CString strDoctorId)
{
	int nCallerId = 0;
	CString strQueueNum(_T(""));
	CString strSql;
	strSql.Format(_T("select Q.queue_id_call,D.caller_id from Queue Q left join doctor D on Q.doctor_id=D.doctor_id where Q.status=%d and Q.doctor_id='%s' and Q.regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime);"),
		qsDoing, strDoctorId, theApp.GetController()->GetDataDays());
	try
	{
		CADODatabase db;
		db.Open(theApp.GetController()->m_strSqlConn);
		CADORecordset rset(&db);
		rset.Open(strSql, CADORecordset::openQuery);
		if(!rset.IsBOF())
		{
			rset.GetFieldValue(0, strQueueNum);
			rset.GetFieldValue(1, nCallerId);
		}
		rset.Close();
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseController::ReturnToHCaller"));
		return FALSE;
	}
	catch (...)
	{
		return FALSE;
	}

	if(nCallerId < 1) return FALSE;

	return ShowCallerNum(nCallerId,strQueueNum);
}
*/
/*
BOOL CMainFrame::ShowCallerNum(int CallerAddress,const CString& msg)
{
#ifdef _DEBUG
	MyWriteConsole(msg);
#endif
	char addbuf[256] = {0};
	addbuf[0]=(char)0xFF;
	addbuf[1]=0x68;
	addbuf[4]=0x68;
	addbuf[5]=CallerAddress;//地址
	addbuf[6]=HARDWARE_CALLER_SHOW;
	CCommonConvert convert;
	int len = convert.CStringToChar(msg,NULL);
	char* tempMsg = new char[len+1];
	memset(tempMsg,0,len+1);
	convert.CStringToChar(msg,tempMsg);
	int i = 7;
	for(int j=0;j<len;j++)
	{
		if(tempMsg[j]>='0' && tempMsg[j]<='9')
		{
			int i_msg = tempMsg[j]-48;
			addbuf[i++] = i_msg;
		}
	}
	delete [] tempMsg;
	//////////////////////////////////

	for(int j=5;j<i;j++)
	{
		addbuf[i]+=addbuf[j];
	}
	addbuf[i+1]='\0';
	addbuf[2]=i-5;
	addbuf[3]=i-5;

	DWORD dwWrited;
	BOOL bres=WriteFile(m_hComWndScreen,addbuf,i+2,&dwWrited,NULL);
	return bres;
}
*/

void CMainFrame::InitThroughWnd()
{
	CADODatabase database;
	database.SetConnectionString(m_strConn);
	if(database.Open())
	{
		CADORecordset* pre = new CADORecordset(&database);
		CString sql = _T("select * from ThroughLED");
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
			{
				while(!pre->IsEOF())
				{
					CString IP;int port;int address;int channel;
					pre->GetFieldValue(_T("ip"),IP);
					pre->GetFieldValue(_T("port"),port);
					pre->GetFieldValue(_T("address"),address);
					pre->GetFieldValue(_T("channel_num"),channel);
					SLZCWndScreen* pSlzWndScreen = SLZCWndScreen::GetInstance();
					if(IP.IsEmpty())
					{
						pSlzWndScreen->InitThroughScreen(address);
					}
					else
					{
						pSlzWndScreen->InitThroughScreen(IP,port,address);
					}
					pre->MoveNext();
				}
			}
			pre->Close();
			delete pre;
			pre = NULL;
		}
		database.Close();
	}
}
void CMainFrame::OnConfig()
{
	CPropSheetConfig dlg(_T("配置"));
	dlg.DoModal();
}

void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_F5:
		{
			OnFrFlush();
		}
		break;
	default:
		{

		}
	}

	CFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

LRESULT CALLBACK MyKeyHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox(_T("KeyHook"));
	if(code >= 0)
	{
		if(wParam < 0)//Key down
		{

		}
		else //Key up
		{
			switch(lParam)
			{
			case VK_F5:
				{
					theApp.m_pNurseWnd->OnFrFlush();
				}
				break;
			default:
				{

				}
			}
		}
	}
	return ::CallNextHookEx(g_hKeyHook, code, wParam, lParam);
}

BOOL CMainFrame::DestroyWindow()
{
	//::UnhookWindowsHookEx(g_hKeyHook);

	return CFrameWnd::DestroyWindow();
}

LRESULT CMainFrame::OnLedNotfiyMessage(WPARAM wParam,LPARAM lParam)
{
	TNotifyParam notifyparam;
	CString str;
	m_pVisonShowLED->LED_GetNotifyParam(&notifyparam, wParam);
	switch(notifyparam.Notify){
		case LM_TIMEOUT:
#ifdef _DEBUG
			MyWriteConsole(_T("发送超时"));
#endif
			break;
		case LM_TX_COMPLETE:
			if (notifyparam.Result==RESULT_FLASH)
			{
#ifdef _DEBUG
				MyWriteConsole(_T("数据传送完成，正在写入Flash"));
#endif
			}
			else
			{
#ifdef _DEBUG
				MyWriteConsole(_T("数据传送完成"));
#endif
			}
			break;
		case LM_RESPOND:
			switch(notifyparam.Command){
		case PKC_COM_TRANSFER:
#ifdef _DEBUG
			MyWriteConsole(_T("串口数据发送完成"));
#endif
			break;
		case PKC_GET_POWER:
#ifdef _DEBUG
			if (notifyparam.Status==LED_POWER_ON) MyWriteConsole(_T("读取电源状态完成，当前为电源开启状态"));
			else MyWriteConsole(_T("读取电源状态完成，当前为电源关闭状态"));
#endif
			break;
		case PKC_SET_POWER:
#ifdef _DEBUG
			if (notifyparam.Result==99) MyWriteConsole(_T("当前为定时开关屏模式"));
			else if (notifyparam.Status==LED_POWER_ON) MyWriteConsole(_T("设置电源状态完成，当前为电源开启状态"));
			else MyWriteConsole(_T("设置电源状态完成，当前为电源关闭状态"));
#endif
			break;
		case PKC_GET_BRIGHT:
#ifdef _DEBUG
			str.Format(_T("读取亮度完成，当前亮度=%d"), notifyparam.Status);
			MyWriteConsole(str);
#endif
			break;
		case PKC_SET_BRIGHT:
#ifdef _DEBUG
			if (notifyparam.Result==99) MyWriteConsole(_T("当前为定时亮度调节模式"));
			else
			{
				str.Format(_T("设置亮度完成，当前亮度=%d"), notifyparam.Status);
				MyWriteConsole(str);
			}
#endif
			break;
		case PKC_ADJUST_TIME:
#ifdef _DEBUG
			MyWriteConsole(_T("校正显示屏时间完成"));
#endif
			break;
		case PKC_SET_CURRENT_CHAPTER:
			//			chapterid=notifyparam.Status;
			break;
			}
			break;
		case LM_NOTIFY:
			break;
	}
	return 0;
}

UINT CMainFrame::InitLEDThreadProc(LPVOID pParam)
{
	CMainFrame* pThis = (CMainFrame*)pParam;

//	pThis->InitThroughWnd();
//	SLZCWndScreen* pSlzWndScreen = SLZCWndScreen::GetInstance();
//	pSlzWndScreen->DoThroughInitMsg();
	return 0;
}

