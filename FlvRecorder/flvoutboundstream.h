#ifndef _FLVOUTBOUNDSTREAM_H
#define	_FLVOUTBOUNDSTREAM_H

#include "fileoutboundstream.h"




class FLVOutboundStream:public FileOutboundStream
{
private:
    uint32_t _prevTagSize;
    double _delta;
public:
    FLVOutboundStream();
    virtual ~FLVOutboundStream();
    virtual bool Initialize(const string& fileName, bool append = false);
    bool SaveData(uint8_t *pData, uint32_t length,uint32_t absoluteTime, bool isAudio);
};
#include "FlvRecorder.h"
class FlvRecorder :public IFlvRecorder
{
private:
	FLVOutboundStream *m_FlvStream;
public:
	FlvRecorder()
	{
		m_FlvStream = 0;
	}
	~FlvRecorder()
	{
		delete m_FlvStream;
		m_FlvStream = 0;
	}
	virtual int startRecord(const char* filename)
	{
		if (m_FlvStream)
			stopRecord();

		m_FlvStream = new FLVOutboundStream();
		if (!m_FlvStream->Initialize(filename, false))
		{
			delete m_FlvStream;
			m_FlvStream = NULL;
			return _FAILED;
		}
		return _SUCCESS;
	}
	virtual int stopRecord()
	{
		delete m_FlvStream;
		m_FlvStream = NULL;
		return _SUCCESS;
	}
	virtual int putFrame(MediaFrame* frame)
	{
		if (m_FlvStream)
			m_FlvStream->SaveData((uint8_t*)frame->buf, frame->len, frame->ts, !frame->type);
		return _SUCCESS;
	}
};
#endif	/* _FLVOUTBOUNDSTREAM_H */

