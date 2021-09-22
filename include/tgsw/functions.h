#pragma once
#include "TGSWSampleFft.h"
#include "../tlwe/functions.h"

void tgswKeyGen(TGSWKey* result);

TGSWSample tgswEncrypt(IntPolynomial* message, double alpha, TGSWKey* key);