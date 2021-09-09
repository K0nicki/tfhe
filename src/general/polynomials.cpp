#include "../../include/general/polynomials.h"

IntPolynomial::IntPolynomial() {}

IntPolynomial::IntPolynomial(int32_t N) {
    this->N = DEF_N;
    for (int i = 0; i < this->coefficients.size(); i++)
            coefficients[i] = 0;
}

IntPolynomial::~IntPolynomial(){
}

void IntPolynomial::setCoefficient(int32_t value, int i) {
    this->coefficients[i] = value;
}

std::array<int32_t, DEF_N>* IntPolynomial::getCoefAsArray() {
    return &(this->coefficients);
}

int32_t* IntPolynomial::getCoef() {
    return this->coefficients.data();
}

int32_t IntPolynomial::getCoef(int i) {
    return (this->coefficients)[i];
}

// I know, it ignores N parameter
TorusPolynomial::TorusPolynomial(int32_t N) {
    this->N = DEF_N;
    for (int i = 0; i < this->TorusCoefficients.size(); i++)
            TorusCoefficients[i] = 0;
}

TorusPolynomial::~TorusPolynomial() {
}

int32_t TorusPolynomial::getLenght() {
    return this->N;
}

void TorusPolynomial::setCoefficient( int i, int32_t value) {
    this->TorusCoefficients[i] = value;
}

int32_t* TorusPolynomial::getCoef() {
    return this->TorusCoefficients.data();
}

int32_t TorusPolynomial::getCoef(int i) {
    return this->TorusCoefficients.at(i);
}

std::array<Torus32, DEF_N>* TorusPolynomial::getCoefAsArray() {
    return &(this->TorusCoefficients);
}

FftPoly::FftPoly() {}
FftPoly::~FftPoly() {}

double* FftPoly::getArray() {
    return this->data.data();
}

double FftPoly::getCoef(int i) {
    if (i >= DEF_N )
        throw "FftPoly got i greater than DEF_N";
    return *((this->data.data())+i);
}

int FftPoly::getLength() {
    return DEF_N;
}

void FftPoly::setCoef(int i, double value) {
    this->data[i] = value;
}