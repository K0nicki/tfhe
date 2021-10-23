#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
// #include "../include/general/secparams.h"

int main(int argc, char const *argv[])
{
    TLWEParams params512_1{10, 1, 0., 1.};
    TLWEParams params512_2{512, 2, 0., 1.};
    TLWEParams params1024_1{1024, 1, 0., 1.};
    TLWEParams params1024_2{1024, 2, 0., 1.};

    TGSWParams tgswParams512_1{&params512_1};
    TGSWParams tgswParams512_2{&params512_2};
    TGSWParams tgswParams1024_1{&params1024_1};
    TGSWParams tgswPrams1024_2{&params1024_2};

    std::vector<TGSWParams *> allParams = {&tgswParams512_1, &tgswParams512_2, &tgswParams1024_1, &tgswPrams1024_2};
    std::vector<TGSWParams *> allParams1024 = {&tgswParams1024_1, &tgswPrams1024_2};

    for (TGSWParams *params : allParams1024)
    {
        int trials = 10;
        TGSWKey key{params};
        tgswKeyGen(&key);
        int32_t keyLength = key.getTLWEKey()->getTLWEParams()->getDegree();
        int32_t k = key.getTLWEKey()->getTLWEParams()->getPolyAmount();

        for (int i = 0; i < k; i++)
        {
            int zero = 0;
            int one = 0;
            for (int j = 0; j < keyLength; j++)
            {
                int32_t value = (&key)->getTLWEKey()->getIntKey()->getCoef(j);

                if (value == 0)
                    zero++;
                else
                    one++;
            }

            if (zero > 20 && one > 20)
                std::cout << "Random key: PASS" << std::endl;
            else
                std::cout << "Random key: ERROR" << std::endl;
        }

        IntPolynomial msg;
        std::uniform_int_distribution<int32_t> distrib(INT32_MIN, INT32_MAX);
        std::random_device rd;
        std::mt19937 gen(rd());

        for (int i = 0; i < ((&msg)->getCoefAsArray())->size(); i++)
        {
            (&msg)->setCoefficient(i, distrib(gen));
        }

        TGSWSample sample{params};


        tgswEncryptInt(&sample, 2, 3.14, &key);
        TGSWSampleFft testsamplefft{&sample};
        testsamplefft.initTGSWFftSample(&sample);

        TGSWSample sampleFftFresh{params};
        TGSWSampleFft sampleFft = tgswEncryptFft(2, 3.14, &key);

        std::array<FftPoly, 2 * DEF_l> fftArr;
        TLWESample tlweSample{params->getTLWEParams()};
    }

    return 0;
}