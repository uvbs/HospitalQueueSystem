#pragma once
#include "LEDApi.h"
#define WM_LED_NOTIFY WM_USER+1001

class CVisonShowLED
{
public:
	CVisonShowLED();
	CVisonShowLED(HWND hWnd);
	~CVisonShowLED(void);
private:
	//动态链接库初始化
	long (_stdcall *LED_Startup)(void);

	//动态链接库销毁
	long (_stdcall *LED_Cleanup)(void);
	//创建控制卡在线监听服务
	//  serverindex 控制卡在线监听服务编号(可以在多个socket udp端口监听)
	//  localport 本地端口
	long (_stdcall *LED_Report_CreateServer)(long serverindex, long localport);

	//删除控制卡在线监听服务
	//  serverindex 控制卡在线监听服务编号
	void (_stdcall *LED_Report_RemoveServer)(long serverindex);

	//删除全部控制卡在线监听服务
	void (_stdcall *LED_Report_RemoveAllServer)(void);

	//获得控制卡在线列表
	//必须先创建控制卡在线监听服务，即调用LED_Report_CreateServer后使用，否则返回值无效
	//  serverindex 控制卡在线监听服务编号
	//  plist 输出在线列表的用户外部缓冲区，
	//        如果传入空(NULL/0)，则输出到动态链接库内部的缓冲区，继续调用下面的接口取得详细信息
	//  count 最大读取个数
	//--返回值-- 小于0表示失败(未创建该在线监听服务)，大于等于0表示在线的控制卡数量
	long (_stdcall *LED_Report_GetOnlineList)(long serverindex, void* plist, long count);

	//获得某个在线控制卡的上报控制卡名称
	//必须先创建控制卡在线监听服务，即调用LED_Report_CreateServer后使用，否则返回值无效
	//  serverindex 控制卡在线监听服务编号
	//  itemindex 该监听服务的在线列表中，在线控制卡的编号
	//--返回值-- 在线控制卡的上报控制卡名称
	char* (_stdcall *LED_Report_GetOnlineItemName)(long serverindex, long itemindex);

	//获得某个在线控制卡的上报控制卡IP地址
	//必须先创建控制卡在线监听服务，即调用LED_Report_CreateServer后使用，否则返回值无效
	//  serverindex 控制卡在线监听服务编号
	//  itemindex 该监听服务的在线列表中，在线控制卡的编号
	//--返回值-- 在线控制卡的IP地址
	char* (_stdcall *LED_Report_GetOnlineItemHost)(long serverindex, long itemindex);

	//获得某个在线控制卡的上报控制卡远程UDP端口号
	//必须先创建控制卡在线监听服务，即调用LED_Report_CreateServer后使用，否则返回值无效
	//  serverindex 控制卡在线监听服务编号
	//  itemindex 该监听服务的在线列表中，在线控制卡的编号
	//--返回值-- 在线控制卡的远程UDP端口号
	long (_stdcall *LED_Report_GetOnlineItemPort)(long serverindex, long itemindex);

	//获得某个在线控制卡的上报控制卡地址
	//必须先创建控制卡在线监听服务，即调用LED_Report_CreateServer后使用，否则返回值无效
	//  serverindex 控制卡在线监听服务编号
	//  itemindex 该监听服务的在线列表中，在线控制卡的编号
	//--返回值-- 在线控制卡的硬件地址
	long (_stdcall *LED_Report_GetOnlineItemAddr)(long serverindex, long itemindex);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/************************************************************************/
	/* 填写通讯参数接口函数，用于一些无法传递结构体和指针的开发环境来调用   */
	/************************************************************************/

	//填写网络通讯参数，供发送时使用
	//  index 参数数组编号
	//  localport 本地端口
	//  host 控制卡IP地址
	//  remoteport 远程端口
	//  address 控制卡地址
	//  notifymode 通讯同步异步模式
	//  wmhandle 接收消息窗体句柄
	//  wmmessage 接收消息的消息号
	//--返回值-- 小于0表示失败，大于等于0表示参数的id号
	long (_stdcall *LED_UDP_SenderParam)(long index, long localport, char* host, long remoteport, long address, long notifymode, long wmhandle, long wmmessage);

