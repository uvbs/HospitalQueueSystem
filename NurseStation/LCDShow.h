#pragma once
#include "afxwin.h"
#include "NurseDef.h"

// CLCDShow 对话框
#include "ReadHisData.h"
#include "QueueServerDef.h"

class CLCDShow : public CDialog
{
	DECLARE_DYNAMIC(CLCDShow)

public:
	CLCDShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLCDShow();

// 对话框数据
	enum { IDD = IDD_DIALOG_SHOWMSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	HICON m_hIcon;

	CComboBox m_com_waitroom;
	CEdit m_ed_content;
	
	
	CReadHisData m_read;

	BOOL OpenMyDataBse();

	typedef struct _tagTopBoxInfo
	{
		CString stb_id;
		CString stb_name;
	}TOPBOXINFO,*LPTOPBOXINFO;
	
	CMap<int,int,TOPBOXINFO,TOPBOXINFO&> m_mapto_topbox;

	void SearchWaitRoom();
	afx_msg void OnBnClickedSendtoScreen();
	CStatic m_send_return;
	afx_msg void OnEnChangeEdContent();
	HACCEL m_hSendReturn;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void AttachToComboxTopBox();
	CComboBox m_com_typemsg;



	void AttachToComboxType();
//	CMap<int,int,CString,CString&> m_map_msgtype;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCancel();
};
