// WaitingRoomConfPropDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "WaitingRoomConfPropDlg.h"
#include "StbPropDlg.h"
#include "OpenDatabase.h"


// CStbPropDlg 对话框

IMPLEMENT_DYNAMIC(CStbPropDlg, CPropertyPage)

CStbPropDlg::CStbPropDlg()
	: CPropertyPage(CStbPropDlg::IDD)
	, m_strStbName(_T(""))
	, m_strStbIp(_T(""))
	, m_strStbPort(_T(""))
	, m_strStbInitText(_T(""))
	, m_strStbId(_T(""))
{
	m_strSqlQuerry = _T("select stb_id,stb_name,stb_type,stb_ip,stb_port,stb_init_text from SetTopBox;");

	m_strarrStbType.Add(_T("诊室门牌液晶机顶盒"));
	m_strarrStbType.Add(_T("科室队列液晶机顶盒"));
	m_strarrStbType.Add(_T("护士站综合液晶机顶盒"));
}

CStbPropDlg::~CStbPropDlg()
{
}

void CStbPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ED_STB_NAME, m_strStbName);
	DDX_Control(pDX, IDC_LIST_STB, m_lcRoomList);
	DDX_Text(pDX, IDC_ED_STB_IP, m_strStbIp);
	DDX_Text(pDX, IDC_ED_STB_PORT, m_strStbPort);
	DDX_Text(pDX, IDC_EDIT_STB_INITTEXT, m_strStbInitText);
	DDX_Text(pDX, IDC_ED_STB_ID, m_strStbId);
	DDX_Control(pDX, IDC_COMBO_STB_TYPE, m_cbStbType);
}


BEGIN_MESSAGE_MAP(CStbPropDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BN_ADDWAITROOM, &CStbPropDlg::OnBnClickedBnAddwaitroom)
	ON_BN_CLICKED(IDC_BN_DELETEWAITROOM, &CStbPropDlg::OnBnClickedBnDeletewaitroom)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STB, &CStbPropDlg::OnLvnItemchangedListRoomlist)
	ON_EN_CHANGE(IDC_ED_STB_NAME, &CStbPropDlg::OnEnChangeEdWaitname)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_STB, &CStbPropDlg::OnNMCustomdrawListDocList)
	ON_EN_CHANGE(IDC_ED_STB_IP, &CStbPropDlg::OnEnChangeEdStbIp)
	ON_EN_CHANGE(IDC_ED_STB_PORT, &CStbPropDlg::OnEnChangeEdStbPort)
	ON_EN_CHANGE(IDC_EDIT_STB_INITTEXT, &CStbPropDlg::OnEnChangeEditStbInitText)
	ON_EN_CHANGE(IDC_ED_STB_ID, &CStbPropDlg::OnEnChangeEdStbId)
	ON_CBN_SELCHANGE(IDC_COMBO_STB_TYPE, &CStbPropDlg::OnCbnSelchangeComboStbType)
END_MESSAGE_MAP()


// CStbPropDlg 消息处理程序

BOOL CStbPropDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//初始化列表控件
	ListView_SetExtendedListViewStyle(m_lcRoomList.m_hWnd, 
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES/*|LVS_EX_CHECKBOXES*/);
	//m_lcRoomList.DeleteAllItems();
	//while(m_lcRoomList.DeleteColumn(0));
	CRect rect;
	m_lcRoomList.GetClientRect(&rect);
	int nColWidth = rect.Width()/20;
	m_lcRoomList.InsertColumn(0, _T("编号"), LVCFMT_CENTER, nColWidth * 1);
	m_lcRoomList.InsertColumn(1, _T("名称"), LVCFMT_CENTER, nColWidth * 4);
	m_lcRoomList.InsertColumn(2, _T("类型"), LVCFMT_CENTER, nColWidth * 4);
	m_lcRoomList.InsertColumn(3, _T("地址"), LVCFMT_CENTER, nColWidth * 3);
	m_lcRoomList.InsertColumn(4, _T("端口"), LVCFMT_CENTER, nColWidth * 2);
	m_lcRoomList.InsertColumn(5, _T("启动显示"), LVCFMT_CENTER, nColWidth * 6);

	m_cbStbType.AddString(m_strarrStbType[0]);
	m_cbStbType.AddString(m_strarrStbType[1]);
	m_cbStbType.AddString(m_strarrStbType[2]);

	OpenDatabase odb;
	if(!odb.OpenDB(m_db))
	{
		return FALSE;
	}
	//m_db.BeginTransaction();
	if(!QueryDB())
	{
		return FALSE;
	}
	RefreshList();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CStbPropDlg::OnBnClickedBnAddwaitroom()
{
	CString strNewRoomName;
	strNewRoomName.Format(_T("液晶机顶盒%d"), m_set.GetRecordCount() + 1);
	m_set.AddNew();
	m_set.SetFieldValue(_T("stb_name"), strNewRoomName);
	m_nCurSel = m_set.GetRecordCount() - 1;
	RefreshList();
	SetModified();
}

