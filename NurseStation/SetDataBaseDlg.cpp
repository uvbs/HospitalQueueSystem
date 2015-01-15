// SetDataBaseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "SetDataBaseDlg.h"
#include "MyString.h"
#include "MyCommon.h"

// CSetDataBaseDlg 对话框

IMPLEMENT_DYNAMIC(CSetDataBaseDlg, CDialog)

CSetDataBaseDlg::CSetDataBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDataBaseDlg::IDD, pParent)
	, m_set_account(_T(""))
	, m_set_pass(_T(""))
	, m_set_serverip(_T(""))
	//, m_selectbase(0)
{

}

CSetDataBaseDlg::~CSetDataBaseDlg()
{
}

void CSetDataBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ED_SETACCOUNT, m_set_account);
	DDX_Text(pDX, IDC_ED_SETPASS, m_set_pass);
	DDX_Text(pDX, IDC_ED_SETSERVERIP, m_set_serverip);
	//DDX_Radio(pDX, IDC_RADIO_ORACLE, m_selectbase);
}


BEGIN_MESSAGE_MAP(CSetDataBaseDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetDataBaseDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_INIT_DB, &CSetDataBaseDlg::OnBnClickedButtonInitDb)
END_MESSAGE_MAP()


// CSetDataBaseDlg 消息处理程序

BOOL CSetDataBaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_set_account=m_baseconfig.GetServerAcount();
	m_set_pass=m_baseconfig.GetServerPassword();
	m_set_serverip=m_baseconfig.GetServerIP();
	//m_set_database=m_baseconfig.GetDataDrive();
	//if(m_set_database==_T("SQL Server"))
	//{
	//	m_selectbase=1;
	//}
	//if(m_set_database==_T("Oracle"))
	//{
	//	m_selectbase=0;
	//}
	//if(m_set_database==_T("MySQL"))
	//{
	//	m_selectbase=2;
	//}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSetDataBaseDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_baseconfig.SetServerAcount(m_set_account);
	m_baseconfig.SetServerIP(m_set_serverip);
	m_baseconfig.SetServerPassword(m_set_pass);
	//CString name=_T("");
	//if(m_selectbase==0)
	//{
	//	name=_T("Oracle");
	//}
	//else if(m_selectbase==1)
	//{
	//	name=_T("SQL Server");
	//}
	//else if(m_selectbase==2)
	//{
	//	name=_T("MySQL");
	//}
	//m_baseconfig.SetDataDrive(name);
	m_baseconfig.SetDataDrive(_T("SQL Server"));
	m_baseconfig.Save();
	OnOK();
}

void CSetDataBaseDlg::OnBnClickedButtonInitDb()
{
	int nRet = MessageBox(_T("确认初始化数据库吗？"), _T("数据库初始化"), MB_OKCANCEL|MB_ICONWARNING|MB_DEFBUTTON2);
	if(nRet != IDOK) return;

	UpdateData(TRUE);

	CString strFilePath = MyCommon::GetModuleDir();
	strFilePath += _T("db.sql");
	if(!MyCommon::PathFileExist(strFilePath))
	{
		MessageBox(_T("数据库脚本文件db.sql缺失！"), _T("数据库初始化"), MB_ICONERROR);
		return;
	}

	CString strParam;
	strParam.Format(_T("-S %s -U %s -P %s  -i \"%s\""), 
		m_set_serverip, m_set_account, m_set_pass, strFilePath);

	HINSTANCE hInst = ::ShellExecute(NULL, _T("open"), _T("osql"), strParam, NULL, SW_HIDE);

	if((int)hInst <= 32)
	{
		MessageBox(_T("本机缺少 SQL SERVER 客户端组件，无法执行osql命令！"), 
			_T("数据库初始化"), MB_ICONERROR);
		return;
	}

	MessageBox(_T("数据库初始化完成！"), 
		_T("数据库初始化"));
}
