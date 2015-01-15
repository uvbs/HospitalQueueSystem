#pragma once



// CMyListBox

class CMyListBox : public CListBox
{
	DECLARE_DYNAMIC(CMyListBox)

public:
	CMyListBox();
	virtual ~CMyListBox();

	//enum { IDD = IDC_LIST_STR_CONF };

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDestroy();
public:
	typedef struct _tagColorText{
		COLORREF	iColor;
		CString		strText;
	} COLORTEXT, *LPCOLORTEXT;
public:
	int AddStringMy(LPCTSTR lpszText, COLORREF iColor);
	int AddStringMy(COLORTEXT* pCt);
	int InsertStringMy(int nItem, LPCTSTR lpszText, COLORREF iColor);
	int InsertStringMy(int nItem, COLORTEXT* pCt);
	BOOL SetColor(int nItem, COLORREF iColor);
	BOOL SetText(int nItem, CString strText);
	BOOL GetTextMy(int nItem, CString& strText);
	BOOL GetColor(int nItem, COLORREF& iColor);
	BOOL GetTextAndColor(int nItem, CString& strText, COLORREF& iColor);
	int DeleteStringMy(int nItem);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


