#pragma once

#include "TrackContrl.h"
#include "PrintDef.h"

// CPropPageConfigPrint 对话框

class CPropPageConfigPrint : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPageConfigPrint)

public:
	CPropPageConfigPrint();
	virtual ~CPropPageConfigPrint();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_CONFIG_PRINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedAddText();
	afx_msg void OnBnClickedAddPic();
	virtual BOOL OnInitDialog();
	CStatic m_sta_printRect;
public:
	CTrackContrl m_trackContrl;
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CRect GetShowRect();
private:
	CRect m_printRect;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPrintEdit();
	afx_msg void OnPrintDelete();
public:
	BOOL m_bIsForEdit;
	BOOL m_bIsAddText;
	BOOL m_bIsAddPicture;
	BOOL m_bIsShowTime;
	BOOL m_bIsForPrintObject;

	afx_msg void OnBnClickedCurTime();
	afx_msg void OnBnClickedNum();
	afx_msg void OnBnClickedWaitNum();
	afx_msg void OnBnClickedCliName();
	afx_msg void OnBnClickedQueue();
	afx_msg void OnBnClickedBnCllev();
private:
	EnumPrintObject m_printObject;
public:
	EnumPrintObject GetPrintObject(){return m_printObject;}
	void SetPrintObject(EnumPrintObject object){m_printObject = object;}
	virtual BOOL OnApply();
private:
	CList<WINDOWCTRINFO,WINDOWCTRINFO&> m_list_printInfo;
	BOOL WritePrintInfoToFile();
	BOOL ReadPrintInfoFromFile();
	CString m_printInfoPath;//写文件的路径
	void CreateAllPrintGDI();//从文件读出信息后，创建所有的printGDI
	BOOL WritePrintRectInfoToFile();
	BOOL LoadPrintRectInfoFormFile();
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//afx_msg void OnBnClickedBnOk();
	//afx_msg void OnBnClickedBnCancel();
	afx_msg void OnBnClickedSerialId();
	afx_msg void OnBnClickedConsultName();
	afx_msg void OnBnClickedDoctorName();
	afx_msg void OnBnClickedPrintIscheck();
private:
	PRINTRECTINFO m_printRectInfo;
public:
	virtual BOOL OnSetActive();
};
