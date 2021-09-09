#include "../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>

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
    double alpha = 1. / (10 * M);;
    int ok = 0;
    int warning = 0;

    for (TLWEKey *key : allKeys1024)
    {
        TLWEParams *params = key->getTLWEParams();
        int32_t N = key->getTLWEParams()->getDegree();
        TLWESample sample{params};

        tlweKeyGen(key);
        TorusPolynomial message{N};
        TorusPolynomial decrypt{N};

        for (int i = 0; i < N; i++)
            (&message)->setCoefficient(i, switchToTorus32(rand() % M, M));

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

        tlweEncrypt(&sample, &message, alpha, key);
        tlweDecrypt(&decrypt, &sample, key, M);

        for (int i = 0; i < N; i++)
        {
            if ((&message)->getCoef(i) != (&decrypt)->getCoef(i))
            {
                // std::cout << "Error!\tPosition: " << i << std::endl;
                // std::cout << (&message)->getCoef(i) << " vs " << (&decrypt)->getCoef(i) << std::endl;
                warning++;
            }
            else
            {
                // std::cout << "OK!\tPosition: " << i << std::endl;
                // std::cout << (&message)->getCoef(i) << " vs " << (&decrypt)->getCoef(i) << std::endl;
                ok++;
            }
        }
        if (ok == N)
        {
            std::cout << "PASS! " << std::endl;
        }

        std::cout << "OKs: " << ok << std::endl;
        std::cout << "WARNINGs: " << warning << std::endl;
    }
    return 0;
}