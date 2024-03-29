#include "../../include/tgsw/functions.h"
#include "../../include/general/polyfunctions.h"

void tgswKeyGen(TGSWKey *result)
{
    TLWEKey *lkey = result->getTLWEKey();
    TLWEParams *lparams = result->getTGSWparams()->getTLWEParams();

    tlweKeyGen(lkey, lparams);
    result->setTLWEKey(result->getTLWEKey());
}

TGSWSample *tgswEncryptZero(TGSWSample *sample, double alpha, TGSWKey *key)
{
    TLWEKey *tlweKey = key->getTLWEKey();

    // Base tlwe encryption (a*s + err) for each TLWESample in array
    for (TLWESample *tlwesample : sample->getCoefAsArray())
        tlweBaseEncryption(tlwesample, alpha, tlweKey);

    return sample;
}

TGSWSample *tgswAddMxH(TGSWSample *sample, int32_t msg, TGSWParams *params)
{
    int32_t l = params->getDecompositionLength();
    std::array<uint32_t, DEF_l> h = params->getBgbitPowers();

    for (int i = 0; i < l; i++)
    {
        uint32_t A00 = sample->getSampleAt(i)->getA(0)->getCoef(0);
        uint32_t A10 = sample->getSampleAt(i + l)->getB()->getCoef(0);

        sample->getSampleAt(i)->getA(0)->setCoefficient(0, A00 + msg * h[i]);
        sample->getSampleAt(i + l)->getB()->setCoefficient(0, A10 + msg * h[i]);
    }

    return sample;
}

TGSWSample *tgswAddMxHInt(TGSWSample *sample, IntPolynomial *msg, TGSWParams *params)
{
    int32_t l = params->getDecompositionLength();
    int32_t N = params->getTLWEParams()->getDegree();
    int32_t k = params->getTLWEParams()->getPolyAmount();
    std::array<uint32_t, DEF_l> h = params->getBgbitPowers();

    for (int bloc = 0; bloc <= k; bloc++)
        for (int i = 0; i < l; i++)
        {
            Torus32 *target = sample->getSampleAt(i)->getA(bloc)->getCoefAsArray()->data();
            Torus32 hi = h[i];
            for (int j = 0; j < N; j++)
                target[j] = msg->getCoef(j) * hi;
        }

    return sample;
}

void tgswEncryptIntPoly(TGSWSample *sample, IntPolynomial *message, double alpha, TGSWKey *key)
{
    sample = tgswEncryptZero(sample, alpha, key);
    sample = tgswAddMxHInt(sample, message, key->getTGSWparams());
}

void tgswEncryptInt(TGSWSample *sample, int32_t message, double alpha, TGSWKey *key)
{
    sample = tgswEncryptZero(sample, alpha, key);
    sample = tgswAddMxH(sample, message, key->getTGSWparams());
}

template <int32_t N = DEF_N, int32_t l = DEF_l, int32_t bg = DEF_Bg>
std::array<IntPolynomial *, 2 * l> decomposition(TLWESample *sample, TGSWParams *params)
{
    int32_t Bgbit = params->getBgbit();             //log2(decomposition base - power of 2)
    uint32_t mask = params->getMask();              // (1 << BgBit) - 1
    int32_t halfBg = (1UL << (Bgbit - 1));
    // offset = Bg/2 * (2^(32 - BgBit) + 2^(32 - 2*BgBit) + ... + 2^(32 - l*BgBit))
    uint32_t offset = params->getOffset();              

    std::array<IntPolynomial *, 2 * l> decompVect;
    for (int i = 0; i < decompVect.size(); i++)         
        decompVect[i] = new IntPolynomial;          // Initialize with zeros 

    for (int i = 0; i < N; i++)
    {
        uint32_t tmp0 = sample->getA(0)->getCoef(i) + offset;
        uint32_t tmp1 = sample->getB()->getCoef(i) + offset;

        for (int j = 0; j < l; j++)
        {
            uint32_t tmp01 = (tmp0 >> (32 - (j + 1) * Bgbit)) & mask;
            uint32_t tmp11 = (tmp1 >> (32 - (j + 1) * Bgbit)) & mask;

            decompVect.at(j)->setCoefficient(i, tmp01 - halfBg);
            decompVect.at(j + l)->setCoefficient(i, tmp11 - halfBg);
        }
    }

    return decompVect;
}

void externalTgswProduct(TLWESample *result, TLWESample *tlweSample, TGSWSample *tgswSample, TGSWParams *params)
{
    std::array<IntPolynomial *, 2 * DEF_l> decompVect;
    decompVect = decomposition(tlweSample, params);

    for (int i = 0; i < 2 * DEF_l; i++)
        tLweAddMulRTo(result, decompVect.at(i), tgswSample->getSampleAt(i), params->getTLWEParams());
}
