// DoctorCallerDlg.h : 头文件
//

#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <afxmt.h>
//#define WM_MY_TRAYICON (WM_USER+1)
//#define WM_MY_CLOSEWINDOW (WM_USER+2)
//#include "..\NurseStation\SocketServer.h"
//#include "..\NurseStation\SocketClient.h"
//#include "..\NurseStation\MySocketUDP.h"
//#include "..\NurseStation\NurseDef.h"
#include "DoctorController.h"
#include "../NurseStation/ListCtrlCl.h"
#include "../NurseStation/ado2.h"
#include "DownDiscardListDlg.h"


// CDoctorCallerDlg 对话框
class CDoctorCallerDlg : public CDialog
{
// 构造
public:
	CDoctorCallerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DOCTORCALLER_DIALOG };
	//enum { IDD = IDD_DOCTORCALLER_DIALOG1 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CDoctorController* GetController()
	{
		return m_pDoctorController;
	}


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedButtonCall();
	afx_msg void OnBnClickedButtonRecall();
	afx_msg void OnBnClickedButtonDiscard();
	afx_msg void OnBnClickedButtonWait();
	afx_msg void OnBnClickedButtonWelcome();
	afx_msg void OnBnClickedButtonEvaluate();
	afx_msg void OnBnClickedButtonThanks();
	afx_msg void OnBnClickedButtonPause();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void AddTrayIcon();
	void RemoveTrayIcon();
	BOOL TaskBarAddIcon(HWND hwnd, UINT uID, HICON hicon, CString strTip);
	BOOL TaskBarDeleteIcon(HWND hwnd, UINT uID);
	afx_msg LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSubmenuDisplay();
	afx_msg void OnSubmenuQuit();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnMenuitemDoccall();
	afx_msg void OnMenuitemDocwait();
	afx_msg void OnNMRClickListPatient(NMHDR *pNMHDR, LRESULT *pResult);
	//发送医生指令
	BOOL SendCmd(DoctorCmdEnum eDoctorCmdEnum, CString strQueSerialId = _T(""));
	//void OnShowPatients(LPCMDRESULT pPatients);
	//void OnShowPatients(DbParam& dbParam);
	int OnShowPatients(CADORecordset& rset);

	//void InitCtrls();
	static UINT SoundThread(LPVOID pParam);
	void StopSoundThread();
	void SoundAdd(CString str);
	int GetCurSel();

public:
	CDoctorController* m_pDoctorController;

	//CListCtrl m_lstPatientList;
	CListCtrlCl m_lstPatientList;
	CButton m_btnCall;
	CButton m_btnRecall;
	CButton m_btnDiscard;
	CButton m_btnWait;
	//CButton m_btnWelcome;
	//CButton m_btnEvaluate;
	//CButton m_btnThanks;
	CButton m_btnPause;
	CButton m_btnChat;
	//CStatic m_stDocName;
	//CStatic m_stCurPatient;
	//CStatic m_stCurPatientName;

	CComboBox m_cbConsult;

	int m_nEveryoneItem;
	//CString m_strUserID;
	//CString m_strUserName;
	//BOOL m_bCallSpecial;

	BOOL m_bHaveCall, m_bHaveWait;
	CWinThread* m_pSoundThread;
	BOOL m_bSoundThread;
	CMutex m_Mutex;
	CList<CString> m_lstStr;
	//CString m_strWavLibPath;

	typedef struct {
		CString strQueSerialId;
		QueueStatusEnum eStatus;

	} PATIENTINFO, *LPPATIENTINFO;

	CMap<int, int, PATIENTINFO, PATIENTINFO&> m_mapPatientList;

public:
	afx_msg void OnBnClickedButtonChat();

	BOOL BeforeUserQuit();

private:
	CString GetStatus(QueueStatusEnum qs);

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void OnUpdatePatient();
	BOOL m_bCanRefrush;
	CStatic m_stOptStatus;
	afx_msg void OnBnClickedButtonReflush();
	void SetOptStatus(DoctorCmdEnum eDocCmd, BOOL bSuccess);
	BOOL m_bOptStatusSuccess;
	afx_msg void OnBnClickedButtonHistory();

	CDlgDoctorHistory* m_pDlgDoctorHistory;
	BOOL m_bShowHistoryDlg;

	int m_nCallTime;

	void SetTitle();
	CStatic m_stStatus;
	afx_msg void OnBnClickedButtonConf();

	//CComboBox m_cbConsult;
	//CComboBox* m_pcbConsult;
	afx_msg void OnCbnDropdownComboConsult();
	//CArray<CString*> m_arrPStr;
	afx_msg void OnCbnSelchangeComboConsult();
	typedef struct  
	{
		CString strConsultId;
		CString strConsultName;
	} ConsultInfo;
	CMap<int, int, ConsultInfo, ConsultInfo&> m_mapConsult;
	void InitComboxConsult();

	BOOL m_bDlgInited;

	BOOL m_bInitConslut;
};

