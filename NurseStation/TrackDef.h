#ifndef TRACKDEF_H
#define TRACKDEF_H
#include "PrintDef.h"
/*
结构体功能:
用于处理控件鼠标左键/右键按下
*/
typedef enum _tagPressTypeEnum
{	
	enmLeftPress = 0,
	enmRightPress = 1
}PressTypeEnum;

/*
结构体功能:
用于应对Button的形状
*/
typedef enum _tagButtonTypeEnum
{
	enumRectButton = 0,
	enumRoundRectButton = 1,
	enumEllipseButton = 2
}ButtonTypeEnum;
/*
控件的类型
*/
typedef enum _tagWindowType
{
	enmScrollBar = 1,
	enmStatic = 2,
	enmButton = 3,
	enmEdit = 4,
	enmListBox = 5,
	enmComboBox = 6
}WindowType;
/*
static 控件文字对齐方式
*/
typedef enum _tagStaticTextAlign
{
	enumLeft = 0,
	enumCenter = 1,
	enumRight  = 2
}StaticTextAlign;

#define  textNum 2048 //最大处理文字限制
#define  addNum 256   //连接处理队列的限制

//时间格式
typedef enum _tagStaticTimeFormat
{
	enumTimeChinese = 0,
	enumTimeYearMonthDay = 1,
	enumTimeHourMinSec  = 2,
	enumTimeYMDHMS = 3
}StaticTimeFormat;
/*
用于写入/读取 控件信息的结构体
*/
typedef struct _tagWindowCtrInfo
{
	WindowType windowType;//控件类型
	COLORREF bkColor;//背景颜色
	COLORREF textColor;//文字颜色
	UINT nAlpha;//透明度
	LOGFONT lfFont;//字体
	ButtonTypeEnum ButtonType;//按钮形状
	WCHAR address[addNum];//链接处理的队列
	UINT   rectLeft;//坐标
	UINT   rectRight;
	UINT   rectTop;
	UINT   rectBottom;
	StaticTextAlign textAlign;//文字对齐方式
	BOOL   IsForImage;//静态框是否为图片格式
	BOOL   IsShowTime;//静态框是否为显示时间
	BOOL   IsShowQueNum;//静态框是否为显示排队人数
	BOOL   IsForPrint;//静态框是否为打印准备
	BOOL   IsForPrintChangeObjec;//静态框是否为打印变量
	StaticTimeFormat timeFormat;//显示时间的格式
	EnumPrintObject printObject;//打印变量
	WCHAR  staticPicPath[MAX_PATH];//静态图片加载的路径
	WCHAR  ctrTextContent[textNum];//控件的文字类容
	WCHAR  forShowQueNumHead[addNum];//显示排队人数固定头
	//如：排队人数:123 前面的排队人数是固定头
}WINDOWCTRINFO,*LPWINDOWCTRINFO;	

#define  TRACK_CTR_ABLE					-3//track时控件可用
#define  TRACK_CTR_CTRL					 3//track时按住ctrl
#define  TRACK_CTR_SHIFT					 4//track时按住shift
#define  TRACK_CTR_MIDORHANDLE  5 
//track时按住handle中间或者边
#define  TRACK_CTR_RUBBERBAND   6//
#define  TRACK_CTR_OTHER				2  //其它操作
#define  TRACK_CTR_LBUTTON           7 //左键
#define  TRACK_CTR_RBUTTON          8 //右键
#define  TRACK_CTR_LDOUBLE           9//左键双击

#define  toolLeftText    _T("左对齐")
#define  toolMiddleText  _T("居中")
#define  toolRightText   _T("右对齐")
#define  toolTopText     _T("顶端对齐")
#define  toolBottomText  _T("底端对齐")
#define  toolWidthText   _T("宽度一致")
#define  toolHeightText  _T("高度一致")
#define  toolSameSizeText    _T("两者一致")
#define  toolHorText     _T("横向")
#define  toolVerText     _T("纵向")


#endif