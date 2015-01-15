#pragma once
#include "TransButton.h"
#include "TrackDef.h"
#include "TransStatic.h"
/*
CControlRect继承CWnd和CRectTracker使其即具有CWnd属性又具有
CRectTracker
*/
class CControlRect : public CWnd,public CRectTracker
{
	friend class CTrackContrl;
public:
	CControlRect(void);
	CControlRect(CRect& rect);
	CControlRect(int l,int t,int r,int b);
	virtual ~CControlRect(void);

public:
	//创建可移动的窗口控件
	BOOL Create(WindowType windtype,LPCTSTR lpszWindowName,CWnd* pParentWnd,
		DWORD dwStyle=WS_DISABLED | WS_CLIPCHILDREN,const CRect& rect=CRect(0,0,0,0),UINT nID=0,
		CCreateContext* pContext=NULL,CString loadPicPath=_T(""));

	//获取窗口注册类名
	LPCTSTR GetWndClassName(WindowType type);
	//覆盖CRectTracker类中的Track函数
	BOOL Track(CWnd* pWnd,CPoint point,BOOL bAllowInvert = FALSE,
		CWnd* pWndClipTo = NULL);
	//设置窗口大小及位置
	void SetRect( int x1, int y1, int x2, int y2 );
	void SetRect( const CRect & rect );
	//设置窗口使用状态
	void ModifyWindowAble();
	//设置窗口编辑状态
	void ModifyWindowDisable();
	//获取窗口控件ID
	UINT GetCtrWindowID(){return m_WindowID;}
	//获取窗口是否处于编辑状态/使用状态
	BOOL GetCtrWindowIsable(){return m_Isable;}
	//获取是否被选中
	BOOL GetCtrWindowSelected(){return m_bSelected;}
	//重写的透明按钮
	CTransButton *m_pTransButton;
	//重写的透明static 控件
	CTransStatic *m_pTransStatic;
	WindowType GetWindowType(){return m_windType;}
	//控件居中
	void MiddleCtr();
private:
	//初始化CRectTracker
	void Initialize();
	//是否被选中
	BOOL m_bSelected;
	//窗口控件ID
	UINT m_WindowID;
	//控件是否可用，用于切换使用和编辑状态
	BOOL m_Isable;
	//控件类型
	WindowType m_windType;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
