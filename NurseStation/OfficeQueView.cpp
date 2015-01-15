// OfficeQueView.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "OfficeQueView.h"
#include "MyString.h"
#include "MyCommon.h"
#include "WriteLog.h"

// COfficeQueView

IMPLEMENT_DYNCREATE(COfficeQueView, CFormView)

COfficeQueView::COfficeQueView()
	: CFormView(COfficeQueView::IDD)
{
	
}

COfficeQueView::~COfficeQueView()
{

}

void COfficeQueView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OFFICEQUE, m_view_officeque);
}

BEGIN_MESSAGE_MAP(COfficeQueView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_OFFICEQUE, &COfficeQueView::OnLvnItemchangedListOfficeque)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OFFICEQUE, &COfficeQueView::OnNMCustomdrawListOfficeque)
END_MESSAGE_MAP()


// COfficeQueView 诊断

#ifdef _DEBUG
void COfficeQueView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COfficeQueView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// COfficeQueView 消息处理程序

void COfficeQueView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	ShowList();
//	m_view_officeque.MoveWindow(&m_Rect);
	// TODO: 在此处添加消息处理程序代码
}

void COfficeQueView::ShowList()
{
	if(!m_view_officeque.m_hWnd)return;

	CRect rect;
//	GetClientRect(&rect); // 获取当前客户区view大小
	GetWindowRect(&rect);
	ScreenToClient(&rect);

	CSize size;
	size.cx = rect.right - rect.left;
	size.cy = rect.bottom - rect.top;
	SetScrollSizes(MM_TEXT, size); // 将CScrollView的大小设置为当前客户区大小
	m_view_officeque.MoveWindow(&rect, TRUE);
}

void COfficeQueView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	ListView_SetExtendedListViewStyle(m_view_officeque.m_hWnd, LVS_EX_FULLROWSELECT |  LVS_EX_GRIDLINES);
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	rect.right-=2;
	m_view_officeque.InsertColumn(0,_T("序号"),LVCFMT_CENTER,rect.Width()/5,-1);
	m_view_officeque.InsertColumn(1,_T("科室名称"),LVCFMT_CENTER,2*rect.Width()/5,-1);
	m_view_officeque.InsertColumn(2,_T("科室ID"),LVCFMT_CENTER,2*rect.Width()/5,-1);
	
	ReadData();

//	GetNurseContrlOfficeID();
}

void COfficeQueView::ReadData()
{
	CString name=_T("");
	CString officeid=_T("");
	CString LoginNurseID=theApp.GetLoginID();
	CAccount account;
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;

	try
	{
		CADODatabase db;
		db.SetConnectionString(pMainFrame->m_strConn);
		db.Open();
		CADORecordset *pre=new CADORecordset(&db);
		int item=0;
		CString sql=_T("");
		sql.Format(_T("select o.name,o.office_id from Nurse_Office n,Office o where Nurse_id='%s' and n.office_id=o.office_id order by o.name"),
			LoginNurseID);
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
			{
				while(!pre->IsEOF())
				{
					pre->GetFieldValue(_T("name"),name);
					pre->GetFieldValue(_T("office_id"),officeid);
					CString str=_T("");
					m_pool.intToCString(item+1,str);
					m_view_officeque.InsertItem(item,str);
					m_view_officeque.SetItemText(item,1,name);
					m_view_officeque.SetItemText(item,2,officeid);
					item++;
					pre->MoveNext();
				}
			}
		}
		delete pre;
		pre=NULL;
		db.Close();
	}
	catch(_com_error &e)
	{
		WriteLog::WriteDbErrLog(_T("COfficeQueView::ReadData"));
	}
}



BOOL COfficeQueView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CFormView::PreCreateWindow(cs);
}

void COfficeQueView::OnLvnItemchangedListOfficeque(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_item=pNMLV->iItem;
	m_officeID=m_view_officeque.GetItemText(m_item,2);

	CMainFrame* MainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	SearchData(m_officeID);//显示本科室排队患者
	MainFrame->GetDocInfoView()->ShowAllInlineDoctorInfo();//显示本科室在线医生
	
	MainFrame->SearchDoNum();
	MainFrame->GetNurseView()->SerchDoctor();
	m_officename = m_view_officeque.GetItemText(m_item,1);
	*pResult = 0;
}


void COfficeQueView::OnNMCustomdrawListOfficeque(NMHDR *pNMHDR, LRESULT *pResult)
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
		POSITION pos = m_view_officeque.GetFirstSelectedItemPosition();
		int index = m_view_officeque.GetNextSelectedItem(pos);
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

