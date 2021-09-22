#pragma once

#include "polynomials.h"

/**
 * @brief 
 * 
 * @param result 
 * @param poly1 
 * @param poly2 
 */
template<int32_t N>
void torusPolyfft(FftPoly* result, std::array<int32_t, N>* poly);
template<int32_t N>
void torusPolyifft (std::array<int32_t, N>* result, FftPoly* poly);

void torusPolyMulFD(TorusPolynomial* result, IntPolynomial* poly1, TorusPolynomial* poly2);
void torusPolyMulSubFD(TorusPolynomial* result, IntPolynomial* poly1, TorusPolynomial* poly2);

void torusPolyMakeRandom(TorusPolynomial* result);

void torusPolyAddTo(TorusPolynomial* result, TorusPolynomial* a);
void torusPolySubTo(TorusPolynomial* result, TorusPolynomial* a);
void torusPolyCopy(TorusPolynomial* result, TorusPolynomial* a);