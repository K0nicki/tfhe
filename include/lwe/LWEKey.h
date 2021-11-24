#pragma once
#include "../tfhe_core.h"
#include "LWESample.h"

/**
 * @brief           Class which contains the LWE key
 * 
 * @param params    LWEParams class, constains basic info 
 * @param key       Created key
 * 
 * @note            Also this class can not be overwritten too
 */
class LWEKey
{
private:
    LWEParams *params;
    std::array<int32_t ,DEF_N> key;

public:
    LWEParams *getParams();
    int32_t *getLWEKey();
    int32_t getLWEKey(int i);

    void setKey(int32_t val, int i);

    LWEKey(LWEParams *params);
    ~LWEKey();
};

