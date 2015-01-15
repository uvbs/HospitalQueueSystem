// PaientView.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PaientView.h"
#include "EditPatNameDlg.h"
#include "WriteLog.h"
#include "MyString.h"
#include "MyCommon.h"
#include "addCommentDlg.h"

// CPaientView

IMPLEMENT_DYNCREATE(CPaientView, CFormView)

CPaientView::CPaientView()
: CFormView(CPaientView::IDD)
, m_bCanRefrush(TRUE)
{
	
	m_officeID=_T("");
	
	CMinute=_T("");
	m_bSelect=FALSE;
	
	m_pChangQueDlg = NULL;

	//m_strSqlDate = _T("regtime>cast(CONVERT(varchar(100),dateadd(day,-1,GETDATE()),23) as datetime)");

	//CBaseConfig baseConfig;
	//m_strConn.Format(_T("Driver={%s};Server=%s;Database=Nurse;Uid=%s;Pwd=%s;"),
	//	baseConfig.GetDataDrive(),baseConfig.GetServerIP(),
	//	baseConfig.GetServerAcount(), baseConfig.GetServerPassword());
}

CPaientView::~CPaientView()
{
	if(m_pChangQueDlg)
	{
		delete m_pChangQueDlg;
		m_pChangQueDlg = NULL;
	}
}

void CPaientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEW_PAITIENT, m_view_paient);
}

BEGIN_MESSAGE_MAP(CPaientView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_VIEW_PAITIENT, &CPaientView::OnLvnItemchangedViewPaitient)
	ON_NOTIFY(NM_RCLICK, IDC_VIEW_PAITIENT, &CPaientView::OnNMRClickViewPaitient)
	

	ON_COMMAND(IDM_INSERTQUEUE, &CPaientView::OnInsertqueue)
	ON_COMMAND(IDM_QUIT,&CPaientView::OnQuitNum)
	ON_COMMAND(ID_TIME_SUSPEND,&CPaientView::OnTimeSuspend)
	ON_COMMAND(ID_NOTIME_SUSPEND,&CPaientView::OnNoTimeSuspend)
	ON_COMMAND(ID_ACTIVE,&CPaientView::OnPatientActive)
	
	ON_COMMAND(ID_CHANGEQUE, &CPaientView::OnChangeque)
	ON_COMMAND(ID_SETDOCTOR,&CPaientView::OnSetDoctor)
//	ON_COMMAND(ID_RECALL_RECONSULTQUE,&CPaientView::OnRecallConsultQueue)
//	ON_COMMAND(ID_RECALL_RECONSULT,&CPaientView::OnRecallConsult)
	ON_COMMAND(ID_RECONSULT,&CPaientView::OnReConsult)
//	ON_COMMAND(ID_SEARCH, &CPaientView::OnSearch)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_VIEW_PAITIENT, &CPaientView::OnNMCustomdrawViewPaitient)
	ON_COMMAND(ID_RECALLCONSULT, &CPaientView::OnRecallconsult)
	
	ON_COMMAND(ID_CALL, &CPaientView::OnCall)
	ON_COMMAND(ID_RECALL, &CPaientView::OnRecall)
	ON_COMMAND(ID_WAIT, &CPaientView::OnWait)
	ON_COMMAND(ID_EDITNAME, &CPaientView::OnEditname)
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CPaientView 诊断

#ifdef _DEBUG
void CPaientView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPaientView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPaientView 消息处理程序

void CPaientView::ShowList()
{
	if(!m_view_paient.m_hWnd)return;

	CRect rect;
//	GetClientRect(&rect); // 获取当前客户区view大小
	GetWindowRect(&rect);
	ScreenToClient(&rect);

	CSize size;
	size.cx = rect.right - rect.left;
	size.cy = rect.bottom - rect.top;
//	SetScrollSizes(MM_HIMETRIC, size); // 将CScrollView的大小设置为当前客户区大小
	m_view_paient.MoveWindow(&rect, TRUE);
}
void CPaientView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	ShowList();
	// TODO: 在此处添加消息处理程序代码
}

void CPaientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	ListView_SetExtendedListViewStyle(m_view_paient.m_hWnd, LVS_EX_FULLROWSELECT |  LVS_EX_GRIDLINES);
	////////////////////////////////////////
	
	/////////////////////////////////////
	m_view_paient.SetRowHeigt(18);
	m_view_paient.SetFontHW(14, 0);
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.right-=2;
	m_view_paient.InsertColumn(0,_T("行号"),LVCFMT_CENTER,rect.Width()*5/100);
	m_view_paient.InsertColumn(1,_T("流水号"),LVCFMT_CENTER,0);
	m_view_paient.InsertColumn(2,_T("就诊号"),LVCFMT_CENTER,rect.Width()*14/100);
	m_view_paient.InsertColumn(3,_T("排队号"),LVCFMT_CENTER,rect.Width()*7/100);
	m_view_paient.InsertColumn(4,_T("患者姓名"),LVCFMT_CENTER,rect.Width()*7/100);
	m_view_paient.InsertColumn(5,_T("当前状态"),LVCFMT_CENTER,rect.Width()*7/100);
	m_view_paient.InsertColumn(6,_T("优先级"),LVCFMT_CENTER,rect.Width()*6/100);
	m_view_paient.InsertColumn(7,_T("备注"),LVCFMT_CENTER,rect.Width()*6/100);
	m_view_paient.InsertColumn(8,_T("呼叫医生"),LVCFMT_CENTER,rect.Width()*7/100);
	m_view_paient.InsertColumn(9,_T("诊室"),LVCFMT_CENTER,rect.Width()*15/100);
	m_view_paient.InsertColumn(10,_T("排队时间"),LVCFMT_CENTER,rect.Width()*15/100);
	m_view_paient.InsertColumn(11,_T("科别"),LVCFMT_CENTER,0);
	m_view_paient.InsertColumn(12,_T("检查项目"),LVCFMT_CENTER,0);
	m_view_paient.InsertColumn(13,_T("已等候"),LVCFMT_CENTER,rect.Width()*10/100);
	//m_view_paient.InsertColumn(13,_T("等候室"),LVCFMT_CENTER,0);
	

	//ConnectToBase();
	
	
	ReadOfficeInformation();
	FlushListOfficeData();
	
	//office默认选中第一行
	theApp.m_pOfficeQueView->m_view_officeque.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	theApp.m_pOfficeQueView->m_view_officeque.EnsureVisible(0, TRUE);
}
void CPaientView::FlushListOfficeData()
{
	CNurseOfficeQueue office;
	CString	Queue=_T("");//科室科室ID
	CString Queue_id=_T("");//排队号码
	int NumStart;//开始排队号码
	int queNumber;//实际排队人数
	
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	//Queue中有数据 
	CString sqlhaveData=_T("select  q.office_id,max(q.queue_id)queue_id,max(o.call_id)call_id,count(q.status)number from queue q,Office o where q.office_id=o.office_id and convert(varchar(100),getdate(),120)=convert(varchar(100),q.regtime,120) group by q.office_id");
	
	try
	{
		CADODatabase db;
		db.SetConnectionString(pMainFrame->m_strConn);
		db.Open();
		CADORecordset *pre=new CADORecordset(&db);
		if(pre->Open(sqlhaveData,CADORecordset::openQuery))
		{
			if(pre->IsBOF())
			{
				delete pre;
				pre = NULL;
				return;
			}
			while(!pre->IsEOF())
			{		
				pre->GetFieldValue(_T("queue_id"),Queue_id);
				pre->GetFieldValue(_T("office_id"),Queue);
				pre->GetFieldValue(_T("number"),queNumber);

				POSITION pos=m_list_office.GetHeadPosition();
				while(pos!=NULL)
				{
					office=m_list_office.GetNext(pos);
					if(pos==NULL && office.GetQueID()==Queue)
					{
						m_pool.CStringToint(NumStart,Queue_id);
						office.SetQuenumStart(NumStart+1);//排队时设置开始号码
						office.SetQueNumber(queNumber);//实际的排队人数

						m_list_office.RemoveTail();
						m_list_office.AddTail(office);
					}
					else if(office.GetQueID()==Queue)
					{
						m_pool.CStringToint(NumStart,Queue_id);
						office.SetQuenumStart(NumStart+1);
						office.SetQueNumber(queNumber);

						m_list_office.GetPrev(pos);
						m_list_office.SetAt(pos,office);
						break;
					}
				}
				pre->MoveNext();
			}
		}
		delete pre;
		pre = NULL;
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CPaientView::FlushListOfficeData"));
	}
}

