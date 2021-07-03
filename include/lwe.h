/**
 * @file lwe.h
 * @author Konrad Konicki
 * @brief This file contains LWE function (???)
 * @date 2021-07-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "tfhe_core.h"

/**
 * @brief               Class which contains LWE parameters such as lenght, min and max noise
 * 
 * @param n             lenght of the key
 * @param noise_min     Min allowed value of the noise which allows to decryption
 * @param noise_max     Max allowed value of the noise which allows to decryption
 * 
 * @note It is not allowed to modify this structure.
 * 
 */
class LWEParams
{
public:
    const int32_t n;
    const double noise_min;
    const double noise_max;

    // int32_t getLength();
    // double getNoiseMin();
    // double getNoiseMax();

    LWEParams(int32_t n, double noise_min, double noise_max);
    ~LWEParams();
    LWEParams(const LWEParams&) = delete;               // Modyfing is not allowed
    LWEParams& operator=(const LWEParams&) = delete;    // This too 
};

/**
 * @brief Class which contains the LWE key
 * 
 * @param params    LWEParams class, constains basic info 
 * @param key       Created key
 * 
 * @note Also this class can not be overwritten too
 */
class LWEKey
{
public:
    const LWEParams* params;
    int32_t* key;

    LWEParams* getParams();
    int32_t* getLWEKey();

    LWEKey(const LWEParams* params);
    ~LWEKey();
    LWEKey(const LWEKey&) = delete;             // Do not overwrite it
    LWEKey* operator=(const LWEKey&) = delete;
};

/**
 * @brief   
 * 
 */
class LWESample {
public:
    Torus32* a;
    Torus32 b;
    double current_variance;
    LWESample();
    ~LWESample();
};

/**
 * @brief           Generate random LWE key for the given parameters 
 * 
 * @param result    LWE Key for the result. It must contains initialized LWE Params
 */
void lweKeyGen(LWEKey* result);
