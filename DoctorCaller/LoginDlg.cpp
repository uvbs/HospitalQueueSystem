// LoginDlg.cpp : 实现文件
//


#include "stdafx.h"
#include "DoctorCaller.h"
#include "LoginDlg.h"
//#include "DoctorCallerDlg.h"
#include "..\NurseStation\ado2.h"
#include "..\NurseStation\BaseConfig.h"
#include "../NurseStation/MyCommon.h"
#include "../NurseStation/MyString.h"
#include "../NurseStation/MyPort.h"
#include "DlgDoctorLoginConf.h"

#define TIMERID_WAIT_VERIFY				1
#define TIMERID_WAIT_CANCEL_AUTOLOGIN	2


// CLoginDlg 对话框
//CLoginDlg* g_pLoginDlg = NULL;
IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)
CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
	, m_strDoctorID(_T(""))
	, m_strPassword(_T(""))
	//, m_pMySocketUDP(NULL)
	//, m_pSocketListenThread(NULL)
{
		//g_pLoginDlg = this;
	//m_nNursePort = MyCommon::GetProfileInt(
	//	_T("sys"), _T("NURSE_PORT"), 4020, _T("SYS\\port.ini"));
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_USERID, m_strDoctorID);
	DDX_Control(pDX, IDC_EDIT_USERID, m_editUserID);
	//DDX_Control(pDX, IDC_CHECK_ACCOUNT, m_ckbAccount);
	DDX_Control(pDX, IDC_CHECK_PASSWORD, m_ckbPassword);
	DDX_Control(pDX, IDC_CHECK_AUTOLOGIN, m_ckbAutoLogin);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDOK, m_btnLogin);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_AUTOLOGIN, &CLoginDlg::OnBnClickedCheckAutologin)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN_CONF, &CLoginDlg::OnBnClickedButtonLoginConf)
END_MESSAGE_MAP()


BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strConfPath = MyCommon::GetModuleDir();
	m_strConfPath += _T("\\sysconf.dat");
	ReadConf();
	//SocketInit();
	return TRUE;
}

// CLoginDlg 消息处理程序

void CLoginDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if(m_strDoctorID.IsEmpty())
	{
		MessageBox(_T("请输入用户名"));
		return;
	}
	Login();
	SaveConf();
	//OnOK();
}

void CLoginDlg::Login()
{
	m_btnLogin.EnableWindow(FALSE);
	UpdateData();
	SetUserId(m_strDoctorID);
	SetPassword(m_strPassword);
	SetUserType(utDoctor);
	StartControl();
	SetTimer(TIMERID_WAIT_VERIFY, 5000, NULL);
}

void CLoginDlg::ReadConf()
{
	LOGINCONF conf = {0};
	if(ReadConfFromFile(&conf, m_strConfPath))
	{
		m_strDoctorID = conf.wszAccount;
		if(conf.bRembPassword)
		{
			m_ckbPassword.SetCheck(BST_CHECKED);
			m_strPassword = conf.wszPassword;
		}
		else
		{
			m_ckbPassword.SetCheck(BST_UNCHECKED);
		}
		if(conf.bAutoLogin)
		{
			m_ckbAutoLogin.SetCheck(BST_CHECKED);
		}
		else
		{
			m_ckbAutoLogin.SetCheck(BST_UNCHECKED);
		}
	}
	UpdateData(FALSE);
	if(conf.bAutoLogin)
	{
		//Login();
		SetTimer(TIMERID_WAIT_CANCEL_AUTOLOGIN, 2000, NULL);
		return;
	}
	m_editUserID.SetFocus();
}

void CLoginDlg::SaveConf()
{
	LOGINCONF conf = {0};
	wcscpy_s(conf.wszAccount, MAX_ID_LENGTH, m_strDoctorID.GetBuffer());
	wcscpy_s(conf.wszPassword, MAX_PASSWORD_LENGTH, m_strPassword.GetBuffer());
	if(m_ckbPassword.GetCheck() == BST_CHECKED)
	{
		conf.bRembPassword= TRUE;
	}
	else
	{
		conf.bRembPassword = FALSE;
	}
	if(m_ckbAutoLogin.GetCheck() == BST_CHECKED)
	{
		conf.bAutoLogin = TRUE;
	}
	else
	{
		conf.bAutoLogin = FALSE;
	}
	WriteConfToFile(&conf, m_strConfPath);
}

