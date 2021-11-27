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
        LWESample resultKeySwitching(tlweParams->getLWEParams());
        TLWESample testvect{tlweParams};
        int32_t N = tlweParams->getDegree();
        TGSWKey tgswkey{params};
        tgswKeyGen(&tgswkey);

        GateKey gk{&tgswkey, &lwekey};

        LWESample resultExtraction(tlweParams->getLWEParams());
        TLWESample resultRotation(tlweParams);
        Torus32 message = 0;
        Torus32 decrypt = 0;
        err = 0;

        for (int trial = 0; trial < trials; trial++)
        {
            bool binMsg = rand() % 2;
            message = binMsg ? switchToTorus32(1, M) : -switchToTorus32(1, M);
            // Torus32 message{switchToTorus32(trial, M)};
            sample = lweEncrypt(&message, DEF_TRLWE_ALPHA, &lwekey);

            // // testvect[0][*] = 0, testvect[1][*] = 0, testvect[2][*] = 1
            for (int i = 0; i < N; i++)
                // testvect.getB()->setCoefficient(i, switchToTorus32(i, 2 * N));
                // testvect.getB()->setCoefficient(i, switchToTorus32(i, 2*N));
            testvect.getB()->setCoefficient(i, switchToTorus32(1, M));

            blindRotate(&resultRotation, &testvect, &sample, &gk);
            tlweSampleIndexExtract(&resultExtraction, &resultRotation, 0, tlweParams);

            decrypt = lweDecrypt(&resultExtraction, tgswkey.getTLWEKey(), M);
            if (message != decrypt)
                err++;
            std::cout << message << " v " << decrypt << " = " << t32tod(decrypt) << std::endl;
            // std::cout << t32tod(message) << " v " << t32tod(decrypt) << std::endl;
            
            assert(message == decrypt);
        }
    }
    std::cout << "Total errors: " << err << std::endl;
    std::cout << "Blind Rotation test - PASS!" << std::endl;

    return 0;
}
