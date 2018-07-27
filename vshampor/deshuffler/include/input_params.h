
#ifndef INPUT_PARAMS_H_
#define INPUT_PARAMS_H_

#include <mfxdefs.h>
#include <string>

struct StreamInfo
{
    std::string filename;
    mfxU32 width;
    mfxU32 height;
    mfxU32 frame_count;
};

class InputParams
{
public:
    InputParams() = default;
    InputParams(int argc, char* argv[]);
    mfxU32 thread_count = 8;

};


#endif /* INPUT_PARAMS_H_ */
