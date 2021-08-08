#include "../../include/lwe/LWEParams.h"
#include <random>

LWEParams::LWEParams(int32_t n, double minNoise, double maxNoise) : n(n),
                                                                    minNoise(minNoise),
                                                                    maxNoise(maxNoise) {}

int32_t LWEParams::getLength()
{
    return this->n;
}

double LWEParams::getMinNoise()
{
    return this->minNoise;
}

double LWEParams::getMaxNoise()
{
    return this->maxNoise;
}
