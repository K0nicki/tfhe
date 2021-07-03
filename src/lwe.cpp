#include "../include/lwe.h"
#include <random>

LWEParams::LWEParams(int32_t n, double noise_min, double noise_max):
    n(n),
    noise_min(noise_min),
    noise_max(noise_max) {}

LWEParams::~LWEParams() {}

LWEKey::LWEKey(const LWEParams* params) {
    this->params = params;
    this->key = new int32_t[params->n];
}

LWEKey::~LWEKey() {
    delete[] key;
}

// int32_t LWEParams::getLength() {
//     return this->n;
// }

// double LWEParams::getNoiseMin() {
//     return this->noise_min;
// }

// double LWEParams::getNoiseMax() {
//     return this->noise_max;
// }

/**
 * @brief LWE key generation function
 * 
 * @cite https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
 * 
 * @param result The LWE secret key
 */
void lweKeyGen(LWEKey* result) {
    const int32_t n = result->params->n;
    // const int32_t n = result->params->getLength();
    std::uniform_int_distribution<int32_t> distrib(0, 1);

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    
    for (int i = 0; i < n; i++)
    {
        result->key[i] = distrib(gen);
    }
}

int32_t* LWEKey::getLWEKey() {
    return this->key;
}