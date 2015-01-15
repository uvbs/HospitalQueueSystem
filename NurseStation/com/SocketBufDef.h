#ifndef _SOCKETBUFDEF_H
#define _SOCKETBUFDEF_H

#define  MAXRECVBUF 512
typedef enum _tagSendDataType
{
	getNumData = 0,//取号数据
	callNumData = 1,//呼叫数据
	discardNumData = 2,//过号数据
	evateNumData = 3   //评价数据(完成数据)
}SendDataType;
#endif