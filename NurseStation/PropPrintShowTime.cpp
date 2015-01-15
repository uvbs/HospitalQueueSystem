// PropPrintShowTime.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PropPrintShowTime.h"
#include "PropPrintSet.h"


// CPropPrintShowTime 对话框

IMPLEMENT_DYNAMIC(CPropPrintShowTime, CPropertyPage)

CPropPrintShowTime::CPropPrintShowTime()
	: CPropertyPage(CPropPrintShowTime::IDD)
	, m_textColor(RGB(0,0,0))
{
//	m_textColor = RGB(0,0,0);
}

CPropPrintShowTime::~CPropPrintShowTime()
{
}

void CPropPrintShowTime::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDPR_EXAMPLE, m_edpr_example);
	DDX_Control(pDX, IDC_COMPR_TIMESTYLE, m_compr_timeStyle);
	DDX_Control(pDX, IDC_STAPR_COLOR, m_stapr_color);
}


BEGIN_MESSAGE_MAP(CPropPrintShowTime, CPropertyPage)
	ON_BN_CLICKED(IDC_BNPR_FONT, &CPropPrintShowTime::OnBnClickedBnprFont)
	ON_STN_CLICKED(IDC_STAPR_COLOR, &CPropPrintShowTime::OnStnClickedStaColor)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMPR_TIMESTYLE, &CPropPrintShowTime::OnCbnSelchangeComprTimestyle)
END_MESSAGE_MAP()


// CPropPrintShowTime 消息处理程序

void CPropPrintShowTime::OnBnClickedBnprFont()
{
	// TODO: 在此添加控件通知处理程序代码
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	CFont* pFont = m_edpr_example.GetFont();
	if(pFont != NULL)
		pFont->GetLogFont(&lf);


	CFontDialog fdlg(&lf);
	LOGFONT lplf;
	if(IDOK==fdlg.DoModal())
	{
		fdlg.GetCurrentFont(&lplf);
		m_textFont.DeleteObject();
		m_textFont.CreateFontIndirect(&lplf);
		m_edpr_example.SetFont(&m_textFont,TRUE);
	}
}

void CPropPrintShowTime::OnStnClickedStaColor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg;
	if(IDOK == dlg.DoModal())
	{
		m_textColor = dlg.GetColor();
		m_stapr_color.SetBkColor(m_textColor);
	}
}

