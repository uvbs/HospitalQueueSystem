// PropPagePrint.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PropPageConfigPrint.h"
#include "DoFile.h"
#include "PropPrintSet.h"

// CPropPageConfigPrint 对话框

IMPLEMENT_DYNAMIC(CPropPageConfigPrint, CPropertyPage)

CPropPageConfigPrint::CPropPageConfigPrint()
	: CPropertyPage(CPropPageConfigPrint::IDD)
{
	memset(&m_printRectInfo,0,sizeof(m_printRectInfo));
	m_printRectInfo.isUsePrint = TRUE;
	CDoFile dofile;
	m_printInfoPath = dofile.GetExeFullFilePath();
	m_printInfoPath += _T("\\PrintInfo");
	dofile.CreateMyDirectory(m_printInfoPath);
}

CPropPageConfigPrint::~CPropPageConfigPrint()
{
}

void CPropPageConfigPrint::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRINT_RECT, m_sta_printRect);
}


BEGIN_MESSAGE_MAP(CPropPageConfigPrint, CPropertyPage)
	ON_BN_CLICKED(IDC_ADD_TEXT, &CPropPageConfigPrint::OnBnClickedAddText)
	ON_BN_CLICKED(IDC_ADD_PIC, &CPropPageConfigPrint::OnBnClickedAddPic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_PRINT_EDIT, &CPropPageConfigPrint::OnPrintEdit)
	ON_COMMAND(ID_PRINT_DELETE, &CPropPageConfigPrint::OnPrintDelete)
	ON_BN_CLICKED(IDC_CUR_TIME, &CPropPageConfigPrint::OnBnClickedCurTime)
	ON_BN_CLICKED(IDC_NUM, &CPropPageConfigPrint::OnBnClickedNum)
	ON_BN_CLICKED(IDC_WAIT_NUM, &CPropPageConfigPrint::OnBnClickedWaitNum)
	ON_BN_CLICKED(IDC_CLI_NAME, &CPropPageConfigPrint::OnBnClickedCliName)

	ON_BN_CLICKED(IDC_QUEUE, &CPropPageConfigPrint::OnBnClickedQueue)
	ON_BN_CLICKED(IDC_BN_CLLEV, &CPropPageConfigPrint::OnBnClickedBnCllev)
	ON_WM_KEYDOWN()

	ON_BN_CLICKED(IDC_SERIAL_ID, &CPropPageConfigPrint::OnBnClickedSerialId)
	ON_BN_CLICKED(IDC_CONSULT_NAME, &CPropPageConfigPrint::OnBnClickedConsultName)
	ON_BN_CLICKED(IDC_DOCTOR_NAME, &CPropPageConfigPrint::OnBnClickedDoctorName)
	ON_BN_CLICKED(IDC_PRINT_ISCHECK, &CPropPageConfigPrint::OnBnClickedPrintIscheck)
END_MESSAGE_MAP()

// CPropPageConfigPrint 消息处理程序

BOOL CPropPageConfigPrint::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO:  在此添加额外的初始化
	//////////////////////////////////////////////
	LoadPrintRectInfoFormFile();
	CButton* pButton = (CButton*)GetDlgItem(IDC_PRINT_ISCHECK);
	ASSERT(pButton);
	if(m_printRectInfo.isUsePrint)
	{
		pButton->SetCheck(BST_CHECKED);
	}
	else
	{
		pButton->SetCheck(BST_UNCHECKED);
	}
	//////////////////////////////////////////////
	m_trackContrl.Create(this);
	m_trackContrl.SetForAdjustMaxRect(TRUE);
	m_trackContrl.SetMaxRect(GetShowRect());

	ReadPrintInfoFromFile();
	CreateAllPrintGDI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropPageConfigPrint::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPropertyPage::OnPaint()
}

BOOL CPropPageConfigPrint::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPropertyPage::OnEraseBkgnd(pDC);
	m_trackContrl.Draw(pDC);
	return TRUE;
}

void CPropPageConfigPrint::OnBnClickedAddText()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = FALSE;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
		SetModified();
	}
}

void CPropPageConfigPrint::OnBnClickedAddPic()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = TRUE;
	m_bIsAddText = FALSE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = FALSE;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
		SetModified();
	}
}

