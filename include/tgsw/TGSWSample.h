#pragma once

#include "TGSWKey.h"
#include "../tlwe/TLWESample.h"

class TGSWSample
{
private:
    // Instead of 3D matrix do 2D matrix with twice length
    std::array<TLWESample*, 2*DEF_l> all_samples;   
    int32_t k;  // number of polynomials in masl
    int32_t l;  // Decomposition matrix depth
public:
    std::array<TLWESample*, 2*DEF_l> getCoefAsArray();
    TLWESample* getSampleAt(int i);
    int32_t getPolyNumber();
    int32_t getDecompLength();

    TGSWSample();
    TGSWSample(TGSWParams* params);
    ~TGSWSample();
};


