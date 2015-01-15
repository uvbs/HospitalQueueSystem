// UserSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "UserSetting.h"


// CUserSetting 对话框

IMPLEMENT_DYNAMIC(CUserSetting, CPropertyPage)

CUserSetting::CUserSetting()
: CPropertyPage(CUserSetting::IDD)
, m_userID(_T(""))
, m_userpassword(_T(""))
, m_baseIP(_T(""))
, m_basePassWord(_T(""))
, m_baseAccount(_T(""))
, m_databaseselect(0)
, m_ed_nursename(_T(""))
{
	//	numtemp=m_listUsername.GetCount()+1;
	pbase=new CADODatabase();
	m_loginPower=-1;
}

CUserSetting::~CUserSetting()
{
	if(pbase)
	{
		delete pbase;
		pbase=NULL;
	}
}

void CUserSetting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ED_NAME, m_userID);
	DDX_Text(pDX, IDC_ED_PASSWORD, m_userpassword);
	DDX_Control(pDX, IDC_CMB_POWER, m_UserPower);
	DDX_Control(pDX, IDC_LIST_USERNAME, m_listUsername);
	//	DDX_Text(pDX, IDC_ED_BASEIP, m_baseIP);
	//	DDX_Text(pDX, IDC_ED_ACCOUNT, m_baseAccount);
	//	DDX_Text(pDX, IDC_ED_BASEPASSWORD, m_basePassWord);
	//	DDX_Text(pDX, IDC_ED_ACCOUNT, m_baseAccount);
	//	DDX_Radio(pDX, IDC_RADIO1, m_databaseselect);
	DDX_Control(pDX, IDC_ED_NAME, m_edit_name);
	DDX_Control(pDX, IDC_ED_PASSWORD, m_edit_password);
	DDX_Text(pDX, IDC_ED_NURSENAME, m_ed_nursename);
	DDX_Control(pDX, IDC_LIST_YESOFFICE, m_list_condo);
	DDX_Control(pDX, IDC_LIST_ALLOFFICE, m_list_alloffice);
	DDX_Control(pDX, IDC_ED_NURSENAME, m_edit_nursename);
	DDX_Control(pDX, IDC_BN_ADDOFFICE, m_bn_addoffice);
	DDX_Control(pDX, IDC_BN_DELETEOFFICE, m_bn_deleteoffice);
}


BEGIN_MESSAGE_MAP(CUserSetting, CPropertyPage)
	ON_BN_CLICKED(IDC_BN_ADDUSER, &CUserSetting::OnBnClickedBnAdduser)
	ON_BN_CLICKED(IDC_BN_DELUSER, &CUserSetting::OnBnClickedBnDeluser)
	ON_LBN_SELCHANGE(IDC_LIST_USERNAME, &CUserSetting::OnLbnSelchangeListUsername)
	ON_EN_CHANGE(IDC_ED_PASSWORD, &CUserSetting::OnEnChangeEdPassword)
	ON_EN_CHANGE(IDC_ED_NAME, &CUserSetting::OnEnChangeEdName)
	ON_CBN_SELCHANGE(IDC_CMB_POWER, &CUserSetting::OnCbnSelchangeCmbPower)
	//	ON_BN_CLICKED(IDC_BASEOK, &CUserSetting::OnBnClickedBaseok)
	//	ON_BN_CLICKED(IDC_BASETEST, &CUserSetting::OnBnClickedBasetest)
	ON_EN_CHANGE(IDC_ED_NURSENAME, &CUserSetting::OnEnChangeEdNursename)
	ON_BN_CLICKED(IDC_BN_ADDOFFICE, &CUserSetting::OnBnClickedBnAddoffice)
	ON_BN_CLICKED(IDC_BN_DELETEOFFICE, &CUserSetting::OnBnClickedBnDeleteoffice)
	//	ON_MESSAGE(WM_MYPAITNMSG,OnReadChangeMsg)
