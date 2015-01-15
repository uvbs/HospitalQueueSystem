// PropPageConfigDisplay.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PropPageConfigDisplay.h"
#include "MyString.h"
#include "MyCommon.h"

// CPropPageConfigDisplay 对话框

IMPLEMENT_DYNAMIC(CPropPageConfigDisplay, CPropertyPage)

CPropPageConfigDisplay::CPropPageConfigDisplay()
	: CPropertyPage(CPropPageConfigDisplay::IDD)
	, m_nShowType(0)
{
	m_strKeshiFile = 
		MyCommon::GetModuleDir() + 
		_T("sys\\stb_keshi.ini");
	m_strZhenshiFile = 
		MyCommon::GetModuleDir() + 
		_T("sys\\stb_zhenshi.ini");
}

CPropPageConfigDisplay::~CPropPageConfigDisplay()
{
}

void CPropPageConfigDisplay::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ITEMCOUNT, m_strPageItemCount);
	DDX_Text(pDX, IDC_EDIT_SWITCH_MINUTE, m_strSwitchPageTime);
	DDX_Control(pDX, IDC_CHECK_SHOW_NUM, m_ckShowQueueNum);
	DDX_Control(pDX, IDC_CHECK_SWITCH_PAGE, m_ckSwitchPage);
	DDX_Text(pDX, IDC_EDIT_PAGE_COL_COUNT, m_strPageColCount);
	DDX_Control(pDX, IDC_CHECK_SHOW_LINE_NUM, m_ckShowLineNum);
	DDX_Text(pDX, IDC_EDIT_CONSULT_STB_TITLE, m_strConsultStbTitle);
	DDX_Control(pDX, IDC_CHECK_CONSULT_SHOW_NUM, m_ckConsultShowNum);
	DDX_Text(pDX, IDC_EDIT_SEPARATOR, m_strSeparator);
	DDX_Control(pDX, IDC_CHECK_SHOW_SUM, m_ckShowSum);
	DDX_Radio(pDX, IDC_RADIO_SHOW_TYPE, m_nShowType);
}


BEGIN_MESSAGE_MAP(CPropPageConfigDisplay, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_SHOW_LINE_NUM, &CPropPageConfigDisplay::OnBnClickedCheckShowLineNum)
	ON_BN_CLICKED(IDC_CHECK_SWITCH_PAGE, &CPropPageConfigDisplay::OnBnClickedCheckSwitchPage)
	ON_BN_CLICKED(IDC_CHECK_SHOW_NUM, &CPropPageConfigDisplay::OnBnClickedCheckShowNum)
	ON_EN_CHANGE(IDC_EDIT_PAGE_COL_COUNT, &CPropPageConfigDisplay::OnEnChangeEditPageColCount)
	ON_EN_CHANGE(IDC_EDIT_ITEMCOUNT, &CPropPageConfigDisplay::OnEnChangeEditItemcount)
	ON_EN_CHANGE(IDC_EDIT_SWITCH_MINUTE, &CPropPageConfigDisplay::OnEnChangeEditSwitchMinute)
	ON_EN_CHANGE(IDC_EDIT_CONSULT_STB_TITLE, &CPropPageConfigDisplay::OnEnChangeEditConsultStbTitle)
	ON_BN_CLICKED(IDC_CHECK_CONSULT_SHOW_NUM, &CPropPageConfigDisplay::OnBnClickedCheckConsultShowNum)
	ON_EN_CHANGE(IDC_EDIT_SEPARATOR, &CPropPageConfigDisplay::OnEnChangeEditSeparator)
	ON_BN_CLICKED(IDC_CHECK_SHOW_SUM, &CPropPageConfigDisplay::OnBnClickedCheckShowSum)
	ON_BN_CLICKED(IDC_RADIO_SHOW_TYPE, &CPropPageConfigDisplay::OnBnClickedRadioShowType)
	ON_BN_CLICKED(IDC_RADIO_SHOW_TYPE2, &CPropPageConfigDisplay::OnBnClickedRadioShowType2)
	ON_BN_CLICKED(IDC_RADIO_SHOW_TYPE3, &CPropPageConfigDisplay::OnBnClickedRadioShowType3)
	ON_BN_CLICKED(IDC_RADIO_SHOW_TYPE4, &CPropPageConfigDisplay::OnBnClickedRadioShowType4)
