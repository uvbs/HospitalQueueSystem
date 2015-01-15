// NurseView.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "NurseView.h"
#include "SLZData.h"
#include "MyString.h"
#include "MyCommon.h"
#include "WriteLog.h"


// CNurseView

IMPLEMENT_DYNCREATE(CNurseView, CFormView)

CNurseView::CNurseView()
	: CFormView(CNurseView::IDD)
	//, m_pChatDlg(((CNurseStationApp*)AfxGetApp())->m_pChatDlg)
	//, m_pChatDlg(g_pDoctorCallerDlg)
	, m_ChatDlgDoModelFlag(FALSE)
	, m_strQueueNum(_T(""))
{
	//m_printer.Start();
}

CNurseView::~CNurseView()
{
	
}


void CNurseView::OnInitialUpdate( )
{
	CFormView::OnInitialUpdate();
	ShowLoginNurseName();
//	m_que_officename.SetReadOnly();
//	m_que_docname.SetReadOnly();
//	m_que_patname.SetReadOnly();
//	m_pait_lastdoc.SetReadOnly(TRUE);
//	m_pait_lastoffice.SetReadOnly(TRUE);
//	m_pait_logid.SetReadOnly(TRUE);
//	m_pait_name.SetReadOnly(TRUE);
	m_que_logid.SetReadOnly(TRUE);
	ShowOffice();
	m_check_quit.SetCheck(0);
	m_check_suspend.SetCheck(0);
	m_check_waitting.SetCheck(1);
	m_check_hacecall.SetCheck(0);
//	m_com_search.AddString(_T("按就诊号查询"));
//	m_com_search.AddString(_T("按患者姓名查询"));
//	m_com_search.AddString(_T("按医生姓名查询"));

//	SerchDoctor();
//	m_date=CTime::GetCurrentTime();

	int nEnable = MyCommon::GetProfileInt(
		_T("sys"), _T("ENABLE_TAKING_NUM"), 0, _T("\\SYS\\sysconfig.ini"));
	EnableTakingNum(nEnable == 1);

	m_printer.Start();
}

void CNurseView::ShowLoginNurseName()
{
	CString str=_T("护士站:");
	CString NurseName=theApp.GetLoginNursename();
	str+=NurseName;
	m_login_nursename.SetWindowText(str);
}

void CNurseView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_STATIC_QUEINFO, m_static_queinfo);
	DDX_Control(pDX, IDC_LOGIN_NURSENAME, m_login_nursename);
	//	DDX_Control(pDX, IDC_PAIT_LOGID, m_pait_logid);
	//	DDX_Control(pDX, IDC_PAIT_LASTOFFICE, m_pait_lastoffice);
	//	DDX_Control(pDX, IDC_PAIT_NAME, m_pait_name);
	//	DDX_Control(pDX, IDC_PAIT_LASTDOC, m_pait_lastdoc);
	//	DDX_Control(pDX, IDC_COMBO_LASTOFFICE, m_com_lastoffice);
	//	DDX_Control(pDX, IDC_COMBO_SETDOCTOR, m_com_setdoctor);
	DDX_Control(pDX, IDC_NURSEVIEW_PATLGOID, m_que_logid);
	DDX_Control(pDX, IDC_NURSEVIEW_PAITNAME, m_que_patname);
	//	DDX_Control(pDX, IDC_NURSEVIEW_OFFICENAME, m_que_officename);
	//	DDX_Control(pDX, IDC_NURSEVIEW_DOCNAME, m_que_docname);
	//DDX_Control(pDX, IDC_COMEX_QUENAME, m_comex_quename);
	DDX_Control(pDX, IDC_COM_QUENAME, m_comex_quename);
	DDX_Control(pDX, IDC_COM_QUEDOCNAME, m_com_quedocname);
	DDX_Control(pDX, IDC_CHECK_QUITQUE, m_check_quit);
	DDX_Control(pDX, IDC_CHECK_HAVECALL, m_check_hacecall);
	DDX_Control(pDX, IDC_CHECK_QUEING, m_check_waitting);
	DDX_Control(pDX, IDC_CHECK_SUSPEND, m_check_suspend);
	//	DDX_Control(pDX, IDC_COM_SEARCH, m_com_search);
	//	DDX_Control(pDX, IDC_SEARCH_TEXT, m_search_text);
	//	DDX_Control(pDX, IDC_DATE_SEARCH, m_date_picker);
	//	DDX_Control(pDX, IDC_NURSEVIEW_SEARCHTEXT, m_ed_searchcontext);
	DDX_Control(pDX, IDC_COM_QUEDOCTOR, m_com_calldoc);
	DDX_Control(pDX, IDC_BN_GETQUENUM, m_btnTakingNum);
	DDX_Text(pDX, IDC_NURSEVIEW_QUEUE_NUM, m_strQueueNum);
	DDX_Control(pDX, IDC_NURSEVIEW_QUEUE_NUM, m_editQueueNum);
}

