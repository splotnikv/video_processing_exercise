#ifndef DESHUFFLER_H_
#define DESHUFFLER_H_

#include "permutation_data.h"
#include "input_params.h"
#include "yuv_reader_seek.h"
#include <sample_utils.h>

class Deshuffler
{
public:
    Deshuffler() = default;
    Deshuffler(const InputParams& params) : m_params(params) {}
    void CalculatePermutation();
    void OutputPermutation();
    void ReconstructStream();
    void OutputStream();
private:
    InputParams m_params;
    PermutationData m_permutation_data;
    YUVReaderSeek m_YUVReaderSeek;
    CSmplYUVWriter m_YUVWriter;
};

#endif /* DESHUFFLER_H_ */