void CStbPropDlg::OnBnClickedBnDeletewaitroom()
{
	if(m_nCurSel < 0)
	{
		//MessageBox(_T("未选中行"), _T("提示"));
		return;
	}
	m_set.Edit();
	m_set.Delete();
	RefreshList();
	SetModified();
	if(m_set.GetRecordCount() == 0)
	{
		m_db.Execute(_T("truncate table settopbox;"));
	}
}

BOOL CStbPropDlg::QueryDB(void)
{
	try
	{
		BOOL isQuerry = m_set.Open(m_db.m_pConnection, m_strSqlQuerry);
	}catch(_com_error&)
	{
		MessageBox(_T("数据库错误"));
		return FALSE; 
	}
	
	return TRUE;
}

void CStbPropDlg::RefreshList()
{
	try
	{
		if(!m_set.IsOpen())
		{
			QueryDB();
		}
		if(m_set.IsBOF() || m_set.GetRecordCount() == 0)//表为空
		{
			//MessageBox(_T("表为空"));
			m_lcRoomList.DeleteAllItems();
			m_nCurSel = -1;
			m_strStbId = _T("");
			m_strStbName = _T("");
			m_strStbIp = _T("");
			m_strStbPort = _T("");
			m_strStbInitText = _T("");
			UpdateData(FALSE);
			return;
		}
		CString strValue;
		int nValue;
		m_set.MoveFirst();
		m_lcRoomList.DeleteAllItems();
		for(LONG iItem = 0;!m_set.IsEOF(); iItem++,m_set.MoveNext())
		{
			//strValue.Format(_T("%d"), iItem+1);
			m_set.GetFieldValue(_T("stb_id"), strValue);
			m_lcRoomList.InsertItem(iItem, strValue);
			m_set.GetFieldValue(_T("stb_name"), strValue);
			m_lcRoomList.SetItemText(iItem, 1, strValue);
			m_set.GetFieldValue(_T("stb_type"), nValue);
			if(nValue >= 0 && nValue < m_strarrStbType.GetCount())
				m_lcRoomList.SetItemText(iItem, 2, m_strarrStbType[nValue]);
			m_set.GetFieldValue(_T("stb_ip"), strValue);
			m_lcRoomList.SetItemText(iItem, 3, strValue);
			m_set.GetFieldValue(_T("stb_port"), strValue);
			m_lcRoomList.SetItemText(iItem, 4, strValue);
			m_set.GetFieldValue(_T("stb_init_text"), strValue);
			m_lcRoomList.SetItemText(iItem, 5, strValue);
		}
		//m_lcRoomList.SetCurSel(m_nCurSel);
		//m_lcRoomList.SetHotItem(m_nCurSel);
		if(m_nCurSel > int(m_set.GetRecordCount() - 1))
		{
			m_nCurSel = m_set.GetRecordCount() - 1;
		}
		if(m_set.GetRecordCount() == 0)
		{
			m_nCurSel = -1;
		}
	}
	catch (_com_error& e)
	{
		if(0x80040e14 == e.Error())
		{
			MessageBox(_T("存在重复的编号！"));
		}
		else
		{
			CString errStr;
			_bstr_t bstrSource(e.Source());
			_bstr_t bstrDescription(e.Description());
			CString m_strErrorDescription(_T(""));
			m_strErrorDescription = (LPCSTR)bstrDescription ;
			CString str=_T("");
			str=(LPCTSTR)bstrSource;
			errStr.Format(_T("CADODataBase Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
				e.Error(), e.ErrorMessage(), str, m_strErrorDescription);
			MessageBox(errStr);
		}
		//QueryDB();
	}

	m_lcRoomList.SetItemState(m_nCurSel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_lcRoomList.EnsureVisible(m_nCurSel, TRUE);
}


void CStbPropDlg::OnLvnItemchangedListRoomlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	LONG iItem = pNMLV->iItem;
	m_nCurSel = iItem;
	if(iItem >= 0)
	{
		//m_set.m_pRecordset->Move(iItem,(LONG)adBookmarkFirst);
		m_set.MoveTo(iItem);
		CString strValue;
		int nValue;
		m_set.GetFieldValue(_T("stb_id"), strValue);
		m_strStbId = strValue;
		m_set.GetFieldValue(_T("stb_name"), strValue);
		m_strStbName = strValue;
		m_set.GetFieldValue(_T("stb_type"), nValue);
		m_cbStbType.SetCurSel(nValue);
		m_set.GetFieldValue(_T("stb_ip"), strValue);
		m_strStbIp = strValue;
		m_set.GetFieldValue(_T("stb_port"), strValue);
		m_strStbPort = strValue;
		m_set.GetFieldValue(_T("stb_init_text"), strValue);
		m_strStbInitText = strValue;

		UpdateData(FALSE);
	}

	*pResult = 0;
}

void CStbPropDlg::OnEnChangeEdWaitname()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData();
	m_set.Edit();
	m_set.SetFieldValue(_T("stb_name"), m_strStbName);
	RefreshList();
	SetModified();
}

