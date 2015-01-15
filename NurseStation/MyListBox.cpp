// MyListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "NurseStation.h"
#include "MyListBox.h"


// CMyListBox

IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox()
{

}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CMyListBox 消息处理程序


extern CMap<int, int, COLORREF, COLORREF> g_colorKey_colorValue;
//extern CMap<int, int, CString, LPCWSTR> g_varKey_varName;

void CMyListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(lpDrawItemStruct->itemID);
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	COLORREF crOldColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();
	//dc.SetTextColor(g_colorKey_colorValue[pCt->nColorKey]);
	dc.SetTextColor(pCt->iColor);
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&  
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.SetBkColor(0x000000);
	}

	dc.DrawText(pCt->strText, pCt->strText.GetLength(), 
		&lpDrawItemStruct->rcItem, DT_WORDBREAK );

	dc.SetTextColor(crOldColor);
	dc.SetBkColor(crOldBkColor);
	dc.Detach();
}

// void CMyListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
// {
// 	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);
// 
// 	CString strText(_T(""));
// 	GetText(lpMeasureItemStruct->itemID, strText);
// 	ASSERT(TRUE != strText.IsEmpty());
// 
// 	CRect rect;
// 	GetItemRect(lpMeasureItemStruct->itemID, &rect);  
// 
// 	CDC* pDC = GetDC();
// 	lpMeasureItemStruct->itemHeight = pDC->DrawText(strText, -1, rect, DT_WORDBREAK | DT_CALCRECT);
// 	ReleaseDC(pDC);
// }

int CMyListBox::AddStringMy(LPCTSTR lpszText, COLORREF nColorKey)
{
	COLORTEXT* pCt = new COLORTEXT;
	pCt->iColor = nColorKey;
	pCt->strText = lpszText;
	int nItem = AddString(pCt->strText);
	SetItemDataPtr(nItem, pCt);
	SetCurSel(nItem);
	return nItem;
}

int CMyListBox::AddStringMy(COLORTEXT* pCt)
{
	ASSERT(pCt != NULL);
	int nItem = AddString(pCt->strText);
	SetItemDataPtr(nItem, pCt);
	SetCurSel(nItem);
	return nItem;
}

int CMyListBox::InsertStringMy(int nItem, LPCTSTR lpszText, COLORREF nColorKey)
{
	COLORTEXT* pCt = new COLORTEXT;
	pCt->iColor = nColorKey;
	pCt->strText = lpszText;
	nItem = InsertString(nItem, pCt->strText);
	SetItemDataPtr(nItem, pCt);
	SetCurSel(nItem);
	return nItem;
}

int CMyListBox::InsertStringMy(int nItem, COLORTEXT* pCt)
{
	ASSERT(pCt != NULL);
	nItem = InsertString(nItem, pCt->strText);
	SetItemDataPtr(nItem, pCt);
	SetCurSel(nItem);
	return nItem;
}

int CMyListBox::DeleteStringMy(int nItem)
{
	if(nItem == -1) return LB_ERR;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(nItem);
	if(pCt)
	{
		delete pCt;
	}
	return DeleteString(nItem);
}

void CMyListBox::OnDestroy()
{
	CListBox::OnDestroy();

	for(int i = 0; i < GetCount(); i++)
	{
		COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(i);
		delete pCt;
	}
}


HBRUSH CMyListBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//pDC->SetBkColor(0x000000);
	//pDC->SetBkMode(TRANSPARENT);

	HBRUSH hbr = CListBox::OnCtlColor(pDC, pWnd, nCtlColor);

	//if(nCtlColor == IDC_LIST_STR_CONF)
	//{
		pDC->SetBkColor(0x000000);
		pDC->SetBkMode(TRANSPARENT);

	//}

	return hbr;
}

BOOL CMyListBox::SetText(int nItem, CString strText)
{
	if(nItem < 0) return FALSE;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(nItem);
	if(!pCt) return FALSE;
	pCt->strText = strText;
	Invalidate(TRUE);
	return TRUE;
}

BOOL CMyListBox::SetColor(int nItem, COLORREF iColor)
{
	if(nItem < 0) return FALSE;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(nItem);
	if(!pCt) return FALSE;
	pCt->iColor = iColor;
	Invalidate(TRUE);
	return TRUE;
}

BOOL CMyListBox::GetTextMy(int nItem, CString& strText)
{
	if(nItem < 0) return FALSE;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(nItem);
	if(!pCt) return FALSE;
	strText = pCt->strText;
	return TRUE;
}

BOOL CMyListBox::GetColor(int nItem, COLORREF& iColor)
{
	if(nItem < 0) return FALSE;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(nItem);
	if(!pCt) return FALSE;
	iColor = pCt->iColor;
	return TRUE;
}

BOOL CMyListBox::GetTextAndColor(int nItem, CString& strText, COLORREF& iColor)
{
	if(nItem < 0) return FALSE;
	COLORTEXT* pCt = (COLORTEXT*)GetItemDataPtr(nItem);
	if(!pCt) return FALSE;
	strText = pCt->strText;
	iColor = pCt->iColor;
	return TRUE;
}

