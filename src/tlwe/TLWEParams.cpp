#include "../../include/tlwe/TLWEParams.h"

TLWEParams::TLWEParams(int32_t N, int32_t k, double alpha_min, double alpha_max):
    N{DEF_N},
    k{k},
    alpha_min{alpha_min},
    alpha_max{alpha_max},
    lweParams{N*k, alpha_min, alpha_max} {}

TLWEParams::~TLWEParams() {}

int32_t TLWEParams::getDegree() { return this->N; }

int32_t TLWEParams::getPolyAmount() { return this->k; }

LWEParams* TLWEParams::getLWEParams() { return &(this->lweParams); }