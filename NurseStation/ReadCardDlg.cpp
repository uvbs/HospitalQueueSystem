// ReadCardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "ReadCardDlg.h"
#include "CommonConvert.h"
#include "com/SLZCWndScreen.h"


// CReadCardDlg 对话框

IMPLEMENT_DYNAMIC(CReadCardDlg, CDialog)

CReadCardDlg::CReadCardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReadCardDlg::IDD, pParent)
	, m_ed_wndaddress(_T(""))
	, m_cs_msg(_T(""))
{
	m_pComInit = CComInit::GetInstance();
}

CReadCardDlg::~CReadCardDlg()
{
	
}

void CReadCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMNUM, m_com_comnum);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_com_baudrate);
	//	DDX_Control(pDX, IDC_ED_CONTEX, m_test_countex);
	DDX_Control(pDX, IDC_COM_WNDSCREEN, m_com_wndscrren);
	DDX_Text(pDX, IDC_ED_SENDMSG, m_ed_wndaddress);
	DDX_Text(pDX, IDC_ED_SENMSG, m_cs_msg);
}


BEGIN_MESSAGE_MAP(CReadCardDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CReadCardDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_COMNUM, &CReadCardDlg::OnCbnSelchangeComboComnum)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &CReadCardDlg::OnCbnSelchangeComboBaudrate)
	ON_CBN_SELCHANGE(IDC_COM_WNDSCREEN, &CReadCardDlg::OnCbnSelchangeComWndscreen)
	ON_BN_CLICKED(IDC_BN_SEND, &CReadCardDlg::OnBnClickedBnSend)
END_MESSAGE_MAP()


// CReadCardDlg 消息处理程序

BOOL CReadCardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	/*
	m_com_comnum.AddString(_T("0"));
	m_com_comnum.AddString(_T("1"));
	m_com_comnum.AddString(_T("2"));
	m_com_comnum.AddString(_T("3"));
	m_com_comnum.AddString(_T("4"));
	
	m_com_wndscrren.AddString(_T("0"));
	m_com_wndscrren.AddString(_T("1"));
	m_com_wndscrren.AddString(_T("2"));
	m_com_wndscrren.AddString(_T("3"));
	m_com_wndscrren.AddString(_T("4"));
		


	m_com_baudrate.AddString(_T("110"));
	m_com_baudrate.AddString(_T("300"));
	m_com_baudrate.AddString(_T("600"));
	m_com_baudrate.AddString(_T("1200"));
	m_com_baudrate.AddString(_T("2400"));
	m_com_baudrate.AddString(_T("4800"));
	m_com_baudrate.AddString(_T("9600"));
	m_com_baudrate.AddString(_T("14400"));
	m_com_baudrate.AddString(_T("19200"));
	m_com_baudrate.AddString(_T("38400"));
	m_com_baudrate.AddString(_T("57600"));
	m_com_baudrate.AddString(_T("115200"));
	m_com_baudrate.AddString(_T("128000"));
	m_com_baudrate.AddString(_T("256000"));
	m_com_baudrate.SetCurSel(6);

	CString strCom=m_baseconfig.GetComNum();
	if(strCom!=_T(""))
	{
		int index;
		m_pool.CStringToint(index,strCom);
		m_com_comnum.SetCurSel(index);
		m_selectCom=index;
	}
	else
	{
		m_selectCom=0;
	}
	
	m_selectBaudRate=m_com_baudrate.GetCurSel();

	CString strWndCom=m_baseconfig.GetWndComNum();
	if(strWndCom!=_T(""))
	{
		int index;
		m_pool.CStringToint(index,strWndCom);
		m_com_wndscrren.SetCurSel(index);
		m_selectWndCom=index;
		CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
		if(pMainFrame->m_hComWndScreen==INVALID_HANDLE_VALUE)
		{
			//MessageBox(_T("窗口屏串口打开失败"),_T("注意"),MB_OK|MB_ICONINFORMATION);
		}
	}
	*/
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

void CReadCardDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
// 	CString str=_T("");
// 	m_pool.intToCString(m_selectBaudRate,str);	
// 	m_baseconfig.SetBaudRate(str);
// 	m_baseconfig.SaveComData();
	m_pComInit->SaveComm();
	OnOK();
}

