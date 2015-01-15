// PropQueSettting.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PropQueSettting.h"
#include "MyString.h"
#include "MyCommon.h"

//extern void MyWriteConsole(CString str);


// CPropQueSettting 对话框

IMPLEMENT_DYNAMIC(CPropQueSettting, CPropertyPage)

CPropQueSettting::CPropQueSettting()
	: CPropertyPage(CPropQueSettting::IDD)
	, m_edit_QueName(_T(""))
	, m_edit_QueCallName(_T(""))
	, m_edit_QueID(_T(""))
	, m_edit_QueCallID(_T(""))
	, m_edit_QueStartNum(_T(""))
	, m_edit_QueEndNum(_T(""))
	, m_edit_AmLimit(_T(""))
	, m_edit_PmLimit(_T(""))
	, m_edit_DayLimit(_T(""))
	, m_consult_name(_T(""))
	, m_consult_callname(_T(""))
	, m_wnd_scrrenid(_T(""))
	, m_evaluator_id(_T(""))
	, m_caller_id(_T(""))
	, m_comprehwnd_id(_T(""))
//	, m_ed_thruscreenip(_T(""))
{
	m_pbase=new CADODatabase;
	m_phisDataBase=new CADODatabase;
	m_dataDrive=m_baseConfig.GetDataDrive();
	m_baseIP=m_baseConfig.GetServerIP();
	m_basePassWord=m_baseConfig.GetServerPassword();
	m_baseAccount=m_baseConfig.GetServerAcount();
	
	m_pre=NULL;
}

CPropQueSettting::~CPropQueSettting()
{
	if(m_pbase)
	{
		delete m_pbase;
		m_pbase=NULL;
	}
	if(m_phisDataBase)
	{
		delete m_phisDataBase;
		m_phisDataBase=NULL;
	}
	if(m_pre)
	{
		delete m_pre;
		m_pre=NULL;
	}
}

void CPropQueSettting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NURESQUEUE, m_list_nofficeque);
	DDX_Text(pDX, IDC_EDIT_NURSEQUENAME, m_edit_QueName);
	DDX_Text(pDX, IDC_EDIT_NURSECALLNAME, m_edit_QueCallName);
	DDX_Text(pDX, IDC_EDIT_NURSEQUEID, m_edit_QueID);
	DDX_Text(pDX, IDC_EDIT_NURSECALLID, m_edit_QueCallID);
	DDX_Text(pDX, IDC_EDIT_NURSEQUESTARTNUM, m_edit_QueStartNum);
	DDX_Text(pDX, IDC_EDIT_NURSEQUEENDNUM, m_edit_QueEndNum);
	DDX_Text(pDX, IDC_EDIT_AMLIMIT, m_edit_AmLimit);
	DDX_Text(pDX, IDC_EDIT_PM_LIMIT, m_edit_PmLimit);
	DDX_Text(pDX, IDC_EDIT_DAYLIMIT, m_edit_DayLimit);
	DDX_Control(pDX, IDC_OFFICE_CONSULT, m_list_consult);
	DDX_Text(pDX, IDC_ED_CONSULT_NAME, m_consult_name);
	DDX_Text(pDX, IDC_ED_CONSULT_CALLNAME, m_consult_callname);
	DDX_Text(pDX, IDC_ED_WND_SCRRENID, m_wnd_scrrenid);
	//DDX_Text(pDX, IDC_ED_EVALUATORID, m_evaluator_id);
	//DDX_Text(pDX, IDC_ED_CALLERID, m_caller_id);
	DDX_Text(pDX, IDC_ED_COMPREHWNDID, m_comprehwnd_id);
	//	DDX_Text(pDX, IDC_ED_WND_GIP, m_ed_thruscreenip);
	//	DDX_Text(pDX, IDC_ED_GADD, m_ed_thruscreenid);
	DDX_Control(pDX, IDC_COMBO_RTB, m_com_stdinfo);
	DDX_Control(pDX, IDC_COM_CONRTB, m_com_consultRtb);
	DDX_Control(pDX, IDC_COM_THROUGHID, m_com_throughWnd);
}


BEGIN_MESSAGE_MAP(CPropQueSettting, CPropertyPage)
	ON_BN_CLICKED(IDC_BN_ADDQUE, &CPropQueSettting::OnBnClickedBnAddque)
	ON_BN_CLICKED(IDC_BN_DELQUE, &CPropQueSettting::OnBnClickedBnDelque)
	ON_LBN_SELCHANGE(IDC_LIST_NURESQUEUE, &CPropQueSettting::OnLbnSelchangeListNuresqueue)
	ON_EN_CHANGE(IDC_EDIT_NURSEQUENAME, &CPropQueSettting::OnEnChangeEditNursequename)
	ON_EN_CHANGE(IDC_EDIT_NURSECALLNAME, &CPropQueSettting::OnEnChangeEditNursecallname)
	ON_EN_CHANGE(IDC_EDIT_NURSEQUEID, &CPropQueSettting::OnEnChangeEditNursequeid)
	ON_EN_CHANGE(IDC_EDIT_NURSECALLID, &CPropQueSettting::OnEnChangeEditNursecallid)
	ON_EN_CHANGE(IDC_EDIT_NURSEQUESTARTNUM, &CPropQueSettting::OnEnChangeEditNursequestartnum)
	ON_EN_CHANGE(IDC_EDIT_NURSEQUEENDNUM, &CPropQueSettting::OnEnChangeEditNursequeendnum)
	ON_EN_CHANGE(IDC_EDIT_AMLIMIT, &CPropQueSettting::OnEnChangeEditAmlimit)
	ON_EN_CHANGE(IDC_EDIT_PM_LIMIT, &CPropQueSettting::OnEnChangeEditPmLimit)
	ON_EN_CHANGE(IDC_EDIT_DAYLIMIT, &CPropQueSettting::OnEnChangeEditDaylimit)

	ON_EN_CHANGE(IDC_ED_CONSULT_NAME, &CPropQueSettting::OnEnChangeEdConsultName)
	ON_EN_CHANGE(IDC_ED_CONSULT_CALLNAME, &CPropQueSettting::OnEnChangeEdConsultCallname)
	ON_EN_CHANGE(IDC_ED_WND_SCRRENID, &CPropQueSettting::OnEnChangeEdWndScrrenid)
	ON_EN_CHANGE(IDC_ED_COMPREHWNDID, &CPropQueSettting::OnEnChangeEdComprehwndid)
	
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OFFICE_CONSULT, &CPropQueSettting::OnLvnItemchangedOfficeConsult)
//	ON_BN_CLICKED(IDC_BN_REVISE, &CPropQueSettting::OnBnClickedBnRevise)
	ON_BN_CLICKED(IDC_BN_DELCONSULT, &CPropQueSettting::OnBnClickedBnDelconsult)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_OFFICE_CONSULT, &CPropQueSettting::OnNMCustomdrawOfficeConsult)
	ON_BN_CLICKED(IDC_BN_ADDCONSULT, &CPropQueSettting::OnBnClickedBnAddconsult)
	ON_BN_CLICKED(IDC_BN_FLUSHHIS, &CPropQueSettting::OnBnClickedBnFlushhis)
//	ON_EN_CHANGE(IDC_ED_WND_GIP, &CPropQueSettting::OnEnChangeEdWndGip)
//	ON_EN_CHANGE(IDC_ED_GADD, &CPropQueSettting::OnEnChangeEdGadd)
	ON_CBN_SELCHANGE(IDC_COMBO_RTB, &CPropQueSettting::OnCbnSelchangeComboRtb)
	ON_CBN_SELCHANGE(IDC_COM_CONRTB, &CPropQueSettting::OnCbnSelchangeComConrtb)
	ON_CBN_SELCHANGE(IDC_COM_THROUGHID, &CPropQueSettting::OnCbnSelchangeComThroughid)
END_MESSAGE_MAP()


// CPropQueSettting 消息处理程序

void CPropQueSettting::OnBnClickedBnAddque()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listquenum=m_list_nofficeque.GetCount()+1;
	CString str1;
	str1.Format(_T("%d"),m_listquenum);
	CString str=_T("科室")+str1;
	m_list_nofficeque.AddString(str);
	int nLast = m_list_nofficeque.GetCount() - 1;
	if (nLast >= 0)
		m_list_nofficeque.SetCurSel(nLast);

	m_list_nofficeque.GetText(nLast,m_edit_QueName);
	m_list_nofficeque.GetText(nLast,m_edit_QueCallName);

	m_edit_AmLimit=_T("-1");
	m_edit_PmLimit=_T("-1");
	m_edit_DayLimit=_T("-1");

	m_edit_QueStartNum=_T("-1");
	m_edit_QueEndNum=_T("-1");
