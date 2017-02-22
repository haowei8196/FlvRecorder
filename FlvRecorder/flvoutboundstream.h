#ifndef _FLVOUTBOUNDSTREAM_H
#define	_FLVOUTBOUNDSTREAM_H

#include "fileoutboundstream.h"




class FLVOutboundStream:public FileOutboundStream
{
private:
    uint32_t _prevTagSize;
    double _delta;

	//* For RTMP
	// 0 = Linear PCM, platform endian
	// 1 = ADPCM
	// 2 = MP3
	// 7 = G.711 A-law logarithmic PCM
	// 8 = G.711 mu-law logarithmic PCM
	// 10 = AAC
	// 11 = Speex
	char sound_format_;
	// 0 = 5.5 kHz
	// 1 = 11 kHz
	// 2 = 22 kHz
	// 3 = 44 kHz
	char sound_rate_;
	// 0 = 8-bit samples
	// 1 = 16-bit samples
	char sound_size_;
	// 0 = Mono sound
	// 1 = Stereo sound
	char sound_type_;

	int video_width_;
	int video_height_;
	int video_framerate_;
	int video_datarate_;

	int sound_samplerate_;
public:
    FLVOutboundStream();
    virtual ~FLVOutboundStream();
    virtual bool Initialize(const string& fileName, bool append = false);
	void SetAudioParameter(int samplerate/*44100*/, int pcmbitsize/*16*/, int channel/*1*/);
	void SetVideoParameter(int width, int height, int videodatarate, int framerate);
    bool SaveData(uint8_t *pData, uint32_t length,uint32_t absoluteTime, bool isAudio);
private:
	bool SetMetaData();
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
	virtual int setAudioParameter(int samplerate/*44100*/, int pcmbitsize/*16*/, int channel/*1*/)
	{
		if (!m_FlvStream)
			return _FAILED;
		m_FlvStream->SetAudioParameter(samplerate, pcmbitsize, channel);
		return _SUCCESS;
	}
	virtual int setVideoParameter(int width, int height, int videodatarate, int framerate)
	{
		if (!m_FlvStream)
			return _FAILED;
		m_FlvStream->SetVideoParameter(width, height, videodatarate, framerate);
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

