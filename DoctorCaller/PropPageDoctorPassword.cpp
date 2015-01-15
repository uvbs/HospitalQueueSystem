// PropPageDoctorPassword.cpp : 实现文件
//

#include "stdafx.h"
#include "DoctorCaller.h"
#include "PropPageDoctorPassword.h"


// CPropPageDoctorPassword 对话框

IMPLEMENT_DYNAMIC(CPropPageDoctorPassword, CPropertyPage)

CPropPageDoctorPassword::CPropPageDoctorPassword(CString strDoctorId, CString strDbConn)
	: CPropertyPage(CPropPageDoctorPassword::IDD)
	, m_strId(strDoctorId)
	, m_strDbConn(strDbConn)
	, m_strOldPassword(_T(""))
	, m_strNewPassword(_T(""))
	, m_strConfirm(_T(""))
	, m_bChanged(FALSE)
{

}

CPropPageDoctorPassword::~CPropPageDoctorPassword()
{
}

void CPropPageDoctorPassword::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OLD_PASSWORD, m_strOldPassword);
	DDX_Text(pDX, IDC_EDIT_NEW_PASSWORD, m_strNewPassword);
	DDX_Text(pDX, IDC_EDIT_NEW_PASSWORD_COMFIRM, m_strConfirm);
}


BEGIN_MESSAGE_MAP(CPropPageDoctorPassword, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT_NEW_PASSWORD, &CPropPageDoctorPassword::OnEnChangeEditNewPassword)
END_MESSAGE_MAP()


// CPropPageDoctorPassword 消息处理程序

BOOL CPropPageDoctorPassword::OnApply()
{
	if(m_bChanged)
	{
		m_bChanged = FALSE;

		UpdateData(TRUE);

		try
		{
			CADODatabase db;
			db.SetConnectionString(m_strDbConn);
			db.Open();
			CString strSelect;
			strSelect.Format(_T("SELECT password FROM Doctor WHERE doctor_id='%s';"), 
				m_strId);
			CADORecordset rset(&db);
			rset.Open(strSelect, CADORecordset::openQuery);
			if(rset.IsBOF())
			{
				MessageBox(_T("当前帐号已被删除!"), _T("错误"));
				return FALSE;
			}
			CString strPasswd;
			rset.GetFieldValue(0, strPasswd);
			rset.Close();
			if(strPasswd != m_strOldPassword)
			{
				MessageBox(_T("原密码不正确!"), _T("错误"));
				return FALSE;
			}
			if(m_strNewPassword.IsEmpty())
			{
				MessageBox(_T("密码不能为空！"),_T("警告"));
				return FALSE;
			}
			if(m_strConfirm != m_strNewPassword)
			{
				MessageBox(_T("新密码确认不一致!"), _T("错误"));
				return FALSE;
			}
			CString strUpdate;
			strUpdate.Format(_T("UPDATE Doctor SET password='%s' WHERE doctor_id='%s';"), 
				m_strNewPassword, m_strId);
			db.Execute(strUpdate);
			db.Close();
		}
		catch (_com_error& e)
		{
			MessageBox(_T("数据库错误!"), _T("错误"));
			return FALSE;
		}

		m_strOldPassword = m_strNewPassword;
		UpdateData(FALSE);
	}

	return CPropertyPage::OnApply();
}

void CPropPageDoctorPassword::OnEnChangeEditNewPassword()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	SetModified();
	m_bChanged = TRUE;
}
