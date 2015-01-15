// NurseStationDoc.cpp : CNurseStationDoc 类的实现
//

#include "stdafx.h"
#include "NurseStation.h"

#include "NurseStationDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNurseStationDoc

IMPLEMENT_DYNCREATE(CNurseStationDoc, CDocument)

BEGIN_MESSAGE_MAP(CNurseStationDoc, CDocument)
END_MESSAGE_MAP()


// CNurseStationDoc 构造/析构

CNurseStationDoc::CNurseStationDoc()
{
	// TODO: 在此添加一次性构造代码

}

CNurseStationDoc::~CNurseStationDoc()
{
}

BOOL CNurseStationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CNurseStationDoc 序列化

void CNurseStationDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CNurseStationDoc 诊断

#ifdef _DEBUG
void CNurseStationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNurseStationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNurseStationDoc 命令
