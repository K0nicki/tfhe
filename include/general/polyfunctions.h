#pragma once

#include "polynomials.h"

/**
 * @brief 
 * 
 * @param result 
 * @param poly1 
 * @param poly2 
 */
void torusPolyMulFD(TorusPolynomial* result, IntPolynomial* poly1, TorusPolynomial* poly2);
void torusPolyMulSubFD(TorusPolynomial* result, IntPolynomial* poly1, TorusPolynomial* poly2);

void torusPolyMakeRandom(TorusPolynomial* result);

void torusPolyAddTo(TorusPolynomial* result, TorusPolynomial* a);
void torusPolySubTo(TorusPolynomial* result, TorusPolynomial* a);
void torusPolyCopy(TorusPolynomial* result, TorusPolynomial* a);