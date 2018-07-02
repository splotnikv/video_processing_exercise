#include "deshuffler.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    InputParams params(argc, argv);
    Deshuffler deshuffler(params);
    deshuffler.CalculatePermutation();
    deshuffler.OutputPermutation();
    deshuffler.ReconstructStream();
    deshuffler.OutputStream();
    return 0;
}

