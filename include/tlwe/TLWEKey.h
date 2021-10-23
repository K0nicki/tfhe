#pragma once
#include "TLWEParams.h"
#include "../general/polynomials.h"

class TLWEKey
{
private:
    TLWEParams *params;
    std::array<IntPolynomial *, 2> key;

public:
    // Getters
    TLWEParams *getTLWEParams();
    IntPolynomial *getIntKey();
    IntPolynomial *getIntKey(int i);

    // Setters

    TLWEKey(TLWEParams *params);
    ~TLWEKey();
    TLWEKey &operator=(TLWEKey &);
};
