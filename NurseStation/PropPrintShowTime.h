#pragma once
#include "afxwin.h"
#include "ColorStatic.h"
#include "TrackDef.h"
#define ShowPrintTime 9569
// CPropPrintShowTime 对话框

class CPropPrintShowTime : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPrintShowTime)

public:
	CPropPrintShowTime();
	virtual ~CPropPrintShowTime();

// 对话框数据
	enum { IDD = IDD_PROPPRINTSHOWTIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edpr_example;
	CComboBox m_compr_timeStyle;
	CColorStatic m_stapr_color;
	afx_msg void OnBnClickedBnprFont();
private:
	CFont m_textFont;
	COLORREF m_textColor;
	StaticTimeFormat m_timeFormat;

	void InitFont();
public:
	afx_msg void OnStnClickedStaColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComprTimestyle();
	virtual void OnOK();
};
