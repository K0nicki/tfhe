#pragma once

#include "TGSWKey.h"
#include "../tlwe/TLWESample.h"

class TGSWSample
{
private:
    std::array<TLWESample*, 2*DEF_l> all_samples;
    // TLWESample* all_samples; // A pointer to the first element in array of TLWE Samples
    int32_t k;
    int32_t l;
public:
    std::array<TLWESample*, 2*DEF_l> getCoefAsArray();
    TLWESample* getSampleAt(int i);
    int32_t getPolyNumber();
    int32_t getDecompLength();

    TGSWSample();
    TGSWSample(TGSWParams* params);
    ~TGSWSample();
};
