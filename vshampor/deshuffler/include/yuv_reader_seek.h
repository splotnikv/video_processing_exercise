#ifndef SRC_YUV_READER_SEEK_H_
#define SRC_YUV_READER_SEEK_H_

#include <sample_utils.h>
#include <string>

class YUVReaderSeek: public CSmplYUVReader
{
public:
    YUVReaderSeek(mfxU32 frame_number = 0);
    virtual mfxStatus Init(std::string filename, mfxU32 ColorFormat) override;
    mfxStatus Seek(mfxU32 frame_number);
};

#endif /* SRC_YUV_READER_SEEK_H_ */
