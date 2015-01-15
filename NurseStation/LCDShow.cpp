// LCDShow.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "LCDShow.h"


// CLCDShow 对话框

IMPLEMENT_DYNAMIC(CLCDShow, CDialog)

CLCDShow::CLCDShow(CWnd* pParent /*=NULL*/)
	: CDialog(CLCDShow::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SHOW);
}

CLCDShow::~CLCDShow()
{
}

void CLCDShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_WAITROOM, m_com_waitroom);
	DDX_Control(pDX, IDC_ED_CONTENT, m_ed_content);
	DDX_Control(pDX, IDC_RETURN, m_send_return);
	DDX_Control(pDX, IDC_COM_TYPEMSG, m_com_typemsg);
}


BEGIN_MESSAGE_MAP(CLCDShow, CDialog)
	ON_BN_CLICKED(ID_SENDTO_SCREEN, &CLCDShow::OnBnClickedSendtoScreen)
	ON_EN_CHANGE(IDC_ED_CONTENT, &CLCDShow::OnEnChangeEdContent)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, &CLCDShow::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLCDShow 消息处理程序

BOOL CLCDShow::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);

	OpenMyDataBse();

	m_hSendReturn=::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE("IDR_MAINFRAME"));

	SearchWaitRoom();
	
	AttachToComboxTopBox();
	
	AttachToComboxType();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CLCDShow::OpenMyDataBse()
{
	if(m_read.OpenMyDataBase())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CLCDShow::SearchWaitRoom()
{
	int count=m_com_waitroom.GetCount();
	for(int index=count;index>=0;index--)
	{
		m_com_waitroom.DeleteString(index);
	}
	m_mapto_topbox.RemoveAll();
	CString sql=_T("select * from settopbox where stb_type=1 or stb_type=2");
	
	if(m_read.IsMyDataBaseOpen())
	{
		if(m_read.m_connectToMyDataBase.m_pMyRecordset->Open(sql,CADORecordset::openQuery))
		{
			if(!m_read.m_connectToMyDataBase.m_pMyRecordset->IsBOF())
			{
				TOPBOXINFO topboxinfo;
				topboxinfo.stb_name=_T("所有液晶");
				topboxinfo.stb_id=_T("0");
				m_mapto_topbox[m_com_waitroom.AddString(topboxinfo.stb_name)] = topboxinfo;

				while(!m_read.m_connectToMyDataBase.m_pMyRecordset->IsEOF())
				{
					m_read.m_connectToMyDataBase.m_pMyRecordset->
						GetFieldValue(_T("stb_name"),topboxinfo.stb_name);
					
					m_read.m_connectToMyDataBase.m_pMyRecordset->
						GetFieldValue(_T("stb_id"),topboxinfo.stb_id);
					m_mapto_topbox[m_com_waitroom.AddString(topboxinfo.stb_name)]=topboxinfo;
					m_read.m_connectToMyDataBase.m_pMyRecordset
						->MoveNext();
				}
			}
			m_read.m_connectToMyDataBase.m_pMyRecordset->Close();
		}
	}
}
void CLCDShow::OnBnClickedSendtoScreen()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame=(CMainFrame*)theApp.m_pNurseWnd;
	int index=m_com_waitroom.GetCurSel();
	CString stb_id,editcontent;
	m_ed_content.GetWindowText(editcontent);
	if(index!=CB_ERR)
	{
		TOPBOXINFO topboxinfo=m_mapto_topbox[index];
		stb_id=topboxinfo.stb_id;

		int typemsg=m_com_typemsg.GetCurSel();
		if(typemsg!=CB_ERR)
		{
			BOOL flag=FALSE;
			switch(typemsg)
			{
			case 0://紧急通知
				//flag=((CNurseStationApp*)AfxGetApp())->GetController()->StbUpdateInstantMsg(editcontent,stb_id);
				flag=((CNurseStationApp*)AfxGetApp())->GetController()->AddVoiceDisplay(editcontent,stb_id);
				break;
			case 1://公告
				flag=((CNurseStationApp*)AfxGetApp())->GetController()->StbUpdateNoticeHtml(editcontent,stb_id);
				break;
			case 2://设置标题
				flag=((CNurseStationApp*)AfxGetApp())->GetController()->StbUpdateTitleHtml(editcontent,stb_id);
				break;
			default:
				break;
			}

			if(flag)
			{
				m_send_return.SetWindowText(_T("发送成功"));
			}
			else
			{
				m_send_return.SetWindowText(_T("发送失败"));
			}
		}
	}
}

void CLCDShow::OnEnChangeEdContent()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	m_send_return.SetWindowText(_T(""));
	// TODO:  在此添加控件通知处理程序代码
}

BOOL CLCDShow::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_hSendReturn)
	{
		if(::TranslateAccelerator(m_hWnd,m_hSendReturn,pMsg))
		{
			OnBnClickedSendtoScreen();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CLCDShow::AttachToComboxTopBox()
{
//	if(theApp.m_pNurseWnd->GetPatientView()->m_view_paient.getse
	CString logid=theApp.m_pNurseWnd->GetPatientView()->GetSelectPaitLogid();
	CString sql=_T("");
	sql.Format(_T("select w.stb_id,q.log_id from queue q,waitingroom w where q.room_id=w.room_id and q.log_id='%s'"),
		logid);
	CString patient_stb_id;
	if(m_read.IsMyDataBaseOpen())
	{
		if(m_read.m_connectToMyDataBase.m_pMyRecordset->Open(sql,CADORecordset::openQuery))
		{
			if(!m_read.m_connectToMyDataBase.m_pMyRecordset->IsBOF())
			{
				m_read.m_connectToMyDataBase.m_pMyRecordset->GetFieldValue(_T("stb_id"),patient_stb_id);
			}
		}
	}
	
	int count=m_com_waitroom.GetCount();
	for(int i=0;i<count;i++)
	{
		TOPBOXINFO topboxinfo=m_mapto_topbox[i];
		if(topboxinfo.stb_id == patient_stb_id &&
			topboxinfo.stb_id != _T(""))
		{
			m_com_waitroom.SetCurSel(i);
			break;
		}
	}
}

void CLCDShow::AttachToComboxType()
{
//	m_map_msgtype[m_com_typemsg.AddString(_T("紧急通知"))]=_T("紧急通知");
//	m_map_msgtype[m_com_typemsg.AddString(_T("公告"))]=_T("公告");
	m_com_typemsg.AddString(_T("发送紧急通知"));
	m_com_typemsg.AddString(_T("发送公告/广告"));
	m_com_typemsg.AddString(_T("设置标题"));

	m_com_typemsg.SetCurSel(0);
}
void CLCDShow::OnDestroy()
{
	CDialog::OnDestroy();
	
	CMainFrame* pMainFrame=(CMainFrame*)theApp.m_pNurseWnd;
	if(pMainFrame->m_pLCDShowDlg)
	{
		delete pMainFrame->m_pLCDShowDlg;
		pMainFrame->m_pLCDShowDlg = NULL;
	}
	// TODO: 在此处添加消息处理程序代码
}

void CLCDShow::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
	OnDestroy();
}
