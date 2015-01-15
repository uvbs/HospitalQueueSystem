// ChangeQueDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "ChangeQueDialog.h"
#include "WriteLog.h"

// CChangeQueDialog 对话框

IMPLEMENT_DYNAMIC(CChangeQueDialog, CDialog)

CChangeQueDialog::CChangeQueDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeQueDialog::IDD, pParent)
{
	m_pbase=new CADODatabase;
	m_hIcon=AfxGetApp()->LoadIcon(IDI_ICON_EXCHANGE);
}

CChangeQueDialog::~CChangeQueDialog()
{
	if(m_pbase)
	{
		delete m_pbase;
		m_pbase=NULL;
	}
}

void CChangeQueDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_EDIT_PATNAME, m_rd_patname);
//	DDX_Control(pDX, IDC_EDIT_LASTQUE, m_rd_lastque);
	DDX_Control(pDX, IDC_COMBO_TOQUE, m_com_toque);
//	DDX_Text(pDX, IDC_EDIT_PATNAME, m_cs_patname);
//	DDX_Text(pDX, IDC_EDIT_LASTQUE, m_cs_lastque);
//	DDX_Control(pDX, IDC_COMBO_SETDOCTOR, m_ch_setdoctor);
}


BEGIN_MESSAGE_MAP(CChangeQueDialog, CDialog)
	ON_MESSAGE(WM_MYPAITNMSG,OnReadMsg)
	ON_BN_CLICKED(IDOK, &CChangeQueDialog::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_TOQUE, &CChangeQueDialog::OnCbnSelchangeComboToque)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, &CChangeQueDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CChangeQueDialog 消息处理程序

BOOL CChangeQueDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);

	m_baseDrive=m_BaseConfig.GetDataDrive();
	m_baseIP=m_BaseConfig.GetServerIP();
	m_basePassWord=m_BaseConfig.GetServerPassword();
	m_baseAccount=m_BaseConfig.GetServerAcount();

	m_pbase->SetConnectionString(_T("Driver={"+m_baseDrive+"};Server="+m_baseIP+";Database=Nurse;Uid="+m_baseAccount+";Pwd="+m_basePassWord+";"));
	
	//ConnectToBase();
	SearchOffice();
 	if(m_com_toque.GetCount()>0)
 		m_com_toque.SetCurSel(0);
// 	if(m_ch_setdoctor.GetCount()>0)
// 		m_ch_setdoctor.SetCurSel(0);
	
	OnCbnSelchangeComboToque();
//	SearchWaitRoom();
	(((CNurseStationApp*)AfxGetApp())->m_pNurseWnd)->GetPatientView()->m_bCanRefrush = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

