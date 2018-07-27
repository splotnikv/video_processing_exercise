#ifndef SRC_YUV_READER_SEEK_H_
#define SRC_YUV_READER_SEEK_H_

#include <sample_utils.h>
#include <string>
#include <input_params.h>

class YUVReaderSeekI420: public CSmplYUVReader
{
public:
    mfxStatus Init(const StreamInfo& stream_info);
    void Seek(mfxU32 frame_number);
protected:
    mfxU32 m_width = 0;
    mfxU32 m_height = 0;
};

#endif /* SRC_YUV_READER_SEEK_H_ */