BEGIN_MESSAGE_MAP(CNurseView, CFormView)
	ON_WM_SIZE()
	ON_WM_INITMENUPOPUP()
	ON_BN_CLICKED(IDC_BN_CHAT, &CNurseView::OnBnClickedBnChat)
//	ON_BN_CLICKED(IDC_BN_FLUSHPAIT,&CNurseView::OnBnClickedFlushPait)
//	ON_BN_CLICKED(IDC_BN_CHANGEOFFICE, &CNurseView::OnBnClickedBnChangeoffice)
//	ON_CBN_SELCHANGE(IDC_COMBO_LASTOFFICE, &CNurseView::OnCbnSelchangeComboLastoffice)
	ON_BN_CLICKED(IDC_BN_GETQUENUM, &CNurseView::OnBnClickedBnGetquenum)
	
	ON_CBN_SELCHANGE(IDC_COM_QUENAME, &CNurseView::OnCbnSelchangeComQuename)
	ON_BN_CLICKED(IDC_CHECK_SUSPEND, &CNurseView::OnBnClickedCheckSuspend)
	ON_BN_CLICKED(IDC_CHECK_QUEING, &CNurseView::OnBnClickedCheckQueing)
	ON_BN_CLICKED(IDC_CHECK_HAVECALL, &CNurseView::OnBnClickedCheckHavecall)
	ON_BN_CLICKED(IDC_CHECK_QUITQUE, &CNurseView::OnBnClickedCheckQuitque)
//	ON_CBN_SELCHANGE(IDC_COM_SEARCH, &CNurseView::OnCbnSelchangeComSearch)
//	ON_BN_CLICKED(IDC_BN_SEARCH, &CNurseView::OnBnClickedBnSearch)
//	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_SEARCH, &CNurseView::OnDtnDatetimechangeDateSearch)
//ON_BN_CLICKED(IDC_BUTTON4, &CNurseView::OnBnClickedButton4)
ON_BN_CLICKED(IDC_BN_CALL, &CNurseView::OnBnClickedBnCall)
ON_BN_CLICKED(IDC_BN_RECALL, &CNurseView::OnBnClickedBnRecall)
ON_BN_CLICKED(IDC_BN_WAIT, &CNurseView::OnBnClickedBnWait)
ON_BN_CLICKED(IDC_BN_QUITNUM, &CNurseView::OnBnClickedBnQuitnum)
END_MESSAGE_MAP()


// CNurseView 诊断

#ifdef _DEBUG
void CNurseView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNurseView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNurseView 消息处理程序

void CNurseView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	
	CRect rect;
	GetClientRect(&rect); // 获取当前客户区view大小

	CSize size;
	size.cx = rect.right - rect.left;
	size.cy = rect.bottom - rect.top;
	SetScrollSizes(MM_HIMETRIC, size); // 将CScrollView的大小设置为当前客户区大小

	/*
	CRect rect;
	GetClientRect(&rect); // 获取当前客户区view大小
	CSize size = GetTotalSize();  // 获取scroll view的总大小
	rect.right = rect.left + size.cx; // 根据scroll view大小重新确定当前view大小
	rect.bottom = rect.top + size.cy;
	rect.right = size.cx;
	rect.bottom = size.cy;
	*/
//	this->MoveWindow(&rect,TRUE);
}

void CNurseView::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CFormView::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

}