void CStbPropDlg::OnEnChangeEdStbIp()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData();
	m_set.Edit();
	m_set.SetFieldValue(_T("stb_ip"), m_strStbIp);
	RefreshList();
	SetModified();
}

void CStbPropDlg::OnEnChangeEdStbPort()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData();
	m_set.Edit();
	m_set.SetFieldValue(_T("stb_port"), m_strStbPort);
	RefreshList();
	SetModified();
}

void CStbPropDlg::OnEnChangeEditStbInitText()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData();
	m_set.Edit();
	m_set.SetFieldValue(_T("stb_init_text"), m_strStbInitText);
	RefreshList();
	SetModified();
}

//列表控件失去焦点时仍能保持对应行选中

void CStbPropDlg::OnNMCustomdrawListDocList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	*pResult = CDRF_DODEFAULT;
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{   
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{           
		COLORREF clrNewTextColor, clrNewBkColor;
		int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		POSITION pos = m_lcRoomList.GetFirstSelectedItemPosition();
		int index = m_lcRoomList.GetNextSelectedItem(pos);
		if (index == nItem)//如果要刷新的项为当前选择的项，则将文字设为白色，背景色设为蓝色
		{             
			clrNewTextColor = RGB(255,255,255);//Set the text to white 
			clrNewBkColor = RGB(49,106,197);        //Set the background color to blue 
		}else{
			clrNewTextColor = RGB(0,0,0);        //set the text black
			clrNewBkColor = RGB(255,255,255);    //leave the background color white
		}
		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor; 
		*pResult = CDRF_DODEFAULT; 
	}
}



void CStbPropDlg::OnEnChangeEdStbId()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString strIdTemp = m_strStbId;
	UpdateData(TRUE);
	try
	{
		m_set.Edit();
		m_set.SetFieldValue(_T("stb_id"), m_strStbId);
		m_set.Update();
	}
	//catch (CADOException& e)
	catch (_com_error& e)
	{
		if(0x80040e14 == e.Error())
		{
			MessageBox(_T("存在重复的工号！"));
			try
			{
				m_set.Edit();
				m_set.SetFieldValue(_T("stb_id"), strIdTemp);
			}
			catch (_com_error& e)
			{

			}
			catch (...)
			{
			}
		}
	}
	catch (...)
	{
	}

	RefreshList();
	SetModified();
}

void CStbPropDlg::OnCbnSelchangeComboStbType()
{
	UpdateData();
	m_set.Edit();
	m_set.SetFieldValue(_T("stb_type"), m_cbStbType.GetCurSel());
	RefreshList();
	SetModified();
}

BOOL CStbPropDlg::OnApply()
{
	//if(m_set.IsConnectionOpen())
	//{
	//	m_set.Update();
	//}
	//if(((CNurseStationApp*)AfxGetApp())->m_pPropSetting->m_WaitingRoomConfPropDlg.m_hWnd)
	//{
	//	((CNurseStationApp*)AfxGetApp())->m_pPropSetting->m_WaitingRoomConfPropDlg.UpdateFromDB();
	//}
	try
	{
		//m_set.Update();
		//m_db.CommitTransaction();
		//m_db.BeginTransaction();
	}
	catch(_com_error& e)
	{

	}
	return CPropertyPage::OnApply();
}

void CStbPropDlg::OnCancel()
{
	try
	{
		//m_set.CancelUpdate();
		//m_db.RollbackTransaction();
	}
	catch(_com_error& e)
	{

	}

	CPropertyPage::OnCancel();
}

void CStbPropDlg::OnOK()
{
	//m_db.CommitTransaction();

	CPropertyPage::OnOK();

	//m_db.CommitTransaction();
}