BOOL COfficeQueView::SearchData(CString officeid)
{
	
	CString sql=_T("");
	CMainFrame* MainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_CHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_UNCHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_UNCHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_UNCHECKED)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d' or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsDone,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
		
	}//1
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_UNCHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_CHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_UNCHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_UNCHECKED)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d' or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsDiscard,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
	}
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_UNCHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_UNCHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_CHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_UNCHECKED)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d'or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsSuspend,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
	}//3
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_UNCHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_UNCHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_UNCHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_CHECKED)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d'or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsInLine,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());	
	}//4
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_CHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_CHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_UNCHECKED
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_UNCHECKED)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d' or R.status='%d'or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsDiscard,qsDone,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
	}//5
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_CHECKED//1
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_UNCHECKED//2
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_CHECKED//3
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_UNCHECKED//0
		)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsDone,qsSuspend,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
	}//6
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_CHECKED//1
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_UNCHECKED//2
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_UNCHECKED//3
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_CHECKED//0
		)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsDone,qsInLine,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
	}//7
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_UNCHECKED//1
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_CHECKED//2
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_CHECKED//3
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_UNCHECKED//0
		)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsDiscard,qsSuspend,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
	}//8
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_UNCHECKED//1
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_CHECKED//2
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_UNCHECKED//3
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_CHECKED//0
		)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsDiscard,qsInLine,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
	}//9
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_UNCHECKED//1
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_UNCHECKED//2
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_CHECKED//3
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_CHECKED//0
		)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsSuspend,qsInLine,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
	}//10
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_CHECKED//1
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_CHECKED//2
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_CHECKED//3
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_UNCHECKED//0
		)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsDone,qsDiscard,qsSuspend,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
	}//11
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_CHECKED//1
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_CHECKED//2
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_UNCHECKED//3
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_CHECKED//0
		)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsDone,qsDiscard,qsInLine,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
	}//12
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_UNCHECKED//1
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_CHECKED//2
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_CHECKED//3
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_CHECKED//0
		)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsDiscard,qsSuspend,qsInLine,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
	}//13
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_CHECKED//1
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_UNCHECKED//2
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_CHECKED//3
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_CHECKED//0
		)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s' and (R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d' or R.status='%d') and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,qsDone,qsSuspend,qsInLine,qsDoing,qsPrepare,theApp.GetController()->GetDataDays());
	}//14
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_CHECKED//1
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_CHECKED//2
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_CHECKED//3
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_CHECKED//0
		)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.room_id,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.his_queue_id,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,W.room_name waitroom_name,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join WaitingRoom W on Q.room_id=W.room_id left join Consult C on D.consult_id=C.consult_id) R where R.office_id='%s'and regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime) order by R.status,R.priority desc,R.regtime")
			,officeid,theApp.GetController()->GetDataDays());
	}//15
	else if(MainFrame->m_menuCheckStates[ID_FR_SHOWCALL]==MF_UNCHECKED//1
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWQUIT]==MF_UNCHECKED//2
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWSUSPEND]==MF_UNCHECKED//3
		&& MainFrame->m_menuCheckStates[ID_FR_SHOWWAITTING]==MF_UNCHECKED//0
		)
	{
		sql=_T("");
	}
	else
	{
		return FALSE;
	}
	MainFrame->GetPatientView()->ShowData(sql);
	return TRUE;
}

