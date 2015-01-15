#pragma once

#include "SLZData.h"
#include "PrintDef.h"
#include "TrackDef.h"
#include <afxmt.h>
//#include <afxwin.h>
#include "atlimage.h"
#include "DrawStars.h"
//打印机驱动winspool
#pragma comment(lib, "winspool.lib")
#include <winspool.h>

class SLZPrinter
{
public:
	SLZPrinter(void);
	~SLZPrinter(void);

	BOOL Print(const SLZData& data,const int waitNum);
	///检测打印机状态
	EnumPrintStaus CheckPrinterStatus();
	BOOL Start();
	///重新加载设置
	BOOL ReFreshPrintInfo();
private:
	BOOL GetJobs(HANDLE hPrinter,
		JOB_INFO_2 **ppJobInfo,
		int *pcJobs,          
		DWORD *pStatus);
	EnumPrintStaus CheckPrintJobStatus(HANDLE hPrinter);
	HANDLE m_hPrinter;
	CList<SLZData> m_list_printData;
	CMutex m_printLock;
private:
	BOOL LoadPrintInofFromFile();
	CList<WINDOWCTRINFO> m_list_setPrintInfo;
	CString m_printInfoPath;
	//打印线程
	HANDLE m_hPrintThread;
	static DWORD WINAPI OnPrint(LPVOID lpParam);
	//打印
	void PrintInfo(SLZData& data);
	//准备打印信息
	void OnBeginPrinting(CDC *pDC, CPrintInfo* pInfo);
	//设置打印DC
	void SetPrintStyle(CDC* pDC,const WINDOWCTRINFO& info,SLZData& data);
	CImage m_picImage;
	BOOL LoadImage(const CString& path);
	void DrawImageToDC(CDC* pDC,const CRect& rect);
private:
	int m_prWaitNum;//打印排队人数
	///根据客户级别画五角星
	void DrawStarsForClientLev(CDC* pDC,const UINT num,const CRect& rect,
		const UINT level);
	////////////////////////
	//打印的区域
	CRect m_printRect;
	BOOL LoadPrintRectFormFile();
public:
	BOOL m_isUsePrint;
};
