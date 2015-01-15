// AddCommentDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "AddCommentDlg.h"


// CAddCommentDlg 对话框

IMPLEMENT_DYNAMIC(CAddCommentDlg, CDialog)

CAddCommentDlg::CAddCommentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddCommentDlg::IDD, pParent)
	//, m_nRdSelectComment(0)
	//, m_strSelfComment(_T(""))
{

}

CAddCommentDlg::~CAddCommentDlg()
{
}

void CAddCommentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Radio(pDX, IDC_RADIO_COMMENT_SELECT, m_nRdSelectComment);
	DDX_Control(pDX, IDC_COMBO_COMMENT, m_cbComment);
	//DDX_Text(pDX, IDC_EDIT_COMMENT, m_strSelfComment);
}


BEGIN_MESSAGE_MAP(CAddCommentDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAddCommentDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddCommentDlg 消息处理程序

BOOL CAddCommentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cbComment.AddString(_T(""));
	m_cbComment.AddString(_T("急诊"));
	m_cbComment.AddString(_T("复诊"));
	m_cbComment.AddString(_T("预约"));
	m_cbComment.AddString(_T("网约"));
	m_cbComment.AddString(_T("住院"));
	//m_cbComment.AddString(_T("过号召回"));
	m_cbComment.SetCurSel(0);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAddCommentDlg::OnBnClickedOk()
{
	UpdateData();
	//m_cbComment.GetLBText(
	//	m_cbComment.GetCurSel(), m_strComment);
	m_cbComment.GetWindowText(m_strComment);
	OnOK();
}