void CNurseView::OnBnClickedBnChat()
{
	((CNurseStationApp*)AfxGetApp())->GetController()->ShowChat();
}
/*
void CNurseView::OnBnClickedFlushPait()
{
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	pMainFrame->OnFrFlush();
}
*/
/*
void CNurseView::OnBnClickedBnChangeoffice()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CString logid=pMainFrame->GetPatientView()->GetSelectPaitLogid();
	CString sql=_T("");
	CString queue_call_id;
	int maxqueid,number;
	CString call_id=_T("");
	int index=m_com_lastoffice.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	CString office=_T("");
	m_com_lastoffice.GetLBText(index,office);
	CString office_id=m_pool.RequestBracket(office);
	sql.Format(_T("select max(q.queue_id)queue_id,count(q.status)number,max(o.NumStart)NumStart,max(o.call_id)call_id from queue q,Office o where q.office_id=o.office_id and (q.status=0 or q.status=3) and q.office_id='%s'"),office_id);
	if(pMainFrame->pbase->IsOpen())
	{
		CADORecordset* pre=new CADORecordset(pMainFrame->pbase);
		pre->Open(sql,CADORecordset::openQuery);
		pre->GetFieldValue(_T("number"),number);
		if(number==0)
		{
			pre->GetFieldValue(_T("NumStart"),maxqueid);
		}
		else
		{
			pre->GetFieldValue(_T("queue_id"),maxqueid);
		}
		pre->GetFieldValue(_T("call_id"),call_id);
		delete pre;
		pre=NULL;
	}
	maxqueid++;
	if(maxqueid<1000&&maxqueid>0)
	{
		//C_NumStart.Format(_T("%03d"), NumStart);
		queue_call_id.Format(_T("%03d"),maxqueid);
	}
	else if(maxqueid<10000&&maxqueid>0)
	{
		//C_NumStart.Format(_T("%04d"), NumStart);
		queue_call_id.Format(_T("%04d"),maxqueid);
	}
	else
	{
		//C_NumStart.Format(_T("%d"), NumStart);
		queue_call_id.Format(_T("%d"),maxqueid);
	}
	call_id+=queue_call_id;

	CString doctor_id=_T("");
	index=m_com_setdoctor.GetCurSel();
	if(index==CB_ERR)
	{
		doctor_id=_T("");
	}
	else
	{
		CString doctor=_T("");
		m_com_setdoctor.GetLBText(index,doctor);
		doctor_id=m_pool.RequestBracket(doctor);
	}
	if(doctor_id!=_T(""))
	{
		sql.Format(_T("update queue set office_id='%s',doctor_id='%s',queue_id='%d',queue_id_call='%s' where log_id='%s'"),
			office_id,doctor_id,maxqueid,call_id,logid);
		if(pMainFrame->pbase->IsOpen())
		{
			pMainFrame->pbase->Execute(sql);
		}
	}
	else
	{
		sql.Format(_T("update queue set office_id='%s',doctor_id=null,queue_id='%d',queue_id_call='%s' where log_id='%s'"),
			office_id,maxqueid,call_id,logid);
		if(pMainFrame->pbase->IsOpen())
		{
			pMainFrame->pbase->Execute(sql);
		}
	}
	pMainFrame->OnFrFlush();
}
*/
/*
void CNurseView::ShowData()
{
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CString logid=pMainFrame->GetPatientView()->GetSelectPaitLogid();
	CString name=pMainFrame->GetPatientView()->GetSelectPaitname();
	CString officename=pMainFrame->GetOfficeQueView()->GetSelectOfficename();
	CString docname=pMainFrame->GetPatientView()->GetSelectDocname();

//	m_pait_lastdoc.SetWindowText(docname);
//	m_pait_lastoffice.SetWindowText(officename);
//	m_pait_logid.SetWindowText(logid);
//	m_pait_name.SetWindowText(name);
	
}
*/

void CNurseView::ShowOffice()
{
//	int count=m_com_lastoffice.GetCount();
//	for(int i=count;i>=0;i--)
//	{
//		m_com_lastoffice.DeleteString(i);
//	}
	CString sql=_T("");
	CString name=_T("");
	CString office_id=_T("");
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
			if(!pre->IsBOF())
			{
				while(!pre->IsEOF())
				{
					pre->GetFieldValue(_T("name"),name);
					pre->GetFieldValue(_T("office_id"),office_id);
					CString str=_T("");
					str=name+ '(' + office_id + ')';
				//			m_com_lastoffice.AddString(str);
					m_comex_quename.AddString(str);
					pre->MoveNext();
				}
			}
			pre->Close();
		}
		delete pre;
		pre = NULL;
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseView::ShowOffice"));
	}
	if(m_comex_quename.GetCount()>0)
	{
		m_comex_quename.SetCurSel(0);
		OnCbnSelchangeComQuename();
	}
}

