#include "../../include/general/polynomials.h"

IntPolynomial::IntPolynomial()
{
    this->N = DEF_N;
    for (int i = 0; i < this->coefficients.size(); i++)
        coefficients[i] = 0;
}

IntPolynomial::IntPolynomial(std::array<int32_t, DEF_N> coefficients) {
    this->N = DEF_N;
    for (int i = 0; i < this->coefficients.size(); i++)
        this->coefficients[i] = coefficients[i];
}

IntPolynomial::~IntPolynomial() {}

IntPolynomial &IntPolynomial::operator=(IntPolynomial &other)
{
    for (int i = 0; i < this->coefficients.size(); i++)
        this->coefficients[i] = other.coefficients[i];

    this->N = other.N;

    return *this;
}

void IntPolynomial::setCoefficient(int i, int32_t value)
{
    this->coefficients[i] = value;
}

std::array<int32_t, DEF_N> *IntPolynomial::getCoefAsArray()
{
    return &(this->coefficients);
}

int32_t *IntPolynomial::getCoef()
{
    return this->coefficients.data();
}

int32_t IntPolynomial::getCoef(int i)
{
    return this->coefficients.at(i);
}

// I know, it ignores N parameter
TorusPolynomial::TorusPolynomial(int32_t N)
{
    this->N = DEF_N;
    for (int i = 0; i < this->TorusCoefficients.size(); i++)
        TorusCoefficients[i] = 0;
}

TorusPolynomial::TorusPolynomial()
{
    this->N = DEF_N;
    for (int i = 0; i < this->TorusCoefficients.size(); i++)
        TorusCoefficients[i] = 0;
}

TorusPolynomial::~TorusPolynomial() {}

int32_t TorusPolynomial::getLenght()
{
    return this->N;
}

void TorusPolynomial::setCoefficient(int i, int32_t value)
{
    this->TorusCoefficients[i] = value;
}

int32_t *TorusPolynomial::getCoef()
{
    return this->TorusCoefficients.data();
}

int32_t TorusPolynomial::getCoef(int i)
{
    return this->TorusCoefficients.at(i);
}

std::array<Torus32, DEF_N> *TorusPolynomial::getCoefAsArray()
{
    return &(this->TorusCoefficients);
}

FftPoly::FftPoly()
{
    for (int i = 0; i < this->data.size(); i++)
        data.at(i) = 0;
}
FftPoly::~FftPoly() {}

double *FftPoly::getArray()
{
    return this->data.data();
}

double FftPoly::getCoef(int i)
{
    return data.at(i);
}

int FftPoly::getLength()
{
    return DEF_N;
}

void FftPoly::setCoef(int i, double value)
{
    this->data[i] = value;
}