void CPaientView::ShowData(CString sql)
{
	if(sql==_T(""))
	{
		m_view_paient.DeleteAllItems();
		return;
	}
//	
	CMainFrame* pMainFrame=(CMainFrame*)theApp.m_pNurseWnd;
	int priority;
	CString doctorname,patientname,queue_id,regtime,que_id_call,logid;
	int status;
	BOOL gender_flag;
	//COleDateTime datetime;
	CString strIdPay,strPayDesc;
	int nInFlag;
	CString strWaitSeconds;
	CString strComment;
	CString strRegId;
	CString strConsultName;
	
	//时间换算
	//CTime current_time;
	//current_time=CTime::GetCurrentTime();
	//COleDateTime m_tEndDateTime;
	//m_tEndDateTime.SetDateTime(current_time.GetYear(),current_time.GetMonth(),
	//	current_time.GetDay(),current_time.GetHour(),current_time.GetMinute(),
	//	current_time.GetSecond());

	try
	{
		CADODatabase db;
		db.SetConnectionString(theApp.GetController()->m_strSqlConn);
		db.Open();
		CADORecordset *pRecordset=new CADORecordset(&db);
		if(pRecordset->Open(sql,CADORecordset::openQuery))
		{
			int item=0;
			m_view_paient.DeleteAllItems();
			if(!pRecordset->IsBOF())
			{
				while(!pRecordset->IsEOF())
				{
					pRecordset->GetFieldValue(_T("reg_id"),strRegId);
					pRecordset->GetFieldValue(_T("status"),status);
					pRecordset->GetFieldValue(_T("priority"),priority);
					pRecordset->GetFieldValue(_T("doctor_name"),doctorname);
					pRecordset->GetFieldValue(_T("patient_name"),patientname);
					pRecordset->GetFieldValue(_T("queue_id"),queue_id);
					pRecordset->GetFieldValue(_T("regtime"),regtime);
					//pRecordset->GetFieldValue(_T("regtime"),datetime);
					pRecordset->GetFieldValue(_T("gender_flag"),gender_flag);
					pRecordset->GetFieldValue(_T("queue_id_call"),que_id_call);
					pRecordset->GetFieldValue(_T("log_id"),logid);
//					pRecordset->GetFieldValue(_T("waitroom_name"),waitroom_name);
					pRecordset->GetFieldValue(_T("id_item"), strIdPay);
					pRecordset->GetFieldValue(_T("item_desc"), strPayDesc);
					pRecordset->GetFieldValue(_T("in_flag"), nInFlag);
					pRecordset->GetFieldValue(_T("wait_seconds"), strWaitSeconds);
					pRecordset->GetFieldValue(_T("comment"), strComment);
					pRecordset->GetFieldValue(_T("ConsultName"), strConsultName);


					//COleDateTimeSpan tMinute = m_tEndDateTime - datetime;//计算时间差
					//int iMinute = (int)tMinute.GetTotalMinutes();//换算成分钟
					//m_pool.intToCString(iMinute,CMinute);
					int nWaitSeconds = MyString::Str2Int(strWaitSeconds);

					//CMinute.Format(_T("%d秒"), nWaitSeconds);

					int nWaitMinutes = nWaitSeconds/60;
					int iHour = nWaitMinutes/60;
					nWaitMinutes = nWaitMinutes%60;
					if(iHour == 0)
					{
						CMinute.Format(_T("%d分钟"), nWaitMinutes);
					}
					else
					{
						CMinute.Format(_T("%d小时%d分钟"), iHour, nWaitMinutes);
					}

					CString serial=_T("");
					m_pool.intToCString(item+1,serial);
					m_view_paient.InsertItem(item,serial);
					m_view_paient.SetItemText(item,1,logid);
					m_view_paient.SetItemText(item,2,strRegId);
					m_view_paient.SetItemText(item,3,que_id_call);
					m_view_paient.SetItemText(item,4,patientname);
					//m_view_paient.SetItemText(item,iCol++,waitroom_name);
					if(status==qsInLine)
					{
						m_view_paient.SetItemText(item,5,statusWaitLine);
						m_view_paient.SetItemColor(5,item,RGB(255,255,255));
					}
					else if(status==qsSuspend)
					{
						m_view_paient.SetItemText(item,5,statusSuspend);
						m_view_paient.SetItemColor(5,item,0x436EEE);
					}
					else if(status==qsDiscard)
					{
						m_view_paient.SetItemText(item,5,statusQuitQue);
						m_view_paient.SetItemColor(5,item,RGB(128,128,128));
					}
					else if(status==qsDone)
					{
						m_view_paient.SetItemText(item,5,statusDone);
						m_view_paient.SetItemColor(5,item,RGB(0,255,0));
					}
					else if(status==qsPrepare)
					{
						m_view_paient.SetItemText(item,5,statusPre);
						m_view_paient.SetItemColor(5,item,RGB(255,255,0));
					}
					else if(status==qsDoing)
					{
						m_view_paient.SetItemText(item,5,statusDoing);
						m_view_paient.SetItemColor(5,item,RGB(255,0,0));
					}

					if(priority== priZero)
					{
						m_view_paient.SetItemText(item,6,priZeroText);
						m_view_paient.SetItemColor(6,item,RGB(255,255,255));
					}
					else if(priority== priOne)
					{
						m_view_paient.SetItemText(item,6,priOneText);
					}
					else if(priority== priTwo)
					{
						m_view_paient.SetItemText(item,6,priTwoText);
					}
					else if(priority== priThree)
					{
						m_view_paient.SetItemText(item,6,priThreeText);
						m_view_paient.SetItemColor(6,item,RGB(0,255,0));
					}
					
					m_view_paient.SetItemText(item,7,strComment);
					m_view_paient.SetItemText(item,8,doctorname);
					m_view_paient.SetItemText(item,9,strConsultName);
					m_view_paient.SetItemText(item,10,regtime);
					CString strInFlag;
					if(nInFlag == 1)
					{
						strInFlag = _T("住院");
					}
					else
					{
						strInFlag = _T("门诊");
					}
					m_view_paient.SetItemText(item,11,strInFlag);
					m_view_paient.SetItemText(item,12,strPayDesc);
					m_view_paient.SetItemText(item,13,CMinute);

					pRecordset->MoveNext();
					item++;
				}
			}
			pRecordset->Close();
		}
		delete pRecordset;
		pRecordset=NULL;
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CPaientView::ShowData"));
	}
}
void CPaientView::OnLvnItemchangedViewPaitient(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_item=pNMLV->iItem;
	m_paitname=m_view_paient.GetItemText(m_item,3);
	m_paitlogid=m_view_paient.GetItemText(m_item,1);
	m_selectdocname=m_view_paient.GetItemText(m_item,4);
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	*pResult = 0;
}

