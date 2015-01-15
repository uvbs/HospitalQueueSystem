// Doc_NurseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "Doc_NurseDlg.h"
#include "BaseConfig.h"
#include "ado2.h"
#include "ConnectToBase.h"
#include "Parameter.h"
#include "MainFrm.h"
#include "NurseDef.h"
#include "MyString.h"
#include "MyCommon.h"
#include "MyPort.h"
#include "WriteLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDoc_NurseDlg 对话框

CDoc_NurseDlg::CDoc_NurseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDoc_NurseDlg::IDD, pParent)
	, m_loadNurseID(_T(""))
	, m_loadNursePassword(_T(""))
	, m_bCanLogin(TRUE)
	//, m_pMySocketUDP(NULL)
	//, m_pSocketListenThread(NULL)
{
	
	m_bShow=TRUE;
	m_range=0;
	m_bIsConnect=FALSE;
	m_bIsCreate=FALSE;
	m_bSetShowText=FALSE;
	
	//pbase = NULL;
	m_pImageList = NULL;
	//g_Doc_NurseDlg = this;
	CBaseConfig baseConfig;
	m_strConn.Format(_T("Driver={%s};Server=%s;Database=Nurse;Uid=%s;Pwd=%s;"),
		baseConfig.GetDataDrive(),baseConfig.GetServerIP(),
		baseConfig.GetServerAcount(), baseConfig.GetServerPassword());
}

void CDoc_NurseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECKBACK, m_checkBack);
	//DDX_Text(pDX, IDC_USERNAME, m_loadNurseID);
	DDX_Text(pDX, IDC_PASSWORD, m_loadNursePassword);
	DDX_Control(pDX, IDC_CH_REQUEUE, m_checkReque);
	DDX_Control(pDX, IDC_COMBO_USERNAME, m_combo_userlogin);
	DDX_Control(pDX, IDC_CHECK_REMPASS, m_check_rempass);
	DDX_Control(pDX, IDC_CHECK_AUTOLOGIN, m_check_autologin);
	DDX_Control(pDX, IDC_PROGRESSZ_LOGIN, m_progress_login);
	DDX_Control(pDX, ID_BN_BASESET, m_bn_baseset);
	DDX_Control(pDX, IDC_PASSWORD, m_edit_password);
	DDX_Control(pDX, IDC_STATIC_USERID, m_static_userid);
	DDX_Control(pDX, IDC_STATIC_USERPASS, m_static_password);
	DDX_Control(pDX, IDOK, m_bn_login);
}

BEGIN_MESSAGE_MAP(CDoc_NurseDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDoc_NurseDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_BN_BASESET, &CDoc_NurseDlg::OnBnClickedBnBaseset)
	ON_WM_DESTROY()
	ON_CBN_EDITCHANGE(IDC_COMBO_USERNAME, &CDoc_NurseDlg::OnCbnEditchangeComboUsername)
	ON_CBN_SELCHANGE(IDC_COMBO_USERNAME, &CDoc_NurseDlg::OnCbnSelchangeComboUsername)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDoc_NurseDlg 消息处理程序

BOOL CDoc_NurseDlg::OnInitDialog()
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
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_baseDrive=m_baseconfig.GetDataDrive();
	m_baseAccount=m_baseconfig.GetServerAcount();
	m_baseIp=m_baseconfig.GetServerIP();
	m_basePassword=m_baseconfig.GetServerPassword();
