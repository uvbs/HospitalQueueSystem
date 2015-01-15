// TransStatic.cpp : 实现文件
//

#include "stdafx.h"
//#include "HallQueFront.h"
#include "TransStatic.h"


extern CDC* g_pBackDC;
// CTransStatic
//extern void MyWriteConsole(CString str);

IMPLEMENT_DYNAMIC(CTransStatic, CStatic)

CTransStatic::CTransStatic(CWnd* pParentWnd,CString loadPicPath)
{
	ASSERT(pParentWnd!=NULL);
	m_pParentWnd=pParentWnd;
	StaticInit();
	m_loadPicPath = loadPicPath;
	LoadPathImage();
}

CTransStatic::~CTransStatic()
{
}


BEGIN_MESSAGE_MAP(CTransStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CTransStatic 消息处理程序

void CTransStatic::StaticInit()
{
//	m_textColor
	m_bkColor=RGB(255,255,255);
	m_textColor=0x000000;
	m_nAlpha=0;
	
	m_enmTextAlign = enumLeft;
	m_bIsShowTime = FALSE;
	m_bIsShowQueNum = FALSE;
	m_bIsForPrint = FALSE;
	m_bIsForPrintChangeObject = FALSE;

	m_lfFont.lfHeight = -MulDiv(15, GetDeviceCaps(m_pParentWnd->GetDC()->GetSafeHdc(), LOGPIXELSY), 72);
	m_lfFont.lfWidth = 0;
	m_lfFont.lfEscapement = 0;
	m_lfFont.lfOrientation = 0;
	m_lfFont.lfWeight = FW_NORMAL;
	m_lfFont.lfItalic = 0;
	m_lfFont.lfUnderline = 0;
	m_lfFont.lfStrikeOut = 0;
	m_lfFont.lfCharSet = ANSI_CHARSET;
	m_lfFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_lfFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lfFont.lfQuality = DEFAULT_QUALITY;
	m_lfFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;

	_tcsncpy_s(m_lfFont.lfFaceName, LF_FACESIZE, _T("宋体"),5);
}
void CTransStatic::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(CS_VREDRAW | CS_HREDRAW,WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	CStatic::PreSubclassWindow();
//	m_objHelper.Install( GetSafeHwnd());
//	SetWindowTransparent(m_hWnd,TRUE,m_nAlpha);
}

void CTransStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	DrawStatic(dc);
}


void CTransStatic::DrawStatic(HDC hDestDC)
{
	GetClientRect(m_clientRect);
	int nWindth=m_clientRect.Width();
	int nHeight=m_clientRect.Height();
	
	HDC hDC=CreateCompatibleDC(hDestDC);//创建兼容DC,采用双缓冲画出
	HDC hMaskDC=CreateCompatibleDC(hDestDC);

	HBITMAP hBitmap=CreateCompatibleBitmap(hDestDC,nWindth,nHeight);
	HBITMAP hMaskBitmap=CreateCompatibleBitmap(hDestDC,nWindth,nHeight);
	
	HBITMAP hOldBitmap=(HBITMAP)SelectObject(hDC,hBitmap);
	HBITMAP hOldMaskBitmap=(HBITMAP)SelectObject(hMaskDC,hMaskBitmap);
	SetBkMode(hDC,TRANSPARENT);
////////////////////////////////////////////////
	
	CPoint pt(0,0);
	MapWindowPoints(m_pParentWnd,&pt,1);
	if(g_pBackDC && !m_bIsForPrint)
	{
		BitBlt(hDC,0,0,nWindth,nHeight,g_pBackDC->GetSafeHdc(),
			pt.x,pt.y,SRCCOPY);
	}
	else
	{
		HBRUSH hbr=CreateSolidBrush(m_bkColor);
		FillRect(hDC,&m_clientRect,hbr);
		DeleteObject(hbr);
	}
//////////////////////////////////////////////////////
	if(!IsForImage())
	{
		HBRUSH hbr=CreateSolidBrush(m_bkColor);
		FillRect(hMaskDC,&m_clientRect,hbr);
		DeleteObject(hbr);
	}
	else
	{
		DrawPathImageToDC(hMaskDC);
	}
	/////////////////////////////////
	BLENDFUNCTION blend;
	memset( &blend, 0, sizeof( blend ) );
	blend.BlendOp= AC_SRC_OVER;
	blend.SourceConstantAlpha= m_nAlpha; // 透明度 最大255
	AlphaBlend (hDC,0,0,nWindth,nHeight,hMaskDC,m_clientRect.left,
		m_clientRect.top,m_clientRect.Width(),m_clientRect.Height(),blend);
	
	///显示文本
	if(!IsForImage())
		ShowText(hDC,m_clientRect);
	BitBlt(hDestDC,0,0,nWindth,nHeight,hDC,0,0,SRCCOPY);
	//删除资源,释放内存-----------------------
	SelectObject(hMaskDC,hOldMaskBitmap);
	DeleteObject(hMaskBitmap);
	DeleteDC(hMaskDC);

	SelectObject(hDC,hOldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hDC);
}

