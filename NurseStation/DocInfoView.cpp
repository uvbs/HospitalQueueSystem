// DocInfoView.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "DocInfoView.h"


// CDocInfoView

IMPLEMENT_DYNCREATE(CDocInfoView, CFormView)

CDocInfoView::CDocInfoView()
	: CFormView(CDocInfoView::IDD)
{

}

CDocInfoView::~CDocInfoView()
{
	
}

void CDocInfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEW_DOCINFO, m_view_docinfo);
}

BEGIN_MESSAGE_MAP(CDocInfoView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_VIEW_DOCINFO, &CDocInfoView::OnNMRClickViewDocinfo)
	ON_COMMAND(ID_DOCTOR_CALL, &CDocInfoView::OnDoctorCall)
	ON_COMMAND(ID_DOCTOR_RECALL, &CDocInfoView::OnDoctorRecall)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_VIEW_DOCINFO, &CDocInfoView::OnLvnItemchangedViewDocinfo)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VIEW_DOCINFO, &CDocInfoView::OnNMCustomdrawViewDocinfo)
	ON_COMMAND(ID_DOCTOR_WAIT, &CDocInfoView::OnDoctorWait)
END_MESSAGE_MAP()


// CDocInfoView 诊断

#ifdef _DEBUG
void CDocInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDocInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDocInfoView 消息处理程序


void CDocInfoView::ShowList()
{
	if(!m_view_docinfo.m_hWnd)return;

	
	GetWindowRect(&m_rect); // 获取当前客户区view大小
	ScreenToClient(&m_rect);

	CSize size;
	size.cx = m_rect.right - m_rect.left;
	size.cy = m_rect.bottom - m_rect.top;
	SetScrollSizes(MM_TEXT, size); // 将CScrollView的大小设置为当前客户区大小
	m_view_docinfo.MoveWindow(&m_rect, TRUE);
}


void CDocInfoView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	ShowList();
	// TODO: 在此处添加消息处理程序代码
}



void CDocInfoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	
	ListView_SetExtendedListViewStyle(m_view_docinfo.m_hWnd, LVS_EX_FULLROWSELECT |  LVS_EX_GRIDLINES);
//	GetClientRect(m_rect);
	GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rect.right-=2;
	
	m_view_docinfo.InsertColumn(0,_T("状态"),LVCFMT_CENTER,m_rect.Width()*20/100,-1);
	m_view_docinfo.InsertColumn(1,_T("医生姓名"),LVCFMT_CENTER,m_rect.Width()*40/100,-1);
	m_view_docinfo.InsertColumn(2,_T("就诊患者"),LVCFMT_CENTER,m_rect.Width()*40/100,-1);
//	m_view_docinfo.SetHeaderBKColor(255,255,255,0);
	
	DWORD color=GetSysColor(COLOR_BTNFACE);
//	m_view_docinfo.SetHeaderBKColor()
//	ReadData();
}

void CDocInfoView::FlushDoctorData(USERSTATUS& userstatus)
{
	CString officeID(userstatus.strOfficeID);
	if(officeID!=_T(""))
	{
		if(userstatus.eUserStatus == usOutLine)
		{
			m_listUserStatus.RemoveKey(userstatus.strUserID);
		}
		else
		{
			m_listUserStatus[userstatus.strUserID]=userstatus;
		}
		
		ShowAllInlineDoctorInfo();
		//ShowDoctorData(userstatus);
	}
}

void CDocInfoView::ShowDoctorData(USERSTATUS& userstatus)
{
	int count=m_view_docinfo.GetItemCount();
	CString status;
	for(int i=0;i<count;i++)
	{
		if(userstatus.strUserID==m_itemToDocID[i])
		{
			if(userstatus.eUserStatus == dsWorking)
			{
				status=docStaOnlineText;
			}
			else if(userstatus.eUserStatus == dsPause)
			{
				status=docStaSuspendText;
			}
			//else if(userstatus.eUserStatus == usOutLine)
			//{
			//	status=docStaOutlineText;
			//}
			m_view_docinfo.SetItemText(i,0,status);
			m_view_docinfo.SetItemText(i,2,userstatus.strPatientName);
			break;
		}
	}
}