	//填写串口通讯参数，供发送时使用
	//  index 参数数组编号
	//  comport 串口号
	//  baudrate 波特率
	//  address 控制卡地址
	//  notifymode 通讯同步异步模式
	//  wmhandle 接收消息窗体句柄
	//  wmmessage 接收消息的消息号
	//--返回值-- 小于0表示失败，大于等于0表示参数的id号
	long (_stdcall *LED_COM_SenderParam)(long index, long comport, long baudrate, long address, long notifymode, long wmhandle, long wmmessage);

	//填写网络通讯参数，供发送时使用
	//  index 参数数组编号
	//  localport 本地端口
	//  serverindex 在线监听服务编号
	//  name 控制卡名称(控制卡上报来的名称)
	//  notifymode 通讯同步异步模式
	//  wmhandle 接收消息窗体句柄
	//  wmmessage 接收消息的消息号
	//--返回值-- 小于0表示失败，大于等于0表示参数的id号
	long (_stdcall *LED_UDP_SenderParam_ByReportName)(long index, long localport, long serverindex, char* name, long notifymode, long wmhandle, long wmmessage);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//复位控制卡节目播放，重新显示控制卡Flash中存储的节目
	long (_stdcall *LED_ResetDisplay)(PSenderParam param);
	long (_stdcall *LED_ResetDisplay2)(long senderparam_index);

	//校正时间，以当前计算机的系统时间校正控制卡的时钟
	long (_stdcall *LED_AdjustTime)(PSenderParam param);
	long (_stdcall *LED_AdjustTime2)(long senderparam_index);

	//校正时间扩展，以指定的时间校正控制卡的时钟
	long (_stdcall *LED_AdjustTimeEx)(PSenderParam param, LPSYSTEMTIME time);
	long (_stdcall *LED_AdjustTimeEx2)(long senderparam_index, LPSYSTEMTIME time);

	//设置当前显示的节目
	long (_stdcall *LED_SetCurChapter)(PSenderParam param, long value);
	long (_stdcall *LED_SetCurChapter2)(long senderparam_index, long value);

	//设置控制卡电源 value=LED_POWER_ON表示开启电源 value=LED_POWER_OFF表示关闭电源
	long (_stdcall *LED_SetPower)(PSenderParam param, long value);
	long (_stdcall *LED_SetPower2)(long senderparam_index, long value);

	//串口转发数据
	long (_stdcall *LED_ComTransfer)(PSenderParam param, BYTE* buffer, DWORD size);
	long (_stdcall *LED_ComTransfer2)(long senderparam_index, BYTE* buffer, DWORD size);

	//读取控制卡电源状态
	long (_stdcall *LED_GetPower)(PSenderParam param);
	long (_stdcall *LED_GetPower2)(long senderparam_index);

	//设置控制卡亮度 value取值范围0-7
	long (_stdcall *LED_SetBright)(PSenderParam param, long value);
	long (_stdcall *LED_SetBright2)(long senderparam_index, long value);

	//读取控制卡亮度
	long (_stdcall *LED_GetBright)(PSenderParam param);
	long (_stdcall *LED_GetBright2)(long senderparam_index);

	//设置控制卡的定时开关屏计划
	long (_stdcall *LED_SetPowerSchedule)(PSenderParam param, PPowerSchedule value);
	long (_stdcall *LED_SetPowerSchedule2)(long senderparam_index, PPowerSchedule value);

	//读取控制卡的定时开关屏计划
	long (_stdcall *LED_GetPowerSchedule)(PSenderParam param);
	long (_stdcall *LED_GetPowerSchedule2)(long senderparam_index);

	//设置控制卡的定时亮度调节计划
	long (_stdcall *LED_SetBrightSchedule)(PSenderParam param, PBrightSchedule value);
	long (_stdcall *LED_SetBrightSchedule2)(long senderparam_index, PBrightSchedule value);

	//读取控制卡的定时亮度调节计划
	long (_stdcall *LED_GetBrightSchedule)(PSenderParam param);
	long (_stdcall *LED_GetBrightSchedule2)(long senderparam_index);

