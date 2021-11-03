#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <cassert>
#include <chrono>

int main(int argc, char const *argv[])
{
    // Random {0,1} generator
    std::uniform_int_distribution<int32_t> distrib(0, 1);
    std::uniform_int_distribution<int32_t> distrib07(0, 7);
    std::random_device rd;
    std::mt19937 gen(rd());

    TLWEParams params1024_1{1024, 1, 0., 1.};
    TGSWParams tgswParams1024_1{&params1024_1};
    std::vector<TGSWParams *> tgswparams1024 = {&tgswParams1024_1};
    std::chrono::system_clock::time_point start, end;

    int min;
    int max;

    int trials = 10;
    int M = 8;
    double avgTime = 0;

    for (TGSWParams *params : tgswparams1024)
    {
        int err = 0;
        int N = params->getTLWEParams()->getDegree();
        TGSWKey key{params};
        tgswKeyGen(&key);

        for (int trial = 0; trial < trials; trial++)
        {
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
            std::vector<TGSWSample *> cs(trials);
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
            start = std::chrono::system_clock::now();
            // ----------------------------------------------------------
            for (int i = 0; i < trials; i++)
                cMux(&result[i], cs[i], &d1s[i], &d0s[i], params);
            // ----------------------------------------------------------
            end = std::chrono::system_clock::now();
            avgTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            // veryfication
            std::vector<TorusPolynomial> torusResult(trials);
            torusResult[trial] = tlweDecrypt(&result[trial], key.getTLWEKey(), M);
            for (int j = 0; j < torusResult[trial].getLenght(); j++)
            {
                if (torusResult[trial].getCoef(j) != ((m[trial] > 0) ? d1Polys[trial].getCoef(j) : d0Polys[trial].getCoef(j)))
                    err++;

                assert(torusResult[trial].getCoef(j) == ((m[trial] > 0) ? d1Polys[trial].getCoef(j) : d0Polys[trial].getCoef(j)));
            }

            min = rand() % torusResult[trial].getLenght();
            max = min + 1;
            for (int j = min; j < max; j++)
                std::cout << torusResult[trial].getCoef(j) << " == " << m[trial] << " ? "
                          << d1Polys[trial].getCoef(j) << " : " << d0Polys[trial].getCoef(j) << std::endl;
        }

        std::cout << "Total errors: " << err << std::endl;
        std::cout << "Average CMux gate execution time[s]: " << avgTime / trials / tgswparams1024.size() * 1e-6 << std::endl;
        std::cout << std::endl;
    }

    std::cout << " CMUX test passed!" << std::endl;
    return 0;
}
