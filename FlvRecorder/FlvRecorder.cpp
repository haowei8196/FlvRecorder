// FlvRecorder.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "flvoutboundstream.h"

FLV_EXPORT IFlvRecorder*  GetRecorder()
{
	return new FlvRecorder();
}
FLV_EXPORT void           DelRecorder(IFlvRecorder* recorder)
{
	delete recorder;
	recorder = 0;
}
