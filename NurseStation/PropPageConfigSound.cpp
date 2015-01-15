// PropPageConfigSound.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PropPageConfigSound.h"
#include "MyString.h"
#include "MyCommon.h"

// CPropPageConfigSound 对话框

IMPLEMENT_DYNAMIC(CPropPageConfigSound, CPropertyPage)

CPropPageConfigSound::CPropPageConfigSound()
	: CPropertyPage(CPropPageConfigSound::IDD)
{
	m_strFile = 
		MyCommon::GetModuleDir() + 
		_T("sys\\sound.ini");
}

CPropPageConfigSound::~CPropPageConfigSound()
{
}

void CPropPageConfigSound::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TIMES, m_strTimes);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_slSpeed);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_slVolume);
}


BEGIN_MESSAGE_MAP(CPropPageConfigSound, CPropertyPage)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SPEED, &CPropPageConfigSound::OnNMReleasedcaptureSliderSpeed)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_VOLUME, &CPropPageConfigSound::OnNMReleasedcaptureSliderVolume)
	ON_EN_CHANGE(IDC_EDIT_TIMES, &CPropPageConfigSound::OnEnChangeEditTimes)
END_MESSAGE_MAP()


void CPropPageConfigSound::Load()
{
	m_strTimes = 
		MyCommon::GetProfileString(
		_T("sys"),_T("SOUND_REPLAY_TIMES"), _T("1"), 
		m_strFile);
	m_slSpeed.SetRange(0, 9);
	m_slSpeed.SetPos(
		MyCommon::GetProfileInt(
		_T("sys"),_T("SOUND_SPEED"), 5, m_strFile)
		);

	m_slVolume.SetRange(0, 9);
	m_slVolume.SetPos(
		MyCommon::GetProfileInt(
		_T("sys"),_T("SOUND_VOLUME"), 5, m_strFile)
		);

	UpdateData(FALSE);
}

void CPropPageConfigSound::Save()
{
	UpdateData();

	MyCommon::WriteProfileString(
		_T("sys"), _T("SOUND_REPLAY_TIMES"), m_strTimes, 
		m_strFile);
	if(m_strTimes.IsEmpty() || 
		MyString::Str2Int(m_strTimes) < 0)
	{
		m_strTimes = _T("0");
	}
	MyCommon::WriteProfileString(
		_T("sys"),_T("SOUND_SPEED"), 
		MyString::Int2Str(m_slSpeed.GetPos()), 
		m_strFile);

	MyCommon::WriteProfileString(
		_T("sys"),_T("SOUND_VOLUME"),
		MyString::Int2Str(m_slVolume.GetPos()), 
		m_strFile);

	((CNurseStationApp*)AfxGetApp())->GetController()->ReLoadConf();
}


// CPropPageConfigSound 消息处理程序

BOOL CPropPageConfigSound::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	Load();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CPropPageConfigSound::OnApply()
{
	Save();

	return CPropertyPage::OnApply();
}


void CPropPageConfigSound::OnNMReleasedcaptureSliderSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetModified();
	*pResult = 0;
}

void CPropPageConfigSound::OnNMReleasedcaptureSliderVolume(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetModified();
	*pResult = 0;
}

void CPropPageConfigSound::OnEnChangeEditTimes()
{
	SetModified();
}
