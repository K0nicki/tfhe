#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <cassert>

int main(int argc, char const *argv[])
{
    LWEParams params1024(1024, 0., 1.);
    LWEKey key1024(&params1024);
    lweKeyGen(&key1024);

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

    std::uniform_int_distribution<int32_t> distrib07(0, 2);
    std::random_device rd;
    std::mt19937 gen(rd());
    int trials = 10;
    int32_t N = params1024.getLength();
    int M = 16;
    int warning = 0;

    for (TLWEParams * params : allParams1024) {
        int32_t N = params->getDegree();
        TLWESample sample{params};

        TLWEKey key{params};
        tlweKeyGen(&key, params);
        TorusPolynomial message{N};
        TorusPolynomial decrypt{N};
        TorusPolynomial decryptVerif{N};
        TLWESample cipherVerif{params};
        std::array<FftPoly,2> fftPolyVect;
        std::array<TorusPolynomial,2> resultpoly;

        for (int i = 0; i < N; i++)
            // message.setCoefficient(i, 2);
            message.setCoefficient(i, switchToTorus32(distrib07(gen), M));
        
        // Encrypt message
        TLWESample cipher = tlweEncrypt(&message, DEF_TRLWE_ALPHA, &key);

        for (int i = 0; i < params->getPolyAmount(); i++) {
            //fft
            torusPolyfft<DEF_N>(&fftPolyVect.at(i), cipher.getA(i)->getCoefAsArray());
            //ifft
            torusPolyifft<DEF_N>(resultpoly.at(i).getCoefAsArray(), &fftPolyVect.at(i));
        }

        cipherVerif.setA(0, &resultpoly.at(0));
        cipherVerif.setA(1, &resultpoly.at(1));
        cipherVerif.setB();

        for (int i = 0; i < N; i++)
            for (int j = 0; j < params->getPolyAmount(); j++)
            {
                if (cipher.getA(j)->getCoef(i) != resultpoly.at(j).getCoef(i)) {
                    // std::cout << cipher.getA(j)->getCoef(i) << " vs " << resultpoly.at(j).getCoef(i) << std::endl;
                    warning++;
                }
            }
            
        decrypt = tlweDecrypt(&cipher, &key, M);
        decryptVerif = tlweDecrypt(&cipherVerif, &key, M);

        for (int i = 0; i < N; i++)
        {
            if (message.getCoef(i) != decryptVerif.getCoef(i)) {
                // std::cout << message.getCoef(i) << " vs " << decryptVerif.getCoef(i) << std::endl;
                warning++;
            }
        }

        IntPolynomial intpoly;
        TorusPolynomial toruspoly;
        TorusPolynomial torusresult;

        for (int i = 0; i < N; i++)
        {
            intpoly.setCoefficient(i, 1);
            toruspoly.setCoefficient(i, switchToTorus32(1, M));
        }

        torusPolyMulFD(&torusresult, &intpoly, &toruspoly);

        // for (int i = 0; i < N; i++)
        // {
        // }
                
        
    }
    std::cout << "WARNINGS: " << warning << std::endl;

    return 0;
}