BOOL CLoginDlg::WriteConfToFile(LPLOGINCONF conf, CString strFilePath)
{
	CFile file;
	if(!file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite))
	{
		return FALSE;
	}
	file.Write(conf, sizeof(LOGINCONF));
	file.Close();
	return TRUE;
}
BOOL CLoginDlg::ReadConfFromFile(LPLOGINCONF conf, CString strFilePath)
{
	CFile file;
	if(!file.Open(strFilePath, CFile::modeRead))
	{
		return FALSE;
	}

	if(file.Read(conf, sizeof(LOGINCONF)) <= 0)
	{
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}

void CLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMERID_WAIT_CANCEL_AUTOLOGIN:
		{
			KillTimer(TIMERID_WAIT_CANCEL_AUTOLOGIN);
			if(m_ckbAutoLogin.GetCheck() == BST_CHECKED)
			{
				Login();
			}
		}
		break;
	case TIMERID_WAIT_VERIFY:
		{
			KillTimer(TIMERID_WAIT_VERIFY);
			EndControl();
			MessageBox(_T("无法连接到护士站：护士站未启动或网络错误"),  _T("错误"));
			m_btnLogin.EnableWindow(TRUE);
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CLoginDlg::OnDestroy()
{
	CDialog::OnDestroy();

	//SocketClose();
}

void CLoginDlg::OnBnClickedCheckAutologin()
{
	if(m_ckbAutoLogin.GetCheck() == BST_CHECKED)
	{
		m_ckbPassword.SetCheck(BST_CHECKED);
	}
}

void CLoginDlg::OnRecvLoginResult(LPCMDRESULT pLoginResult, LPSOCKADDR_IN psockddr)
{
	EndControl();
	switch(pLoginResult->eRes)
	{
	case vrPass:
		{
			SetUserId(pLoginResult->sUserInfoFrom.wszUserID);
			SetUserName(pLoginResult->sUserInfoFrom.wszUserName);
			SetOffices(pLoginResult->sUserInfoFrom.wszOffices);
			SetCallSpecial(pLoginResult->bCallSpecial);
			SetNurseSock(psockddr);
			SetNurseId(pLoginResult->header.sUserInfo.wszUserID);

			//((CDoctorCallerApp*)AfxGetApp())->m_strUserID = m_strDoctorID;
			//((CDoctorCallerApp*)AfxGetApp())->m_strUserName = pLoginResult->header.sUserInfo.wszUserName;
			//((CDoctorCallerApp*)AfxGetApp())->m_strNurseServerIP = NetToHostIP(psockddr->sin_addr);
			//((CDoctorCallerApp*)AfxGetApp())->m_iNurseServerPort = NetToHostShort(psockddr->sin_port);
			//((CDoctorCallerApp*)AfxGetApp())->m_bCallSpecial= pLoginResult->bCallSpecial;
			EndDialog(IDOK);
		}
		return;
	case vrNotPass:
		KillTimer(TIMERID_WAIT_VERIFY);
		MessageBox(_T("用户名或密码错误"),  _T("错误"));
		break;
	case vrDBError:
		KillTimer(TIMERID_WAIT_VERIFY);
		MessageBox(_T("服务器错误"),  _T("错误"));
		break;
	case vrLogined:
		KillTimer(TIMERID_WAIT_VERIFY);
		MessageBox(_T("该帐号已登录"),  _T("错误"));
		break;
	default:
		KillTimer(TIMERID_WAIT_VERIFY);
		MessageBox(_T("登录失败，未知错误"),  _T("错误"));
	}
	//KillTimer(TIMERID_WAIT_VERIFY);
	m_btnLogin.EnableWindow(TRUE);
}

//void CLoginDlg::SendVerify()
//{
//	REQLOGIN login;
//	ZeroMemory(&login, sizeof(login));
//	SetSendBaseInfo((LPBASEINFO)&login, ntiReqLogin, sizeof(login));
//	wcscpy_s(login.wszPassword, MAX_PASSWORD_LENGTH, m_strPassword.GetBuffer());
//
//	Broadcast((char*)&login, sizeof(login), m_nNursePort);
//}



void CLoginDlg::OnBnClickedButtonLoginConf()
{
	CDlgDoctorLoginConf dlg;
	dlg.DoModal();
}
