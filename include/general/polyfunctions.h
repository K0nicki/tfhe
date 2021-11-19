#pragma once

#include "polynomials.h"

/**
 * @brief
 *
 * @param result
 * @param poly1
 * @param poly2
 */
template <int32_t N = DEF_N>
void torusPolyfft(FftPoly *result, std::array<int32_t, N> *poly);

template <int32_t N = DEF_N>
void torusPolyifft(std::array<int32_t, N> *result, FftPoly *poly);

void fmaPolyFD(FftPoly *res, FftPoly *a, FftPoly *b);
FftPoly mulPolyFD(FftPoly *a, FftPoly *b);

void polyMulByX_i(TorusPolynomial *result, TorusPolynomial *source, uint32_t p);
void polyMulByX_iMinOne(TorusPolynomial *result, TorusPolynomial *source, int32_t p);

void torusPolyMulFD(TorusPolynomial *result, IntPolynomial *poly1, TorusPolynomial *poly2);
void torusPolyMulSubFD(TorusPolynomial *result, IntPolynomial *poly1, TorusPolynomial *poly2);

void torusPolyMakeRandom(TorusPolynomial *result);

void torusPolyClear(TorusPolynomial *result);
bool torusPolyEQ(TorusPolynomial *a, TorusPolynomial *b);
void torusPolyAddTo(TorusPolynomial *result, TorusPolynomial *a);
void torusPolyAdd(TorusPolynomial *result, TorusPolynomial *a, TorusPolynomial *b);
void torusPolySubTo(TorusPolynomial *result, TorusPolynomial *a);
void torusPolySub(TorusPolynomial *result, TorusPolynomial *a, TorusPolynomial *b);
void torusPolyCopy(TorusPolynomial *result, TorusPolynomial *a);
double intPolySq2(IntPolynomial *poly);
