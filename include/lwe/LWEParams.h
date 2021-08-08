#pragma once
#include "../tfhe_core.h"

/**
 * @brief               Class which contains LWE parameters such as lenght, min and max noise
 * 
 * @param n             lenght of the key
 * @param minNoise      Min allowed value of the noise which allows to decryption
 * @param maxNoise      Max allowed value of the noise which allows to decryption
 * 
 * @note                It is not allowed to modify this structure.
 * 
 */
class LWEParams
{
private:
    int32_t n;
    double minNoise;
    double maxNoise;

public:
    int32_t getLength();
    double getMinNoise();
    double getMaxNoise();

    void setLength(int32_t n);
    void setNoiseMin(double minNoise);
    void setNoiseMax(double maxNoise);

    LWEParams(int32_t n, double minNoise, double maxNoise);
    LWEParams(LWEParams &) = delete;            // Modyfing is not allowed
    LWEParams &operator=(LWEParams &) = delete; // This too
};
