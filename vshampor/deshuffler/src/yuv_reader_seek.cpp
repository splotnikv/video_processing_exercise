/*
 * yuv_reader_seek.cpp
 *
 *  Created on: Jul 5, 2018
 *      Author: root
 */

#include "yuv_reader_seek.h"
#include <list>

YUVReaderSeek::YUVReaderSeek(mfxU32 frame_number)
{
    if (frame_number != 0)
    {

    }
    return;
}


virtual mfxStatus YUVReaderSeek::Init(std::string filename, mfxU32 ColorFormat)
{
    std::list<msdk_string> input(filename);
    return CSmplYUVReader::Init(input, ColorFormat);
}

mfxStatus YUVReaderSeek::Seek(mfxU32 frame_number)
{
    return MFX_ERR_MORE_DATA;
}