//	pbase=new CADODatabase();
	//读文件
	ReadUserIdFromFile();
	//
	AddStringToComBo();
	SetLastLoginShow();

	MyInitLoginPictrue();
	m_show_picture.ShowWindow(SW_HIDE);
	
	
	if(m_check_autologin.GetCheck()==1)
	{
		PostMessage(WM_COMMAND,IDOK,NULL);
	}
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDoc_NurseDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDoc_NurseDlg::OnPaint()
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
HCURSOR CDoc_NurseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDoc_NurseDlg::OnBnClickedOk()
{
	m_bShow=!m_bShow;
	if(m_bShow)//取消
	{
		if(m_checkBack.GetCheck() != BST_CHECKED)
		{
			EndControl();
		}

//		WaitForSingleObject(m_hCheckDataBase,1000*30);
		m_bn_login.SetWindowText(_T("确定"));
		m_bn_baseset.ShowWindow(SW_SHOW);
		m_progress_login.ShowWindow(SW_HIDE);
		m_check_autologin.ShowWindow(SW_SHOW);
		m_check_rempass.ShowWindow(SW_SHOW);
		m_checkBack.ShowWindow(SW_SHOW);
		m_checkReque.ShowWindow(SW_SHOW);
		m_static_password.ShowWindow(SW_SHOW);
		m_static_userid.ShowWindow(SW_SHOW);
		m_edit_password.ShowWindow(SW_SHOW);
		m_combo_userlogin.ShowWindow(SW_SHOW);
		m_show_picture.ShowWindow(SW_HIDE);
		m_bshowpicture=FALSE;
		m_bIsCreate=FALSE;
		m_bSetShowText=FALSE;
		m_bIsConnect=FALSE;
		
		m_range=0;
		m_progress_login.SetPos(0);
		if(m_show_static_userid.m_hWnd)
			m_show_static_userid.DestroyWindow();
		TerminateThread(m_hCheckDataBase,0);

		KillTimer(MYLOGINTIMEID);
		Sleep(10);
		m_bn_login.EnableWindow(TRUE);
		return;
	}
	else //确定
	{
		CString strNurseID;
		m_combo_userlogin.GetWindowText(strNurseID);
		if(m_check_autologin.GetCheck()==1)
		{
			Sleep(300);
		}

		m_bCanLogin = TRUE;
		UpdateData();

		SetUserId(strNurseID);
		SetPassword(m_loadNursePassword);
		SetUserType(utNurse);

		//BOOL bRet = StartControl();

		m_bn_login.SetWindowText(_T("取消"));
		m_bn_login.EnableWindow(FALSE);
		m_bn_baseset.ShowWindow(SW_HIDE);
		m_progress_login.ShowWindow(SW_SHOW);
		m_check_autologin.ShowWindow(SW_HIDE);
		m_check_rempass.ShowWindow(SW_HIDE);
		m_checkBack.ShowWindow(SW_HIDE);
		m_checkReque.ShowWindow(SW_HIDE);
		m_static_password.ShowWindow(SW_HIDE);
		m_static_userid.ShowWindow(SW_HIDE);
		m_edit_password.ShowWindow(SW_HIDE);
		m_combo_userlogin.ShowWindow(SW_HIDE);
		m_show_picture.ShowWindow(SW_SHOW);
		m_bshowpicture=TRUE;

		if(m_checkBack.GetCheck() != BST_CHECKED)
		{
			BOOL bRet = StartControl();
			if(!bRet)
			{
				m_bIsConnect=FALSE;
				m_bIsCreate=FALSE;
				EndControl();
				//TerminateThread(m_hCheckDataBase,0);
				//KillTimer(MYLOGINTIMEID);
				PostMessage(WM_COMMAND,IDOK,NULL);
				return;
				//PostMessage(WM_COMMAND,IDCANCEL,NULL);
				//DestroyWindow();
				//::ExitThread(0);
			}
		}

		if(!m_bIsCreate)
		{
			m_hCheckDataBase=CreateThread(NULL,0,CheckDataBase,this,NULL,0);//创建连接数据库线程
			if(INVALID_HANDLE_VALUE == m_hCheckDataBase)
			{
				m_bIsConnect = FALSE;
			}
//			m_hCheckHisDataBase=CreateThread(NULL,0,CheckHisDataBase,this,NULL,0);
		}
		m_progress_login.SetRange32(0,99);
		SetTimer(MYLOGINTIMEID,1000,NULL);
	}
}

BOOL CDoc_NurseDlg::ConnectToBase()
{
	CADODatabase* pdataBase;
	pdataBase=new CADODatabase;
	if(pdataBase->IsOpen())
	{
		pdataBase->Close();
	}
	try
	{
		pdataBase->SetConnectionString(_T("Driver={"+m_baseDrive+"};Server="+m_baseIp+";Database=Nurse;Uid="+m_baseAccount+";Pwd="+m_basePassword+";"));
		if(pdataBase->Open())
		{
			return TRUE;
		}
		else
		{
			delete pdataBase;
			return FALSE;
		}
	}
	catch(_com_error& e)
	{
		return FALSE;
	}
}