afx_msg HRESULT CChangeQueDialog::OnReadMsg(WPARAM wParam,LPARAM lParam)
{
	CPoolManage::CMyInformation* information=(CPoolManage::CMyInformation*)wParam;
//	m_cs_lastque=information->GetOfficeName();
//	m_cs_patname=information->GetPaitentName();
	m_paitentlogid=information->GetLogid();

	UpdateData(FALSE);
	return 0;
}
/*
BOOL CChangeQueDialog::ConnectToBase()
{
	m_pbase->SetConnectionString(_T("Driver={"+m_baseDrive+"};Server="+m_baseIP+";Database=Nurse;Uid="+m_baseAccount+";Pwd="+m_basePassWord+";"));
	m_pbase->SetConnectionTimeout();
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
*/
void CChangeQueDialog::SearchOffice()
{
	CMainFrame* pMainFrame=(CMainFrame*)this->GetParent();
	CString sql=_T("");
	CString Nurse_ID=theApp.GetLoginID();
	sql.Format(_T("select o.name,o.office_id from Office o where office_id in(select office_id from Nurse_Office where nurse_id='%s') and o.office_id<>'%s' order by o.name"),
		Nurse_ID, pMainFrame->GetOfficeQueView()->GetOfficeID());
//	sql.Format(_T("select name,office_id from Office"));
	CString officename=_T("");
	CString officeid=_T("");
	CString str=_T("");
	try
	{
		if(m_pbase->Open())
		{
			CADORecordset* pre=new CADORecordset(m_pbase);
			if(pre->Open(sql,CADORecordset::openQuery))
			{
				if(!pre->IsBOF())
				{
					while(!pre->IsEOF())
					{
						pre->GetFieldValue(_T("name"),officename);
						pre->GetFieldValue(_T("office_id"),officeid);
						str=officename+ '('+ officeid + ')';
						m_com_toque.AddString(str);
						pre->MoveNext();
					}
				}
				pre->Close();
			}
			delete pre;
			pre=NULL;
			m_pbase->Close();
		}
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CChangeQueDialog::SearchOffice"));
	}
	catch (...)
	{
	}
}
void CChangeQueDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame=(CMainFrame*)this->GetParent();
	POSITION pos=pMainFrame->GetPatientView()->m_view_paient.GetFirstSelectedItemPosition();
	if(!pos)
	{
		return;
	}
	else
	{
		while(pos)
		{
			int nItem=pMainFrame->GetPatientView()->m_view_paient.GetNextSelectedItem(pos);
			CString log_id=pMainFrame->GetPatientView()->GetSelectLogidFromItem(nItem);
//			CString status=pView->GetSelectStatusFromItem(nItem);

			CString sql=_T("");
			CString queue_call_id=_T("");
			CString call_id=GetCallID();
			int maxqueid=GetMaxQuenumber() + 1;
			queue_call_id.Format(_T("%03d"),maxqueid);
			call_id+=queue_call_id;

// 			CString waitroom_id=_T("");
// 			int index=m_ch_setdoctor.GetCurSel();
// 			if(index==CB_ERR)
// 			{
// 				MessageBox(_T("请选择等候室"),MB_OK,MB_ICONINFORMATION);
// 				return;
// 			}
// 			else
// 			{
// 				WAITROOMINFO waitinfo;
// 				waitinfo=m_map_waitinfo[index];
// 				waitroom_id=waitinfo.waitid;
// 			}
			sql.Format(_T("update queue set office_id='%s',queue_id='%d',queue_id_call='%s',status='%d',doctor_id=NULL,regtime=getdate() where log_id='%s'"),
					m_selectofficeid,maxqueid,call_id,qsInLine,log_id);
			if(m_pbase->Open())
			{
				m_pbase->Execute(sql);
				m_pbase->Close();
			}
			
			//pMainFrame->OnFrFlush();		
		}
	}
	(((CNurseStationApp*)AfxGetApp())->m_pNurseWnd)->GetPatientView()->m_bCanRefrush = TRUE;
	(((CNurseStationApp*)AfxGetApp())->m_pNurseWnd)->OnFrFlush();
	OnOK();
	OnDestroy();
}


