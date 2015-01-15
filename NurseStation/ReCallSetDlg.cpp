// ReCallSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "ReCallSetDlg.h"


// CReCallSetDlg 对话框

IMPLEMENT_DYNAMIC(CReCallSetDlg, CDialog)

CReCallSetDlg::CReCallSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReCallSetDlg::IDD, pParent)
{

}

CReCallSetDlg::~CReCallSetDlg()
{
}

void CReCallSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_LASTPATNAME, m_ed_paitname);
	DDX_Control(pDX, IDC_ED_LASTOFFICE, m_ed_lastque);
	DDX_Control(pDX, IDC_COMBO_RESETQUEUE, m_com_setque);
}


BEGIN_MESSAGE_MAP(CReCallSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CReCallSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CReCallSetDlg 消息处理程序

BOOL CReCallSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetName_Que();
	m_ed_lastque.SetReadOnly(TRUE);
	m_ed_paitname.SetReadOnly(TRUE);
	SearchOffice();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CReCallSetDlg::SetName_Que()
{
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CString officename=pMainFrame->GetOfficeQueView()->GetSelectOfficename();
	CString paitname=pMainFrame->GetPatientView()->GetSelectPaitname();
	m_ed_paitname.SetWindowText(paitname);
	m_ed_lastque.SetWindowText(officename);
}
void CReCallSetDlg::SearchOffice()
{
	CString officename=_T("");
	CString officeid=_T("");
	CString str=_T("");
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CADORecordset* pre=pMainFrame->GetOfficeQueView()->SearchNurseContrlOffice();
	while(!pre->IsEOF())
	{
		pre->GetFieldValue(_T("name"),officename);
		pre->GetFieldValue(_T("office_id"),officeid);
		str=officename+ '('+ officeid + ')';
		m_com_setque.AddString(str);
		pre->MoveNext();
	}
	delete pre;
}
void CReCallSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame = ((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CString logid=pMainFrame->GetPatientView()->GetSelectPaitLogid();
	CString sql=_T("");
	int index=m_com_setque.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	CString str=_T("");
	CString call_id=_T("");
	CString queue_call_id=_T("");
	int queue_id;
	int number;
	m_com_setque.GetLBText(index,str);
	CString officeid=m_pool.RequestBracket(str);
	sql.Format(_T("select max(q.queue_id)queue_id,count(q.status)number,max(o.NumStart)NumStart,max(o.call_id)call_id from queue q,Office o where q.office_id=o.office_id and (q.status=0 or q.status=3) and q.office_id='%s' and convert(varchar(10),getdate(),120)=convert(varchar(10),q.regtime,120)"),officeid);
	try
	{
		CADODatabase db;
		db.SetConnectionString(pMainFrame->m_strConn);
		db.Open();
		CADORecordset *pre=new CADORecordset(&db);
		pre->Open(sql,CADORecordset::openQuery);
		pre->GetFieldValue(_T("number"),number);
		if(number==0)
		{
			pre->GetFieldValue(_T("NumStart"),queue_id);
		}
		else
		{
			pre->GetFieldValue(_T("number"),queue_id);
		}
		pre->GetFieldValue(_T("call_id"),call_id);
		delete pre;
		pre=NULL;
		queue_id++;
		if(queue_id<1000&&queue_id>0)
		{
			//C_NumStart.Format(_T("%03d"), NumStart);
			queue_call_id.Format(_T("%03d"),queue_id);
		}
		else if(queue_id<10000&&queue_id>0)
		{
			//C_NumStart.Format(_T("%04d"), NumStart);
			queue_call_id.Format(_T("%04d"),queue_id);
		}
		else
		{
			//C_NumStart.Format(_T("%d"), NumStart);
			queue_call_id.Format(_T("%d"),queue_id);
		}
		call_id+=queue_call_id;
		sql.Format(_T("update queue set office_id='%s',doctor_id=null,queue_id='%d',queue_id_call='%s',status=0 where log_id='%s'"),
			officeid,queue_id,call_id,logid);
		db.Execute(sql);
		db.Close();
	}
	catch(_com_error& e)
	{

	}

	pMainFrame->OnFrFlush();
	OnOK();
}
