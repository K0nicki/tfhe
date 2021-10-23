#pragma once

#include "../tlwe/TLWEParams.h"
#include <array>

/**
 * @brief 
 * 
 */
class TGSWParams
{
private:
    // decomposition length
    int32_t l;

    // log2(Bg)
    int32_t BgBit;

    // Decomposition base
    uint32_t Bg;

    // Bg-1 
    uint32_t mask;

    // Params for each row
    TLWEParams* tlweParams;

    // number of rows = (k+1)*l
    int32_t rows;

    // powers of BgBit
    std::array<uint32_t,DEF_l> h;

    // offset = Bg/2 *(2^(32-BgBit) + 2^(32-2*BgBit) + ... + 2^(32-l*BgBit))
    uint32_t offset;
public:
    TGSWParams(TLWEParams* tlweParams);
    ~TGSWParams();

    int32_t getDecompositionLength();
    int32_t getBgbit();
    uint32_t getBg();
    uint32_t getMask();
    TLWEParams* getTLWEParams();
    int32_t getNoRows();
    std::array<uint32_t,DEF_l> getBgbitPowers();
    uint32_t getOffset();
};