void CReadCardDlg::OnCbnSelchangeComboComnum()
{
	// TODO: 在此添加控件通知处理程序代码
// 	int sel=m_com_comnum.GetCurSel();
// 	if(sel==CB_ERR)return;
// 	CString com=_T("");
// 	m_com_comnum.GetLBText(sel,com);
// 	
// 	m_pool.CStringToint(m_selectCom,com);
// 
// 	m_selectBaudRate=m_com_baudrate.GetCurSel();
// 
// 	if(InitReadCard(m_selectCom,m_selectBaudRate)!=1)
// 	{
// 		MessageBox(_T("打开刷卡串口失败!"),_T("注意"),MB_OK|MB_ICONINFORMATION);
// 	}
// 	
// 	m_baseconfig.SetComNum(com);
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

	if(m_pComInit->OpenCardComm(i_readCardCom) == -1)
	{
		MessageBox(_T("刷卡串口打开失败或被占用"),_T("注意"),MB_OK|MB_ICONINFORMATION);
	}
}
/*
long CReadCardDlg::InitReadCard(const int ncom,const int baudRate)
{
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;;
	if(ncom<0) return 2;
	if(pMainFrame->m_count!=0)
	{
		if(pMainFrame->m_hComReadCard!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(pMainFrame->m_hComReadCard);
			pMainFrame->m_hComReadCard=INVALID_HANDLE_VALUE;
		//		return 3;
		}
	}

	CString strcom;
	strcom.Format(_T("COM%d"),ncom);
	HANDLE hCom=CreateFile(strcom,GENERIC_READ | GENERIC_WRITE,0,
		NULL,OPEN_EXISTING,0,NULL);//打开串口
	if(INVALID_HANDLE_VALUE==hCom)
	{
		CloseHandle(hCom);
		return 4;
	}
	DCB dcb={0};
	dcb.DCBlength=sizeof(dcb);
	BOOL bres=GetCommState(hCom,&dcb);
	if(!bres)
	{
		CloseHandle(hCom);
		hCom=INVALID_HANDLE_VALUE;
		return 9;
	}
	switch (baudRate)
	{
	case 0:
		dcb.BaudRate=CBR_110;
		break;
	case 1:
		dcb.BaudRate=CBR_300;
		break;
	case 2:
		dcb.BaudRate=CBR_600;
		break;
	case 3:
		dcb.BaudRate=CBR_1200;
		break;
	case 4:
		dcb.BaudRate=CBR_2400;
		break;
	case 5:
		dcb.BaudRate=CBR_4800;
		break;
	case 6:
		dcb.BaudRate=CBR_9600;
		break;
	case 7:
		dcb.BaudRate=CBR_14400;
		break;
	case 8:
		dcb.BaudRate=CBR_19200;
		break;
	case 9:
		dcb.BaudRate=CBR_38400;
		break;
	case 10:
		dcb.BaudRate=CBR_57600;
		break;
	case 11:
		dcb.BaudRate=CBR_115200;
		break;
	case 12:
		dcb.BaudRate=CBR_128000;
		break;
	case 13:
		dcb.BaudRate=CBR_256000;
		break;
	}
	dcb.ByteSize=8;//后改
	dcb.fParity=0;

	bres=SetCommState(hCom,&dcb);
	if(!bres)
	{
		CloseHandle(hCom);
		hCom=INVALID_HANDLE_VALUE;
		return 5;
	}
	COMMTIMEOUTS comm_time={0};
	comm_time.ReadIntervalTimeout=MAXDWORD;
	bres=SetCommTimeouts(hCom,&comm_time);
	if(!bres)
	{
		CloseHandle(hCom);
		return 6;
	}
	pMainFrame->m_hComReadCard=hCom;
	(pMainFrame->m_count)++;
	//	if(m_hThreadCard)return 6;
	return 1;
}
*/
void CReadCardDlg::OnCbnSelchangeComboBaudrate()
{
	// TODO: 在此添加控件通知处理程序代码
//	m_selectBaudRate=m_com_baudrate.GetCurSel();
}

void CReadCardDlg::OnCbnSelchangeComWndscreen()
{
	// TODO: 在此添加控件通知处理程序代码
// 	int index=m_com_wndscrren.GetCurSel();
// 	if(index==CB_ERR)
// 	{
// 		return;
// 	}
// 	CString com=_T("");
// 	m_com_comnum.GetLBText(index,com);
// 
// 	m_pool.CStringToint(m_selectWndCom,com);
// 	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
// 	if(pMainFrame->InitWndScreen(m_selectWndCom)!=1)
// 	{
// 		MessageBox(_T("打开窗口屏串口失败!"),_T("注意"),MB_OK|MB_ICONINFORMATION);
// 	}
// 	theApp.m_pNurseWnd->m_WndCom=m_selectWndCom;
// 	m_baseconfig.SetWndComNum(com);
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

	if(m_pComInit->OpenWndScreen(i_wndsScreenCom) == -1)
	{
		MessageBox(_T("呼叫器串口打开失败或被占用"),_T("注意"),MB_OK|MB_ICONINFORMATION);
	}
}

void CReadCardDlg::OnBnClickedBnSend()
{
	// TODO: 在此添加控件通知处理程序代码
// 	UpdateData();
// 	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
// 	int address=0;
// 	m_pool.CStringToint(address,m_ed_wndaddress);
// 	m_cs_msg=pMainFrame->m_doWndScreen.FlushCstringToFitWndScreen(m_cs_msg);
// 	pMainFrame->SendMsgToWndScreen(address,m_cs_msg);
	UpdateData();
	int address = 0;
	m_pool.CStringToint(address,m_ed_wndaddress);
	SLZCWndScreen* pWndScreen = SLZCWndScreen::GetInstance();
	pWndScreen->AddScreenMsg(m_cs_msg,address);
}