//	m_poolManage.intToCString(m_listquenum,m_edit_QueID);
	m_edit_QueID=str1;
	m_edit_QueCallID=_T("");
//	m_com_stdinfo.SetCurSel(0);
	
	CNurseOfficeQueue que;

	que.SetQueName(m_edit_QueName);
	que.SetQueCallName(m_edit_QueName);
	que.SetQueCallID(m_edit_QueCallID);
	que.SetQueID(m_edit_QueID);

	int i_AmLimit;
	m_poolManage.CStringToint(i_AmLimit,m_edit_AmLimit);
	que.SetAmLimitPatient(i_AmLimit);

	int i_PmLimit;
	m_poolManage.CStringToint(i_PmLimit,m_edit_PmLimit);
	que.SetPmLimitPatient(i_PmLimit);

	int i_DayLimit;
	m_poolManage.CStringToint(i_DayLimit,m_edit_DayLimit);
	que.SetDayLimitPatient(i_DayLimit);

	int i_QueStartNum;
	m_poolManage.CStringToint(i_QueStartNum,m_edit_QueStartNum);
	que.SetQuenumStart(i_QueStartNum);

	int i_QueEndNum;
	m_poolManage.CStringToint(i_QueEndNum,m_edit_QueEndNum);
	que.SetQuenumEnd(i_QueEndNum);

	m_listNuOfQue.AddTail(que);
	m_listquenum++;
//	m_com_stdinfo.SetCurSel(CB_ERR);
	
	UpdateData(FALSE);
	SetModified();
	OnLbnSelchangeListNuresqueue();
	
}

void CPropQueSettting::OnBnClickedBnDelque()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_listNuOfQue.GetCount()==0)
	{
		return;
	}
	int index=m_list_nofficeque.GetCurSel();
	CString name=_T("");
	m_list_nofficeque.GetText(index,name);
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	CNurseOfficeQueue que;

	if(index==m_list_nofficeque.GetCount()-1 && m_list_nofficeque.GetCount()==1)
	{
		m_list_nofficeque.DeleteString(index);
	}
	else if(index==m_list_nofficeque.GetCount()-1)
	{
		m_list_nofficeque.DeleteString(index);
		m_list_nofficeque.SetCurSel(index-1);
	}
	else
	{
		m_list_nofficeque.DeleteString(index);
		m_list_nofficeque.SetCurSel(index);
	}
	while(pos!=NULL)
	{
		POSITION posdel=pos;
		que=m_listNuOfQue.GetNext(pos);
		if(que.GetQueName()==name)
		{
			que.m_list_officeConsult.RemoveAll();//删除下层链表
			m_listNuOfQue.RemoveAt(posdel);
			m_list_delNurseOffQue.AddTail(que);
			break;
		}
	}
	if(m_listNuOfQue.GetCount()>0)
	{
		OnLbnSelchangeListNuresqueue();
	}
	else
	{
		m_edit_QueEndNum=_T("");
		m_edit_AmLimit=_T("");
		m_edit_DayLimit=_T("");
		m_edit_PmLimit=_T("");
		m_edit_QueCallID=_T("");
		m_edit_QueCallName=_T("");
		m_edit_QueStartNum=_T("");
		m_edit_QueID=_T("");
		m_edit_QueName=_T("");
		UpdateData(FALSE);
	}
	SetModified();
}

BOOL CPropQueSettting::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	ListView_SetExtendedListViewStyle(m_list_consult.m_hWnd, LVS_EX_FULLROWSELECT |  LVS_EX_GRIDLINES);
	CRect rect;
	m_list_consult.GetClientRect(&rect);

	m_list_consult.InsertColumn(0,_T("诊室编号"),LVCFMT_LEFT,0,-1);
	m_list_consult.InsertColumn(1,_T("诊室名称"),LVCFMT_LEFT,15*rect.Width()/100,-1);
	m_list_consult.InsertColumn(2,_T("呼叫名称"),LVCFMT_LEFT,15*rect.Width()/100,-1);
	//m_list_consult.InsertColumn(2,_T("呼叫器"),LVCFMT_LEFT,rect.Width()/7,-1);
	m_list_consult.InsertColumn(3,_T("窗口屏"),LVCFMT_LEFT,10*rect.Width()/100,-1);
	//m_list_consult.InsertColumn(4,_T("评价器"),LVCFMT_LEFT,rect.Width()/7,-1);
	m_list_consult.InsertColumn(4,_T("综合屏"),LVCFMT_LEFT,10*rect.Width()/100,-1);
//	m_list_consult.InsertColumn(5,_T("通屏IP"),LVCFMT_LEFT,15*rect.Width()/100,-1);
	m_list_consult.InsertColumn(5,_T("通屏"),LVCFMT_LEFT,15*rect.Width()/100,-1);
	m_list_consult.InsertColumn(6,_T("机顶盒"),LVCFMT_LEFT,20*rect.Width()/100,-1);

	ConnectToBase();
	/**********************从his中读office数据********************/
//	if(ConnectToHis())
//	{
//		ReadOfficeInfoFromHis();
//	}
	/************************************************************/
	FlushMapToStbInfo();//读液晶显示器
	FlushMapToThroughWnd();//读通屏
	WriteDataToList();

	if(m_pbase->IsOpen())
	{
		m_pre=new CADORecordset(m_pbase);
	}
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos!=NULL)
	{
		CString name=_T("");
		name=m_listNuOfQue.GetNext(pos).GetQueName();
		m_list_nofficeque.AddString(name);
	}
	m_list_nofficeque.SetCurSel(0);
	OnLbnSelchangeListNuresqueue();

	//m_consultID=GetMaxConsultID();
	
//	m_list_consult.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
//	m_list_consult.EnsureVisible(0, TRUE);
	return TRUE;
}

