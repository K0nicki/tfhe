#include "../../include/lwe/functions.h"


void lweKeyGen(LWEKey *result)
{
    const int32_t n{result->getParams()->getLength()};
    std::uniform_int_distribution<int32_t> distrib(0, 1);

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

    for (int i = 0; i < n; i++)
        result->setKey(distrib(gen), i);
}

LWESample lweEncrypt(Torus32 *message, double alpha, LWEKey *key)
{
    const int32_t n{key->getParams()->getLength()};
    LWESample result{key->getParams()};
    std::uniform_int_distribution<Torus32> distrib(INT32_MIN, INT32_MAX);
    Torus32 a;

    std::random_device rd; 
    std::mt19937 gen(rd());

    (&result)->setB(addGaussianNoise(message, alpha));
    for (int i = 0; i < n; i++)
    {
        (&result)->setA(distrib(gen), i);
        (&result)->setB((&result)->getB() + (((&result)->getA())[i] * (key->getLWEKey())[i]));
    }

    (&result)->setVariance(alpha * alpha);
    return result;
}

Torus32 lwePhase(LWESample *sample, LWEKey *key)
{
    int32_t n{key->getParams()->getLength()};
    Torus32 axs{0};

    Torus32 *a{sample->getA()};
    int32_t *k{key->getLWEKey()};

    for (int32_t i = 0; i < n; i++)
        axs += sample->getA()[i] * key->getLWEKey()[i];
    return sample->getB() - axs;
}

Torus32 lweDecrypt(LWESample *sample, LWEKey *key, int32_t Msize)
{
    Torus32 phi{lwePhase(sample, key)};
    return approxPhase(phi, Msize);
}

void lweClear(LWESample *result, LWEParams *params)
{
    const int32_t n = params->getLength();

    for (int32_t i = 0; i < n; ++i)
        result->setA(0, i);
    result->setB(0);
    result->setVariance(0.);
}

void lweAdd(LWESample *result, LWESample *sample, LWEParams *params){
    const int32_t n = params->getLength();

    for (int i = 0; i < n; i++)
        result->setA((result->getA())[i] + (sample->getA())[i], i);

    result->setB(result->getB() + sample->getB());
    result->setVariance(result->getCurrentNoise() + sample->getCurrentNoise());
}

void lweSub(LWESample *result, LWESample *sample, LWEParams *params) {
    const int32_t n = params->getLength();
    for (int i = 0; i < n; i++)
        result->setA((result->getA())[i] - (sample->getA())[i], i);

    result->setB(result->getB() - sample->getB());
    result->setVariance(result->getCurrentNoise() - sample->getCurrentNoise());
}

void lweCopy(LWESample *result, LWESample *sample, LWEParams *params) {
    const int32_t n = params->getLength();
    for (int i = 0; i < n; i++)
        result->setA((sample->getA())[i], i);

    result->setB(sample->getB());
    result->setVariance(sample->getCurrentNoise());
}