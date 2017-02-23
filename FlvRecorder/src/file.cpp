#include "stdafx.h"
#include "file.h"
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib") 
#endif // WIN32



#define STR(x) (((std::string)(x)).c_str())
bool fileExists(string path) {
	struct stat fileInfo;

	if (stat(STR(path), &fileInfo) == 0) {
		return true;
	}
	else {
		return false;
	}
}

File::File() {
    _path = "";
    _truncate = false;
    _append = false;
}

File::~File() {
    _file.flush();
    _file.close();
}


void File::flush()
{
	_file.flush();
}

bool File::Initialize(const string& path) {
    return Initialize(path, false, false);
}

bool File::Initialize(const string& path, bool truncate, bool append) {
    _path = path;
    _truncate = truncate;
    _append = append;

    ios_base::openmode openMode = ios_base::in | ios_base::out | ios_base::binary;

    if (fileExists(_path)) {
        //FINEST("File %s exists",STR(path));
        if (_append) {
            //FINEST("File %s will be appended",STR(path));
            openMode |= ios_base::app;
        }
        if (_truncate) {
            //FINEST("File %s will be truncated",STR(path));
            openMode |= ios_base::trunc;
        }
    } else {
        //FINEST("File %s will be truncated anyway...",STR(path));
        openMode |= ios_base::trunc;
    }

    _file.open(STR(_path), openMode);
    if (_file.fail()) {

        return false;
    }

    if (!SeekEnd())
        return false;

    uint64_t tempSize = _file.tellg();
    _size = (uint32_t) tempSize;
    if (tempSize != _size) {
        
        return false;
    }

    if (!SeekBegin())
        return false;

	
    return true;
}

void File::Close() {
    _file.flush();
    _file.close();
}

uint32_t File::Size() {
    return _size;
}

uint32_t File::Cursor() {
    return (uint32_t) _file.tellg();
}

bool File::IsEOF() {
    return _file.eof();
}

string File::GetPath() {
    return _path;
}

bool File::Failed() {
    return _file.fail();
}

bool File::IsOpen() {
    return _file.is_open();
}

bool File::SeekBegin() {
    _file.seekg(0, ios_base::beg);
    if (_file.fail()) {

        return false;
    }
    return true;
}

bool File::SeekEnd() {
    _file.seekg(0, ios_base::end);
    if (_file.fail()) {
        
        return false;
    }
    return true;
}

bool File::SeekAhead(int32_t count) {
    if (count < 0) {
        
        return false;
    }

    if ((int64_t) _size - (int64_t) Cursor() < count) {
        
        return false;
    }

    _file.seekg(count, ios_base::cur);
    if (_file.fail()) {
        
        return false;
    }
    return true;
}

bool File::SeekBehind(int32_t count) {
    if (count < 0) {
        
        return false;
    }

    if ((int64_t) Cursor() < count) {

        return false;
    }

    _file.seekg((-1) * count, ios_base::cur);
    if (_file.fail()) {
        
        return false;
    }
    return true;
}

bool File::SeekTo(int32_t position) {
    if (position < 0) {
        
        return false;
    }

    if ((int64_t) _size < position) {
        
        return false;
    }

    _file.seekg(position, ios_base::beg);
    if (_file.fail()) {
        
        return false;
    }

    return true;
}

bool File::ReadI8(int8_t *pValue) {
    return ReadBuffer((uint8_t *) pValue, 1);
}

bool File::ReadI16(int16_t *pValue, bool networkOrder) {
    if (!ReadBuffer((uint8_t *) pValue, 2))
        return false;
    if (networkOrder)
        *pValue = ntohs(*pValue); //----MARKED-SHORT----
    return true;
}

bool File::ReadI24(int32_t *pValue, bool networkOrder) {
    *pValue = 0;
    if (!ReadBuffer((uint8_t *) pValue, 3))
        return false;
    if (networkOrder)
        *pValue = ntohl(*pValue) >> 8; //----MARKED-LONG---
    else
        *pValue = *pValue << 8;
    return true;
}

bool File::ReadI32(int32_t *pValue, bool networkOrder) {
    if (!ReadBuffer((uint8_t *) pValue, 4))
        return false;
    if (networkOrder)
        *pValue = ntohl(*pValue); //----MARKED-LONG---
    return true;
}

bool File::ReadSI32(int32_t *pValue) {
    if (!ReadI32(pValue, false))
        return false;
	//xueqiang chanage
    *pValue = ntohl(*pValue); //----MARKED-LONG---
    return true;
}

bool File::ReadI64(int64_t *pValue, bool networkOrder) {
    if (!ReadBuffer((uint8_t *) pValue, 8))
        return false;
    if (networkOrder)
        *pValue = ntohll(*pValue); //----MARKED-LONG---
    return true;
}

bool File::ReadUI8(uint8_t *pValue) {
    return ReadI8((int8_t *) pValue);
}

bool File::ReadUI16(uint16_t *pValue, bool networkOrder) {
    return ReadI16((int16_t *) pValue, networkOrder);
}

bool File::ReadUI24(uint32_t *pValue, bool networkOrder) {
    return ReadI24((int32_t *) pValue, networkOrder);
}

bool File::ReadUI32(uint32_t *pValue, bool networkOrder) {
    return ReadI32((int32_t *) pValue, networkOrder);
}

bool File::ReadSUI32(uint32_t *pValue) {
    return ReadSI32((int32_t *) pValue);
}

