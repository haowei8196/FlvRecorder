#pragma once

#ifndef FLV_EXPORT
#define FLV_EXPORT __declspec(dllexport)
#endif

#define MEDIAFRAME_TYPE_AUDIO 0
#define MEDIAFRAME_TYPE_VIDEO 1

#define _SUCCESS 0
#define _FAILED  -1

struct MediaFrame
{
	char* buf;
	int   len;
	int   type;
	unsigned int ts;
};
class IFlvRecorder
{
public:
	virtual int startRecord(const char* filename) = 0;
	virtual int stopRecord() = 0;
	virtual int putFrame(MediaFrame* frame)=0;
};

FLV_EXPORT IFlvRecorder*  GetRecorder();
FLV_EXPORT void           DelRecorder(IFlvRecorder* recorder);