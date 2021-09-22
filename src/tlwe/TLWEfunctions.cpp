#include "../../include/tlwe/functions.h"
#include "../../include/general/functions.h"

TLWEKey tlweKeyGen(TLWEParams *params)
{
    int32_t N = params->getDegree();
    int32_t k = params->getPolyAmount();
    TLWEKey key{params};
    std::uniform_int_distribution<int32_t> distrib(0, 1);

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < k; i++)
        for (int j = 0; j < N; j++)
            ((&key)->getKey(i))->setCoefficient(distrib(gen), j);

    return key;
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

TLWESample tlweEncrypt( TorusPolynomial *message, double alpha, TLWEKey *key)
{
    int32_t N = key->getTLWEParams()->getDegree();
    int32_t rCoef;
    int32_t mCoef;
    TLWESample result{key->getTLWEParams()};

    // prepare tlwe Sample object (add random noise)
    tlweBaseEncryption((&result), alpha, key);

    for (int i = 0; i < N; i++)
    {
        rCoef = (&result)->getB()->getCoef(i);
        mCoef = message->getCoef(i);
        (&result)->getB()->setCoefficient(i, rCoef + mCoef);
    }

    return result;
}

TorusPolynomial tlweGetPhase(TLWESample *sample, TLWEKey *key)
{
    int32_t k = key->getTLWEParams()->getPolyAmount();
    int32_t N = key->getTLWEParams()->getDegree();
    TorusPolynomial phase{key->getTLWEParams()->getDegree()};

    // phi = b
    torusPolyCopy((&phase), sample->getB());      

    for (int i = 0; i < k; i++)
    {
        // phi = b - a*s
        torusPolyMulSubFD((&phase), key->getKey(i), sample->getA(i));
    }

    return phase;
}

TorusPolynomial tlweApproxPhase(TorusPolynomial *phase, int32_t M, int32_t N)
{
    TorusPolynomial message{N}; 
    for (int i = 0; i < N; i++)
        (&message)->setCoefficient(i, approxPhase(phase->getCoef(i), M));
    
    return message;
}

TorusPolynomial tlweDecrypt(TLWESample* sample, TLWEKey* key, int32_t M) {
    TorusPolynomial result{key->getTLWEParams()->getDegree()}; 
    result = tlweGetPhase(sample, key);
    result = tlweApproxPhase(&result, M, key->getTLWEParams()->getDegree());

    return result;
}