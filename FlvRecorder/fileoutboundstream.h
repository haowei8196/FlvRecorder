#ifndef _FILEOUTBOUNDSTREAM_H
#define	_FILEOUTBOUNDSTREAM_H

#include "file.h"


class FileOutboundStream
{
protected:
    File _file;
    bool _append;
public:
    FileOutboundStream();
    virtual ~FileOutboundStream();

    virtual bool Initialize(const string& fileName, bool append);
};


#endif	/* _FILEOUTBOUNDSTREAM_H */

