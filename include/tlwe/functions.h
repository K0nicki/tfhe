#pragma once
#include "TLWESample.h"
#include "../lwe/functions.h"

/**
 * @brief 
 * 
 * @param result 
 */
void tlweKeyGen(TLWEKey *key, TLWEParams *params);

void tlweBaseEncryption(TLWESample *result, double alpha, TLWEKey *key);
TLWESample tlweEncrypt(TorusPolynomial *message, double alpha, TLWEKey *key);

// Phi = b - a*s
TorusPolynomial tlweGetPhase(TLWESample *sample, TLWEKey *key);

TorusPolynomial tlweApproxPhase(TorusPolynomial *phase, int32_t M, int32_t N);

TorusPolynomial tlweDecrypt(TLWESample *sample, TLWEKey *key, int32_t M);

void tlweSampleIndexExtract(LWESample *result, TLWESample *x, int32_t index, TLWEParams *rparams);

void tLweAddMulRTo(TLWESample *result, IntPolynomial *p, TLWESample *sample, TLWEParams *params);
void tlweAddTo(TLWESample* a, TLWESample * b, TLWEParams* params);
void tlweAdd(TLWESample* result, TLWESample *a, TLWESample *b, TLWEParams *params);
void tlweSubTo(TLWESample* a, TLWESample * b, TLWEParams* params);
void tlweSub(TLWESample* result, TLWESample *a, TLWESample *b, TLWEParams *params);