/*
void COfficeQueView::SearchDocData()
{
	m_mutexDoc.Lock();

	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	pMainFrame->GetDocInfoView()->m_view_docinfo.DeleteAllItems();
	int nKey;
	USERSTATUS usbuf;
	int item=0;
	
	POSITION pos = pMainFrame->m_pChatDlg->m_mapUserList.GetStartPosition();
	for(;pos;)
	{
		pMainFrame->m_pChatDlg->m_mapUserList.GetNextAssoc(pos,nKey,usbuf);
		CString docName=usbuf.wszUserName;
		CString docID=usbuf.wszUserID;
		CString status=_T("");
		if(usbuf.eUserStatus == dsWorking)
		{
			status=_T("工作");
		}
		else
		{
			status=_T("暂停");
		}
		if(pMainFrame->pbase->IsOpen())
		{
			CADORecordset* pre=new CADORecordset(pMainFrame->pbase);
			CString sql;
			sql.Format(_T("select office_id from Doctor where doctor_id='%s'"),docID);
			if(pre->Open(sql,CADORecordset::openQuery))
			{
				if(!pre->IsBOF())
				{
					CString officeid;
					pre->GetFieldValue(_T("office_id"),officeid);
					if(officeid==GetOfficeID())
					{
						pMainFrame->GetDocInfoView()->m_view_docinfo.InsertItem(item,status);
						pMainFrame->GetDocInfoView()->m_view_docinfo.SetItemText(item,1,docName);
						pMainFrame->GetDocInfoView()->m_view_docinfo.SetItemText(item,2,usbuf.wszPatientName);
						m_itemToDocID[item]=docID;
						item++;
					}
				}
				pre->Close();
			}

			delete pre;
			pre=NULL;
		}
	}
	m_mutexDoc.Unlock();
}
*/
BOOL COfficeQueView::SearchData(CString &date,CString& contex,int index)
{
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CString sql=_T("");
	if(index==0)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,Q.his_queue_id,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join Consult C on D.consult_id=C.consult_id) R where R.reg_id='%s' and '%s'=convert(varchar(10),R.regtime,120) and R.office_id='%s' order by R.status,R.his_queue_id,R.priority")
			,contex,date,GetOfficeID());
	}
	else if(index==1)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,Q.his_queue_id,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join Consult C on D.consult_id=C.consult_id) R where R.patient_name like '%%%s%%' and '%s'=convert(varchar(10),R.regtime,120) and R.office_id in(select o.office_id from Nurse_Office n,Office o where Nurse_id='%s' and n.office_id=o.office_id and o.office_id='%s') order by R.status,R.his_queue_id,R.priority")
			,contex,date,theApp.GetLoginID(),GetOfficeID());
	}
	else if(index==2)
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,Q.his_queue_id,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join Consult C on D.consult_id=C.consult_id) R where R.doctor_name like '%%%s%%' and '%s'=convert(varchar(10),R.regtime,120) and R.office_id in(select o.office_id from Nurse_Office n,Office o where Nurse_id='%s' and n.office_id=o.office_id and o.office_id='%s') order by R.status,R.his_queue_id,R.priority")
			,contex,date,theApp.GetLoginID(),GetOfficeID());
	}
	else if(index==CB_ERR || contex==_T(""))
	{
		sql.Format(_T("select * from (select Q.reg_id,Q.gender_flag,Q.office_id,Q.regtime,datediff(ss,Q.regtime,getdate()) wait_seconds,Q.status,Q.priority,Q.queue_id,Q.patient_name,Q.comment,D.name doctor_name,O.call_id,Q.log_id,Q.queue_id_call,Q.his_queue_id,Q.id_item,Q.item_desc,Q.in_flag,C.call_name ConsultName from Queue Q left join Office O on Q.office_id=O.office_id left join Doctor D on Q.doctor_id=D.doctor_id left join Consult C on D.consult_id=C.consult_id) R where '%s'=convert(varchar(10),R.regtime,120) and R.office_id in(select o.office_id from Nurse_Office n,Office o where Nurse_id='%s' and n.office_id=o.office_id and o.office_id='%s') order by R.status,R.his_queue_id,R.priority")
			,date,theApp.GetLoginID(),GetOfficeID());
	}
	
	pMainFrame->GetPatientView()->ShowData(sql);
	return TRUE;
}

CADORecordset* COfficeQueView::SearchNurseContrlOffice()
{
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CString LoginNurseID=theApp.GetLoginID();
	try
	{
		CADODatabase db;
		db.SetConnectionString(pMainFrame->m_strConn);
		db.Open();
		CADORecordset *pre=new CADORecordset(&db);
		int item=0;
		CString sql=_T("");
		sql.Format(_T("select o.name,o.office_id from Nurse_Office n,Office o where Nurse_id='%s' and n.office_id=o.office_id order by o.name"),
			LoginNurseID);
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			return pre;
		}
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("COfficeQueView::SearchNurseContrlOffice"));
	}
	return NULL;
}

void COfficeQueView::GetNurseContrlOfficeID()
{
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CString LoginNurseID=theApp.GetLoginID();
	CString officeid=_T("");

	try
	{
		CADODatabase db;
		db.SetConnectionString(pMainFrame->m_strConn);
		db.Open();
		CADORecordset *pre=new CADORecordset(&db);
		CString sql=_T("");
		sql.Format(_T("select o.name,o.office_id from Nurse_Office n,Office o where Nurse_id='%s' and n.office_id=o.office_id order by o.name"),
			LoginNurseID);
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
			{
				while(!pre->IsEOF())
				{
					pre->GetFieldValue(_T("office_id"),officeid);
					m_CandoOfficeID.push_back(officeid);
					pre->MoveNext();
				}
			}
		}
		delete pre;
		pre=NULL;
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("COfficeQueView::GetNurseContrlOfficeID"));
	}
}
/*
void COfficeQueView::SetDataDays(int nDays)
{
	if(nDays < 0)
	{
		nDays = 0;
	}
	//m_strSqlDate.Format(_T("regtime>cast(CONVERT(varchar(100),dateadd(day,-%d,GETDATE()),23) as datetime)")
	//	, nDays);
}
*/
 

