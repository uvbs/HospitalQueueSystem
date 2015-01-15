// PropPageConfigComp.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PropPageConfigComp.h"
#include "MyString.h"
#include "MyCommon.h"


// CPropPageConfigComp 对话框

IMPLEMENT_DYNAMIC(CPropPageConfigComp, CPropertyPage)

CPropPageConfigComp::CPropPageConfigComp()
	: CPropertyPage(CPropPageConfigComp::IDD)
	, m_strTimeCall(_T(""))
{
	m_strFile = 
		MyCommon::GetModuleDir() + 
		_T("sys\\sysconfig.ini");
}

CPropPageConfigComp::~CPropPageConfigComp()
{
}

void CPropPageConfigComp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DISABLE_TAKING, m_nRadioDisableTakingNum);
	DDX_Text(pDX, IDC_EDIT_REFRESH_TIME, m_strRefreshTime);
	DDX_Text(pDX, IDC_EDIT_DATA_DAYS, m_strDataDays);
	DDX_Radio(pDX, IDC_RADIO_READ_IN, m_nReadIn);
	DDX_Text(pDX, IDC_EDIT_TIME_CALL, m_strTimeCall);
}


BEGIN_MESSAGE_MAP(CPropPageConfigComp, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT_REFRESH_TIME, &CPropPageConfigComp::OnEnChangeEditRefreshTime)
	ON_EN_CHANGE(IDC_EDIT_DATA_DAYS, &CPropPageConfigComp::OnEnChangeEditDataDays)
	ON_BN_CLICKED(IDC_RADIO_READ_IN, &CPropPageConfigComp::OnBnClickedRadioReadIn)
	ON_BN_CLICKED(IDC_RADIO_NOT_READ_IN, &CPropPageConfigComp::OnBnClickedRadioNotReadIn)
	ON_BN_CLICKED(IDC_RADIO_DISABLE_TAKING, &CPropPageConfigComp::OnBnClickedRadioDisableTaking)
	ON_BN_CLICKED(IDC_RADIO_ENABLE_TAKING, &CPropPageConfigComp::OnBnClickedRadioEnableTaking)
	ON_EN_CHANGE(IDC_EDIT_TIME_CALL, &CPropPageConfigComp::OnEnChangeEditTimeCall)
END_MESSAGE_MAP()


void CPropPageConfigComp::Load()
{
	m_nRadioDisableTakingNum = MyCommon::GetProfileInt(
		_T("sys"), _T("ENABLE_TAKING_NUM"), 0, m_strFile);

	m_strRefreshTime = MyCommon::GetProfileString(
		_T("time"),_T("FLUSHTIMESEC"),_T("30"), m_strFile);

	m_strDataDays = MyCommon::GetProfileString(
		_T("sys"), _T("DATA_DAYS"), _T("0"), m_strFile);

	m_nReadIn = MyCommon::GetProfileInt(
		_T("sys"), _T("READ_INPATIENT"), 0, m_strFile);

	m_strTimeCall = MyCommon::GetProfileString(
		_T("sys"), _T("CALL_SECONDS"), _T("0"), m_strFile);

	UpdateData(FALSE);
}

void CPropPageConfigComp::Save()
{
	UpdateData();

	MyCommon::WriteProfileString(
		_T("sys"), _T("ENABLE_TAKING_NUM"), 
		MyString::Int2Str(m_nRadioDisableTakingNum),
		m_strFile);
	((CNurseStationApp*)AfxGetApp())->m_pNurseView
		->EnableTakingNum(m_nRadioDisableTakingNum == 1 ? TRUE : FALSE);

	int nRefreshTime = MyString::Str2Int(m_strRefreshTime);
	if(MyString::Str2Int(m_strRefreshTime) < 0)
	{
		nRefreshTime = 0;
	}
	MyCommon::WriteProfileString(
		_T("time"),_T("FLUSHTIMESEC"), m_strRefreshTime, m_strFile);
	theApp.m_pNurseWnd->m_fushsec=nRefreshTime;
	theApp.m_pNurseWnd->m_originFlushTime=nRefreshTime;

	int nDataDays = MyString::Str2Int(m_strDataDays);
	if(nDataDays < 0)
	{
		nDataDays = 0;
		m_strDataDays = _T("0");
	}
	MyCommon::WriteProfileString(
		_T("sys"), _T("DATA_DAYS"), m_strDataDays, m_strFile);

	MyCommon::WriteProfileString(
		_T("sys"), _T("READ_INPATIENT"), 
		MyString::Int2Str(m_nReadIn), m_strFile);

	MyCommon::WriteProfileString(
		_T("sys"), _T("CALL_SECONDS"), 
		m_strTimeCall, m_strFile);

	((CNurseStationApp*)AfxGetApp())->GetController()->ReLoadConf();
}

// CPropPageConfigComp 消息处理程序


BOOL CPropPageConfigComp::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	Load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CPropPageConfigComp::OnApply()
{
	Save();

	return CPropertyPage::OnApply();
}

void CPropPageConfigComp::OnEnChangeEditRefreshTime()
{
	SetModified();
}

void CPropPageConfigComp::OnEnChangeEditDataDays()
{
	SetModified();
}

void CPropPageConfigComp::OnBnClickedRadioReadIn()
{
	SetModified();
}

void CPropPageConfigComp::OnBnClickedRadioNotReadIn()
{
	SetModified();
}

void CPropPageConfigComp::OnBnClickedRadioDisableTaking()
{
	SetModified();
}

void CPropPageConfigComp::OnBnClickedRadioEnableTaking()
{
	SetModified();
}

void CPropPageConfigComp::OnEnChangeEditTimeCall()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	SetModified();
}
