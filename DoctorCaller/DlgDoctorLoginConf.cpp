// DlgDoctorLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "DoctorCaller.h"
#include "DlgDoctorLoginConf.h"
#include "../NurseStation/MyCommon.h"
#include "../NurseStation/MyString.h"


// CDlgDoctorLoginConf 对话框

IMPLEMENT_DYNAMIC(CDlgDoctorLoginConf, CDialog)

CDlgDoctorLoginConf::CDlgDoctorLoginConf(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDoctorLoginConf::IDD, pParent)
	, m_strNurseIP(_T(""))
	, m_strNursePort(_T(""))
	, m_nLoginType(0)
{

}

CDlgDoctorLoginConf::~CDlgDoctorLoginConf()
{
}

void CDlgDoctorLoginConf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NURSE_IP, m_strNurseIP);
	DDX_Text(pDX, IDC_EDIT_NURSE_PORT, m_strNursePort);
	DDX_Radio(pDX, IDC_RADIO_LOGIN_TYPE_1, m_nLoginType);
	DDX_Control(pDX, IDC_EDIT_NURSE_IP, m_editNurseIP);
	DDX_Control(pDX, IDC_EDIT_NURSE_PORT, m_editNursePort);
	DDX_Control(pDX, IDC_RADIO_LOGIN_TYPE_2, m_ckLoginType2);
	DDX_Control(pDX, IDC_RADIO_LOGIN_TYPE_1, m_ckLoginType1);
}


BEGIN_MESSAGE_MAP(CDlgDoctorLoginConf, CDialog)
	ON_BN_CLICKED(IDC_RADIO_LOGIN_TYPE_2, &CDlgDoctorLoginConf::OnBnClickedRadioLoginType2)
	ON_BN_CLICKED(IDC_RADIO_LOGIN_TYPE_1, &CDlgDoctorLoginConf::OnBnClickedRadioLoginType1)
END_MESSAGE_MAP()


// CDlgDoctorLoginConf 消息处理程序

BOOL CDlgDoctorLoginConf::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_nLoginType = MyCommon::GetProfileInt(
		_T("sys"), _T("LOGIN_TYPE"), 0, _T("SYS\\port.ini"));
	if(m_nLoginType != 0)
	{
		m_nLoginType = 1;
	}

	m_strNurseIP = MyCommon::GetProfileString(
		_T("sys"), _T("NURSE_IP"), _T("192.168.1.100"), _T("SYS\\port.ini"));
	m_strNursePort = MyCommon::GetProfileString(
		_T("sys"), _T("NURSE_PORT"), _T("4020"), _T("SYS\\port.ini"));

	UpdateData(FALSE);

	if(m_nLoginType == 0)
	{
		m_editNurseIP.EnableWindow(FALSE);
		m_editNursePort.EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDoctorLoginConf::OnOK()
{
	UpdateData();

	MyCommon::WriteProfileString(
		_T("sys"), _T("LOGIN_TYPE"), MyString::Int2Str(m_nLoginType), _T("SYS\\port.ini"));

	MyCommon::WriteProfileString(
		_T("sys"), _T("NURSE_IP"), m_strNurseIP, _T("SYS\\port.ini"));
	MyCommon::WriteProfileString(
		_T("sys"), _T("NURSE_PORT"), m_strNursePort, _T("SYS\\port.ini"));

	CDialog::OnOK();
}

void CDlgDoctorLoginConf::OnBnClickedRadioLoginType2()
{
	if(m_ckLoginType2.GetCheck() == BST_CHECKED)
	{
		m_editNurseIP.EnableWindow(TRUE);
		m_editNursePort.EnableWindow(TRUE);
	}
	else
	{
		m_editNurseIP.EnableWindow(FALSE);
		m_editNursePort.EnableWindow(FALSE);
	}
}

void CDlgDoctorLoginConf::OnBnClickedRadioLoginType1()
{
	if(m_ckLoginType1.GetCheck() == BST_UNCHECKED)
	{
		m_editNurseIP.EnableWindow(TRUE);
		m_editNursePort.EnableWindow(TRUE);
	}
	else
	{
		m_editNurseIP.EnableWindow(FALSE);
		m_editNursePort.EnableWindow(FALSE);
	}
}
