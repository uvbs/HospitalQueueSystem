#include "StdAfx.h"
#include "ControlRect.h"


UINT g_WindowID;//控件的ID
//extern void MyWriteConsole(CString str);

CControlRect::CControlRect(void)
{
	Initialize();
}

CControlRect::~CControlRect(void)
{
	if(m_pTransButton)
	{
		delete m_pTransButton;
	}
	if(m_pTransStatic)
	{
		delete m_pTransStatic;
	}
}

CControlRect::CControlRect(CRect& rect)
{
	m_rect.CopyRect(&rect);

	Initialize();
}

CControlRect::CControlRect(int l,int t,int r,int b) :
	CRectTracker( & CRect( l, t, r, b ), CRectTracker::resizeOutside | CRectTracker::solidLine  )
{
	Initialize();
}

BOOL CControlRect::Create(WindowType windtype,LPCTSTR lpszWindowName,CWnd* pParentWnd,
						  DWORD dwStyle/* =WS_DISABLED */,const CRect& rect/* =CRect */,
						  UINT nID,CCreateContext* pContext,CString loadPicPath)
{
	g_WindowID++;
	m_WindowID += g_WindowID;
	m_windType = windtype;
	if(windtype == enmStatic)
	{
		CTransStatic* pTransStatic=new CTransStatic(pParentWnd,loadPicPath);
		BOOL result = pTransStatic->Create(lpszWindowName,dwStyle | WS_DISABLED | WS_CHILD | WS_VISIBLE,
			rect.IsRectEmpty() ? m_rect : rect,pParentWnd,m_WindowID);
		ASSERT(pTransStatic->m_hWnd);
		this->m_hWnd=pTransStatic->m_hWnd;
		m_pTransStatic=pTransStatic;
		return result;
	}
	else if(windtype == enmButton)
	{		
		CTransButton *pAlphaButton=new CTransButton(pParentWnd);
		BOOL result=pAlphaButton->Create(lpszWindowName,dwStyle | WS_DISABLED | WS_CHILD | WS_VISIBLE,
			rect.IsRectEmpty() ? m_rect : rect,pParentWnd,m_WindowID);
		ASSERT(pAlphaButton->m_hWnd);
		this->m_hWnd=pAlphaButton->m_hWnd;
		m_pTransButton=pAlphaButton;
		return result;
	}
	else
	{
		return CWnd::Create(GetWndClassName(windtype),lpszWindowName,dwStyle | WS_DISABLED | WS_CHILD | WS_VISIBLE,
			rect.IsRectEmpty() ? m_rect : rect,pParentWnd,m_WindowID,pContext);
	}
}


LPCTSTR CControlRect::GetWndClassName(WindowType type)
{
	switch(type)
	{
	case enmScrollBar:
		return L"SCROLLBAR";
		break;
	case enmStatic:
		return L"STATIC";
		break;
	case enmButton:
		return L"BUTTON";
		break;
	case enmEdit:
		return L"EDIT";
		break;
	case enmListBox:
		return L"LISTBOX";
		break;
	case enmComboBox:
		return L"COMBOBOX";
		break;
	default:
		return L"";
		break;
	}
}

BOOL CControlRect::Track(CWnd* pWnd,CPoint point,
						 BOOL bAllowInvert /* = FALSE */, 
						 CWnd* pWndClipTo /* = NULL */ )
{
	BOOL flag=CRectTracker::Track(pWnd,point,bAllowInvert,pWndClipTo);
	MoveWindow(m_rect);	
	return flag;
}

void CControlRect::SetRect( const CRect & rect )
{
	m_rect.SetRect(rect.left,rect.top,rect.right,rect.bottom);
	MoveWindow(m_rect);
/*	if(m_pTransButton)
	{
		m_pTransButton->MoveWindow(m_rect);
	}
	*/
}

void CControlRect::SetRect( int l, int t, int r, int b )
{
	m_rect.SetRect( l, t, r, b );
	MoveWindow(m_rect);
}

void CControlRect::Initialize()
{
	m_bSelected = FALSE;
	m_Isable = FALSE;
	m_WindowID = 10000;
	m_nHandleSize = 6;
//	m_buttonFunction = -1;
	m_nStyle = CRectTracker::resizeOutside | CRectTracker::solidLine;
	m_pTransButton = NULL;
	m_pTransStatic = NULL;
}
/*
使所有控件可用
*/
void CControlRect::ModifyWindowAble()
{
	if(m_windType == enmButton)
	{
		//ModifyStyle(WS_DISABLED,BS_PUSHBUTTON);
		m_pTransButton->EnableWindow(TRUE);
	}
	else
		ModifyStyle(WS_DISABLED,WS_VISIBLE);
#ifdef _DEBUG
	CString id;
	id.Format(_T("%d\n"),m_WindowID);
//	MyWriteConsole(id);
#endif
	m_Isable = TRUE;
}
/*
使所有的控件Disable
*/
void CControlRect::ModifyWindowDisable()
{
	ModifyStyle(0,WS_DISABLED);
	m_Isable = FALSE;
}

BOOL CControlRect::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return __super::PreCreateWindow(cs);
}

BEGIN_MESSAGE_MAP(CControlRect, CWnd)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CControlRect::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
//	return __super::OnEraseBkgnd(pDC);
}
/*
控件左右居中
*/
void CControlRect::MiddleCtr()
{
	CRect ParentRect;
	GetParent()->GetClientRect(&ParentRect);
		
	CRect destRect;
	destRect.left = ParentRect.Width()/2 - m_rect.Width()/2;
	destRect.right = ParentRect.Width()/2 + m_rect.Width()/2;
	destRect.top = m_rect.top;
	destRect.bottom = m_rect.bottom;
		
	CRect oldRect; 
	GetTrueRect(oldRect);

	m_rect=destRect;
	MoveWindow(&destRect,FALSE);
		
	CRect newRect;
	GetTrueRect(newRect);
	GetParent()->InvalidateRect(oldRect);
	GetParent()->InvalidateRect(newRect);
}