#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>

int main(int argc, char const *argv[])
{
    // LWEParams params(128, 0.1, 0.3);
    // LWEKey key(&params);

    // lweKeyGen(&key);
    // std::cout <<(&key)->getLWEKey()<<std::endl;

    // for (int i = 0; i < (&params)->getLength(); i++)
    // {
    //     std::cout << ((&key)->getLWEKey())[i];
    // }

    // int32_t Msize = 444;
    // unsigned long phase = 2UL;
    // int64_t interv = ((UINT64_C(1) << 64 / Msize));
    // int64_t half_interv = interv / 2;
    // uint64_t phase64 = (uint64_t(phase<<32)) + half_interv;
    // phase64 -= phase64%interv;
    // // printf("Result: %d\n", int32_t(phase64>>32));

    int32_t M = 8;
    double alpha = 1. / (10 * M);
    int ok = 0;
    int warning = 0;

    LWEParams params500(500, 0., 1.);
    LWEParams params750(750, 0., 1.);
    LWEParams params1024(1024, 0., 1.);

    LWEKey key500(&params500);
    LWEKey key750(&params750);
    LWEKey key1024(&params1024);

    std::vector<LWEParams *> params = {&params500, &params750, &params1024};
    std::vector<LWEKey *> keys = {&key500, &key750, &key1024};

    lweKeyGen(&key500);
    lweKeyGen(&key750);
    lweKeyGen(&key1024);

    // Need to encrypt message with different keys
    // Afterwards decrypt them and compare
    for (LWEKey *key : keys)
    {
        // Decryption
        LWESample sample(key->getParams());
        // Addition
        LWESample addSample1(key->getParams());
        LWESample addSample2(key->getParams());
        LWESample addSampleResult(key->getParams());
        //Subtraction
        LWESample subSample1(key->getParams());
        LWESample subSample2(key->getParams());
        LWESample subSampleResult(key->getParams());

        for (int32_t i = 0; i < 10; i++)
        {
            Torus32 message{switchToTorus32(i % 2, M)};
            sample = lweEncrypt(&message, alpha, key);
            Torus32 phase{lwePhase(&sample, key)};
            Torus32 decryption = lweDecrypt(&sample, key, M);
            double dmsg = t32tod(message);
            double dphase = t32tod(phase);
            // std::cout << "Original message [Torus]: " << message << "; decrypted message [Torus]: " << decryption << std::endl;
            if (message == decryption)
            {
                // std::cout << "[OK] Correct encryption\n";
                ok++;
            }
            else
            {
                std::cout << "[WARNING] Decryption is not correct:\t" << switchFromTorus32(message, M) << " v " << switchFromTorus32(decryption, M) << std::endl;
                warning++;
            }

            double result = absfrac(dmsg - dphase);
            // std::cout << "Checking error lvl..." << std::endl;
            if (result < 10 * alpha)
            {
                // std::cout << "[OK]\n";
                ok++;
            }
            else
            {
                std::cout << "[WARNING] Error grather that expected\n";
                warning++;
            }

            // Addition
            addSample1 = lweEncrypt(&message, alpha, key);
            addSample2 = lweEncrypt(&message, alpha, key);
            lweCopy(&addSampleResult, &addSample1, key->getParams());
            lweAdd(&addSampleResult, &addSample2, key->getParams());
            // for (int i = 0; i < key->getParams()->getLength(); i++)
            // {

            //     if ((&addSampleResult)->getA()[i] == ((&addSample1)->getA()[i] + (&addSample2)->getA()[i]))
            //         ok++;
            //     else
            //     {
            //         std::cout << "[WARNING] Addition incorrect at " << i << " index!\n";
            //         warning++;
            //     }
            // }
            // if ((&addSampleResult)->getB() == ((&addSample1)->getB() + (&addSample2)->getB()))
            //     ok++;
            // else
            // {
            //     std::cout << "[WARNING] Addition incorrect for B parameter!!\nExpected " << (&addSampleResult)->getB() << " but got " << (&addSample1)->getB() + (&addSample2)->getB() << std::endl;
            //     warning++;
            // }
            // if ((&addSampleResult)->getCurrentNoise() == ((&addSample1)->getCurrentNoise() + (&addSample2)->getCurrentNoise()))
            //     ok++;
            // else
            // {
            //     std::cout << "[WARNING] Addition incorrect for noise value parameter!!\nExpected " << (&addSampleResult)->getCurrentNoise() << " but got " << (&addSample1)->getCurrentNoise() + (&addSample2)->getCurrentNoise() << std::endl;
            //     warning++;
            // }

            // Subtraction
            subSample1 = lweEncrypt(&message, alpha, key);
            subSample2 = lweEncrypt(&message, alpha, key);
            lweCopy(&subSampleResult, &subSample1, key->getParams());
            lweSubTo(&subSampleResult, &subSample2, key->getParams());
            // for (int i = 0; i < key->getParams()->getLength(); i++)
            // {

            //     if ((&subSampleResult)->getA()[i] == ((&subSample1)->getA()[i] - (&subSample2)->getA()[i]))
            //         ok++;
            //     else
            //     {
            //         std::cout << "[WARNING] Subtraction incorrect at " << i << " index!\n";
            //         warning++;
            //     }
            // }
            // if ((&subSampleResult)->getB() == ((&subSample1)->getB() - (&subSample2)->getB()))
            //     ok++;
            // else
            // {
            //     std::cout << "[WARNING] Subtraction incorrect for B parameter!!\nExpected " << (&subSampleResult)->getB() << " but got " << (&subSample1)->getB() - (&subSample2)->getB() << std::endl;
            //     warning++;
            // }
            // if ((&subSampleResult)->getCurrentNoise() == ((&subSample1)->getCurrentNoise() - (&subSample2)->getCurrentNoise()))
            //     ok++;
            // else
            // {
            //     std::cout << "[WARNING] Subtraction incorrect for noise value parameter!!\nExpected " << (&subSampleResult)->getCurrentNoise() << " but got " << (&subSample1)->getCurrentNoise() - (&subSample2)->getCurrentNoise() << std::endl;
            //     warning++;
            // }

            // Prepare for the next round
            lweClear(&sample, key->getParams());
        }
    }

    std::cout << std::endl
              << "Summarization:\n- OK: " << ok << "\n- WARNING: " << warning << std::endl;
    return 0;
}