	//发送节目数据 index为MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject函数的返回值
	long (_stdcall *LED_SendToScreen)(PSenderParam param, long index);
	long (_stdcall *LED_SendToScreen2)(long senderparam_index, long index);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 用于对控制卡进行一些参数设置
	// 流程为：读取控制卡参数--提取各个参数--
	//                       --修改各个参数--将修改后的参数接入控制卡
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//读取控制卡参数
	long (_stdcall *LED_Cache_GetBoardParam)(PSenderParam param);
	long (_stdcall *LED_Cache_GetBoardParam2)(long senderparam_index);
	//提取各个参数
	long (_stdcall *LED_Cache_GetBoardParam_SaveToFile)(char* filename);  //将控制卡回读的参数，写入文件
	char* (_stdcall *LED_Cache_GetBoardParam_IP)(void);
	char* (_stdcall *LED_Cache_GetBoardParam_Mac)(void);
	long (_stdcall *LED_Cache_GetBoardParam_Addr)(void);
	long (_stdcall *LED_Cache_GetBoardParam_Width)(void);
	long (_stdcall *LED_Cache_GetBoardParam_Height)(void);
	long (_stdcall *LED_Cache_GetBoardParam_Brightness)(void);
	//修改各个参数
	long (_stdcall *LED_Cache_SetBoardParam_LoadFromFile)(char* filename);  //从文件读取参数，保存到动态链接库中，以供写入到控制卡
	void (_stdcall *LED_Cache_SetBoardParam_IP)(char* value);
	void (_stdcall *LED_Cache_SetBoardParam_Mac)(char* value);
	void (_stdcall *LED_Cache_SetBoardParam_Addr)(long value);
	void (_stdcall *LED_Cache_SetBoardParam_Width)(long value);
	void (_stdcall *LED_Cache_SetBoardParam_Height)(long value);
	void (_stdcall *LED_Cache_SetBoardParam_Brightness)(long value);
	//将修改后的参数接入控制卡
	long (_stdcall *LED_Cache_SetBoardParam)(PSenderParam param);
	long (_stdcall *LED_Cache_SetBoardParam2)(long senderparam_index);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//节目数据组织形式
	//  ROOT
	//   |
	//   |---Chapter(节目)
	//   |      |
	//   |      |---Region(分区)
	//   |      |     |
	//   |      |     |---Leaf(页面)
	//   |      |     |    |
	//   |      |     |    |---Object(对象[文字、时钟、图片等])
	//   |      |     |    |
	//   |      |     |    |---Object(对象[文字、时钟、图片等])
	//   |      |     |    |
	//   |      |     |    |   ......
	//   |      |     |    |
	//   |      |     |
	//   |      |     |---Leaf(页面)
	//   |      |     |
	//   |      |     |   ......
	//   |      |     |
	//   |      |
	//   |      |---Region(分区)
	//   |      |
	//   |      |   ......
	//   |      |
	//   |---Chapter(节目)
	//   |
	//   |   ......

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//生成播放数据（从VisionShow软件编辑的Vsq文件载入，并生成要下发的节目数据）
	//  RootType 为播放类型；=ROOT_PLAY表示更新控制卡RAM中的节目(掉电丢失)；=ROOT_DOWNLOAD表示更新控制卡Flash中的节目(掉电不丢失)
	//  ColorMode 为颜色模式；取值为COLOR_MODE_MONO或者COLOR
	//  survive 为RAM节目生存时间，在RootType=ROOT_PLAY时有效，当RAM节目播放达到时间后，恢复显示FLASH中的节目
	//  filename 由VisionShow软件编辑的节目文件
	long (_stdcall *MakeFromVsqFile)(char* filename, long RootType, long ColorMode, long survive );

	//生成播放数据
	//  RootType 为播放类型；=ROOT_PLAY表示更新控制卡RAM中的节目(掉电丢失)；=ROOT_DOWNLOAD表示更新控制卡Flash中的节目(掉电不丢失)
	//  ColorMode 为颜色模式；取值为COLOR_MODE_MONO或者COLOR
	//  survive 为RAM节目生存时间，在RootType=ROOT_PLAY时有效，当RAM节目播放达到时间后，恢复显示FLASH中的节目
	long (_stdcall *MakeRoot)(long RootType, long ColorMode, long survive );