END_MESSAGE_MAP()


void CPropPageConfigDisplay::Load()
{
	m_nShowType = MyCommon::GetProfileInt(
		_T("sys"), _T("SHOW_TYPE"), 0, m_strKeshiFile);
	BOOL bCheck;
	bCheck = MyCommon::GetProfileInt(
		_T("sys"), _T("SHOW_QUEUE_NUM"), 1, m_strKeshiFile);
	if(bCheck)
	{
		m_ckShowQueueNum.SetCheck(BST_CHECKED);
	}
	else
	{
		m_ckShowQueueNum.SetCheck(BST_UNCHECKED);
	}

	bCheck = MyCommon::GetProfileInt(
		_T("sys"), _T("SWITCH_PAGE"), 1, m_strKeshiFile);
	if(bCheck)
	{
		m_ckSwitchPage.SetCheck(BST_CHECKED);
	}
	else
	{
		m_ckSwitchPage.SetCheck(BST_UNCHECKED);
	}

	bCheck = MyCommon::GetProfileInt(
		_T("sys"), _T("SHOW_LINE_NUM"), 1, m_strKeshiFile);
	if(bCheck)
	{
		m_ckShowLineNum.SetCheck(BST_CHECKED);
	}
	else
	{
		m_ckShowLineNum.SetCheck(BST_UNCHECKED);
	}

	bCheck = MyCommon::GetProfileInt(
		_T("sys"), _T("SHOW_SUM"), 1, m_strKeshiFile);
	if(bCheck)
	{
		m_ckShowSum.SetCheck(BST_CHECKED);
	}
	else
	{
		m_ckShowSum.SetCheck(BST_UNCHECKED);
	}

	m_strPageItemCount = 
		MyCommon::GetProfileString(
		_T("sys"),_T("PAGE_ITEM_COUNT"), _T("8"), m_strKeshiFile);
	m_strSwitchPageTime = 
		MyCommon::GetProfileString(
		_T("sys"),_T("PAGE_SWITCH_TIME"), _T("8"), m_strKeshiFile);
	m_strPageColCount = 
		MyCommon::GetProfileString(
		_T("sys"), _T("PAGE_COL_COUNT"), _T("3"), m_strKeshiFile);

	//诊室
	bCheck = 
		MyCommon::GetProfileInt(
		_T("sys"), _T("CONSULT_SHOW_QUEUE_NUM"), 1, 
		m_strZhenshiFile);
	if(bCheck)
	{
		m_ckConsultShowNum.SetCheck(BST_CHECKED);
	}
	else
	{
		m_ckConsultShowNum.SetCheck(BST_UNCHECKED);
	}

	m_strConsultStbTitle = 
		MyCommon::GetProfileString(
		_T("sys"), _T("CONSULT_STB_TITLE"), _T(""), 
		m_strZhenshiFile);

	m_strSeparator = 
		MyCommon::GetProfileString(
		_T("sys"), _T("CONSULT_WAIT_SEPARATOR"), _T(""), 
		m_strZhenshiFile);

	UpdateData(FALSE);
}

