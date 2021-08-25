#include "../../include/tlwe/functions.h"

void tlweKeyGen(TLWEKey* result) {
    int32_t N = result->getTLWEParams()->getDegree();
    int32_t k = result->getTLWEParams()->getPolyAmount();
    std::uniform_int_distribution<int32_t> distrib(0,1);

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

    for (int i = 0; i < k; i++)
        for (int j = 0; j < N; j++)
            (result->getKey(i))->setCoefficient(distrib(gen), j);
}
