#include "../../include/tfhe.h"
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <cassert>
#include <chrono>

int main(int argc, char const *argv[])
{
    std::uniform_int_distribution<int32_t> distrib(0, 1);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::chrono::system_clock::time_point start, end;

    LWEParams params1024(DEF_N, 0., 1.);
    LWEKey lwekey(&params1024);
    lweKeyGen(&lwekey);

    TLWEParams params1024_1{DEF_N, 1, 0., 1.};
    TGSWParams tgswParams1024_1{&params1024_1};

    std::vector<TGSWParams *> tgswparams1024 = {&tgswParams1024_1};
    double alpha = DEF_TLWE_ALPHA;
    int trials = 5;
    int32_t M = 8;
    int err = 0;
    double avgTime = 0;
    double maxTime = 0;
    double minTime = 1.;
    double diff = 0;

    for (TGSWParams *params : tgswparams1024)
    {
        TLWEParams *tlweParams = params->getTLWEParams();
        LWESample sample(tlweParams->getLWEParams());
        TGSWKey tgswkey{params};
        tgswKeyGen(&tgswkey);

        GateKey gk{&tgswkey, &lwekey};

        err = 0;
        Torus32 message = 0;
        Torus32 decrypt = 0;

        LWESample extractionResult(tlweParams->getLWEParams());
        LWESample result(&params1024);
        TLWESample testvect{tlweParams};
        TLWESample tlweresult{tlweParams};

        std::cout << "Average execution time for " << trials << " trials...\n";

        for (int j = 0; j < trials; j++)
        {
            lweClear(&extractionResult, tlweParams->getLWEParams());
            lweClear(&result, &params1024);
            tlweClear(&testvect, tlweParams);
            tlweClear(&tlweresult, tlweParams);

            bool binMsg = rand() % 2;
            message = binMsg ? switchToTorus32(1, M) : -switchToTorus32(1, M);
            // message = switchToTorus32(j, trials);
            sample = lweEncrypt(&message, DEF_TLWE_ALPHA, &lwekey);

            // bootstrapinglwe2lwe(&result, &sample, &gk);

            // testvect[0][*] = 0, testvect[1][*] = 1
            for (int i = 0; i < DEF_N; i++)
                testvect.getB()->setCoefficient(i, switchToTorus32(1, 8));

            start = std::chrono::system_clock::now();
            // ----------------------------------------------------------
            blindRotate(&tlweresult, &testvect, &sample, &gk);
            tlweSampleIndexExtract(&extractionResult, &tlweresult, 0, tlweParams);
            // ----------------------------------------------------------
            end = std::chrono::system_clock::now();
            diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            avgTime += diff;
            maxTime = (diff > maxTime) ? diff : maxTime;
            minTime = (diff < minTime) ? diff : minTime;

            decrypt = lweDecrypt(&extractionResult, tgswkey.getTLWEKey(), M);

            if (message != decrypt)
            {
                // std::cout << message << " v " << decrypt << std::endl;
                // Torus32 phase = lwePhase(&extractionResult, tgswkey.getTLWEKey());
                // std::cout << "phase " << j << " = " << t32tod(phase) << std::endl;
                err++;
            }

            // std::cout << message << " v " << decrypt << std::endl;

            // assert(message == decrypt);
            // std::cout << "|" << j << "| "
            //           << "Successfully rotated!"
            //           << std::endl;

            // keySwitch(&result, &extractionResult, &gk);
            // decrypt = lweDecrypt(&result, &lwekey, M);

            // if (message != decrypt)
            // {
            //     // std::cout << binMsg << " v " << decrypt << std::endl;
            //     err++;
            // }
            // std::cout << message << " v " << decrypt << std::endl;

            // assert(message == decrypt);
            // std::cout << "Successfully bootstrapped!\n" << std::endl;
        }
        std::cout << "Bootstrapping function execution time[s]\nAverage: " << avgTime / trials / tgswparams1024.size() * 1e-6 / 2 << std::endl;
        std::cout << "Max: " << maxTime * 1e-6 << std::endl;
        std::cout << "Min: " << minTime * 1e-6 << std::endl;

        std::cout << "\n-----MAX ADDITION TEST-----\n";

        Torus32 msg0 = switchToTorus32(0, M);
        Torus32 msg1 = switchToTorus32(1, M);
        LWESample encrypt0;
        LWESample encrypt1;
        LWESample encrypt0Duo;
        LWESample resultLoop{tlweParams->getLWEParams()};
        LWESample resultExtraction(tlweParams->getLWEParams());
        TLWESample resultRotation(tlweParams);
        Torus32 res = 0;
        err = 0;
        int loops = 1;
        int avgloops = 0;
        int minloops = 99999;
        int maxloops = 0;
        int tmp = 1;
        bool condition;
        bool conditionBootstrapp;
        for (int i = 0; i < DEF_N; i++)
            testvect.getB()->setCoefficient(i, switchToTorus32(1, M));

        for (int trial = 0; trial < trials; trial++)
        {
            lweClear(&encrypt0, tlweParams->getLWEParams());
            lweClear(&encrypt1, tlweParams->getLWEParams());
            lweClear(&encrypt0Duo, tlweParams->getLWEParams());
            lweClear(&resultExtraction, tlweParams->getLWEParams());
            tlweClear(&resultRotation, tlweParams);

            encrypt0 = lweEncrypt(&msg0, alpha, &lwekey);
            encrypt1 = lweEncrypt(&msg1, alpha, &lwekey);
            condition = true;
            conditionBootstrapp = true;
            tmp = 1;
            loops = 1;
            // while (conditionBootstrapp)
            // {
            while (condition)
            {
                // ----------------------------------------------------------
                lweAdd(&encrypt1, &encrypt0, tlweParams->getLWEParams());
                // ----------------------------------------------------------
                res = lweDecrypt(&encrypt1, &lwekey, M);

                if (res != msg1)
                {
                    condition = false;
                    // std::cout << "|" << tmp << "|\n\t" << switchFromTorus32(res, M) << " v " << msg0 % M << std::endl
                    //           << "\t" << res << " v " << switchToTorus32(msg0, M) << std::endl;
                }

                if (!condition)
                    break;

                lweCopy(&resultLoop, &encrypt1, tlweParams->getLWEParams());
                tmp++;
            }
            // std::cout << "|" << trial <<"|\nMaximum number of addition operations is: " << tmp << std::endl;
            // std::cout << "Begin bootrastapping..." << std::endl; // Too low noise, check it later

            // TLWESample - One operation before encryption is not correct
            blindRotate(&resultRotation, &testvect, &resultLoop, &gk);
            tlweSampleIndexExtract(&resultExtraction, &resultRotation, 0, tlweParams);
            decrypt = lweDecrypt(&resultExtraction, tgswkey.getTLWEKey(), M);
            
            if (msg1 != decrypt)
            {
                // std::cout << "Wrong bootstrapping result!\n";
                std::cout << msg1 << " v " << decrypt << std::endl;
                conditionBootstrapp = false;
            }

            if (!conditionBootstrapp)
                break;

            // Because key is different we need to create new LWESample{0} for next addition loop
            encrypt0Duo = lweEncrypt(&msg0, alpha, tgswkey.getTLWEKey());
            // std::cout << "DONE." << std::endl;

            while (conditionBootstrapp)
            {
                // ----------------------------------------------------------
                lweAdd(&resultExtraction, &encrypt0Duo, tlweParams->getLWEParams());
                // ----------------------------------------------------------
                res = lweDecrypt(&resultExtraction, tgswkey.getTLWEKey(), M);

                if (res != msg1)
                    conditionBootstrapp = false;

                if (!conditionBootstrapp)
                    break;

                loops++;
            }
            // std::cout << "Maximum number of addition operations with bootstrapping is: " << loops + tmp << std::endl;
            // }
            avgloops += loops + tmp;
            maxloops = ((loops + tmp) > maxloops) ? (loops + tmp) : maxloops;
            minloops = ((loops + tmp) < minloops) ? (loops + tmp) : minloops;
        }
        std::cout << "\n-------- SUMMARIZATION --------\n";
        std::cout << "Number of add operations with bootstrapping:\nAverage: " << avgloops / trials << std::endl;
        std::cout << "Maximum: " << maxloops << std::endl;
        std::cout << "Minimum: " << minloops << std::endl;
    }
    std::cout << "Total errors: " << err << std::endl;

    return 0;
}
