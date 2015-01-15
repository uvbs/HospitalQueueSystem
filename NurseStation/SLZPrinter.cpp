#include "StdAfx.h"
#include "SLZPrinter.h"
#include "DoFile.h"
#include "NurseStation.h"

//extern void MyWriteConsole(CString str);

SLZPrinter::SLZPrinter(void) : m_hPrintThread(NULL)
,m_isUsePrint(TRUE)
{
//	m_hPrintThread = NULL;
	CDoFile dofile;
	m_printInfoPath = dofile.GetExeFullFilePath();
	m_printInfoPath += _T("\\PrintInfo");
	LoadPrintInofFromFile();
	LoadPrintRectFormFile();
}

SLZPrinter::~SLZPrinter(void)
{
	if(m_hPrintThread)
	{
		TerminateThread(m_hPrintThread,0);
		DeleteObject(m_hPrintThread);
		m_hPrintThread = NULL;
	}
}

BOOL SLZPrinter::Print(const SLZData& data,const int waitNum)
{
	if(!m_isUsePrint)
	{
		return FALSE;
	}
	m_printLock.Lock();
	m_list_printData.AddTail(data);
	m_prWaitNum = waitNum;
	m_printLock.Unlock();
#ifdef _DEBUG
	CString str;
	str.Format(_T("%d"),m_list_printData.GetCount());
	MyWriteConsole(str);
#endif
	return TRUE;
}

BOOL SLZPrinter::GetJobs(HANDLE hPrinter, JOB_INFO_2 **ppJobInfo, 
						 int *pcJobs, DWORD *pStatus)
{
	DWORD cByteNeeded = 0,nReturned = 0,cByteUsed = 0;
	JOB_INFO_2          *pJobStorage = NULL;
	PRINTER_INFO_2       *pPrinterInfo = NULL;

	/* Get the buffer size needed. */ 
	if (!GetPrinter(hPrinter, 2, NULL, 0, &cByteNeeded))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			return FALSE;
	}

	pPrinterInfo = (PRINTER_INFO_2 *)malloc(cByteNeeded);
	if (!(pPrinterInfo))
		/* Failure to allocate memory. */ 
		return FALSE;

	/* Get the printer information. */ 
	if (!GetPrinter(hPrinter,
		2,
		(LPBYTE)pPrinterInfo,
		cByteNeeded,
		&cByteUsed))
	{
		/* Failure to access the printer. */ 
		free(pPrinterInfo);
		pPrinterInfo = NULL;
		return FALSE;
	}
	
	cByteNeeded = 0;
	/* Get job storage space. */ 
	::EnumJobs(hPrinter,0,0XFFFFFFFF,2,NULL,0,&cByteNeeded,&nReturned);

	pJobStorage = (JOB_INFO_2 *)malloc(cByteNeeded);
	if (!pJobStorage)
	{
		/* Failure to allocate Job storage space. */ 
		free(pPrinterInfo);
		pPrinterInfo = NULL;
		return FALSE;
	}
	ZeroMemory(pJobStorage, cByteNeeded);

	if(!::EnumJobs(hPrinter,0,0XFFFFFFFF,2,(LPBYTE)pJobStorage,
		cByteNeeded,&cByteNeeded,&nReturned))
	{
		free(pPrinterInfo);
		free(pJobStorage);
		pJobStorage = NULL;
		pPrinterInfo = NULL;
		return FALSE;
	}

	//Return the information.
	*pcJobs = nReturned;
	*pStatus = pPrinterInfo->Status;
	*ppJobInfo = pJobStorage;
	free(pPrinterInfo);
	return TRUE;
}

