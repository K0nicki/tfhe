#include "../../include/general/blocks.h"

// res = cs ? d1 : d0
void cMux(TLWESample *result, TGSWSample *cs, TLWESample *d1, TLWESample *d0, TGSWParams *params)
{
    int32_t N = params->getTLWEParams()->getDegree();
    TLWEParams *tlweParams = params->getTLWEParams();
    TLWESample d1md0{params->getTLWEParams()};
    TLWESample csResult{params->getTLWEParams()};

    // d1 - d0
    tlweSub(&d1md0, d1, d0, tlweParams);

    // (d1 - d0) * cs
    externalTgswProduct(&csResult, &d1md0, cs, params);

    // (d1 - d0) * cs + d0
    tlweAdd(result, &csResult, d0, tlweParams);
}

void rotate(TLWESample *result, TLWESample *d1, TLWESample *d0, Torus32 barai, int position, GateKey *gateKey)
{
    int32_t N2 = 2 * gateKey->getTGSWParams()->getTLWEParams()->getDegree();
    int32_t k = gateKey->getTGSWParams()->getTLWEParams()->getPolyAmount();
    TGSWParams *tgswparams = gateKey->getTGSWParams();
    TLWEParams *tlweParams = tgswparams->getTLWEParams();
    TLWESample cMuxResult{tgswparams->getTLWEParams()};

    int32_t a_til = switchFromTorus32(barai, N2);

    if (a_til == 0)
        return; // Don't even try to rotate by 0 positions!

    // Rotate A and B
    for (int i = 0; i <= k; i++)
        polyMulByX_i(d1->getA(i), d0->getA(i), a_til);

    // p ? d1 : d0
    cMux(&cMuxResult, gateKey->getBootstrappingKey()->getSampleAt(position), d1, d0, tgswparams);

    tlweCopy(result, &cMuxResult, tlweParams);
}

void blindRotate(TLWESample *result, TLWESample *testvect, LWESample *lweSample, GateKey *gateKey)
{
    TGSWParams *tgswparams = gateKey->getTGSWParams();
    TLWEParams *tlweParams = tgswparams->getTLWEParams();
    int32_t N2 = 2 * gateKey->getTGSWParams()->getTLWEParams()->getDegree();
    int32_t k = gateKey->getTGSWParams()->getTLWEParams()->getPolyAmount();
    int32_t lweB = lweSample->getB();

    int32_t barab = switchFromTorus32(lweB, N2);
    int32_t b_til = N2 - barab;

    TLWESample d0{tlweParams};

    // Rotate A and B
    if (barab != 0 )     
        for (int i = 0; i <= k; i++)
            polyMulByX_i(d0.getA(i), testvect->getA(i), b_til);

    for (int i = 1; i < DEF_n; i++)
        rotate(&d0, new TLWESample{tgswparams->getTLWEParams()}, &d0, lweSample->getA(i), i, gateKey);

    tlweCopy(result, &d0, tlweParams);
}

void bootstrapinglwe2lwe(LWESample *result, LWESample *source, GateKey *gateKey)
{
    TLWEParams *tlweParams = gateKey->getTGSWParams()->getTLWEParams();
    TLWESample testvect{tlweParams};
    TLWESample tlweresult{tlweParams};

    // testvect[0][*] = 0, testvect[1][*] = 1
    for (int i = 0; i < tlweParams->getDegree(); i++)
        testvect.getB()->setCoefficient(i, switchToTorus32(1, 8));

    blindRotate(&tlweresult, &testvect, source, gateKey);

    tlweSampleIndexExtract(result, &tlweresult, 0, tlweParams);
}