	//生成节目数据，后续需要调用[AddRegion]->[AddLeaf]->[AddObject]->[AddWindows/AddDateTime等]
	//  RootType 必须设为ROOT_PLAY_CHAPTER
	//  ActionMode 必须设为0
	//  ChapterIndex 要更新的节目序号
	//  ColorMode 同MakeRoot中的定义
	//  time 播放的时间长度
	//  wait 等待模式，=WAIT_CHILD，表示当达到播放时间长度时，需要等待子节目播放完成再切换；
	//                 =WAIT_USE_TIME，表示当达到播放时间长度时，不等待子节目播放完成，直接切换下一节目
	long (_stdcall *MakeChapter)(long RootType, long ActionMode, long ChapterIndex, long ColorMode, DWORD time, WORD wait);

	//生成分区，后续需要调用[AddLeaf]->[AddObject]->[AddWindows/AddDateTime等]
	//  RootType 必须设为ROOT_PLAY_REGION
	//  ActionMode 必须设为0
	//  ChapterIndex 要更新的节目序号
	//  RegionIndex 要更新的分区序号
	//  ColorMode 同MakeRoot中的定义
	//  left、top、width、height 左、上、宽度、高度
	//  border 流水边框
	long (_stdcall *MakeRegion)(long RootType, long ActionMode, long ChapterIndex, long RegionIndex, long ColorMode, long left, long top, long width, long height, long border);

	//生成页面，后续需要调用[AddObject]->[AddWindows/AddDateTime等]
	//  RootType 必须设为ROOT_PLAY_LEAF
	//  ActionMode 必须设为0
	//  ChapterIndex 要更新的节目序号
	//  RegionIndex 要更新的分区序号
	//  LeafIndex 要更新的页面序号
	//  ColorMode 同MakeRoot中的定义
	//  time 播放的时间长度
	//  wait 等待模式，=WAIT_CHILD，表示当达到播放时间长度时，需要等待子节目播放完成再切换；
	//                 =WAIT_USE_TIME，表示当达到播放时间长度时，不等待子节目播放完成，直接切换下一页面
	long (_stdcall *MakeLeaf)(long RootType, long ActionMode, long ChapterIndex, long RegionIndex, long LeafIndex, long ColorMode, DWORD time, WORD wait);

	//生成播放对象，后续需要调用[AddWindows/AddDateTime等]
	//  RootType 必须设为ROOT_PLAY_OBJECT
	//  ActionMode 必须设为0
	//  ChapterIndex 要更新的节目序号
	//  RegionIndex 要更新的分区序号
	//  LeafIndex 要更新的页面序号
	//  ObjectIndex 要更新的对象序号
	//  ColorMode 同MakeRoot中的定义
	long (_stdcall *MakeObject)(long RootType, long ActionMode, long ChapterIndex, long RegionIndex, long LeafIndex, long ObjectIndex, long ColorMode);

	//添加节目
	//  num 节目数据缓冲区编号，是MakeRoot的返回值
	//  time 播放的时间长度
	//  wait 等待模式，=WAIT_CHILD，表示当达到播放时间长度时，需要等待子节目播放完成再切换；
	//                 =WAIT_USE_TIME，表示当达到播放时间长度时，不等待子节目播放完成，直接切换下一节目
	long (_stdcall *AddChapter)(WORD num, DWORD time, WORD wait); //time为毫秒

	//添加分区
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter的返回值
	//  left、top、width、height 左、上、宽度、高度
	//  border 流水边框
	long (_stdcall *AddRegion)(WORD num, long left, long top, long width, long height, long border);

	//添加页面
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion的返回值
	//  time 播放的时间长度
	//  wait 等待模式，=WAIT_CHILD，表示当达到播放时间长度时，需要等待子节目播放完成再切换；
	//                 =WAIT_USE_TIME，表示当达到播放时间长度时，不等待子节目播放完成，直接切换下一页面
	long (_stdcall *AddLeaf)(WORD num, DWORD time, WORD wait); //time为毫秒