void CPropQueSettting::WriteDataToList()
{
	if(m_pbase->IsOpen())
	{
		m_listNuOfQue.RemoveAll();
		CADORecordset* pre=new CADORecordset(m_pbase);
		CADORecordset* pConre=new CADORecordset(m_pbase);
		CString sql=_T("select * from Office order by name");
		CString consultSql=_T("");
		

		CString queName=_T("");
		CString queCallName=_T("");
		CString queID=_T("");
		CString queIDCallName=_T("");
		
//		CString stb_name;
//		STDINFO stdinfo;
		

		CString queNumStart=_T("");
		CString queNumEnd=_T("");
		CString AmLimitPatient=_T("");
		CString PmLimitPatient=_T("");
		CString DayLimitPatient=_T("");
		
		int i_queNumStart=0;
		int i_queNumEnd=0;
		int i_AmLimitPatient=0;
		int i_PmLimitPatient=0;
		int i_DayLimitPatient=0;
		BOOL  hisFlag = FALSE;
		//int std_id=0;
		CString strStbId;
		
		int serial_id=0;
		//读科室/诊室到链表
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
			{
				while(!pre->IsEOF())
				{
					CNurseOfficeQueue officeQue;
					pre->GetFieldValue(_T("office_id"),queID);
					pre->GetFieldValue(_T("call_id"),queIDCallName);
					pre->GetFieldValue(_T("name"),queName);
					pre->GetFieldValue(_T("call_name"),queCallName);

					pre->GetFieldValue(_T("NumStart"),queNumStart);
					pre->GetFieldValue(_T("NumEnd"),queNumEnd);
					pre->GetFieldValue(_T("AmLimit"),AmLimitPatient);
					pre->GetFieldValue(_T("PmLimit"),PmLimitPatient);
					pre->GetFieldValue(_T("DayLimit"),DayLimitPatient);
					pre->GetFieldValue(_T("stb_id"),strStbId);
					pre->GetFieldValue(_T("serial_id"),serial_id);
				
					pre->GetFieldValue(_T("his_flag"),hisFlag);
					

//					pre->GetFieldValue(_T("stb_name"),stdinfo.m_stdName);

					m_poolManage.CStringToint(i_queNumStart,queNumStart);
					m_poolManage.CStringToint(i_queNumEnd,queNumEnd);
					m_poolManage.CStringToint(i_AmLimitPatient,AmLimitPatient);
					m_poolManage.CStringToint(i_PmLimitPatient,PmLimitPatient);
					m_poolManage.CStringToint(i_DayLimitPatient,DayLimitPatient);

					officeQue.SetQueID(queID);
					officeQue.SetQueName(queName);
					officeQue.SetQueCallName(queCallName);
					officeQue.SetQueCallID(queIDCallName);
					if(serial_id)
						officeQue.SetSerialId(serial_id);
					if(!strStbId.IsEmpty())
						officeQue.SetstdId(strStbId);
//					m_mapto_stdinfo[m_com_stdinfo.AddString(stdinfo.m_stdName)]=stdinfo;
					
					officeQue.SetQuenumStart(i_queNumStart);
					officeQue.SetQuenumEnd(i_queNumEnd);
					officeQue.SetAmLimitPatient(i_AmLimitPatient);
					officeQue.SetPmLimitPatient(i_PmLimitPatient);
					officeQue.SetDayLimitPatient(i_DayLimitPatient);
					officeQue.SetHisFlag(hisFlag);
					
					//读诊室
					CConsult consult;
					CString ConsultName,CallName,ThroughIP;
					int ShowAddress;int ComprehAddress;CString consultStbId;
					int ThroughAddress;CString ConsultID;BOOL consultHisFlag=FALSE;
					consultSql.Format(_T("select * from consult where office_id='%s' order by name"),queID);
					if(pConre->Open(consultSql,CADORecordset::openQuery))
					{
						if(!pConre->IsBOF())
						{
							while(!pConre->IsEOF())
							{
								pConre->GetFieldValue(_T("consult_id"),ConsultID);
								pConre->GetFieldValue(_T("name"),ConsultName);
								pConre->GetFieldValue(_T("call_name"),CallName);
								pConre->GetFieldValue(_T("wnd_screen_id"),ShowAddress);
								pConre->GetFieldValue(_T("compreh_screen_id"),ComprehAddress);
								pConre->GetFieldValue(_T("through_screen_id"),ThroughAddress);
								pConre->GetFieldValue(_T("through_screen_ip"),ThroughIP);
								pConre->GetFieldValue(_T("stb_id"),consultStbId);
								pConre->GetFieldValue(_T("his_flag"),consultHisFlag);
								
								consult.SetConsultName(ConsultName);
								consult.SetConsultID(ConsultID);
								consult.SetCallName(CallName);
								consult.SetShowAddress(ShowAddress);
								consult.SetComprehAddress(ComprehAddress);
								consult.SetTroughAddress(ThroughAddress);
//								consult.SetTroughIp(ThroughIP);
								consult.SetstdId(consultStbId);
								consult.SetHisFlag(consultHisFlag);

								officeQue.m_list_officeConsult.AddTail(consult);
								pConre->MoveNext();
							}
						}
					}
					m_listNuOfQue.AddTail(officeQue);
					pre->MoveNext();
				}
			}
		}
		pConre->Close();
		delete pConre;
		pConre=NULL;
		pre->Close();
		delete pre;
		pre=NULL;
	}
}
/*
BOOL CPropQueSettting::ReadConsultToList()
{
	if(m_pbase->IsOpen())
	{
		CADORecordset* pre=new CADORecordset(m_pbase);
		CString sql=_T("selct * from consult");
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(pre->IsBOF())
			{
				return FALSE;
			}
			while(!pre->IsEOF())
			{
				
			}
		}
	}
}
*/
void CPropQueSettting::OnLbnSelchangeListNuresqueue()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CB_ERR==m_list_nofficeque.GetCurSel())
	{
		return;
	}
	////////////////////////
	m_com_consultRtb.SetCurSel(CB_ERR);
	////////////////////////
	m_list_nofficeque.GetText(m_list_nofficeque.GetCurSel(),m_edit_QueName);
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	CNurseOfficeQueue que;
	STDINFO stbinfo;
	while(pos)
	{
		que=m_listNuOfQue.GetNext(pos);
		
		if(que.GetQueName()==m_edit_QueName)
		{
			m_edit_QueCallName=que.GetQueCallName();
			m_edit_QueCallID=que.GetQueCallID();
			m_edit_QueID=que.GetQueID();
		
			SetQueEditStatus(que.GetHisFlag());
			m_selectOfficeID=que.GetQueID();
			LoadConsultData(que.GetQueID());
			m_poolManage.intToCString(que.GetAmLimitPatient(),m_edit_AmLimit);
			m_poolManage.intToCString(que.GetPmLimitPatient(),m_edit_PmLimit);
			m_poolManage.intToCString(que.GetDayLimitPatient(),m_edit_DayLimit);
			m_poolManage.intToCString(que.GetQuenumStart(),m_edit_QueStartNum);
			m_poolManage.intToCString(que.GetQuenumEnd(),m_edit_QueEndNum);
			//int stb_id=que.GetstdId();
			CString strStbId=que.GetstdId();
			int count=m_com_stdinfo.GetCount();
			BOOL flag=FALSE;
			for(int i=0;i<count;i++)
			{
				stbinfo=m_mapto_stdinfo[i];
				if(stbinfo.m_stdId==strStbId)
				{
					flag=TRUE;
					m_com_stdinfo.SetCurSel(i);
					break;
				}
			}
			if(!flag)
			{
				m_com_stdinfo.SetWindowText(_T(""));
			}
			
			m_listconsult_count=m_list_consult.GetItemCount();
			m_consultItem=m_listconsult_count;
/*
			POSITION posdel=que.m_list_officeConsult.GetHeadPosition();
			while(posdel)
			{
				CConsult consult=que.m_list_officeConsult.GetNext(posdel);
			}
*/
			break;
		}
	}
	m_consult_name=_T("");
	m_consult_callname=_T("");
	m_wnd_scrrenid=_T("");
	m_evaluator_id=_T("");
	m_caller_id=_T("");
	m_comprehwnd_id=_T("");
//	m_ed_thruscreenip=_T("");
//	m_ed_thruscreenid=_T("");
	m_com_throughWnd.SetCurSel(CB_ERR);
	UpdateData(FALSE);
}

