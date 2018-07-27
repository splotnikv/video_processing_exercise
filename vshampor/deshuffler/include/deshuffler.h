#ifndef DESHUFFLER_H_
#define DESHUFFLER_H_

#include "permutation_data.h"
#include "input_params.h"
#include "yuv_reader_seek_i420.h"
#include "permut_calc_task.h"
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
    std::vector<PermutCalcTask> GeneratePermutCalcTasks();
    mfxStatus CalculatePermutCalcTask(PermutCalcTask& task);
    InputParams m_params;
    PermutationData m_permutation_data;
    CSmplYUVWriter m_YUVWriter;
};

#endif /* DESHUFFLER_H_ */
