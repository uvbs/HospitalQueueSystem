#pragma once
#include "atlimage.h"


// CPropPrintPic 对话框

class CPropPrintPic : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPrintPic)

public:
	CPropPrintPic();
	virtual ~CPropPrintPic();

// 对话框数据
	enum { IDD = IDD_PROPPRINTPIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edpr_picHei;
	CEdit m_edpr_picWid;
	CEdit m_edpr_picPath;
	CStatic m_sta_prPic;
	afx_msg void OnBnClickedBnprChoosepic();
private:
	CString m_picType;
	BOOL LoadPicImage(const CString& path);
	CImage m_picImage;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	void InitPrStaPicInfo();
	virtual void OnOK();
};