void CPropQueSettting::OnEnChangeEditNursequename()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if(LB_ERR==m_list_nofficeque.GetCurSel())
	{
		AfxMessageBox(_T("请选择科室"));
		return;
	}

	CString name=_T("");
	m_list_nofficeque.GetText(m_list_nofficeque.GetCurSel(),name);
	UpdateData(TRUE);
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	CNurseOfficeQueue que;

	m_index=m_list_nofficeque.GetCurSel();
	m_list_nofficeque.DeleteString(m_index);
	m_list_nofficeque.InsertString(m_index,m_edit_QueName);
	m_list_nofficeque.SetCurSel(m_index);

	while(pos!=NULL)
	{
		que=m_listNuOfQue.GetNext(pos);
		if(pos==NULL && que.GetQueName()==name)
		{
			que.SetQueName(m_edit_QueName);
			m_listNuOfQue.RemoveTail();
			m_listNuOfQue.AddTail(que);
		}
		else if(que.GetQueName()==name)
		{
			que.SetQueName(m_edit_QueName);
			m_listNuOfQue.GetPrev(pos);
			m_listNuOfQue.SetAt(pos,que);
			break;
		}

	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CPropQueSettting::OnEnChangeEditNursecallname()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if(LB_ERR==m_list_nofficeque.GetCurSel())
	{
		AfxMessageBox(_T("请选择科室"));
		return;
	}

	m_list_nofficeque.GetText(m_list_nofficeque.GetCurSel(),m_edit_QueName);
	UpdateData();
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	CNurseOfficeQueue que;
	while(pos!=NULL)
	{
		que=m_listNuOfQue.GetNext(pos);
		if(pos==NULL && que.GetQueName()==m_edit_QueName)//最后一个
		{
			que.SetQueCallName(m_edit_QueCallName);
			m_listNuOfQue.RemoveTail();
			m_listNuOfQue.AddTail(que);
		}
		else if(que.GetQueName()==m_edit_QueName)
		{
			que.SetQueCallName(m_edit_QueCallName);
			m_listNuOfQue.GetPrev(pos);
			m_listNuOfQue.SetAt(pos,que);
			break;
		}
	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CPropQueSettting::OnEnChangeEditNursequeid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if(LB_ERR==m_list_nofficeque.GetCurSel())
	{
		AfxMessageBox(_T("请选择科室"));
		return;
	}

	m_list_nofficeque.GetText(m_list_nofficeque.GetCurSel(),m_edit_QueName);
	UpdateData();
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	CNurseOfficeQueue que;
	while(pos!=NULL)
	{
		que=m_listNuOfQue.GetNext(pos);
		if(pos==NULL && que.GetQueName()==m_edit_QueName)//最后一个
		{
			que.SetQueID(m_edit_QueID);
			m_listNuOfQue.RemoveTail();
			m_listNuOfQue.AddTail(que);
		}
		else if(que.GetQueName()==m_edit_QueName)
		{
			que.SetQueID(m_edit_QueID);
			m_listNuOfQue.GetPrev(pos);
			m_listNuOfQue.SetAt(pos,que);
			break;
		}
	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CPropQueSettting::OnEnChangeEditNursecallid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if(LB_ERR==m_list_nofficeque.GetCurSel())
	{
		AfxMessageBox(_T("请选择科室"));
		return;
	}

	m_list_nofficeque.GetText(m_list_nofficeque.GetCurSel(),m_edit_QueName);
	UpdateData();
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	CNurseOfficeQueue que;
	while(pos!=NULL)
	{
		que=m_listNuOfQue.GetNext(pos);
		if(pos==NULL && que.GetQueName()==m_edit_QueName)//最后一个
		{
			que.SetQueCallID(m_edit_QueCallID);
			m_listNuOfQue.RemoveTail();
			m_listNuOfQue.AddTail(que);
		}
		else if(que.GetQueName()==m_edit_QueName)
		{
			que.SetQueCallID(m_edit_QueCallID);
			m_listNuOfQue.GetPrev(pos);
			m_listNuOfQue.SetAt(pos,que);
			break;
		}
	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CPropQueSettting::OnEnChangeEditNursequestartnum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if(LB_ERR==m_list_nofficeque.GetCurSel())
	{
		AfxMessageBox(_T("请选择科室"));
		return;
	}

	m_list_nofficeque.GetText(m_list_nofficeque.GetCurSel(),m_edit_QueName);
	UpdateData();
	int temp=0;
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	CNurseOfficeQueue que;
	while(pos!=NULL)
	{
		que=m_listNuOfQue.GetNext(pos);
		if(pos==NULL && que.GetQueName()==m_edit_QueName)//最后一个
		{
			
			m_poolManage.CStringToint(temp,m_edit_QueStartNum);
			que.SetQuenumStart(temp);
			m_listNuOfQue.RemoveTail();
			m_listNuOfQue.AddTail(que);
		}
		else if(que.GetQueName()==m_edit_QueName)
		{
			m_poolManage.CStringToint(temp,m_edit_QueStartNum);
			que.SetQuenumStart(temp);
			m_listNuOfQue.GetPrev(pos);
			m_listNuOfQue.SetAt(pos,que);
			break;
		}
	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CPropQueSettting::OnEnChangeEditNursequeendnum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if(LB_ERR==m_list_nofficeque.GetCurSel())
	{
		AfxMessageBox(_T("请选择科室"));
		return;
	}

	m_list_nofficeque.GetText(m_list_nofficeque.GetCurSel(),m_edit_QueName);
	UpdateData();
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	CNurseOfficeQueue que;
	int temp=0;
	while(pos!=NULL)
	{
		que=m_listNuOfQue.GetNext(pos);
		if(pos==NULL && que.GetQueName()==m_edit_QueName)//最后一个
		{
			m_poolManage.CStringToint(temp,m_edit_QueEndNum);
			que.SetQuenumEnd(temp);
			m_listNuOfQue.RemoveTail();
			m_listNuOfQue.AddTail(que);
		}
		else if(que.GetQueName()==m_edit_QueName)
		{
			m_poolManage.CStringToint(temp,m_edit_QueEndNum);
			que.SetQuenumEnd(temp);
//			que.SetQueCallName(m_edit_QueEndNum);
			m_listNuOfQue.GetPrev(pos);
			m_listNuOfQue.SetAt(pos,que);
			break;
		}
	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CPropQueSettting::OnEnChangeEditAmlimit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if(LB_ERR==m_list_nofficeque.GetCurSel())
	{
		AfxMessageBox(_T("请选择科室"));
		return;
	}

	m_list_nofficeque.GetText(m_list_nofficeque.GetCurSel(),m_edit_QueName);
	UpdateData();
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	CNurseOfficeQueue que;
	int temp=0;
	while(pos!=NULL)
	{
		que=m_listNuOfQue.GetNext(pos);
		if(pos==NULL && que.GetQueName()==m_edit_QueName)//最后一个
		{
			m_poolManage.CStringToint(temp,m_edit_AmLimit);
			que.SetAmLimitPatient(temp);
			m_listNuOfQue.RemoveTail();
			m_listNuOfQue.AddTail(que);
		}
		else if(que.GetQueName()==m_edit_QueName)
		{
			m_poolManage.CStringToint(temp,m_edit_AmLimit);
			que.SetAmLimitPatient(temp);
			//			que.SetQueCallName(m_edit_QueEndNum);
			m_listNuOfQue.GetPrev(pos);
			m_listNuOfQue.SetAt(pos,que);
			break;
		}
	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CPropQueSettting::OnEnChangeEditPmLimit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if(LB_ERR==m_list_nofficeque.GetCurSel())
	{
		AfxMessageBox(_T("请选择科室"));
		return;
	}

	m_list_nofficeque.GetText(m_list_nofficeque.GetCurSel(),m_edit_QueName);
	UpdateData();
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	CNurseOfficeQueue que;
	int temp=0;
	while(pos!=NULL)
	{
		que=m_listNuOfQue.GetNext(pos);
		if(pos==NULL && que.GetQueName()==m_edit_QueName)//最后一个
		{
			m_poolManage.CStringToint(temp,m_edit_PmLimit);
			que.SetPmLimitPatient(temp);
			m_listNuOfQue.RemoveTail();
			m_listNuOfQue.AddTail(que);
		}
		else if(que.GetQueName()==m_edit_QueName)
		{
			m_poolManage.CStringToint(temp,m_edit_PmLimit);
			que.SetPmLimitPatient(temp);
			//			que.SetQueCallName(m_edit_QueEndNum);
			m_listNuOfQue.GetPrev(pos);
			m_listNuOfQue.SetAt(pos,que);
			break;
		}
	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CPropQueSettting::OnEnChangeEditDaylimit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if(LB_ERR==m_list_nofficeque.GetCurSel())
	{
		AfxMessageBox(_T("请选择用户"));
		return;
	}

	m_list_nofficeque.GetText(m_list_nofficeque.GetCurSel(),m_edit_QueName);
	UpdateData();
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	CNurseOfficeQueue que;
	int temp=0;
	while(pos!=NULL)
	{
		que=m_listNuOfQue.GetNext(pos);
		if(pos==NULL && que.GetQueName()==m_edit_QueName)//最后一个
		{
			m_poolManage.CStringToint(temp,m_edit_DayLimit);
			que.SetDayLimitPatient(temp);
			m_listNuOfQue.RemoveTail();
			m_listNuOfQue.AddTail(que);
		}
		else if(que.GetQueName()==m_edit_QueName)
		{
			m_poolManage.CStringToint(temp,m_edit_DayLimit);
			que.SetDayLimitPatient(temp);
			//			que.SetQueCallName(m_edit_QueEndNum);
			m_listNuOfQue.GetPrev(pos);
			m_listNuOfQue.SetAt(pos,que);
			break;
		}
	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CPropQueSettting::OnEnChangeEdConsultName()
{
	if(m_selectConsultitem < 0) return;
	UpdateData();

	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos)
	{
		POSITION posNuOfQueRe=pos;
		CNurseOfficeQueue officeque=m_listNuOfQue.GetNext(pos);
		if(officeque.GetQueID()==GetSelectOfficeID())
		{
			POSITION posConsult=officeque.m_list_officeConsult.GetHeadPosition();
			while(posConsult)
			{
				POSITION posdel=posConsult;
				CConsult consult=officeque.m_list_officeConsult.GetNext(posConsult);
#ifdef _DEBUG
				MyWriteConsole(consult.GetConsultID());
#endif
				if(m_selectConsultID==consult.GetConsultID())
				{
					consult.SetConsultName(m_consult_name);
					officeque.m_list_officeConsult.SetAt(posdel,consult);
					m_list_consult.SetItemText(m_selectConsultitem,1,m_consult_name);
					break;
				}
			}
			//修改外层链表
			m_listNuOfQue.SetAt(posNuOfQueRe,officeque);
			break;
		}
	}
	SetModified();
/*
	CString m_consult_name;
	CString m_consult_callname;
	CString m_wnd_scrrenid;
	CString m_evaluator_id;
	CString m_caller_id;
	CString m_comprehwnd_id;
	*/
}

void CPropQueSettting::OnEnChangeEdConsultCallname()
{
	if(m_selectConsultitem < 0) return;
	UpdateData();

	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos)
	{
		POSITION posNuOfQueRe=pos;
		CNurseOfficeQueue officeque=m_listNuOfQue.GetNext(pos);
		if(officeque.GetQueID()==GetSelectOfficeID())
		{
			POSITION posConsult=officeque.m_list_officeConsult.GetHeadPosition();
			while(posConsult)
			{
				POSITION posdel=posConsult;
				CConsult consult=officeque.m_list_officeConsult.GetNext(posConsult);
				if(m_selectConsultID==consult.GetConsultID())
				{
					consult.SetCallName(m_consult_callname);
					officeque.m_list_officeConsult.SetAt(posdel,consult);
					m_list_consult.SetItemText(m_selectConsultitem,2,m_consult_callname);
					break;
				}
			}
			//修改外层链表
			m_listNuOfQue.SetAt(posNuOfQueRe,officeque);
			break;
		}
	}
	SetModified();
}

void CPropQueSettting::OnEnChangeEdWndScrrenid()
{
	if(m_selectConsultitem < 0) return;
	UpdateData();

	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos)
	{
		POSITION posNuOfQueRe=pos;
		CNurseOfficeQueue officeque=m_listNuOfQue.GetNext(pos);
		if(officeque.GetQueID()==GetSelectOfficeID())
		{
			POSITION posConsult=officeque.m_list_officeConsult.GetHeadPosition();
			while(posConsult)
			{
				POSITION posdel=posConsult;
				CConsult consult=officeque.m_list_officeConsult.GetNext(posConsult);
				if(m_selectConsultID==consult.GetConsultID())
				{
					int wnd_screen_address=-1;
					m_poolManage.CStringToint(wnd_screen_address,m_wnd_scrrenid);
					consult.SetShowAddress(wnd_screen_address);
					officeque.m_list_officeConsult.SetAt(posdel,consult);
					m_list_consult.SetItemText(m_selectConsultitem,3,m_wnd_scrrenid);
					break;
				}
			}
			//修改外层链表
			m_listNuOfQue.SetAt(posNuOfQueRe,officeque);
			break;
		}
	}
	SetModified();
}

void CPropQueSettting::OnEnChangeEdComprehwndid()
{

	if(m_selectConsultitem < 0) return;
	UpdateData();

	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos)
	{
		POSITION posNuOfQueRe=pos;
		CNurseOfficeQueue officeque=m_listNuOfQue.GetNext(pos);
		if(officeque.GetQueID()==GetSelectOfficeID())
		{
			POSITION posConsult=officeque.m_list_officeConsult.GetHeadPosition();
			while(posConsult)
			{
				POSITION posdel=posConsult;
				CConsult consult=officeque.m_list_officeConsult.GetNext(posConsult);
				if(m_selectConsultID==consult.GetConsultID())
				{
					int compreh_screen_address=-1;
					m_poolManage.CStringToint(compreh_screen_address,m_comprehwnd_id);
					consult.SetComprehAddress(compreh_screen_address);
					officeque.m_list_officeConsult.SetAt(posdel,consult);
					m_list_consult.SetItemText(m_selectConsultitem,4,m_comprehwnd_id);
					break;
				}
			}
			//修改外层链表
			m_listNuOfQue.SetAt(posNuOfQueRe,officeque);
			break;
		}
	}
	SetModified();
}


BOOL CPropQueSettting::OnApply()
{
	UpdateData();

	CString sql=_T("");
	try
	{
		if(m_pbase->IsOpen())
		{

			POSITION delPosOffQue=m_list_delNurseOffQue.GetHeadPosition();
			while(delPosOffQue)
			{
				CNurseOfficeQueue officeQue=m_list_delNurseOffQue.GetNext(delPosOffQue);
				sql.Format(_T("delete from nurse_office where office_id='%s'"),officeQue.GetQueID());
				m_pbase->Execute(sql);
				sql.Format(_T("delete from office where office_id='%s'"),officeQue.GetQueID());
				m_pbase->Execute(sql);
				sql.Format(_T("delete from consult where office_id='%s'"),officeQue.GetQueID());
				m_pbase->Execute(sql);
			}
			///删除诊室
			POSITION posDelconsult = m_list_delConsult.GetHeadPosition();
			while(posDelconsult)
			{
				CConsult consult =  m_list_delConsult.GetNext(posDelconsult);
				sql.Format(_T("delete from consult where consult_id='%s'"),consult.GetConsultID());
				m_pbase->Execute(sql);
			}

			POSITION pos=m_listNuOfQue.GetHeadPosition();
			CADORecordset *pre=new CADORecordset(m_pbase);
			while(pos)
			{
				CNurseOfficeQueue officeQue=m_listNuOfQue.GetNext(pos);
				CString sql=_T("");
				sql.Format(_T("select * from office where serial_id='%d'"),
					officeQue.GetSerialId());
				if(pre->Open(sql,CADORecordset::openQuery))
				{
					if(pre->IsBOF())
					{
						sql.Format(_T("insert into Office(office_id,call_id,name,call_name,NumStart,NumEnd,AmLimit,PmLimit,DayLimit,stb_id) values('%s','%s','%s','%s','%d','%d','%d','%d','%d','%s')"),
							officeQue.GetQueID(),officeQue.GetQueCallID(),officeQue.GetQueName(),
							officeQue.GetQueCallName(),officeQue.GetQuenumStart(),
							officeQue.GetQuenumEnd(),officeQue.GetAmLimitPatient(),
							officeQue.GetPmLimitPatient(),officeQue.GetDayLimitPatient(),officeQue.GetstdId());
						try
						{
							m_pbase->Execute(sql);
						}
						catch(...)
						{
#ifdef _DEBUG
							MyWriteConsole(sql);
#endif
							MessageBox(_T("insert操作失败"),MB_OK,MB_ICONINFORMATION);
							delete pre;
							pre=NULL;
							return FALSE;
						}
					}
					else
					{
						CString office_id;
						pre->GetFieldValue(_T("office_id"),office_id);
						sql.Format(_T("update office set office_id='%s',call_id='%s',name='%s',call_name='%s',numstart='%d',numend='%d',amlimit='%d',pmlimit='%d',daylimit='%d',stb_id='%s' where serial_id='%d'"),
							officeQue.GetQueID(),officeQue.GetQueCallID(),officeQue.GetQueName(),
							officeQue.GetQueCallName(),officeQue.GetQuenumStart(),
							officeQue.GetQuenumEnd(),officeQue.GetAmLimitPatient(),
							officeQue.GetPmLimitPatient(),officeQue.GetDayLimitPatient(),
							officeQue.GetstdId(),officeQue.GetSerialId());
						try
						{
							m_pbase->Execute(sql);
						}
						catch(...)
						{
#ifdef _DEBUG
							MyWriteConsole(sql);
#endif
							MessageBox(_T("update操作失败"),MB_OK,MB_ICONINFORMATION);
							delete pre;
							pre=NULL;
							return FALSE;
						}

						//当office_id修改时相应修改可处理队列的里面的office_id
						sql.Format(_T("update nurse_office set office_id='%s' where office_id='%s'"),
							officeQue.GetQueID(),office_id);
						try
						{
							m_pbase->Execute(sql);	
						}
						catch(...)
						{
							MessageBox(_T("操作失败"),MB_OK,MB_ICONINFORMATION);
							delete pre;
							pre=NULL;
							return FALSE;
						}

						//////////////////////////////////////////////////////////////////////
					}

					pre->Close();
				}



				POSITION posconsult=officeQue.m_list_officeConsult.GetHeadPosition();
				while(posconsult)
				{

					CConsult consult=officeQue.m_list_officeConsult.GetNext(posconsult);
					sql.Format(_T("select * from consult where consult_id='%s'"),consult.GetConsultID());
					if(pre->Open(sql,CADORecordset::openQuery))
					{
						if(pre->IsBOF())
						{
							sql.Format(_T("insert into consult(consult_id,name,call_name,wnd_screen_id,compreh_screen_id,office_id,through_screen_id,stb_id) values('%s','%s','%s','%d','%d','%s','%d','%s')"),
								consult.GetConsultID(),consult.GetConsultName(),consult.GetCallName(),
								consult.GetShowAddress(),consult.GetComprehAddress(),officeQue.GetQueID(),
								consult.GetTroughAddress(),consult.GetstdId());
							try
							{
								m_pbase->Execute(sql);
							}
							catch(...)
							{
								MessageBox(_T("操作失败"),MB_OK,MB_ICONINFORMATION);
								delete pre;
								pre=NULL;
								return FALSE;
							}
						}
						else
						{
							sql.Format(_T("update consult set name='%s',call_name='%s',wnd_screen_id='%d',compreh_screen_id='%d',office_id='%s',through_screen_id='%d',stb_id='%s' where consult_id='%s'"),
								consult.GetConsultName(),consult.GetCallName(),
								consult.GetShowAddress(),consult.GetComprehAddress(),
								officeQue.GetQueID(),consult.GetTroughAddress(),
								consult.GetstdId(),consult.GetConsultID());
							try
							{
								m_pbase->Execute(sql);
							}
							catch(...)
							{
								MessageBox(_T("操作失败"),MB_OK,MB_ICONINFORMATION);
								delete pre;
								pre=NULL;
								return FALSE;
							}
						}
						pre->Close();
					}

				}
			}


			delete pre;
			pre=NULL;
		}
		if(theApp.m_pUserSetting->m_hWnd)
		{
			//office_id修改以后user重新读取内存.
			theApp.m_pUserSetting->WirteDataToList();
			theApp.m_pUserSetting->SearchOffice();//刷新map
			theApp.m_pUserSetting->ReFulshOffice();
			theApp.m_pUserSetting->OnLbnSelchangeListUsername();
		}
		//	if(theApp.m_pPropSetting->m_DoctorManageDlg.m_hWnd)
		//		theApp.m_pPropSetting->m_DoctorManageDlg.OnBnClickedButtonRefresh();
		if(m_hWnd)
		{
			WriteDataToList();
			FlushMapToStbInfo();//读液晶
			FlushMapToThroughWnd();//读通屏
			OnLbnSelchangeListNuresqueue();
		}
	}
	catch(_com_error& e)
	{
		//WriteLog::WriteDbErrLog(_T("CPropQueSettting::OnApply"));
	}
	return CPropertyPage::OnApply();
}



void CPropQueSettting::LoadConsultData(CString SelectOfficeID)
{
	m_list_consult.DeleteAllItems();
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	int item=0;
	while(pos)
	{
		CNurseOfficeQueue nurseoffice=m_listNuOfQue.GetNext(pos);
		if(nurseoffice.GetQueID()==SelectOfficeID)
		{
			POSITION posconsult=nurseoffice.m_list_officeConsult.GetHeadPosition();
			while(posconsult)
			{
				CConsult consult=nurseoffice.m_list_officeConsult.GetNext(posconsult);	
				CString consultID,showAddress,throughaddress,comprehaddress;
				//m_poolManage.intToCString(consult.GetConsultID(),consultID);
				consultID = consult.GetConsultID();
				m_poolManage.intToCString(consult.GetTroughAddress(),throughaddress);
				m_poolManage.intToCString(consult.GetShowAddress(),showAddress);
				m_poolManage.intToCString(consult.GetComprehAddress(),comprehaddress);
				m_list_consult.InsertItem(item,consultID);
				m_list_consult.SetItemText(item,1,consult.GetConsultName());
				m_list_consult.SetItemText(item,2,consult.GetCallName());
				m_list_consult.SetItemText(item,3,showAddress);
				m_list_consult.SetItemText(item,4,comprehaddress);
//				m_list_consult.SetItemText(item,5,consult.GetThroughIP());
//				m_list_consult.SetItemText(item,6,throughaddress);
//				m_list_consult.SetItemText(item,7,consult.GetstdId());
				int count = m_mapto_consultStdInfo.GetCount();
				for(int i=0;i<count;i++)
				{
					if(m_mapto_consultStdInfo[i].m_stdId == consult.GetstdId())
					{
						m_list_consult.SetItemText(item,6,m_mapto_consultStdInfo[i].
							m_stdName);
						break;
					}
				}
				count = m_mapto_throughWnd.size();
				for(int i=0;i<count;i++)
				{
					if(m_mapto_throughWnd[i].serialID == consult.GetTroughAddress())
					{
						m_list_consult.SetItemText(item,5,m_mapto_throughWnd[i].throughName);
						break;
					}
				}
				item++;
			}
			break;
		}
	}
}
void CPropQueSettting::OnLvnItemchangedOfficeConsult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_selectConsultitem=pNMLV->iItem;
	m_consult_name=m_list_consult.GetItemText(m_selectConsultitem,1);
	m_consult_callname=m_list_consult.GetItemText(m_selectConsultitem,2);
	m_wnd_scrrenid=m_list_consult.GetItemText(m_selectConsultitem,3);
	m_comprehwnd_id=m_list_consult.GetItemText(m_selectConsultitem,4);
//	m_ed_thruscreenip=m_list_consult.GetItemText(m_selectConsultitem,5);
//	m_ed_thruscreenid=m_list_consult.GetItemText(m_selectConsultitem,6);
//	CString stb_name=m_list_consult.GetItemText(m_selectConsultitem,7);
	
	m_selectConsultID=m_list_consult.GetItemText(m_selectConsultitem,0);
//	m_poolManage.CStringToint(m_selectConsultID,consultID);
	
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	CNurseOfficeQueue que;
	CConsult consult;
	while(pos)
	{
		que=m_listNuOfQue.GetNext(pos);
		if(que.GetQueName()==m_edit_QueName)
		{
			POSITION posconsult=que.m_list_officeConsult.GetHeadPosition();
			while(posconsult)
			{
				consult = que.m_list_officeConsult.GetNext(posconsult);
				if(m_selectConsultID == consult.GetConsultID())
				{
					SetConsultEditStatus(consult.GetHisFlag());
					break;
				}
			}
			break;
		}
	}
	BOOL flag = FALSE; 
	int count = m_com_consultRtb.GetCount();
	for(int i=0;i<count;i++)
	{
		CString strStdId = m_mapto_consultStdInfo[i].m_stdId;
		CString strCompStdId = consult.GetstdId();
		if(strStdId == strCompStdId)
		{
			m_com_consultRtb.SetCurSel(i);
			flag = TRUE;
			break;
		}
	}
	if(!flag)
	{
		m_com_consultRtb.SetCurSel(-1);
	}
	///////////////////////////通屏
	flag = FALSE;
	count = m_com_throughWnd.GetCount();
	for(int i=0;i<count;i++)
	{
		int serialID = m_mapto_throughWnd[i].serialID;
		if(serialID == consult.GetTroughAddress())
		{
			m_com_throughWnd.SetCurSel(i);
			flag = TRUE;
			break;
		}
	}
	if(!flag)
		m_com_throughWnd.SetCurSel(CB_ERR);
	UpdateData(FALSE);
	*pResult = 0;
}

void CPropQueSettting::OnBnClickedBnDelconsult()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_selectConsultitem < 0) return;

	
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos)
	{
		POSITION posNuOfQueRe=pos;
		CNurseOfficeQueue officeque=m_listNuOfQue.GetNext(pos);
		if(officeque.GetQueID()==GetSelectOfficeID())
		{
			POSITION posConsult=officeque.m_list_officeConsult.GetHeadPosition();
			while(posConsult)
			{
				POSITION posdel=posConsult;
				CConsult consult=officeque.m_list_officeConsult.GetNext(posConsult);
				if(m_selectConsultID==consult.GetConsultID())
				{
					officeque.m_list_officeConsult.RemoveAt(posdel);
					m_list_delConsult.AddTail(consult);
					m_consultItem--;
					//m_consultID--;
					break;
				}
			}
			//修改外层链表
			m_listNuOfQue.SetAt(posNuOfQueRe,officeque);
			break;
		}
	}
	m_list_consult.DeleteItem(m_selectConsultitem);
	/*
	if(m_selectConsultitem-1>=0)
	{
		m_list_consult.SetItemState(m_selectConsultitem-1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_list_consult.EnsureVisible(m_selectConsultitem-1, TRUE);
	}
	*/
	SetModified();
}

void CPropQueSettting::OnNMCustomdrawOfficeConsult(NMHDR *pNMHDR, LRESULT *pResult)
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
		POSITION pos = m_list_consult.GetFirstSelectedItemPosition();
		int index = m_list_consult.GetNextSelectedItem(pos);
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

void CPropQueSettting::OnBnClickedBnAddconsult()
{
	// TODO: 在此添加控件通知处理程序代码
	CString consultName;
	m_consultItem++;
	consultName.Format(_T("%d"),m_consultItem);
	consultName+=_T("诊室");

	
	//m_consultID++;
	CConsult addConsult;
	//addConsult.SetConsultID(MyString::Int2Str(m_consultID));
	//CTime time = CTime::GetCurrentTime();
	//addConsult.SetConsultID(time.Format(_T("%Y%m%d%H%M%S")));
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strTime;
	strTime.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), st.wYear, st.wMonth, st.wDay, 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	addConsult.SetConsultID(strTime);
	addConsult.SetCallName(consultName);
	addConsult.SetConsultName(consultName);
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos)
	{
		POSITION lastPos=pos;
		CNurseOfficeQueue myNurseOfficeQue=m_listNuOfQue.GetNext(pos);
		if(myNurseOfficeQue.GetQueID()==GetSelectOfficeID())
		{
			myNurseOfficeQue.m_list_officeConsult.AddTail(addConsult);
			CString c_consultID,wnd_screen_id,compreh_screen_id,through_screen_id;
			m_poolManage.intToCString(addConsult.GetShowAddress(),wnd_screen_id);
			m_poolManage.intToCString(addConsult.GetComprehAddress(),compreh_screen_id);
			m_poolManage.intToCString(addConsult.GetTroughAddress(),through_screen_id);
			//m_poolManage.intToCString(m_consultID,c_consultID);
			//m_list_consult.InsertItem(m_consultItem-1,c_consultID);
			m_list_consult.InsertItem(m_consultItem-1,addConsult.GetConsultID());
			m_list_consult.SetItemText(m_consultItem-1,1,addConsult.GetConsultName());
			m_list_consult.SetItemText(m_consultItem-1,2,addConsult.GetCallName());
			m_list_consult.SetItemText(m_consultItem-1,3,wnd_screen_id);
			m_list_consult.SetItemText(m_consultItem-1,4,compreh_screen_id);
//			m_list_consult.SetItemText(m_consultItem-1,5,through_screen_id);
			m_listNuOfQue.SetAt(lastPos,myNurseOfficeQue);
			break;
		}
	}
