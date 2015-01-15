#pragma once


// CPropSocket 对话框

class CPropSocket : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropSocket)

public:
	CPropSocket();
	virtual ~CPropSocket();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_SOCKET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strLocalIp;
	CString m_strListenPort;
	afx_msg void OnEnChangeEditListenPort();
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
};
