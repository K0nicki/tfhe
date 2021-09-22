#include "../../include/tgsw/TGSWSampleFft.h"

TGSWSampleFft::TGSWSampleFft(TGSWSample* sample)
{
    this->setDecompLength(sample->getDecompLength());
    this->setPolyNumber(sample->getPolyNumber());
    // for (int i = 0; i < 2*DEF_l; i++)
    //     for (int j = 0; j < 2; j++)
    //         fftSamples.at(i).at(j) = new FftPoly;        
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