//	m_list_consult.SetItemState(m_consultItem-1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
//	m_list_consult.EnsureVisible(m_consultItem-1, TRUE);
	SetModified();
}

void CPropQueSettting::ReadOfficeInfoFromHis()
{
	try
	{
		if(m_phisDataBase->IsOpen() && m_pbase->IsOpen())
		{
			CString office_id,office_name,office_callname,have_office_id;
			CString numstart=_T("-1");
			CString numend=_T("-1");
			CString amlimit=_T("-1");
			CString pmlimit=_T("-1");
			CString daylimit=_T("-1");

			CString insertSql=_T("");
			CADORecordset* pHisRecord=new CADORecordset(m_phisDataBase);
			CString hissql=_T("select * from HisForBjlb_Office");
			CADORecordset* pMyRecord=new CADORecordset(m_pbase);
			CString mysql=_T("select office_id from office");
			pHisRecord->Open(hissql,CADORecordset::openQuery);
			pMyRecord->Open(mysql,CADORecordset::openQuery);
			if(pMyRecord->IsBOF() && !pHisRecord->IsBOF())
			{
				while(!pHisRecord->IsEOF())
				{
					pHisRecord->GetFieldValue(_T("office_id"),office_id);
					pHisRecord->GetFieldValue(_T("office_name"),office_name);
					pHisRecord->GetFieldValue(_T("office_callname"),office_callname);
					insertSql.Format(_T("insert into office(office_id,name,call_name,numstart,numend,amlimit,pmlimt,daylimit) values('%s','%s','%s','%s','%s','%s','%s','%s')"),
						office_id,office_name,office_callname,numstart,numend,amlimit,pmlimit,daylimit);
					m_pbase->Execute(insertSql);
					pHisRecord->MoveNext();
				}
			}
			else if(!pHisRecord->IsBOF() && !pMyRecord->IsBOF())
			{
				while(!pHisRecord->IsEOF())
				{
					BOOL flag=FALSE;
					pHisRecord->GetFieldValue(_T("office_id"),office_id);
					pHisRecord->GetFieldValue(_T("office_name"),office_name);
					pHisRecord->GetFieldValue(_T("office_callname"),office_callname);
					pMyRecord->MoveFirst();
					while(!pMyRecord->IsEOF())
					{
						pMyRecord->GetFieldValue(_T("office_id"),have_office_id);
						if(have_office_id==office_id)
						{
							flag=TRUE;
							break;
						}
						pMyRecord->MoveNext();
					}
					if(!flag)
					{
						insertSql.Format(_T("insert into office(office_id,name,call_name,numstart,numend,amlimit,pmlimit,daylimit) values('%s','%s','%s','%s','%s','%s','%s','%s')"),
							office_id,office_name,office_callname,numstart,numend,amlimit,pmlimit,daylimit);
						m_pbase->Execute(insertSql);
					}
					pHisRecord->MoveNext();
				}
			}

			delete pHisRecord;
			delete pMyRecord;
			pHisRecord=NULL;
			pMyRecord=NULL;
		}
	}
	catch(_com_error& e)
	{

	}
}

