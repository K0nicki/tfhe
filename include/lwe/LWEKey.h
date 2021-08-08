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
    int32_t *key;

public:
    LWEParams *getParams();
    int32_t *getLWEKey();

    void setKey(int32_t val, int i);

    LWEKey(LWEParams *params);
    ~LWEKey();
    LWEKey(const LWEKey &) = delete; // Do not overwrite it
    LWEKey *operator=(const LWEKey &) = delete;
};

/**
 * @brief           Generate random LWE key for the given parameters 
 * 
 * @param result    LWE Key for the result. It must contains initialized LWE Params
 */
void lweKeyGen(LWEKey *result);