void CDoc_NurseDlg::OnBnClickedBnBaseset()
{
	// TODO: 在此添加控件通知处理程序代码
	CSetDataBaseDlg dlg(this);
	if(dlg.DoModal() == IDOK)
	{
		m_baseDrive=m_baseconfig.GetDataDrive();
		m_baseAccount=m_baseconfig.GetServerAcount();
		m_baseIp=m_baseconfig.GetServerIP();
		m_basePassword=m_baseconfig.GetServerPassword();
	}
}

void CDoc_NurseDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	//if(pbase)
	//{
	//	delete pbase;
	//	pbase = NULL;
	//}
	if(m_pImageList)
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
}

void CDoc_NurseDlg::GetNurseCandoOfficeID()
{
	try
	{

		CADODatabase db;
		db.SetConnectionString(m_strConn);
		if(db.Open())
		{
			CADORecordset *pre=new CADORecordset(&db);
			CString sql=_T("");
			sql.Format(_T("select o.name,o.office_id from Nurse_Office n,Office o where Nurse_id='%s' and n.office_id=o.office_id order by o.serial_id"),
				m_loadNurseID);
			if(pre->Open(sql,CADORecordset::openQuery))
			{
				if(!pre->IsBOF())
				{
					while(!pre->IsEOF())
					{
						CString officeid;
						pre->GetFieldValue(_T("office_id"),officeid);
						((CNurseStationApp*)AfxGetApp())->m_canDoOfficeIDList.Add(officeid);
						pre->MoveNext();
					}
				}
			}
			delete pre;
			pre=NULL;
			db.Close();
		}
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CDoc_NurseDlg::GetNurseCandoOfficeID()"));
	}
	catch (...)
	{
	}
}

void CDoc_NurseDlg::DeleteDayData()
{
	try
	{

		CADODatabase db;
		db.SetConnectionString(m_strConn);
		if(db.Open())
		{
			CString sql=_T("");
			sql.Format(_T("delete from Queue where convert(varchar(10),getdate(),120)=convert(varchar(10),regtime,120) and office_id in(select o.office_id from Nurse_Office n,Office o where Nurse_id='%s' and n.office_id=o.office_id)"),
				m_loadNurseID);
			db.Execute(sql);

			sql.Format(_T("delete from hisforbjlb_patient where convert(varchar(10),getdate(),120)=convert(varchar(10),register_time,120) and office_id in(select o.office_id from Nurse_Office n,Office o where Nurse_id='%s' and n.office_id=o.office_id)"),
				m_loadNurseID);
			db.Execute(sql);
			db.Close();
		}
	}
	catch(_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CDoc_NurseDlg::DeleteDayData()"));
	}
}

BOOL CDoc_NurseDlg::WriteUserIdToFile()
{
	CString filepath=m_mygetfile.GetExeFullFilePath();
	filepath+=_T("//ICreatdit.dat");
	CFile file;
	if(file.Open(filepath,CFile::modeCreate|CFile::modeReadWrite))
	{
		CTime time=CTime::GetCurrentTime();
		LOGINUSERINFO loginfo;
		POSITION pos=m_login_info.GetHeadPosition();
		while(pos)
		{
			CTimeSpan timespan;
			loginfo=m_login_info.GetNext(pos);
			CString strTemp(loginfo.password);
			timespan=time-loginfo.loginTime;
			if(timespan.GetDays()<=30)
			{
				file.Write(&loginfo,sizeof(LOGINUSERINFO));
			}
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CDoc_NurseDlg::ReadUserIdFromFile()
{
	CString filepath=m_mygetfile.GetExeFullFilePath();
	filepath+=_T("//ICreatdit.dat");
	CFile file;
	if(file.Open(filepath,CFile::modeRead))
	{
		ULONGLONG count=file.GetLength()/sizeof(LOGINUSERINFO);
		for(ULONGLONG i=0;i<count;i++)
		{
			LOGINUSERINFO loginfo;
			if(file.Read(&loginfo,sizeof(LOGINUSERINFO))>0)
			{
				m_login_info.AddTail(loginfo);
			}
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CDoc_NurseDlg::AddStringToComBo()
{
	
	m_pImageList = new CImageList();
	m_pImageList->Create(16,16,ILC_COLOR32|ILC_MASK,5,5);
	m_pImageList->Add(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ADMINISTOR)));
	m_pImageList->Add(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_BAKSET)));
	m_pImageList->Add(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_USER)));
	m_pImageList->Add(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_USERS)));
	m_pImageList->Add(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_MYUSERS)));
	m_combo_userlogin.SetImageList(m_pImageList);
	
	
	POSITION pos=m_login_info.GetHeadPosition();
	LOGINUSERINFO loginfo;

	while(pos)
	{
		loginfo=m_login_info.GetNext(pos);
		CString strtemp(loginfo.loginID);
		COMBOBOXEXITEM   Item;    
		Item.mask   =   CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE; 
		Item.pszText   =  strtemp.GetBuffer(0);
		if(loginfo.power==0)
		{
			Item.iImage=0;
			Item.iSelectedImage=0;
		}
		if(loginfo.power==1)
		{
			Item.iImage=3;
			Item.iSelectedImage=3;
		}
		if(loginfo.power==2)
		{
			Item.iImage=4;
			Item.iSelectedImage=4;
		}
		Item.iItem   =   0;
		m_combo_userlogin.InsertItem(&Item);
	}
	
	return TRUE;
}