BOOL CPropQueSettting::ConnectToHis()
{
	CString hisBaseDrive=m_baseConfig.GetHisDataDrive();
	CString hisIP=m_baseConfig.GetHisServerIP();
	CString hisAcount=m_baseConfig.GetHisAcount();
	CString hisPass=m_baseConfig.GetHisPass();
	CString hisBaseName=m_baseConfig.GetHisBaseName();
	//小宋改连接MySql时用
	CString hisPort=m_baseConfig.GetHisPort();
	////////////////////////////////////
	if(hisBaseDrive==_T("SQL Server"))
	{
		m_phisDataBase->SetConnectionString(_T("Driver={"+hisBaseDrive+"};Server="+hisIP+";Database=HisData;Uid="+hisAcount+";Pwd="+hisPass+";"));
	}
	else if(hisBaseDrive==_T("Oracle"))
	{
		CString conStr;
		conStr = _T("Provider=MSDAORA.1;hos=");
		conStr += hisIP + _T(";port=");
		conStr += hisPort + _T(";Data Source=");//;User ID=");
		conStr += hisBaseName + _T(";User ID=");
		conStr += hisAcount + _T(";Password=");
		conStr += hisPass + _T(";PersistSecurityInfo=False;");
		m_phisDataBase->SetConnectionString(conStr);
	}
	if(m_phisDataBase->IsOpen())
	{
		m_phisDataBase->Close();
	}
	try
	{
		if(m_phisDataBase->GetConnectionString().IsEmpty())
		{
			return FALSE;
		}
		if(m_phisDataBase->Open())
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

BOOL CPropQueSettting::ConnectToBase()
{
	m_pbase->SetConnectionString(_T("Driver={"+m_dataDrive+"};Server="+m_baseIP+";Database=Nurse;Uid="+m_baseAccount+";Pwd="+m_basePassWord+";"));
	if(m_pbase->IsOpen())
	{
		m_pbase->Close();
	}
	try
	{
		if(m_pbase->Open())
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
void CPropQueSettting::OnBnClickedBnFlushhis()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_phisDataBase->IsOpen())
	{
		ReadOfficeInfoFromHis();
		WriteDataToList();
		RemoveAllListBoxData();
		AddDataToListBox();
	}
}

void CPropQueSettting::RemoveAllListBoxData()
{
	int count=m_list_nofficeque.GetCount();
	for(count;count>0;count--)
	{
		m_list_nofficeque.DeleteString(count);
	}
}

void CPropQueSettting::AddDataToListBox()
{
	CString name=_T("");
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos!=NULL)
	{
		name=m_listNuOfQue.GetNext(pos).GetQueName();
		m_list_nofficeque.AddString(name);
	}
}
/*
void CPropQueSettting::OnEnChangeEdWndGip()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	if(m_selectConsultitem < 0) return;
	UpdateData();

	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos)
	{
		POSITION posNuOfQueRe=pos;
		CNurseOfficeQueue officeque=m_listNuOfQue.GetNext(pos);
		if(officeque.GetQueID()==GetSelectOfficeID())
		{
			POSITION posConsult=officeque.m_list_officeConsult.GetHeadPosition();
			while(posConsult)
			{
				POSITION posdel=posConsult;
				CConsult consult=officeque.m_list_officeConsult.GetNext(posConsult);
				if(m_selectConsultID==consult.GetConsultID())
				{
					consult.SetTroughIp(m_ed_thruscreenip);
					officeque.m_list_officeConsult.SetAt(posdel,consult);
					m_list_consult.SetItemText(m_selectConsultitem,5,m_ed_thruscreenip);
					break;
				}
			}
			//修改外层链表
			m_listNuOfQue.SetAt(posNuOfQueRe,officeque);
			break;
		}
	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}
*/
/*
void CPropQueSettting::OnEnChangeEdGadd()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	if(m_selectConsultitem < 0) return;
	UpdateData();

	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos)
	{
		POSITION posNuOfQueRe=pos;
		CNurseOfficeQueue officeque=m_listNuOfQue.GetNext(pos);
		if(officeque.GetQueID()==GetSelectOfficeID())
		{
			POSITION posConsult=officeque.m_list_officeConsult.GetHeadPosition();
			while(posConsult)
			{
				POSITION posdel=posConsult;
				CConsult consult=officeque.m_list_officeConsult.GetNext(posConsult);
				if(m_selectConsultID==consult.GetConsultID())
				{
					int throughaddress=-1;
					m_poolManage.CStringToint(throughaddress,m_ed_thruscreenid);
					consult.SetTroughAddress(throughaddress);
					officeque.m_list_officeConsult.SetAt(posdel,consult);
					m_list_consult.SetItemText(m_selectConsultitem,6,m_ed_thruscreenid);
					break;
				}
			}
			//修改外层链表
			m_listNuOfQue.SetAt(posNuOfQueRe,officeque);
			break;
		}
	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}
*/
int CPropQueSettting::GetMaxConsultID()
{
	CString sql=_T("select max(cast(consult_id as int))maxid from consult");
	int maxConsultID = 0;
	if(m_pbase->IsOpen())
	{
		CADORecordset *pre=new CADORecordset(m_pbase);
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			pre->GetFieldValue(_T("maxid"),maxConsultID);
		}
	}

	return maxConsultID;
}
void CPropQueSettting::OnCbnSelchangeComboRtb()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos)
	{
		POSITION poslast=pos;
		CNurseOfficeQueue que=m_listNuOfQue.GetNext(pos);
		if(que.GetQueID()==GetSelectOfficeID())
		{

			int index=m_com_stdinfo.GetCurSel();
			if(index!=CB_ERR)
			{
				STDINFO stbinfo=m_mapto_stdinfo[index];
				que.SetstdId(stbinfo.m_stdId);
				
				m_listNuOfQue.SetAt(poslast,que);
			}
			break;
		}
		
	}
	SetModified();
}


