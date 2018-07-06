/*
 * yuv_reader_seek.cpp
 *
 *  Created on: Jul 5, 2018
 *      Author: root
 */

#include "yuv_reader_seek_i420.h"
#include <list>

mfxStatus YUVReaderSeekI420::Init(std::string filename, mfxU32 width, mfxU32 height)
{
    m_width = width;
    m_height = height;
    std::list<msdk_string> input(1, filename);
    return CSmplYUVReader::Init(input, MFX_FOURCC_I420);
}

// After calling Seek with "frame_number", LoadNextFrame will load
// the frame with an index equal to "frame_number".
void YUVReaderSeekI420::Seek(mfxU32 frame_number)
{
    mfxU32 offset = frame_number * 3 * m_width * m_height / 2; // I420 only
    for (mfxU32 i = 0; i < m_files.size(); i++)
    {
        fseek(m_files[i], offset, SEEK_SET);
    }
    return;
}
