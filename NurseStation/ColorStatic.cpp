// ColorStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "ColorStatic.h"


// CColorStatic

IMPLEMENT_DYNAMIC(CColorStatic, CStatic)

CColorStatic::CColorStatic() : m_bkColor(RGB(255,255,255))
{
//	m_bkColor = RGB(255,255,255);
}

CColorStatic::~CColorStatic()
{
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CColorStatic 消息处理程序

void CColorStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	HBRUSH hBrush=CreateSolidBrush(m_bkColor);
	CRect rect;
	GetClientRect(&rect);
	FillRect(dc,&rect,hBrush);
	DeleteObject(hBrush);
}

void CColorStatic::SetBkColor(COLORREF color)
{
	m_bkColor = color;
	Invalidate(TRUE);
}