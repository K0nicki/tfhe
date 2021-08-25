#include "../../include/general/polynomials.h"

IntPolynomial::IntPolynomial() {}

IntPolynomial::IntPolynomial(int32_t N) {
    this->N = N;
    this->coefficients = new int32_t[N];
}

IntPolynomial::~IntPolynomial(){
    delete[] this->coefficients;
}

void IntPolynomial::setCoefficient(int32_t value, int i) {
    this->coefficients[i] = value;
}

int32_t IntPolynomial::getCoef(int i) {
    return (this->coefficients)[i];
}

TorusPolynomial::TorusPolynomial(int32_t N) {
    this->N = N;
    this->TorusCoefficients = new int32_t[N];
}

TorusPolynomial::~TorusPolynomial() {
    delete[] this->TorusCoefficients;
}

