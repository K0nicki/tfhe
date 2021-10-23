#include "../../include/tlwe/TLWESample.h"

TLWESample::TLWESample(TLWEParams *params)
{
    for (size_t i = 0; i < this->a.size(); i++)
        a.at(i) = new TorusPolynomial{params->getDegree()};
    this->b = a.at(params->getPolyAmount());
    this->current_noise = 0.;
    this->k = params->getPolyAmount();
}

TLWESample::TLWESample()
{
    for (size_t i = 0; i < this->a.size(); i++)
        a.at(i) = new TorusPolynomial{};
    this->b = a.at(2);
    this->current_noise = 0.;
}

TLWESample::~TLWESample()
{
}

TorusPolynomial *TLWESample::getB()
{
    return this->b;
}

TorusPolynomial *TLWESample::getB(int i)
{
    return (this->b) + i;
}

TorusPolynomial *TLWESample::getA(int i)
{
    return this->a.at(i);
}

void TLWESample::setNoise(double noise)
{
    this->current_noise = noise;
}

double TLWESample::getNoise(){ return this->current_noise; }

void TLWESample::initSampleWthZero()
{
    int32_t N = b->getLenght();

    for (int i = 0; i < k; i++)
        for (int j = 0; j < N; i++)
            a.at(k)->setCoefficient(j, 0);
}

void TLWESample::setA(int i, TorusPolynomial *poly)
{
    this->a.at(i) = poly;
}

void TLWESample::setB()
{
    this->b = this->a.at(2);
}