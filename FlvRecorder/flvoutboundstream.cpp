#include "stdafx.h"
#include "flvoutboundstream.h"


FLVOutboundStream::FLVOutboundStream():FileOutboundStream()
{
    _prevTagSize = 0;
    _delta = -1;
}

FLVOutboundStream::~FLVOutboundStream() {
    if (_prevTagSize != 0) {
        if (!_file.WriteUI32(_prevTagSize)) {
            
        }
    }
	_file.Close();
}

bool FLVOutboundStream::Initialize( const string& fileName, bool append) 
{
    if (!FileOutboundStream::Initialize(fileName, append)) 
	{   
        return false;
    }

    if (!append) {
        //REFERENCE: video_file_format_spec_v10.pdf page 8/48
        const static string flv = "FLV";
        if (!_file.WriteString(flv))
            return false;

        if (!_file.WriteUI8(1))
            return false;

        if (!_file.WriteUI8(5))
            return false;

        if (!_file.WriteUI32(9))
            return false;
    }

    return true;
}

bool FLVOutboundStream::SaveData(uint8_t *pData, uint32_t length,uint32_t absoluteTime, bool isAudio) 
{
    if (_delta < 0)	
        _delta = absoluteTime;		

	//if(isAudio)
	//{
	//	char buf[128] = {0};
	//	sprintf(buf, "record audio ts:%d\n", absoluteTime);
	//	OutputDebugString(buf);

	//	static int counter = 0;
	//	if(counter%100 == 0)
	//	{
	//		char buf[128] = {0};
	//		sprintf(buf, "SaveData %d\n", GetTickCount());
	//		OutputDebugString(buf);
	//	}
	//	counter++;
	//}
	//else
	//{
	//	char buf[128] = {0};
	//	sprintf(buf, "record video ts:%d\n", absoluteTime);
	//	OutputDebugString(buf);
	//}

	/*FLV头文件
	FLV头文件:(9字节)
	1-3： 前3个字节是文件格式标识(FLV 0x46 0x4C 0x56).
	4-4： 第4个字节是版本（0x01）
	5-5： 第5个字节的前5个bit是保留的必须是0.
	第5个字节的第6个bit音频类型标志(TypeFlagsAudio)
	第5个字节的第7个bit也是保留的必须是0
	第5个字节的第8个bit视频类型标志(TypeFlagsVideo)
	6-9: 第6-9的四个字节还是保留的.其数据为 00000009 .
	整个文件头的长度，一般是9（3+1+1+4）

	[编辑]tag 基本格式
	tag类型信息,固定长度为15字节
	1-4: 前一个tag长度(4字节),第一个tag就是0
	5-5: tag类型(1字节);0x8音频；0x9视频；0x12脚本数据
	6-8: tag内容大小(3字节)
	9-11: 时间戳(3字节)(毫秒)(第1个tag的时候总是为0,如果是脚本tag就是0)
	12-12: 时间戳扩展(1字节)让时间戳变成4字节(以存储更长时间的flv时间信息),本字节作为时间戳的最高位.
	在flv回放过程中,播放顺序是按照tag的时间戳顺序播放.任何加入到文件中时间设置数据格式都将被忽略.
	13-15: streamID(3字节) 总是0*/


    if (!_file.WriteUI32(_prevTagSize)) 
        return false;
    

    if (!_file.WriteUI8(isAudio ? 8 : 9)) 	        
        return false;    

    if ( !_file.WriteUI24(length) ) 	        
        return false;    

    if (!_file.WriteSUI32((uint32_t) absoluteTime - (uint32_t) _delta)) {
        
        return false;
    }

    if (!_file.WriteUI24(0)) {
        
        return false;
    }

    if (!_file.WriteBuffer(pData,length)) {
        
        return false;
    }

    _prevTagSize = length + 11;

    return true;
}

















