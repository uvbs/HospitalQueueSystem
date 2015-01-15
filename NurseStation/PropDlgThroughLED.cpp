// WaitingRoomConfPropDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "WaitingRoomConfPropDlg.h"
#include "PropDlgThroughLED.h"
#include "OpenDatabase.h"


// CPropDlgThroughLED 对话框

IMPLEMENT_DYNAMIC(CPropDlgThroughLED, CPropertyPage)

CPropDlgThroughLED::CPropDlgThroughLED()
: CPropertyPage(CPropDlgThroughLED::IDD)
, m_strStbName(_T(""))
, m_strStbIp(_T(""))
, m_strStbPort(_T(""))
, m_strStbId(_T(""))
, m_strLEDAddress(_T(""))
, m_strLEDChannelNum(_T(""))
{
	m_strSqlQuerry = _T("select serial_id,name,ip,port,address,channel_num from ThroughLED;");
}

CPropDlgThroughLED::~CPropDlgThroughLED()
{
}

void CPropDlgThroughLED::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ED_LED_NAME, m_strStbName);
	DDX_Control(pDX, IDC_LIST_LED, m_lcRoomList);
	DDX_Text(pDX, IDC_ED_LED_IP, m_strStbIp);
	DDX_Text(pDX, IDC_ED_LED_PORT, m_strStbPort);
	DDX_Text(pDX, IDC_ED_LED_ID, m_strStbId);
	DDX_Text(pDX, IDC_ED_LED_ADDRESS, m_strLEDAddress);
	DDX_Text(pDX, IDC_ED_LED_CHANNEL_NUM, m_strLEDChannelNum);
}


BEGIN_MESSAGE_MAP(CPropDlgThroughLED, CPropertyPage)
	ON_BN_CLICKED(IDC_BN_ADDWAITROOM, &CPropDlgThroughLED::OnBnClickedBnAddwaitroom)
	ON_BN_CLICKED(IDC_BN_DELETEWAITROOM, &CPropDlgThroughLED::OnBnClickedBnDeletewaitroom)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LED, &CPropDlgThroughLED::OnLvnItemchangedListRoomlist)
	ON_EN_CHANGE(IDC_ED_LED_NAME, &CPropDlgThroughLED::OnEnChangeEdWaitname)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_LED, &CPropDlgThroughLED::OnNMCustomdrawListDocList)
	ON_EN_CHANGE(IDC_ED_LED_IP, &CPropDlgThroughLED::OnEnChangeEdStbIp)
	ON_EN_CHANGE(IDC_ED_LED_PORT, &CPropDlgThroughLED::OnEnChangeEdStbPort)
	ON_EN_CHANGE(IDC_ED_LED_ID, &CPropDlgThroughLED::OnEnChangeEdStbId)
	ON_EN_CHANGE(IDC_ED_LED_ADDRESS, &CPropDlgThroughLED::OnEnChangeEdLedAddress)
	ON_EN_CHANGE(IDC_ED_LED_CHANNEL_NUM, &CPropDlgThroughLED::OnEnChangeEdLedChannelNum)
END_MESSAGE_MAP()


// CPropDlgThroughLED 消息处理程序

BOOL CPropDlgThroughLED::OnInitDialog()
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
	m_lcRoomList.InsertColumn(0, _T("编号"), LVCFMT_CENTER, nColWidth * 2);
	m_lcRoomList.InsertColumn(1, _T("名称"), LVCFMT_CENTER, nColWidth * 6);
	m_lcRoomList.InsertColumn(2, _T("IP地址"), LVCFMT_CENTER, nColWidth * 4);
	m_lcRoomList.InsertColumn(3, _T("端口"), LVCFMT_CENTER, nColWidth * 2);
	m_lcRoomList.InsertColumn(4, _T("屏地址"), LVCFMT_CENTER, nColWidth * 2);
	m_lcRoomList.InsertColumn(5, _T("通道"), LVCFMT_CENTER, nColWidth * 2);

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

void CPropDlgThroughLED::OnBnClickedBnAddwaitroom()
{
	CString strNewRoomName;
	strNewRoomName.Format(_T("通屏通道%d"), m_set.GetRecordCount() + 1);
	m_set.AddNew();
	m_set.SetFieldValue(_T("name"), strNewRoomName);
	m_nCurSel = m_set.GetRecordCount() - 1;
	RefreshList();
	SetModified();
}

