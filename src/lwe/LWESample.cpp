#include "../../include/lwe/LWESample.h"

LWESample::LWESample(LWEParams *params)
{
    this->a = new Torus32[params->getLength()];
    this->b = 0;
    this->currentNoise = 0.;
}

LWESample::~LWESample()
{
    delete[] a;
}

void LWESample::setB(Torus32 value)
{
    this->b = value;
}

void LWESample::setA(Torus32 value, int i)
{
    this->a[i] = value;
}

void LWESample::setVariance(double value)
{
    this->currentNoise = value;
}

Torus32 *LWESample::getA()
{
    return this->a;
}

Torus32 LWESample::getB()
{
    return this->b;
}

double LWESample::getCurrentNoise()
{
    return this->currentNoise;
}