#include "../../include/lwe/LWESample.h"

LWESample::LWESample()
{
    a.fill(0);
    this->b = 0;
    this->currentNoise = 0.;
}

LWESample::LWESample(LWEParams *params)
{
    a.fill(0);
    this->b = 0;
    this->currentNoise = 0.;
}

LWESample::~LWESample()
{
    // delete[] a;
}

void LWESample::setB(Torus32 value)
{
    this->b = value;
}

void LWESample::setA(Torus32 value, int i)
{
    this->a[i] = value;
}

void LWESample::setNoise(double value)
{
    this->currentNoise = value;
}

Torus32 *LWESample::getA()
{
    return this->a.data();
}

Torus32 LWESample::getA(int i) {
    return a.at(i);
}


Torus32 LWESample::getB()
{
    return this->b;
}

double LWESample::getCurrentNoise()
{
    return this->currentNoise;
}