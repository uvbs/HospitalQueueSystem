#include "StdAfx.h"
#include "DrawStars.h"
#include <math.h>

CDrawStars::CDrawStars(void)
{
	ZeroMemory(m_ptStar,10);
}

CDrawStars::~CDrawStars(void)
{
	
}

void CDrawStars::CalculationPoint(const DWORD lRaudis,
								  const CPoint& startPoint)
{
	
	for(int i=0;i<5;i++)
	{
		m_ptStar[i*2].x = startPoint.x + (DWORD)(lRaudis*cos((18+i*72)*PI/180));
		m_ptStar[i*2].y = startPoint.y - (DWORD)(lRaudis*sin((18+i*72)*PI/180));
	}
	double r = lRaudis*sin(18*PI/180)/sin(54*PI/180);
	for(int i=0;i<5;i++)
	{
		m_ptStar[i*2+1].x = startPoint.x + (DWORD)(r*cos((54+i*72)*PI/180));
		m_ptStar[i*2+1].y = startPoint.y - (DWORD)(r*sin((54+i*72)*PI/180));
	}
}

void CDrawStars::Draw(CDC* pDC,BOOL isSoid/* =FALSE */)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	
	if(isSoid)
	{
		HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(pDC->GetSafeHdc(),hBrush);
		DeleteObject(hBrush);
		pDC->Polygon(m_ptStar,10);
		SelectObject(pDC->GetSafeHdc(),hOldBrush);
	}
	else
	{
		pDC->Polygon(m_ptStar,10);
	}
	pen.DeleteObject();
	pDC->SelectObject(pOldPen);
}