void CDoc_NurseDlg::SetLastLoginShow()
{
	LOGINUSERINFO loginfo;
	if(m_login_info.GetCount()>0)
	{
		loginfo=m_login_info.GetTail();
		CString strLoginID(loginfo.loginID);
		m_combo_userlogin.SetCurSel(0);
		if(loginfo.status==1)
			m_check_autologin.SetCheck(1);
		CString strLoiginPass(loginfo.password);
		if(strLoiginPass!=_T(""))
		{
			m_check_rempass.SetCheck(1);
			m_loadNursePassword=strLoiginPass;
			UpdateData(FALSE);
		}
	}
}
void CDoc_NurseDlg::OnCbnEditchangeComboUsername()
{
	// TODO: 在此添加控件通知处理程序代码
	m_check_rempass.SetCheck(0);
	m_check_autologin.SetCheck(0);
	m_loadNursePassword=_T("");
	UpdateData(FALSE);
}

void CDoc_NurseDlg::OnCbnSelchangeComboUsername()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_combo_userlogin.GetCurSel();
	CString loginId=_T("");
	m_combo_userlogin.GetLBText(index,loginId);
	POSITION pos=m_login_info.GetHeadPosition();
	LOGINUSERINFO logininfo;
	while(pos)
	{
		logininfo=m_login_info.GetNext(pos);
		CString strTemp(logininfo.loginID);
		if(strTemp==loginId)
		{
			if(logininfo.status==1)
				m_check_autologin.SetCheck(1);
			CString strPass(logininfo.password);
			if(strPass!=_T(""))
			{
				m_check_rempass.SetCheck(1);
				m_loadNursePassword=strPass;
			}
			break;
		}
	}
	UpdateData(FALSE);
}