void CPropQueSettting::FlushMapToStbInfo()
{
	//删除所有科室Com stdinfo
	int count=m_com_stdinfo.GetCount();
	for(int i=count;i>=0;i--)
	{
		m_com_stdinfo.DeleteString(i);
	}
	//////删除所有诊室com stdinfo
	count = m_com_consultRtb.GetCount();
	for(int i=count;i>=0;i--)
	{
		m_com_consultRtb.DeleteString(i);
	}
	////////////////////////////内存置空
	m_mapto_stdinfo.RemoveAll();
	m_mapto_consultStdInfo.RemoveAll();
	////////////////////////////先加一个空的机顶盒标识不选任何机顶盒
	STDINFO stbinfo;
	stbinfo.m_stdId.Empty();
	stbinfo.m_stdName=_T("");
	m_mapto_stdinfo[m_com_stdinfo.AddString(stbinfo.m_stdName)]=stbinfo;
	m_mapto_consultStdInfo[m_com_consultRtb.AddString(stbinfo.m_stdName)]
	= stbinfo;
	///////////////////////////查询科室机顶盒
	CString sql=_T("select * from settopbox where stb_type=1");
	if(m_pbase->IsOpen())
	{
		CADORecordset *pre=new CADORecordset(m_pbase);
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
			{
				//stbinfo.m_stdId=-1;
				while(!pre->IsEOF())
				{
					pre->GetFieldValue(_T("stb_id"),stbinfo.m_stdId);
					pre->GetFieldValue(_T("stb_name"),stbinfo.m_stdName);

					m_mapto_stdinfo[m_com_stdinfo.AddString(stbinfo.m_stdName)]=stbinfo;
					pre->MoveNext();
				}
			}
		}
		pre->Close();
		/////////////////查询诊室机顶盒
		sql = _T("select * from settopbox where stb_type=0");
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
			{
				while(!pre->IsEOF())
				{
					pre->GetFieldValue(_T("stb_id"),stbinfo.m_stdId);
					pre->GetFieldValue(_T("stb_name"),stbinfo.m_stdName);
					m_mapto_consultStdInfo[m_com_consultRtb.AddString(stbinfo.m_stdName)]
					= stbinfo;
					pre->MoveNext();
				}
			}
		}
		pre->Close();
		delete pre;
		pre=NULL;
	}
}
void CPropQueSettting::OnCbnSelchangeComConrtb()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos)
	{
		POSITION poslast=pos;
		CNurseOfficeQueue que=m_listNuOfQue.GetNext(pos);
		if(que.GetQueID()==GetSelectOfficeID())
		{
			int index=m_com_consultRtb.GetCurSel();
			if(index!=CB_ERR)
			{
				STDINFO stbinfo=m_mapto_consultStdInfo[index];
				POSITION posConsult=que.m_list_officeConsult.GetHeadPosition();
				while(posConsult)
				{
					POSITION posdel=posConsult;
					CConsult consult=que.m_list_officeConsult.GetNext(posConsult);
					if(m_selectConsultID==consult.GetConsultID())
					{
						consult.SetstdId(stbinfo.m_stdId);
						que.m_list_officeConsult.SetAt(posdel,consult);
						m_list_consult.SetItemText(m_selectConsultitem,6,
							stbinfo.m_stdName);
						break;
					}
				}
				m_listNuOfQue.SetAt(poslast,que);
			}
			break;
		}
	}
	SetModified();
}


