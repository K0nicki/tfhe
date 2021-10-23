#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <cassert>

int main(int argc, char const *argv[])
{
    // Random {0,1} generator
    std::uniform_int_distribution<int32_t> distrib(0, 1);
    std::uniform_int_distribution<int32_t> distrib07(0, 7);
    std::random_device rd;
    std::mt19937 gen(rd());

    LWEParams params1024(1024, 0., 1.);
    LWEKey key1024(&params1024);
    lweKeyGen(&key1024);

    TLWEParams params1024_1{1024, 1, 0., 1.};
    // TLWEParams params1024_2{1024, 2, 0., 1.};

    TGSWParams tgswParams1024_1{&params1024_1};
    // TGSWParams tgswPrams1024_2{&params1024_2};

    // std::vector<TGSWParams *> tgswparams1024 = {&tgswParams1024_1, &tgswPrams1024_2};
    std::vector<TGSWParams *> tgswparams1024 = {&tgswParams1024_1};

    int trials = 10;
    int M = 8;

    for (TGSWParams *params : tgswparams1024)
    {
        int err = 0;
        int N = params->getTLWEParams()->getDegree();
        TGSWKey key{params};
        tgswKeyGen(&key);

        std::vector<int32_t> m(trials);
        for (int32_t &i : m)
            i = distrib(gen);

        std::vector<TorusPolynomial> d0Polys(trials);
        for (int i = 0; i < trials; i++)
            for (int j = 0; j < N; j++)
                d0Polys[i].setCoefficient(j, switchToTorus32(distrib07(gen), M));

        std::vector<TorusPolynomial> d1Polys(trials);
        for (int i = 0; i < trials; i++)
            for (int j = 0; j < N; j++)
                d1Polys[i].setCoefficient(j, switchToTorus32(distrib07(gen), M));

        // cs encrypted
        TGSWSample sample{params};
        std::vector<TGSWSample*> cs(trials);
        for (int i = 0; i < trials; i++)
        {
            cs[i] = new TGSWSample{params};
            tgswEncryptInt(cs[i], m[i], DEF_TRLWE_ALPHA, &key);
        }

        // d0 encrypted
        std::vector<TLWESample> d0s(trials);
        for (int i = 0; i < trials; i++)
            d0s[i] = tlweEncrypt(&(d0Polys[i]), DEF_TRLWE_ALPHA, key.getTLWEKey());

        // d1 encrypted
        std::vector<TLWESample> d1s(trials);
        for (int i = 0; i < trials; i++)
            d1s[i] = tlweEncrypt(&(d1Polys[i]), DEF_TRLWE_ALPHA, key.getTLWEKey());

        // CMUX validation
        std::vector<TLWESample> result(trials);
        for (int i = 0; i < trials; i++)
            cMux(&result[i], cs[i], &d1s[i], &d0s[i], params);

        std::vector<TorusPolynomial> torusResult(trials);
        for (int i = 0; i < trials; i++)
        {
            torusResult[i] = tlweDecrypt(&result[i], key.getTLWEKey(), M);
            for (int j = 0; j < torusResult[i].getLenght(); j++)
            {
                if (torusResult[i].getCoef(j) != ((m[i] > 0) ? d1Polys[i].getCoef(j) : d0Polys[i].getCoef(j)))
                    err++;
                assert(torusResult[i].getCoef(j) == ((m[i] > 0) ?  d1Polys[i].getCoef(j) : d0Polys[i].getCoef(j)));
            }
        }
        std::cout << "Total errors: " << err << std::endl;
    }

    std::cout << " CMUX test passed!" << std::endl;
    return 0;
}
