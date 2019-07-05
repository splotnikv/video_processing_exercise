#include "deshuffler.h"
#include "permut_calc_task.h"
#include <thread>
#include <vector>

void Deshuffler::CalculatePermutation()
{
    std::vector<PermutCalcTask> tasks = GeneratePermutCalcTasks();
    std::vector<std::thread> threads;
    for (auto task : tasks)
    {
        mfxStatus sts = MFX_ERR_NONE;
        sts = CalculatePermutCalcTask(task);
    }

}

// Returns MFX_ERR_NONE if all permutation data is computed and valid
// Returns MFX_ERR_MORE_DATA_SUBMIT_TASK if permutation data
// for some frames in the task could not be computed/is invalid

mfxStatus Deshuffler::CalculatePermutCalcTask(PermutCalcTask& task)
{
    return MFX_ERR_MORE_DATA_SUBMIT_TASK;
}

std::vector<PermutCalcTask> Deshuffler::GeneratePermutCalcTasks()
{
    return std::vector<PermutCalcTask>();
}

void Deshuffler::OutputPermutation()
{
}

void Deshuffler::ReconstructStream()
{
}

void Deshuffler::OutputStream()
{
}
