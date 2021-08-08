#pragma once
#include "../tfhe_core.h"
#include "LWEParams.h"

/**
 * @brief LWESample means the class which contains encrypted message with the noise
 * 
 * @param a             Randomly chosen mask
 * @param b             Result of encryption
 * @param currentNoise  Mean value of noise
 * 
 * @note                Overwritting is not allowed
 */
class LWESample
{
private:
    Torus32 *a;
    Torus32 b;
    double currentNoise;

public:
    //Getters
    Torus32 *getA();
    Torus32 getB();
    double getCurrentNoise();

    // Setters
    void setB(Torus32 value);
    void setA(Torus32 value, int i);
    void setVariance(double value);

    LWESample(LWEParams *params);
    ~LWESample();
    LWESample(LWESample &) = delete;
    LWESample &operator=(LWESample &) = delete;
};