void CTransStatic::DrawPathImageToDC(HDC hDC)
{
	if(!m_loadImage.IsNull())
	{
		SetStretchBltMode(hDC, COLORONCOLOR);//避免颜色失真
		m_loadImage.StretchBlt(hDC,0,0,m_clientRect.Width(),m_clientRect.Height(),
			0,0,m_loadImage.GetWidth(),m_loadImage.GetHeight(),SRCCOPY);
	}
}

void CTransStatic::SetAlpha(const UINT alpha)
{
	m_nAlpha = alpha;
//	Invalidate(TRUE);
}

void CTransStatic::SetBkColor(const COLORREF color)
{
	m_bkColor =  color;
//	Invalidate(TRUE);
}

void CTransStatic::SetTextColor(const COLORREF color)
{
	m_textColor = color;
//	Invalidate(TRUE);
}

void CTransStatic::SetFont(const LOGFONT lfFont)
{
	m_lfFont = lfFont;
//	Invalidate(TRUE);
}
BOOL CTransStatic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	return CStatic::OnEraseBkgnd(pDC);
	return TRUE;
}

void CTransStatic::SetStaticTextAlign(StaticTextAlign textAlign)
{
	m_enmTextAlign = textAlign;
//	Invalidate(TRUE);
}

void CTransStatic::ShowText(HDC hDC,CRect& rc)
{
	CString strText;
	GetWindowText(strText);
	if(strText!=_T(""))
	{
//		rc.InflateRect(-2,-2);//让文字距左右，上下1个像素
		CFont font;
		font.CreateFontIndirect(&m_lfFont);
		HFONT hOldFont=(HFONT)SelectObject(hDC,font);
		::SetTextColor(hDC,m_textColor);
		switch(m_enmTextAlign)
		{
		case enumLeft:
			::DrawText(hDC,strText,-1,&rc,DT_LEFT|DT_WORD_ELLIPSIS|DT_WORDBREAK|DT_EDITCONTROL);
			break;
		case enumCenter:
			::DrawText(hDC,strText,-1,&rc,DT_CENTER|DT_WORD_ELLIPSIS|DT_WORDBREAK|DT_EDITCONTROL);
			break;
		case enumRight:
			::DrawText(hDC,strText,-1,&rc,DT_RIGHT|DT_WORD_ELLIPSIS|DT_WORDBREAK|DT_EDITCONTROL);
			break;
		}
		::SelectObject(hDC,hOldFont);
	}
}

void CTransStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
}

BOOL CTransStatic::IsForImage()
{
	DWORD style = ::GetWindowLong(this->m_hWnd,GWL_STYLE);
	if(style & SS_CENTERIMAGE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CTransStatic::LoadPathImage()
{
	if(!m_loadPicPath.IsEmpty())
	{
		if(!m_loadImage.IsNull())
			m_loadImage.Destroy();
		HRESULT hResult=m_loadImage.Load(m_loadPicPath);
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