/*
void CNurseView::ShowDoctor()
{
	CString sql=_T("");
	CString docname=_T("");
	CString docID=_T("");
	int index=m_com_lastoffice.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	CString str_office=_T("");
	m_com_lastoffice.GetLBText(index,str_office);
	CString office_id=m_pool.RequestBracket(str_office);
	int count = m_com_setdoctor.GetCount();
	for(int i=count;i>=0;i--)
	{
		m_com_setdoctor.DeleteString(i);
	}
	//找到科室所对应的在线医生列表
	int nKey;//键
	USERSTATUS usbuf = {0};//医生客户端登录状态
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	if(pMainFrame->pbase->IsOpen())
	{
		sql.Format(_T("select d.name,d.doctor_id from doctor d,office o where d.office_id=o.office_id and o.office_id='%s'"),
			office_id);
		CADORecordset* pre=new CADORecordset(pMainFrame->pbase);
		pre->Open(sql,CADORecordset::openQuery);
		while(!pre->IsEOF())
		{
			pre->GetFieldValue(_T("doctor_id"),docID);
			POSITION pos = pMainFrame->m_pChatDlg->m_mapUserList.GetStartPosition();
			for(;pos;)
			{
				pMainFrame->m_pChatDlg->m_mapUserList.GetNextAssoc(pos,nKey,usbuf);
				if(docID==usbuf.wszUserID)
				{
					CString str=_T("");
					docname=usbuf.wszUserName;
					str=docname+'('+docID+')';
					m_com_setdoctor.AddString(str);
					break;
				}
			}
			pre->MoveNext();
		}
		delete pre;
		pre=NULL;
	}
}
*/
/*
void CNurseView::OnCbnSelchangeComboLastoffice()
{
	// TODO: 在此添加控件通知处理程序代码
//	ShowDoctor();
}
*/
/*
void CNurseView::OnBnClickedBnGetquenum()
{
	CString paitlogid=_T("");
	m_que_logid.GetWindowText(paitlogid);
	if(paitlogid!=_T(""))
	{
		CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
		pMainFrame->GetPatientView()->WriteDataToMyQueue(paitlogid);

		m_que_patname.SetWindowText(pMainFrame->GetPatientView()->GetAddPaitName());
		CString officeName=pMainFrame->GetPatientView()->GetAddPaitOfficeName();
		CString officeID=pMainFrame->GetPatientView()->GetAddPaitOfficeID();
		officeName=officeName+'('+officeID+')';
		int count=m_comex_quename.GetCount();
		CString getOfficeName=_T("");
		for(int i=0;i<count;i++)
		{
			m_comex_quename.GetLBText(i,getOfficeName);
			if(getOfficeName==officeName)
			{
				m_comex_quename.SetCurSel(i);
				break;
			}
		}
		if(m_comex_quename.GetCurSel()!=CB_ERR)
		{
			OnCbnSelchangeComQuename();
		}
		
		CString docName=pMainFrame->GetPatientView()->GetAddPaitDocName();
		CString docID=pMainFrame->GetPatientView()->GetAddPaitDocID();
		docName=docName+'('+docID+')';
		count=m_com_quedocname.GetCount();
		CString getDocName=_T("");
		for(int j=0;j<count;j++)
		{
			m_com_quedocname.GetLBText(j,getDocName);
			if(getDocName==docName)
			{
				m_com_quedocname.SetCurSel(j);
				break;
			}
		}
	}
	else
	{
		CString que_paitname=_T("");
		m_que_patname.GetWindowText(que_paitname);
		//if(que_paitname==_T("") || m_comex_quename.GetCurSel()==CB_ERR)
		if(m_comex_quename.GetCurSel()==CB_ERR)
		{
			return;
		}
		else
		{
			try
			{
				NoHisWriteDataToQueue();
			}
			catch(...)
			{
				
			}
			m_que_patname.SetWindowText(_T(""));
		}
	}
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_pNurseWnd->OnFrFlush();
}
*/
void CNurseView::OnBnClickedBnGetquenum()
{
	UpdateData(TRUE);
	CString strSerialId=_T("");
	m_que_logid.GetWindowText(strSerialId);
	CString patientName=_T("");
	m_que_patname.GetWindowText(patientName);
	//if(que_paitname==_T("") || m_comex_quename.GetCurSel()==CB_ERR)
	if(m_comex_quename.GetCurSel()==CB_ERR)
	{
		MessageBox(_T("请选择科室！"), _T("提示"));
		return;
	}
	else
	{
		try
		{
			int nRet = NoHisWriteDataToQueue(strSerialId);
			if(0 != nRet)
			{
				if(2 == nRet)
				{
					MessageBox(_T("该就诊号已存在！"), _T("提示"));
				}
				else
				{
					MessageBox(_T("数据库错误！"), _T("错误"));
				}
				return;
			}
		}
		catch(...)
		{
			MessageBox(_T("未知错误！"), _T("错误"));
			return;
		}
		m_que_patname.SetWindowText(_T(""));
		m_que_logid.SetWindowText(_T(""));
	}
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_pNurseWnd->OnFrFlush();
}

void CNurseView::OnCbnSelchangeComQuename()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sql=_T("");
	CString docname=_T("");
	CString docID=_T("");
	int index=m_comex_quename.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	CString str_office=_T("");
	m_comex_quename.GetLBText(index,str_office);
	CString office_id=m_pool.RequestBracket(str_office);
	m_com_quedocname.SetWindowText(_T(""));
	int count = m_com_quedocname.GetCount();
	for(int i=count;i>=0;i--)
	{
		m_com_quedocname.DeleteString(i);
	}
	m_mapto_combo.RemoveAll();
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;

	sql.Format(_T("select d.name,d.doctor_id from doctor d,office o where d.office_id=o.office_id and d.office_id='%s'"),
		office_id);
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
				DOCTOCOMBOINFO doctocominfo;
				while(!pre->IsEOF())
				{
					pre->GetFieldValue(_T("name"),doctocominfo.docname);
					pre->GetFieldValue(_T("doctor_id"),doctocominfo.docid);

					m_mapto_combo[m_com_quedocname.AddString(doctocominfo.docname)]=doctocominfo;

					pre->MoveNext();
				}
			}
			pre->Close();
		}
		delete pre;
		pre=NULL;
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseView::OnCbnSelchangeComQuename"));
	}
}