void CDocInfoView::OnNMRClickViewDocinfo(NMHDR* pNMHDR,LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int item = pNMItemActivate->iItem;
	CMenu   menu;   //定义下面要用到的cmenu对象
	menu.LoadMenu(IDR_DOCTOR_MENU); //装载自定义的右键菜单 
	CMenu* pContextMenu=menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单
	CString status=m_view_docinfo.GetItemText(m_item,0);
	if(status==docStaOutlineText || status==docStaSuspendText)
	{
		pContextMenu->EnableMenuItem(ID_DOCTOR_CALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_DOCTOR_RECALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_DOCTOR_WAIT,MF_GRAYED);
	}
	CPoint point;//定义一个用于确定光标位置的位置  
	GetCursorPos(&point);//获取当前光标的位置，以便使得菜单可以跟随光标
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this->GetParent()); //在指定位置显示弹出菜单
	pContextMenu->DestroyMenu();
	*pResult = 0;
}
void CDocInfoView::OnDoctorCall()
{
	CString status=m_view_docinfo.GetItemText(m_item,0);
	if(status==docStaOnlineText)
	{
		CString docid=m_itemToDocID[m_item];
		if(docid!=_T(""))
		{
			//theApp.m_pNurseWnd->m_pChatDlg->SendCmd(docid, _T(""), dcCmdCall);
			((CNurseStationApp*)AfxGetApp())->GetController()->SendCmd(docid, _T(""), dcCmdCall);
		}
	}
}

void CDocInfoView::OnDoctorRecall()
{
	// TODO: 在此添加命令处理程序代码
	CString status=m_view_docinfo.GetItemText(m_item,0);
	if(status==docStaOnlineText)
	{
		CString docid=m_itemToDocID[m_item];
		if(docid!=_T(""))
		{
			((CNurseStationApp*)AfxGetApp())->GetController()->SendCmd(docid, _T(""), dcCmdRecall);
		}
	}
}

void CDocInfoView::ShowAllInlineDoctorInfo()
{
	//m_mutexDoc.Lock();
	int item=0;
	CString status;
	CString key;
	USERSTATUS userstatu;
	m_view_docinfo.DeleteAllItems();
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	POSITION pos=m_listUserStatus.GetStartPosition();
	while(pos)
	{
		m_listUserStatus.GetNextAssoc(pos,key,userstatu);
		if(userstatu.eUserStatus == dsWorking)
		{
			status=docStaOnlineText;
		}
		else if(userstatu.eUserStatus == dsPause)
		{
			status=docStaSuspendText;
		}
		//else if(userstatu.eUserStatus == usOutLine)
		//{
		//	status=docStaOutlineText;
		//}
		if(userstatu.strOfficeID==pMainFrame->GetOfficeQueView()->GetOfficeID())
		{
			m_view_docinfo.InsertItem(item,status);
			m_view_docinfo.SetItemText(item,1,userstatu.strUserName);
			m_view_docinfo.SetItemText(item,2,userstatu.strPatientName);
			m_itemToDocID[item]=key;
			item++;
		}
	}
	//m_mutexDoc.Unlock();
}
void CDocInfoView::OnLvnItemchangedViewDocinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_item=pNMLV->iItem;
	*pResult = 0;
}

void CDocInfoView::OnNMCustomdrawViewDocinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	*pResult = CDRF_DODEFAULT;
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{   
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{           
		COLORREF clrNewTextColor, clrNewBkColor;
		int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		POSITION pos = m_view_docinfo.GetFirstSelectedItemPosition();
		int index = m_view_docinfo.GetNextSelectedItem(pos);
		if (index == nItem)//如果要刷新的项为当前选择的项，则将文字设为白色，背景色设为蓝色
		{             
			clrNewTextColor = RGB(255,255,255);//Set the text to white 
			clrNewBkColor = RGB(49,106,197);        //Set the background color to blue 
		}else
		{
			clrNewTextColor = RGB(0,0,0);        // the text black
			clrNewBkColor = RGB(255,255,255);    //leave the background color white
		}
		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor; 
		*pResult = CDRF_DODEFAULT; 
	}
}

void CDocInfoView::OnDoctorWait()
{
	// TODO: 在此添加命令处理程序代码
	CString status=m_view_docinfo.GetItemText(m_item,0);
	if(status==docStaOnlineText)
	{
		CString docid=m_itemToDocID[m_item];
		if(docid!=_T(""))
		{
			((CNurseStationApp*)AfxGetApp())->GetController()->SendCmd(docid, _T(""), dcCmdWait);
		}
	}
}
