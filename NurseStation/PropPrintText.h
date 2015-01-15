#pragma once
#include "afxwin.h"
#include "ColorStatic.h"

// CPropPrintText 对话框

class CPropPrintText : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPrintText)

public:
	CPropPrintText();
	virtual ~CPropPrintText();

// 对话框数据
	enum { IDD = IDD_PROPPRINTTEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ed_prText;
	CEdit m_ed_prTxWid;
	CEdit m_ed_prTxHei;
	CColorStatic m_sta_prTxColor;
	afx_msg void OnBnClickedBnPrtextfont();
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedStaPrtextcolor();
private:
	COLORREF m_textColor;
	CFont    m_textFont; 
	void InitFont();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEdPrtext();
	virtual void OnOK();
};
