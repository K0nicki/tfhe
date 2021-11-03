#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <cassert>
#include <chrono>
#include <cmath>

int main(int argc, char const *argv[])
{
    TLWEParams params1024_1{1024, 1, 0., 1.};
    TGSWParams tgswParams1024_1{&params1024_1};
    std::vector<TGSWParams *> allParams1024 = {&tgswParams1024_1};
    std::chrono::system_clock::time_point start, end;

    int min;
    int max;

    int32_t M = 16;
    int32_t trials = 30;
    double avgTime = 0;

    for (TGSWParams *params : allParams1024)
    {
        int err = 0;
        int32_t N = params->getTLWEParams()->getDegree();
        TGSWKey key{params};
        tgswKeyGen(&key);
        int32_t keyLength = key.getTLWEKey()->getTLWEParams()->getDegree();
        const int32_t k = key.getTLWEKey()->getTLWEParams()->getPolyAmount();
        const int32_t l = key.getTGSWparams()->getDecompositionLength();
        std::uniform_int_distribution<int32_t> distrib(0, M);
        std::uniform_int_distribution<int32_t> distribMul(1, 3);
        std::random_device rd;
        std::mt19937 gen(rd());

        for (int trial = 0; trial < trials; trial++)
        {
            int multiplier = distribMul(gen);
            std::cout << "Selected multiplier: " << multiplier << std::endl;

            TorusPolynomial msg;
            TGSWSample tgswSample1{params};
            TGSWSample tgswSamplem1{params};
            TLWESample result{params->getTLWEParams()};
            TLWESample resultm1{params->getTLWEParams()};

            for (int i = 0; i < msg.getCoefAsArray()->size(); i++)
                msg.setCoefficient(i, switchToTorus32(distrib(gen), M));

            TLWESample encypt = tlweEncrypt(&msg, DEF_TRLWE_ALPHA, key.getTLWEKey());

            // tgsw encryption
            tgswEncryptInt(&tgswSample1, multiplier, DEF_TRLWE_ALPHA, &key);

            // External product
            // TRGSW x TRLWE = TRLWE

            start = std::chrono::system_clock::now();
            // ----------------------------------------------------------
            externalTgswProduct(&result, &encypt, &tgswSample1, params);
            // ----------------------------------------------------------
            end = std::chrono::system_clock::now();
            avgTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            TorusPolynomial decrypt = tlweDecrypt(&result, key.getTLWEKey(), M);

            for (int i = 0; i < N; i++)
            {
                if (msg.getCoef(i) * multiplier != decrypt.getCoef(i))
                    err++;
                // std::cout << msg.getCoef(i) << " v " << decrypt.getCoef(i) << std::endl;
                // else
                //     std::cout << msg.getCoef(i)*multiplier << " v " << decrypt.getCoef(i) << std::endl;

                assert(msg.getCoef(i) * multiplier == decrypt.getCoef(i));
                assert(switchFromTorus32(msg.getCoef(i) * multiplier, M) == switchFromTorus32(decrypt.getCoef(i), M));
            }

            min = rand() % N;
            max = min + 1;
            for (int i = min; i < max; i++)
                // std::cout << switchFromTorus32(msg.getCoef(i) * multiplier, M) << " == " << switchFromTorus32(decrypt.getCoef(i), M) << std::endl;
                std::cout << switchFromTorus32(msg.getCoef(i), M) << "*" << multiplier << " == "
                          << switchFromTorus32(msg.getCoef(i) * multiplier, M) << " == " << switchFromTorus32(decrypt.getCoef(i), M) << std::endl;

            // Negative numbers
            // Test cs = -1 * multiplier
            err = 0;
            int32_t msgInt = -1 * multiplier;
            tgswEncryptInt(&tgswSamplem1, msgInt, DEF_TRLWE_ALPHA, &key);

            start = std::chrono::system_clock::now();
            // ----------------------------------------------------------
            externalTgswProduct(&resultm1, &encypt, &tgswSamplem1, params);
            // ----------------------------------------------------------
            end = std::chrono::system_clock::now();
            avgTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            decrypt = tlweDecrypt(&resultm1, key.getTLWEKey(), M);

            for (int i = 0; i < N; i++)
            {
                // -INT_MIN == INT_MAX + 1 == INT_MIN
                // -0 == 0
                if (msg.getCoef(i) * multiplier == decrypt.getCoef(i) && msg.getCoef(i) * multiplier != 0 && msg.getCoef(i) * multiplier != INT32_MIN)
                    err++;
                // std::cout << msg.getCoef(i) << " v " << decrypt.getCoef(i) << std::endl;
                // else
                //     std::cout << msg.getCoef(i)*multiplier << " v " << decrypt.getCoef(i) << std::endl;

                assert(-msg.getCoef(i) * multiplier == decrypt.getCoef(i));
                assert(switchFromTorus32(-msg.getCoef(i) * multiplier, M) == switchFromTorus32(decrypt.getCoef(i), M));
            }

            min = rand() % N;
            max = min + 1;
            for (int i = min; i < max; i++)
                std::cout << switchFromTorus32(msg.getCoef(i), M) << "*" << -multiplier << " == "
                          << switchFromTorus32(-msg.getCoef(i) * multiplier, M) << " == " << switchFromTorus32(decrypt.getCoef(i), M) << std::endl;

            std::cout << std::endl;
        }
        std::cout << "Total errors: " << err << std::endl;
        std::cout << "Average External Product function execution time[s]: " << avgTime / trials / allParams1024.size() * 1e-6 / 2 << std::endl;
        std::cout << "External product test passed!" << std::endl;

        std::cout << "\nMaximum multiplication test!" << std::endl;
        int multiplier = 2;

        TorusPolynomial msg;
        TorusPolynomial decrypt;
        TGSWSample tgswLoop{params};
        TLWESample resultLoop{params->getTLWEParams()};
        bool condition = true;
        int tmp = multiplier;

        // msg[*] = 1
        for (int i = 0; i < msg.getCoefAsArray()->size(); i++)
            msg.setCoefficient(i, switchToTorus32(1, M));

        TLWESample encrypt = tlweEncrypt(&msg, DEF_TRLWE_ALPHA, key.getTLWEKey());

        // encrypted 2
        tgswEncryptInt(&tgswLoop, multiplier, DEF_TRLWE_ALPHA, &key);

        while (condition)
        {
            // Clear resultLoop 
            for (int j = 0; j <= k; j++)
                for (int i = 0; i < N; i++)
                    resultLoop.getA(j)->setCoefficient(i, 0);
            
            externalTgswProduct(&resultLoop, &encrypt, &tgswLoop, params);
            decrypt = tlweDecrypt(&resultLoop, key.getTLWEKey(), M);

            for (int i = 0; i < N; i++)
                if (switchFromTorus32(decrypt.getCoef(i), M) != tmp % M)
                {
                    condition = false;
                    std::cout << "Round: " << tmp << " | Position: " << i << std::endl
                              << "\t" << switchFromTorus32(decrypt.getCoef(i), M) << " v " << tmp % M << std::endl
                              << "\t" << decrypt.getCoef(i) << " v " << switchToTorus32(tmp, M) << std::endl;
                    break;
                }

            if (!condition)
                break;

            std::cout << switchFromTorus32(decrypt.getCoef(0), M) << " v " << tmp % M <<std::endl;
            
            // If decryption is correct replace encrypt0 with resultLoop and increment tmp value
            tlweCopy(&encrypt, &resultLoop, params->getTLWEParams());
            tmp *= multiplier;
        }
        std::cout << "\nMaximum number of multiplication operations is: " << log2(tmp) << std::endl;
        std::cout << "Noise lvl: " << resultLoop.getNoise() << std::endl;           // Too low noise, check it later
    }
    return 0;
}