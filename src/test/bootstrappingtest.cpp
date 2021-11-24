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
        TGSWKey tgswkey{params};
        tgswKeyGen(&tgswkey);

        GateKey gk{&tgswkey, &lwekey};

        err = 0;

        for (int j = 0; j < trials; j++)
        {
            LWESample extractionResult(tlweParams->getLWEParams());
            LWESample result(tlweParams->getLWEParams());

            bool binMsg = rand() % 2;
            Torus32 message{binMsg ? switchToTorus32(1, M) : -switchToTorus32(1, M)};
            sample = lweEncrypt(&message, DEF_TLWE_ALPHA, &lwekey);

            bootstrapinglwe2lwe(&result, &sample, &gk);

            // TLWESample testvect{tlweParams};
            // TLWESample tlweresult{tlweParams};
            // // testvect[0][*] = 0, testvect[1][*] = 1
            // for (int i = 0; i < DEF_N; i++)
            //     testvect.getB()->setCoefficient(i, switchToTorus32(1, 8));

            // blindRotate(&tlweresult, &testvect, &sample, &gk);
            // tlweSampleIndexExtract(&extractionResult, &tlweresult, 0, tlweParams);

            // Torus32 decrypt = lweDecryptN(&extractionResult, tgswkey.getTLWEKey(), M);

            // if (message != decrypt)
            // {
            //     // std::cout << binMsg << " v " << decrypt << std::endl;
            //     err++;
            // }
            // // std::cout << message << " v " << decrypt << std::endl;

            // assert(message == decrypt);
            // std::cout << "|" << j << "| "
            //           << "Successfully rotated!\n"
            //           << std::endl;

            // keySwitch(&result, &extractionResult, &gk);
            Torus32 decrypt = lweDecrypt(&result, &lwekey, M);

            if (message != decrypt)
            {
                // std::cout << binMsg << " v " << decrypt << std::endl;
                err++;
            }
            std::cout << message << " v " << decrypt << std::endl;

            // assert(message == decrypt);
            // std::cout << "Successfully bootstrapped!\n" << std::endl;
        }
    }
    std::cout << "Total errors: " << err << std::endl;

    return 0;
}
