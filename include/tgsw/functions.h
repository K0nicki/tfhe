#pragma once

#include "TGSWSampleFft.h"
#include "../tlwe/functions.h"

void tgswKeyGen(TGSWKey *result);

void tgswEncryptInt(TGSWSample *sample, int32_t message, double alpha, TGSWKey *key);
TGSWSample tgswEncrypt(IntPolynomial *message, double alpha, TGSWKey *key);

void tgswEncryptInt(TGSWSample *sample, int32_t message, double alpha, TGSWKey *key);

TGSWSampleFft tgswEncryptPolyFft(IntPolynomial* msg, double alpha, TGSWKey *key);
TGSWSampleFft tgswEncryptFft(int32_t msg, double alpha, TGSWKey *key);

void externalTgswProduct(TLWESample *result, TLWESample *tlweSample, TGSWSample *tgswSample, TGSWParams *params);
void externalTgswProduct(TLWESample *result, TLWESample *tlweSample, TGSWSampleFft *tgswSampleFft, TGSWParams *params);