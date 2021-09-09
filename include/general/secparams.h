#pragma once
#include <cstdint>
#include <cmath>

// TLWE sec params
const uint32_t DEF_n = 630;
const double DEF_TLWE_ALPHA = pow(2.,-15);

// TRLWE sec params
const uint32_t DEF_Nbit = 10;
const uint32_t DEF_N = 1 << DEF_Nbit;     //1024
const double DEF_TRLWE_ALPHA = pow(2.,-25);