END_MESSAGE_MAP()


// CUserSetting 消息处理程序

void CUserSetting::OnBnClickedBnAdduser()
{
	// TODO: 在此添加控件通知处理程序代
	if(m_loginPower!=USERTYPE_SYSADMIN)
	{
		MessageBox(_T("没有权限添加用户"),_T("注意"),MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	DeleteCandoOfficeString();
	numtemp=m_listUsername.GetCount()+1;
	CString str1;
	str1.Format(_T("%d"),numtemp);
	CString str=_T("新用户")+str1;
	m_listUsername.AddString(str);
	int nLast = m_listUsername.GetCount() - 1;
	if (nLast >= 0)
		m_listUsername.SetCurSel(nLast);
	m_listUsername.GetText(nLast,m_userID);
	numtemp++;
	m_userpassword=_T("123");	

	m_UserPower.SetCurSel(0);
	m_UserPower.EnableWindow();
	CString power;
	m_UserPower.GetLBText(m_UserPower.GetCurSel(),power);
	CAccount account;
	account.SetUserID(m_userID);
	account.SetUserPassword(m_userpassword);
	int nstate=0;
	if(power==_T("前台"))
	{
		nstate=2;
		account.SetUserPower(nstate);
	}
	if(power==_T("后台"))
	{
		nstate=3;
		account.SetUserPower(nstate);
	}
	if(power==_T("管理员"))
	{
		nstate=1;
		account.SetUserPower(nstate);
	}
	m_ed_nursename=_T("");
	m_listAccount.AddTail(account);

	UpdateData(FALSE);
	SetModified();
}

void CUserSetting::OnBnClickedBnDeluser()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_loginPower==USERTYPE_MANAGER || m_loginPower==USERTYPE_NURSE)
	{
		MessageBox(_T("没有权限删除用户"),_T("注意"),MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	if(m_listAccount.GetCount()==0)
	{
		return;
	}
	int index=m_listUsername.GetCurSel();
	CString UserID=_T("");
	m_listUsername.GetText(index,UserID);
	POSITION pos=m_listAccount.GetHeadPosition();
	CAccount account;

	if(index==m_listUsername.GetCount()-1 && m_listUsername.GetCount()==1)
	{
		m_listUsername.DeleteString(index);
	}
	else if(index==m_listUsername.GetCount()-1)
	{
		m_listUsername.DeleteString(index);
		m_listUsername.SetCurSel(index-1);
	}
	else
	{
		m_listUsername.DeleteString(index);
		m_listUsername.SetCurSel(index);
	}
	while(pos!=NULL)
	{
		account=m_listAccount.GetNext(pos);
		if(pos==NULL && account.GetUserID()==UserID)//最后一个
		{
			m_deleteAccount.AddTail(account);
			m_listAccount.RemoveTail();
		}
		else if(account.GetUserID()==UserID)
		{
			m_deleteAccount.AddTail(account);
			m_listAccount.GetPrev(pos);
			m_listAccount.RemoveAt(pos);
			break;
		}
	}
	if(m_listAccount.GetCount()>0)
	{
		OnLbnSelchangeListUsername();
	}
	else
	{
		m_userID=_T("");
		m_userpassword=_T("");
		m_UserPower.SetCurSel(2);
		m_ed_nursename=_T("");
		UpdateData(FALSE);
	}
	SetModified();
}

void CUserSetting::OnLbnSelchangeListUsername()
{
	// TODO: 在此添加控件通知处理程序代码
	//	m_index=m_listUsername.GetCurSel();

	if(CB_ERR==m_listUsername.GetCurSel())
	{
		return;
	}

	m_listUsername.GetText(m_listUsername.GetCurSel(),m_userID);

	if(m_loginPower==USERTYPE_SYSADMIN)
	{
		m_edit_name.SetReadOnly(FALSE);
		m_edit_password.SetReadOnly(FALSE);
		m_UserPower.EnableWindow();
		m_edit_nursename.SetReadOnly(FALSE);
		m_bn_deleteoffice.EnableWindow();
		m_bn_addoffice.EnableWindow();
	}
	else
	{
		if(m_userID==m_loginName)
		{
			m_edit_name.SetReadOnly(FALSE);
			m_edit_password.SetReadOnly(FALSE);
			m_UserPower.EnableWindow(FALSE);
			m_edit_nursename.SetReadOnly(FALSE);
			m_bn_deleteoffice.EnableWindow();
			m_bn_addoffice.EnableWindow();
		}
		else
		{
			m_edit_name.SetReadOnly(TRUE);
			m_edit_password.SetReadOnly(TRUE);
			m_UserPower.EnableWindow(FALSE);
			m_edit_nursename.SetReadOnly(TRUE);
			m_bn_deleteoffice.EnableWindow(FALSE);
			m_bn_addoffice.EnableWindow(FALSE);
		}
	}

	if(m_userID==_T("admin"))
	{
		m_edit_name.SetReadOnly(TRUE);
	}

	DeleteCandoOfficeString();

	POSITION pos=m_listAccount.GetHeadPosition();
	CAccount account;
	while(pos!=NULL)
	{
		account=m_listAccount.GetNext(pos);
		if(account.GetUserID()==m_userID)
		{
			m_userpassword=account.GetUserPassword();
			switch(account.GetUserPower())
			{
			case USERTYPE_SYSADMIN:
				m_UserPower.SetCurSel(2);
				break;
			case USERTYPE_MANAGER:
				m_UserPower.SetCurSel(1);
				break;
			case USERTYPE_NURSE:
				m_UserPower.SetCurSel(0);
				break;
			}
			m_ed_nursename=account.GetNurseName();
			int size=account.m_DoOfficeID.size();

			for(int i=0;i<size;i++)
			{
				POSITION pos = m_Nurse_Office.GetStartPosition();
				CString cKey;
				CString cID;
				for(; pos; )
				{
					m_Nurse_Office.GetNextAssoc(pos, cKey, cID);
					if(account.m_DoOfficeID[i]==cID)
					{
						m_list_condo.AddString(cKey);
						break;
					}
				}
			}
			break;
		}
	}

	UpdateData(FALSE);
}

BOOL CUserSetting::OnInitDialog()
{
	CPropertyPage::OnInitDialog();


	m_baseAccount=m_BaseConfig.GetServerAcount();
	m_basePassWord=m_BaseConfig.GetServerPassword();
	m_baseIP=m_BaseConfig.GetServerIP();
	m_baseDrive=m_BaseConfig.GetDataDrive();

	m_UserPower.AddString(_T("前台"));
	m_UserPower.AddString(_T("后台设置"));
	m_UserPower.AddString(_T("管理员"));
	m_UserPower.SetCurSel(2);
	//	readlogInfo();
	if(ConnectToBase())
	{
		SearchOffice();
		GetLoginPower();
		m_loginName=theApp.GetLoginID();
		WirteDataToList();
		POSITION pos=m_listAccount.GetHeadPosition();
		while(pos!=NULL)
		{
			CString name=_T("");
			name=m_listAccount.GetNext(pos).GetUserID();
			m_listUsername.AddString(name);
		}
		m_listUsername.SetCurSel(0);
		/*		
		if(m_baseDrive==_T("SQL Server"))
		{
		m_databaseselect=1;
		}
		if(m_baseDrive==_T("Oracle"))
		{	
		m_databaseselect=0;
		}
		if(m_baseDrive==_T("MySQL"))
		{
		m_databaseselect=2;
		}
		*/
	}
	OnLbnSelchangeListUsername();
	UpdateData(FALSE);
	return TRUE;
}
void CUserSetting::OnEnChangeEdPassword()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	if(LB_ERR==m_listUsername.GetCurSel())
	{
		AfxMessageBox(_T("请选择用户"));
		return;
	}

	m_listUsername.GetText(m_listUsername.GetCurSel(),m_userID);
	UpdateData();
	POSITION pos=m_listAccount.GetHeadPosition();
	CAccount account;
	while(pos!=NULL)
	{
		account=m_listAccount.GetNext(pos);
		if(pos==NULL && account.GetUserID()==m_userID)//最后一个
		{
			account.SetUserPassword(m_userpassword);
			m_listAccount.RemoveTail();
			m_listAccount.AddTail(account);
		}
		else if(account.GetUserID()==m_userID)
		{
			account.SetUserPassword(m_userpassword);
			m_listAccount.GetPrev(pos);
			m_listAccount.SetAt(pos,account);
			break;
		}
	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CUserSetting::OnEnChangeEdName()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if(LB_ERR==m_listUsername.GetCurSel())
	{
		AfxMessageBox(_T("请选择用户"));
		return;
	}

	CString name;
	m_listUsername.GetText(m_listUsername.GetCurSel(),name);
	UpdateData(TRUE);
	POSITION pos=m_listAccount.GetHeadPosition();
	CAccount account;

	m_index=m_listUsername.GetCurSel();
	m_listUsername.DeleteString(m_index);
	m_listUsername.InsertString(m_index,m_userID);
	m_listUsername.SetCurSel(m_index);

	while(pos!=NULL)
	{
		account=m_listAccount.GetNext(pos);
		if(pos==NULL && account.GetUserID()==name)
		{
			account.SetUserID(m_userID);
			m_listAccount.RemoveTail();
			m_listAccount.AddTail(account);
		}
		else if(account.GetUserID()==name)
		{
			account.SetUserID(m_userID);
			m_listAccount.GetPrev(pos);
			m_listAccount.SetAt(pos,account);
			break;
		}

	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

BOOL CUserSetting::OnApply()
{
	UpdateData();
//	if(theApp.m_pPropQueSetting->m_hWnd)
//		theApp.m_pPropQueSetting->OnApply();
	
	CAccount account;
	CString sql=_T("");
	CString serial_id=_T("");
	CString nurse_office_sql=_T("");
	//	sql=_T("Truncate Table Nurse_Office");
	//	pbase->Execute(sql);


	//删除的candooffice
	
	DELCANDOOFFICE deloffice;
	POSITION pos=m_delCandoOffice.GetHeadPosition();
	try
	{
		while(pos)
		{
			deloffice=m_delCandoOffice.GetNext(pos);
			sql.Format(_T("delete Nurse_Office where nurse_id='%s' and office_id='%s'"),
				deloffice.nurse_id,deloffice.office_id);
			pbase->Execute(sql);
		}
		//删除的account
		pos=m_deleteAccount.GetHeadPosition();
		while(pos!=NULL)
		{
			account=m_deleteAccount.GetNext(pos);
			sql.Format(_T("delete Nurse where nurse_id='%s'"),account.GetUserID());
			pbase->Execute(sql);
			sql.Format(_T("delete Nurse_Office where nurse_id='%s'"),account.GetUserID());
			pbase->Execute(sql);
		}


		pos=m_listAccount.GetHeadPosition();
		while(pos!=NULL)
		{
			account=m_listAccount.GetNext(pos);
			serial_id=NurseidIsInDataBase(account.GetUserID());
			if(serial_id!=_T(""))
			{
				if(account.GetNurseName()!=_T(""))
				{
					sql.Format(_T("update Nurse set nurse_id='%s',password='%s',power='%d',nurse_name='%s' where serial_id='%s'"),
						account.GetUserID(),account.GetUserPassword(),
						account.GetUserPower(),account.GetNurseName(),
						serial_id);
					pbase->Execute(sql);	
				}
				else
				{
					sql.Format(_T("update Nurse set nurse_id='%s',password='%s',power='%d',nurse_name=null where serial_id='%s'"),
						account.GetUserID(),account.GetUserPassword(),
						account.GetUserPower(),serial_id);
					pbase->Execute(sql);			
				}		
			}
			else
			{
				if(account.GetNurseName()!=_T(""))
				{
					sql.Format(_T("insert into Nurse(nurse_id,password,power,nurse_name,regtime) values('%s','%s','%d','%s',getdate())"),
						account.GetUserID(),account.GetUserPassword(),
						account.GetUserPower(),account.GetNurseName());
					pbase->Execute(sql);	
				}
				else
				{
					sql.Format(_T("insert into Nurse(nurse_id,password,power,regtime) values('%s','%s','%d',getdate())"),
						account.GetUserID(),account.GetUserPassword(),
						account.GetUserPower());
					pbase->Execute(sql);
				}
			}
			//刷新对应处理队列表
			CADORecordset *pre=new CADORecordset(pbase);
			sql.Format(_T("select * from nurse_office"));
			if(pre->Open(sql,CADORecordset::openQuery))
			{
				if(pre->IsBOF())
				{
					int size=account.m_DoOfficeID.size();
					for(int i=0;i<size;i++)
					{
						sql.Format(_T("insert into Nurse_Office (nurse_id,office_id) values('%s','%s')"),
							account.GetUserID(),account.m_DoOfficeID[i]);
						pbase->Execute(sql);
					}
				}
				else
				{
					CString Nurse_id,nurserial_id,office_id;

					int size=account.m_DoOfficeID.size();
					for(int i=0;i<size;i++)
					{
						BOOL flag=FALSE;
						pre->MoveFirst();
						while(!pre->IsEOF())
						{
							pre->GetFieldValue(_T("nurse_id"),Nurse_id);
							pre->GetFieldValue(_T("serial_id"),nurserial_id);
							pre->GetFieldValue(_T("office_id"),office_id);

							if(account.GetUserID()==Nurse_id && office_id==account.m_DoOfficeID[i])
							{
								flag=TRUE;
								sql.Format(_T("update Nurse_Office set nurse_id='%s',office_id='%s' where serial_id='%s'"),
									account.GetUserID(),account.m_DoOfficeID[i],nurserial_id);
								pbase->Execute(sql);
								break;
							}
							pre->MoveNext();
						}
						if(!flag)
						{
							sql.Format(_T("insert into Nurse_Office (nurse_id,office_id) values('%s','%s')"),
								account.GetUserID(),account.m_DoOfficeID[i]);
							pbase->Execute(sql);
						}
					}
				}
				pre->Close();
			}
			delete pre;
			pre=NULL;
		}
	}
	catch(_com_error& e)
	{

	}
	
	return CPropertyPage::OnApply();
}

BOOL CUserSetting::WirteDataToList()
{
	m_listAccount.RemoveAll();
	if(pbase->IsOpen())
	{
		CADORecordset* pre=new CADORecordset(pbase);
		CString sql=_T("");
		if(m_loginPower==USERTYPE_MANAGER)
		{
			sql.Format(_T("select * from Nurse where nurse_id='%s'"),
				theApp.GetLoginID());
		}
		else if(m_loginPower==USERTYPE_SYSADMIN)
		{
			sql=_T("select * from Nurse;");
		}
		if(sql.IsEmpty())
		{
			return FALSE;
		}
		CString nurse_id,password,regtime;
		CString nurse_name=_T("");
		CString office_id=_T("");
		int power=-1;
		CAccount account;
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
			{
				while(!pre->IsEOF())
				{
					pre->GetFieldValue(_T("nurse_id"),nurse_id);
					pre->GetFieldValue(_T("password"),password);
					pre->GetFieldValue(_T("nurse_name"),nurse_name);
					pre->GetFieldValue(_T("regtime"),regtime);
					pre->GetFieldValue(_T("power"),power);
					account.SetUserID(nurse_id);
					account.SetNurseName(nurse_name);
					account.SetUserPassword(password);
					account.SetUserRegtime(regtime);
					account.SetUserPower(power);
					m_listAccount.AddTail(account);
					pre->MoveNext();
				}
			}
			pre->Close();
		}

		CAccount account_office;
		sql=_T("select * from Nurse_Office");
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(!pre->IsBOF())
			{
				while(!pre->IsEOF())
				{
					pre->GetFieldValue(_T("nurse_id"),nurse_id);
					pre->GetFieldValue(_T("office_id"),office_id);
					POSITION pos=m_listAccount.GetHeadPosition();
					while(pos!=NULL)
					{
						account_office=m_listAccount.GetNext(pos);
						if(nurse_id==account_office.GetUserID() && pos==NULL)
						{
							account_office.AddOfficeID(office_id);
							m_listAccount.RemoveTail();
							m_listAccount.AddTail(account_office);
						}
						else if(nurse_id==account_office.GetUserID())
						{
							account_office.AddOfficeID(office_id);
							m_listAccount.GetPrev(pos);
							m_listAccount.SetAt(pos,account_office);
							break;
						}
					}
					pre->MoveNext();
				}
			}
			pre->Close();
		}
		delete pre;
		pre=NULL;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void CUserSetting::OnCbnSelchangeCmbPower()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_listAccount.GetCount()==0)
	{
		AfxMessageBox(_T("请添加用户"));
		return;
	}
	else if(LB_ERR==m_listUsername.GetCurSel())
	{
		AfxMessageBox(_T("请选择用户"));
		return;
	}

	CString name;
	m_listUsername.GetText(m_listUsername.GetCurSel(),name);
	POSITION pos;
	CAccount account;
	pos=m_listAccount.GetHeadPosition();
	while(pos!=NULL)
	{
		POSITION poslast=pos;
		account=m_listAccount.GetNext(pos);
		if(account.GetUserID()==name)
		{
			switch(m_UserPower.GetCurSel())
			{
			case 0:
				account.SetUserPower(USERTYPE_NURSE);
				break;
			case 1:
				account.SetUserPower(USERTYPE_MANAGER);
				break;
			case 2:
				account.SetUserPower(USERTYPE_SYSADMIN);
				break;
			}

			m_listAccount.SetAt(poslast,account);
			break;
		}
	}
	SetModified();
}
/*
void CUserSetting::OnBnClickedBaseok()
{
// TODO: 在此添加控件通知处理程序代码
UpdateData();
m_BaseConfig.SetServerAcount(m_baseAccount);
m_BaseConfig.SetServerIP(m_baseIP);
m_BaseConfig.SetServerPassword(m_basePassWord);
if(m_base.TestConnectToDb(m_baseIP,m_baseAccount,m_basePassWord,m_databaseselect))
{
CString name=_T("");
if(m_databaseselect==0)
{
name=_T("Oracle");
}
else if(m_databaseselect==1)
{
name=_T("SQL Server");
}
else if(m_databaseselect==2)
{
name=_T("MySQL");
}
m_BaseConfig.Save(name);
AfxMessageBox(_T("连接数据库成功"));
}
}
*/
/*
void CUserSetting::OnBnClickedBasetest()
{
// TODO: 在此添加控件通知处理程序代码
UpdateData();
if(m_base.TestConnectToDb(m_baseIP,m_baseAccount,m_basePassWord,m_databaseselect))
{
AfxMessageBox(_T("连接数据库成功"));
}
}

*/
BOOL CUserSetting::GetLoginPower()
{
	m_loginPower=theApp.GetUserPower();
	return TRUE;
}

BOOL CUserSetting::ConnectToBase()
{
	if(pbase->IsOpen())
	{
		pbase->Close();
	}
	pbase->SetConnectionString(_T("Driver={"+m_baseDrive+"};Server="+m_baseIP+";Database=Nurse;Uid="+m_baseAccount+";Pwd="+m_basePassWord+";"));

	try
	{
		if(pbase->Open())
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch(_com_error& e)
	{
		MessageBox(_T("数据库连接失败"),_T("Error"),MB_OK | MB_ICONERROR);
		return FALSE;
	}
}


void CUserSetting::SearchOffice()
{
	if(pbase->IsOpen())
	{
		m_Nurse_Office.RemoveAll();
		CString sql=_T("select office_id,name from office order by name");
		CString name=_T("");
		CString officeid=_T("");
		CADORecordset* pre=new CADORecordset(pbase);
		if(pre->Open(sql,CADORecordset::openQuery))
		{
			if(pre->IsBOF())
			{
				return;
			}
			while(!pre->IsEOF())
			{
				pre->GetFieldValue(_T("name"),name);
				pre->GetFieldValue(_T("office_id"),officeid);
				m_list_alloffice.AddString(name);
				m_Nurse_Office[name]=officeid;
				pre->MoveNext();
			}
		}
		delete pre;
		pre=NULL;
	}
}





void CUserSetting::OnEnChangeEdNursename()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	if(LB_ERR==m_listUsername.GetCurSel())
	{
		AfxMessageBox(_T("请选择用户"));
		return;
	}
	m_listUsername.GetText(m_listUsername.GetCurSel(),m_userID);
	UpdateData();
	POSITION pos=m_listAccount.GetHeadPosition();
	CAccount account;
	while(pos!=NULL)
	{
		account=m_listAccount.GetNext(pos);
		if(pos==NULL && account.GetUserID()==m_userID)//最后一个
		{
			account.SetNurseName(m_ed_nursename);
			m_listAccount.RemoveTail();
			m_listAccount.AddTail(account);
		}
		else if(account.GetUserID()==m_userID)
		{
			account.SetNurseName(m_ed_nursename);
			m_listAccount.GetPrev(pos);
			m_listAccount.SetAt(pos,account);
			break;
		}
	}
	SetModified();
	// TODO:  在此添加控件通知处理程序代码
}

void CUserSetting::OnBnClickedBnAddoffice()
{
	// TODO: 在此添加控件通知处理程序代码
	CAccount account;
	int index=m_list_alloffice.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	CString selectoffice=_T("");
	CString selectNurseID=_T("");
	m_list_alloffice.GetText(index,selectoffice);
	m_listUsername.GetText(m_listUsername.GetCurSel(),selectNurseID);

	int count=m_list_condo.GetCount();
	if(count==0)
	{
		m_list_condo.AddString(selectoffice);

		SetCandoOfficeNull();
		GetCandoOffice();

		POSITION pos=m_listAccount.GetHeadPosition();

		while(pos!=NULL)
		{
			POSITION lastPos=pos;
			account=m_listAccount.GetNext(pos);
			if(account.GetUserID()==m_userID)
			{
				account.SetOfficeID(m_CandoOfficeID);
				m_listAccount.SetAt(lastPos,account);
				break;
			}
		}
	}
	else
	{
		if(!IsHave(count,selectoffice))
		{
			m_list_condo.AddString(selectoffice);

			SetCandoOfficeNull();
			GetCandoOffice();

			POSITION pos=m_listAccount.GetHeadPosition();

			while(pos!=NULL)
			{
				POSITION lastPos=pos;
				account=m_listAccount.GetNext(pos);
				if(account.GetUserID()==m_userID)
				{
					account.SetOfficeID(m_CandoOfficeID);
					m_listAccount.SetAt(lastPos,account);
					break;
				}

			}

		}
	}
	SetModified();
}

BOOL CUserSetting::IsHave(int count,CString &str)
{
	CString candoOffice=_T("");
	for(int i=0;i<count;i++)
	{
		m_list_condo.GetText(i,candoOffice);
		if(m_Nurse_Office[candoOffice]==m_Nurse_Office[str])
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CUserSetting::OnBnClickedBnDeleteoffice()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=m_list_condo.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}

	CString selectCandoOffice=_T("");
	m_list_condo.GetText(index,selectCandoOffice);
	CString officeid=m_Nurse_Office[selectCandoOffice];//取出officeid
	m_list_condo.DeleteString(index);

	index=m_listUsername.GetCurSel();
	if(index==CB_ERR)
	{
		return;
	}
	CString Nurse_ID=_T("");
	m_listUsername.GetText(index,Nurse_ID);
	POSITION pos=m_listAccount.GetHeadPosition();
	CAccount account;
	DELCANDOOFFICE delCandoOffice;
	while(pos!=NULL)
	{
		POSITION lastpos=pos;
		account=m_listAccount.GetNext(pos);
		if(account.GetUserID()==Nurse_ID)
		{
			for(::std::vector<CString>::iterator iter = account.m_DoOfficeID.begin();
				iter!=account.m_DoOfficeID.end();iter++)
			{
				if(officeid == *iter)
				{
					delCandoOffice.nurse_id=account.GetUserID();
					delCandoOffice.office_id=officeid;
					m_delCandoOffice.AddTail(delCandoOffice);
					account.m_DoOfficeID.erase(iter);
					break;
				}
			}
			m_listAccount.SetAt(lastpos,account);
		}
	}
	SetModified();
}

void CUserSetting::GetCandoOffice()
{

	int count=m_list_condo.GetCount();
	if(count==0)
	{
		return;
	}
	CString candoOffice=_T("");
	for(int i=0;i<count;i++)
	{
		m_list_condo.GetText(i,candoOffice);
		CString str=m_Nurse_Office[candoOffice];

		m_CandoOfficeID.push_back(str);
	}
}

void CUserSetting::SetCandoOfficeNull()
{
	m_CandoOfficeID.clear();
}

CString CUserSetting::NurseidIsInDataBase(CString nurse_id)
{
	CADORecordset *pre=new CADORecordset(pbase);
	CString Nurse_ID=_T("");
	CString Serial_ID=_T("");
	CString sql=_T("select serial_id,nurse_id,nurse_name,password,power from Nurse");
	if(pre->Open(sql,CADORecordset::openQuery))
	{
		if(!pre->IsBOF())
		{
			while(!pre->IsEOF())
			{
				pre->GetFieldValue(_T("nurse_id"),Nurse_ID);
				pre->GetFieldValue(_T("serial_id"),Serial_ID);
				if(nurse_id==Nurse_ID)
				{
					return Serial_ID;
				}
				pre->MoveNext();
			}
		}
	}
	delete pre;
	return _T("");
}


CString CUserSetting::NurseidIsInNurse_Office(CString nurse_id)
{
	CString serial_id=_T("");
	CString Nurse_ID=_T("");
	CADORecordset *pre=new CADORecordset(pbase);
	CString sql=_T("select * from Nurse_Office");
	if(pre->Open(sql,CADORecordset::openQuery))
	{
		if(!pre->IsBOF())
		{
			while(!pre->IsEOF())
			{
				pre->GetFieldValue(_T("nurse_id"),Nurse_ID);
				pre->GetFieldValue(_T("serial_id"),serial_id);
				if(nurse_id==Nurse_ID)
				{
					return serial_id;
				}
			}
		}
	}

	return serial_id;
}

void CUserSetting::DeleteCandoOfficeString()
{
	int count=m_list_condo.GetCount();
	for(int i=count-1;i>=0;i--)
	{
		m_list_condo.DeleteString(i);
	}
}

void CUserSetting::ReFulshOffice()
{
	int count=m_list_alloffice.GetCount();
	for(int i=count-1;i>=0;i--)
	{
		m_list_alloffice.DeleteString(i);
	}
	SearchOffice();
}