EnumPrintStaus SLZPrinter::CheckPrintJobStatus(HANDLE hPrinter)
{
	JOB_INFO_2  *pJobs;
	int cJobs,i;
	DWORD       dwPrinterStatus;
	/*
	*  Get the state information for the Printer Queue and
	*  the jobs in the Printer Queue.
	*/ 
	if (!GetJobs(hPrinter, &pJobs, &cJobs, &dwPrinterStatus))
		return enumPrintFailedJob;

	/*
	*  If the Printer reports an error, believe it.
	*/ 
	if (dwPrinterStatus &
		(PRINTER_STATUS_ERROR |
		PRINTER_STATUS_PAPER_JAM |
		PRINTER_STATUS_PAPER_OUT |
		PRINTER_STATUS_PAPER_PROBLEM |
		PRINTER_STATUS_OUTPUT_BIN_FULL |
		PRINTER_STATUS_NOT_AVAILABLE |
		PRINTER_STATUS_NO_TONER |
		PRINTER_STATUS_OUT_OF_MEMORY |
		PRINTER_STATUS_OFFLINE |
		PRINTER_STATUS_DOOR_OPEN))
	{
		free( pJobs );
		return enumPrintErr;
	}

	/*
	*  Find the Job in the Queue that is printing.
	*/ 
	for (i=0; i < cJobs; i++)
	{
		if (pJobs[i].Status & JOB_STATUS_PRINTING)
		{
			/*
			*  If the job is in an error state,
			*  report an error for the printer.
			*  Code could be inserted here to
			*  attempt an interpretation of the
			*  pStatus member as well.
			*/ 	
			if(pJobs[i].Status & JOB_STATUS_PAPEROUT)
			{
				free( pJobs );
				return enumPrintPaperOut;
			}
			if (pJobs[i].Status & JOB_STATUS_OFFLINE)
			{
				free( pJobs );
				return enumPrintOffline;
			}

			if(pJobs[i].Status & JOB_STATUS_ERROR)
			{
				free( pJobs );
				return enumPrintErr;
			}

			if(pJobs[i].Status == JOB_STATUS_PRINTING)
			{
				free( pJobs );
				return enumPrintPrinting;
			}
		}
	}
	/*
	*  No error condition.
	*/ 
	free( pJobs );
	return enumPrintNormal;
}

EnumPrintStaus SLZPrinter::CheckPrinterStatus()
{
	CPrintDialog printDlg(FALSE);
	if(!printDlg.GetDefaults())
	{
		return enumPrintNoDefPrinter;//没有默认打印机
	}
	EnumPrintStaus status;
	if(::OpenPrinter(printDlg.GetDeviceName().GetBuffer(0),&m_hPrinter,NULL))
	{
		status = CheckPrintJobStatus(m_hPrinter);
		ClosePrinter(m_hPrinter);
	}
	return status;
}

DWORD WINAPI SLZPrinter::OnPrint(LPVOID lpParam)
{
	SLZPrinter* pThis = (SLZPrinter*)lpParam;
	while(TRUE)
	{
		if(pThis->m_list_printData.IsEmpty())
		{
			Sleep(200);
		}
		else
		{
			pThis->CheckPrinterStatus();
			pThis->m_printLock.Lock();
			SLZData data = pThis->m_list_printData.GetHead();
			pThis->m_list_printData.RemoveHead();
			pThis->m_printLock.Unlock();
			pThis->PrintInfo(data);
		}
	}
	return 0;
}