void CPropDlgThroughLED::OnBnClickedBnDeletewaitroom()
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
}

BOOL CPropDlgThroughLED::QueryDB(void)
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

void CPropDlgThroughLED::RefreshList()
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
			UpdateData(FALSE);
			return;
		}
		CString strValue;
		int nValue;
		m_set.MoveFirst();
		m_lcRoomList.DeleteAllItems();
		for(LONG iItem = 0;!m_set.IsEOF(); iItem++,m_set.MoveNext())
		{
			//select serial_id,name,ip,port,address,channel_num from ThroughLED
			//strValue.Format(_T("%d"), iItem+1);
			m_set.GetFieldValue(_T("serial_id"), strValue);
			m_lcRoomList.InsertItem(iItem, strValue);
			m_set.GetFieldValue(_T("name"), strValue);
			m_lcRoomList.SetItemText(iItem, 1, strValue);
			m_set.GetFieldValue(_T("ip"), strValue);
			m_lcRoomList.SetItemText(iItem, 2, strValue);
			m_set.GetFieldValue(_T("port"), strValue);
			m_lcRoomList.SetItemText(iItem, 3, strValue);
			m_set.GetFieldValue(_T("address"), strValue);
			m_lcRoomList.SetItemText(iItem, 4, strValue);
			m_set.GetFieldValue(_T("channel_num"), strValue);
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


void CPropDlgThroughLED::OnLvnItemchangedListRoomlist(NMHDR *pNMHDR, LRESULT *pResult)
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
		m_set.GetFieldValue(_T("serial_id"), strValue);
		m_strStbId = strValue;
		m_set.GetFieldValue(_T("name"), strValue);
		m_strStbName = strValue;
		m_set.GetFieldValue(_T("ip"), strValue);
		m_strStbIp = strValue;
		m_set.GetFieldValue(_T("port"), strValue);
		m_strStbPort = strValue;
		m_set.GetFieldValue(_T("address"), strValue);
		m_strLEDAddress = strValue;
		m_set.GetFieldValue(_T("channel_num"), strValue);
		m_strLEDChannelNum = strValue;

		UpdateData(FALSE);
	}

	*pResult = 0;
}

void CPropDlgThroughLED::OnEnChangeEdWaitname()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData();
	m_set.Edit();
	m_set.SetFieldValue(_T("name"), m_strStbName);
	RefreshList();
	SetModified();
}

void CPropDlgThroughLED::OnEnChangeEdStbIp()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData();
	m_set.Edit();
	m_set.SetFieldValue(_T("ip"), m_strStbIp);
	RefreshList();
	SetModified();
}

void CPropDlgThroughLED::OnEnChangeEdStbPort()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData();
	m_set.Edit();
	m_set.SetFieldValue(_T("port"), m_strStbPort);
	RefreshList();
	SetModified();
}

//列表控件失去焦点时仍能保持对应行选中

void CPropDlgThroughLED::OnNMCustomdrawListDocList(NMHDR *pNMHDR, LRESULT *pResult)
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



void CPropDlgThroughLED::OnEnChangeEdStbId()
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
		m_set.SetFieldValue(_T("serial_id"), m_strStbId);
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
				m_set.SetFieldValue(_T("serial_id"), strIdTemp);
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

void CPropDlgThroughLED::OnEnChangeEdLedAddress()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData();
	m_set.Edit();
	m_set.SetFieldValue(_T("address"), m_strLEDAddress);
	RefreshList();
	SetModified();
}

void CPropDlgThroughLED::OnEnChangeEdLedChannelNum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData();
	m_set.Edit();
	m_set.SetFieldValue(_T("channel_num"), m_strLEDChannelNum);
	RefreshList();
	SetModified();
}

BOOL CPropDlgThroughLED::OnApply()
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

void CPropDlgThroughLED::OnCancel()
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

void CPropDlgThroughLED::OnOK()
{
	//m_db.CommitTransaction();

	CPropertyPage::OnOK();

	//m_db.CommitTransaction();
}


