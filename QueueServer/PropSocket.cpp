// PropSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "QueueServer.h"
#include "PropSocket.h"
#include "../NurseStation/MyPort.h"
#include "../NurseStation/MyCommon.h"
#include "../NurseStation/MyString.h"


// CPropSocket 对话框

IMPLEMENT_DYNAMIC(CPropSocket, CPropertyPage)

CPropSocket::CPropSocket()
	: CPropertyPage(CPropSocket::IDD)
	, m_strLocalIp(_T(""))
	, m_strListenPort(_T(""))
{

}

CPropSocket::~CPropSocket()
{
}

void CPropSocket::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOCAL_IP, m_strLocalIp);
	DDX_Text(pDX, IDC_EDIT_LISTEN_PORT, m_strListenPort);
}


BEGIN_MESSAGE_MAP(CPropSocket, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT_LISTEN_PORT, &CPropSocket::OnEnChangeEditListenPort)
END_MESSAGE_MAP()


// CPropSocket 消息处理程序

BOOL CPropSocket::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//
	MyPort myPort;
	m_strListenPort = MyString::Int2Str(myPort.GetOpenPort(4020));
	m_strLocalIp = MyCommon::GetLocalIP();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropSocket::OnEnChangeEditListenPort()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	SetModified();
}

BOOL CPropSocket::OnApply()
{
	UpdateData();

	MyPort myPort;
	myPort.SetOpenPort(m_strListenPort);

	return CPropertyPage::OnApply();
}