	//添加日期时间显示
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  left、top、width、height 左、上、宽度、高度
	//  transparent 是否透明 =1表示透明；=0表示不透明
	//  border 流水边框(未实现)
	//  fontname 字体名称
	//  fontsize 字体大小
	//  fontcolor 字体颜色
	//  fontstyle 字体样式 举例：=WFS_BOLD表示粗体；=WFS_ITALIC表示斜体；=WFS_BOLD+WFS_ITALIC表示粗斜体
	//  year_offset 年偏移量
	//  month_offset 月偏移量
	//  day_offset 日偏移量
	//  sec_offset 秒偏移量
	//  format 显示格式 
	//      #y表示年 #m表示月 #d表示日 #h表示时 #n表示分 #s表示秒 #w表示星期 #c表示农历
	//      举例： format="#y年#m月#d日 #h时#n分#s秒 星期#w 农历#c"时，显示为"2009年06月27日 12时38分45秒 星期六 农历五月初五"
	long (_stdcall *AddDateTime)(WORD num, long left, long top, long width, long height, long transparent, long border,
		char* fontname, long fontsize, long fontcolor, long fontstyle, 
		long year_offset, long month_offset, long day_offset, long sec_offset, char* format);

	//添加作战时间显示
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  left、top、width、height 左、上、宽度、高度
	//  transparent 是否透明 =1表示透明；=0表示不透明
	//  border 流水边框(未实现)
	//  fontname 字体名称
	//  fontsize 字体大小
	//  fontcolor 字体颜色
	//  fontstyle 字体样式 举例：=WFS_BOLD表示粗体；=WFS_ITALIC表示斜体；=WFS_BOLD+WFS_ITALIC表示粗斜体
	//  format 显示格式 
	//      #y表示年 #m表示月 #d表示日 #h表示时 #n表示分 #s表示秒
	//  basetime 作战时间
	//  fromtime 开始时间
	//  totime 结束时间
	//  step 计时走秒时间步长（多少毫秒走一秒）
	long (_stdcall *AddCampaignEx)(WORD num, long left, long top, long width, long height, long transparent, long border, 
		char* fontname, long fontsize, long fontcolor, long fontstyle, 
		char* format, PTimeStamp basetime, PTimeStamp fromtime, PTimeStamp totime, long step);

	//添加模拟时钟
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  left、top、width、height 左、上、宽度、高度
	//  transparent 是否透明 =1表示透明；=0表示不透明
	//  border 流水边框(未实现)
	//  offset 秒偏移量
	//  bkcolor: 背景颜色
	//  framecolor: 表框颜色
	//  framewidth: 表框颜色
	//  frameshape: 表框形状 =0表示正方形；=1表示圆角方形；=2表示圆形
	//  dotradius: 刻度距离表盘中心半径
	//  adotwidth: 0369点刻度大小
	//  adotcolor: 0369点刻度颜色
	//  bdotwidth: 其他点刻度大小
	//  bdotcolor: 其他点刻度颜色
	//  hourwidth: 时针粗细
	//  hourcolor: 时针颜色
	//  minutewidth: 分针粗细
	//  minutecolor: 分针颜色
	//  secondwidth: 秒针粗细
	//  secondcolor: 秒针颜色
	long (_stdcall *AddClock)(WORD num, long left, long top, long width, long height, long transparent, long border, long offset,
		DWORD bkcolor, DWORD framecolor, DWORD framewidth, long frameshape,
		long dotradius, long adotwidth, DWORD adotcolor, long bdotwidth, DWORD bdotcolor,
		long hourwidth, DWORD hourcolor, long minutewidth, DWORD minutecolor, long secondwidth, DWORD secondcolor);

	//添加动画
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  left、top、width、height 左、上、宽度、高度
	//  transparent 是否透明 =1表示透明；=0表示不透明
	//  border 流水边框(未实现)
	//  filename avi文件名
	//  stretch: 图像是否拉伸以适应对象大小
	long (_stdcall *AddMovie)(WORD num, long left, long top, long width, long height, long transparent, long border, char* filename, long stretch);

	//添加图片组播放
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  left、top、width、height 左、上、宽度、高度
	//  transparent 是否透明 =1表示透明；=0表示不透明
	//  border 流水边框(未实现)
	long (_stdcall *AddWindows)(WORD num, long left, long top, long width, long height, long transparent, long border);

