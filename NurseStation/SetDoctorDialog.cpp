// SetDoctorDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "SetDoctorDialog.h"
#include "WriteLog.h"


// CSetDoctorDialog 对话框

IMPLEMENT_DYNAMIC(CSetDoctorDialog, CDialog)

CSetDoctorDialog::CSetDoctorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDoctorDialog::IDD, pParent)
{
	pbase=new CADODatabase;
}

CSetDoctorDialog::~CSetDoctorDialog()
{
	if(pbase)
	{
		delete pbase;
		pbase=NULL;
	}
}

void CSetDoctorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_ED_LASTNAME, m_ct_lastname);
//	DDX_Text(pDX, IDC_ED_LASTNAME, m_cs_lastname);
//	DDX_Control(pDX, IDC_ED_LASTDOCNAME, m_ct_lastdoc);
//	DDX_Text(pDX, IDC_ED_LASTDOCNAME, m_cs_lastdoc);
	DDX_Control(pDX, IDC_COMBO_SETDOCTOR, m_com_setdoc);
}


BEGIN_MESSAGE_MAP(CSetDoctorDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CSetDoctorDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetDoctorDialog 消息处理程序

BOOL CSetDoctorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
//	DataSend();
	
	m_baseDrive=m_BaseConfig.GetDataDrive();
	m_baseIP=m_BaseConfig.GetServerIP();
	m_basePassWord=m_BaseConfig.GetServerPassword();
	m_baseAccount=m_BaseConfig.GetServerAcount();
	
	if(ConnectToBase())
	{
		SearchDoctor();
	}

//	m_ct_lastdoc.SetReadOnly(TRUE);
//	m_ct_lastname.SetReadOnly(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CSetDoctorDialog::ConnectToBase()
{
	pbase->SetConnectionString(_T("Driver={"+m_baseDrive+"};Server="+m_baseIP+";Database=Nurse;Uid="+m_baseAccount+";Pwd="+m_basePassWord+";"));
	if(pbase->IsOpen())
	{
		pbase->Close();
	}
	try
	{
		if(pbase->Open())
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

void CSetDoctorDialog::SearchDoctor()
{
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	CString sql=_T("");
	sql.Format(_T("select d.name,d.doctor_id from doctor d,office o where o.office_id=d.office_id and o.office_id='%s'"),
		pMainFrame->GetOfficeQueView()->GetOfficeID());
	CADODatabase db;
	db.SetConnectionString(pMainFrame->m_strConn);
	try
	{
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
					m_map_doctor[m_com_setdoc.AddString(doctocomboinfo.docname)]=doctocomboinfo;
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
		WriteLog::WriteDbErrLog(_T("CSetDoctorDialog::SearchDoctor"));
	}
}


void CSetDoctorDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pView=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	/*
	CString SelectName=pView->GetPatientView()->GetSelectPaitname();
	CString SelectId=pView->GetPatientView()->GetSelectPaitLogid();
	CString Selectofficeid=pView->GetOfficeQueView()->GetOfficeID();

	CString SelectDoc=_T("");
	int index=m_com_setdoc.GetCurSel();
	if(CB_ERR!=index)
	{
		m_com_setdoc.GetLBText(index,SelectDoc);
		CString DocId=m_pool.RequestBracket(SelectDoc);
		CString sql=_T("");
		sql.Format(_T("update queue set doctor_id='%s' where log_id='%s'"),DocId,SelectId);
		if(pbase->IsOpen())
		{
			pbase->Execute(sql);
		}
		CADORecordset* pre=new CADORecordset(pbase);
		sql.Format(_T("select name from doctor where doctor_id='%s'"),DocId);
		pre->Open(sql,CADORecordset::openQuery);
		if(!pre->IsBOF())
		{
			CString docname=_T("");
			pre->GetFieldValue(_T("name"),docname);
			pView->GetPatientView()->ReviseListCtrDocName(SelectId,docname);
		}
		delete pre;
		pre=NULL;
	}
	*/
	int nItem=-1;
	CString docID=GetSelectDoctorId();
	POSITION pos=pView->GetPatientView()->m_view_paient.GetFirstSelectedItemPosition();
	if(!pos)
	{
		return;
	}
	else
	{
		while(pos)
		{
			nItem=pView->GetPatientView()->m_view_paient.GetNextSelectedItem(pos);
			CString log_id=pView->GetPatientView()->GetSelectLogidFromItem(nItem);
			CString status=pView->GetPatientView()->GetSelectStatusFromItem(nItem);

			if((status==statusWaitLine || status==statusSuspend) && !log_id.IsEmpty())
			{
				CString sql=_T("");
				sql.Format(_T("update queue set doctor_id='%s' where log_id='%s'"),
					docID,log_id);
				try
				{
					if(pbase->IsOpen())
					{
						pbase->Execute(sql);

						CADORecordset *pre=new CADORecordset(pbase);
						sql.Format(_T("select D.name,C.call_name ConsultName from doctor D left join Consult C on D.consult_id=C.consult_id where D.doctor_id='%s'"),docID);
						if(pre->Open(sql,CADORecordset::openQuery))
						{
							if(!pre->IsBOF())
							{
								CString docname;
								CString strConsultName;
								pre->GetFieldValue(_T("name"),docname);
								pView->GetPatientView()->ReviseListCtrDocName(log_id,docname);
								pre->GetFieldValue(_T("ConsultName"), strConsultName);
								pView->GetPatientView()->ReviseListCtrConsultName(log_id, strConsultName);
							}
						}
						delete pre;
						pre = NULL;
					}
				}
				catch(_com_error& e)
				{
					WriteLog::WriteDbErrLog(_T("CSetDoctorDialog::OnBnClickedOk"));
				}
			}
		}
	}
	OnOK();
}

CString CSetDoctorDialog::GetSelectDoctorId()
{
	int index=m_com_setdoc.GetCurSel();
	if(index!=CB_ERR)
	{
		DOCTOCOMBOINFO docinfo=m_map_doctor[index];
		return docinfo.docid;
	}
	else
	{
		return _T("");
	}
}