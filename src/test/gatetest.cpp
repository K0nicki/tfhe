#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>

int main(int argc, char const *argv[])
{
    LWEParams params1024(1024, 0., 1.);
    LWEKey key1024(&params1024);
    lweKeyGen(&key1024);

    TLWEParams params1024_1{1024, 1, 0., 1.};
    TLWEParams params1024_2{1024, 2, 0., 1.};

    TGSWParams tgswParams1024_1{&params1024_1};
    TGSWParams tgswPrams1024_2{&params1024_2};

    std::vector<TGSWParams *> tgswparams1024 = {&tgswParams1024_1, &tgswPrams1024_2};
    int trials = 10;

    for (TGSWParams *params : tgswparams1024)
    {
        TGSWKey key{params};
        tgswKeyGen(&key);

        LweKeySwitch testLWEKS{&key1024};
        BootstrappingKey testBK{&key, &key1024};
        GateKey testGK{&key, &key1024};
    }
    std::cout << "Test passed!\n";
    return 0;
}