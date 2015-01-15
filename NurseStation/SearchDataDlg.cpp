// SearchDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "SearchDataDlg.h"


// CSearchDataDlg 对话框

IMPLEMENT_DYNAMIC(CSearchDataDlg, CDialog)

CSearchDataDlg::CSearchDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchDataDlg::IDD, pParent)
{

}

CSearchDataDlg::~CSearchDataDlg()
{
}

void CSearchDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEARCH_ITEM, m_search_item);
	DDX_Control(pDX, IDC_COMBO_SEARCHWAY, m_com_searchway);
	DDX_Control(pDX, IDC_DATETIME_PICKER, m_date_picker);
	DDX_Control(pDX, IDC_ED_SEARCHCONTEX, m_ed_searchcontex);
}


BEGIN_MESSAGE_MAP(CSearchDataDlg, CDialog)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIME_PICKER, &CSearchDataDlg::OnDtnDatetimechangeDatetimePicker)
	ON_CBN_SELCHANGE(IDC_COMBO_SEARCHWAY, &CSearchDataDlg::OnCbnSelchangeComboSearchway)
	ON_BN_CLICKED(IDOK, &CSearchDataDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSearchDataDlg 消息处理程序

void CSearchDataDlg::OnDtnDatetimechangeDatetimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_date_picker.GetTime(m_date);
	
	*pResult = 0;
}

BOOL CSearchDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_com_searchway.AddString(_T("按就诊号查询"));
	m_com_searchway.AddString(_T("按患者姓名查询"));
	m_com_searchway.AddString(_T("按医生姓名查询"));
	m_com_searchway.SetCurSel(0);
	m_search_item.SetWindowText(_T("就诊号"));
	m_date=CTime::GetCurrentTime();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSearchDataDlg::OnCbnSelchangeComboSearchway()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_com_searchway.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	if(index==0)
	{
		m_search_item.SetWindowText(_T("就诊号"));
	}
	else if(index==1)
	{
		m_search_item.SetWindowText(_T("患者姓名"));
	}
	else if(index==2)
	{
		m_search_item.SetWindowText(_T("医生姓名"));
	}
}

void CSearchDataDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CString c_date=m_date.Format("%Y-%m-%d");
	CString context=_T("");
	m_search_item.GetWindowText(context);
	int index=m_com_searchway.GetCurSel();
	m_ed_searchcontex.GetWindowText(context);
	pMainFrame->GetOfficeQueView()->SearchData(c_date,context,index);
	OnOK();
}