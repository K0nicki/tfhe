#pragma once
#include "TLWESample.h"

/**
 * @brief 
 * 
 * @param result 
 */
void tlweKeyGen(TLWEKey* result);

void tlweEncrypt(TLWESample* result, TorusPolynomial* message, double alpha, TLWEKey* key);

// Phi = b - a*s
void tlweGetPhase(TorusPolynomial* phase, TLWESample* sample, TLWEKey* key);

void tlweApproxPhase(TorusPolynomial* message, TorusPolynomial* phase, int32_t M, int32_t N);

void tlweDecrypt(TorusPolynomial* result, TLWESample* sample, TLWEKey* key, int32_t M);