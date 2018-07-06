#ifndef SRC_YUV_READER_SEEK_H_
#define SRC_YUV_READER_SEEK_H_

#include <sample_utils.h>
#include <string>

class YUVReaderSeekI420: public CSmplYUVReader
{
public:
    mfxStatus Init(std::string filename, mfxU32 width, mfxU32 height);
    void Seek(mfxU32 frame_number);
protected:
    mfxU32 m_width = 0;
    mfxU32 m_height = 0;
};

#endif /* SRC_YUV_READER_SEEK_H_ */
