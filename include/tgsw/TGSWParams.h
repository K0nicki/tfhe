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
    int32_t Bg;

    // Bg-1 
    uint32_t mask;

    // Params for each row
    TLWEParams* tlweParams;

    // number of rows = (k+1)*l
    int32_t rows;

    // powers of BgBit
    std::array<int32_t,DEF_l> h;

    // offset = Bg/2 *(2^(32-BgBit) + 2^(32-2*BgBit) + ... + 2^(32-l*BgBit))
    uint32_t offset;
public:
    TGSWParams(TLWEParams* tlweParams);
    ~TGSWParams();

    int32_t getDecompositionLength();
    int32_t getBgbit();
    int32_t getBg();
    int32_t getMask();
    TLWEParams* getTLWEParams();
    int32_t getNoRows();
    int32_t* getBgbitPowers();
    uint32_t getOffset();
};
