#pragma once
#include "afxwin.h"
#include "PoolManage.h"
#include "ConnectToBase.h"
#include "BaseConfig.h"
#include "Account.h"
#include "NurseDef.h"
// CUserSetting 对话框

#include "ado2.h"

class CUserSetting : public CPropertyPage
{
	DECLARE_DYNAMIC(CUserSetting)

public:
	CUserSetting();
	virtual ~CUserSetting();

	// 对话框数据
	enum { IDD = IDD_PROPUSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBnAdduser();
	afx_msg void OnBnClickedBnDeluser();
	BOOL OnInitDialog();
	CString m_userID;
	CString m_userpassword;
	CComboBox m_UserPower;
	CListBox m_listUsername;

private:
	int numtemp;
public:
	BOOL GetLoginPower();
	afx_msg void OnLbnSelchangeListUsername();
	CPoolManage m_pool;
	CList<CAccount,CAccount&> m_listAccount;
	afx_msg void OnEnChangeEdPassword();
	afx_msg void OnEnChangeEdName();
	BOOL OnApply();
	BOOL WirteDataToList();
	int m_index;
	afx_msg void OnCbnSelchangeCmbPower();
	CString m_baseIP;
	//	CEdit m_baseAccount;
	CString m_basePassWord;
	//	afx_msg void OnBnClickedBaseok();
	//	afx_msg void OnBnClickedBasetest();

	CBaseConfig m_BaseConfig;

	CConnectToBase m_base;
	CADODatabase *pbase;
	CString m_baseAccount;
	CString m_baseDrive;
	int m_databaseselect;
	int m_loginPower;
	CString m_loginName;
	CString m_loginIP;
	CEdit m_edit_name;
	CEdit m_edit_password;
	BOOL ConnectToBase();
	CString m_ed_nursename;


	void SearchOffice();


	CListBox m_list_condo;
	CListBox m_list_alloffice;
	CEdit m_edit_nursename;
	afx_msg void OnEnChangeEdNursename();
	afx_msg void OnBnClickedBnAddoffice();
	afx_msg void OnBnClickedBnDeleteoffice();

	CMap<CString,LPCTSTR,CString,CString&> m_Nurse_Office;
	BOOL IsHave(int count,CString& str);
	std::vector<CString> m_CandoOfficeID;
	void GetCandoOffice();
	CButton m_bn_addoffice;
	CButton m_bn_deleteoffice;
	void SetCandoOfficeNull();
	CString NurseidIsInDataBase(CString nurse_id);
	CString NurseidIsInNurse_Office(CString nurse_id);

	void DeleteCandoOfficeString();

	CList<CAccount,CAccount&> m_deleteAccount;
	//	afx_msg HRESULT OnReadChangeMsg(WPARAM wParam,LPARAM lParam);
	void ReFulshOffice();

	typedef struct _tagDelCandoOffice
	{
		CString		office_id;
		CString     nurse_id;
	} DELCANDOOFFICE,*LPDELCANDOOFFICE;

	CList<DELCANDOOFFICE,DELCANDOOFFICE&> m_delCandoOffice;
};