BOOL SLZPrinter::LoadPrintInofFromFile()
{
	m_list_setPrintInfo.RemoveAll();
	CString path = m_printInfoPath + _T("\\printinfo.dat");
	CFile file;
	if(file.Open(path,CFile::modeRead))
	{
		ULONGLONG count=file.GetLength()/sizeof(WINDOWCTRINFO);
		for(ULONGLONG i=0;i<count;i++)
		{
			WINDOWCTRINFO windowctrinfo;
			if(file.Read(&windowctrinfo,sizeof(WINDOWCTRINFO))>0)
			{
				m_list_setPrintInfo.AddTail(windowctrinfo);
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL SLZPrinter::LoadPrintRectFormFile()
{
	CString path = m_printInfoPath + _T("\\PrintRect.dat");
	CFile file;
//	POINT pt[2]={0};
	PRINTRECTINFO printRectInfo;
	if(file.Open(path,CFile::modeRead))
	{
		ULONGLONG count=file.GetLength()/sizeof(PRINTRECTINFO);
		for(ULONGLONG i=0;i<count;i++)
		{
			if(file.Read(&printRectInfo,sizeof(PRINTRECTINFO))>0)
			{
				continue;
			}
		}
		m_printRect.left = printRectInfo.pt[0].x;
		m_printRect.top = printRectInfo.pt[0].y;
		m_printRect.right = printRectInfo.pt[1].x;
		m_printRect.bottom = printRectInfo.pt[1].y;
		m_isUsePrint = printRectInfo.isUsePrint;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL SLZPrinter::Start()
{
	m_hPrintThread = CreateThread(NULL,0,OnPrint,this,0,NULL);
	if(m_hPrintThread != NULL)
		return TRUE;
	else
		return FALSE;
}
void SLZPrinter::PrintInfo(SLZData& data)
{
	CDC dc;
	CPrintDialog printDlg(FALSE);
	BOOL res = printDlg.GetDefaults();
	if( !res ) return;
	dc.Attach(printDlg.GetPrinterDC());     // Get and attach a printer DC
	CString strTitle;                       // Get the application title
	strTitle.LoadString(AFX_IDS_APP_TITLE);

	DOCINFO di;  
	di.cbSize = sizeof(DOCINFO); 
	di.lpszDocName = _T("排队机打印"); 
	di.lpszOutput = NULL; 
	di.lpszDatatype = NULL; 
	di.fwType = 0; 
	int  bPrintingOK=dc.StartDoc(&di); 
	
	// Get the printing extents and store in the m_rectDraw field of a 
	// CPrintInfo object

	CPrintInfo Info;
	Info.SetMaxPage( 1 );
	Info.SetMinPage( 0 );

	int width = dc.GetDeviceCaps(HORZRES);
	int height = dc.GetDeviceCaps(VERTRES);
//	CString str;
//	str.Format(_T("%d,%d"),width,height);
//	AfxMessageBox(str);
	Info.m_rectDraw.SetRect(0,0, 
		dc.GetDeviceCaps(HORZRES), 
		dc.GetDeviceCaps(VERTRES));	
	OnBeginPrinting(&dc, &Info);            // Call your "Init printing" function			

	dc.StartPage();// begin new page
	Info.m_nCurPage = 0;	

	//打印
	/***************************************/
	if(m_list_setPrintInfo.GetCount()==0)
	{
		return;
	}
	POSITION pos=m_list_setPrintInfo.GetHeadPosition();
	while(pos)
	{
		WINDOWCTRINFO info=m_list_setPrintInfo.GetNext(pos);
		SetPrintStyle(&dc,info,data);
	}
	/*******************************************/
	dc.EndPage();
	if (bPrintingOK)	
	{
		dc.EndDoc();                        // end a print job  //xj 执行打印
	}
	else
	{
		dc.AbortDoc();   
	}
	dc.DeleteDC();
}

void SLZPrinter::OnBeginPrinting(CDC *pDC, CPrintInfo* pInfo)
{
	const CSize size(380, 600);
	int	nPageWidth	 = size.cx;
	int nPageHeight = size.cy;

	// 象素（单位：个）

	int x_pixels = pDC->GetDeviceCaps( HORZRES );
	int y_pixels = pDC->GetDeviceCaps( VERTRES ); //(x_pixels*2) / 3 ;

	// 物理（单位：毫米）
	//int	x_mm = pDC->GetDeviceCaps( HORZSIZE );
	//int y_mm = pDC->GetDeviceCaps( VERTSIZE );

	// 	x_pixels	512
	//  y_pixels	2126
	pDC->SetMapMode( MM_ISOTROPIC );
	CSize oldWinExt = pDC->SetWindowExt( 1000, 1000 );
	CSize oldViewprtExt = pDC->SetViewportExt( 
		1000 * x_pixels/nPageWidth, 
		1000 * y_pixels/nPageHeight );
}

void SLZPrinter::SetPrintStyle(CDC* pDC,const WINDOWCTRINFO& info,SLZData& data)
{
	if(!info.IsForPrint)return;
	CFont	font;//字体
	CRect   rect;//文字打印rect
	CPoint  pt;//变量打印坐标
	CString content;//打印内容
	font.CreateFontIndirect(&info.lfFont);
	CFont* pOldFont = pDC->SelectObject( &font );
	//修正打印区域
	rect.left = info.rectLeft - m_printRect.left;
	rect.top = info.rectTop - m_printRect.top;
	rect.right = info.rectRight -m_printRect.left;
	rect.bottom = info.rectBottom - m_printRect.top;
	if(info.IsForPrintChangeObjec)
	{
		pt.x = rect.left;pt.y = rect.top;
		switch(info.printObject)
		{
		case enumPrintType://业务类型(队列)
			content = data.GetOfficeName();
			break;
		case enumPrintWaitNum://排队人数
			content.Format(_T("%d"),m_prWaitNum);
			break;
		case enumPrintClientNum://客户号码
			content = data.GetQueNum();
			break;
		case 	enumPrintClientName:
			content = data.GetPatientName();
			break;//客户姓名
		case 	enumPrintClientLevel: //客户星级
			DrawStarsForClientLev(pDC,7,rect,3);
			break;
		case enumPrintDoctorName://医生姓名
			content = data.GetDoctorName();
			break;
		case enumPrintConsultName://诊室名称
			content = data.GetConsultName();
			break;
		case enumPrintSerialID://流水号
			content = data.GetSerialId();
			break;
		}
		if(!content.IsEmpty())
			pDC->TextOut(pt.x,pt.y,content,content.GetLength());
	}
	else if(info.IsForImage)
	{
		LoadImage(info.staticPicPath);
		DrawImageToDC(pDC,rect);
		return;
	}
	else if(info.IsShowTime)
	{
		CTime currentTime;
		currentTime = data.GetTakingNumTime();
		switch(info.timeFormat)
		{
		case enumTimeChinese:
			content.Format(_T("%d年%d月%d日 %d时%d分%d秒"),
				currentTime.GetYear(),currentTime.GetMonth(),currentTime.GetDay(),
				currentTime.GetHour(),currentTime.GetMinute(),currentTime.GetSecond());
			break;
		case enumTimeYearMonthDay:
			content = currentTime.Format(_T("%Y-%m-%d"));
			break;
		case enumTimeHourMinSec:
			content = currentTime.Format(_T("%H:%M:%S"));
			break;
		case enumTimeYMDHMS:
			content = currentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			break;
		}
		::DrawText(pDC->GetSafeHdc(),content,-1,&rect,
			DT_LEFT|DT_WORD_ELLIPSIS|DT_WORDBREAK|DT_EDITCONTROL);
	}
	else
	{
		content = info.ctrTextContent;
		::DrawText(pDC->GetSafeHdc(),content,-1,&rect,
			DT_LEFT|DT_WORD_ELLIPSIS|DT_WORDBREAK|DT_EDITCONTROL);
	}
	pDC->SelectObject( pOldFont );
	font.DeleteObject();
}

BOOL SLZPrinter::LoadImage(const CString& path)
{
	if(!path.IsEmpty())
	{
		if(!m_picImage.IsNull())
			m_picImage.Destroy();
		HRESULT hResult=m_picImage.Load(path);
		if(FAILED(hResult))
		{
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void SLZPrinter::DrawImageToDC(CDC* pDC,const CRect& rect)
{
	if(!m_picImage.IsNull())
	{
		SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);//避免颜色失真
		m_picImage.StretchBlt(pDC->GetSafeHdc(),rect.left,rect.top,rect.Width(),
			rect.Height(),SRCCOPY);
	}
}

void SLZPrinter::DrawStarsForClientLev(CDC* pDC,const UINT num,
									   const CRect& rect,const UINT level)
{
	int len = ( m_printRect.Width() - rect.Height()*num ) / 2;
	CPoint startPoint;
	startPoint.x = rect.left + len;
	startPoint.y = rect.top;
	int raudis = rect.Height()/2;
	UINT i=0;
	for(i=0;i<num && i<level;i++)
	{
		startPoint.x = rect.left + len + raudis + 2*i*raudis;
		CDrawStars stars;
		stars.CalculationPoint(raudis,startPoint);
		stars.Draw(pDC,TRUE);
	}
	for(i;i<num;i++)
	{
		startPoint.x = rect.left + len + raudis + 2*i*raudis;
		CDrawStars stars;
		stars.CalculationPoint(raudis,startPoint);
		stars.Draw(pDC);
	}
	//画线
	CPen pen;
	pen.CreatePen(PS_DASH, 1, RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	CPoint lineStart,lineEnd;
	lineStart.x = rect.left;lineStart.y = rect.top;
	lineEnd.x = rect.left + len;lineEnd.y = rect.top;
	pDC->MoveTo(lineStart);
	pDC->LineTo(lineEnd);
	lineStart.x = rect.left+len+num*raudis*2;
	lineStart.y = rect.top;
	lineEnd.x = m_printRect.right;
	lineEnd.y = rect.top;
	pDC->MoveTo(lineStart);
	pDC->LineTo(lineEnd);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

BOOL SLZPrinter::ReFreshPrintInfo()
{
	LoadPrintInofFromFile();
	LoadPrintRectFormFile();
	return TRUE;
}