// DoctorCallerDlg.h : 头文件
//

#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <afxext.h>
//#include "ado2.h"
//#include "NurseDef.h"
//#include "SockDef.h"
//#include "afxmt.h"
//#include "MySocketUDP.h"
//#include "afxsock.h"
//#include "JttsAPI.h"
#include "afxmt.h"


// CChatDlg 对话框
class CChatDlg : public CDialog
{
// 构造
public:
	//CChatDlg(CWnd* pParent = NULL);	// 标准构造函数
	CChatDlg(void* pController, CWnd* pParent = NULL);

// 对话框数据
	enum { IDD = IDD_CHAT_DIALOG };

	void AddUser(void* pUser, BOOL bLogin);
	void DelUser(void* pUser);
	void AddChat(void* pUser, CString strChat, BOOL bPublic);
	void AddChat(CString strUserName, CString strChat, BOOL bPublic, int);

	void MoveControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog();
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:

	void AddLoginMsg(CString strUserName);
	void AddHeartBeatMsg(CString strUserName);
	void AddQuitMsg(CString strUserName);

	void AddUserMsg(CString strUserName, CString strMsg);

	void AddMsg(CString strMsg);


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonSend();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()


private:
	CComboBox m_cbUserList;
	CString m_strChatRec;
	CString m_strSendData;
	CEdit m_editChatRec;
	CEdit m_editSendData;
	CButton m_btnSend;

	int m_nEveryoneItem;

	//void EnableButtons();
	//void DisableButtons();

	BOOL SendMsg();

	void* m_pQueueController;
	CMap<void* , void*&, int, int&> m_mapUserCb;

	CMutex m_mtChat;

};

