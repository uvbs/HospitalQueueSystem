// PropPageDoctorInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DoctorCaller.h"
#include "PropPageDoctorInfo.h"


// CPropPageDoctorInfo 对话框

IMPLEMENT_DYNAMIC(CPropPageDoctorInfo, CPropertyPage)

CPropPageDoctorInfo::CPropPageDoctorInfo(CString strDoctorId, CString strDbConn)
	: CPropertyPage(CPropPageDoctorInfo::IDD)
	, m_strId(strDoctorId)
	, m_strDbConn(strDbConn)
	, m_strName(_T(""))
	, m_strDept(_T(""))
	, m_strLoginId(_T(""))
	, m_strTitle(_T(""))
{

}

CPropPageDoctorInfo::~CPropPageDoctorInfo()
{
}

void CPropPageDoctorInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_strId);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_DEPART, m_strDept);
	DDX_Text(pDX, IDC_EDIT_LOGIN_ID, m_strLoginId);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strTitle);
	DDX_Control(pDX, IDC_BUTTON_PASSWORD, m_btnPassword);
}


BEGIN_MESSAGE_MAP(CPropPageDoctorInfo, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT_LOGIN_ID, &CPropPageDoctorInfo::OnEnChangeEditLoginId)
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD, &CPropPageDoctorInfo::OnBnClickedButtonPassword)
END_MESSAGE_MAP()


// CPropPageDoctorInfo 消息处理程序

BOOL CPropPageDoctorInfo::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	try
	{
		CADODatabase db;
		db.SetConnectionString(m_strDbConn);
		db.Open();
		CString strSelect;
		strSelect.Format(_T("SELECT D.name doctor_name,D.gender,D.title,D.login_id,O.name office_name FROM Doctor D LEFT JOIN Office O on D.office_id=O.office_id WHERE doctor_id='%s';"),
			m_strId);
		CADORecordset rset(&db);
		rset.Open(strSelect, CADORecordset::openQuery);
		if(!rset.IsBOF())
		{
			rset.GetFieldValue(_T("doctor_name"), m_strName);
			rset.GetFieldValue(_T("title"), m_strTitle);
			rset.GetFieldValue(_T("login_id"), m_strLoginId);
			rset.GetFieldValue(_T("office_name"), m_strDept);
		}
		rset.Close();
		db.Close();
	}
	catch (_com_error& e)
	{
	}

	UpdateData(FALSE);

	//m_btnPassword.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropPageDoctorInfo::OnEnChangeEditLoginId()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	SetModified();
}

BOOL CPropPageDoctorInfo::OnApply()
{
	UpdateData(TRUE);

	if(m_strLoginId.IsEmpty())
	{
		MessageBox(_T("登录名不能为空"), _T("提示"));
		return FALSE;
	}

	try
	{
		CADODatabase db;
		db.SetConnectionString(m_strDbConn);
		db.Open();
		CString strSelect;
		strSelect.Format(_T("SELECT login_id FROM Doctor WHERE login_id='%s' AND doctor_id<>'%s';"), 
			m_strLoginId, m_strId);
		CADORecordset rset(&db);
		rset.Open(strSelect, CADORecordset::openQuery);
		if(rset.GetRecordCount() > 0)
		{
			MessageBox(_T("该登录名已被使用!"), _T("提示"));
			return FALSE;
		}
		rset.Close();
		CString strUpdate;
		strUpdate.Format(_T("UPDATE Doctor SET login_id='%s' WHERE doctor_id='%s';"), 
			m_strLoginId, m_strId);
		db.Execute(strUpdate);
		db.Close();
	}
	catch (_com_error& e)
	{
		MessageBox(_T("数据库错误!"), _T("错误"));
		return FALSE;
	}

	return CPropertyPage::OnApply();
}



void CPropPageDoctorInfo::OnBnClickedButtonPassword()
{
	//CDlgDoctorPassword dlg(m_strId, m_strDbConn, this);
	//dlg.DoModal();
}
