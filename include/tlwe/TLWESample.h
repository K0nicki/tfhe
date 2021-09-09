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
    TLWESample(TLWEParams* params);
    ~TLWESample();

    // Setters
    void setNoise(double);

    // Getters
    TorusPolynomial* getA(int i);
    TorusPolynomial* getB();
    TorusPolynomial* getB(int i);
};
