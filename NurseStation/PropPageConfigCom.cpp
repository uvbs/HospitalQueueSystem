// PropPageConfigCom.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PropPageConfigCom.h"
#include "CommonConvert.h"
#include "com/SLZCWndScreen.h"

// CPropPageConfigCom 对话框

IMPLEMENT_DYNAMIC(CPropPageConfigCom, CPropertyPage)

CPropPageConfigCom::CPropPageConfigCom()
	: CPropertyPage(CPropPageConfigCom::IDD)
	, m_ed_wndaddress(_T(""))
	, m_cs_msg(_T(""))
{
	m_pComInit = CComInit::GetInstance();
}

CPropPageConfigCom::~CPropPageConfigCom()
{
}

void CPropPageConfigCom::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMNUM, m_com_comnum);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_com_baudrate);
	//	DDX_Control(pDX, IDC_ED_CONTEX, m_test_countex);
	DDX_Control(pDX, IDC_COM_WNDSCREEN, m_com_wndscrren);
	DDX_Text(pDX, IDC_ED_SENDMSG, m_ed_wndaddress);
	DDX_Text(pDX, IDC_ED_SENMSG, m_cs_msg);
}


BEGIN_MESSAGE_MAP(CPropPageConfigCom, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_COMNUM, &CPropPageConfigCom::OnCbnSelchangeComboComnum)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &CPropPageConfigCom::OnCbnSelchangeComboBaudrate)
	ON_CBN_SELCHANGE(IDC_COM_WNDSCREEN, &CPropPageConfigCom::OnCbnSelchangeComWndscreen)
	ON_BN_CLICKED(IDC_BN_SEND, &CPropPageConfigCom::OnBnClickedBnSend)
END_MESSAGE_MAP()


// CPropPageConfigCom 消息处理程序

BOOL CPropPageConfigCom::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CString readCardCom = m_pComInit->GetCardComm();
	CString callerCom = m_pComInit->GetWndComm();

	m_com_comnum.AddString(_T("0"));
	m_com_wndscrren.AddString(_T("0"));

	for(int i=0;i<10;i++)
	{
		if(m_pComInit->m_canUse[i]>0)
		{
			CString comm;
			comm.Format(_T("%d"),m_pComInit->m_canUse[i]);
			m_com_comnum.AddString(comm);
			m_com_wndscrren.AddString(comm);
		}
	}
	////////////////////////////////////////////
	for(int i=0;i<m_com_wndscrren.GetCount();i++)
	{
		CString content;
		m_com_wndscrren.GetLBText(i,content);
		if(callerCom == content)
		{
			m_com_wndscrren.SetCurSel(i);
			break;
		}
	}
	///////////////////////////////////////
	for(int i=0;i<m_com_comnum.GetCount();i++)
	{
		CString content;
		m_com_comnum.GetLBText(i,content);
		if(readCardCom == content)
		{
			m_com_comnum.SetCurSel(i);
			break;
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropPageConfigCom::OnCbnSelchangeComboComnum()
{
	CCommonConvert convert;
	int index=m_com_comnum.GetCurSel();
	if(index == CB_ERR)
	{
		return;
	}
	CString readCardCom=_T("");
	m_com_comnum.GetLBText(index,readCardCom);
	int i_readCardCom=0;
	convert.CStringToint(i_readCardCom,readCardCom);

	SetModified();

	if(m_pComInit->OpenCardComm(i_readCardCom) == -1)
	{
		MessageBox(_T("刷卡串口打开失败或被占用"),_T("注意"),MB_OK|MB_ICONINFORMATION);
	}
}

void CPropPageConfigCom::OnCbnSelchangeComboBaudrate()
{
	SetModified();
}

void CPropPageConfigCom::OnCbnSelchangeComWndscreen()
{
	CCommonConvert convert;
	int index=m_com_wndscrren.GetCurSel();
	if(index == CB_ERR)
	{
		return;
	}
	CString wndScreenCom=_T("");
	m_com_wndscrren.GetLBText(index,wndScreenCom);
	int i_wndsScreenCom=0;
	convert.CStringToint(i_wndsScreenCom,wndScreenCom);

	SetModified();

	if(m_pComInit->OpenWndScreen(i_wndsScreenCom) == -1)
	{
		MessageBox(_T("呼叫器串口打开失败或被占用"),_T("注意"),MB_OK|MB_ICONINFORMATION);
	}
}

void CPropPageConfigCom::OnBnClickedBnSend()
{
	UpdateData();
	int address = 0;
	m_pool.CStringToint(address,m_ed_wndaddress);
	SLZCWndScreen* pWndScreen = SLZCWndScreen::GetInstance();
	pWndScreen->AddScreenMsg(m_cs_msg,address);
}

BOOL CPropPageConfigCom::OnApply()
{
	// 	CString str=_T("");
	// 	m_pool.intToCString(m_selectBaudRate,str);	
	// 	m_baseconfig.SetBaudRate(str);
	// 	m_baseconfig.SaveComData();
	m_pComInit->SaveComm();

	return CPropertyPage::OnApply();
}
