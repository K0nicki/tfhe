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

// void rotate(TLWESample *result, TLWESample *d0, LWESample *lweSample, int length, GateKey *gateKey)
// {
//     int32_t N2 = 2 * gateKey->getTGSWParams()->getTLWEParams()->getDegree();
//     int32_t k = gateKey->getTGSWParams()->getTLWEParams()->getPolyAmount();
//     TGSWParams *tgswparams = gateKey->getTGSWParams();
//     TLWEParams *tlweParams = tgswparams->getTLWEParams();
//     TLWESample cMuxResult{tgswparams->getTLWEParams()};
//     TLWESample d1{tlweParams};

//     for (int i = 0; i < DEF_n; i++)
//     {
//         tlweClear(&cMuxResult, tlweParams);
//         tlweClear(&d1, tlweParams);

//         int32_t a_til = switchFromTorus32(lweSample->getA(i), N2);

//         if (a_til == 0)
//             continue; // Don't even try to rotate by 0 positions!

//         for (int j = 0; j <= k; j++)
//             polyMulByX_i(d1.getA(j), d0->getA(j), a_til);
//         externalTgswProduct(&cMuxResult, &d1, gateKey->getBootstrappingKey()->getSampleAt(i), tgswparams);
//         tlweAddTo(d0, &cMuxResult, tlweParams);

//         // tlweCopy(d0, &cMuxResult, tlweParams);
//     }

//     tlweCopy(result, d0, tlweParams);
// }

void rotate(TLWESample *result, TLWESample *d0, LWESample *lweSample, int length, GateKey *gateKey)
{
    int32_t N2 = 2 * gateKey->getTGSWParams()->getTLWEParams()->getDegree();
    int32_t k = gateKey->getTGSWParams()->getTLWEParams()->getPolyAmount();
    TGSWParams *tgswparams = gateKey->getTGSWParams();
    TLWEParams *tlweParams = tgswparams->getTLWEParams();

    TLWESample d1{tlweParams};
    TLWESample cMuxResult{tlweParams};

    for (int i = 0; i < DEF_n; i++)
    {
        tlweClear(&cMuxResult, tlweParams);
        tlweClear(&d1, tlweParams);

        // compute ai
        int32_t a_til = switchFromTorus32(lweSample->getA(i), N2);
        // int32_t a_til = (lweSample->getA(i) + (1 << (31 - DEF_Nbit - 1)) >> (32 - DEF_Nbit - 1));

        // if (a_til == 0)
        //     continue; // Don't even try to rotate by 0 positions!

        // prepare rotated testvect by ai position in case of bk(si) is 1
        for (int j = 0; j <= k; j++)
            polyMulByX_i(d1.getA(j), d0->getA(j), a_til);

        // Blind Rotation - keep secret key encrypted in tgsw format but nevertheless perform rotation using CMUX gate
        // bk(si) ? d1 : d0
        cMux(&cMuxResult, gateKey->getBootstrappingKey()->getSampleAt(i), &d1, d0, tgswparams);

        // if (!tlweEQ(d0, &cMuxResult, tlweParams))
        // tlweAddTo(d0, &cMuxResult, tlweParams);

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
            polyMulByX_i(d0.getA(i), testvect->getA(i), 0);
    else
        tlweCopy(&d0, testvect, tlweParams);

    // perform a real bind rotation
    rotate(result, &d0, lweSample, DEF_n, gateKey);
    // for (int i = 0; i < DEF_n; i++)
    // {
    //     TLWESample cMuxResult{tgswparams->getTLWEParams()};
    //     TLWESample d1{tgswparams->getTLWEParams()};
    //     int32_t a_til = switchFromTorus32(lweSample->getA(i), N2);

    //     if (a_til == 0)
    //         return; // Don't even try to rotate by 0 positions!

    //     // Rotate A and B
    //     for (int i = 0; i <= k; i++)
    //         polyMulByX_i(d1.getA(i), d0.getA(i), a_til);

    //     // p ? d1 : d0
    //     cMux(&cMuxResult, gateKey->getBootstrappingKey()->getSampleAt(i), &d1, &d0, tgswparams);

    //     tlweCopy(&d0, &cMuxResult, tlweParams);
    // }

    // tlweCopy(result, &d0, tlweParams);
}


void keySwitch(LWESample* result, LWESample* source, GateKey* gk) {
    TLWEParams *tlweparams = gk->getTGSWParams()->getTLWEParams();
    int32_t N = tlweparams->getDegree();
    int32_t k = tlweparams->getPolyAmount();
    LWEParams *lweparams = tlweparams->getLWEParams();
    int32_t n = lweparams->getLength();
    std::array<std::array<std::array<LWESample, (1U << DEF_basebit) - 1>, DEF_tt>, DEF_N> switchkey = gk->getSwitchKey()->getSwitchKey();

    int32_t offset = 1 << (32 - (1+DEF_basebit*DEF_tt));
    int32_t mask = (1 << DEF_basebit) - 1;

    // result[0] = 0, result[1] = 0, result[2] = source[2]
    lweClear(result, lweparams);
    result->setB(source->getB());

    for (int j = 0; j < N; j++)
    {
        int32_t a_bar = source->getA(j) + offset;

        for (int i = 0; i < DEF_tt; i++)
        {
            int32_t l = (a_bar >> (32 -(i+1)*DEF_basebit)) & mask;

            if (l!=0) {
                for (int m = 0; m < n; m++)
                    result->setA(result->getA(m) - switchkey[j][i][l-1].getA(m),m);                
                result->setB(result->getB() - switchkey[j][i][l-1].getA(n));
            }
        }
    }
}

void bootstrapinglwe2lwe(LWESample *result, LWESample *source, GateKey *gateKey)
{
    TLWEParams *tlweParams = gateKey->getTGSWParams()->getTLWEParams();
    int32_t N = tlweParams->getDegree();
    TLWESample testvect{tlweParams};
    TLWESample tlweresult{tlweParams};

    // testvect[0][*] = 0, testvect[1][*] = 1
    for (int i = 0; i < N; i++)
        testvect.getB()->setCoefficient(i, switchToTorus32(1, 8));

    blindRotate(&tlweresult, &testvect, source, gateKey);

    tlweSampleIndexExtract(result, &tlweresult, 0, tlweParams);
}