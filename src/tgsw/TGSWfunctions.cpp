#include "../../include/tgsw/functions.h"
#include "../../include/general/polyfunctions.h"

void tgswKeyGen(TGSWKey *result)
{
    result->setTLWEKey(&tlweKeyGen(result->getTLWEKey()->getTLWEParams()));
}

TGSWSample *tgswEncryptZero(TGSWSample *sample, double alpha, TGSWKey *key)
{
    TLWEKey *tlweKey = key->getTLWEKey();
    int32_t k = tlweKey->getTLWEParams()->getPolyAmount();
    for (int i = 0; i < k; i++)
        tlweBaseEncryption(&sample->getSampleAt(i), alpha, tlweKey);
    return sample;
}

TGSWSample *tgswAddMxH(TGSWSample *sample, int32_t msg, TGSWParams *params)
{
    int32_t k = params->getTLWEParams()->getPolyAmount();
    int32_t N = params->getTLWEParams()->getDegree();
    int32_t l = params->getDecompositionLength();
    Torus32 *h = params->getBgbitPowers();

    for (int i = 0; i < l; i++)
    {
        sample->getSampleAt(i).getA(0)->setCoefficient(0, sample->getSampleAt(i).getA(0)->getCoef(0) + msg * h[i]);
        sample->getSampleAt(i + l).getA(1)->setCoefficient(0, sample->getSampleAt(i).getA(1)->getCoef(0) + msg * h[i]);
    }
    return sample;
}

TGSWSample tgswEncryptInt(TGSWSample *sample, int32_t message, double alpha, TGSWKey *key)
{
    sample = tgswEncryptZero(sample, alpha, key);
    sample = tgswAddMxH(sample, message, key->getTGSWparams());
}

TGSWSampleFft tgswEncryptFft(TGSWSample *sample, int32_t msg, double alpha, TGSWKey *key)
{
    TGSWSampleFft result{sample};
    int32_t n = (&result)->getSamplesSize();

    sample = &(tgswEncryptInt(sample, msg, alpha, key));
    // *sample = tgswEncryptInt(sample, msg, alpha, key);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < 2; j++)
            torusPolyfft<DEF_N>((&result)->getPoly(i, j), sample->getSampleAt(i).getA(j)->getCoefAsArray());

    return result;
}

template <int32_t N = DEF_N, int32_t l = DEF_l, int32_t bg = DEF_Bg>
std::array<std::array<uint32_t, N>, 2 * l> decomposition(TGSWSample *sample, TGSWKey *key)
{
    std::array<std::array<uint32_t, N>, 2 * l> decompVect, 
    int32_t mask = key->getTGSWparams()->getMask();
    int32_t bgBit = key->getTGSWparams()->getBgbit();
    int32_t halfBg = (1UL << (key->getTGSWparams()->getBg() - 1));
    uint32_t offset = key->getTGSWparams()->getOffset();

    for (int i = 0; i < N; i++)
    {
        uint32_t tmp0 = sample->getSampleAt(0).getA(i) + offset;
        uint32_t tmp1 = sample->getSampleAt(1).getA(i) + offset;

        for (int j = 0; j < l; j++)
        {
            decompVect[i][j] = ((tmp0 >> (32 - (i + 1) * bgBit)) & mask) - halfBg;
            decompVect[i + l][j] = ((tmp1 >> (32 - (i + 1) * bgBit)) & mask) - halfBg;
        }
    }

    return decompVect;
}

template <int32_t N = DEF_N, int32_t l = DEF_l, int32_t bg = DEF_Bg>
std::array<std::array<double,N>,2*l> decompositionFft(TGSWSample *sample, TGSWKey *key) {
    std::array<FftPoly,2*l> decompVectFft;

    std::array<std::array<uint32_t, N>, 2 * l> result = decomposition(sample, key);

    for (int i = 0; i < 2*l; i++)
        torusPolyfft(decompVectFft.at(i),result);
    
    return decompVectFft;
}

