// PropPrintText.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PropPrintText.h"
#include "CommonConvert.h"
#include "PropPrintSet.h"
#include "PrintDef.h"

//extern void MyWriteConsole(CString str);
// CPropPrintText 对话框

IMPLEMENT_DYNAMIC(CPropPrintText, CPropertyPage)

CPropPrintText::CPropPrintText()
	: CPropertyPage(CPropPrintText::IDD)
	, m_textColor(RGB(0,0,0))
{
//	m_textColor = RGB(0,0,0);
}

CPropPrintText::~CPropPrintText()
{
}

void CPropPrintText::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_PRTEXT, m_ed_prText);
	DDX_Control(pDX, IDC_ED_PRWIDTH, m_ed_prTxWid);
	DDX_Control(pDX, IDC_ED_PRHEIGHT, m_ed_prTxHei);
	DDX_Control(pDX, IDC_STA_PRTEXTCOLOR, m_sta_prTxColor);
}


BEGIN_MESSAGE_MAP(CPropPrintText, CPropertyPage)
	ON_BN_CLICKED(IDC_BN_PRTEXTFONT, &CPropPrintText::OnBnClickedBnPrtextfont)
	ON_STN_CLICKED(IDC_STA_PRTEXTCOLOR, &CPropPrintText::OnStnClickedStaPrtextcolor)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_ED_PRTEXT, &CPropPrintText::OnEnChangeEdPrtext)
END_MESSAGE_MAP()


// CPropPrintText 消息处理程序

void CPropPrintText::OnBnClickedBnPrtextfont()
{
	// TODO: 在此添加控件通知处理程序代码
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	CFont* pFont = m_ed_prText.GetFont();
	if(pFont != NULL)
		pFont->GetLogFont(&lf);


	CFontDialog fdlg(&lf);
	//	fdlg.m_cf.rgbColors = m_textColor;//设置CFontDialog默认颜色
	LOGFONT lplf;
	if(IDOK==fdlg.DoModal())
	{
		//		m_textColor = fdlg.GetColor();
		fdlg.GetCurrentFont(&lplf);

		m_textFont.DeleteObject();
		m_textFont.CreateFontIndirect(&lplf);
		m_ed_prText.SetFont(&m_textFont,TRUE);//改变内容编辑框的字体
		OnEnChangeEdPrtext();
	}
}

BOOL CPropPrintText::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ed_prTxHei.SetReadOnly(TRUE);
	m_ed_prTxWid.SetReadOnly(TRUE);

	InitFont();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropPrintText::OnStnClickedStaPrtextcolor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog colorDlg;
	if(IDOK == colorDlg.DoModal())
	{
		m_textColor = colorDlg.GetColor();
		m_sta_prTxColor.SetBkColor(m_textColor);
		Invalidate(TRUE);
	}
}

HBRUSH CPropPrintText::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if(nCtlColor==CTLCOLOR_EDIT &&
		pWnd->GetDlgCtrlID()==IDC_ED_PRTEXT)//注意此处的（pWnd->），否则没效果
	{
		CFont * pfntOld = pDC->SelectObject(&m_textFont);
		pDC->SetTextColor(m_textColor);
	}
	return hbr;
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
}

void CPropPrintText::OnEnChangeEdPrtext()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	
	// TODO:  在此添加控件通知处理程序代码
}

