#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <cassert>

int main(int argc, char const *argv[])
{
    LWEParams params1024(DEF_n, 0., 1.);
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
        TLWESample testvect{tlweParams};
        int32_t N = tlweParams->getDegree();
        TGSWKey tgswkey{params};
        tgswKeyGen(&tgswkey);

        GateKey gk{&tgswkey, &lwekey};

        err = 0;

        for (int j = 0; j < trials; j++)
        {
            int32_t binMsg = switchToTorus32(rand() % 8, M);
            // int32_t myMSG = binMsg ? 1 : -1;
            int32_t myMSG = binMsg;
            Torus32 message{switchToTorus32(myMSG, M)};
            sample = lweEncrypt(&message, DEF_TLWE_ALPHA, &lwekey);

            // testvect[0][*] = 0, testvect[1][*] = 0, testvect[2][*] = 1
            for (int i = 0; i < N; i++)
                testvect.getB()->setCoefficient(i, switchToTorus32(1, M));

            // int32_t phase = switchFromTorus32(sample.getB(), N * 2);
            // for (int i = 0; i < params1024.getLength(); i++)
            // {
            //     int32_t barai = switchFromTorus32(sample.getA(i), N * 2);
            //     phase -= lwekey.getLWEKey()[i] * barai;
            // }
            // phase = (N * 2 + (phase % (N * 2))) % (N * 2);

            TLWESample resultRotation(tlweParams);
            blindRotate(&resultRotation, &testvect, &sample, &gk);
            
            LWESample resultExtraction(tlweParams->getLWEParams());
            tlweSampleIndexExtract(&resultExtraction, &resultRotation, 0, tlweParams);

            Torus32 decrypt = lweDecrypt(&resultExtraction, &lwekey, M);

            // if (binMsg != (phase < N ? decrypt : -decrypt) )
            // {
            //     // std::cout << binMsg << " v " << decrypt << std::endl;
            //     err++;
            // }
            // std::cout << binMsg << " v " << decrypt << " v " << (phase < N ? switchToTorus32(1, M) : -switchToTorus32(1, M)) << std::endl;

            if (binMsg != decrypt)
            {
                // std::cout << binMsg << " v " << decrypt << std::endl;
                err++;
            }
            std::cout << binMsg << " v " << decrypt << std::endl;

            // assert(message == decrypt);
        }
    }
    std::cout << "Total errors: " << err << std::endl;

    // std::cout << "\nSecond test" << std::endl;
    // for (TGSWParams *params : tgswparams1024)
    // {
    //     int32_t n = lwekey.getParams()->getLength();
    //     int32_t k = params->getTLWEParams()->getPolyAmount();
    //     TLWEParams *tlweParams = params->getTLWEParams();
    //     LWESample sample(lwekey.getParams());
    //     TLWESample testvect{tlweParams};
    //     int32_t N = params->getTLWEParams()->getDegree();
    //     TorusPolynomial testPoly{N};
    //     TGSWKey tgswkey{params};
    //     tgswKeyGen(&tgswkey);

    //     GateKey gk{&tgswkey, &lwekey};

    //     err = 0;

    //     for (int j = 0; j < trials; j++)
    //     {
    //         int32_t expectedOffset = 0;

    //         TLWESample resultRotation(tlweParams);
    //         TLWESample verifyRotation(tlweParams);
    //         LWESample resultExtraction(tlweParams->getLWEParams());

    //         int32_t binMsg = switchToTorus32(rand() % 8, M);
    //         // int32_t myMSG = binMsg ? 1 : -1;
    //         int32_t myMSG = binMsg;
    //         Torus32 message{switchToTorus32(myMSG, M)};
    //         sample = lweEncrypt(&message, DEF_TLWE_ALPHA, &lwekey);

    //         TorusPolynomial initMsg = sample.getB();
    //         TorusPolynomial expectedResult = sample.getB();

    //         // create bara
    //         int32_t *bara = new int32_t[n];
    //         for (int32_t i = 0; i < n; i++)
    //             bara[i] = switchFromTorus32(sample.getA(i), (2 * N));

    //         // sample.setB(*bara);
    //         // for (int i = 0; i <= tlweParams->getPolyAmount(); i++)
    //         //     torusPolyMakeRandom(&testPoly);

    //         // testvect[0][*] = 0, testvect[1][*] = 0, testvect[2][*] = 1
    //         for (int i = 0; i < N; i++)
    //             testvect.getB()->setCoefficient(i, switchToTorus32(1, M));

    //         blindRotate(&resultRotation, &testvect, &sample, &gk);

    //         for (int i = 0; i < n; i++)
    //         {
    //             expectedOffset = (expectedOffset + bara[i]) % (2 * N);
    //             for (int l = 0; l < k; l++)
    //                 polyMulByX_i(&expectedResult, &initMsg, expectedOffset);
    //         }
    //         for (int i = 0; i < N; i++)
    //             if (expectedResult.getCoef(i) != resultRotation.getB()->getCoef(i))
    //             {
    //                 std::cout << expectedResult.getCoef(i) << " v " << resultRotation.getB()->getCoef(i) << std::endl;
    //                 err++;
    //             }

    //         // tlweSampleIndexExtract(&resultExtraction, &resultRotation, 0, tlweParams);
    //     }

    //     std::cout << "Total errors: " << err << std::endl;
    //     std::cout << "Blind Rotation test passed!" << std::endl;
    // }
    return 0;
}
