#pragma once
#include "PrintDef.h"

class CDrawStars
{
public:
	CDrawStars(void);
	~CDrawStars(void);
	///计算五角星的是个点
	void CalculationPoint(const  DWORD lRaudis,const CPoint& startPoint);
	//画五角星
	void Draw(CDC* pDC,BOOL isSoid=FALSE);
private:
	//保存五角星的是个点
	POINT m_ptStar[10];
};