void CDoc_NurseDlg::LoginToNurse()
{
	
	///////////////////////////////////////
	//创建登录user
	//////////////////////////////////////
	//写登录信息文件判断是否重复,重复删除
	
	POSITION pos;
	LOGINUSERINFO logininfo;
	int datacount=m_login_info.GetCount();
	for(int i=0;i<datacount;i++)
	{
		pos=m_login_info.FindIndex(i);
		logininfo=m_login_info.GetAt(pos);
		CString strtemp(logininfo.loginID);
		if(strtemp==m_loadNurseID)
		{
			m_login_info.RemoveAt(pos);
			break;
		}
	}

	///////////////////////////////////////
	//UpdateData();
	try
	{
		CADODatabase db;
		db.SetConnectionString(m_strConn);
		db.Open();
		CADORecordset *pRecord=new CADORecordset(&db);
		CString sql;
		sql.Format(_T("select * from Nurse where nurse_id='%s'and Password='%s'"),m_loadNurseID,m_loadNursePassword);							
		if(pRecord->Open(sql,CADORecordset::openQuery))
		{
			if(pRecord->IsBOF())
			{
				OnBnClickedOk();
				MessageBox(_T("账号密码错误"),_T("Error"),MB_OK|MB_ICONERROR);
				return;
			}
			pRecord->GetFieldValue(_T("power"),theApp.m_power);
			pRecord->GetFieldValue(_T("nurse_name"),theApp.m_loadNurseName);
			pRecord->GetFieldValue(_T("nurse_id"),theApp.m_loadNurseID);
			//写入文件
			CTime time=CTime::GetCurrentTime();
			wcscpy_s(logininfo.loginID, MAX_ID_LENGTH, m_loadNurseID.GetBuffer(0));
			logininfo.loginTime=time;
			logininfo.power=theApp.m_power;
			if(m_check_rempass.GetCheck()==1)
			{
				//记住密码
				wcscpy_s(logininfo.password, MAX_PASSWORD_LENGTH, m_loadNursePassword.GetBuffer(0));
			}
			else
			{
				memset(logininfo.password,0,MAX_PASSWORD_LENGTH);
			}
			if(m_check_autologin.GetCheck()==1)
			{
				logininfo.status=1;//自动登录
			}
			else
			{
				logininfo.status=0;
			}
			m_login_info.AddTail(logininfo);
			WriteUserIdToFile();
		}
		//////////////////////////////////////////////
		if(m_checkBack.GetCheck()==0)
		{
			////////////////////////////////////////////////////////////////////////////
			////////////////  检测当前程序是否已运行 ///////////////////////////////////
			//HWND hWndExist = ::FindWindow(NULL, _T("护士站"));
			//if(hWndExist)
			//{
			//	//::ShowWindow(hWndExist, SW_RESTORE);
			//	//::ShowWindow(hWndExist, SW_HIDE);
			//	::ShowWindow(hWndExist, SW_MINIMIZE);
			//	::ShowWindow(hWndExist, SW_SHOWMAXIMIZED);
			//	//::ShowWindow(hWndExist, SW_SHOW);
			//	//::ShowWindow(hWndExist, SW_MAXIMIZE);
			//	//::SetActiveWindow(hWndExist);
			//	//::SetWindowPos(hWndExist, HWND_TOPMOST , NULL , NULL , NULL , NULL , SWP_NOSIZE | SWP_NOMOVE);
			//	::SetForegroundWindow(hWndExist);
			//	DestroyWindow();
			//	return;
			//}
			///////////////////		End 运行实例检测	///////////////////////////////

			if(theApp.m_power==USERTYPE_SYSADMIN || theApp.m_power==USERTYPE_NURSE || theApp.m_power==USERTYPE_MANAGER)
			{
				if(m_checkReque.GetCheck()==1)
				{
					DeleteDayData();//删除今天已经排队的数据
				}
				sql.Format(_T("insert into LoginInfo(UserName,UserPassword,LoadTime,UserPower,UserIP) values('%s','%s',getdate(),'%d','%s')"),
					theApp.m_loadNurseID,theApp.m_loadNursePassword,theApp.m_power,theApp.m_strIP);
				db.Execute(sql);//写入登录日志

				GetNurseCandoOfficeID();//获取护士站处理科室
				OnOK();
			}
			else
			{
				OnBnClickedOk();
				MessageBox(_T("没有前台权限"),_T("注意"),MB_OK|MB_ICONINFORMATION);
				return;
			}
		}
		///////////////////////////////////////
		//if(m_checkBack.GetCheck()==1)
		else
		{
			////////////////////////////////////////////////////////////////////////////
			////////////////  检测当前程序是否已运行 ///////////////////////////////////
			//HWND hWndExist = ::FindWindow(NULL, _T("后台设置"));
			//if(hWndExist)
			//{
			//	//::ShowWindow(hWndExist, SW_RESTORE);
			//	//::ShowWindow(hWndExist, SW_HIDE);
			//	::ShowWindow(hWndExist, SW_MINIMIZE);
			//	::ShowWindow(hWndExist, SW_SHOWMAXIMIZED);
			//	//::ShowWindow(hWndExist, SW_SHOW);
			//	//::ShowWindow(hWndExist, SW_MAXIMIZE);
			//	//::SetActiveWindow(hWndExist);
			//	//::SetWindowPos(hWndExist, HWND_TOPMOST , NULL , NULL , NULL , NULL , SWP_NOSIZE | SWP_NOMOVE);
			//	::SetForegroundWindow(hWndExist);
			//	DestroyWindow();
			//	return;
			//}
			///////////////////		End 运行实例检测	///////////////////////////////

			if(theApp.m_power==USERTYPE_MANAGER || theApp.m_power==USERTYPE_SYSADMIN)
			{
				if(m_checkReque.GetCheck()==1)
				{
					//DeleteDayData();
				}
				GetNurseCandoOfficeID();//获取护士站处理科室
				sql.Format(_T("insert into LoginInfo(UserName,UserPassword,LoadTime,UserPower,UserIP) values('%s','%s',getdate(),'%d','%s')"),
					theApp.m_loadNurseID,theApp.m_loadNursePassword,theApp.m_power,theApp.m_strIP);
				db.Execute(sql);
				theApp.m_backSet=TRUE;
				OnOK();
			}
			else
			{
				OnBnClickedOk();
				MessageBox(_T("没有设置权限"),_T("注意"),MB_OK|MB_ICONEXCLAMATION);
				return;
			}	
		}

		delete pRecord;
		pRecord=NULL;
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CDoc_NurseDlg::LoginToNurse()"));
		MessageBox(_T("无法连接到服务器数据库"),_T("错误"),MB_OK|MB_ICONERROR);
		OnBnClickedOk();
		//
	}

	//////////////////////////////////
}
HBRUSH CDoc_NurseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if(nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkMode(TRANSPARENT);
	}
	return hbr;
}


