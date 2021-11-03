#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <cassert>
#include <chrono>

int main(int argc, char const *argv[])
{

    TLWEParams params512_1{10, 1, 0., 1.};
    TLWEParams params512_2{512, 2, 0., 1.};
    TLWEParams params1024_1{1024, 1, 0., 1.};
    TLWEParams params1024_2{1024, 2, 0., 1.};

    TLWEKey key512_1{&params512_1};
    TLWEKey key1024_1{&params1024_1};
    TLWEKey key512_2{&params512_2};
    TLWEKey key1024_2{&params1024_2};

    std::vector<TLWEParams *> allParams = {&params512_1, &params512_2, &params1024_1, &params1024_2};
    std::vector<TLWEKey *> allKeys = {&key512_1, &key1024_1, &key1024_1, &key1024_2};
    std::vector<TLWEParams *> allParams1024 = {&params1024_1, &params1024_2};
    std::vector<TLWEKey *> allKeys1024 = {&key1024_1, &key1024_2};

    std::chrono::system_clock::time_point start, end;

    int32_t M = 8;
    double alpha = 1. / (10 * M);
    int32_t trials = 10;
    double avgTime = 0;

    for (TLWEParams *params : allParams1024)
    {
        int32_t N = params->getDegree();

        std::uniform_int_distribution<int32_t> distrib(0, M);
        std::random_device rd;
        std::mt19937 gen(rd());

        TorusPolynomial message{N};
        TorusPolynomial msg2{N};
        TorusPolynomial decrypt{N};
        TLWESample encrypt{params};
        TLWESample addResult{params};
        TLWESample subResult{params};

        TLWEKey key{params};
        tlweKeyGen(&key, params);

        for (int trial = 0; trial < trials; trial++)
        {

            // Generate random msg
            for (int i = 0; i < N; i++)
            {
                message.setCoefficient(i, switchToTorus32(distrib(gen), M));
                msg2.setCoefficient(i, switchToTorus32(distrib(gen), M));
            }

            encrypt = tlweEncrypt(&message, alpha, &key);
            TLWESample encrypt2 = tlweEncrypt(&msg2, alpha, &key);

            start = std::chrono::system_clock::now();
            // ----------------------------------------------------------
            tlweAdd(&addResult, &encrypt, &encrypt2, params);
            // ----------------------------------------------------------
            end = std::chrono::system_clock::now();
            avgTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            decrypt = tlweDecrypt(&addResult, &key, M);

            for (int i = 0; i < N; i++)
                assert((message.getCoef(i) + msg2.getCoef(i)) == decrypt.getCoef(i));

            int min = rand() % N;
            int max = min + 1;
            for (int i = min; i < max; i++)
                // assert(switchFromTorus32(message.getCoef(i) + msg2.getCoef(i), M) == switchFromTorus32(decrypt.getCoef(i), M));
                std::cout << switchFromTorus32(message.getCoef(i) + msg2.getCoef(i), M) << " == " << switchFromTorus32(decrypt.getCoef(i), M) << std::endl;
        }

        std::cout << "ADDITION PASS! " << std::endl;
        std::cout << "Average addition time[s]: " << avgTime / trials * 1e-6 << std::endl;
        std::cout << std::endl;

        avgTime = 0;
        for (int trial = 0; trial < trials; trial++)
        {

            // Generate random msg
            for (int i = 0; i < N; i++)
            {
                message.setCoefficient(i, switchToTorus32(distrib(gen), M));
                msg2.setCoefficient(i, switchToTorus32(distrib(gen), M));
            }

            encrypt = tlweEncrypt(&message, alpha, &key);
            TLWESample encrypt2 = tlweEncrypt(&msg2, alpha, &key);

            start = std::chrono::system_clock::now();
            // ----------------------------------------------------------
            tlweSub(&subResult, &encrypt, &encrypt2, params);
            // ----------------------------------------------------------
            end = std::chrono::system_clock::now();
            avgTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            decrypt = tlweDecrypt(&subResult, &key, M);

            for (int i = 0; i < N; i++)
                assert((message.getCoef(i) - msg2.getCoef(i)) == decrypt.getCoef(i));

            int min = rand() % N;
            int max = min + 1;
            for (int i = min; i < max; i++)
                // assert(switchFromTorus32(message.getCoef(i) - msg2.getCoef(i), M) == switchFromTorus32(decrypt.getCoef(i), M));
                std::cout << switchFromTorus32(message.getCoef(i) - msg2.getCoef(i), M) << " == " << switchFromTorus32(decrypt.getCoef(i), M) << std::endl;
        }

        std::cout << "SUBTRACTION PASS! " << std::endl;
        std::cout << "Average subtraction time[s]: " << avgTime / trials * 1e-6 << std::endl;
        std::cout << std::endl;

        std::cout << "MAX ADDITION TEST\n"
                  << std::endl;

        TorusPolynomial msg0{N};
        TorusPolynomial msg1{N};
        for (int i = 0; i < N; i++)
        {
            msg0.setCoefficient(i, switchToTorus32(0, M));
            msg1.setCoefficient(i, switchToTorus32(1, M));
        }

        TLWESample encrypt0 = tlweEncrypt(&msg0, alpha, &key);
        TLWESample encrypt1 = tlweEncrypt(&msg1, alpha, &key);
        TLWESample resultLoop{params};
        int tmp = 1;
        bool condition = true;

        while (condition)
        {
            tlweAdd(&resultLoop, &encrypt0, &encrypt1, params);
            TorusPolynomial res = tlweDecrypt(&resultLoop, &key, M);

            for (int i = 0; i < N; i++)
                if (switchFromTorus32(res.getCoef(i), M) != tmp % M)
                {
                    condition = false;
                    std::cout << "Round: " << tmp << " | Position: " << i << std::endl
                              << "\t" << switchFromTorus32(res.getCoef(i), M) << " v " << tmp % M << std::endl
                              << "\t" << res.getCoef(i) << " v " << switchToTorus32(tmp, M) << std::endl;
                    break;
                }

            if (!condition)
                break;

            // If decryption is correct replace encrypt0 with resultLoop and increment tmp value
            tlweCopy(&encrypt0, &resultLoop, params);
            tmp++;
        }

        std::cout << "\nMaximum number of addition operations is: " << tmp << std::endl;
        std::cout << "Noise lvl: " << resultLoop.getNoise() << std::endl;
        
    }
    return 0;
}