// NurseStationView.cpp : CNurseStationView 类的实现
//

#include "stdafx.h"
#include "NurseStation.h"

#include "NurseStationDoc.h"
#include "NurseStationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNurseStationView
////////////保存背景内存DC
//CDC*         g_pBackDC = new CDC;
//HBITMAP  g_hBackMemBitmap = NULL;
//HBITMAP   g_hOldBackMemBitmap;
///////////////////////////////////

IMPLEMENT_DYNCREATE(CNurseStationView, CView)

BEGIN_MESSAGE_MAP(CNurseStationView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CNurseStationView 构造/析构

CNurseStationView::CNurseStationView()
{
	// TODO: 在此处添加构造代码

}

CNurseStationView::~CNurseStationView()
{
}

BOOL CNurseStationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CNurseStationView 绘制

void CNurseStationView::OnDraw(CDC* /*pDC*/)
{
	CNurseStationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CNurseStationView 打印

BOOL CNurseStationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CNurseStationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CNurseStationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CNurseStationView 诊断

#ifdef _DEBUG
void CNurseStationView::AssertValid() const
{
	CView::AssertValid();
}

void CNurseStationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNurseStationDoc* CNurseStationView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNurseStationDoc)));
	return (CNurseStationDoc*)m_pDocument;
}
#endif //_DEBUG


// CNurseStationView 消息处理程序