	//添加图片组的子图片 此函数要跟在AddWindows后面调用
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  dc 源图片DC句柄
	//  width 图片宽度
	//  height 图片高度
	//  inmethod 引入方式(下面有列表说明)
	//  inspeed 引入速度(取值范围0-5，从快到慢)
	//  outmethod 引出方式(下面有列表说明)
	//  outspeed 引出速度(取值范围0-5，从快到慢)
	//  stopmethod 停留方式(下面有列表说明)
	//  stopspeed 停留速度(取值范围0-5，从快到慢)
	//  stoptime 停留时间(单位毫秒)
	long (_stdcall *AddChildWindow)(WORD num, HDC dc, long width, long height, long alignment, long inmethod, long inspeed, long outmethod, long outspeed, long stopmethod, long stopspeed, long stoptime); //stoptime单位为秒

	//添加图片组的子图片 此函数要跟在AddWindows后面调用
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  filename 图片文件名
	//  inmethod 引入方式(下面有列表说明)
	//  inspeed 引入速度(取值范围0-5，从快到慢)
	//  outmethod 引出方式(下面有列表说明)
	//  outspeed 引出速度(取值范围0-5，从快到慢)
	//  stopmethod 停留方式(下面有列表说明)
	//  stopspeed 停留速度(取值范围0-5，从快到慢)
	//  stoptime 停留时间(单位毫秒)
	long (_stdcall *AddChildPicture)(WORD num, char* filename, long alignment, long inmethod, long inspeed, long outmethod, long outspeed, long stopmethod, long stopspeed, long stoptime); //stoptime单位为秒

	//添加图片组的子图片 此函数要跟在AddWindows后面调用
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  str 文字字符串
	//  fontname 字体名称
	//  fontsize 字体大小
	//  fontcolor 字体颜色
	//  fontstyle 字体样式 举例：=WFS_BOLD表示粗体；=WFS_ITALIC表示斜体；=WFS_BOLD+WFS_ITALIC表示粗斜体
	//  wordwrap 是否自动换行 =1自动换行；=0不自动换行
	//  inmethod 引入方式(下面有列表说明)
	//  inspeed 引入速度(取值范围0-5，从快到慢)
	//  outmethod 引出方式(下面有列表说明)
	//  outspeed 引出速度(取值范围0-5，从快到慢)
	//  stopmethod 停留方式(下面有列表说明)
	//  stopspeed 停留速度(取值范围0-5，从快到慢)
	//  stoptime 停留时间(单位毫秒)
	long (_stdcall *AddChildText)(WORD num, char* str, char* fontname, long fontsize, long fontcolor, long fontstyle, long wordwrap, long alignment, long inmethod, long inspeed, long outmethod, long outspeed, long stopmethod, long stopspeed, long stoptime); //stoptime单位为毫秒

	//添加内码文字组播放
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  left、top、width、height 左、上、宽度、高度
	//  transparent 是否透明 =1表示透明；=0表示不透明
	//  border 流水边框(未实现)
	long (_stdcall *AddStrings)(WORD num, long left, long top, long width, long height, long transparent, long border);

	//添加图片组的子图片 此函数要跟在AddWindows后面调用
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  str 文字字符串
	//  fontset 字库 =FONTSET_16P表示16点阵字库；=FONTSET_24P表示24点阵字库
	//  color 颜色
	//  inmethod 引入方式(下面有列表说明)
	//  inspeed 引入速度(取值范围0-5，从快到慢)
	//  outmethod 引出方式(下面有列表说明)
	//  outspeed 引出速度(取值范围0-5，从快到慢)
	//  stopmethod 停留方式(下面有列表说明)
	//  stopspeed 停留速度(取值范围0-5，从快到慢)
	//  stoptime 停留时间(单位毫秒)
	long (_stdcall *AddChildString)(WORD num, char* str, long fontset, long color, long inmethod, long inspeed, long outmethod, long outspeed, long stopmethod, long stopspeed, long stoptime); //stoptime单位为毫秒

