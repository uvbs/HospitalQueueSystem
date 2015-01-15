// QueueServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueServer.h"
#include "QueueServerDlg.h"
#include "PropSetting.h"
#include "../NurseStation/MyCommon.h"
#include "../NurseStation/MyString.h"
#include "WriteLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MY_TRAYICON		(WM_USER+1)

#define IDEVENT_HIDEWINDOW	1
#define IDEVENT_RUNSERVICE	2


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CQueueServerDlg 对话框




CQueueServerDlg::CQueueServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQueueServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQueueServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQueueServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CONF, &CQueueServerDlg::OnBnClickedButtonConf)
	ON_BN_CLICKED(IDC_BUTTON_RESTART, &CQueueServerDlg::OnBnClickedButtonRestart)
	ON_COMMAND(ID_MENUITEM_SHOW, &CQueueServerDlg::OnMenuitemShow)
	ON_COMMAND(ID_MENUITEM_CONF, &CQueueServerDlg::OnMenuitemConf)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_MY_TRAYICON, OnTrayIcon)
	ON_COMMAND(ID_MENUITEM_RESTART, &CQueueServerDlg::OnMenuitemRestart)
	ON_COMMAND(ID_MENUITEM_QUIT, &CQueueServerDlg::OnMenuitemQuit)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_HIDE, &CQueueServerDlg::OnBnClickedButtonHide)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CQueueServerDlg::OnBnClickedButtonQuit)
	ON_WM_TIMER()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()


// CQueueServerDlg 消息处理程序

BOOL CQueueServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	//读病人线程启动
	//m_readHisData.StartReadDataThread();
	//读office线程启动
	//m_readHisOffice.StartReadHisOfficeThread();
	//读doctor线程启动
	//m_readHisDoctor.StartReadHisDoctorThread();

	//m_ServerController.StartControl();
	//m_ReadHis.Start();

	AddTrayIcon();

	//ModifyStyleEx(WS_EX_APPWINDOW, 0);
	//ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);

	//ShowWindow(SW_HIDE);
	//SetTimer(IDEVENT_HIDEWINDOW, 20, NULL);
	SetTimer(IDEVENT_RUNSERVICE, 10, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQueueServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQueueServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CQueueServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQueueServerDlg::OnBnClickedButtonConf()
{
	/*
	if(m_bProSheet == FALSE)
	{
		m_bProSheet = TRUE;
		m_pProSheet = new CPropSetting(_T("排队系统服务器主程序设置"), this);
		m_pProSheet->DoModal();
		m_bProSheet = FALSE;
	}
	else
	{
		m_pProSheet->ShowWindow(SW_SHOW);
		m_pProSheet->ShowWindow(SW_NORMAL);
		m_pProSheet->UpdateWindow();
	}
	*/
//	m_pProSheet = new CPropSetting(_T("排队系统服务器主程序设置"), this);
	CPropSetting propSheet(_T("设置面板--排队叫号系统服务器主程序"), this);
	if(IDOK == propSheet.DoModal())
	{
		m_ReadHis.LoadConf();
	}
}

void CQueueServerDlg::OnBnClickedButtonRestart()
{
	//char strPath[100]; 
	//::GetModuleFileName(NULL,strPath,100);

	CString strPath = MyCommon::GetModuleFileName();

	//创建守护进程，在启用新进程成功后发送WM_QUIT消息结束原来的进程；
	STARTUPINFO startInfo;
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));
	startInfo.cb = sizeof(STARTUPINFO);
	if(::CreateProcess(NULL,(LPTSTR)(LPCTSTR)strPath, 
		NULL, NULL,FALSE, 0, NULL, NULL, &startInfo, &processInfo))
	{
		::CloseHandle( processInfo.hProcess );
		::CloseHandle( processInfo.hThread );
		PostQuitMessage(WM_DESTROY);
	}
}

void CQueueServerDlg::OnBnClickedButtonHide()
{
	ShowWindow(SW_HIDE);
}

void CQueueServerDlg::OnBnClickedButtonQuit()
{
	if(IDCANCEL == MessageBox(_T("确认退出吗？"), _T("排队叫号系统服务器主程序"), MB_OKCANCEL)) return;
	DestroyWindow();
}

void CQueueServerDlg::OnClose()
{
	ShowWindow(SW_HIDE);

	//CDialog::OnClose();
}

void CQueueServerDlg::OnDestroy()
{
	//m_ServerController.EndControl();
	WriteLog::WriteLogWithTime(_T("服务已正常退出"));
	RemoveTrayIcon();

	CDialog::OnDestroy();
}


BOOL CQueueServerDlg::TaskBarAddIcon(HWND hwnd, 
									  UINT uID, 
									  HICON hIcon, 
									  LPCWSTR lpszTip)
{
	BOOL res;
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = uID;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
	nid.uCallbackMessage = WM_MY_TRAYICON;
	nid.hIcon = hIcon;
	if (lpszTip)
	{
		lstrcpyn(nid.szTip, lpszTip, sizeof(nid.szTip));
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

void CQueueServerDlg::AddTrayIcon(void)
{
	HICON hIcon = LoadIcon(AfxGetApp()->m_hInstance, 
		MAKEINTRESOURCE(IDR_MAINFRAME));
	wchar_t tip[128] = {0};
	wsprintf(tip, _T("排队叫号系统服务器主程序"));
	TaskBarAddIcon(this->m_hWnd, IDR_MAINFRAME, hIcon, tip);
}

LRESULT CQueueServerDlg::OnTrayIcon(WPARAM wParam, LPARAM lParam)
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
			menu.LoadMenu(IDR_MENU_ICON);
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

BOOL CQueueServerDlg::TaskBarDeleteIcon(HWND hwnd, UINT uID)
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = uID;
	return Shell_NotifyIcon(NIM_DELETE, &nid);
}

void CQueueServerDlg::RemoveTrayIcon()
{
	TaskBarDeleteIcon(this->m_hWnd, IDR_MAINFRAME);
}

void CQueueServerDlg::OnMenuitemShow()
{
	ShowWindow(SW_SHOWNORMAL);
}

void CQueueServerDlg::OnMenuitemConf()
{
	OnBnClickedButtonConf();
}

void CQueueServerDlg::OnMenuitemRestart()
{
	OnBnClickedButtonRestart();
}

void CQueueServerDlg::OnMenuitemQuit()
{
	OnBnClickedButtonQuit();
}



void CQueueServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case IDEVENT_HIDEWINDOW:
		{
			KillTimer(IDEVENT_HIDEWINDOW);
			ShowWindow(SW_HIDE);
		}
		break;
	case IDEVENT_RUNSERVICE:
		{
			//m_ServerController.StartControl();
			KillTimer(IDEVENT_RUNSERVICE);
			if(m_ReadHis.Start())
			{
				WriteLog::WriteLogWithTime(_T("HIS数据读取服务已启动"));
			}
			else
			{
				WriteLog::WriteLogWithTime(_T("HIS数据读取服务启动失败"));
			}
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CQueueServerDlg::OnNcPaint()
{
	static int i = 2;
	if(i > 0)
	{
		ShowWindow(SW_HIDE);
		i--;
	}
	else
	{
		CDialog::OnNcPaint();
	}
}