CRect CPropPageConfigPrint::GetShowRect()
{
	m_sta_printRect.GetWindowRect(m_printRect);
	m_printRect.left += 2;
	m_printRect.top += 8;
	m_printRect.right -= 2;
	m_printRect.bottom -= 2;
	ScreenToClient(m_printRect);
	return m_printRect;
}
void CPropPageConfigPrint::OnLButtonDown(UINT nFlags, CPoint point)
{
	int result = m_trackContrl.Track( point,nFlags);
	if(result == TRACK_CTR_MIDORHANDLE)
	{
		SetModified();
	}

	CPropertyPage::OnLButtonDown(nFlags, point);
}

void CPropPageConfigPrint::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int result = m_trackContrl.Track( point,nFlags);
	if(result == TRACK_CTR_MIDORHANDLE)
	{
		OnPrintEdit();
	}
	CPropertyPage::OnLButtonDblClk(nFlags, point);
}

void CPropPageConfigPrint::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropertyPage::PreSubclassWindow();
}

BOOL CPropPageConfigPrint::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( m_trackContrl.SetCursor( nHitTest, message ) )
		return TRUE;
	return CPropertyPage::OnSetCursor(pWnd, nHitTest, message);
}

void CPropPageConfigPrint::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt;
	GetCursorPos(&pt);
	int result = m_trackContrl.Track( point,nFlags);
	if(result == TRACK_CTR_RBUTTON)
	{
		CMenu menu;
		menu.LoadMenu(IDR_MENU_PRINT);
		CMenu *pContentMenu=menu.GetSubMenu(0);
		pContentMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this); //在指定位置显示弹出菜单
		pContentMenu->DestroyMenu();
	}
	CPropertyPage::OnRButtonDown(nFlags, point);
}

void CPropPageConfigPrint::OnPrintEdit()
{
	// TODO: 在此添加命令处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = FALSE;
	m_bIsForEdit = TRUE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = FALSE;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
		SetModified();
	}
}

void CPropPageConfigPrint::OnPrintDelete()
{
	// TODO: 在此添加命令处理程序代码
	m_trackContrl.DeleteRightBnSelectCtr();
	{
		SetModified(TRUE);
	}
}

void CPropPageConfigPrint::OnBnClickedCurTime()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = FALSE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = TRUE;
	m_bIsForPrintObject = FALSE;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
		SetModified();
	}
}
//排队号码
void CPropPageConfigPrint::OnBnClickedNum()
{
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintClientNum;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
		SetModified();
	}
}
//等候人数
void CPropPageConfigPrint::OnBnClickedWaitNum()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintWaitNum;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
		SetModified();
	}
}
//客户姓名
void CPropPageConfigPrint::OnBnClickedCliName()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintClientName;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
		SetModified();
	}
}
//所在队列
void CPropPageConfigPrint::OnBnClickedQueue()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintType;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
		SetModified();
	}
}
//客户星级
void CPropPageConfigPrint::OnBnClickedBnCllev()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintClientLevel;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
		SetModified();
	}
}

