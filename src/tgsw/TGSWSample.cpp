#include "../../include/tgsw/TGSWSample.h"

TGSWSample::TGSWSample() {}

TGSWSample::TGSWSample(TGSWParams* params)
{
    this->k = params->getTLWEParams()->getPolyAmount();
    this->l = params->getDecompositionLength();
    for (int i = 0; i < all_samples.size(); i++)
        all_samples[i] = new TLWESample{params->getTLWEParams()};    
}

TGSWSample::~TGSWSample() {}

TLWESample* TGSWSample::getSampleAt(int i)
{
    return this->all_samples.at(i);
}

int32_t TGSWSample::getPolyNumber() {
    return this->k;
}

int32_t TGSWSample::getDecompLength() {
    return this->l;
}

std::array<TLWESample*, 2*DEF_l> TGSWSample::getCoefAsArray() {
    return all_samples;
}
