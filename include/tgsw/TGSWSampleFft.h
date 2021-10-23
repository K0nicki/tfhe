#pragma once

#include "TGSWSample.h"
#include "../general/polynomials.h"

class TGSWSampleFft
{
private:
    std::array<std::array<FftPoly,2>, 2*DEF_l> fftSamples;
    int32_t k;
    int32_t l;
public:
    TGSWSampleFft();
    TGSWSampleFft(TGSWSample* sample);
    ~TGSWSampleFft();

    void setPoly(int i, int j, FftPoly poly);
    FftPoly* getPoly(int i, int j);
    int getSamplesSize();

    void initTGSWFftSample(TGSWSample* sample);

    void setDecompLength(int32_t value);
    void setPolyNumber(int32_t value);
};
