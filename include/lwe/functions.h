#pragma once
#include "LWEKey.h"
#include "../general/functions.h"
#include "../tlwe/TLWEKey.h"

/**
 * @brief           Generate random LWE key for the given parameters 
 * 
 * @param result    LWE Key for the result. It must contains initialized LWE Params
 * 
 * @cite https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
 */
LWEKey lweKeyGen();

/**
 * @brief           Encryption function
 *                  To do so we need a message which we want to encrypt, standard deviation for small error
 *                  and a secret key
 * 
 * @details         Algorithm relies on masking the secret key with randomly generated vector a<Torus32>.
 *                  To the result we just need to add the noise - vector e generated with gaussian deviation funciton (definied in general/functions)
 *                  After all computations we just need to update noise (but what is the value of the noise? I thought it is currentNoise * noise factor - is it true?)
 * 
 * @param result    The encrypted LWE message as LWESample
 * @param message   Torus32 message to encrypt
 * @param alpha     Standard deviation alpha
 * @param key       The secret key, necessary to encrypt the message
 */
LWESample lweEncrypt(Torus32 *message, double noise, LWEKey *key);

/**
 * @brief           Get phase of the encrypted in Torus32 message
 *                  Use pattern phi = b - a*s
 * 
 * @param sample    Encrypted message
 * @param key       Secret key
 * @return          Torus32 phase of the encrypted message (needed to be aproximated for full encryption)
 */
Torus32 lwePhase(LWESample *sample, LWEKey *key);

template <int32_t n=DEF_N>
Torus32 lwePhaseN(LWESample *sample, TLWEKey *key);
/**
 * @brief           Decryption function. It returns a Torus32 value which is expected result
 * 
 * @param sample    Sample which contains encrypted message
 * @param key       Secret key
 * @param Msize     Message space, neccesary for aproximate the phase
 * @return          Torus32 encrypted message
 */
Torus32 lweDecrypt(LWESample *sample, LWEKey *key, int32_t Msize);
Torus32 lweDecryptN(LWESample *sample, TLWEKey *key, int32_t Msize);

/**
 * @brief   Function clear LWE Sample object
 *          Set all variable to default values
 */
void lweClear(LWESample* result, LWEParams *params);

/**
 * @brief Add two LWE Samples
 * 
 * @param result    Result of the addition
 * @param sample    This sample will be added to the result
 * @param params    Provide information about the key lenght
 */
void lweAdd(LWESample *a, LWESample *b, LWEParams *params);

/**
 * @brief   Subtract two LWE Samples
 * 
 * @param result    Result of the subtraction
 * @param sample    This sample will be substracted from the result
 * @param params    Provide information about the key lenght
 */
void lweSub(LWESample *a, LWESample *b, LWEParams *params);

/**
 * @brief    Copy LWE samples
 * 
 */
void lweCopy(LWESample *a, LWESample *b, LWEParams *params);