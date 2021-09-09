#pragma once
#include "TLWEParams.h"
#include "../general/polynomials.h"

class TLWEKey
{
private:
    TLWEParams *params;
    std::array<IntPolynomial*, 2> key;
public:
    // Getters
    TLWEParams* getTLWEParams();
    IntPolynomial* getKey();
    IntPolynomial* getKey(int i);

    // Setters
    TLWEKey(TLWEParams* params);
    ~TLWEKey();

    // forbidden
    TLWEKey(TLWEKey&) = delete;
    TLWEKey* operator=(TLWEKey&) = delete;
};
