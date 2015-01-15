#ifndef  PRINTDEF_H
#define PRINTDEF_H

typedef enum _tagEnumPrintObjectEnum
{
	enumPrintType = 0,//(队列)
	enumPrintWaitNum = 1,//排队人数
	enumPrintClientNum = 2,//客户号码
	enumPrintClientName = 3,//客户姓名
	enumPrintClientLevel  = 4,//客户星级
	enumPrintConsultName = 5,//诊室名称
	enumPrintDoctorName = 6,//医生名字
	enumPrintSerialID = 7 //流水号
}EnumPrintObject;

typedef enum _tagEnumPrintStatus
{
	enumPrintErr = -1,//打印错误
	enumPrintPaperOut = -2,//缺纸
	enumPrintOffline = -3,//掉线
	enumPrintPrinting = 0,//正在打印，可能打印机出现故障
	enumPrintNormal = 1,//打印机正常
	enumPrintNoDefPrinter = -4,//没有默认打印机
	enumPrintFailedJob = -5//获取job失败
}EnumPrintStaus;

const CString CS_printType = _T("科室名称");
const CString CS_printWaitNum = _T("等候人数");
const CString CS_printClientNum = _T("排队号码");
const CString CS_printClientName = _T("病人姓名");
const CString CS_printClientLevel = _T("星级");
const CString CS_printConsultName =_T("诊室名称");
const CString CS_printDoctorName = _T("医生姓名");
const CString CS_printSerialID = _T("流水号");

const double PI = 3.14159265359;

typedef struct _tagPrintRectInfo
{
	POINT pt[2];
	BOOL isUsePrint;
}PRINTRECTINFO,*LPPRINTRECTINFO;
#endif