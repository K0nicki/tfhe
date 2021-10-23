#pragma once
#include "../tfhe_core.h"
#include "../general/secparams.h"
#include <array>

class IntPolynomial
{
private:
    int32_t N;
    std::array<int32_t, DEF_N> coefficients;

public:
    IntPolynomial();
    IntPolynomial(std::array<int32_t, DEF_N> coefficients);
    ~IntPolynomial();

    // Setters
    void setCoefficient(int i, int32_t value);

    // Getters
    int32_t *getCoef();
    int32_t getCoef(int i);
    std::array<int32_t, DEF_N> *getCoefAsArray();
    IntPolynomial &operator=(IntPolynomial &);
};

class TorusPolynomial
{
private:
    int32_t N;
    std::array<Torus32, DEF_N> TorusCoefficients;

public:
    TorusPolynomial(int32_t N);
    TorusPolynomial();
    ~TorusPolynomial();

    // Getter
    int32_t getLenght();
    int32_t *getCoef();
    int32_t getCoef(int i);
    std::array<Torus32, DEF_N> *getCoefAsArray();

    // Setters
    void setCoefficient(int i, int32_t value);
};

/**
 * @brief Class used for fft operations
 * Stores returned structured from fft processor
 */
class FftPoly
{
private:
    std::array<double, DEF_N> data;

public:
    FftPoly();
    ~FftPoly();

    // Setters
    void setCoef(int i, double value);

    // Getters
    double *getArray();
    double getCoef(int i);
    int getLength();
};