int CNurseView::NoHisWriteDataToQueue(CString strSerialId)
{
	m_editQueueNum.SetWindowText(_T(""));
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	
	CString deptName=_T("");
	m_que_patname.GetWindowText(deptName);
	CString que_paitOfficeID=_T("");
	m_comex_quename.GetLBText(m_comex_quename.GetCurSel(),que_paitOfficeID);
	DOCTOCOMBOINFO docinfo;
	int index=m_com_quedocname.GetCurSel();
	if(index!=CB_ERR)
	{
		docinfo=m_mapto_combo[index];
	}
	//排队的科室ID和医生ID
	CString Queue=m_pool.RequestBracket(que_paitOfficeID);
	CString officeName = m_pool.DeleRequest(que_paitOfficeID);
	
	CString docID=docinfo.docid;
	
	//插入sql语句
	CString insertSql=_T("");

	//排队
	CString Queue_id=_T("");//系统排队号码
	CString call_id=_T("");//id前缀
	int NumStart;//开始排队号码
	int NumEnd;//结束号码
	int AmLimit;//上午限制人数
	int PmLimit;//下午限制人数
	int DayLimit;//整天限制人数
	CString C_NumStart=_T("");
	int queNumber;//实际排队人数
	CString queue_id_his=_T("");
	//生成唯一ID


	//上午下午人数限制
	CTime CurrentTime=CTime::GetCurrentTime();
	int year=CurrentTime.GetYear();
	int month=CurrentTime.GetMonth();
	int day=CurrentTime.GetDay();
	CTime AmTime(year,month,day,12,0,0);
	CTime PmTime(year,month,day,23,59,59);
	//用于排队的类
	CNurseOfficeQueue office;

	int nTodayCount = 0;

	//获取当前对应科室中数据
	POSITION pos=pMainFrame->GetPatientView()->m_list_office.GetHeadPosition();
	while(pos!=NULL)
	{
		POSITION posLast=pos;
		office=pMainFrame->GetPatientView()->m_list_office.GetNext(pos);
		if(office.GetQueID()==Queue)
		{

			CADODatabase db;
			db.SetConnectionString(pMainFrame->m_strConn);
			try
			{
				db.Open();
			}
			catch (_com_error& e)
			{
				WriteLog::WriteDbErrLog(_T("CNurseView::NoHisWriteDataToQueue"));
				return 1;//数据库错误
			}
			
			CADORecordset rset(&db);

			
			CString log_ID;
			if(strSerialId.IsEmpty())
			{
				log_ID=GetOnlyID();//获取新就诊号
			}
			else
			{
				CString sql;
				sql.Format(_T("select log_id from queue where log_id='%s'"),
					strSerialId);
				try
				{
					rset.Open(sql,CADORecordset::openQuery);
					if(rset.IsBOF())
					{
						log_ID = strSerialId;
					}
					else
					{
						rset.Close();
						return 2;//输入的就诊号已存在
					}
					rset.Close();
				}
				catch (_com_error& e)
				{
					return 1;//数据库错误
				}
			}

			CString sql;
			sql.Format(_T("select count(log_id) from queue where office_id='%s' and regtime>cast(CONVERT(varchar(100),GETDATE(),23) as datetime)"),
				Queue);
			try
			{
				rset.Open(sql,CADORecordset::openQuery);
				rset.GetFieldValue(0, nTodayCount);
				rset.Close();
			}
			catch (_com_error& e)
			{
				return 1;//数据库错误
			}

			//NumStart=office.GetQuenumStart();
			NumStart = nTodayCount + 1;
			NumEnd=office.GetQuenumEnd();
			if(NumEnd <= 0)
			{
				NumEnd = 100000;
			}
			AmLimit=office.GetAmLimitPatient();
			PmLimit=office.GetPmLimitPatient();
			DayLimit=office.GetDayLimitPatient();
			queNumber=office.GetQueNumber();//实际排队人数
			call_id=office.GetQueCallID();
			if(CurrentTime<=AmTime)
			{
				BOOL judge=FALSE;
				if(AmLimit<=0)
				{
					judge=NumStart<=NumEnd && queNumber<DayLimit && 
						NumEnd>=NumStart;
				}
				else
				{
					judge=NumStart<=NumEnd && NumEnd>=NumStart &&
						queNumber<AmLimit;
				}
				if(judge)
				{
					if(NumStart<1000&&NumStart>0)
					{
						C_NumStart.Format(_T("%03d"), NumStart);

					}
					else if(NumStart<10000&&NumStart>=1000)
					{
						C_NumStart.Format(_T("%04d"), NumStart);
					}
					else
					{
						C_NumStart.Format(_T("%d"), NumStart);
					}
					call_id+=C_NumStart;

					if(!m_strQueueNum.IsEmpty())
					{
						call_id = m_strQueueNum;
					}

					if(docID!=_T(""))
					{
						insertSql.Format(_T("insert into Queue(log_id,reg_id,queue_id,patient_name,office_id,doctor_id,regtime,status,priority,queue_id_call) values('%s','%s',%d,'%s','%s','%s',getdate(),%d,%d,'%s')"),
							log_ID,log_ID,NumStart,deptName,Queue,docID,qsInLine,priZero,call_id);
					}
					else
					{
						insertSql.Format(_T("insert into Queue(log_id,reg_id,queue_id,patient_name,office_id,regtime,status,priority,queue_id_call) values('%s','%s',%d,'%s','%s',getdate(),%d,%d,'%s')"),
							log_ID,log_ID,NumStart,deptName,Queue,qsInLine,priZero,call_id);
					}
					try
					{
						db.Execute(insertSql);
					}
					catch (_com_error& e)
					{
						WriteLog::WriteDbErrLog(_T("CNurseView::NoHisWriteDataToQueue2"));
						return FALSE;//数据库错误
					}



					//////////////////////////////////////打印
					if(m_printer.m_isUsePrint)
					{
						EnumPrintStaus satus = m_printer.CheckPrinterStatus();
						if(satus == enumPrintPrinting || satus == enumPrintNormal)
						{
							SLZData data;
							data.SetSerialId(log_ID);
							data.SetQueNum(call_id);
							data.SetPatientName(deptName);
							data.SetTakingNumTime(CTime::GetCurrentTime());
							data.SetDoctorName(docinfo.docname);
							data.SetOfficeName(officeName);
							m_printer.Print(data,GetQueNum(Queue));
						}
						else
						{
							switch(satus)
							{
							case enumPrintErr:
								MessageBox(_T("打印机出现故障,请检查打印机硬件"),_T("注意"),
									MB_OK|MB_ICONERROR);
								break;
							case enumPrintPaperOut:
								MessageBox(_T("打印机缺纸"),_T("注意"),
									MB_OK|MB_ICONERROR);
								break;
							case enumPrintOffline:
								MessageBox(_T("打印机掉线"),_T("注意"),
									MB_OK|MB_ICONERROR);
								break;
							case enumPrintNoDefPrinter:
								MessageBox(_T("没有默认打印机"),_T("注意"),
									MB_OK|MB_ICONERROR);
								break;
							}
						}
					}
					//////////////////////////////////////

					NumStart++;
					queNumber++;
					office.SetQuenumStart(NumStart);
					office.SetQueNumber(queNumber);
					pMainFrame->GetPatientView()->m_list_office.SetAt(posLast,office);
					break;
				}
			}
			else if(CurrentTime<=PmTime)
			{
				BOOL judge=FALSE;
				if(PmLimit<=0)
				{
					judge=NumStart<=NumEnd && queNumber<DayLimit && 
						NumEnd>=NumStart;
				}
				else
				{
					judge=NumStart<=NumEnd && NumEnd>=NumStart &&
						queNumber<PmLimit;
				}
				if(judge)
				{
					if(NumStart<1000&&NumStart>0)
					{
						C_NumStart.Format(_T("%03d"), NumStart);

					}
					else if(NumStart<10000&&NumStart>=1000)
					{
						C_NumStart.Format(_T("%04d"), NumStart);
					}
					else
					{
						C_NumStart.Format(_T("%d"), NumStart);
					}
					call_id+=C_NumStart;

					if(!m_strQueueNum.IsEmpty())
					{
						call_id = m_strQueueNum;
					}

					if(docID!=_T(""))
					{
						insertSql.Format(_T("insert into Queue(log_id,reg_id,queue_id,patient_name,office_id,doctor_id,regtime,status,priority,queue_id_call) values('%s','%s',%d,'%s','%s','%s',getdate(),%d,%d,'%s')"),
							log_ID,log_ID,NumStart,deptName,Queue,docID,qsInLine,priZero,call_id);
					}
					else
					{
						insertSql.Format(_T("insert into Queue(log_id,reg_id,queue_id,patient_name,office_id,regtime,status,priority,queue_id_call) values('%s','%s',%d,'%s','%s',getdate(),%d,%d,'%s')"),
							log_ID,log_ID,NumStart,deptName,Queue,qsInLine,priZero,call_id);
					}
					try
					{
						db.Execute(insertSql);
					}
					catch (_com_error& e)
					{
						WriteLog::WriteDbErrLog(_T("CNurseView::NoHisWriteDataToQueue3"));
					}

					//////////////////////////////////////打印
					if(m_printer.m_isUsePrint)
					{
						EnumPrintStaus satus = m_printer.CheckPrinterStatus();
						if(satus == enumPrintPrinting || satus == enumPrintNormal)
						{
							SLZData data;
							data.SetSerialId(log_ID);
							data.SetQueNum(call_id);
							data.SetPatientName(deptName);
							data.SetTakingNumTime(CTime::GetCurrentTime());
							data.SetDoctorName(docinfo.docname);
							data.SetOfficeName(officeName);
							m_printer.Print(data,GetQueNum(Queue));
						}
						else
						{
							switch(satus)
							{
							case enumPrintErr:
								MessageBox(_T("打印机出现故障,请检查打印机硬件"),_T("注意"),
									MB_OK|MB_ICONERROR);
								break;
							case enumPrintPaperOut:
								MessageBox(_T("打印机缺纸"),_T("注意"),
									MB_OK|MB_ICONERROR);
								break;
							case enumPrintOffline:
								MessageBox(_T("打印机掉线"),_T("注意"),
									MB_OK|MB_ICONERROR);
								break;
							case enumPrintNoDefPrinter:
								MessageBox(_T("没有默认打印机"),_T("注意"),
									MB_OK|MB_ICONERROR);
								break;
							}
						}
					}
					//////////////////////////////////////
					NumStart++;
					queNumber++;
					office.SetQuenumStart(NumStart);
					office.SetQueNumber(queNumber);
					pMainFrame->GetPatientView()->m_list_office.SetAt(posLast,office);
					break;
				}
			}
			try
			{
				db.Close();
			}
			catch (_com_error& e)
			{
				WriteLog::WriteDbErrLog(_T("CNurseView::NoHisWriteDataToQueue4"));
				return 1;
			}
		}
	}
	return 0;
}


