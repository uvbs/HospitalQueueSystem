// SoundConfDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "ConfDisplayDlg.h"
#include "TextModifyDlg.h"


//extern CMap<int, int, CString, LPCWSTR> g_colorKey_colorCode;
//extern CMap<CString, LPCWSTR, int, int> g_colorCode_colorKey;
//extern CMap<int, int, CString, LPCWSTR> g_varKey_varName;

// CConfDisplayDlg 对话框

IMPLEMENT_DYNAMIC(CConfDisplayDlg, CDialog)

CConfDisplayDlg::CConfDisplayDlg(CString& strConf, CWnd* pParent /*=NULL*/)
	: CDialog(CConfDisplayDlg::IDD, pParent)
	, m_strConf(strConf)
{

}

CConfDisplayDlg::~CConfDisplayDlg()
{

}

void CConfDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEXT_ADD, m_editTextAdd);
	//DDX_Control(pDX, IDC_LIST_STR_CONF, m_lbList);
	DDX_Control(pDX, IDC_LIST_STR_CONF, m_mylbList);
	DDX_Control(pDX, IDC_COMBO_COLOR, m_combColor);
}


BEGIN_MESSAGE_MAP(CConfDisplayDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TEXT, &CConfDisplayDlg::OnBnClickedButtonAddText)
	ON_BN_CLICKED(IDC_BUTTON_QUEUE_ID, &CConfDisplayDlg::OnBnClickedButtonQueueId)
	ON_BN_CLICKED(IDC_BUTTON_PATIENT_NAME, &CConfDisplayDlg::OnBnClickedButtonPatientName)
	ON_BN_CLICKED(IDC_BUTTON_DOCTOR_NAME, &CConfDisplayDlg::OnBnClickedButtonDoctorName)
	ON_BN_CLICKED(IDC_BUTTON_OFFICE_NAME, &CConfDisplayDlg::OnBnClickedButtonOfficeName)
	ON_BN_CLICKED(IDC_BUTTON_CONSULT_NAME, &CConfDisplayDlg::OnBnClickedButtonConsultName)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CConfDisplayDlg::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CConfDisplayDlg::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CConfDisplayDlg::OnBnClickedButtonDelete)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOR, &CConfDisplayDlg::OnCbnSelchangeComboColor)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CConfDisplayDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDOK, &CConfDisplayDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CConfDisplayDlg::OnBnClickedCancel)
	ON_WM_DRAWITEM()
	ON_LBN_SELCHANGE(IDC_LIST_STR_CONF, &CConfDisplayDlg::OnLbnSelchangeListStrConf)
	ON_BN_CLICKED(IDC_BUTTON_PATIENT_GENDER, &CConfDisplayDlg::OnBnClickedButtonPatientGender)
	ON_BN_CLICKED(IDC_BUTTON_PATIENT_AGE, &CConfDisplayDlg::OnBnClickedButtonPatientAge)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CConfDisplayDlg::OnBnClickedButtonModify)
END_MESSAGE_MAP()


BOOL CConfDisplayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_mylbList.SetWindowRgn(0x000000, TRUE);

	int nItem = m_combColor.AddString(_T("红"));
	m_combColor.SetItemData(nItem, COLORREF_RED);
	nItem = m_combColor.AddString(_T("绿"));
	m_combColor.SetItemData(nItem, COLORREF_GREEN);
	nItem = m_combColor.AddString(_T("黄"));
	m_combColor.SetItemData(nItem, COLORREF_YELLOW);

	//CString strDisplay;
	//m_pRset->GetFieldValue(_T("display_call"), strDisplay);

	CArray<CString> arr;
	SplitColorStringToArray(m_strConf, arr);

//	m_mylbList.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL |  
//		LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS, NULL, this, IDC_LIST_STR_CONF);  
	ListView_SetExtendedListViewStyle(m_mylbList.m_hWnd, WS_CHILD | WS_VISIBLE | WS_BORDER 
		| WS_HSCROLL | WS_VSCROLL |  LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS);

	CMap<CString, LPCWSTR, int, int> mapColorCode_ColorKey;
	mapColorCode_ColorKey[COLORCODE_RED] = COLORREF_RED;
	mapColorCode_ColorKey[COLORCODE_GREEN] = COLORREF_GREEN;
	mapColorCode_ColorKey[COLORCODE_YELLOW] = COLORREF_YELLOW;
	for(int i = 0; i < arr.GetCount();)
	{
		CString strColor = arr[i++];
		int nColorKey = mapColorCode_ColorKey[strColor];
		if( nColorKey != COLORREF_RED
			&& nColorKey != COLORREF_GREEN
			&& nColorKey != COLORREF_YELLOW)
		{
			nColorKey = COLORREF_RED;
		}
		if(i < arr.GetCount())
		{
			m_mylbList.AddStringMy(arr[i++], nColorKey);
		}
	}
	m_mylbList.SetCurSel(m_mylbList.GetCount()-1);
	//::SendMessage(m_mylbList.m_hWnd, WM_PAINT, 0, 0);
	OnLbnSelchangeListStrConf();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CConfDisplayDlg 消息处理程序

void CConfDisplayDlg::OnBnClickedButtonAddText()
{
	CString strText;
	m_editTextAdd.GetWindowText(strText);
	m_mylbList.AddStringMy(strText, 
		COLORREF_RED);
}

void CConfDisplayDlg::OnBnClickedButtonQueueId()
{
	m_mylbList.AddStringMy(STRING_QUEUEID_CALL, 
		COLORREF_RED);
}

