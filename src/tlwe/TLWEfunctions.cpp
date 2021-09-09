#include "../../include/tlwe/functions.h"
#include "../../include/general/functions.h"

void tlweKeyGen(TLWEKey *result)
{
    int32_t N = result->getTLWEParams()->getDegree();
    int32_t k = result->getTLWEParams()->getPolyAmount();
    std::uniform_int_distribution<int32_t> distrib(0, 1);

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < k; i++)
        for (int j = 0; j < N; j++)
            (result->getKey(i))->setCoefficient(distrib(gen), j);
}

void tlweBaseEncryption(TLWESample *result, double alpha, TLWEKey *key)
{
    int32_t N = key->getTLWEParams()->getDegree();
    int32_t k = key->getTLWEParams()->getPolyAmount();
    Torus32 msgZero = 0;

    for (int i = 0; i < N; i++)
        result->getB()->setCoefficient(i, addGaussianNoise(&msgZero, alpha));

    for (int i = 0; i < k; i++)
    {
        torusPolyMakeRandom(result->getA(i));
        torusPolyMulFD(result->getB(), key->getKey(i), result->getA(i));
    }
    result->setNoise(alpha * alpha);
}

void tlweEncrypt(TLWESample *result, TorusPolynomial *message, double alpha, TLWEKey *key)
{
    int32_t N = key->getTLWEParams()->getDegree();
    int32_t rCoef;
    int32_t mCoef;

    // prepare tlwe Sample object (add random noise)
    tlweBaseEncryption(result, alpha, key);

    for (int i = 0; i < N; i++)
    {
        rCoef = result->getB()->getCoef(i);
        mCoef = message->getCoef(i);
        result->getB()->setCoefficient(i, rCoef + mCoef);
    }
}

void tlweGetPhase(TorusPolynomial *phase, TLWESample *sample, TLWEKey *key)
{
    int32_t k = key->getTLWEParams()->getPolyAmount();
    int32_t N = key->getTLWEParams()->getDegree();

    // phi = b
    torusPolyCopy(phase, sample->getB());      

    for (int i = 0; i < k; i++)
    {
        // phi = b - a*s
        torusPolyMulSubFD(phase, key->getKey(i), sample->getA(i));
    }
}

void tlweApproxPhase(TorusPolynomial *message, TorusPolynomial *phase, int32_t M, int32_t N)
{
    for (int i = 0; i < N; i++)
        message->setCoefficient(i, approxPhase(phase->getCoef(i), M));
}

void tlweDecrypt(TorusPolynomial* result, TLWESample* sample, TLWEKey* key, int32_t M) {
    tlweGetPhase(result, sample, key);
    tlweApproxPhase(result, result, M, key->getTLWEParams()->getDegree());
}