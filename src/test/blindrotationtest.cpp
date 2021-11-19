#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <cassert>

int main(int argc, char const *argv[])
{
    std::uniform_int_distribution<int32_t> distrib(0, 1);
    std::random_device rd;
    std::mt19937 gen(rd());

    LWEParams params1024(DEF_N, 0., 1.);
    LWEKey lwekey(&params1024);
    lweKeyGen(&lwekey);

    TLWEParams params1024_1{DEF_N, 1, 0., 1.};
    TGSWParams tgswParams1024_1{&params1024_1};

    std::vector<TGSWParams *> tgswparams1024 = {&tgswParams1024_1};
    int trials = 10;
    int32_t M = 8;
    int err = 0;

    for (TGSWParams *params : tgswparams1024)
    {
        TLWEParams *tlweParams = params->getTLWEParams();
        LWESample sample(tlweParams->getLWEParams());
        LWESample resultKeySwitching(tlweParams->getLWEParams());
        TLWESample testvect{tlweParams};
        int32_t N = tlweParams->getDegree();
        TGSWKey tgswkey{params};
        tgswKeyGen(&tgswkey);

        GateKey gk{&tgswkey, &lwekey};

        err = 0;

        for (int j = 0; j < trials; j++)
        {
            int32_t binMsg = j;
            // int32_t binMsg = 1;
            // int32_t binMsg = distrib(gen) ? 1 : -1;
            // int32_t myMSG = binMsg ? 1 : -1;
            // int32_t myMSG = binMsg;
            Torus32 message{switchToTorus32(binMsg, M)};
            sample = lweEncrypt(&message, DEF_TRLWE_ALPHA, &lwekey);

            // testvect[0][*] = 0, testvect[1][*] = 0, testvect[2][*] = 1
            for (int i = 0; i < N; i++)
                // testvect.getB()->setCoefficient(i, i);
                testvect.getB()->setCoefficient(i, approxPhase(switchToTorus32(i, N),M));
            // testvect.getB()->setCoefficient(i, switchToTorus32(1, M));

            TLWESample resultRotation(tlweParams);
            // blindRotate(&resultRotation, &testvect, &sample, &gk);

            TGSWParams *tgswparams = gk.getTGSWParams();
            TLWEParams *tlweParams = tgswparams->getTLWEParams();
            int32_t N2 = 2 * gk.getTGSWParams()->getTLWEParams()->getDegree();
            int32_t k = gk.getTGSWParams()->getTLWEParams()->getPolyAmount();

            // To perform rotation correctly we need to rotate tlwesample by -b + sum(ai * bk(si))
            int32_t barab = switchFromTorus32(sample.getB(), N2);
            int32_t b_til = N2 - barab; // compute -b

            TLWESample d0{tlweParams};
            TLWESample d0_ncmux{tlweParams};
            // TLWESample d1{tlweParams};
            // TLWESample cMuxResult{tlweParams};

            // Rotate testvect by well known -b parameter
            if (barab != 0)
                for (int i = 0; i <= k; i++)
                    polyMulByX_i(d0.getA(i), testvect.getA(i), b_til);
            else
                tlweCopy(&d0, &testvect, tlweParams);


            if (barab != 0)
                for (int i = 0; i <= k; i++)
                    polyMulByX_i(d0_ncmux.getA(i), testvect.getA(i), b_til);
            else
                tlweCopy(&d0_ncmux, &testvect, tlweParams);

            // perform a real bind rotation
            for (int i = 0; i < DEF_n; i++)
            {
                // tlweClear(&d1, params->getTLWEParams());
                // tlweClear(&cMuxResult, params->getTLWEParams());
                TLWESample d1{tlweParams};
                TLWESample cMuxResult{tlweParams};
                TLWESample d1_ncmux{tlweParams};
                TLWESample ncmuxResult{tlweParams};

                // compute ai
                int32_t a_til = switchFromTorus32(sample.getA(i), N2);

                if (a_til == 0)
                    continue; // Don't even try to rotate by 0 positions!

                // prepare rotated testvect by ai position in case of bk(si) is 1
                for (int j = 0; j <= k; j++)
                    polyMulByX_i(d1.getA(j), d0.getA(j), a_til);
                // Sth is wrong with a_til rotation - it rotates poly by strange positions - reason?

                // Blind Rotation - keep secret key encrypted in tgsw format but nevertheless perform rotation using CMUX gate
                // bk(si) ? d1 : d0
                cMux(&cMuxResult, gk.getBootstrappingKey()->getSampleAt(i), &d1, &d0, params);


                // externalTgswProduct(&cMuxResult, &d1, gk.getBootstrappingKey()->getSampleAt(i), tgswparams);
                // tlweAddTo(&d1, &d0, tlweParams);

                // -----------------------------------------------------------------------------
                //                          CMUX is not working correctly
                // -----------------------------------------------------------------------------
                // Torus32 cMuxRes = tlweDecryptT(&cMuxResult, tgswkey.getTLWEKey(), M);
                // Torus32 d0Res = tlweDecryptT(&d0, tgswkey.getTLWEKey(), M);
                // Torus32 d1Res = tlweDecryptT(&d1, tgswkey.getTLWEKey(), M);

                
                for (int j = 0; j <= k; j++)
                    polyMulByX_i(d1_ncmux.getA(j), d0_ncmux.getA(j), a_til);
                ncmuxResult = lwekey.getLWEKey()[i] ? d1_ncmux : d0_ncmux;

                TorusPolynomial cMuxRes = tlweDecrypt(&ncmuxResult, tgswkey.getTLWEKey(), M);
                TorusPolynomial d0Res = tlweDecrypt(&d0_ncmux, tgswkey.getTLWEKey(), M);
                TorusPolynomial d1Res = tlweDecrypt(&d1_ncmux, tgswkey.getTLWEKey(), M);

                if (!torusPolyEQ(&cMuxRes, lwekey.getLWEKey()[i] ? &d1Res : &d0Res))
                // if (cMuxRes != (lwekey.getLWEKey()[i] ? d1Res : d0Res))
                {
                    //  if (cMuxRes != (lwekey.getLWEKey()[i] ? d1Res : d0Res))
                    //     {
                    //         std::cout << "|" << j << "," << i << "| " << cMuxRes << " v "
                    //                   << lwekey.getLWEKey()[i] << " ? " << d1Res
                    //                   << " : " << d0Res << std::endl;
                    //     }
                    for (int l = 0; l < N; l++)
                    {
                        if (cMuxRes.getCoef(l) != (lwekey.getLWEKey()[i] ? d1Res.getCoef(l) : d0Res.getCoef(l)))
                        {
                            std::cout << "|" << j << "," << i << "," << l << "| " << cMuxRes.getCoef(l) << " v "
                                      << lwekey.getLWEKey()[i] << " ? " << d1Res.getCoef(l)
                                      << " : " << d0Res.getCoef(l) << std::endl;
                        }
                    }
                    err++;
                }

                // Override testvect with rotated (or not) testvect
                tlweCopy(&d0, &cMuxResult, tlweParams);
                tlweCopy(&d0_ncmux, &ncmuxResult, tlweParams);
                // TLWESample *tmp = new TLWESample(tlweParams);
                // tlweCopy(tmp, &d1, tlweParams);
                // tlweCopy(&d1, &d0, tlweParams);
                // tlweCopy(&d0, tmp, tlweParams);
                // delete tmp;
            }

            tlweCopy(&resultRotation, &d0_ncmux, tlweParams);

            // TorusPolynomial torusMsg = tlweDecrypt(&resultRotation, tgswkey.getTLWEKey(), M);

            // for (int i = 0; i < N; i++)
            //     if (torusMsg.getCoef(i) != message)
            //         std::cout << torusMsg.getCoef(i) << " v " << message << std::endl;

            LWESample resultExtraction(tlweParams->getLWEParams());
            tlweSampleIndexExtract(&resultExtraction, &resultRotation, 0, tlweParams);

            // -----------------------------------------------------------------------------
            //                          LWESample extraction test - PASSED
            // -----------------------------------------------------------------------------
            // LWESample *lweSampleTest = new LWESample(&params1024);
            // TLWESample tlweSampleTest = tlweEncryptT(switchToTorus32(1,4), DEF_TRLWE_ALPHA, tgswkey.getTLWEKey());
            // tlweSampleIndexExtract(lweSampleTest, &tlweSampleTest, 0, tgswkey.getTGSWparams()->getTLWEParams());

            // if (lweDecryptN(lweSampleTest, tgswkey.getTLWEKey(), 4) != switchToTorus32(1,4))
            //     std::cout << "Extraction error!\n";

            // Torus32 phase = lwePhase(&sample, &lwekey);
            // Torus32 phase = lwePhaseN(&resultExtraction, tgswkey.getTLWEKey());
            // std::cout << "phase " << j << " = " << t32tod(phase)  << " v " << t32tod(lwePhase(&sample, &lwekey)) << std::endl;

            Torus32 decrypt = lweDecryptN(&resultExtraction, tgswkey.getTLWEKey(), M);
            // Torus32 decrypt = lweDecrypt(&resultExtraction, &lwekey, M);

            if (message != decrypt)
            // if (decrypt != (message >= 0 ? switchToTorus32(1, M) : -switchToTorus32(1, M)))
            {
                // std::cout << message << " v " << decrypt << std::endl;
                // err++;
            }
            // std::cout << (message >= 0 ? switchToTorus32(1, M) : -switchToTorus32(1, M)) << " v " << decrypt << std::endl;
            std::cout << message << " v " << decrypt << std::endl;

            // assert(message == decrypt);
        }
    }
    std::cout << "Total errors: " << err << std::endl;

    return 0;
}
