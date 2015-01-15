#pragma once


// CColorStatic
/*
继承自静态控件类专门用来显示颜色
*/
class CColorStatic : public CStatic
{
	DECLARE_DYNAMIC(CColorStatic)

public:
	CColorStatic();
	virtual ~CColorStatic();
	afx_msg void OnPaint();
	void SetBkColor(COLORREF color);
protected:
	DECLARE_MESSAGE_MAP()
	COLORREF m_bkColor;
};


