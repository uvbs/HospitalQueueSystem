// DoctorManageDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ado2.h"
//#include "DocDef.h"
#include "resource.h"		// 主符号
//#include "SockDef.h"
#include "NurseDef.h"
#include "BaseConfig.h"
#include "ListCtrlCl.h"

/*科室结构*/
typedef struct _tagOffice
{
	CString			strOfficeID;
	CString			strOfficeName;
} OFFICE, *LPOFFICE;

/*诊室结构*/
typedef struct _tagConsult
{
	CString			strConsultID;
	CString			strConsultName;
} CONSULT, *LPCONSULT;

/*用户性别信息*/
typedef struct _tagUserGender
{
	GenderEnum		eGenderID;
	CString			strGenderName;

}USERGENDER, *LPUSERGENDER;

// CDoctorManageDlg 对话框
class CDoctorManageDlg : public CPropertyPage
{
// 构造
public:
	CDoctorManageDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CDoctorManageDlg();

// 对话框数据
	enum { IDD = IDD_DOCTORMANAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	CButton m_btnDel;
	//CButton m_btnSetDisplay;
	CButton m_btnSetPhoto;
	//CButton m_btnSetSound;
	CEdit m_editName;
	CEdit m_editId;
	CComboBox m_cbGender;
	CComboBox m_cbOffice;
	CComboBox m_cbConsult;
	CEdit m_editPosition;
	CEdit m_editPassword;
	CStatic m_picPhoto;
	//CListCtrlCl m_listDocList;
	CListCtrl m_listDocList;
	CButton m_btnAdd;
	CButton m_btnExport;
	CButton m_btnImport;

	CString m_strCallerID;
	CString m_strEvaluatorID;
	CString m_strName;
	CString m_strId;
	CString m_strGender;
	CString m_strPosition;
	CString m_strPassword;
	CString m_strPhotoPath;

	CString m_strPicFilter;
	CString m_strFileFilter;

	CADODatabase m_db;
	CADORecordset m_set;
	CString m_strSqlConn;
	CString m_strSqlQuerry;
	LONG m_nCurSel;

	CString m_strSoundCall;
	CString m_strSoundWait;
	CString m_strDisplayCall;
	CString m_strDisplayWait;
	BOOL m_bCallSpecial;
	//CString m_strRecallTimes;
	int m_iRecallTimes;
	//int m_nConfType;
	int m_nWaitNum;
	int m_bWaitWhenCall;

public:
	afx_msg void OnBnClickedButtonDel();
	//afx_msg void OnBnClickedButtonSave();

	afx_msg void OnBnClickedButtonSetPhoto();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnLvnItemchangedListDocList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListDocList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditName();
	afx_msg void OnEnChangeEditId();
	afx_msg void OnCbnSelchangeComboGender();
	afx_msg void OnEnChangeEditPosition();
	afx_msg void OnEnChangeEditPassword();
	afx_msg void OnCbnSelchangeComboOffice();
	afx_msg void OnCbnSelchangeComboConsult();
	//void EnableSave(void);
	void UpdateDocInfo();
	//virtual BOOL OnSetActive(void);
	void InitControls();
	BOOL QueryDB(void);
	void RefreshList(void);
	CString GetTransStr(CString strSrc);
	CString GetOfficeName(CString strOfficeID);
	int GetOfficeCbItem(CString strOfficeID);
	CString GetConsultName(CString strConsultID);
	int GetConsultCbItem(CString strConsultID);
	CString GetGenderName(int eGenderID);
	int GetGenderCbItem(int eGenderID);
	void ShowList(void);
	void ShowPhoto(CString strPhotoPath);
	BOOL ShowPhoto(CString strPhotoPath, CWnd* pWnd);
	BOOL SetStaticPicture(CWnd* pWnd, int iPicture);
	//BOOL ShowPhoto(CString& strPhotoPath, HDC* hdc);
	int ReadFileToDB(CString strFilePath);
	int WriteFileFromDB(CString strFilePath);

	BOOL InsertDocToDB(CHAR** docInfoStr);
//	int SplitStrToStrArray(CHAR* docInfoStr, CHAR (*outArray)[512]);
	int SplitWStrToWStrArray(WCHAR* docInfoStr, WCHAR** outArray);
	//BOOL Char2WChar(wchar_t *pwstr,size_t len,const char *str);
	//BOOL WChar2Char(char* dstBuf, size_t len, wchar_t* src);

	CMap<int, int, OFFICE, OFFICE&> m_mapOfficeList;
	BOOL LoadOfficeList();

	CMap<CString, LPCWSTR, CList<CONSULT>*, CList<CONSULT>*&> m_mapOfficeConsults;
	CMap<int, int, CONSULT, CONSULT&> m_mapConsultList;
	BOOL LoadConsults();
	BOOL UpdateConsultList();
	CMap<CString, LPCWSTR, CString, LPCWSTR> m_mapConsultIDConsultName;

	//CMap<int, int, USERGENDER, USERGENDER&> m_mapGenderList;
	//BOOL LoadGenderList();
	//BOOL OpenDB(CADODatabase &db);
	virtual BOOL DestroyWindow();

	virtual BOOL OnApply();

	//CString m_strSoundCallConf;
	//CString m_strSoundWaitConf;
	//CString m_strDisplayCallConf;
	//CString m_strDisplayWaitConf;
	//afx_msg void OnBnClickedButtonSetSoundCall();
	//afx_msg void OnBnClickedButtonSetDisplayCall();
	//afx_msg void OnBnClickedButtonSetSoundWait();
	//afx_msg void OnBnClickedButtonSetDisplayWait();
	
	afx_msg void OnEnChangeEditCallerid();
	afx_msg void OnEnChangeEditEvaluatorid();
	//CStatic m_picSound;
	//CStatic m_picDisplay;
	virtual void OnCancel();
	/////////////////////////////小宋这里是刷新数据库//////////////////////
	//////////////中间gender注意一边是varchar2,一边是int注意对接改这里有异常
	/*
	void FlushHisDataToMyData();
	BOOL ConnectToHis();
	CBaseConfig m_baseConfig;
	CADODatabase* m_phisDataBase;
	*/
	afx_msg void OnBnClickedButtonCallConf();
	CComboBox m_cbExpert;

	//void LoadExpertList();
	afx_msg void OnCbnSelchangeComboExpert();
	virtual BOOL OnSetActive();

	CString m_strPhotoDir;
	virtual void OnOK();
	CString m_strLoginId;
	afx_msg void OnEnChangeEditLoginId();
};
