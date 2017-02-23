#include "stdafx.h"
#include "fileoutboundstream.h"

FileOutboundStream::FileOutboundStream()
{
}

FileOutboundStream::~FileOutboundStream() {
    
}

bool FileOutboundStream::Initialize( const string& fileName,bool append)
{
	if (!_file.Initialize(fileName, !append, append)) {

        return false;
    }

    //3. Are we appending?
    if (append) {
        if (!_file.SeekEnd()) {

            return false;
        }
    }

    return true;
}

