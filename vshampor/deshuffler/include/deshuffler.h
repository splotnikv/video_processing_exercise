#ifndef DESHUFFLER_H_
#define DESHUFFLER_H_

#include "permutation_data.h"
#include "input_params.h"

class Deshuffler
{
public:
    Deshuffler() = default;
    Deshuffler(const InputParams& _params) : params(_params) {}
    void CalculatePermutation();
    void OutputPermutation();
    void ReconstructStream();
    void OutputStream();
private:
    InputParams params;
    PermutationData permutation_data;

};

#endif /* DESHUFFLER_H_ */
