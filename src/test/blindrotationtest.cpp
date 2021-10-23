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

    TLWEParams params1024_1{1024, 1, 0., 1.};
    TLWEParams params1024_2{1024, 2, 0., 1.};

    TGSWParams tgswParams1024_1{&params1024_1};
    TGSWParams tgswPrams1024_2{&params1024_2};

    std::vector<TGSWParams *> tgswparams1024 = {&tgswParams1024_1};
    int trials = 10;
    int32_t M = 8;

    for (TGSWParams *params : tgswparams1024)
    {
        TLWEParams* tlweParams = params->getTLWEParams();
        LWESample sample(lwekey.getParams());
        TLWESample testvect{tlweParams};
        TLWESample resultRotation(tlweParams);
        LWESample resultExtraction(tlweParams->getLWEParams());
        TGSWKey tgswkey{params};
        GateKey gk{&tgswkey, &lwekey};

        tgswKeyGen(&tgswkey);
        

        for (int j = 0; j < trials; j++)
        {
            Torus32 message{switchToTorus32(0, M)};
            sample = lweEncrypt(&message, DEF_TLWE_ALPHA, &lwekey);

            // testvect[0][*] = 0, testvect[1][*] = 0, testvect[2][*] = 1
            for (int i = 0; i < tlweParams->getDegree(); i++)
                testvect.getB()->setCoefficient(i, 1);

            blindRotate(&resultRotation, &testvect, &sample, &gk);

            tlweSampleIndexExtract(&resultExtraction, &resultRotation, 0, tlweParams);

            Torus32 decrypt = lweDecrypt(&resultExtraction, &lwekey, M);

            assert(message == decrypt);
        }
        
        
    }

    return 0;
}
