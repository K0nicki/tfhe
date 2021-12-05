#include "../../include/lwe/functions.h"

void lweKeyGen(LWEKey *result)
{
    const int32_t n{result->getParams()->getLength()};
    std::uniform_int_distribution<int32_t> distrib(0, 1);

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

    for (int i = 0; i < n; i++)
        result->setKey(distrib(gen), i);
}

/**
 * @brief           Encryption function
 *                  To do so we need a message which we want to encrypt, standard deviation for small error
 *                  and a secret key
 *
 * @details         Algorithm relies on masking the secret key with randomly generated vector a<Torus32>.
 *                  To the result we just need to add the noise e generated with gaussian deviation funciton
 *
 * @param result    Encrypted message as LWESample
 * @param message   Torus32 message to encrypt
 * @param alpha     Standard deviation alpha
 * @param key       Secret key, necessary to encrypt the message
 */
LWESample lweEncrypt(Torus32 *message, double alpha, LWEKey *key)
{
    const int32_t n{key->getParams()->getLength()};
    LWESample result{key->getParams()};
    std::uniform_int_distribution<Torus32> distrib(INT32_MIN, INT32_MAX);
    int32_t *s = key->getLWEKey();

    std::random_device rd;
    std::mt19937 gen(rd());

    result.setB(addGaussianNoise(message, alpha));  // b = delta * m + e
    for (int i = 0; i < n; i++)                     // b = a*s + (delta * m + e)
    {
        result.setA(distrib(gen), i);
        result.setB(result.getB() + result.getA(i) * s[i]);
    }

    result.setNoise(alpha * alpha);
    return result;
}

LWESample lweEncrypt(Torus32 *message, double alpha, TLWEKey *key)
{
    const int32_t n{key->getTLWEParams()->getLWEParams()->getLength()};
    LWESample result{key->getTLWEParams()->getLWEParams()};
    std::uniform_int_distribution<Torus32> distrib(INT32_MIN, INT32_MAX);
    int32_t *s = key->getIntKey(0)->getCoefAsArray()->data();

    std::random_device rd;
    std::mt19937 gen(rd());

    result.setB(addGaussianNoise(message, alpha));  // b = delta * m + e
    for (int i = 0; i < n; i++)                     // b = a*s + (delta * m + e)
    {
        result.setA(distrib(gen), i);
        result.setB(result.getB() + result.getA(i) * s[i]);
    }

    result.setNoise(alpha * alpha);
    return result;
}

LWESample lweEncryptWthExtNoise(Torus32 *message, double alpha, Torus32 noise, LWEKey *key) {

    const int32_t n{key->getParams()->getLength()};
    LWESample result{key->getParams()};
    std::uniform_int_distribution<Torus32> distrib(INT32_MIN, INT32_MAX);
    int32_t *s = key->getLWEKey();

    std::random_device rd;
    std::mt19937 gen(rd());

    result.setB(*message + noise);                  // b = delta * m + e
    for (int i = 0; i < n; i++)                     // b = a*s + (delta * m + e)
    {
        result.setA(distrib(gen), i);
        result.setB(result.getB() + result.getA(i) * s[i]);
    }

    result.setNoise(alpha * alpha);
    return result;
}

template <int32_t n = DEF_N>
Torus32 lwePhase(LWESample *sample, LWEKey *key)
{
    // int32_t n{key->getParams()->getLength()};
    Torus32 axs{0};

    Torus32 *a{sample->getA()};
    int32_t *k{key->getLWEKey()};

    for (int32_t i = 0; i < n; i++)
        axs += a[i] * k[i];
    return sample->getB() - axs;
}

template <int32_t n = DEF_N>
Torus32 lwePhase(LWESample *sample, TLWEKey *key)
{
    // int32_t n{key->getParams()->getLength()};
    Torus32 axs{0};

    Torus32 *a{sample->getA()};
    int32_t *k{key->getIntKey(0)->getCoefAsArray()->data()};

    for (int32_t i = 0; i < n; i++)
        axs += a[i] * k[i];
    return sample->getB() - axs;
}

Torus32 lwePhase(LWESample *sample, LWEKey *key)
{
    return lwePhase<DEF_N>(sample, key);
}

Torus32 lwePhase(LWESample *sample, TLWEKey *key)
{
    return lwePhase<DEF_N>(sample, key);
}

Torus32 lweDecrypt(LWESample *sample, LWEKey *key, int32_t Msize)
{
    Torus32 phi{lwePhase(sample, key)};         // Get phase
    return approxPhase(phi, Msize);             // Approximation
}

Torus32 lweDecrypt(LWESample *sample, TLWEKey *key, int32_t Msize)
{
    Torus32 phi{lwePhase(sample, key)};
    return approxPhase(phi, Msize);
}

int32_t lweBoolDecrypt(LWESample *sample, TLWEKey *key, int32_t M)
{
    Torus32 phase = lwePhase(sample, key);
    return phase > 0 ? 0 : 1;
}

void lweClear(LWESample *result, LWEParams *params)
{
    const int32_t n = params->getLength();

    for (int32_t i = 0; i < n; ++i)
        result->setA(0, i);
    result->setB(0);
    result->setNoise(0.);
}

void lweAdd(LWESample *result, LWESample *sample, LWEParams *params)
{
    const int32_t n = params->getLength();

    for (int i = 0; i < n; i++)
        result->setA(result->getA(i) + sample->getA(i), i);

    result->setB(result->getB() + sample->getB());
    result->setNoise(result->getCurrentNoise() + sample->getCurrentNoise());
}

void lweSubTo(LWESample *result, LWESample *sample, LWEParams *params)
{
    const int32_t n = params->getLength();
    for (int i = 0; i < n; i++)
        result->setA(result->getA(i) - sample->getA(i), i);

    result->setB(result->getB() - sample->getB());
    result->setNoise(result->getCurrentNoise() + sample->getCurrentNoise());
}

void lweCopy(LWESample *result, LWESample *sample, LWEParams *params)
{
    const int32_t n = params->getLength();
    for (int i = 0; i < n; i++)
        result->setA(sample->getA(i), i);

    result->setB(sample->getB());
    result->setNoise(sample->getCurrentNoise());
}