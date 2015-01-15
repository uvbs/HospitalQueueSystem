#pragma once
#include "afxext.h"
#include "ControlRect.h"
#include "TrackDef.h"
class CTrackContrl : public CRectTracker
{
public:
	CTrackContrl(void);
	virtual ~CTrackContrl(void);
	//渲染CRectTracker矩形覆盖CRectTracker类Draw
	void Draw(CDC* pDC)const; 
	//添加一个CControlRect控件
	void Add( CControlRect* pObject );
	//改变界面上相互重叠的控件顺序
	void CorrectZOrders( CWnd* pWndInsertAfter );
	//判断控件是否被选中
	BOOL IsSelected( CControlRect* pObject ) const;
	//将控件选中
	BOOL Select( CControlRect* pObject );
	//切换控件选中状态
	BOOL Toggle( CControlRect* pObject );
	//删除被选中的控件
	BOOL DeSelect( CControlRect* pObject );
	//删除所有被选中控件,返回被选中的按钮个数
	int DeSelectAll();
	//覆盖Track
	int Track(const CPoint & point, UINT nFlags = 0,BOOL bTrackRubberBand = TRUE );
	//创建CRectTracker对象
	void Create(CWnd* pWnd);
	//获取选中控件的
	CRect GetBoundsRect() const;//不能修改成员变量的值
	//SetCursor显示鼠标形状
	BOOL SetCursor( UINT nHitTest, UINT message );
	//改变所有控件的使用状态
	void SetAllCtrContralEnable();
	//改变所有控件为编辑状态
	void SetAllCtrContralDisable();
	//设置重画的部分
	void ReDrawRect(CRect &rect,int handleSize);
	//两个响应函数参见CRectTracker
	void OnChangedRect(const CRect& rectOld );
	void AdjustRect(int nHandle,LPRECT lpRect );
	//鼠标右键选中的控件(当前鼠标放在控件中心的控件)
	CControlRect* m_pRightBnSelect;
	
	//删除鼠标右键选中的控件
	BOOL DeleteRightBnSelectCtr();
	//使控件居中
	void DoMiddleCtr();
	//控件左对齐
	void AlignLeftCtr();
	//控件右对齐
	void AlignRightCtr();
	//控件宽度一致
	void SameWidthCtr();
	//控件高度一致
	void SameHeightCtr();
	//控件大小一致
	void SameSizeCtr();
	//顶对齐
	void AlignTopCtr();
	//底对齐
	void AlignBottomCtr();
	//纵向
	void DivideHorCtr();
	//横向
	void DivideVerCtr();
	//当编辑和退出编辑状态时对所有控件进行位置调整
	void AlterAllCtrTop(int nAlterTop);
	//键盘上下左右移动
	void KeyUpMove();
	void KeyDownMove();
	void KeyLeftMove();
	void KeyRightMove();
	//
	void SetForAdjustMaxRect(BOOL isForAdjustMaxRect){m_bIsForMaxRect = isForAdjustMaxRect;}
	//设置最大矩形区域
	void SetMaxRect(const CRect& maxRect){m_maxMoveRect = maxRect;}
	//当大于最大矩形时，重新确定
	void ReAdjustMaxRect(CRect& rect,BOOL isSameSize = FALSE/*大小不变*/);
private:
	CWnd* m_pwndParent;//父窗口
	//释放
	void DeleteAllCtrPtr();
	int FindRate(int coor[],const int size);//找出出现次数最多的坐标
	void AlterRectLeft(const int nAlterLeft );//修改控件位置,左对齐
	void AlterRectRight(const  int nAlterRight );//右对齐
	void AlterRectWidth(const int nWidth );//宽度一致
	void AlterRectHeight(const int nHeight );//高度一致
	void AlterRectSameSize(const int nWidth,const int nHeight );//大小一致
	void AlterRectTop(const int nAlterTop);//顶对齐
	void AlterRectBottom(const int nAlterBottom);//底对齐
	//纵向更改平分
	void AlterDivideTop(const int nAlterDivideTop,int coor[],const int nSize);
	//横向更改平分
	void AlterDivideLeft(const int nAlterDivideWid,int coor[],const int nSize);
	//归并排序，不改变控件的先后顺序
	void Merge(int *SR, int *TR, int i, int m, int n);
	void Msort( int *SR, int *TR1, int s, int t );
	//最大的矩形框,即能拉、移动的最大矩形
	CRect m_maxMoveRect;
	//是否启用最大移动 矩形框
	BOOL m_bIsForMaxRect;
public:
	CArray<CControlRect*,CControlRect*> m_selectRectTracker;//被选中的CRectTracker控件
	CArray<CControlRect*,CControlRect*> m_selTrackerAll;//所有的CRectTracker控件
};
