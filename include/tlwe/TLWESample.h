#pragma once
#include "../general/polynomials.h"
#include "TLWEKey.h"
#include "../general/secparams.h"
#include <array>

class TLWESample
{
private:
    std::array<TorusPolynomial*, 3> a;      // k(==2) + 1 = 3
    TorusPolynomial* b;
    double current_noise;
    int32_t k;
public:
    TLWESample();
    TLWESample(TLWEParams* params);
    ~TLWESample();

    // Setters
    void setNoise(double);
    void setA(int i, TorusPolynomial* poly);
    void setB();
    void initSampleWthZero();

    // Getters
    double getNoise();
    TorusPolynomial* getA(int i);
    TorusPolynomial* getB();
    TorusPolynomial* getB(int i);
};
