#pragma once
#include "../tfhe_core.h"
#include "LWEParams.h"
#include <array>

/**
 * @brief LWESample means the class which contains encrypted message with the noise
 *
 * @param a             Randomly chosen mask
 * @param b             Result of encryption
 * @param currentNoise  Mean value of noise
 */
class LWESample
{
private:
    std::array<Torus32, DEF_N> a;       // array<int32_t, 1024>
    Torus32 b; 
    double currentNoise;

public:
    // Getters
    Torus32 getA(int i);
    Torus32 *getA();
    Torus32 getB();
    double getCurrentNoise();

    // Setters
    void setB(Torus32 value);
    void setA(Torus32 value, int i);
    void setNoise(double value);

    LWESample();
    LWESample(LWEParams *params);
    ~LWESample();
};