int CNurseView::GetSrand(const int &max)
{
	time_t t;	
	srand((unsigned) time(&t));	
	return	rand() % max;
}

CString CNurseView::GetOnlyID()
{
	CString m_id;
	//CTime now=CTime::GetCurrentTime();
	//static int nCount;
	//m_id.Format(_T("%04d%02d%02d%02d%02d%02d%04d%04d"),
	//	now.GetYear(),now.GetMonth(),now.GetDay(),now.GetHour(),
	//	now.GetSecond(),now.GetMinute(),GetSrand(9999),++nCount);
	SYSTEMTIME t;
	::GetLocalTime(&t);
	m_id.Format(_T("%04d%02d%02d%02d%02d%02d%03d"),
		t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);
	return m_id;
}
void CNurseView::OnBnClickedCheckSuspend()
{
	// TODO: 在此添加控件通知处理程序代码
	//theApp.m_pNurseWnd->OnFrShowsuspend();
	::PostMessage(theApp.m_pNurseWnd->m_hWnd,WM_COMMAND,ID_FR_SHOWSUSPEND,NULL);
}

void CNurseView::OnBnClickedCheckQueing()
{
	// TODO: 在此添加控件通知处理程序代码
	//theApp.m_pNurseWnd->OnFrShowwaitting();
	::PostMessage(theApp.m_pNurseWnd->m_hWnd,WM_COMMAND,ID_FR_SHOWWAITTING,NULL);
}

