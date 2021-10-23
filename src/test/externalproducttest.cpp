#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <cassert>

int main(int argc, char const *argv[])
{
    TLWEParams params1024_1{1024, 1, 0., 1.};
    // TLWEParams params1024_2{1024, 2, 0., 1.};

    TGSWParams tgswParams1024_1{&params1024_1};
    // TGSWParams tgswPrams1024_2{&params1024_2};

    // std::vector<TGSWParams *> allParams1024 = {&tgswParams1024_1, &tgswPrams1024_2};
    std::vector<TGSWParams *> allParams1024 = {&tgswParams1024_1};

    int32_t M = 16;

    for (TGSWParams *params : allParams1024)
    {
        int err = 0;
        TGSWKey key{params};
        tgswKeyGen(&key);
        int32_t keyLength = key.getTLWEKey()->getTLWEParams()->getDegree();
        const int32_t k = key.getTLWEKey()->getTLWEParams()->getPolyAmount();
        const int32_t l = key.getTGSWparams()->getDecompositionLength();

        TorusPolynomial msg;
        IntPolynomial one;        
        TGSWSample tgswSample1{params};
        TGSWSample tgswSamplem1{params};
        TGSWSampleFft tgswSampleFft;
        TLWESample result{params->getTLWEParams()};
        TLWESample resultm1{params->getTLWEParams()};
        std::uniform_int_distribution<int32_t> distrib(0, M);
        std::random_device rd;
        std::mt19937 gen(rd());

        for (int i = 0; i < msg.getCoefAsArray()->size(); i++)
            msg.setCoefficient(i, switchToTorus32(distrib(gen), M));

        for (int i = 0; i < params->getTLWEParams()->getDegree(); i++)
            one.setCoefficient(i, 1);

        TLWESample encypt = tlweEncrypt(&msg, DEF_TRLWE_ALPHA, key.getTLWEKey());

        // tgsw encryption
        tgswEncryptInt(&tgswSample1, 1, DEF_TRLWE_ALPHA, &key);

        // External product
        // TRGSW x TRLWE = TRLWE
        externalTgswProduct(&result, &encypt, &tgswSample1, params);

        TorusPolynomial decrypt = tlweDecrypt(&result, key.getTLWEKey(), M);

        for (int i = 0; i < params->getTLWEParams()->getDegree(); i++)
        {
            if (msg.getCoef(i) != decrypt.getCoef(i))
            {
                err++;
                std::cout << msg.getCoef(i) << " v " << decrypt.getCoef(i) << std::endl;
            }
            assert(msg.getCoef(i) == decrypt.getCoef(i));
        }
        std::cout << "Total errors: " << err << std::endl;

        // Test cs = -1
        err=0;
        tgswEncryptInt(&tgswSamplem1, -1, DEF_TRLWE_ALPHA, &key);
        externalTgswProduct(&resultm1, &encypt, &tgswSamplem1, params);
        decrypt = tlweDecrypt(&resultm1, key.getTLWEKey(), M);

        for (int i = 0; i < params->getTLWEParams()->getDegree(); i++)
        {
            // -INT_MIN == INT_MAX + 1 == INT_MIN
            // -0 == 0
            if (msg.getCoef(i) == decrypt.getCoef(i) && msg.getCoef(i) != 0 && msg.getCoef(i) != INT32_MIN)
            {
                err++;
                std::cout << msg.getCoef(i) << " v " << decrypt.getCoef(i) << std::endl;
            }
            assert(-msg.getCoef(i) == decrypt.getCoef(i));
        }
        std::cout << "Total errors: " << err << std::endl;
    }
    std::cout << "External product test passed!"<< std::endl;
    return 0;
}