HBRUSH CPropPrintShowTime::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if(nCtlColor==CTLCOLOR_EDIT &&
		pWnd->GetDlgCtrlID()==IDC_EDPR_EXAMPLE)//注意此处的（pWnd->），否则没效果
	{
		CFont * pfntOld = pDC->SelectObject(&m_textFont);
		pDC->SetTextColor(m_textColor);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CPropPrintShowTime::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CTime currentTime;
	currentTime = CTime::GetCurrentTime();
	CString time;
	time.Format(_T("%d年%d月%d日 %d时%d分%d秒"),
		currentTime.GetYear(),currentTime.GetMonth(),currentTime.GetDay(),
		currentTime.GetHour(),currentTime.GetMinute(),currentTime.GetSecond());
	m_compr_timeStyle.AddString(time);
	time = currentTime.Format(_T("%Y-%m-%d"));
	m_compr_timeStyle.AddString(time);
	time = currentTime.Format(_T("%H:%M:%S"));
	m_compr_timeStyle.AddString(time);
	time = currentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	m_compr_timeStyle.AddString(time);

	InitFont();

	SetTimer(ShowPrintTime,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropPrintShowTime::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CTime currentTime;
	currentTime = CTime::GetCurrentTime();
	CString time;

	switch(nIDEvent)
	{
	case ShowPrintTime:
		{
			int index = m_compr_timeStyle.GetCurSel();
			if(index!=-1)
			{
				switch(index)
				{
				case enumTimeChinese:
					time.Format(_T("%d年%d月%d日 %d时%d分%d秒"),
						currentTime.GetYear(),currentTime.GetMonth(),currentTime.GetDay(),
						currentTime.GetHour(),currentTime.GetMinute(),currentTime.GetSecond());
					break;
				case enumTimeYearMonthDay:
					time = currentTime.Format(_T("%Y-%m-%d"));
					break;
				case enumTimeHourMinSec:
					time = currentTime.Format(_T("%H:%M:%S"));
					break;
				case enumTimeYMDHMS:
					time = currentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
					break;
				}
			}
			m_edpr_example.SetWindowText(time);
		}
		break;
	}
	CPropertyPage::OnTimer(nIDEvent);
}

void CPropPrintShowTime::OnCbnSelchangeComprTimestyle()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CPropPrintShowTime::InitFont()
{
	CPropPrintSet* pPropPrintSet = (CPropPrintSet*)GetParent();
	//CPrintSetDlg* pSetDlg = pPropPrintSet->m_pPrSetDlg;
	CPropPageConfigPrint* pSetDlg = pPropPrintSet->m_pPrSetDlg;
	if(pSetDlg->m_trackContrl.m_pRightBnSelect && pSetDlg->m_bIsForEdit)
	{
		LOGFONT lfFont = pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic
			->GetTextFont();
		m_textFont.DeleteObject();
		m_textFont.CreateFontIndirect(&lfFont);
		m_textColor = pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic
			->GetTextColor();
		m_timeFormat = pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic
			->GetTimeFormat();
		m_compr_timeStyle.SetCurSel(m_timeFormat);
	}
	else
	{
		CClientDC dc(this);
		m_textFont.CreateFont(
			-MulDiv(14, dc.GetDeviceCaps(LOGPIXELSY), 72),                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_BOLD,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("宋体")); 
	}

	m_edpr_example.SetFont(&m_textFont,TRUE);
	m_stapr_color.SetBkColor(m_textColor);
}
void CPropPrintShowTime::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropPrintSet* pPropPrintSet = (CPropPrintSet*)GetParent();
	//CPrintSetDlg* pSetDlg = pPropPrintSet->m_pPrSetDlg;
	CPropPageConfigPrint* pSetDlg = pPropPrintSet->m_pPrSetDlg;

	//获取字体
	LOGFONT lfFont;
	m_textFont.GetLogFont(&lfFont);
	int index = m_compr_timeStyle.GetCurSel();
	if(index==-1)
	{
		return;
	}
	else
	{
		switch(index)
		{
		case 0:
			m_timeFormat = enumTimeChinese;
			break;
		case 1:
			m_timeFormat = enumTimeYearMonthDay;
			break;
		case 2:
			m_timeFormat = enumTimeHourMinSec;
			break;
		case 3:
			m_timeFormat = enumTimeYMDHMS;
			break;
		}
	}
	CString text;
	m_edpr_example.GetWindowText(text);
	CRect rect = pSetDlg->GetShowRect();
	CSize textSize;
	HDC hdc = ::GetDC(m_edpr_example.m_hWnd);
	HFONT hOldFont =(HFONT) ::SelectObject(hdc,m_textFont);
	GetTextExtentPoint32(hdc,text,text.GetLength(),&textSize);
	::SelectObject(hdc,hOldFont);
	::ReleaseDC(m_edpr_example.m_hWnd,hdc);
	////////////////////////////////////////
	if(pSetDlg->m_trackContrl.m_pRightBnSelect && pSetDlg->m_bIsForEdit)
	{
		rect = pSetDlg->m_trackContrl.m_pRightBnSelect->m_rect;
		//获取OldRect
		CRect oldRect;
		pSetDlg->m_trackContrl.m_pRightBnSelect->GetTrueRect(&oldRect);
		/////////////////////////////////////////
		rect.right = rect.left + textSize.cx;
		rect.bottom = rect.top + textSize.cy;
		pSetDlg->m_trackContrl.ReAdjustMaxRect(rect,TRUE);
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_rect = rect;
		pSetDlg->m_trackContrl.m_pRightBnSelect->MoveWindow(&rect,FALSE);
		//设置字体颜色
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			SetTextColor(m_textColor);
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			SetFont(lfFont);
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			SetTimeFormat(m_timeFormat);
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			SetWindowText(text);
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			SetStaticForPrint(TRUE);
		//重绘
		pSetDlg->m_trackContrl.m_pRightBnSelect->GetParent()->
			InvalidateRect(&oldRect);
		CRect newRect;
		pSetDlg->m_trackContrl.m_pRightBnSelect->GetTrueRect(newRect);
		pSetDlg->m_trackContrl.m_pRightBnSelect->GetParent()->
			InvalidateRect(&newRect);
	}
	else
	{
		//创建显示时间
		rect.right=rect.left + textSize.cx;
		rect.bottom=rect.top + textSize.cy;

		CControlRect *pStatic = new CControlRect(rect);
		pStatic->Create(enmStatic,text,pSetDlg,0,rect,0,0);
		
		pStatic->m_pTransStatic->SetShowTime(TRUE);
		//设置字体颜色
		pStatic->m_pTransStatic->SetTextColor(m_textColor);
		pStatic->m_pTransStatic->SetFont(lfFont);
		pStatic->m_pTransStatic->SetTimeFormat(m_timeFormat);
		pStatic->m_pTransStatic->SetStaticForPrint(TRUE);
		pSetDlg->m_trackContrl.Add(pStatic);
	}
	CPropertyPage::OnOK();
}