bool File::ReadUI64(uint64_t *pValue, bool networkOrder) {
    return ReadI64((int64_t *) pValue, networkOrder);
}

bool File::ReadDouble(double *pValue) {
    return ReadBuffer((uint8_t *) pValue, sizeof (double));
}

bool File::ReadBuffer(uint8_t *pBuffer, uint32_t count) {
    _file.read((char *) pBuffer, count);
    if (_file.fail()) {
        
        return false;
    }
    return true;
}

bool File::ReadLine(uint8_t *pBuffer, uint32_t &maxSize) {
    _file.getline((char *) pBuffer, maxSize);
    if (_file.fail()) {
        
        return false;
    }
    return true;
}

bool File::ReadAll(string &str) {
    str = "";
    if (Size() == 0)
        return true;
    if (!SeekBegin()) {
        
        return false;
    }
    uint8_t *pBuffer = new uint8_t[Size()];
    if (!ReadBuffer(pBuffer, Size())) {
        
        delete[] pBuffer;
        return false;
    }
    str = string((char *) pBuffer, Size());
    delete[] pBuffer;
    return true;
}

bool File::PeekI8(int8_t *pValue) {
    if (!ReadI8(pValue))
        return false;
    return SeekBehind(1);
}

bool File::PeekI16(int16_t *pValue, bool networkOrder) {
    if (!ReadI16(pValue, networkOrder))
        return false;
    return SeekBehind(2);
}

bool File::PeekI24(int32_t *pValue, bool networkOrder) {
    if (!ReadI24(pValue, networkOrder))
        return false;
    return SeekBehind(3);
}

bool File::PeekI32(int32_t *pValue, bool networkOrder) {
    if (!ReadI32(pValue, networkOrder))
        return false;
    return SeekBehind(4);
}

bool File::PeekSI32(int32_t *pValue) {
    if (!ReadSI32(pValue))
        return false;
    return SeekBehind(4);
}

bool File::PeekI64(int64_t *pValue, bool networkOrder) {
//    NYIA;
    return false;
}

bool File::PeekUI8(uint8_t *pValue) {
    return PeekI8((int8_t *) pValue);
}

bool File::PeekUI16(uint16_t *pValue, bool networkOrder) {
    return PeekI16((int16_t *) pValue, networkOrder);
}

bool File::PeekUI24(uint32_t *pValue, bool networkOrder) {
    return PeekI24((int32_t *) pValue, networkOrder);
}

bool File::PeekUI32(uint32_t *pValue, bool networkOrder) {
    return PeekI32((int32_t *) pValue, networkOrder);
}

bool File::PeekSUI32(uint32_t *pValue) {
    return PeekSI32((int32_t *) pValue);
}

bool File::PeekUI64(uint64_t *pValue, bool networkOrder) {
    return PeekI64((int64_t *) pValue, networkOrder);
}

bool File::PeekDouble(double *pValue) {
    if (!ReadDouble(pValue))
        return false;
    return SeekBehind(sizeof (double));
}

bool File::PeekBuffer(uint8_t *pBuffer, uint32_t count) {
    if (!ReadBuffer(pBuffer, count))
        return false;
    return SeekBehind(count);
}

bool File::WriteI8(int8_t value) {
    return WriteBuffer((uint8_t *) & value, 1);
}

bool File::WriteI16(int16_t value, bool networkOrder) {
    if (networkOrder)
        value = htons(value); //----MARKED-SHORT----
    return WriteBuffer((uint8_t *) & value, 2);
}

bool File::WriteI24(int32_t value, bool networkOrder) {
    if (networkOrder)
        value = htonl(value << 8); //----MARKED-LONG---
    return WriteBuffer((uint8_t *) & value, 3);
}

bool File::WriteI32(int32_t value, bool networkOrder) {
    if (networkOrder)
        value = htonl(value); //----MARKED-LONG---
    return WriteBuffer((uint8_t *) & value, 4);
}

bool File::WriteSI32(int32_t value) {
    if (!WriteI24(value, true))
        return false;
    if (!WriteI8(value >> 24))
        return false;
    return true;
}

bool File::WriteI64(int64_t value, bool networkOrder) {
//    NYIA;
    return false;
}

bool File::WriteUI8(uint8_t value) {
    return WriteI8((int8_t) value);
}

bool File::WriteUI16(uint16_t value, bool networkOrder) {
    return WriteI16((int16_t) value, networkOrder);
}

bool File::WriteUI24(uint32_t value, bool networkOrder) {
    return WriteI24((int32_t) value, networkOrder);
}

bool File::WriteUI32(uint32_t value, bool networkOrder) {
    return WriteI32((int32_t) value, networkOrder);
}

bool File::WriteSUI32(uint32_t value) {
    return WriteSI32((int32_t) value);
}

bool File::WriteUI64(uint64_t value, bool networkOrder) {
    return WriteI64((int64_t) value, networkOrder);
}

bool File::WriteDouble(double value) {
    return WriteBuffer((uint8_t *) & value, sizeof (double));
}

bool File::WriteString(const string &value) {
    return WriteBuffer((uint8_t *) STR(value), value.length());
}

bool File::WriteBuffer(uint8_t *pBuffer, uint32_t count) {
    _file.write((char *) pBuffer, count);
    if (_file.fail()) {
        
        return false;
    }
	_size += count;
    return true;
}