	//添加图片点阵播放
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  left、top、width、height 左、上、宽度、高度
	//  transparent 是否透明 =1表示透明；=0表示不透明
	//  border 流水边框(未实现)
	//  dc 源图片DC句柄
	//  src_width 图片宽度
	//  src_height 图片高度
	//  inmethod 引入方式(下面有列表说明)
	//  inspeed 引入速度(取值范围0-5，从快到慢)
	//  outmethod 引出方式(下面有列表说明)
	//  outspeed 引出速度(取值范围0-5，从快到慢)
	//  stopmethod 停留方式(下面有列表说明)
	//  stopspeed 停留速度(取值范围0-5，从快到慢)
	//  stoptime 停留时间(单位毫秒)
	long (_stdcall *AddWindow)(WORD num, long left, long top, long width, long height, long transparent, long border,
		HDC dc, long src_width, long src_height, long alignment, long inmethod, long inspeed, long outmethod, long outspeed, long stopmethod, long stopspeed, long stoptime); //stoptime单位为毫秒

	//添加图片文件播放
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  left、top、width、height 左、上、宽度、高度
	//  transparent 是否透明 =1表示透明；=0表示不透明
	//  border 流水边框(未实现)
	//  filename 图片文件
	//  inmethod 引入方式(下面有列表说明)
	//  inspeed 引入速度(取值范围0-5，从快到慢)
	//  outmethod 引出方式(下面有列表说明)
	//  outspeed 引出速度(取值范围0-5，从快到慢)
	//  stopmethod 停留方式(下面有列表说明)
	//  stopspeed 停留速度(取值范围0-5，从快到慢)
	//  stoptime 停留时间(单位毫秒)
	long (_stdcall *AddPicture)(WORD num, long left, long top, long width, long height, long transparent, long border,
		char* filename, long alignment, long inmethod, long inspeed, long outmethod, long outspeed, long stopmethod, long stopspeed, long stoptime); //stoptime单位为毫秒

	//添加文字播放
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  left、top、width、height 左、上、宽度、高度
	//  transparent 是否透明 =1表示透明；=0表示不透明
	//  border 流水边框(未实现)
	//  str 文字字符串
	//  fontname 字体名称
	//  fontsize 字体大小
	//  fontcolor 字体颜色
	//  fontstyle 字体样式 举例：=WFS_BOLD表示粗体；=WFS_ITALIC表示斜体；=WFS_BOLD+WFS_ITALIC表示粗斜体
	//  wordwrap 是否自动换行 =1自动换行；=0不自动换行
	//  inmethod 引入方式(下面有列表说明)
	//  inspeed 引入速度(取值范围0-5，从快到慢)
	//  outmethod 引出方式(下面有列表说明)
	//  outspeed 引出速度(取值范围0-5，从快到慢)
	//  stopmethod 停留方式(下面有列表说明)
	//  stopspeed 停留速度(取值范围0-5，从快到慢)
	//  stoptime 停留时间(单位毫秒)
	long (_stdcall *AddText)(WORD num, long left, long top, long width, long height, long transparent, long border,
		char* str, char* fontname, long fontsize, long fontcolor, 
		long fontstyle, long wordwrap, long alignment, 
		long inmethod, long inspeed, long outmethod, long outspeed,
		long stopmethod, long stopspeed, long stoptime); //stoptime单位为毫秒

