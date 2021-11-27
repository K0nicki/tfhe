#include "../../include/tgsw/TGSWSampleFft.h"
#include "../../include/general/polyfunctions.h"

TGSWSampleFft::TGSWSampleFft() {}

TGSWSampleFft::TGSWSampleFft(TGSWSample* sample)
{
    this->l = sample->getDecompLength();
    this->k = sample->getPolyNumber();
}

TGSWSampleFft::~TGSWSampleFft()
{
}

void TGSWSampleFft::setPoly(int i, int j, FftPoly poly)
{
    fftSamples.at(i).at(j) = poly;
}


int TGSWSampleFft::getSamplesSize() {
    return this->fftSamples.size();
}

FftPoly* TGSWSampleFft::getPoly(int i, int j) {
    return &fftSamples.at(i).at(j);
}

void TGSWSampleFft::setDecompLength(int32_t value) {
    this->l = value;
}

void TGSWSampleFft::setPolyNumber(int32_t value) {
    this->k = value;
}

void TGSWSampleFft::initTGSWFftSample(TGSWSample* sample) {
    int32_t l = sample->getDecompLength();
    int32_t k = sample->getPolyNumber();

    for (int i = 0; i < 2*DEF_l; i++)
        for (int j = 0; j < 2; j++)
            torusPolyfft<DEF_N>(&fftSamples[i][j], sample->getSampleAt(i)->getA(j)->getCoefAsArray());
}
