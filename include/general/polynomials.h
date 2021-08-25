#pragma once
#include "../tfhe_core.h"

class IntPolynomial
{
private:
    int32_t N;
    int32_t* coefficients;
public:
    IntPolynomial();
    IntPolynomial(int32_t N);
    ~IntPolynomial();

    // Setters
    void setCoefficient(int32_t value, int i);

    // Getters
    int32_t getCoef(int i);
    
    // forbidden
    IntPolynomial(IntPolynomial&) = delete;
    IntPolynomial* operator=(IntPolynomial&) = delete;
};


class TorusPolynomial
{
private:
    int32_t N;
    Torus32* TorusCoefficients;
public:
    TorusPolynomial();
    TorusPolynomial(int32_t N);
    ~TorusPolynomial();

    // forbidden
    TorusPolynomial(TorusPolynomial&) = delete;
    TorusPolynomial* operator=(TorusPolynomial&) = delete;
};
