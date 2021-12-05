#include "../../include/general/blocks.h"
#include <iostream>

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

void rotate(TLWESample *result, TLWESample *d0, LWESample *lweSample, GateKey *gateKey)
{
    int32_t N2 = 2 * gateKey->getTGSWParams()->getTLWEParams()->getDegree();
    int32_t k = gateKey->getTGSWParams()->getTLWEParams()->getPolyAmount();
    TGSWParams *tgswparams = gateKey->getTGSWParams();
    TLWEParams *tlweParams = tgswparams->getTLWEParams();

    for (int i = 0; i < DEF_N; i++)
    {
        TLWESample d1{tlweParams};
        TLWESample cMuxResult{tlweParams};

        // compute ai
        int32_t a_til = switchFromTorus32(lweSample->getA(i), N2);

        if (a_til == 0)
            continue; // Don't even try to rotate by 0 positions!

        // Prepare rotated testvect by ai position in case of bk(si) is 1
        for (int j = 0; j <= k; j++)
            polyMulByX_i(d1.getA(j), d0->getA(j), a_til);

        // Blind Rotation - keep secret key encrypted in tgsw format but nevertheless perform rotation using CMUX gate
        // bk(si) ? d1 : d0
        cMux(&cMuxResult, gateKey->getBootstrappingKey()->getSampleAt(i), &d1, d0, tgswparams);

        // Override testvect with rotated (or not) testvect
        tlweCopy(d0, &cMuxResult, tlweParams);
    }

    tlweCopy(result, d0, tlweParams);
}

void blindRotate(TLWESample *result, TLWESample *testvect, LWESample *lweSample, GateKey *gateKey)
{
    TGSWParams *tgswparams = gateKey->getTGSWParams();
    TLWEParams *tlweParams = tgswparams->getTLWEParams();
    int32_t N2 = 2 * gateKey->getTGSWParams()->getTLWEParams()->getDegree();
    int32_t k = gateKey->getTGSWParams()->getTLWEParams()->getPolyAmount();

    // To perform rotation correctly we need to rotate tlwesample by -b + sum(ai * bk(si))

    int32_t barab = switchFromTorus32(lweSample->getB(), N2);
    int32_t b_til = N2 - barab; // compute -b

    TLWESample d0{tlweParams};
    TLWESample res{tlweParams};

    // Rotate testvect by well known -b parameter
    if (barab != 0)
        for (int i = 0; i <= k; i++)
            polyMulByX_i(d0.getA(i), testvect->getA(i), b_til);
    else
        tlweCopy(&d0, testvect, tlweParams);

    // Perform a real bind rotation
    rotate(result, &d0, lweSample, gateKey);
}

void keySwitch(LWESample *result, LWESample *source, GateKey *gk)
{
    TLWEParams *tlweparams = gk->getTGSWParams()->getTLWEParams();
    LWEParams *lweparams = tlweparams->getLWEParams();
    int32_t N = tlweparams->getDegree();
    int32_t n = lweparams->getLength();
    int32_t k = tlweparams->getPolyAmount();
    std::array<std::array<std::array<LWESample *, (1U << DEF_basebit)>, DEF_tt>, DEF_N> switchkey = gk->getSwitchKey()->getSwitchKey();

    uint32_t offset = 1 << (32 - (1 + DEF_basebit * DEF_tt));
    uint32_t mask = (1 << DEF_basebit) - 1;

    // result[0] = 0, result[1] = 0, result[2] = source[2]
    lweClear(result, lweparams);
    result->setB(source->getB());

    for (int j = 0; j < DEF_N; j++)
    {
        uint32_t a_bar = source->getA(j) + offset;
        for (int i = 0; i < DEF_tt; i++)
        {
            uint32_t l = (a_bar >> (32 - (i + 1) * DEF_basebit)) & mask;
            if (l != 0)
            {
                lweSubTo(result, switchkey[j][i][l], lweparams);
                // for (int m = 0; m < n; m++)
                //     result->setA(result->getA(m) - switchkey[j][i][l-1]->getA(m), m);
                // result->setB(result->getB() - switchkey[j][i][l-1]->getB());
            }
        }
    }
}

void bootstrapinglwe2lwe(LWESample *result, LWESample *source, GateKey *gateKey)
{
    TLWEParams *tlweParams = gateKey->getTGSWParams()->getTLWEParams();
    int32_t N = tlweParams->getDegree();
    TLWESample testvect{tlweParams};
    TLWESample tlweResult{tlweParams};
    // LWESample extractionResult{tlweParams->getLWEParams()};

    // testvect[0][*] = 0, testvect[1][*] = 1
    for (int i = 0; i < DEF_N; i++)
        testvect.getB()->setCoefficient(i, switchToTorus32(1, 8));

    blindRotate(&tlweResult, &testvect, source, gateKey);

    tlweSampleIndexExtract(result, &tlweResult, 0, tlweParams);
    // keySwitch(result, &extractionResult, gateKey);
}