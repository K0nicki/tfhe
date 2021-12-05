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
    double alpha = pow(2.,-25);

    TLWEParams params1024_1{1024, 1, 0., 1.};
    TGSWParams tgswParams1024_1{&params1024_1};
    std::vector<TGSWParams *> tgswparams1024 = {&tgswParams1024_1};
    std::chrono::system_clock::time_point start, end;

    int min;
    int max;

    int trials = 5;
    int M = 8;
    double avgTime = 0;

    std::cout << "Params:\nKey Length: " << tgswParams1024_1.getTLWEParams()->getDegree() << "\nPolynomials amount: " << tgswParams1024_1.getTLWEParams()->getPolyAmount()
                << "\ntrials: " << trials << "\nalpha: " << alpha << "\nmodulo: " << M << std::endl << std::endl;

    std::vector<int32_t> m(trials);
    std::vector<TorusPolynomial> d0Polys(trials);
    std::vector<TorusPolynomial> d1Polys(trials);
    TGSWSample sample{&tgswParams1024_1};
    std::vector<TGSWSample *> cs(trials);
    std::vector<TLWESample> d0s(trials);
    std::vector<TLWESample> d1s(trials);

    for (TGSWParams *params : tgswparams1024)
    {
        int err = 0;
        int N = params->getTLWEParams()->getDegree();
        TGSWKey key{params};
        tgswKeyGen(&key);

        for (int32_t &i : m)
            i = distrib(gen);

        for (int i = 0; i < trials; i++){
            // d0 encryption
            for (int j = 0; j < N; j++)
                d0Polys[i].setCoefficient(j, switchToTorus32(distrib07(gen), M));

            // d1 encryption
            for (int j = 0; j < N; j++)
                d1Polys[i].setCoefficient(j, switchToTorus32(distrib07(gen), M));

            // cs encryption
            cs[i] = new TGSWSample{params};
            tgswEncryptInt(cs[i], m[i], alpha, &key);

            // Message encryption
            d0s[i] = tlweEncrypt(&(d0Polys[i]), alpha, key.getTLWEKey());
            d1s[i] = tlweEncrypt(&(d1Polys[i]), alpha, key.getTLWEKey());
        }

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

        for (int trial = 0; trial < trials; trial++)
        {
            torusResult[trial] = tlweDecrypt(&result[trial], key.getTLWEKey(), M);

            if (!torusPolyEQ(&torusResult[trial], m[trial] > 0 ? &d1Polys[trial] : &d0Polys[trial]))
                err++;

            // Assertion test
            for (int j = 0; j < torusResult[trial].getLenght(); j++)
                assert(torusResult[trial].getCoef(j) == ((m[trial] > 0) ? d1Polys[trial].getCoef(j) : d0Polys[trial].getCoef(j)));

            // Show randomly chosen message
            min = rand() % torusResult[trial].getLenght();
            max = min + 1;
            for (int j = min; j < max; j++)
                std::cout << t32tod(torusResult[trial].getCoef(j)) << " == " << m[trial] << " ? "
                          << t32tod(d1Polys[trial].getCoef(j)) << " : " << t32tod(d0Polys[trial].getCoef(j)) << std::endl;
        }

        std::cout << "Total errors: " << err << std::endl;
        std::cout << "Average CMux gate execution time[s]: " << avgTime / trials / tgswparams1024.size() * 1e-6 << std::endl;
        std::cout << std::endl;
    }
    std::cout << " CMUX test passed!" << std::endl;

    // // Never ending loop
    // TGSWSample cs{&tgswParams1024_1};
    // TLWESample result{tgswParams1024_1.getTLWEParams()};
    // TGSWKey key{&tgswParams1024_1};
    // tgswKeyGen(&key);

    // tgswEncryptInt(&cs, 1, alpha, &key);
    // TLWESample d0 = tlweEncryptT(switchToTorus32(0, M), alpha, key.getTLWEKey());
    // TLWESample d1 = tlweEncryptT(switchToTorus32(1, M), alpha, key.getTLWEKey());
    // bool condition = true;
    // int32_t loop = 1;

    // while (condition)
    // {
    //     cMux(&result, &cs, &d1, &d0, &tgswParams1024_1);

    //     if (tlweDecryptT(&result, key.getTLWEKey(), M) != switchToTorus32(1, M))
    //         condition = !condition;

    //     if (!condition)
    //         break;        

    //     tlweCopy(&d1, &result, tgswParams1024_1.getTLWEParams());
    //     tlweClear(&result, tgswParams1024_1.getTLWEParams());
    //     loop++;
    // }

    // std::cout << "\nMaximum number of substraction operations is: " << loop << std::endl;
    // std::cout << "Noise :" <<  alpha << std::endl;
    // std::cout << "Avg time [s]: " << avgTime / loop * 1e-6 << std::endl;
    
    return 0;
}