void CPropPrintText::InitFont()
{
	CPropPrintSet* pPropPrintSet = (CPropPrintSet*)GetParent();
	//CPrintSetDlg* pSetDlg = pPropPrintSet->m_pPrSetDlg;
	CPropPageConfigPrint* pSetDlg = pPropPrintSet->m_pPrSetDlg;
	if(pSetDlg->m_bIsForEdit)
	{
		m_textColor = pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic
			->GetTextColor();
		LOGFONT lf = pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic
			->GetTextFont();
		m_textFont.CreateFontIndirect(&lf);
		CString content;
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic
			->GetWindowText(content);
		m_ed_prText.SetWindowText(content);

		CRect rect;
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->GetClientRect(&rect);
		CString c_width,c_height;
		c_width.Format(_T("%d"),rect.Width());
		c_height.Format(_T("%d"),rect.Height());
		m_ed_prTxHei.SetWindowText(c_height);
		m_ed_prTxWid.SetWindowText(c_width);
		if(pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			GetStaForPrintChangeObject())
		{
			m_ed_prText.SetReadOnly(TRUE);
		}
	}
	else
	{
		CClientDC dc(this);
		BOOL flag = m_textFont.CreateFont(
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
		if(pSetDlg->m_bIsForPrintObject)
		{
			m_ed_prText.SetReadOnly(TRUE);
			EnumPrintObject object = pSetDlg->GetPrintObject();
			switch(object)
			{
			case enumPrintClientName:
				m_ed_prText.SetWindowText(CS_printClientName);
				break;
			case enumPrintType: 
				m_ed_prText.SetWindowText(CS_printType);
					break;
			case enumPrintWaitNum:
				m_ed_prText.SetWindowText(CS_printWaitNum);
				break;
			case enumPrintClientNum:
				m_ed_prText.SetWindowText(CS_printClientNum);
					break;
			case enumPrintClientLevel:
				m_ed_prText.SetWindowText(CS_printClientLevel);
				break;
			case enumPrintConsultName:
				m_ed_prText.SetWindowText(CS_printConsultName);
				break;
			case enumPrintDoctorName:
				m_ed_prText.SetWindowText(CS_printDoctorName);
				break;
			case enumPrintSerialID:
				m_ed_prText.SetWindowText(CS_printSerialID);
				break;
			default:
				break;
			}
		}
	}
	m_ed_prText.SetFont(&m_textFont,TRUE);
	m_sta_prTxColor.SetBkColor(m_textColor);
}
void CPropPrintText::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString Content;
	m_ed_prText.GetWindowText(Content);
	CPropPrintSet* pPropPrintSet = (CPropPrintSet*)GetParent();
	//CPrintSetDlg* pSetDlg = pPropPrintSet->m_pPrSetDlg;
	CPropPageConfigPrint* pSetDlg = pPropPrintSet->m_pPrSetDlg;
	//获取文本的尺寸
	int lineCount = m_ed_prText.GetLineCount();
	int maxLeng = 0,maxLine = 0;
	for(int i=0;i<lineCount;i++)
	{
		int len = m_ed_prText.LineLength(m_ed_prText.LineIndex(i));
		if(len > maxLeng)
		{
			maxLeng = len;
			maxLine = i;
		}
	}
	/////获取最大行的内容
	CString strMaxLineText;
	m_ed_prText.GetLine(maxLine,strMaxLineText.GetBuffer(maxLeng),maxLeng);
	strMaxLineText.ReleaseBuffer(maxLeng);
	//获取字的尺寸
	CSize textSize;
	HDC hdc = ::GetDC(m_ed_prText.m_hWnd);
	HFONT hOldFont =(HFONT) ::SelectObject(hdc,m_textFont);
	GetTextExtentPoint32(hdc,strMaxLineText,maxLeng,&textSize);
	::SelectObject(hdc,hOldFont);
	::ReleaseDC(m_ed_prText.m_hWnd,hdc);
	/////////////////////////////////////////////////////
	textSize.cy *= lineCount;
	/////////////////////////////////////////
	int width = textSize.cx;
	int height = textSize.cy;
	////////////////////////////
	
	//字体
	LOGFONT lfFont;
	m_textFont.GetLogFont(&lfFont);
	if(pSetDlg->m_bIsForEdit)
	{
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			SetTextColor(m_textColor);
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			SetFont(lfFont);
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			SetWindowText(Content);
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			SetStaticForPrint(TRUE);
		CRect oldRect;
		pSetDlg->m_trackContrl.m_pRightBnSelect->GetTrueRect(&oldRect);
		CRect newRect;
		newRect.left = oldRect.left;newRect.top = oldRect.top;
		newRect.right = oldRect.left + textSize.cx;
		newRect.bottom = oldRect.top + textSize.cy;
		//大于最大矩形时
		pSetDlg->m_trackContrl.ReAdjustMaxRect(newRect,TRUE);
		pSetDlg->m_trackContrl.m_pRightBnSelect->SetRect(newRect);
		//重画
		pSetDlg->m_trackContrl.m_pRightBnSelect->GetParent()->
			InvalidateRect(&oldRect);
		CRect newTrueRect;
		pSetDlg->m_trackContrl.m_pRightBnSelect->GetTrueRect(&newTrueRect);
		pSetDlg->m_trackContrl.m_pRightBnSelect->GetParent()->
			InvalidateRect(&newTrueRect);
	}
	else
	{
		CRect rect;
		CRect printRect = pSetDlg->GetShowRect();
		rect.left = printRect.left;
		rect.right = printRect.left + width;
		rect.top = printRect.top;
		rect.bottom = printRect.top + height;
		//创建文本
		CControlRect *pStatic = new CControlRect(rect);
		pStatic->Create(enmStatic,Content,pSetDlg,0,rect,0,0);
		//设置字体颜色和背景颜色
		pStatic->m_pTransStatic->SetTextColor(m_textColor);
		pStatic->m_pTransStatic->SetFont(lfFont);
		pStatic->m_pTransStatic->SetStaticForPrint(TRUE);
		if(pSetDlg->m_bIsForPrintObject)
		{
			pStatic->m_pTransStatic->SetStaForPrintChangeObject(TRUE);
			EnumPrintObject object = pSetDlg->GetPrintObject();
			pStatic->m_pTransStatic->SetStaForPrintObject(object);
		}
		pSetDlg->m_trackContrl.Add(pStatic);
	}
	CPropertyPage::OnOK();
}