void CNurseView::OnBnClickedCheckHavecall()
{
	// TODO: 在此添加控件通知处理程序代码
	//theApp.m_pNurseWnd->OnFrShowcall();
	::PostMessage(theApp.m_pNurseWnd->m_hWnd,WM_COMMAND,ID_FR_SHOWCALL,NULL);
}

void CNurseView::OnBnClickedCheckQuitque()
{
	// TODO: 在此添加控件通知处理程序代码
	//theApp.m_pNurseWnd->OnFrShowquit();
	::PostMessage(theApp.m_pNurseWnd->m_hWnd,WM_COMMAND,ID_FR_SHOWQUIT,NULL);
}
/*
void CNurseView::OnCbnSelchangeComSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_com_search.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	switch(index)
	{
	case 0:
		m_search_text.SetWindowText(_T("就诊号"));
		break;
	case 1:
		m_search_text.SetWindowText(_T("患者姓名"));
		break;
	case 2:
		m_search_text.SetWindowText(_T("医生姓名"));
		break;
	}
}
*/
/*
void CNurseView::OnBnClickedBnSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CString c_date=m_date.Format("%Y-%m-%d");
	CString context=_T("");
	
	int index=m_com_search.GetCurSel();
	m_ed_searchcontext.GetWindowText(context);
	pMainFrame->GetOfficeQueView()->SearchData(c_date,context,index);
}
*/
/*
void CNurseView::OnDtnDatetimechangeDateSearch(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_date_picker.GetTime(m_date);
	*pResult = 0;
}
*/

