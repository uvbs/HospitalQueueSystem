// PropPrintPic.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "PropPrintPic.h"
#include "PropPrintSet.h"
#include "CommonConvert.h"


// CPropPrintPic 对话框

IMPLEMENT_DYNAMIC(CPropPrintPic, CPropertyPage)

CPropPrintPic::CPropPrintPic()
	: CPropertyPage(CPropPrintPic::IDD)
	,m_picType(("files(*.jpg,*.jpeg,*.bmp,*png,*gif)|*.jpg;*.jpeg;*.bmp;*png;*gif||"))
{
	
}

CPropPrintPic::~CPropPrintPic()
{
}

void CPropPrintPic::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDPR_PICHEI, m_edpr_picHei);
	DDX_Control(pDX, IDC_EDPR_PICWID, m_edpr_picWid);
	DDX_Control(pDX, IDC_EDPR_PICPATH, m_edpr_picPath);
	DDX_Control(pDX, IDC_STA_PIC, m_sta_prPic);
}


BEGIN_MESSAGE_MAP(CPropPrintPic, CPropertyPage)
	ON_BN_CLICKED(IDC_BNPR_CHOOSEPIC, &CPropPrintPic::OnBnClickedBnprChoosepic)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPropPrintPic 消息处理程序

void CPropPrintPic::OnBnClickedBnprChoosepic()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog picdlg(TRUE, 0, 0, 4|2, m_picType);
	if(IDOK==picdlg.DoModal())
	{
		CString path=picdlg.GetPathName();
		m_edpr_picPath.SetWindowText(path);
		LoadPicImage(path);
		Invalidate(TRUE);
	}
}

BOOL CPropPrintPic::LoadPicImage(const CString& path)
{
	if(!path.IsEmpty())
	{
		if(!m_picImage.IsNull())
			m_picImage.Destroy();
		HRESULT hResult=m_picImage.Load(path);
		if(FAILED(hResult))
		{
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CPropPrintPic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPropertyPage::OnEraseBkgnd(pDC);
	if(!m_picImage.IsNull())
	{
		CRect rect;
		m_sta_prPic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.top += 7;
		SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);//避免颜色失真
		m_picImage.StretchBlt(pDC->GetSafeHdc(),rect.left,rect.top,rect.Width(),
			rect.Height(),SRCCOPY);
	}
	return TRUE;
}

BOOL CPropPrintPic::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_edpr_picWid.SetReadOnly(TRUE);
	m_edpr_picWid.SetReadOnly(TRUE);
	//初始化
	InitPrStaPicInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropPrintPic::InitPrStaPicInfo()
{
	CPropPrintSet* pPropPrintSet = (CPropPrintSet*)GetParent();
	//CPrintSetDlg* pSetDlg = pPropPrintSet->m_pPrSetDlg;
	CPropPageConfigPrint* pSetDlg = pPropPrintSet->m_pPrSetDlg;
	if(pSetDlg->m_trackContrl.m_pRightBnSelect && pSetDlg->m_bIsForEdit)
	{
		//获取高、宽
		CRect rect;
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			GetClientRect(&rect);
		CString c_width,c_height;
		c_width.Format(_T("%d"),rect.Width());
		c_height.Format(_T("%d"),rect.Height());
		m_edpr_picWid.SetWindowText(c_width);
		m_edpr_picPath.SetWindowText(c_height);

		CString lastPath = pSetDlg->m_trackContrl.m_pRightBnSelect->
			m_pTransStatic->GetLoadImagePath();
		m_edpr_picPath.SetWindowText(lastPath);
		LoadPicImage(lastPath);
	}
	else
	{
		m_edpr_picHei.SetWindowText(_T("80"));
		m_edpr_picWid.SetWindowText(_T("120"));
	}
}

void CPropPrintPic::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropPrintSet* pPropPrintSet = (CPropPrintSet*)GetParent();
	//CPrintSetDlg* pSetDlg = pPropPrintSet->m_pPrSetDlg;
	CPropPageConfigPrint* pSetDlg = pPropPrintSet->m_pPrSetDlg;
	if(pSetDlg->m_trackContrl.m_pRightBnSelect && pSetDlg->m_bIsForEdit)
	{
		CString path;
		m_edpr_picPath.GetWindowText(path);	
		CString lastPath = pSetDlg->m_trackContrl.m_pRightBnSelect->
			m_pTransStatic->GetLoadImagePath();
		if(path != lastPath)
		{
			pSetDlg->m_trackContrl.m_pRightBnSelect->
				m_pTransStatic->SetLoadImagePath(path);
			pSetDlg->m_trackContrl.m_pRightBnSelect->
				m_pTransStatic->LoadPathImage();
		}
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			SetStaticForPrint(TRUE);
		pSetDlg->m_trackContrl.m_pRightBnSelect->m_pTransStatic->
			Invalidate(TRUE);
	}
	else
	{
		int width = 0;
		int height = 0;
		CString c_width,c_heigh;
		CCommonConvert convert;
		m_edpr_picHei.GetWindowText(c_heigh);
		m_edpr_picWid.GetWindowText(c_width);
		convert.CStringToint(width,c_width);
		convert.CStringToint(height,c_heigh);
		CRect rect;
		CRect printRect = pSetDlg->GetShowRect();
		rect.left = printRect.left;
		rect.right = printRect.left + width;
		rect.top = printRect.top;
		rect.bottom = printRect.top + height;

		CControlRect *pPic = new CControlRect(rect);
		CString path; 
		m_edpr_picPath.GetWindowText(path);
		pPic->Create(enmStatic,L"",pSetDlg,SS_CENTERIMAGE,rect,0,0,path);
		pPic->m_pTransStatic->SetAlpha(255);
		pPic->m_pTransStatic->SetStaticForPrint(TRUE);
		pSetDlg->m_trackContrl.Add(pPic);
	}
	CPropertyPage::OnOK();
}