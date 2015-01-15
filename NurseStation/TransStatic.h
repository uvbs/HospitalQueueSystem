#pragma once
#include <atlimage.h>
#include "TrackDef.h"
#include "ComputeFuncationTime.h"
#include "PrintDef.h"
//#include "TransparentHelper.h"
// CTransStatic


class CTransStatic : public CStatic
{
	DECLARE_DYNAMIC(CTransStatic)

public:
	CTransStatic(CWnd* pParentWnd,CString loadPicPath=_T(""));
	virtual ~CTransStatic();
protected:
	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_bkColor;
	COLORREF m_textColor;
	UINT m_nAlpha;
	CWnd* m_pParentWnd;
	LOGFONT m_lfFont;
	
	void StaticInit();
	void DrawStatic(HDC hDestDC);
	
	CRect m_clientRect;
	StaticTextAlign m_enmTextAlign;
	CImage m_loadImage;//加载的图片
	CString m_loadPicPath;//加载图片路径
	BOOL m_bIsShowTime;//是否显示时间
	BOOL m_bIsShowQueNum;//是否显示排队人数
	BOOL m_bIsForPrint;//是否为打印准备
	StaticTimeFormat m_timeFormat;//时间格式
	CString m_address;//链接处理的队列
	CString m_forShowQueNumHead;//显示排队人数的头字符串
	//如：排队人数：+变量
	
	BOOL m_bIsForPrintChangeObject;//为打印准备的变量如:业务类型等
	EnumPrintObject m_printObject;//打印的变量
protected:
	virtual void PreSubclassWindow();
public:
	//
	void SetForShowQueNumHead(const CString& head){m_forShowQueNumHead = head;}
	CString GetForShowQueNumHead()const {return m_forShowQueNumHead;}
	//连接地址参数
	void SetLinkAddress(const CString& address){m_address = address;}
	CString GetLinkAddress()const {return m_address;}
	//设置显示时间参数
	BOOL GetIsShowTime(){return m_bIsShowTime;}
	BOOL GetIsShowQueNum()const {return m_bIsShowQueNum;}
	void SetShowTime(BOOL isShow){m_bIsShowTime=isShow;}
	void SetShowQueNum(BOOL isShow){m_bIsShowQueNum=isShow;}
	StaticTimeFormat GetTimeFormat()const {return m_timeFormat;}
	void SetTimeFormat(StaticTimeFormat timeFormat){m_timeFormat=timeFormat;}
	//设置文本颜色字体背景颜色透明度以及对齐方式
	void SetBkColor(const COLORREF color);
	void SetTextColor(const COLORREF color);
	void SetAlpha(const UINT alpha);
	void SetFont(const LOGFONT lfFont);
	void SetStaticTextAlign(StaticTextAlign textAlign);

	COLORREF GetBkColor()const {return m_bkColor;}
	COLORREF GetTextColor()const {return m_textColor;}
	LOGFONT GetTextFont()const {return m_lfFont;}
	UINT GetAlpha()const {return m_nAlpha;}
	StaticTextAlign GetStaticTextAlign()const {return m_enmTextAlign;}
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void ShowText(HDC hDC,CRect& rc);
	BOOL IsForImage();
	void SetLoadImagePath(const CString& path){m_loadPicPath = path;}
	CString GetLoadImagePath(){return m_loadPicPath;}
	BOOL LoadPathImage();//加载图片
	void DrawPathImageToDC(HDC hDC);
private:
	CComputeFuncationTime m_Time;
public:
	void SetStaticForPrint(BOOL isForPrint){m_bIsForPrint = isForPrint;}
	BOOL GetStaticForPrint(){return m_bIsForPrint;}
	void SetStaForPrintChangeObject(BOOL isForChangeObject){
		m_bIsForPrintChangeObject = isForChangeObject;
	}
	BOOL GetStaForPrintChangeObject(){return m_bIsForPrintChangeObject;}
	void SetStaForPrintObject(EnumPrintObject printObject){m_printObject = printObject;}
	EnumPrintObject GetStaPrintObject(){return m_printObject;}
};


