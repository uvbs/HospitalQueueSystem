// PropConnectToHisSet.cpp : 实现文件
//

#include "stdafx.h"
//#include "NurseStation.h"
#include "MyDbPropDlg.h"
#include "../NurseStation/MyCommon.h"
#include "../NurseStation/MyString.h"
#include "InitLocalDbDlg.h"


// CMyDbPropDlg 对话框

IMPLEMENT_DYNAMIC(CMyDbPropDlg, CPropertyPage)

CMyDbPropDlg::CMyDbPropDlg()
	: CPropertyPage(CMyDbPropDlg::IDD)
	, m_strUID(_T(""))
	, m_strPWD(_T(""))
	, m_strHostIP(_T(""))
{

}

CMyDbPropDlg::~CMyDbPropDlg()
{
}

void CMyDbPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UID, m_strUID);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPWD);
	DDX_Text(pDX, IDC_EDIT_HOST, m_strHostIP);
}


BEGIN_MESSAGE_MAP(CMyDbPropDlg, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT_UID, &CMyDbPropDlg::OnEnChangeEditMyaccount)
	ON_EN_CHANGE(IDC_EDIT_PWD, &CMyDbPropDlg::OnEnChangeEditMypass)
	ON_EN_CHANGE(IDC_EDIT_HOST, &CMyDbPropDlg::OnEnChangeEditMyip)
	ON_BN_CLICKED(IDC_BUTTON_INIT_DB, &CMyDbPropDlg::OnBnClickedButtonInitDb)
END_MESSAGE_MAP()


// CMyDbPropDlg 消息处理程序

BOOL CMyDbPropDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//
	m_strUID = MyCommon::GetProfileString(
		_T("conn"), _T("UID"), _T("sa"), _T("\\SYS\\mydb.ini"));
	m_strPWD = MyCommon::GetProfileString(
		_T("conn"), _T("PWD"), _T("123456"), _T("\\SYS\\mydb.ini"));
	m_strHostIP = MyCommon::GetProfileString(
		_T("conn"), _T("SERVER"), _T("127.0.0.1"), _T("\\SYS\\mydb.ini"));

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMyDbPropDlg::OnEnChangeEditMyaccount()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CMyDbPropDlg::OnEnChangeEditMypass()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CMyDbPropDlg::OnEnChangeEditMyip()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

BOOL CMyDbPropDlg::OnApply()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData();

	MyCommon::WriteProfileString(
		_T("conn"), _T("UID"), m_strUID, _T("\\SYS\\mydb.ini"));
	MyCommon::WriteProfileString(
		_T("conn"), _T("SERVER"), m_strHostIP, _T("\\SYS\\mydb.ini"));
	MyCommon::WriteProfileString(
		_T("conn"), _T("PWD"), m_strPWD, _T("\\SYS\\mydb.ini"));

	return CPropertyPage::OnApply();
}

void CMyDbPropDlg::OnBnClickedButtonInitDb()
{
	//CInitLocalDbDlg dlg;
	//dlg.DoModal();
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
		m_strHostIP, m_strUID, m_strPWD, strFilePath);

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

