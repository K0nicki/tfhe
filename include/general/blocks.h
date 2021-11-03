#pragma once

#include "../tgsw/functions.h"
#include "../tlwe/functions.h"
#include "cloud.h"

void cMux(TLWESample *result, TGSWSample *cs, TLWESample *d1, TLWESample *d0, TGSWParams *params);

void blindRotate(TLWESample *result, TLWESample *source, LWESample *lweSample, GateKey *gateKey);

void bootstrapinglwe2lwe(LWESample *result, LWESample *testvect, GateKey *gateKey);