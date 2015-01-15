// DoctorCallConfDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "DoctorCallConfDlg.h"
#include "ConfSoundDlg.h"
#include "ConfDisplayDlg.h"
#include "MyString.h"
#include "MyCommon.h"


// CDoctorCallConfDlg 对话框

IMPLEMENT_DYNAMIC(CDoctorCallConfDlg, CDialog)

CDoctorCallConfDlg::CDoctorCallConfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDoctorCallConfDlg::IDD, pParent)
	, m_strRecallTimes(_T(""))
	, m_nRadioConfType(0)
	, m_strMaxWaitNum(_T(""))
{
	CDoctorManageDlg* pPWnd = (CDoctorManageDlg*)m_pParentWnd;

	m_strSoundCall = pPWnd->m_strSoundCall;
	m_strSoundWait = pPWnd->m_strSoundWait;
	m_strDisplayCall = pPWnd->m_strDisplayCall;
	m_strDisplayWait = pPWnd->m_strDisplayWait;
	m_strRecallTimes = MyString::Int2Str(pPWnd->m_iRecallTimes);
	//m_ckCallSpecial.SetCheck(pPWnd->m_bCallSpecial ? BST_CHECKED : BST_UNCHECKED);
	m_bCallSpecial = pPWnd->m_bCallSpecial;
	m_bWaitWhenCall = pPWnd->m_bWaitWhenCall;
	m_strMaxWaitNum = MyString::Int2Str(pPWnd->m_nWaitNum);
}

CDoctorCallConfDlg::~CDoctorCallConfDlg()
{
}

void CDoctorCallConfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RECALL_TIMES, m_strRecallTimes);
	DDX_Control(pDX, IDC_CHECK_CALLSPECIAL, m_ckCallSpecial);
	//DDX_Control(pDX, IDC_CHECK_CONF_ALL, m_ckConfAll);
	DDX_Radio(pDX, IDC_RADIO_CUR_DOC, m_nRadioConfType);
	DDX_Control(pDX, IDC_CHECK_WAIT_WHEN_CALL, m_ckWaitWhenCall);
	DDX_Text(pDX, IDC_EDIT_MAX_WAIT_NUM, m_strMaxWaitNum);
}


BEGIN_MESSAGE_MAP(CDoctorCallConfDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SET_SOUND_CALL, &CDoctorCallConfDlg::OnBnClickedButtonSetSoundCall)
	ON_BN_CLICKED(IDC_BUTTON_SET_SOUND_WAIT, &CDoctorCallConfDlg::OnBnClickedButtonSetSoundWait)
	ON_BN_CLICKED(IDC_BUTTON_SET_DISPLAY_CALL, &CDoctorCallConfDlg::OnBnClickedButtonSetDisplayCall)
	ON_BN_CLICKED(IDC_BUTTON_SET_DISPLAY_WAIT, &CDoctorCallConfDlg::OnBnClickedButtonSetDisplayWait)
	ON_BN_CLICKED(IDOK, &CDoctorCallConfDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDoctorCallConfDlg 消息处理程序

BOOL CDoctorCallConfDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ckCallSpecial.SetCheck(m_bCallSpecial ? BST_CHECKED : BST_UNCHECKED);
	m_ckWaitWhenCall.SetCheck(m_bWaitWhenCall ? BST_CHECKED : BST_UNCHECKED);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDoctorCallConfDlg::OnBnClickedButtonSetSoundCall()
{
	CConfSoundDlg dlg(m_strSoundCall);
	dlg.DoModal();
}

void CDoctorCallConfDlg::OnBnClickedButtonSetSoundWait()
{
	CConfSoundDlg dlg(m_strSoundWait);
	dlg.DoModal();
}

void CDoctorCallConfDlg::OnBnClickedButtonSetDisplayCall()
{
	CConfDisplayDlg dlg(m_strDisplayCall);
	dlg.DoModal();
}

void CDoctorCallConfDlg::OnBnClickedButtonSetDisplayWait()
{
	CConfDisplayDlg dlg(m_strDisplayWait);
	dlg.DoModal();
}

void CDoctorCallConfDlg::OnBnClickedOk()
{
	CDoctorManageDlg* pPWnd = (CDoctorManageDlg*)m_pParentWnd;

	UpdateData();

	pPWnd->m_strSoundCall = m_strSoundCall;
	pPWnd->m_strSoundWait = m_strSoundWait;
	pPWnd->m_strDisplayCall = m_strDisplayCall;
	pPWnd->m_strDisplayWait = m_strDisplayWait;
	pPWnd->m_iRecallTimes = MyString::Str2Int(m_strRecallTimes);
	pPWnd->m_bCallSpecial = m_ckCallSpecial.GetCheck() == BST_CHECKED;
	//pPWnd->m_nConfType = m_ckConfAll.GetCheck() == BST_CHECKED;
	pPWnd->m_bWaitWhenCall = m_ckWaitWhenCall.GetCheck() == BST_CHECKED;
	pPWnd->m_nWaitNum = MyString::Str2Int(m_strMaxWaitNum);

	OnOK();
}