int CChangeQueDialog::GetMaxQuenumber()
{
	CString sql=_T("");
	int nCount = 0;
	//sql.Format(_T("select max(q.queue_id)queue_id,count(q.status)number,max(o.NumStart)NumStart from queue q,Office o where q.office_id=o.office_id and q.office_id='%s'"),
	//	m_selectofficeid);
	sql.Format(_T("select count(log_id) from queue where office_id='%s' and regtime>cast(CONVERT(varchar(100),GETDATE(),23) as datetime);"),
		m_selectofficeid);
	try
	{
		m_pbase->Open();
		CADORecordset* pre=new CADORecordset(m_pbase);
		pre->Open(sql,CADORecordset::openQuery);
		if(!pre->IsBOF())
		{
			pre->GetFieldValue(0,nCount);
		}
		pre->Close();
		delete pre;
		pre=NULL;
		m_pbase->Close();
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CChangeQueDialog::GetMaxQuenumber"));
	}
	return nCount;
}
CString CChangeQueDialog::GetCallID()
{
	CString sql=_T("");
	CString call_id=_T("");
	sql.Format(_T("select max(q.queue_id)queue_id,count(q.status)number,max(o.call_id)call_id from queue q,Office o where q.office_id=o.office_id and q.office_id='%s'"),
		m_selectofficeid);
	try
	{
		if(m_pbase->Open())
		{
			CADORecordset* pre=new CADORecordset(m_pbase);
			if(pre->Open(sql,CADORecordset::openQuery))
			{
				if(!pre->IsBOF())
				{
					pre->GetFieldValue(_T("call_id"),call_id);
				}
				pre->Close();
			}
			delete pre;
			pre=NULL;
			m_pbase->Close();
		}
	}
	catch (_com_error& e)
	{
		WriteLog::WriteDbErrLog(_T("CChangeQueDialog::GetCallID"));
	}
	catch (...)
	{
	}

	return call_id;
}
/*
void CChangeQueDialog::SearchDoctor()
{
	//删除combox里string
	int count=m_ch_setdoctor.GetCount();
	for(int index=count;index>=0;index--)
	{
		m_ch_setdoctor.DeleteString(index);
	}
	CMainFrame* pMainFrame=((CNurseStationApp*)AfxGetApp())->m_pNurseWnd;
	int nKey;
	USERSTATUS usbuf = {0};

	CString sql=_T("");
	sql.Format(_T("select d.name,d.doctor_id from doctor d,office o where d.office_id=o.office_id and o.office_id='%s'"),
		m_selectofficeid);
	CString docname=_T("");
	CString docid=_T("");
	CString str=_T("");

	try
	{
		if(m_pbase->IsOpen())
		{
			CADORecordset* pre=new CADORecordset(m_pbase);
			if(pre->Open(sql,CADORecordset::openQuery))
			{
				if(!pre->IsBOF())
				{
					while(!pre->IsEOF())
					{
						pre->GetFieldValue(_T("doctor_id"),docid);
						POSITION pos = pMainFrame->m_pChatDlg->m_mapUserList.GetStartPosition();
						for(;pos;)
						{
							pMainFrame->m_pChatDlg->m_mapUserList.GetNextAssoc(pos,nKey,usbuf);
							if(docid==usbuf.wszUserID)
							{
								docname=usbuf.wszUserName;
								str=docname+'('+docid+')';
								m_ch_setdoctor.AddString(str);
								break;
							}
						}
						pre->MoveNext();
					}

				}
			}

			delete pre;
			pre=NULL;
		}
	}
	catch (_com_error& e)
	{

	}
	catch (...)
	{
	}

}
*/

void CChangeQueDialog::OnCbnSelchangeComboToque()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_com_toque.GetCurSel();
	if(index!=CB_ERR)
	{
		CString selectoffice=_T("");
		m_com_toque.GetLBText(index,selectoffice);
		m_selectofficeid=m_pool.RequestBracket(selectoffice);
	}
//	SearchDoctor();
//	SearchWaitRoom();
}

// void CChangeQueDialog::SearchWaitRoom()
// {
// 	int count=m_ch_setdoctor.GetCount();
// 	for(int index=count;index>=0;index--)
// 	{
// 		m_ch_setdoctor.DeleteString(index);
// 	}
// 	CString sql=_T("select * from waitingroom");
// 	if(m_pbase->IsOpen())
// 	{
// 		CADORecordset* pre=new CADORecordset(m_pbase);
// 		if(pre->Open(sql,CADORecordset::openQuery))
// 		{
// 			WAITROOMINFO waitinfo;
// 			if(!pre->IsBOF())
// 			{
// 				while(!pre->IsEOF())
// 				{
// 					pre->GetFieldValue(_T("room_name"),waitinfo.waitName);
// 					pre->GetFieldValue(_T("room_id"),waitinfo.waitid);
// 					m_map_waitinfo[m_ch_setdoctor.AddString(waitinfo.waitName)]=waitinfo;
// 					pre->MoveNext();
// 				}
// 			}
// 			pre->Close();
// 		}
// 
// 		delete pre;
// 		pre=NULL;
// 	}
// }
void CChangeQueDialog::OnDestroy()
{
	CDialog::OnDestroy();
	CMainFrame* pMainFrame=(CMainFrame*)this->GetParent();
	if(pMainFrame->GetPatientView()->m_pChangQueDlg)
	{
		delete pMainFrame->GetPatientView()->m_pChangQueDlg;
		pMainFrame->GetPatientView()->m_pChangQueDlg = NULL;
	}
	(((CNurseStationApp*)AfxGetApp())->m_pNurseWnd)->GetPatientView()->m_bCanRefrush = TRUE;
}

void CChangeQueDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
	OnDestroy();
}
