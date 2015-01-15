#pragma once
#include "afxwin.h"
//#include "../NurseStation/NurseDef.h"
//#include "../NurseStation/SockDef.h"
//#include "../NurseStation/MySocketUDP.h"
#include "../NurseStation/LoginController.h"


// CLoginDlg 对话框


class CLoginDlg : public CDialog, public CLoginController
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strPassword;
	CString m_strDoctorID;
	//CDoctorCallerDlg* m_pParentDlg;
	CEdit m_editUserID;
	typedef struct {
		//BOOL	bRembAccount;
		BOOL	bRembPassword;
		BOOL	bAutoLogin;
		WCHAR	wszAccount[MAX_ID_LENGTH];
		WCHAR	wszPassword[64];
	} LOGINCONF, *LPLOGINCONF;
	BOOL WriteConfToFile(LPLOGINCONF conf, CString strFilePath);
	BOOL ReadConfFromFile(LPLOGINCONF conf, CString strFilePath);
	void ReadConf();
	void SaveConf();
	CString GetModuleDir(void);
	void Login();
	//void OnRcvVerifyRes(LPLOGINRESULT pLoginRes, WCHAR* wszIP, USHORT iPort);
	//BOOL Char2WChar(wchar_t *dstBuf, size_t len,const char *srcBuf);
	//BOOL SocketInit();
	//BOOL SocketClose();
	void OnRecvLoginResult(LPCMDRESULT pLoginResult, LPSOCKADDR_IN psockddr);

	//CButton m_ckbAccount;
	CButton m_ckbPassword;
	CButton m_ckbAutoLogin;
	CEdit m_editPassword;
	CString m_strConfPath;

	//SocketClient* m_pSocketClient;
	//SocketServer* m_pSocketServer;
	//MySocketUDP* m_pMySocketUDP;
	//static UINT SocketListenThreadProc(LPVOID lpParam);
	//CWinThread* m_pSocketListenThread;
	//BOOL m_bSocketListenThread;
	//USHORT GetPort();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	afx_msg void OnBnClickedCheckAutologin();
	CButton m_btnLogin;

	CString m_strNurseId;
	CString GetNurseId(){return m_strNurseId;}
	void SetNurseId(CString strNurseId){m_strNurseId = strNurseId;}

	//USHORT m_nNursePort;

	//virtual void SendVerify();
	afx_msg void OnBnClickedButtonLoginConf();
};

//void OnRcvPackage(void* pUDPPackege, sockaddr_in* psockddr);
