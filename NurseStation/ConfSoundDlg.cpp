// SoundWaitConfDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "ConfSoundDlg.h"
#include "TextModifyDlg.h"

// CConfSoundDlg 对话框

IMPLEMENT_DYNAMIC(CConfSoundDlg, CDialog)

CConfSoundDlg::CConfSoundDlg(CString& strConf, CWnd* pParent /*=NULL*/)
	: CDialog(CConfSoundDlg::IDD, pParent)
	, m_strConf(strConf)
{

}

CConfSoundDlg::~CConfSoundDlg()
{
}

void CConfSoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEXT_ADD, m_editTextAdd);
	DDX_Control(pDX, IDC_LIST_STR_CONF, m_lbList);
}


BEGIN_MESSAGE_MAP(CConfSoundDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TEXT, &CConfSoundDlg::OnBnClickedButtonAddText)
	ON_BN_CLICKED(IDC_BUTTON_QUEUE_ID, &CConfSoundDlg::OnBnClickedButtonQueueId)
	ON_BN_CLICKED(IDC_BUTTON_PATIENT_NAME, &CConfSoundDlg::OnBnClickedButtonPatientName)
	ON_BN_CLICKED(IDC_BUTTON_DOCTOR_NAME, &CConfSoundDlg::OnBnClickedButtonDoctorName)
	ON_BN_CLICKED(IDC_BUTTON_OFFICE_NAME, &CConfSoundDlg::OnBnClickedButtonOfficeName)
	ON_BN_CLICKED(IDC_BUTTON_CONSULT_NAME, &CConfSoundDlg::OnBnClickedButtonConsultName)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CConfSoundDlg::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CConfSoundDlg::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CConfSoundDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CConfSoundDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDOK, &CConfSoundDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CConfSoundDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CConfSoundDlg::OnBnClickedButtonModify)
END_MESSAGE_MAP()

BOOL CConfSoundDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//CString strSound;
	//m_pRset->GetFieldValue(_T("sound_wait"), strSound);
	CArray<CString> arrStr;
	SplitStringToArray(m_strConf, arrStr);
	for(int i = 0; i < arrStr.GetCount(); i++)
	{
		m_lbList.AddString(arrStr[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CConfSoundDlg 消息处理程序

void CConfSoundDlg::OnBnClickedButtonAddText()
{
	CString strText;
	m_editTextAdd.GetWindowText(strText);
	int nItem = m_lbList.AddString(strText);
	m_lbList.SetCurSel(nItem);
}

//extern CMap<int, int, CString, LPCWSTR> g_varKey_varName;


void CConfSoundDlg::OnBnClickedButtonQueueId()
{
	int nItem = m_lbList.AddString(STRING_QUEUEID_CALL);
	m_lbList.SetCurSel(nItem);
}

void CConfSoundDlg::OnBnClickedButtonPatientName()
{
	int nItem = m_lbList.AddString(STRING_PATIENTNAME_CALL);
	m_lbList.SetCurSel(nItem);
}

void CConfSoundDlg::OnBnClickedButtonDoctorName()
{
	int nItem = m_lbList.AddString(STRING_DOCTORNAME);
	m_lbList.SetCurSel(nItem);
}

void CConfSoundDlg::OnBnClickedButtonOfficeName()
{
	int nItem = m_lbList.AddString(STRING_OFFICENAME);
	m_lbList.SetCurSel(nItem);
}

void CConfSoundDlg::OnBnClickedButtonConsultName()
{
	int nItem = m_lbList.AddString(STRING_CONSULTNAME);
	m_lbList.SetCurSel(nItem);
}

void CConfSoundDlg::OnBnClickedButtonUp()
{
	int nItem = m_lbList.GetCurSel();
	if(nItem <= 0) return;
	CString strText;
	m_lbList.GetText(nItem, strText);
	m_lbList.DeleteString(nItem);
	m_lbList.InsertString(--nItem, strText);
	m_lbList.SetCurSel(nItem);
}

void CConfSoundDlg::OnBnClickedButtonDown()
{
	int nItem = m_lbList.GetCurSel();
	if(nItem >= m_lbList.GetCount()-1 || nItem < 0) return;
	CString strText;
	m_lbList.GetText(nItem, strText);
	m_lbList.DeleteString(nItem);
	m_lbList.InsertString(++nItem, strText);
	m_lbList.SetCurSel(nItem);
}

void CConfSoundDlg::OnBnClickedButtonDelete()
{
	m_lbList.DeleteString(m_lbList.GetCurSel());
	m_lbList.SetCurSel(m_lbList.GetCount()-1);
}

void CConfSoundDlg::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CConfSoundDlg::OnBnClickedOk()
{
	CString strAll;
	for(int i = 0; i < m_lbList.GetCount(); i++)
	{
		CString strLine;
		m_lbList.GetText(i, strLine);
		strAll += strLine;
	}

	m_strConf = strAll;

	OnOK();
}

void CConfSoundDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

int CConfSoundDlg::SplitStringToArray(CString strSrc, CArray<CString>& arrStr)
{
	//CString strTemp(strSrc);
	int nIndex = -1;
	//for(; !strSrc.IsEmpty();)
	for(; strSrc.GetLength() != 0;)
	{
		CString strTempLeft, strTempMid;
		nIndex = strSrc.Find(_T('['));
		if(nIndex != -1)
		{
			strTempLeft = strSrc.Left(nIndex);
			if(!strTempLeft.IsEmpty())
			{
				arrStr.Add(strTempLeft);
			}
			strSrc = strSrc.Mid(nIndex);
			nIndex = strSrc.Find(_T(']'));
			if(nIndex != -1)
			{
				strTempMid = strSrc.Left(nIndex+1);
				arrStr.Add(strTempMid);
				strSrc = strSrc.Mid(nIndex+1);
			}
		}
		else
		{
			arrStr.Add(strSrc);
			break;
		}
	}
	return 0;
}



void CConfSoundDlg::OnBnClickedButtonModify()
{
	CString strModify;
	int iIndex = m_lbList.GetCurSel();
	if(iIndex == -1)
	{
		return;
	}
	m_lbList.GetText(iIndex, strModify);
	if(strModify.Find('[') != -1)
	{
		return;
	}
	CTextModifyDlg dlg(strModify);
	INT_PTR iRet = dlg.DoModal();
	if(IDOK == iRet)
	{
		//strModify = dlg.m_strText;
		m_lbList.DeleteString(iIndex);
		m_lbList.InsertString(iIndex, strModify);
	}
}
