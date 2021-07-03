#include "../include/lwe.h"
#include <random>

LWEParams::LWEParams(int32_t n, double minNoise, double maxNoise) : n(n),
                                                                    minNoise(minNoise),
                                                                    maxNoise(maxNoise) {}

LWEParams::~LWEParams() {}

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

void LWEKey::setKey(int32_t val, int i)
{
    this->key[i] = val;
}

LWEKey::LWEKey(LWEParams *params)
{
    this->params = params;
    this->key = new int32_t[params->getLength()];
}

LWEKey::~LWEKey()
{
    delete[] key;
}

LWEParams *LWEKey::getParams()
{
    return this->params;
}

int32_t *LWEKey::getLWEKey()
{
    return this->key;
}

/**
 * @brief LWE key generation function
 * 
 * @cite https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
 * 
 * @param result The LWE secret key
 */
void lweKeyGen(LWEKey *result)
{
    const int32_t n = result->getParams()->getLength();
    std::uniform_int_distribution<int32_t> distrib(0, 1);

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

    for (int i = 0; i < n; i++)
    {
        result->setKey(distrib(gen), i);
    }
}