BOOL CPropPageConfigPrint::WritePrintInfoToFile()
{
	m_list_printInfo.RemoveAll();
	int count = m_trackContrl.m_selTrackerAll.GetCount();
	for(int i=0;i<count;i++)
	{
		WINDOWCTRINFO windowCtrInfo;
		memset(&windowCtrInfo,0,sizeof(windowCtrInfo));
		windowCtrInfo.windowType = m_trackContrl.m_selTrackerAll[i]->GetWindowType();
		if(windowCtrInfo.windowType == enmStatic)
		{
			if(m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->IsForImage())
			{
				windowCtrInfo.IsForImage = TRUE;
			}
			if(m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->GetIsShowTime())
			{
				windowCtrInfo.IsShowTime = TRUE;
				windowCtrInfo.timeFormat = m_trackContrl.m_selTrackerAll[i]->
					m_pTransStatic->GetTimeFormat();
			}
			if(m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->GetStaForPrintChangeObject())
			{
				windowCtrInfo.IsForPrintChangeObjec = TRUE;
				windowCtrInfo.printObject = m_trackContrl.m_selTrackerAll[i]->
					m_pTransStatic->GetStaPrintObject();
			}
			if(m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->GetStaticForPrint())
			{
				windowCtrInfo.IsForPrint = TRUE;
			}
			windowCtrInfo.bkColor = m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->
				GetBkColor();
			windowCtrInfo.textAlign = m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->
				GetStaticTextAlign();
			windowCtrInfo.textColor = m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->
				GetTextColor();
			windowCtrInfo.nAlpha = m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->
				GetAlpha();
			windowCtrInfo.lfFont = m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->
				GetTextFont();
			CString strContent;
			m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->GetWindowText(strContent);
			_tcsncpy_s(windowCtrInfo.ctrTextContent, textNum, 
				strContent,strContent.GetLength());

			CString picPath = m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->GetLoadImagePath();
			wcsncpy_s(windowCtrInfo.staticPicPath, MAX_PATH, picPath,picPath.GetLength());

			CRect rect;
			//m_trackContrl.m_selTrackerAll[i]->m_pTransStatic->GetWindowRect(&rect);
			rect = m_trackContrl.m_selTrackerAll[i]->m_rect;
			windowCtrInfo.rectLeft = rect.left;
			windowCtrInfo.rectRight = rect.right;
			windowCtrInfo.rectTop = rect.top;
			windowCtrInfo.rectBottom = rect.bottom;
		}
		m_list_printInfo.AddTail(windowCtrInfo);
	}

	CFile file;
	CFileException e;
	WINDOWCTRINFO wndctrInfo;
	if(file.Open(m_printInfoPath,CFile::modeWrite | CFile::modeCreate,&e))
	{
		POSITION pos=m_list_printInfo.GetHeadPosition();
		while(pos)
		{
			wndctrInfo=m_list_printInfo.GetNext(pos);
			file.Write(&wndctrInfo,sizeof(WINDOWCTRINFO));
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CPropPageConfigPrint::ReadPrintInfoFromFile()
{
	m_list_printInfo.RemoveAll();
	m_printInfoPath += _T("\\printinfo.dat");
	CFile file;
	if(file.Open(m_printInfoPath,CFile::modeRead))
	{
		ULONGLONG count=file.GetLength()/sizeof(WINDOWCTRINFO);
		for(ULONGLONG i=0;i<count;i++)
		{
			WINDOWCTRINFO windowctrinfo;
			if(file.Read(&windowctrinfo,sizeof(WINDOWCTRINFO))>0)
			{
				m_list_printInfo.AddTail(windowctrinfo);
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void CPropPageConfigPrint::CreateAllPrintGDI()
{
	WINDOWCTRINFO winfo;
	POSITION pos = m_list_printInfo.GetHeadPosition();
	while(pos)
	{
		winfo = m_list_printInfo.GetNext(pos);
		switch(winfo.windowType)
		{
		case enmStatic:
			{
				CRect rect;
				rect.left=winfo.rectLeft;rect.right=winfo.rectRight;
				rect.top=winfo.rectTop;rect.bottom=winfo.rectBottom;
				if(winfo.IsForImage)//静态框for图片
				{
					CControlRect *pPic = new CControlRect(rect);
					pPic->Create(enmStatic,L"",this,SS_CENTERIMAGE,rect,0,0,
						winfo.staticPicPath);
					pPic->m_pTransStatic->SetStaticForPrint(TRUE);
					pPic->m_pTransStatic->SetAlpha(winfo.nAlpha);
					m_trackContrl.Add(pPic);
				}
				else if(winfo.IsShowTime && !winfo.IsForImage)//时间
				{
					CControlRect* pShowTime = new CControlRect(rect);
					pShowTime->Create(enmStatic,winfo.ctrTextContent,this,0,rect,0,0);
					pShowTime->m_pTransStatic->SetShowTime(TRUE);
					pShowTime->m_pTransStatic->SetStaticForPrint(TRUE);
					pShowTime->m_pTransStatic->SetTimeFormat(winfo.timeFormat);
					pShowTime->m_pTransStatic->SetFont(winfo.lfFont);
					pShowTime->m_pTransStatic->SetTextColor(winfo.textColor);
					m_trackContrl.Add(pShowTime);
				}
				else if(winfo.IsForPrintChangeObjec && !winfo.IsForImage)//用于打印的变量
				{
					CControlRect* pPrintObject = new CControlRect(rect);
					pPrintObject->Create(enmStatic,winfo.ctrTextContent,this,0,rect,0,0);
					pPrintObject->m_pTransStatic->SetStaticForPrint(TRUE);
					pPrintObject->m_pTransStatic->SetStaForPrintChangeObject(TRUE);
					pPrintObject->m_pTransStatic->SetStaForPrintObject(winfo.printObject);
					pPrintObject->m_pTransStatic->SetFont(winfo.lfFont);
					pPrintObject->m_pTransStatic->SetTextColor(winfo.textColor);
					m_trackContrl.Add(pPrintObject);
				}
				else if(winfo.IsShowQueNum && !winfo.IsForImage)//显示等候人数
				{
					continue;
				}
				else//文本
				{
					CControlRect *pStatic = new CControlRect(rect);
					pStatic->Create(enmStatic,winfo.ctrTextContent,this,0,rect,0,0);
					pStatic->m_pTransStatic->SetStaticForPrint(TRUE);
					//设置字体颜色和背景颜色
					pStatic->m_pTransStatic->SetTextColor(winfo.textColor);
					pStatic->m_pTransStatic->SetBkColor(winfo.bkColor);
					pStatic->m_pTransStatic->SetFont(winfo.lfFont);
					//设置对齐方式
					pStatic->m_pTransStatic->SetStaticTextAlign(winfo.textAlign);
					//设置透明度
					pStatic->m_pTransStatic->SetAlpha(winfo.nAlpha);
					m_trackContrl.Add(pStatic);
				}
			}
			break;
		}
	}
}

BOOL CPropPageConfigPrint::WritePrintRectInfoToFile()
{
	CRect rect = GetShowRect();
	m_printRectInfo.pt[0].x = rect.left;	m_printRectInfo.pt[0].y = rect.top;
	m_printRectInfo.pt[1].x = rect.right; m_printRectInfo.pt[1].y = rect.bottom;
	CDoFile dofile;
	m_printInfoPath = dofile.GetExeFullFilePath();
	m_printInfoPath += _T("\\PrintInfo\\PrintRect.dat");
	CFile file;
	CFileException e;
	if(file.Open(m_printInfoPath,CFile::modeWrite | CFile::modeCreate,&e))
	{
		//		for(int i=0;i<2;i++)
		//		{
		file.Write(&m_printRectInfo,sizeof(PRINTRECTINFO));
		//		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void CPropPageConfigPrint::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CPropertyPage::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CPropPageConfigPrint::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_UP:
			m_trackContrl.KeyUpMove();
			break;
		case VK_DOWN:
			m_trackContrl.KeyDownMove();
			break;
		case VK_LEFT:
			m_trackContrl.KeyLeftMove();
			break;
		case VK_RIGHT:
			m_trackContrl.KeyRightMove();
			break;
		default:
			break;
		}
	}
	return CPropertyPage::PreTranslateMessage(pMsg);
}


BOOL CPropPageConfigPrint::OnApply()
{
	// TODO: 在此添加专用代码和/或调用基类
	WritePrintInfoToFile();
	WritePrintRectInfoToFile();
	return CPropertyPage::OnApply();
}

void CPropPageConfigPrint::OnBnClickedSerialId()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintSerialID;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
		SetModified();
	}
}

void CPropPageConfigPrint::OnBnClickedConsultName()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintConsultName;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
		SetModified();
	}
}

void CPropPageConfigPrint::OnBnClickedDoctorName()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsAddPicture = FALSE;
	m_bIsAddText = TRUE;
	m_bIsForEdit = FALSE;
	m_bIsShowTime = FALSE;
	m_bIsForPrintObject = TRUE;
	m_printObject = enumPrintDoctorName;
	CPropPrintSet printSet(_T("打印"),this);
	if(IDOK == printSet.DoModal())
	{
		SetModified();
	}
}

void CPropPageConfigPrint::OnBnClickedPrintIscheck()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pButton = (CButton*)GetDlgItem(IDC_PRINT_ISCHECK);
	if(pButton->GetCheck() == BST_CHECKED)
	{
		m_printRectInfo.isUsePrint = TRUE;
	}
	else
	{
		m_printRectInfo.isUsePrint = FALSE;
	}
	SetModified();
}

BOOL CPropPageConfigPrint::LoadPrintRectInfoFormFile()
{
	CString path = m_printInfoPath + _T("\\PrintRect.dat");
	CFile file;
	//	POINT pt[2]={0};
	if(file.Open(path,CFile::modeRead))
	{
		ULONGLONG count=file.GetLength()/sizeof(PRINTRECTINFO);
		for(ULONGLONG i=0;i<count;i++)
		{
			if(file.Read(&m_printRectInfo,sizeof(PRINTRECTINFO))>0)
			{
				continue;
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CPropPageConfigPrint::OnSetActive()
{
	//SetModified();

	return CPropertyPage::OnSetActive();
}
