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

        for (int j = 0; j < 1; j++)
        {
            int32_t binMsg = j;
            // int32_t binMsg = 1;
            // int32_t binMsg = distrib(gen) ? 1 : -1;
            // int32_t myMSG = binMsg ? 1 : -1;
            // int32_t myMSG = binMsg;
            Torus32 message{switchToTorus32(binMsg, M)};
            sample = lweEncrypt(&message, DEF_TLWE_ALPHA, &lwekey);

            // testvect[0][*] = 0, testvect[1][*] = 0, testvect[2][*] = 1
            for (int i = 0; i < N; i++)
            {
                testvect.getB()->setCoefficient(i, approxPhase(switchToTorus32(i, N),M));
                // testvect.getB()->setCoefficient(i, switchFromTorus32(switchToTorus32(i, N), M));
                std::cout << "|" << testvect.getB()->getCoef(i);
            }    
            std::cout << std::endl;

        }
    }
    std::cout << "Total errors: " << err << std::endl;

    return 0;
}
