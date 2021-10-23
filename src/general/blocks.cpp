#include "../../include/general/blocks.h"

// res = cs ? d1 : d0
void cMux(TLWESample *result, TGSWSample *cs, TLWESample *d1, TLWESample *d0, TGSWParams *params)
{
    int32_t N = params->getTLWEParams()->getDegree();
    TLWEParams* tlweParams = params->getTLWEParams();
    TLWESample d1md0{params->getTLWEParams()};
    TLWESample csResult{params->getTLWEParams()};

    // d1 - d0
    tlweSub(&d1md0, d1, d0, tlweParams);

    // (d1 - d0) * cs
    externalTgswProduct(&csResult, &d1md0, cs, params);

    // (d1 - d0) * cs + d0
    tlweAdd(result, &csResult, d0, tlweParams);
}

void blindRotate(TLWESample *result, TLWESample *source, LWESample *lweSample, GateKey *gateKey)
{
    // lwe[DEF_n]
    int32_t lweDef_n = *(lweSample->getA() + DEF_n);
    int32_t b_til = 2 * DEF_N - (lweDef_n + (1 << (31 - DEF_Nbit - 1)) >> (32 - DEF_Nbit - 1));
    TGSWParams *tgswparams = gateKey->getTGSWParams();

    TLWESample d0{tgswparams->getTLWEParams()};

    // d0.initSampleWthZero();

    polyMulByX_i(d0.getA(0), source->getA(0), b_til);
    polyMulByX_i(d0.getB(), source->getB(), b_til);

    for (int i = 0; i < DEF_n; i++)
    {
        TLWESample d1{tgswparams->getTLWEParams()};

        uint32_t a_til = (lweSample->getA(i) + (1 << (31 - DEF_Nbit - 1)) >> (32 - DEF_Nbit - 1));

        polyMulByX_i(d1.getA(0), d0.getA(0), a_til);
        polyMulByX_i(d1.getB(), d0.getB(), a_til);


        // p ? d1 : d0
        cMux(result, gateKey->getBootstrappingKey()->getSampleAt(i), &d1, &d0, tgswparams);

        d0 = *result;
    }
}

void bootstrapinglwe2lwe(LWESample *result, LWESample *source, GateKey *gateKey)
{
    TLWEParams *tlweParams = gateKey->getTGSWParams()->getTLWEParams();
    TLWESample testvect{tlweParams};
    TLWESample tlweresult{tlweParams};

    // testvect[0][*] = 0, testvect[1][*] = 1
    for (int i = 0; i < tlweParams->getDegree(); i++)
        testvect.getA(1)->setCoefficient(i, 1);

    blindRotate(&tlweresult, &testvect, source, gateKey);

    tlweSampleIndexExtract(result, &tlweresult,0,tlweParams);
}