void CPropQueSettting::SetQueEditStatus(const BOOL hisflag)
{
	if(hisflag)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NURSEQUENAME);
		//pEdit->SetReadOnly();
		pEdit->EnableWindow(FALSE);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NURSEQUEID);
		//pEdit->SetReadOnly();
		pEdit->EnableWindow(FALSE);
	}
	else
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NURSEQUENAME);
		//pEdit->SetReadOnly(FALSE);
		pEdit->EnableWindow(TRUE);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NURSEQUEID);
		//pEdit->SetReadOnly(FALSE);
		pEdit->EnableWindow(TRUE);
	}
}

void CPropQueSettting::SetConsultEditStatus(const BOOL hisFlag)
{
	if(hisFlag)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ED_CONSULT_NAME);
		//pEdit->SetReadOnly(TRUE);
		pEdit->EnableWindow(FALSE);
	}
	else
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ED_CONSULT_NAME);
		//pEdit->SetReadOnly(FALSE);
		pEdit->EnableWindow(TRUE);
	}
}

void CPropQueSettting::FlushMapToThroughWnd()
{
	int count=m_com_throughWnd.GetCount();
	for(int i=count;i>=0;i--)
	{
		m_com_throughWnd.DeleteString(i);
	}
	m_mapto_throughWnd.clear();
	//////////////////////////
	ThroughWndInfo throughWndInfo;
	//先加一个空
	//////////////////////////
	m_mapto_throughWnd[m_com_throughWnd.AddString(throughWndInfo.throughName)]
		= throughWndInfo;
	//////////////////////////
	CString sql=_T("select * from ThroughLED");
	if(m_pbase->IsOpen())
	{
		CADORecordset *pre=new CADORecordset(m_pbase);
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
			{
				while(!pre->IsEOF())
				{
					pre->GetFieldValue(_T("serial_id"),throughWndInfo.serialID);
					pre->GetFieldValue(_T("name"),throughWndInfo.throughName);

					m_mapto_throughWnd[m_com_throughWnd.AddString(throughWndInfo.throughName)]=throughWndInfo;
					pre->MoveNext();
				}
			}
		}
		pre->Close();
		delete pre;
		pre = NULL;
	}
}
void CPropQueSettting::OnCbnSelchangeComThroughid()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos=m_listNuOfQue.GetHeadPosition();
	while(pos)
	{
		POSITION poslast=pos;
		CNurseOfficeQueue que=m_listNuOfQue.GetNext(pos);
		if(que.GetQueID()==GetSelectOfficeID())
		{
			int index=m_com_throughWnd.GetCurSel();
			if(index!=CB_ERR)
			{
				ThroughWndInfo throughWndInfo=m_mapto_throughWnd[index];
				POSITION posConsult=que.m_list_officeConsult.GetHeadPosition();
				while(posConsult)
				{
					POSITION posdel=posConsult;
					CConsult consult=que.m_list_officeConsult.GetNext(posConsult);
					if(m_selectConsultID==consult.GetConsultID())
					{
						consult.SetTroughAddress(throughWndInfo.serialID);
						que.m_list_officeConsult.SetAt(posdel,consult);
//						m_list_consult.SetItemText(m_selectConsultitem,7,
//							stbinfo.m_stdName);
						m_list_consult.SetItemText(m_selectConsultitem,5,
							throughWndInfo.throughName);
						break;
					}
				}
				m_listNuOfQue.SetAt(poslast,que);
			}
			break;
		}
	}
	SetModified();
}