void CNurseView::SerchDoctor()
{
	m_com_calldoc.SetWindowText(_T(""));
	int count=m_com_calldoc.GetCount();
	for(int i=count;i>=0;i--)
	{	
		m_com_calldoc.DeleteString(i);
	}
	m_maptocall_combo.RemoveAll();
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CString sql=_T("");
	sql.Format(_T("select d.name,d.doctor_id from doctor d,office o where d.office_id=o.office_id and d.office_id='%s'"),
		pMainFrame->GetOfficeQueView()->GetOfficeID());
	try
	{
		CADODatabase db;
		db.SetConnectionString(pMainFrame->m_strConn);
		db.Open();
		CADORecordset *pre=new CADORecordset(&db);
		DOCTOCOMBOINFO doctocomboinfo;
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
			{
				while(!pre->IsEOF())
				{
					pre->GetFieldValue(_T("name"),doctocomboinfo.docname);
					pre->GetFieldValue(_T("doctor_id"),doctocomboinfo.docid);
					m_maptocall_combo[m_com_calldoc.AddString(doctocomboinfo.docname)]=doctocomboinfo;
					pre->MoveNext();
				}
			}
			pre->Close();
		}
		delete pre;
		pre=NULL;
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseView::SerchDoctor"));
	}
}

void CNurseView::OnBnClickedBnCall()
{
	// TODO: 在此添加控件通知处理程序代码
	CString doctor_id=GetCallDoctorId();
	((CNurseStationApp*)AfxGetApp())->GetController()->SendCmd(doctor_id,_T(""),dcCmdCall);
}

void CNurseView::OnBnClickedBnRecall()
{
	// TODO: 在此添加控件通知处理程序代码
	CString doctor_id=GetCallDoctorId();
	((CNurseStationApp*)AfxGetApp())->GetController()->SendCmd(doctor_id,_T(""),dcCmdRecall);
}

void CNurseView::OnBnClickedBnWait()
{
	// TODO: 在此添加控件通知处理程序代码
	CString doctor_id=GetCallDoctorId();
	((CNurseStationApp*)AfxGetApp())->GetController()->SendCmd(doctor_id,_T(""),dcCmdWait);
}

void CNurseView::OnBnClickedBnQuitnum()
{
	// TODO: 在此添加控件通知处理程序代码
	CString doctor_id=GetCallDoctorId();
	((CNurseStationApp*)AfxGetApp())->GetController()->SendCmd(doctor_id,_T(""),dcCmdDiscard);

}

CString CNurseView::GetCallDoctorId()
{
	int index=m_com_calldoc.GetCurSel();
	if(index!=CB_ERR)
	{
		DOCTOCOMBOINFO docinfo=m_maptocall_combo[index];
		return docinfo.docid;
	}
	else
	{
		return _T("");
	}
}

int CNurseView::GetQueNum(const CString& queID)
{
	if(queID.IsEmpty())return 0;
	int countNum=0;//等待人数
	CString searchSql;
	searchSql.Format(_T("select count(log_id) count_num from queue where office_id='%s' and status='%d' and convert(varchar(10),getdate(),120)=convert(varchar(10),regtime,120)"),
		queID,qsInLine);
	
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	
	try
	{
		CADODatabase db;
		db.SetConnectionString(pMainFrame->m_strConn);
		db.Open();
		CADORecordset *pre=new CADORecordset(&db);
		if(pre->Open(searchSql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
			{
				pre->GetFieldValue(_T("count_num"),countNum);
			}
			pre->Close();
		}
		delete pre;
		pre = NULL;
		db.Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CNurseView::GetQueNum"));
	}
	return countNum;
}

void CNurseView::EnableTakingNum(BOOL bEnable)
{
	if(bEnable)
	{
		m_btnTakingNum.EnableWindow(TRUE);
		m_comex_quename.EnableWindow(TRUE);
		m_com_quedocname.EnableWindow(TRUE);
		m_que_patname.EnableWindow(TRUE);
		m_editQueueNum.EnableWindow(TRUE);
	}
	else
	{
		m_btnTakingNum.EnableWindow(FALSE);
		m_comex_quename.EnableWindow(FALSE);
		m_com_quedocname.EnableWindow(FALSE);
		m_que_patname.EnableWindow(FALSE);
		m_editQueueNum.EnableWindow(FALSE);
	}
}