void CPropPageConfigDisplay::Save()
{
	UpdateData();

	MyCommon::WriteProfileString(
		_T("sys"), _T("SHOW_TYPE"), 
		MyString::Int2Str(m_nShowType),
		m_strKeshiFile);

	MyCommon::WriteProfileString(
		_T("sys"), _T("SHOW_QUEUE_NUM"), 
		m_ckShowQueueNum.GetCheck() == BST_CHECKED ? _T("1"): _T("0"),
		m_strKeshiFile);

	MyCommon::WriteProfileString(
		_T("sys"), _T("SWITCH_PAGE"), 
		m_ckSwitchPage.GetCheck() == BST_CHECKED ? _T("1"): _T("0"),
		m_strKeshiFile);

	MyCommon::WriteProfileString(
		_T("sys"), _T("SHOW_LINE_NUM"), 
		m_ckShowLineNum.GetCheck() == BST_CHECKED ? _T("1"): _T("0"),
		m_strKeshiFile);

	MyCommon::WriteProfileString(
		_T("sys"), _T("SHOW_SUM"), 
		m_ckShowSum.GetCheck() == BST_CHECKED ? _T("1"): _T("0"),
		m_strKeshiFile);

	if(m_strPageItemCount.IsEmpty() || 
		MyString::Str2Int(m_strPageItemCount) < 1)
	{
		m_strPageItemCount = _T("8");
	}
	MyCommon::WriteProfileString(
		_T("sys"), _T("PAGE_ITEM_COUNT"), 
		m_strPageItemCount, m_strKeshiFile);

	if(m_strSwitchPageTime.IsEmpty() || 
		MyString::Str2Int(m_strSwitchPageTime) < 1)
	{
		m_strSwitchPageTime = _T("8");
	}
	MyCommon::WriteProfileString(
		_T("sys"), _T("PAGE_SWITCH_TIME"), 
		m_strSwitchPageTime, m_strKeshiFile);

	if(m_strPageColCount.IsEmpty() || 
		MyString::Str2Int(m_strPageColCount) < 1)
	{
		m_strPageColCount = _T("3");
	}
	MyCommon::WriteProfileString(
		_T("sys"), _T("PAGE_COL_COUNT"), 
		m_strPageColCount, m_strKeshiFile);

	//诊室
	int nConsultShowQueueNum;
	if(m_ckShowLineNum.GetCheck() == BST_CHECKED)
	{
		nConsultShowQueueNum = 1;
	}
	else
	{
		nConsultShowQueueNum = 0;
	}

	MyCommon::WriteProfileString(
		_T("sys"), _T("CONSULT_SHOW_QUEUE_NUM"), 
		MyString::Int2Str(nConsultShowQueueNum),
		m_strZhenshiFile);

	MyCommon::WriteProfileString(
		_T("sys"), _T("CONSULT_STB_TITLE"), 
		m_strConsultStbTitle, m_strZhenshiFile);

	MyCommon::WriteProfileString(
		_T("sys"), _T("CONSULT_WAIT_SEPARATOR"), 
		m_strSeparator, m_strZhenshiFile);

	((CNurseStationApp*)AfxGetApp())->GetController()->ReLoadConf();
}


// CPropPageConfigDisplay 消息处理程序

BOOL CPropPageConfigDisplay::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	Load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CPropPageConfigDisplay::OnApply()
{
	Save();

	return CPropertyPage::OnApply();
}

void CPropPageConfigDisplay::OnBnClickedCheckShowLineNum()
{
	SetModified();
}

void CPropPageConfigDisplay::OnBnClickedCheckSwitchPage()
{
	SetModified();
}

void CPropPageConfigDisplay::OnBnClickedCheckShowNum()
{
	SetModified();
}

void CPropPageConfigDisplay::OnEnChangeEditPageColCount()
{
	SetModified();
}

void CPropPageConfigDisplay::OnEnChangeEditItemcount()
{
	SetModified();
}

void CPropPageConfigDisplay::OnEnChangeEditSwitchMinute()
{
	SetModified();
}

void CPropPageConfigDisplay::OnEnChangeEditConsultStbTitle()
{
	SetModified();
}

void CPropPageConfigDisplay::OnBnClickedCheckConsultShowNum()
{
	SetModified();
}

void CPropPageConfigDisplay::OnEnChangeEditSeparator()
{
	SetModified();
}

void CPropPageConfigDisplay::OnBnClickedCheckShowSum()
{
	SetModified();
}

void CPropPageConfigDisplay::OnBnClickedRadioShowType()
{
	SetModified();
}

void CPropPageConfigDisplay::OnBnClickedRadioShowType2()
{
	SetModified();
}

void CPropPageConfigDisplay::OnBnClickedRadioShowType3()
{
	SetModified();
}

void CPropPageConfigDisplay::OnBnClickedRadioShowType4()
{
	SetModified();
}