	//添加内码文字播放
	//  num 节目数据缓冲区编号，是MakeRoot、MakeChapter、MakeRegion、MakeLeaf、MakeObject的返回值
	//  left、top、width、height 左、上、宽度、高度
	//  transparent 是否透明 =1表示透明；=0表示不透明
	//  border 流水边框(未实现)
	//  str 文字字符串
	//  fontset 字库 =FONTSET_16P表示16点阵字库；=FONTSET_24P表示24点阵字库
	//  color 颜色
	//  inmethod 引入方式(下面有列表说明)
	//  inspeed 引入速度(取值范围0-5，从快到慢)
	//  outmethod 引出方式(下面有列表说明)
	//  outspeed 引出速度(取值范围0-5，从快到慢)
	//  stopmethod 停留方式(下面有列表说明)
	//  stopspeed 停留速度(取值范围0-5，从快到慢)
	//  stoptime 停留时间(单位毫秒)
	long (_stdcall *AddString)(WORD num, long left, long top, long width, long height, long transparent, long border,
		char* str, long fontset, long color, long inmethod, long inspeed, long outmethod, long outspeed, long stopmethod, long stopspeed, long stoptime); //stoptime单位为毫秒

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 深圳人才市场项目专门定制
	//生成文字播放对象
	//  ObjectIndex 需要更新替换掉的对象编号
	//  width 对象宽度
	//  height 对象高度
	//  str 文字字符串
	//  fontname 字体名称
	//  fontsize 字体大小
	//  fontcolor 字体颜色
	//  FontStyle 字体类型
	//  wordwrap 自动换行
	//  alignment 对齐方式
	//  inmethod 引入方式(下面有列表说明)
	//  inspeed 引入速度(取值范围0-5，从快到慢)
	//  outmethod 引出方式(下面有列表说明)
	//  outspeed 引出速度(取值范围0-5，从快到慢)
	//  stopmethod 停留方式(下面有列表说明)
	//  stopspeed 停留速度(取值范围0-5，从快到慢)
	//  stoptime 停留时间(单位毫秒)
	//  istitle 是否标题
	long (_stdcall *SZRC_MakeTextObject)(long ObjectIndex, long width, long height, 
		char* str, char* fontname, long fontsize, long fontcolor, long fontstyle, long wordwrap, long alignment,
		long inmethod, long inspeed, long outmethod, long outspeed, long stopmethod, long stopspeed, long stoptime, long istitle);

	// ====引入动作方式列表(数值从0开始)====
	//    0 = '随机',
	//    1 = '立即显示',
	//    2 = '左滚显示',
	//    3 = '上滚显示',
	//    4 = '右滚显示',
	//    5 = '下滚显示',
	//    6 = '连续左滚显示',
	//    7 = '连续上滚显示',
	//    8 = '中间向上下展开',
	//    9 = '中间向两边展开',
	//   10 = '中间向四周展开',
	//   11 = '从右向左移入',
	//   12 = '从左向右移入',
	//   13 = '从左向右展开',
	//   14 = '从右向左展开',
	//   15 = '从右上角移入',
	//   16 = '从右下角移入',
	//   17 = '从左上角移入',
	//   18 = '从左下角移入',
	//   19 = '从上向下移入',
	//   20 = '从下向上移入',
	//   21 = '横向百叶窗',
	//   22 = '纵向百叶窗',
	// =====================================

	// ====引出动作方式列表(数值从0开始)====
	//    0 = '随机',
	//    1 = '不消失',
	//    2 = '立即消失',
	//    3 = '上下向中间合拢',
	//    4 = '两边向中间合拢',
	//    5 = '四周向中间合拢',
	//    6 = '从左向右移出',
	//    7 = '从右向左移出',
	//    8 = '从右向左合拢',
	//    9 = '从左向右合拢',
	//   10 = '从右上角移出',
	//   11 = '从右下角移出',
	//   12 = '从左上角移出',
	//   13 = '从左下角移出',
	//   14 = '从下向上移出',
	//   15 = '从上向下移出',
	//   16 = '横向百叶窗',
	//   17 = '纵向百叶窗'
	// =====================================

	// ====停留动作方式列表(数值从0开始)====
	//    0 = '静态显示',
	//    1 = '闪烁显示'
	// =====================================
private:
//	long m_lDev;
	HINSTANCE m_hInstance;//动态库实例
	TDeviceParam m_tLedParam;//控制卡参数
	TSenderParam m_tLedSendParam;//发送参数
private:
	///封装函数
	void Destory();
	bool LED_Initialize();
	long CreateData(const CString& msg,const CRect& rect,
		DWORD lShowTime,int nRegion);
	long m_index;
public:
//	long VisonShowOpen(HWND hWnd);
	//接口函数
	void SendDataToScreen(const CString& msg,const CString& wStrIP,const CRect& rect,
		DWORD lShowTime,int nRegion/*区域编号*/);
	void SetHWnd(HWND hWnd);
	//获取控制卡应答结果的数据
	long (_stdcall *LED_GetNotifyParam)(PNotifyParam notify, long index);
};
