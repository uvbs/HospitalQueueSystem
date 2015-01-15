// DlgQueueChange.cpp : 实现文件
//

#include "stdafx.h"
#include "DoctorCaller.h"
#include "DlgQueueChange.h"
#include "../NurseStation/ado2.h"
#include "DoctorCallerDlg.h"


// CDlgQueueChange 对话框

IMPLEMENT_DYNAMIC(CDlgQueueChange, CDialog)

CDlgQueueChange::CDlgQueueChange(CString strConn, CString strDoctorId, CString strNurseId, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQueueChange::IDD, pParent)
	, m_strConn(strConn)
	, m_strDoctorId(strDoctorId)
	, m_strNurseId(strNurseId)
{
	m_strSqlSelQueue.Format(_T("select D.office_id,O.name office_name from Doctor_Office D left join Office O on D.office_id=O.office_id where D.doctor_id='%s' order by office_name"), 
		m_strDoctorId);
	m_strSqlAllQueue.Format(_T("select N.office_id,O.name office_name from Nurse_Office N left join Office O on N.office_id=O.office_id where N.nurse_id='%s' and N.office_id not in (select office_id from Doctor_Office where doctor_id='%s') order by office_name"), 
		m_strNurseId, m_strDoctorId);
}

CDlgQueueChange::~CDlgQueueChange()
{
}

void CDlgQueueChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SELECT_QUEUE, m_lbSelectQueue);
	DDX_Control(pDX, IDC_LIST_ALL_QUEUE, m_lbAllQueue);
}


BEGIN_MESSAGE_MAP(CDlgQueueChange, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgQueueChange::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CDlgQueueChange::OnBnClickedButtonRemove)
END_MESSAGE_MAP()


// CDlgQueueChange 消息处理程序
BOOL CDlgQueueChange::OnInitDialog()
{
	CDialog::OnInitDialog();

	RefreshQueue();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//
void CDlgQueueChange::RefreshQueue()
{
	try
	{
		CADODatabase db;
		db.SetConnectionString(m_strConn);
		db.Open();
		CADORecordset rset(&db);
		rset.Open(m_strSqlSelQueue, CADORecordset::openQuery);
		m_lbSelectQueue.ResetContent();
		m_mapSelQueue.RemoveAll();
		for(; !rset.IsEOF(); rset.MoveNext())
		{
			CString strOfficeId;
			CString strOfficeName;
			rset.GetFieldValue(_T("office_id"), strOfficeId);
			rset.GetFieldValue(_T("office_name"), strOfficeName);
			int nIndex = m_lbSelectQueue.AddString(strOfficeName);
			m_mapSelQueue[nIndex] = strOfficeId;
		}
		rset.Close();

		rset.Open(m_strSqlAllQueue, CADORecordset::openQuery);
		m_lbAllQueue.ResetContent();
		m_mapAllQueue.RemoveAll();
		for(; !rset.IsEOF(); rset.MoveNext())
		{
			CString strOfficeId;
			CString strOfficeName;
			rset.GetFieldValue(_T("office_id"), strOfficeId);
			rset.GetFieldValue(_T("office_name"), strOfficeName);
			int nIndex = m_lbAllQueue.AddString(strOfficeName);
			m_mapAllQueue[nIndex] = strOfficeId;
		}
		rset.Close();

		db.Close();
	}
	catch (_com_error& e)
	{
	}
}

void CDlgQueueChange::OnBnClickedButtonAdd()
{
	int nCount= m_lbAllQueue.GetSelCount();
	if(nCount < 1) return;
	CArray<int, int> arrSel;
	arrSel.SetSize(nCount);
	m_lbAllQueue.GetSelItems(nCount, arrSel.GetData());
	CADODatabase db;
	db.SetConnectionString(m_strConn);
	try
	{
		db.Open();
	}
	catch (_com_error& e)
	{
		return;
	}
	for(int i = 0; i < nCount; i++)
	{
		CString strOfficeId;
		strOfficeId = m_mapAllQueue[arrSel[i]];
		if(strOfficeId.IsEmpty()) continue;
		CString strInsert;
		strInsert.Format(_T("insert into Doctor_Office values('%s','%s')"),
			m_strDoctorId, strOfficeId);
		try
		{
			db.Execute(strInsert);
		}
		catch (_com_error& e)
		{
		}
	}
	db.Close();

	RefreshQueue();
}

void CDlgQueueChange::OnBnClickedButtonRemove()
{
	int nCount = m_lbSelectQueue.GetSelCount();
	if(nCount < 1) return;
	CArray<int, int> arrSel;
	arrSel.SetSize(nCount);
	m_lbSelectQueue.GetSelItems(nCount, arrSel.GetData());
	CADODatabase db;
	db.SetConnectionString(m_strConn);
	try
	{
		db.Open();
	}
	catch (_com_error& e)
	{
		return;
	}
	for(int i = 0; i < nCount; i++)
	{
		CString strOfficeId;
		strOfficeId = m_mapSelQueue[arrSel[i]];
		if(strOfficeId.IsEmpty()) continue;
		CString strInsert;
		strInsert.Format(_T("delete from Doctor_Office where doctor_id='%s' and office_id='%s'"),
			m_strDoctorId, strOfficeId);
		try
		{
			db.Execute(strInsert);
		}
		catch (_com_error& e)
		{
		}
	}
	db.Close();

	RefreshQueue();
}
