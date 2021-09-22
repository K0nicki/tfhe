#include "../../include/tgsw/TGSWSample.h"

TGSWSample::TGSWSample(std::array<TLWESample, 2 * DEF_l> samples, int32_t k, int32_t l) : all_samples(samples), k(k), l(l) {}

TGSWSample::~TGSWSample()
{
}

TLWESample TGSWSample::getSampleAt(int i)
{
    return this->all_samples.at(i);
}

int32_t TGSWSample::getPolyNumber() {
    return this->k;
}

int32_t TGSWSample::getDecompLength() {
    return this->l;
}