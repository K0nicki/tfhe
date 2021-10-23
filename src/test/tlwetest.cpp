#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <cassert>

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

    int32_t M = 8;
    double alpha = 1. / (10 * M);

    for (TLWEParams *params : allParams1024)
    {
        int32_t N = params->getDegree();
        TLWESample sample{params};

        TLWEKey key{params};
        tlweKeyGen(&key, params);
        TorusPolynomial message{N};
        TorusPolynomial testvect{N};
        TorusPolynomial decrypt{N};

        for (int i = 0; i < N; i++)
        {
            message.setCoefficient(i, switchToTorus32(rand() % M, M));
            testvect.setCoefficient(i, switchToTorus32(rand() % M, M));
        }

        // for (int i = 0; i < k; i++)
        // {
        //     for (int j = 0; j < N; j++)
        //     {
        //         std::cout << (&s[i])->getCoef(j);
        //     }
        //     std::cout << "\nDegree: " << param->getDegree() << "\nPoly amount in mask: " << param->getPolyAmount() << " | Poly number: " << i+1
        //               << std::endl
        //               << std::endl;
        // }

        sample = tlweEncrypt(&message, alpha, &key);
        TLWESample sample2 = tlweEncrypt(&testvect, alpha, &key);

        if (true)
        {
            for (int j = 0; j < params->getPolyAmount(); j++)
                for (int i = 0; i < N; i++)
                    sample.getA(j)->setCoefficient(i, sample.getA(j)->getCoef(i) + sample2.getA(j)->getCoef(i));
            for (int i = 0; i < N; i++)
                sample.getB()->setCoefficient(i, sample.getB()->getCoef(i) + sample2.getB()->getCoef(i));
        
        }

        decrypt = tlweDecrypt(&sample, &key, M);

        for (int i = 0; i < N; i++)
        {
            assert((message.getCoef(i) + testvect.getCoef(i)) == decrypt.getCoef(i));
        }
        std::cout << "PASS! " << std::endl;
    }
    return 0;
}