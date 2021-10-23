#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>

int main(int argc, char const *argv[])
{
    LWEParams params1024(1024, 0., 1.);
    LWEKey tlwekey(&params1024);
    lweKeyGen(&tlwekey);

    TLWEParams params1024_1{1024, 1, 0., 1.};
    TLWEParams params1024_2{1024, 2, 0., 1.};

    TGSWParams tgswParams1024_1{&params1024_1};
    TGSWParams tgswPrams1024_2{&params1024_2};

    std::vector<TGSWParams *> tgswparams1024 = {&tgswParams1024_1, &tgswPrams1024_2};
    int trials = 10;
    int32_t M = 8;

    for (TGSWParams *params : tgswparams1024)
    {
        LWESample sample(tlwekey.getParams());
        LWESample result(tlwekey.getParams());

        TGSWKey tgswkey{params};
        tgswKeyGen(&tgswkey);

        GateKey gk{&tgswkey, &tlwekey};

        for (int j = 0; j < trials; j++)
        {
            Torus32 message{switchToTorus32(j, M)};
            sample = lweEncrypt(&message, DEF_TLWE_ALPHA, &tlwekey);

            bootstrapinglwe2lwe(&result, &sample, &gk);
        }
        
        
    }

    return 0;
}
