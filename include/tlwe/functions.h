#pragma once
#include "TLWESample.h"

/**
 * @brief 
 * 
 * @param result 
 */
TLWEKey tlweKeyGen(TLWEParams * params);

void tlweBaseEncryption(TLWESample *result, double alpha, TLWEKey *key);
TLWESample tlweEncrypt(TorusPolynomial* message, double alpha, TLWEKey* key);

// Phi = b - a*s
TorusPolynomial tlweGetPhase(TLWESample* sample, TLWEKey* key);

TorusPolynomial tlweApproxPhase(TorusPolynomial* phase, int32_t M, int32_t N);

TorusPolynomial tlweDecrypt(TLWESample* sample, TLWEKey* key, int32_t M);