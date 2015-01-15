// PropConnectToHisSet.cpp : 实现文件
//

#include "stdafx.h"
//#include "NurseStation.h"
//#include "MyDbPropDlg.h"
#include "../NurseStation/MyCommon.h"
#include "../NurseStation/MyString.h"
#include "PropConnectToHisDbDlg.h"


// CPropConnectToHisDbDlg 对话框

IMPLEMENT_DYNAMIC(CPropConnectToHisDbDlg, CPropertyPage)

CPropConnectToHisDbDlg::CPropConnectToHisDbDlg()
	: CPropertyPage(CPropConnectToHisDbDlg::IDD)
	, m_strUID(_T(""))
	, m_strPWD(_T(""))
	, m_strHostIP(_T(""))
	, m_strPort(_T(""))
	, m_strServiceName(_T(""))
	, m_strDataDays(_T(""))
{

}

CPropConnectToHisDbDlg::~CPropConnectToHisDbDlg()
{
}

void CPropConnectToHisDbDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UID, m_strUID);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPWD);
	DDX_Text(pDX, IDC_EDIT_HOST, m_strHostIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_strPort);
	DDX_Text(pDX, IDC_EDIT_SERVICE_NAME, m_strServiceName);
	DDX_Text(pDX, IDC_EDIT_DATA_DAYS, m_strDataDays);
}


BEGIN_MESSAGE_MAP(CPropConnectToHisDbDlg, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT_UID, &CPropConnectToHisDbDlg::OnEnChangeEditMyaccount)
	ON_EN_CHANGE(IDC_EDIT_PWD, &CPropConnectToHisDbDlg::OnEnChangeEditMypass)
	ON_EN_CHANGE(IDC_EDIT_HOST, &CPropConnectToHisDbDlg::OnEnChangeEditMyip)
	ON_EN_CHANGE(IDC_EDIT_PORT, &CPropConnectToHisDbDlg::OnEnChangeEditPort)
	ON_EN_CHANGE(IDC_EDIT_SERVICE_NAME, &CPropConnectToHisDbDlg::OnEnChangeEditServiceName)
	ON_EN_CHANGE(IDC_EDIT_DATA_DAYS, &CPropConnectToHisDbDlg::OnEnChangeEditDataDays)
END_MESSAGE_MAP()


// CPropConnectToHisDbDlg 消息处理程序

BOOL CPropConnectToHisDbDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//
	m_strUID = MyCommon::GetProfileString(
		_T("conn"), _T("USER_ID"), _T("hisforbjlb"), _T("\\SYS\\hisdb.ini"));
	m_strPWD = MyCommon::GetProfileString(
		_T("conn"), _T("PASSWORD"), _T("bjlb2007"), _T("\\SYS\\hisdb.ini"));
	m_strHostIP = MyCommon::GetProfileString(
		_T("conn"), _T("HOST"), _T("192.168.1.100"), _T("\\SYS\\hisdb.ini"));
	m_strPort = MyCommon::GetProfileString(
		_T("conn"), _T("PORT"), _T("1521"), _T("\\SYS\\hisdb.ini"));
	m_strServiceName = MyCommon::GetProfileString(
		_T("conn"), _T("SERVICE_NAME"), _T("orcl"), _T("\\SYS\\hisdb.ini"));
	m_strDataDays = MyCommon::GetProfileString(
		_T("data"), _T("DATA_DAYS"), _T("0"), _T("\\SYS\\hisdb.ini"));

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPropConnectToHisDbDlg::OnEnChangeEditMyaccount()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CPropConnectToHisDbDlg::OnEnChangeEditMypass()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CPropConnectToHisDbDlg::OnEnChangeEditMyip()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CPropConnectToHisDbDlg::OnEnChangeEditPort()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	SetModified();
}

void CPropConnectToHisDbDlg::OnEnChangeEditServiceName()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	SetModified();
}

void CPropConnectToHisDbDlg::OnEnChangeEditDataDays()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	SetModified();
}

BOOL CPropConnectToHisDbDlg::OnApply()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData();

	MyCommon::WriteProfileString(
		_T("conn"), _T("USER_ID"), m_strUID, _T("\\SYS\\hisdb.ini"));
	MyCommon::WriteProfileString(
		_T("conn"), _T("PASSWORD"), m_strPWD, _T("\\SYS\\hisdb.ini"));
	MyCommon::WriteProfileString(
		_T("conn"), _T("HOST"), m_strHostIP, _T("\\SYS\\hisdb.ini"));
	MyCommon::WriteProfileString(
		_T("conn"), _T("PORT"), m_strPort, _T("\\SYS\\hisdb.ini"));
	MyCommon::WriteProfileString(
		_T("conn"), _T("SERVICE_NAME"), m_strServiceName, _T("\\SYS\\hisdb.ini"));
	MyCommon::WriteProfileString(
		_T("data"), _T("DATA_DAYS"), m_strDataDays, _T("\\SYS\\hisdb.ini"));

	return CPropertyPage::OnApply();
}