void CConfDisplayDlg::OnBnClickedButtonPatientName()
{
	m_mylbList.AddStringMy(STRING_PATIENTNAME_CALL, 
		COLORREF_RED);
}

void CConfDisplayDlg::OnBnClickedButtonDoctorName()
{
	m_mylbList.AddStringMy(STRING_DOCTORNAME, 
		COLORREF_RED);
}

void CConfDisplayDlg::OnBnClickedButtonOfficeName()
{
	m_mylbList.AddStringMy(STRING_OFFICENAME, 
		COLORREF_RED);
}

void CConfDisplayDlg::OnBnClickedButtonConsultName()
{
	m_mylbList.AddStringMy(STRING_CONSULTNAME, 
		COLORREF_RED);
}

void CConfDisplayDlg::OnBnClickedButtonUp()
{
	int nItem = m_mylbList.GetCurSel();
	if(nItem <= 0) return;
	CString strText;
	COLORREF iColor;
	m_mylbList.GetTextAndColor(nItem, strText, iColor);
	m_mylbList.DeleteString(nItem);
	m_mylbList.InsertStringMy(--nItem, strText, iColor);
}

void CConfDisplayDlg::OnBnClickedButtonDown()
{
	int nItem = m_mylbList.GetCurSel();
	if(nItem >= m_mylbList.GetCount()-1 || nItem < 0) return;
	CString strText;
	COLORREF iColor;
	m_mylbList.GetTextAndColor(nItem, strText, iColor);
	m_mylbList.DeleteString(nItem);
	m_mylbList.InsertStringMy(++nItem, strText, iColor);


}

void CConfDisplayDlg::OnBnClickedButtonDelete()
{
	m_mylbList.DeleteStringMy(m_mylbList.GetCurSel());
	m_mylbList.SetCurSel(m_mylbList.GetCount()-1);
}

void CConfDisplayDlg::OnCbnSelchangeComboColor()
{
	m_mylbList.SetColor(m_mylbList.GetCurSel(), 
		m_combColor.GetItemData(m_combColor.GetCurSel()));
}

void CConfDisplayDlg::OnLbnSelchangeListStrConf()
{
	COLORREF iColor;
	if(!m_mylbList.GetColor(m_mylbList.GetCurSel(), iColor)) return;
	int nItem = -1;
	for(int i = 0; i < m_combColor.GetCount(); i++)
	{
		if(m_combColor.GetItemData(i) == iColor)
		{
			nItem = i;
			break;
		}
	}
	m_combColor.SetCurSel(nItem);

}

void CConfDisplayDlg::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CConfDisplayDlg::OnBnClickedOk()
{
	CMap<int, int, CString, LPCWSTR> mapColorKey_ColorCode;
	mapColorKey_ColorCode[COLORREF_RED] = COLORCODE_RED;
	mapColorKey_ColorCode[COLORREF_GREEN] = COLORCODE_GREEN;
	mapColorKey_ColorCode[COLORREF_YELLOW] = COLORCODE_YELLOW;

	CString strDisplayCall;
	for(int i = 0; i < m_mylbList.GetCount(); i++)
	{
		CString strText;
		COLORREF iColor;
		m_mylbList.GetTextAndColor(i, strText, iColor);
		strDisplayCall += mapColorKey_ColorCode[iColor];
		strDisplayCall += strText;
	}

	m_strConf = strDisplayCall;

	OnOK();
}

void CConfDisplayDlg::OnBnClickedCancel()
{
	OnCancel();
}

int CConfDisplayDlg::SplitColorStringToArray(CString strSrc, CArray<CString>& arrStr)
{
	int nIndex = -1;
	for(; !strSrc.IsEmpty();)
	{
		CString strTempLeft, strTempMid;
		nIndex = strSrc.Find(_T('#'));
		if(nIndex != -1)
		{
			strTempLeft = strSrc.Left(nIndex);
			if(!strTempLeft.IsEmpty())
			{
				arrStr.Add(strTempLeft);
				strSrc = strSrc.Mid(nIndex);
				continue;
			}
			strTempMid = strSrc.Left(nIndex+1);
			strSrc = strSrc.Mid(nIndex+1);
			nIndex = strSrc.Find(_T('#'));
			if(nIndex != -1)
			{
				strTempMid += strSrc.Left(nIndex+1);
				arrStr.Add(strTempMid);
				strSrc = strSrc.Mid(nIndex+1);
			}
		}
		else
		{
			//arrStr.Add(_T("#0#"));
			arrStr.Add(strSrc);
			break;
		}
	}
	return 0;
}

void CConfDisplayDlg::OnBnClickedButtonPatientGender()
{
	m_mylbList.AddStringMy(STRING_PATIENT_GENDER, 
		COLORREF_RED);
}

void CConfDisplayDlg::OnBnClickedButtonPatientAge()
{
	m_mylbList.AddStringMy(STRING_PATIENT_AGE, 
		COLORREF_RED);
}

void CConfDisplayDlg::OnBnClickedButtonModify()
{
	CString strModify;
	int iIndex = m_mylbList.GetCurSel();
	if(iIndex == -1)
	{
		return;
	}
	m_mylbList.GetTextMy(iIndex, strModify);
	if(strModify.Find('[') != -1)
	{
		return;
	}
	CTextModifyDlg dlg(strModify);
	INT_PTR iRet = dlg.DoModal();
	if(IDOK == iRet)
	{
		m_mylbList.SetText(iIndex, strModify);
	}
}