void CDoc_NurseDlg::MySetLoginText(CString str)
{
	str+=_T("正在尝试连接数据库...");
	HDC hDC=::GetDC(NULL);
	CRect rect;
	GetClientRect(&rect);
	m_font.CreateFont(-17,0,0,0,800,0,0,0,0,0,0,0,0,_T("宋体"));
	CSize	size;
	HGDIOBJ hOld = SelectObject( hDC, m_font.m_hObject );
	GetTextExtentPoint32( hDC, str, str.GetLength(), &size );
	CRect staticrect;
	staticrect.left=rect.Width()/2-(size.cx/2);
	staticrect.right=rect.Width()/2+(size.cx/2);
	staticrect.top=rect.top+10;
	staticrect.bottom=rect.top+30;
	m_show_static_userid.Create(_T(""),WS_CHILD|WS_VISIBLE,staticrect,this);
	m_show_static_userid.SetFont(&m_font);
	m_show_static_userid.SetWindowText(str);
	SelectObject( hDC, hOld );
	::ReleaseDC( NULL, hDC );
	m_font.DeleteObject();
}


void CDoc_NurseDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case MYLOGINTIMEID:
		{
			//KillTimer(MYLOGINTIMEID);
			m_bn_login.EnableWindow(TRUE);
			if(!m_bSetShowText)
			{
				MyInitSetLoginText();
				m_bSetShowText=TRUE;
			}
			m_range+=50;
			m_progress_login.SetPos(m_range);
			if(m_range>=99)
			{
				m_range=0;
				KillTimer(MYLOGINTIMEID);
				///////  Modified by Songyz 2014.01.04 //////////////
				if(m_bIsConnect && m_bCanLogin)
				//if(m_bCanLogin)
				{
					//KillTimer(MYLOGINTIMEID);
					LoginToNurse();
				}
				else
				{
					PostMessage(WM_COMMAND, IDOK, NULL);
					if(!m_bIsConnect)
					{
						MessageBox(_T("无法连接到服务器数据库"), _T("登录错误"));
					}
					else
					{
						MessageBox(_T("Socket通信错误"), _T("登录错误"));
					}
					//PostMessage(WM_COMMAND, IDOK, NULL);
				}
				///////  End Modified	/////////////////////////////
			}
			if(m_bshowpicture)
			{
				ShowLoginBitmap();
			}
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CDoc_NurseDlg::MyInitLoginInfoDataBase()
{
	m_baseIp=m_baseconfig.GetServerIP();
	m_baseAccount=m_baseconfig.GetServerAcount();
	m_basePassword=m_baseconfig.GetServerPassword();
	m_baseDrive=m_baseconfig.GetDataDrive();
	
	/////////////////////////////////////
	if(!ConnectToBase())
	{
		return FALSE;
	}
	////////////////////////////////////////
	return TRUE;
}

BOOL CDoc_NurseDlg::MyInitSetLoginText()
{
	m_combo_userlogin.GetWindowText(m_loadNurseID);
	MySetLoginText(m_loadNurseID);
	return TRUE;
}

BOOL CDoc_NurseDlg::MyInitLoginPictrue()
{
	CRect rect;
	GetClientRect(&rect);
	CRect staticrect;
	staticrect.left=rect.Width()/2-150;
	staticrect.right=rect.Width()/2+150;
	staticrect.top=35;
	staticrect.bottom=188;
	m_show_picture.Create(_T(""),WS_VISIBLE|WS_CHILD|SS_ICON|SS_CENTERIMAGE,staticrect,this);
	return TRUE;
}

DWORD WINAPI CDoc_NurseDlg::CheckDataBase(LPVOID pParam)
{
	CDoc_NurseDlg* pDlg=(CDoc_NurseDlg*)pParam;
	pDlg->m_bIsCreate = TRUE;
	pDlg->m_bIsConnect=FALSE;
	if(pDlg->MyInitLoginInfoDataBase())
	{
		pDlg->m_bIsConnect=TRUE;
		return 1;
	}
	else
	{
		pDlg->m_bIsConnect=FALSE;
		pDlg->MessageBox(_T("数据库连接失败"),_T("Error"),MB_OK | MB_ICONINFORMATION);
		pDlg->PostMessage(WM_COMMAND,IDOK,NULL);
		return 0;
	}
}

BOOL CDoc_NurseDlg::ShowLoginBitmap()
{	
	CBitmap bitmap;
	
	bitmap.LoadBitmap(IDB_BITMAP_HOSPITAL);
	//获取内容
	BITMAP bm;
	bitmap.GetBitmap(&bm);

	CDC dc;
	dc.CreateCompatibleDC(GetDC());
	CBitmap *pOldBitmap=(CBitmap*)dc.SelectObject(bitmap);
	CRect showrect;
	//获取static控件大小
	dc.SetStretchBltMode(COLORONCOLOR);
	m_show_picture.GetClientRect(&showrect);
	m_show_picture.GetDC()->StretchBlt(showrect.left,showrect.top,showrect.Width(),showrect.Height(),&dc,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
	dc.SelectObject(&pOldBitmap);

	return TRUE;
}

//////// Modified by Songyz	2014.01.04//////////////////////////////////

void CDoc_NurseDlg::OnRecvLoginResult(LPCMDRESULT pLoginRes, LPSOCKADDR_IN psockddr)
{
	EndControl();
	switch(pLoginRes->eRes)
	{
	case vrLogined:
		{
			m_bCanLogin = FALSE;
//			OnBnClickedOk();
			PostMessage(WM_COMMAND,IDOK,NULL);
			MessageBox(_T("该帐号已登录"),_T("登录错误"),MB_OK | MB_ICONERROR);
			//PostMessage(WM_COMMAND,IDOK,NULL);
		}
		break;
	case vrFoundOffice:
		{
			m_bCanLogin = FALSE;
			PostMessage(WM_COMMAND,IDOK,NULL);
			MessageBox(_T("该帐号与某已登录帐号的可处理科室存在重叠"),_T("登录错误"),MB_OK | MB_ICONERROR);
			//PostMessage(WM_COMMAND,IDOK,NULL);
		}
		break;
	}
}

void CDoc_NurseDlg::OnSendVerify()
{
	UpdateData();
	CString strNurseID;
	m_combo_userlogin.GetWindowText(strNurseID);
	REQLOGIN login;
	ZeroMemory(&login, sizeof(login));
	login.header.eTypeID = ntiReqLogin;
	wcscpy_s(login.header.sUserInfo.wszUserID, MAX_ID_LENGTH, strNurseID.GetBuffer());
	wcscpy_s(login.wszPassword, MAX_PASSWORD_LENGTH, m_loadNursePassword.GetBuffer());

	Broadcast((char*)&login, sizeof(login));
}

//////// End Modify //////////////////////////////////////////////////////

BOOL CDoc_NurseDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return CDialog::OnEraseBkgnd(pDC);
//	return TRUE;
}

BOOL CDoc_NurseDlg::DrawBack(CDC* pDC,CRect& destRect)
{
	CBitmap bitmap;

	if(!bitmap.LoadBitmap(IDB_BITMAP_BACK))
	{
		return FALSE;
	}

	BITMAP bm;
	bitmap.GetBitmap(&bm);

	CDC dc;
	dc.CreateCompatibleDC(GetDC());
	CBitmap *pOldBitmap=(CBitmap*)dc.SelectObject(bitmap);

	pDC->SetStretchBltMode(COLORONCOLOR);//避免颜色失真
	pDC->StretchBlt(destRect.left,destRect.top,destRect.Width(),destRect.Height(),
		&dc,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
	dc.SelectObject(&pOldBitmap);
	bitmap.DeleteObject();
	return TRUE;
}
void CDoc_NurseDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	GetClientRect(m_rect);
	// TODO: 在此处添加消息处理程序代码
}



