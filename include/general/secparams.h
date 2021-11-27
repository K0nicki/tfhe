#pragma once
#include <cstdint>
#include <cmath>

// TLWE sec params
const int32_t DEF_n = 630;
const double DEF_TLWE_ALPHA = pow(2.,-15);

// TRLWE sec params
const int32_t DEF_Nbit = 10;
const int32_t DEF_N = 1 << DEF_Nbit;     //1024
const double DEF_TRLWE_ALPHA = pow(2.,-25);

// TGSW params
const int32_t DEF_l = 3;
const int32_t DEF_BgBit = 6;
const uint32_t DEF_Bg = 1UL << DEF_BgBit;  //64

// KeySwitch params
const int32_t DEF_basebit = 2;
const int32_t DEF_tt = 8;