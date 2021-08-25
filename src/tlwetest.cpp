#include "../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>

int main(int argc, char const *argv[])
{

    TLWEParams params512_1(10, 1, 0., 1.);
    TLWEParams params512_2(512, 2, 0., 1.);
    TLWEParams params1024_1(1024, 1, 0., 1.);
    TLWEParams params1024_2(1024, 2, 0., 1.);
    TLWEParams params2048_1(2048, 1, 0., 1.);
    TLWEParams params2048_2(2048, 2, 0., 1.);

    TLWEKey key512_1(&params512_1);
    TLWEKey key1024_1(&params1024_1);
    TLWEKey key2048_1(&params2048_1);
    TLWEKey key512_2(&params512_2);
    TLWEKey key1024_2(&params1024_2);
    TLWEKey key2048_2(&params2048_2);

    std::vector<TLWEParams *> allParams = {&params512_1, &params512_2, &params1024_1, &params1024_2, &params2048_1, &params2048_2};
    std::vector<TLWEKey *> allKeys = {&key512_1, &key1024_1, &key1024_1, &key1024_2, &key2048_1, &key2048_2};

    for (TLWEParams *param : allParams)
    {
        int32_t N = param->getDegree();
        int32_t k = param->getPolyAmount();

        TLWEKey key(param);
        tlweKeyGen(&key);

        IntPolynomial *s = (&key)->getKey();

        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < N; j++)
            {
                std::cout << (&s[i])->getCoef(j);
            }
            std::cout << "\nDegree: " << param->getDegree() << "\nPoly amount in mask: " << param->getPolyAmount() << " | Poly number: " << i+1
                      << std::endl
                      << std::endl;
        }
    }
    return 0;
}