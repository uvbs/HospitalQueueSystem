// NurseStation.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "NurseStation.h"
#include "MainFrm.h"
#include "NurseStationDoc.h"
#include "NurseStationView.h"
#include "MyString.h"
#include "MyCommon.h"
#include "LicenseMaker.h"

#pragma comment(lib, "QueSystemLicense.lib")


#ifdef _DEBUG
#define new DEBUG_NEW

//#pragma comment(lib,"C:/Program Files/Visual Leak Detector/lib/Win32/vld.lib")
//#include "C:\Program Files\Visual Leak Detector\include\vld.h"

HANDLE g_hStdOutput;
void MyWriteConsole(CString str)
{
	::WriteConsole(g_hStdOutput, str, str.GetLength(), NULL, NULL);
	::WriteConsole(g_hStdOutput, "\n", 1, NULL, NULL);
}
#endif


// CNurseStationApp

BEGIN_MESSAGE_MAP(CNurseStationApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CNurseStationApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CNurseStationApp 构造

CNurseStationApp::CNurseStationApp()
	: m_pNurseWnd(NULL)
	//, m_pJttsAPI(NULL)
	, m_pNurseController(NULL)
{
	m_backSet=FALSE;
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CNurseStationApp 对象

CNurseStationApp theApp;
//int g_nDataDays;

// CNurseStationApp 初始化

BOOL CNurseStationApp::InitInstance()
{

#ifdef _DEBUG
	::AllocConsole();
	g_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
#endif


	HWND hWnd = ::FindWindow(NULL, _T("排队叫号系统护士站"));
	if(hWnd)
	{
		::ShowWindow(hWnd, SW_SHOWMAXIMIZED);
		::SetForegroundWindow(hWnd);
		return FALSE;
	}

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("Lingbhc"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	///////////////////////////////////////////////////////////////////////////
	////////////////////	检测注册信息	///////////////////////////////////
	BOOL bGo = FALSE;
	int a = 1, b = 1;
	if(a == b)
	{
		bGo = TRUE;
	}
	if(!bGo) return FALSE;

	if(!VerifyLicense())
	{
		AfxMessageBox(_T("该软件未注册，请先运行注册程序进行注册"));
		return FALSE;
	}
	//////////////////		End 检测注册信息	///////////////////////////////

	//让程序开机启动
	MyCommon::AddAutoRun(_T("NurseStation"));

	//CPoolManage pool;
	//m_strIP=pool.GetComputerIP();
	m_strIP = MyCommon::GetLocalIP();

	////////////////////////////////////////////////////
	/////////////////	弹出登录界面	////////////////
	CDoc_NurseDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return FALSE;
	}
	dlg.EndControl();
	if(m_backSet)
	{
		CPropSetting prop(_T("后台设置"));
		m_pPropSetting = &prop;
		prop.DoModal();
		//delete pdlg;
		return TRUE;
	}
	////////////////////////////////////////////////////

	m_pNurseController = new CNurseController;
	m_pNurseController->SetUserId(dlg.GetUserId());
	//delete pdlg;
	if(!m_pNurseController->StartControl())
	{
		AfxMessageBox(_T("控制服务启动失败!"));
		return FALSE;
	}

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNurseStationDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		NULL);
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	//m_pNurseWnd = (CMainFrame*)m_pMainWnd;



	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新

	m_pMainWnd->SetWindowText(_T("排队叫号系统护士站"));
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}

int CNurseStationApp::ExitInstance()
{
	if(m_pNurseController)
	{
		m_pNurseController->EndControl();
		delete m_pNurseController;
		m_pNurseController = NULL;
	}

	return CWinApp::ExitInstance();
}
/*
BOOL CNurseStationApp::VerifySerialNo()
{
	CString strSerialNoFromReg;
	NurseLicense lic;
	if(!ReadSerialNo(strSerialNoFromReg))
	{
		return FALSE;
	}
	if(licerrNone != lic.VerifyLicense(strSerialNoFromReg))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CNurseStationApp::ReadSerialNo(CString& strSerialNo)
{
	HKEY hKey;
	LONG ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		_T("SOFTWARE\\QueueSystem\\"), 0, KEY_READ, &hKey);
	if(ERROR_SUCCESS != ret)
	{
		return FALSE;
	}
	WCHAR wszSerialNo[128] = {0};
	DWORD dType = REG_SZ;
	DWORD dLength = 128;
	ret = ::RegQueryValueEx(hKey, _T("SerialNo"), NULL, &dType, (LPBYTE)wszSerialNo, &dLength);
	//ret = ::RegGetValue(hKey, NULL, _T("SerialNo"), REG_SZ, NULL, wszSerialNo, NULL);
	::RegCloseKey(hKey);
	//AfxMessageBox(wszSerialNo);
	if(ERROR_SUCCESS != ret)
	{
		return FALSE;
	}
	strSerialNo.Format(_T("%s"), wszSerialNo);

	return TRUE;
}
*/
/*
BOOL CNurseStationApp::AddAutoRun(CString strValueName)
{
	HKEY hKey;
	LONG ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_READ, &hKey);
	if(ERROR_SUCCESS != ret)
	{
		return FALSE;
	}
	WCHAR wszPathGet[MAX_PATH] = {0};
	DWORD dType = REG_SZ;
	DWORD dLength = MAX_PATH;
	ret = ::RegQueryValueEx(hKey, strValueName, NULL, &dType, (LPBYTE)wszPathGet, &dLength);

	//CString strPath = MyCommon::GetModuleFileName();
	WCHAR wszPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL, wszPath, MAX_PATH);

	if(ERROR_SUCCESS == ret && 
		::wcscmp(::wcslwr(wszPathGet), ::wcslwr(wszPath)) == 0)
	{
		::RegCloseKey(hKey);
		return TRUE;
	}

	ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE, &hKey);
	if(ERROR_SUCCESS != ret)
	{

		LPCTSTR lpSubKey = _T("Software\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			return FALSE;
		}
		lpSubKey = _T("Microsoft\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			return FALSE;
		}
		lpSubKey = _T("Windows\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			return FALSE;
		}
		lpSubKey = _T("CurrentVersion\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			return FALSE;
		}
		lpSubKey = _T("Run\\");
		ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_WRITE, &hKey);
		if(ERROR_SUCCESS != ret)
		{
			ret = ::RegCreateKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
		}
		if(ERROR_SUCCESS != ret)
		{
			return FALSE;
		}
	}

	ret = ::RegSetValueEx(hKey, strValueName, 0, REG_SZ,
		(const BYTE*)wszPath, ::wcslen(wszPath) * 2);

	if(ERROR_SUCCESS != ret)
	{
		return FALSE;
	}

	return TRUE;
}
*/

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CNurseStationApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CNurseStationApp::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			if(m_pNurseWnd && m_pNurseWnd->m_wndToolBar)
			{
				int suspendState=m_pNurseWnd->
					m_wndToolBar.GetToolBarCtrl().GetState(IDI_ICON_SUSPEND);
				int recallState=m_pNurseWnd->
					m_wndToolBar.GetToolBarCtrl().GetState(IDI_ICON_RECALL);
				int queState=m_pNurseWnd->
					m_wndToolBar.GetToolBarCtrl().GetState(IDI_ICON_QUE);
				int exportState=m_pNurseWnd->
					m_wndToolBar.GetToolBarCtrl().GetState(IDI_ICON_EXPORT);
				m_pNurseWnd->m_wndToolBar.GetToolBarCtrl().SetState(IDI_ICON_EXPORT,
					exportState &= ~TBSTATE_CHECKED);
				m_pNurseWnd->m_wndToolBar.GetToolBarCtrl().SetState(IDI_ICON_SUSPEND,
					suspendState &= ~TBSTATE_CHECKED);
				m_pNurseWnd->m_wndToolBar.GetToolBarCtrl().SetState(IDI_ICON_RECALL,
					recallState &= ~TBSTATE_CHECKED);
				m_pNurseWnd->m_wndToolBar.GetToolBarCtrl().SetState(IDI_ICON_QUE,
					queState &= ~TBSTATE_CHECKED);

				m_pNurseWnd->m_iconQueCheck=FALSE;
				m_pNurseWnd->m_iconExportCheck=FALSE;
				m_pNurseWnd->m_iconSuspendCheck=FALSE;
				m_pNurseWnd->m_iconRecall=FALSE;
			}
		}
		break;
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

