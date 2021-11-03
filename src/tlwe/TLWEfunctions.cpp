#include "../../include/tlwe/functions.h"
#include "../../include/general/functions.h"
#include "../../include/general/polyfunctions.h"

void tlweKeyGen(TLWEKey *key, TLWEParams *params)
{
    int32_t N = params->getDegree();
    int32_t k = params->getPolyAmount();
    std::uniform_int_distribution<int32_t> distrib(0, 1);

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < k; i++)
        for (int j = 0; j < N; j++)
            (key->getIntKey(i))->setCoefficient(j, distrib(gen));
}

void tlweBaseEncryption(TLWESample *result, double alpha, TLWEKey *key)
{
    int32_t N = key->getTLWEParams()->getDegree();
    int32_t k = key->getTLWEParams()->getPolyAmount();
    Torus32 msgZero = 0;

    // Add noise around 0
    for (int i = 0; i < N; i++)
        result->getB()->setCoefficient(i, addGaussianNoise(&msgZero, alpha));

    for (int i = 0; i < k; i++)
    {
        // Random mask
        torusPolyMakeRandom(result->getA(i));
        // a * s + err
        torusPolyMulFD(result->getB(), key->getIntKey(i), result->getA(i));
    }
    result->setNoise(alpha * alpha);
}

TLWESample tlweEncrypt(TorusPolynomial *message, double alpha, TLWEKey *key)
{
    int32_t N = key->getTLWEParams()->getDegree();
    int32_t rCoef;
    int32_t mCoef;
    TLWESample result{key->getTLWEParams()};

    // prepare tlwe Sample object (add random noise)
    // a*s + err
    tlweBaseEncryption((&result), alpha, key);

    for (int i = 0; i < N; i++)
    {
        rCoef = (&result)->getB()->getCoef(i);
        mCoef = message->getCoef(i);
        // a*s + err + m
        (&result)->getB()->setCoefficient(i, rCoef + mCoef);
    }

    return result;
}

// result = result + p*sample;
void tLweAddMulRTo(TLWESample *result, IntPolynomial *p, TLWESample *sample, TLWEParams *params)
{
    int32_t k = params->getPolyAmount();
    const int32_t N = params->getDegree();

    for (int i = 0; i <= k; i++)
        torusPolyMulFD(result->getA(i), p, sample->getA(i));
    result->setNoise(result->getNoise() + intPolySq2(p) * sample->getNoise());
}

void tlweSubTo(TLWESample *a, TLWESample *b, TLWEParams *params)
{
    int32_t N = params->getDegree();
    int32_t k = params->getPolyAmount();

    for (int i = 0; i < k; i++)
        torusPolySubTo(a->getA(i), b->getA(i));
    torusPolySubTo(a->getB(), b->getB());
    a->setNoise(a->getNoise() + b->getNoise());
}

void tlweSub(TLWESample *result, TLWESample *a, TLWESample *b, TLWEParams *params)
{
    int32_t N = params->getDegree();
    int32_t k = params->getPolyAmount();

    for (int i = 0; i < k; i++)
        torusPolySub(result->getA(i), a->getA(i), b->getA(i));
    torusPolySub(result->getB(), a->getB(), b->getB());

    a->setNoise(a->getNoise() + b->getNoise());
}

void tlweAddTo(TLWESample *a, TLWESample *b, TLWEParams *params)
{
    int32_t N = params->getDegree();
    int32_t k = params->getPolyAmount();

    for (int i = 0; i < k; i++)
        torusPolyAddTo(a->getA(i), b->getA(i));
    torusPolyAddTo(a->getB(), b->getB());
    a->setNoise(a->getNoise() + b->getNoise());
}

void tlweAdd(TLWESample *result, TLWESample *a, TLWESample *b, TLWEParams *params)
{
    int32_t N = params->getDegree();
    int32_t k = params->getPolyAmount();

    for (int i = 0; i < k; i++)
        torusPolyAdd(result->getA(i), a->getA(i), b->getA(i));
    torusPolyAdd(result->getB(), a->getB(), b->getB());

    result->setNoise(a->getNoise() + b->getNoise());
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
        torusPolyMulSubFD((&phase), key->getIntKey(i), sample->getA(i));
    }

    return phase;
}

void tlweCopy(TLWESample *result, TLWESample *sample, TLWEParams *params)
{
    int32_t k = params->getPolyAmount();
    int32_t N = params->getDegree();

    for (int i = 0; i <= k; i++)
        for (int j = 0; j < N; j++)
            result->getA(i)->setCoefficient(j, sample->getA(i)->getCoef(j));

    result->setNoise(sample->getNoise());
}

TorusPolynomial tlweApproxPhase(TorusPolynomial *phase, int32_t M, int32_t N)
{
    TorusPolynomial message{N};
    for (int i = 0; i < N; i++)
        (&message)->setCoefficient(i, approxPhase(phase->getCoef(i), M));

    return message;
}

TorusPolynomial tlweDecrypt(TLWESample *sample, TLWEKey *key, int32_t M)
{
    TorusPolynomial result{key->getTLWEParams()->getDegree()};
    result = tlweGetPhase(sample, key);
    result = tlweApproxPhase(&result, M, key->getTLWEParams()->getDegree());

    return result;
}

// Poprawne? Napisac test
// Tak, powinno byc ok - rozszerzam o k wielomianów
void tlweSampleIndexExtract(LWESample *result, TLWESample *x, int32_t index, TLWEParams *params)
{
    int32_t N = params->getDegree();
    int32_t k = params->getPolyAmount();

    // for (int i = 0; i < k; i++)
    // {
    //     for (int j = 0; j <= index; j++)
    //         result->setA(x->getA(i)->getCoef(index), i * N + j);
    //     for (int j = index + 1; j < N; j++)
    //         result->setA(-x->getA(i)->getCoef(N + index - j), i * N + j);
    // }

    for (int i = 0; i <= index; i++)
        result->setA(x->getA(0)->getCoef(index - i), i);
    // for (int i = (index + 1); i < N; i++)
    //     result->setA(-x->getA(0)->getCoef(N + index - i), i);

    result->setB(x->getB()->getCoef(index));
}