void CPaientView::OnNMRClickViewPaitient(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int item = pNMItemActivate->iItem;
	CString status=GetSelectStatus();
	CMenu   menu;   //定义下面要用到的cmenu对象
	menu.LoadMenu(IDR_PAINET_MENU); //装载自定义的右键菜单 
	CMenu* pContextMenu=menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单 
	CPoint point;//定义一个用于确定光标位置的位置  
	GetCursorPos(&point);//获取当前光标的位置，以便使得菜单可以跟随光标

	if(item==-1)
	{
		pContextMenu->EnableMenuItem(IDM_QUIT,MF_GRAYED);
		pContextMenu->EnableMenuItem(IDM_CALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(IDM_INSERTQUEUE,MF_GRAYED);
		pContextMenu->EnableMenuItem(IDM_RECALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_ACTIVE,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_TIME_SUSPEND,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_NOTIME_SUSPEND,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECONSULT,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_SETDOCTOR,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_CHANGEQUE,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALL_RECONSULT,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALL_RECONSULTQUE,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_CALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_WAIT,MF_GRAYED);
	}
	if(status==statusWaitLine)
	{
		pContextMenu->EnableMenuItem(IDM_RECALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALL_RECONSULT,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALL_RECONSULTQUE,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECONSULT,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_ACTIVE,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALLCONSULT,MF_GRAYED);
	}
	if(status==statusQuitQue)
	{
		pContextMenu->EnableMenuItem(IDM_QUIT,MF_GRAYED);
		pContextMenu->EnableMenuItem(IDM_CALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(IDM_INSERTQUEUE,MF_GRAYED);
		pContextMenu->EnableMenuItem(IDM_RECALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_ACTIVE,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_TIME_SUSPEND,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_NOTIME_SUSPEND,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_SETDOCTOR,MF_GRAYED);
//		pContextMenu->EnableMenuItem(ID_CHANGEQUE,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECONSULT,MF_GRAYED);
	}
	if(status==statusSuspend)
	{
		pContextMenu->EnableMenuItem(IDM_INSERTQUEUE,MF_GRAYED);
		pContextMenu->EnableMenuItem(IDM_RECALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALL_RECONSULT,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALL_RECONSULTQUE,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECONSULT,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_CALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_WAIT,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALLCONSULT,MF_GRAYED);
	}
	if(status==statusDone)
	{
		pContextMenu->EnableMenuItem(IDM_QUIT,MF_GRAYED);
		pContextMenu->EnableMenuItem(IDM_CALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(IDM_INSERTQUEUE,MF_GRAYED);
		
		pContextMenu->EnableMenuItem(ID_ACTIVE,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_TIME_SUSPEND,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_NOTIME_SUSPEND,MF_GRAYED);
		
		pContextMenu->EnableMenuItem(ID_SETDOCTOR,MF_GRAYED);
	//	pContextMenu->EnableMenuItem(ID_CHANGEQUE,MF_GRAYED);
	//	pContextMenu->EnableMenuItem(ID_RECALL_RECONSULT,MF_GRAYED);
	//	pContextMenu->EnableMenuItem(ID_RECALL_RECONSULTQUE,MF_GRAYED);
	}
	if(status==statusDoing || status==statusPre)
	{
		pContextMenu->EnableMenuItem(IDM_QUIT,MF_GRAYED);
		pContextMenu->EnableMenuItem(IDM_CALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(IDM_INSERTQUEUE,MF_GRAYED);
		pContextMenu->EnableMenuItem(IDM_RECALL,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_ACTIVE,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_TIME_SUSPEND,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_NOTIME_SUSPEND,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECONSULT,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_SETDOCTOR,MF_GRAYED);
//		pContextMenu->EnableMenuItem(ID_CHANGEQUE,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALL_RECONSULT,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALL_RECONSULTQUE,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_WAIT,MF_GRAYED);
		pContextMenu->EnableMenuItem(ID_RECALLCONSULT,MF_GRAYED);
	}
	m_bCanRefrush = FALSE;
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this->GetParent()); //在指定位置显示弹出菜单
	pContextMenu->DestroyMenu();
	m_bCanRefrush = TRUE;
	
	*pResult = 0;
}

/*
void CPaientView::OnCallPatient()
{
	// TODO: 在此添加命令处理程序代码
	
}
*/

void CPaientView::OnQuitNum()
{
	int nItem=-1;
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	POSITION pos=m_view_paient.GetFirstSelectedItemPosition();
	if(!pos)
	{
		return;
	}
	else
	{
		while(pos)
		{
			nItem=m_view_paient.GetNextSelectedItem(pos);
			CString log_id=GetSelectLogidFromItem(nItem);
			CString status=GetSelectStatusFromItem(nItem);
			if((status == statusWaitLine || status == statusSuspend)&&!log_id.IsEmpty())
			{
				CString sql=_T("");
				sql.Format(_T("update Queue set status='%d' from Queue where log_id='%s'"),qsDiscard,log_id);
				try
				{
					CADODatabase db;
					db.SetConnectionString(pMainFrame->m_strConn);
					if(db.Open())
					{
						db.Execute(sql);
						db.Close();
					}
				}
				catch(_com_error& e)
				{
					WriteLog::WriteDbErrLog(_T("CPaientView::OnQuitNum"));
				}

				ReviseListCtrPatStatus(log_id,qsDiscard);
			}
		}
	}
	(((CNurseStationApp*)AfxGetApp())->m_pNurseWnd)->OnFrFlush();
}

void CPaientView::OnInsertqueue()
{
	// TODO: 在此添加命令处理程序代码
	int nItem=-1;
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;

	POSITION pos=m_view_paient.GetFirstSelectedItemPosition();
	if(!pos)
	{
		return;
	}
	else
	{
		//
		CAddCommentDlg dlg;
		m_bCanRefrush = FALSE;
		INT_PTR nRet = dlg.DoModal();
		m_bCanRefrush = TRUE;
		if(nRet == IDCANCEL)
		{
			return;
		}
		CString strComment = dlg.GetComment();
		while(pos)
		{
			nItem=m_view_paient.GetNextSelectedItem(pos);
			CString log_id=GetSelectLogidFromItem(nItem);
			CString status=GetSelectStatusFromItem(nItem);

			if((status==statusWaitLine || status==statusSuspend) && !log_id.IsEmpty())
			{
				CString sql=_T("");
				sql.Format(_T("update queue set priority=%d,comment='%s' where log_id='%s'"), 
					priThree, strComment, log_id);
				try
				{
					CADODatabase db;
					db.SetConnectionString(pMainFrame->m_strConn);
					if(db.Open())
					{
						db.Execute(sql);
						db.Close();
					}
				}
				catch(_com_error& e)
				{
					WriteLog::WriteDbErrLog(_T("CPaientView::OnInsertqueue"));
				}

				ReviseListCtrPatPriority(log_id,priThree);
			}
		}
	}
	(((CNurseStationApp*)AfxGetApp())->m_pNurseWnd)->OnFrFlush();
}

void CPaientView::ReadOfficeInformation()
{
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	//CNurseOfficeQue操作
	CString m_officeid;//科室ID
	CString m_callid;//ID前缀
	CString m_officeName;
	CString m_callName;
	int m_officeNumStart;//开始号码
	int m_officeNumEnd;//结束号码
	int m_officeAmLimit;//上午限制人数
	int m_officePmLimit;//下午限制人数
	int m_officeDayLimit;//全天限制人数
	
	CNurseOfficeQueue m_office;

	CString sql=_T("select * from Office");

	try
	{
		CADODatabase db;
		db.SetConnectionString(pMainFrame->m_strConn);
		db.Open();
		CADORecordset *pre=new CADORecordset(&db);
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
			{
				while(!pre->IsEOF())
				{
					pre->GetFieldValue(_T("office_id"),m_officeid);
					pre->GetFieldValue(_T("call_id"),m_callid);
					pre->GetFieldValue(_T("NumStart"),m_officeNumStart);
					pre->GetFieldValue(_T("NumEnd"),m_officeNumEnd);
					pre->GetFieldValue(_T("AmLimit"),m_officeAmLimit);
					pre->GetFieldValue(_T("PmLimit"),m_officePmLimit);
					pre->GetFieldValue(_T("DayLimit"),m_officeDayLimit);
					pre->GetFieldValue(_T("name"),m_officeName);
					pre->GetFieldValue(_T("call_name"),m_callName);
				
					if(m_officeNumStart<=0)
					{
						m_officeNumStart=1;
					}
					if(m_officeNumEnd<=0)
					{
						m_officeNumEnd=2147483647;
					}
					if(m_officeDayLimit<=0)
					{
						m_officeDayLimit=2147483647;
					}
					if(m_officeAmLimit<=0)
					{
						m_officeAmLimit=2147483647;
					}
					if(m_officePmLimit<=0)
					{
						m_officePmLimit=2147483647;
					}
				
					m_office.SetQueID(m_officeid);
					m_office.SetQueCallID(m_callid);
					m_office.SetQueCallName(m_callName);
					m_office.SetQueName(m_officeName);
					m_office.SetQuenumStart(m_officeNumStart);
					m_office.SetQuenumEnd(m_officeNumEnd);
					m_office.SetAmLimitPatient(m_officeAmLimit);
					m_office.SetPmLimitPatient(m_officePmLimit);
					m_office.SetDayLimitPatient(m_officeDayLimit);
			

					m_list_office.AddTail(m_office);
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
		WriteLog::WriteDbErrLog(_T("CPaientView::ReadOfficeInformation"));
	}
}


void CPaientView::DeleteYesterdayData()
{
	CString sql=_T("delete from Queue where convert(varchar(10),getdate(),120)<>convert(varchar(10),regtime,120)");
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	try
	{
		CADODatabase db;
		db.SetConnectionString(pMainFrame->m_strConn);
		if(db.Open())
		{
			db.Execute(sql);
			db.Close();
		}
	}
	catch(_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CPaientView::DeleteYesterdayData"));
	}
}

void CPaientView::OnTimeSuspend()
{
	CString logid=GetSelectPaitLogid();
	if(logid==_T(""))
	{
		return;
	}
	CString status=GetSelectStatus();
	if(status==statusWaitLine)
	{
		CSuspendPatDlg dlg(this);
		dlg.DoModal();
	}
}


void CPaientView::OnNoTimeSuspend()
{
	int nItem=-1;
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	POSITION pos=m_view_paient.GetFirstSelectedItemPosition();
	if(!pos)
	{
		return;
	}
	else
	{
		while(pos)
		{
			nItem=m_view_paient.GetNextSelectedItem(pos);
			CString log_id=GetSelectLogidFromItem(nItem);
			CString status=GetSelectStatusFromItem(nItem);
			
			if(status==statusWaitLine && !log_id.IsEmpty())
			{
				CString sql=_T("");
				sql.Format(_T("update Queue set status='%d'from Queue where log_id='%s'"),qsSuspend,log_id);
				try
				{
					CADODatabase db;
					db.SetConnectionString(pMainFrame->m_strConn);
					if(db.Open())
					{
						db.Execute(sql);
						db.Close();
					}
				}
				catch(_com_error& e)
				{
					WriteLog::WriteDbErrLog(_T("CPaientView::OnNoTimeSuspend"));
				}
				CMainFrame* MainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
				ReviseListCtrPatStatus(log_id,qsSuspend);
			}
		}
	}
	(((CNurseStationApp*)AfxGetApp())->m_pNurseWnd)->OnFrFlush();
}

void CPaientView::OnPatientActive()
{
	int nItem=-1;
	SUSPENDTIME suspendtime;
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	POSITION pos=m_view_paient.GetFirstSelectedItemPosition();
	if(!pos)
	{
		return;
	}
	else
	{
		while(pos)
		{
			nItem=m_view_paient.GetNextSelectedItem(pos);
			CString log_id=GetSelectLogidFromItem(nItem);
			CString status=GetSelectStatusFromItem(nItem);
			if(status == statusSuspend && !log_id.IsEmpty())
			{
				CString sql=_T("");
				sql.Format(_T("update Queue set status='%d'from Queue where log_id='%s'"),qsInLine,log_id);
				try
				{

					CADODatabase db;
					db.SetConnectionString(pMainFrame->m_strConn);
					if(db.Open())
					{
						db.Execute(sql);
						db.Close();
					}
				}
				catch(_com_error& e)
				{
					WriteLog::WriteDbErrLog(_T("CPaientView::OnPatientActive"));
				}
				
				//如果是限时挂起手动激活后删除挂起链表中对应数据
				/*
				POSITION pos=pMainFrame->m_list_suspendtime.GetHeadPosition();

				while(pos!=NULL)
				{
					pMainFrame->m_timeMenutex.Lock();
					suspendtime=pMainFrame->m_list_suspendtime.GetNext(pos);
					if(suspendtime.logid==log_id && pos==NULL)
					{
						pMainFrame->m_list_suspendtime.RemoveTail();
					}
					else if(suspendtime.logid==log_id)
					{
						pMainFrame->m_list_suspendtime.GetPrev(pos);
						pMainFrame->m_list_suspendtime.RemoveAt(pos);
					}
					pMainFrame->m_timeMenutex.Unlock();
				}
				//修改状态
				ReviseListCtrPatStatus(log_id,qsInLine);
				*/
			}
		}
	}
}

/*
BOOL CPaientView::JudgeQueNull()
{
//	CMainFrame* pMainFrame=(CMainFrame*)this->GetParent()->GetParent();
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CADORecordset *pre=new CADORecordset(pMainFrame->pbase);
	CString sql=_T("select * from queue");
	if(pMainFrame->pbase->IsOpen())
	{
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
				return FALSE;
		}
	}

	return TRUE;
}
*/
void CPaientView::OnChangeque()
{
	if(m_view_paient.GetSelectedCount() < 1) return;
	CString logid=GetSelectPaitLogid();
	if(logid==_T(""))
	{
		return;
	}
	CPoolManage::CMyInformation myinformation;
	m_pChangQueDlg=new CChangeQueDialog;
	m_pChangQueDlg->Create(IDD_CHANGE_DIALOG,this);
	
	myinformation.SetPaitentName(GetSelectPaitname());
	CMainFrame* MainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	myinformation.SetOfficeName(MainFrame->GetOfficeQueView()->GetSelectOfficename());
	myinformation.SetLogid(GetSelectPaitLogid());
	::SendMessage(m_pChangQueDlg->m_hWnd,WM_MYPAITNMSG,WPARAM(&myinformation),0);
	
	CRect rectfirst;
	m_pChangQueDlg->GetWindowRect(&rectfirst);
	
	CRect rect;
	MainFrame->GetWindowRect(&rect);

	m_pChangQueDlg->MoveWindow(rect.left+rect.Width()*2/5,rect.top+rect.Height()/3,rectfirst.Width(),rectfirst.Height());
	m_bCanRefrush = FALSE;
	m_pChangQueDlg->ShowWindow(SW_SHOWNORMAL);
	m_bCanRefrush = TRUE;
	//m_bCanRefrush = FALSE;
	//m_pChangQueDlg->DoModal();
	//m_bCanRefrush = TRUE;
	//(((CNurseStationApp*)AfxGetApp())->m_pNurseWnd)->OnFrFlush();
}

void CPaientView::OnSetDoctor()
{
	if(m_view_paient.GetSelectedCount() < 1) return;
	CString logid=GetSelectPaitLogid();
	if(logid==_T(""))
	{
		return;
	}
	
	CSetDoctorDialog dlg(this);
	m_bCanRefrush = FALSE;
	INT_PTR nRet = dlg.DoModal();
	m_bCanRefrush = TRUE;
	if(IDOK == nRet)
	{
		(((CNurseStationApp*)AfxGetApp())->m_pNurseWnd)->OnFrFlush();
	}
}
/*
void CPaientView::OnRecallConsultQueue()
{
	CString logid=GetSelectPaitLogid();
	CString status=GetSelectStatus();
	if(logid==_T(""))
	{
		return;
	}
	if(status==statusQuitQue || status==statusDone)
	{
		CReCallSetDlg dlg(this);
		dlg.DoModal();
	}
}
*/
/*
void CPaientView::OnRecallConsult()
{
	
}
*/
void CPaientView::OnReConsult()
{
	int nItem=-1;
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	POSITION pos=m_view_paient.GetFirstSelectedItemPosition();
	if(!pos)
	{
		return;
	}
	else
	{
		try
		{
			CADODatabase db;
			db.SetConnectionString(pMainFrame->m_strConn);
			db.Open();
			while(pos)
			{
				nItem=m_view_paient.GetNextSelectedItem(pos);
				CString logid=GetSelectLogidFromItem(nItem);
				CString status=GetSelectStatusFromItem(nItem);

				if(status==statusDone && !logid.IsEmpty())
				{
					CString officeid=pMainFrame->GetOfficeQueView()->GetOfficeID();
					CString sql=_T("");
					int queue_id=0;
					CString doctor_id=_T("");
					CString docname=_T("");
					CString strConsultName = _T("");
					sql.Format(_T("select count(status)queue_id from queue where office_id='%s' and convert(varchar(10),getdate(),120)=convert(varchar(10),regtime,120)"),officeid);
					CADORecordset* pre=new CADORecordset(&db);
					if(pre->Open(sql,CADORecordset::openQuery))
					{
						if(!pre->IsBOF())
							pre->GetFieldValue(_T("queue_id"),queue_id);
						pre->Close();
					}

					sql.Format(_T("select Q.doctor_id,D.name,C.call_name ConsultName from Queue Q left join Doctor D on Q.doctor_id=D.doctor_id left join Consult C on D.consult_id=C.consult_id where Q.log_id='%s'"),logid);


					if(pre->Open(sql,CADORecordset::openQuery))
					{
						if(!pre->IsBOF())
						{
							pre->GetFieldValue(_T("doctor_id"),doctor_id);
							pre->GetFieldValue(_T("name"),docname);
							pre->GetFieldValue(_T("ConsultName"),strConsultName);
						}
					}
					delete pre;
					pre=NULL;

					if(doctor_id=_T(""))
					{
						sql.Format(_T("update queue set status='%d',queue_id='%d',priority='%d' where log_id='%s'"),qsInLine,queue_id+1,priThree,logid);
					}
					else
					{	
						sql.Format(_T("update queue set status='%d',queue_id='%d',priority='%d',doctor_id='%s' where log_id='%s'"),qsInLine,queue_id+1,priThree,doctor_id,logid);
					}
					db.Execute(sql);

					ReviseListCtrPatStatus(logid,qsInLine);
					ReviseListCtrDocName(logid,docname);
					ReviseListCtrConsultName(logid,strConsultName);
				}
			}
		}
		catch (_com_error& e)
		{
			WriteLog::WriteDbErrLog(_T("CPaientView::OnReConsult"));
		}
	}
}
/*
void CPaientView::OnSearch()
{
	// TODO: 在此添加命令处理程序代码
//	CMainFrame* pMainFrame=(CMainFrame*)this->GetParent()->GetParent()->GetParent();
//	pMainFrame->OnFrSearch();
//	CSearchDataDlg dlg;
//	dlg.DoModal();
}
*/


BOOL CPaientView::ReviseListCtrDocName(CString paitLogid,CString docName)
{
	int count=m_view_paient.GetItemCount();
	for(int i=0;i<count;i++)
	{
		CString logid=GetSelectLogidFromItem(i);
		if(paitLogid==logid)
		{
			m_view_paient.SetItemText(i,8,docName);
			break;
		}
	}
	return TRUE;
}

BOOL CPaientView::ReviseListCtrConsultName(CString paitLogid,CString strConsultName)
{
	int count=m_view_paient.GetItemCount();
	for(int i=0;i<count;i++)
	{
		CString logid=GetSelectLogidFromItem(i);
		if(paitLogid==logid)
		{
			m_view_paient.SetItemText(i,9,strConsultName);
			break;
		}
	}
	return TRUE;
}

BOOL CPaientView::ReviseListCtrPatStatus(CString paitLogid,
										 int status,CString DocName, CString strConsultName)
{
	if(paitLogid.IsEmpty()) return FALSE;

	int count=m_view_paient.GetItemCount();
	CString c_status=_T("");
	COLORREF color;
	if(status==qsInLine)
	{
		c_status=statusWaitLine;
		color=RGB(255,255,255);
	}
	if(status==qsDiscard)
	{
		c_status=statusQuitQue;
		color=RGB(128,128,128);
	}
	if(status==qsDoing)
	{
		c_status=statusDoing;
		color=RGB(255,0,0);
	}
	if(status==qsDone)
	{
		c_status=statusDone;
		color=RGB(0,255,0);
	}
	if(status==qsSuspend)
	{
		c_status=statusSuspend;
		color=0x436EEE;
	}
	if(status==qsPrepare)
	{
		c_status=statusPre;
		color=RGB(255,255,0);
	}

	for(int i=0;i<count;i++)
	{
		CString logid=m_view_paient.GetItemText(i,1);
		if(paitLogid==logid)
		{
			m_view_paient.SetItemText(i,5,c_status);
			m_view_paient.SetItemColor(5,i,color);
			m_view_paient.SetItemText(i,8,DocName);
			m_view_paient.SetItemText(i,9,strConsultName);
			break;
		}
	}
	return TRUE;
}

BOOL CPaientView::ReviseListCtrPatStatus(CString paitLogid,int status)
{
	int count=m_view_paient.GetItemCount();
	CString c_status=_T("");
	COLORREF color;
	if(status==qsInLine)
	{
		c_status=statusWaitLine;
		color=RGB(255,255,255);
	}
	if(status==qsDiscard)
	{
		c_status=statusQuitQue;
		color=RGB(128,128,128);
	}
	if(status==qsDoing)
	{
		c_status=statusDoing;
		color=RGB(255,0,0);
	}
	if(status==qsDone)
	{
		c_status=statusDone;
		color=RGB(0,255,0);
	}
	if(status==qsSuspend)
	{
		c_status=statusSuspend;
		color=0x436EEE;
	}
	if(status==qsPrepare)
	{
		c_status=statusPre;
		color=RGB(255,255,0);
	}

	for(int i=0;i<count;i++)
	{
		CString logid=m_view_paient.GetItemText(i,1);
		if(paitLogid==logid)
		{
			m_view_paient.SetItemText(i,5,c_status);
			m_view_paient.SetItemColor(5,i,color);
			break;
		}
	}
	return TRUE;
}

BOOL CPaientView::ReviseListCtrPatPriority(CString paitLogid,int priority)
{
	int count=m_view_paient.GetItemCount();
	CString c_pri=_T("");

	if(priority==priZero)
	{
		c_pri=priZeroText;
	}
	if(priority==priOne)
	{
		c_pri=priOneText;
	}
	if(priority==priTwo)
	{
		c_pri=priTwoText;
	}
	if(priority==priThree)
	{
		c_pri=priThreeText;
	}

	for(int i=0;i<count;i++)
	{
		CString logid=m_view_paient.GetItemText(i,1);
		if(paitLogid==logid)
		{
			m_view_paient.SetItemText(i,6,c_pri);
			m_view_paient.SetItemColor(6,i,RGB(0,255,0));
			break;
		}
	}
	return TRUE;
}

BOOL CPaientView::ReviseListCtrPatQueidcall(const CString& paitLogid,const CString& queidcall)
{
	int count=m_view_paient.GetItemCount();
	for(int i=0;i<count;i++)
	{
		CString logid=m_view_paient.GetItemText(i,1);
		if(paitLogid==logid)
		{
			m_view_paient.SetItemText(i,3,queidcall);
			break;
		}
	}
	return TRUE;
}
void CPaientView::OnNMCustomdrawViewPaitient(NMHDR *pNMHDR, LRESULT *pResult)
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
		POSITION pos = m_view_paient.GetFirstSelectedItemPosition();
		int index = m_view_paient.GetNextSelectedItem(pos);
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
/*
BOOL CPaientView::ConnectToBase()
{
	CMainFrame* pMainFrame=theApp.m_pNurseWnd;
	m_dataBase.SetConnectionString(_T("Driver={"+pMainFrame->m_baseDrive+"};Server="+pMainFrame->m_baseIP+";Database=Nurse;Uid="+pMainFrame->m_baseAccount+";Pwd="+pMainFrame->m_basePassWord+";"));
	if(m_dataBase.IsOpen())
	{
		m_dataBase.Close();
	}
	try
	{
		if(m_dataBase.Open())
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch(...)
	{
		return FALSE;
	}
}
*/
CString CPaientView::GetSelectLogid()
{
	CString log_id=m_view_paient.GetItemText(m_item,1);
	return log_id;
}

CString CPaientView::GetSelectStatus()
{
	CString status=m_view_paient.GetItemText(m_item,5);
	return status;
}

CString CPaientView::GetSelectStatusFromItem(int nItem)
{
	CString status=m_view_paient.GetItemText(nItem,5);
	return status;
}

CString CPaientView::GetSelectLogidFromItem(int nItem)
{
	CString logid=m_view_paient.GetItemText(nItem,1);
	return logid;
}

void CPaientView::OnRecallconsult()
{
	// TODO: 在此添加命令处理程序代码
	int nItem=-1;
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	POSITION pos=m_view_paient.GetFirstSelectedItemPosition();
	if(!pos)
	{
		return;
	}
	else
	{
		try
		{
			CADODatabase db;
			db.SetConnectionString(theApp.GetController()->m_strSqlConn);
			db.Open();
			while(pos)
			{
				nItem=m_view_paient.GetNextSelectedItem(pos);
				CString logid=GetSelectLogidFromItem(nItem);
				CString status=GetSelectStatusFromItem(nItem);

				if((status==statusQuitQue || status==statusDone) && !logid.IsEmpty())
				{
					CString officeid=pMainFrame->GetOfficeQueView()->GetOfficeID();
					CString sql=_T("");
					int queue_id=0;
					CString doctor_id=_T("null");
					CString docname=_T("");

					sql.Format(_T("select count(log_id) queue_id from queue where office_id='%s' and convert(varchar(10),getdate(),120)=convert(varchar(10),regtime,120)"),officeid);
					CADORecordset *pre=new CADORecordset(&db);
					if(pre->Open(sql,CADORecordset::openQuery))
					{
						if(!pre->IsBOF())
						{
							pre->GetFieldValue(_T("queue_id"),queue_id);
						}
						pre->Close();
					}
					/*
					sql.Format(_T("select doctor_id from Queue where log_id='%s'"),logid);
					if(pre->Open(sql,CADORecordset::openQuery))
					{
						if(!pre->IsBOF())
						{
							pre->GetFieldValue(_T("doctor_id"),doctor_id);
							doctor_id = _T("'") + doctor_id + _T("'");
						}
						pre->Close();
					}
					if(doctor_id != _T("null"))
					{
						sql.Format(_T("select name from doctor where doctor_id='%s'"),doctor_id);
						if(pre->Open(sql,CADORecordset::openQuery))
						{
							if(!pre->IsBOF())
							{
								pre->GetFieldValue(_T("name"),docname);
							}
							pre->Close();
						}
					}
					*/
					delete pre;
					pre=NULL;
					
					sql.Format(_T("update queue set status='%d',queue_id='%d',priority='%d',doctor_id=%s,comment='%s' where log_id='%s'"),qsInLine,queue_id+1,priThree,doctor_id,_T("过号召回"),logid);
					db.Execute(sql);

					ReviseListCtrPatStatus(logid,qsInLine);

					if(docname!=_T(""))
					{
						ReviseListCtrDocName(logid,docname);
					}
					ReviseListCtrConsultName(logid,_T(""));
					ReviseListCtrPatPriority(logid,priThree);
				}
			}
			db.Close();
		}
		catch (_com_error& e)
		{
			WriteLog::WriteDbErrLog(_T("CPaientView::OnRecallconsult"));
		}
	}
}
/*
void CPaientView::OnCall()
{
	// TODO: 在此添加命令处理程序代码
	CString doctor_id=theApp.m_pNurseWnd->GetNurseView()->GetCallDoctorId();
	CString logid=GetSelectLogid();
	theApp.m_pNurseWnd->m_pChatDlg->SendCmd(doctor_id,logid,dcCmdCall);
}

void CPaientView::OnRecall()
{
	// TODO: 在此添加命令处理程序代码
	CString doctor_id=theApp.m_pNurseWnd->GetNurseView()->GetCallDoctorId();
	CString logid=GetSelectLogid();
	theApp.m_pNurseWnd->m_pChatDlg->SendCmd(doctor_id,logid,dcCmdRecall);
}

void CPaientView::OnWait()
{
	// TODO: 在此添加命令处理程序代码
	CString doctor_id=theApp.m_pNurseWnd->GetNurseView()->GetCallDoctorId();
	CString logid=GetSelectLogid();
	theApp.m_pNurseWnd->m_pChatDlg->SendCmd(doctor_id,logid,dcCmdWait);
}
*/
void CPaientView::OnCall()
{
	// TODO: 在此添加命令处理程序代码
	CString doctor_id=theApp.m_pNurseWnd->GetNurseView()->GetCallDoctorId();
	CString logid=GetSelectLogid();
	if(!doctor_id.IsEmpty() && !logid.IsEmpty())
	{
		((CNurseStationApp*)AfxGetApp())->GetController()->SendCmd(doctor_id,logid,dcCmdCall);
	}
}

void CPaientView::OnRecall()
{
	// TODO: 在此添加命令处理程序代码
	CString doctor_id=theApp.m_pNurseWnd->GetNurseView()->GetCallDoctorId();
	CString logid=GetSelectLogid();
	if(!doctor_id.IsEmpty() && !logid.IsEmpty())
	{
		((CNurseStationApp*)AfxGetApp())->GetController()->SendCmd(doctor_id,logid,dcCmdRecall);
	}
}

void CPaientView::OnWait()
{
	// TODO: 在此添加命令处理程序代码
	CString doctor_id=theApp.m_pNurseWnd->GetNurseView()->GetCallDoctorId();
	CString logid=GetSelectLogid();
	if(!doctor_id.IsEmpty() && !logid.IsEmpty())
	{
		((CNurseStationApp*)AfxGetApp())->GetController()->SendCmd(doctor_id,logid,dcCmdWait);
	}
}

void CPaientView::OnEditname()
{
	// TODO: 在此添加命令处理程序代码
	CString logid = GetSelectLogid();
	if(logid.IsEmpty())return;
	CEditPatNameDlg dlg(this);
	m_bCanRefrush = FALSE;
	dlg.DoModal();
	m_bCanRefrush = TRUE;
	(((CNurseStationApp*)AfxGetApp())->m_pNurseWnd)->OnFrFlush();
}

BOOL CPaientView::ReviseListCtrPatName(const CString& paitLogid,const CString& newName)
{
	int count=m_view_paient.GetItemCount();
	for(int i=0;i<count;i++)
	{
		CString logid=m_view_paient.GetItemText(i,1);
		if(paitLogid==logid)
		{
			m_view_paient.SetItemText(i,4,newName);
			break;
		}
	}
	return TRUE;
}


void CPaientView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_F5:
		{
			(((CNurseStationApp*)AfxGetApp())->m_pNurseWnd)->OnFrFlush();
		}
		break;
	default:
		{

		}
	}

	CFormView::OnKeyUp(nChar, nRepCnt, nFlags);
}
