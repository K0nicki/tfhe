#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <cassert>
#include <chrono>

int main(int argc, char const *argv[])
{
    int32_t M = 8;
    double alpha = DEF_TLWE_ALPHA;
    double alpha_max = 0.071;
    double alpha_min = 0.01;
    double avgTime = 0;
    int32_t trials = 1000;

    TLWEParams params512_1{512, 1, alpha_min, alpha_max};
    TLWEParams params512_2{512, 2, alpha_min, alpha_max};
    TLWEParams params1024_1{1024, 1, alpha_min, alpha_max};
    TLWEParams params1024_2{1024, 2, alpha_min, alpha_max};

    TLWEKey key512_1{&params512_1};
    TLWEKey key1024_1{&params1024_1};
    TLWEKey key512_2{&params512_2};
    TLWEKey key1024_2{&params1024_2};

    std::vector<TLWEParams *> allParams = {&params512_1, &params512_2, &params1024_1, &params1024_2};
    std::vector<TLWEKey *> allKeys = {&key512_1, &key1024_1, &key1024_1, &key1024_2};
    std::vector<TLWEParams *> allParams1024 = {&params1024_1, &params1024_2};
    std::vector<TLWEParams *> _2Params1024 = {&params1024_2};
    std::vector<TLWEKey *> allKeys1024 = {&key1024_1, &key1024_2};

    std::chrono::system_clock::time_point start, end;

    // // for (TLWEParams *params : allParams1024)
    for (TLWEParams *params : allParams1024)
    {
        std::cout << "Params:\nKey Length: " << params->getDegree() << "\nPolynomials amount: " << params->getPolyAmount()
                  << "\ntrials: " << trials << std::endl
                  << std::endl;

        int32_t N = params->getDegree();

        std::uniform_int_distribution<int32_t> distrib(0, M - 1);
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
                assert(switchFromTorus32(message.getCoef(i) + msg2.getCoef(i), M) == switchFromTorus32(decrypt.getCoef(i), M));
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
                assert(switchFromTorus32(message.getCoef(i) - msg2.getCoef(i), M) == switchFromTorus32(decrypt.getCoef(i), M));
        }

        std::cout << "SUBTRACTION PASS! " << std::endl;
        std::cout << "Average subtraction time[s]: " << avgTime / trials * 1e-6 << std::endl;
        std::cout << std::endl;

        std::cout << "MAX ADDITION TEST\n"
                  << std::endl;

        TLWEKey tlwekey{params};
        tlweKeyGen(&tlwekey, params);

        int32_t msg0 = 0;
        int32_t msg1 = 1;
        TLWESample encrypt0 = tlweEncryptT(switchToTorus32(msg0, M), alpha, &tlwekey);
        TLWESample encrypt1 = tlweEncryptT(switchToTorus32(msg1, M), alpha, &tlwekey);
        TLWESample resultLoop{params};
        int tmp = 1;
        bool condition = true;

        while (condition)
        {
            start = std::chrono::system_clock::now();
            // ----------------------------------------------------------
            tlweAddTo(&encrypt0, &encrypt1, params);
            // ----------------------------------------------------------
            end = std::chrono::system_clock::now();
            avgTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            msg0 += msg1;
            Torus32 res = tlweDecryptT(&encrypt0, &tlwekey, M);

            if (switchFromTorus32(res, M) != msg0 % M)
            {
                condition = false;
                // std::cout << "|" << tmp << "|\n\t" << switchFromTorus32(res, M) << " v " << msg0 % M << std::endl
                //           << "\t" << res << " v " << switchToTorus32(msg0, M) << std::endl;
            }

            if (!condition)
                break;

            tmp++;
        }

        std::cout << "\nMaximum number of addition operations is: " << tmp << std::endl;
        std::cout << "Avg time [s]: " << avgTime / tmp * 1e-6 << std::endl;
        std::cout << "Noise lvl: " << encrypt0.getNoise() << std::endl;

        std::cout << "\nMAX SUBSTRACTION TEST\n"
                  << std::endl;

        msg0 = 0;
        msg1 = 1;
        encrypt0 = tlweEncryptT(switchToTorus32(msg0, M), alpha, &tlwekey);
        encrypt1 = tlweEncryptT(switchToTorus32(msg1, M), alpha, &tlwekey);
        tmp = 1;
        condition = true;

        while (condition)
        {
            start = std::chrono::system_clock::now();
            // ----------------------------------------------------------
            tlweSubTo(&encrypt0, &encrypt1, params);
            // ----------------------------------------------------------
            end = std::chrono::system_clock::now();
            avgTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            msg0 -= msg1;
            Torus32 res = tlweDecryptT(&encrypt0, &tlwekey, M);

            if (res != switchToTorus32(msg0, M))
            {
                condition = false;
                // std::cout << "|" << tmp << "|\n\t" << switchFromTorus32(res, M) << " v " << msg0 % M << std::endl
                //           << "\t" << res << " v " << switchToTorus32(msg0, M) << std::endl;
            }

            if (!condition)
                break;

            tmp++;
        }

        std::cout << "\nMaximum number of substraction operations is: " << tmp << std::endl;
        std::cout << "Avg time [s]: " << avgTime / tmp * 1e-6 << std::endl;
        std::cout << "Noise lvl: " << encrypt0.getNoise() << std::endl;
    }
    return 0;
}