#include "../../include/tlwe/TLWESample.h"

TLWESample::TLWESample(TLWEParams* params)
{
    for (size_t i = 0; i < this->a.size(); i++)
        a.at(i) = new TorusPolynomial{params->getDegree()};
    this->b = a.at(params->getPolyAmount());
    this->current_noise = 0.;
}

TLWESample::~TLWESample()
{
}

TorusPolynomial* TLWESample::getB() {
    return this->b;
}

TorusPolynomial* TLWESample::getB(int i) {
    return (this->b) + i;
}

TorusPolynomial* TLWESample::getA(int i) {
    return this->a.at(i);
}

void TLWESample::setNoise(double noise) {
    this->current_noise = noise;
}