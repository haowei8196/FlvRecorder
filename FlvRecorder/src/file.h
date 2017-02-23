
#ifndef _FILE_H
#define	_FILE_H

//#include "utils/core.h"
//#include "platform.h"
#include <fstream>
using namespace std;
class  File 
{
private:
    fstream _file;
    uint32_t _size;
    string _path;
    bool _truncate;
    bool _append;
public:
    File();
    virtual ~File();

    //Init
    bool Initialize(const string& path);
    bool Initialize(const string& path, bool truncate, bool append);
	void flush();
    void Close();

    //info
    uint32_t Size();
    uint32_t Cursor();
    bool IsEOF();
    string GetPath();
    bool Failed();
    bool IsOpen();

    //seeking
    bool SeekBegin();
    bool SeekEnd();
    bool SeekAhead(int32_t count);
    bool SeekBehind(int32_t count);
    bool SeekTo(int32_t position);

    //read data
    bool ReadI8(int8_t *pValue);
    bool ReadI16(int16_t *pValue, bool networkOrder = true);
    bool ReadI24(int32_t *pValue, bool networkOrder = true);
    bool ReadI32(int32_t *pValue, bool networkOrder = true);
    bool ReadSI32(int32_t *pValue);
    bool ReadI64(int64_t *pValue, bool networkOrder = true);
    bool ReadUI8(uint8_t *pValue);
    bool ReadUI16(uint16_t *pValue, bool networkOrder = true);
    bool ReadUI24(uint32_t *pValue, bool networkOrder = true);
    bool ReadUI32(uint32_t *pValue, bool networkOrder = true);
    bool ReadSUI32(uint32_t *pValue);
    bool ReadUI64(uint64_t *pValue, bool networkOrder = true);
    bool ReadDouble(double *pValue);
    bool ReadBuffer(uint8_t *pBuffer, uint32_t count);
    bool ReadLine(uint8_t *pBuffer, uint32_t &maxSize);
    bool ReadAll(string &str);

    //peek data
    bool PeekI8(int8_t *pValue);
    bool PeekI16(int16_t *pValue, bool networkOrder = true);
    bool PeekI24(int32_t *pValue, bool networkOrder = true);
    bool PeekI32(int32_t *pValue, bool networkOrder = true);
    bool PeekSI32(int32_t *pValue);
    bool PeekI64(int64_t *pValue, bool networkOrder = true);
    bool PeekUI8(uint8_t *pValue);
    bool PeekUI16(uint16_t *pValue, bool networkOrder = true);
    bool PeekUI24(uint32_t *pValue, bool networkOrder = true);
    bool PeekUI32(uint32_t *pValue, bool networkOrder = true);
    bool PeekSUI32(uint32_t *pValue);
    bool PeekUI64(uint64_t *pValue, bool networkOrder = true);
    bool PeekDouble(double *pValue);
    bool PeekBuffer(uint8_t *pBuffer, uint32_t count);

    //write data
    bool WriteI8(int8_t value);
    bool WriteI16(int16_t value, bool networkOrder = true);
    bool WriteI24(int32_t value, bool networkOrder = true);
    bool WriteI32(int32_t value, bool networkOrder = true);
    bool WriteSI32(int32_t value);
    bool WriteI64(int64_t value, bool networkOrder = true);
    bool WriteUI8(uint8_t value);
    bool WriteUI16(uint16_t value, bool networkOrder = true);
    bool WriteUI24(uint32_t value, bool networkOrder = true);
    bool WriteUI32(uint32_t value, bool networkOrder = true);
    bool WriteSUI32(uint32_t value);
    bool WriteUI64(uint64_t value, bool networkOrder = true);
    bool WriteDouble(double value);
    bool WriteString(const string &value);
    bool WriteBuffer(uint8_t *pBuffer, uint32_t count);
};


#endif	/* _FILE_H */

