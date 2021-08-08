#include <iostream>
#include <random>
#include "good-tfhe/src/include/tfhe.h"

static const int64_t _b32 = int64_t(1) << 32; // 2^32
static const double _b32_double = _b32; // 2^32

double t32tod(Torus32 message)
{
    // Return message
    return double(message) / _b32_double;
}

Torus32 dtot32(double message)
{
    return int32_t(int64_t((message - int64_t(message)) * _b32));
}

Torus32 modSwitchToTorus32(int32_t mu, int32_t Msize){
    uint64_t interv = ((UINT64_C(1)<<63)/Msize)*2; // width of each intervall
    uint64_t phase64 = mu*interv;
    //floor to the nearest multiples of interv
    return phase64>>32;
}

Torus32 switchToTorus32(int32_t message, int32_t Msize)
{
    uint64_t interval{((UINT64_C(1) << 63) / Msize)};
    uint64_t phase64{message * interval};

    // floor to the nearest possible multiples of interval
    return phase64 >> 32;
}

int main(int argc, char const *argv[])
{
    int M = 8;
    //     double alpha = 1. / (10 * M);
    //     std::normal_distribution<double> distrib(0., alpha);
    //     std::default_random_engine gen;

    //     double err = distrib(gen);
    //     int32_t message = 1;

    //     std::cout << int32_t(int64_t( (err - int32_t(err)) * (uint32_t(1) << 31) )) << std::endl;
    //     std::cout << message + int32_t(int64_t( (err - int32_t(err)) * (uint32_t(1) << 31) ));
    //     // floor to the nearest possible multiples of interval
    // return int32_t(int64_t((message - int32_t(message)) * _b32 ));

    // uint64_t val = 32;

    // std::cout<<val<<std::endl;
    // std::cout<<int32_t(val)<<std::endl;
    // std::cout<<(val - int32_t(val))<<std::endl;
    // std::cout<<int64_t((val - int32_t(val)))<<std::endl;
    // std::cout<<int32_t(int64_t((val - int32_t(val)) * (uint32_t(1) << 31)))<<std::endl;

    int32_t msg = 7;
    Torus32 message = switchToTorus32(msg, M);
    std::cout<<msg<<std::endl;
    std::cout<<message<<std::endl;
    std::cout<<t32tod(message)<<std::endl;
    std::cout<<dtot32(t32tod(message))<<std::endl;
    std::cout<<t32tod(message)<<std::endl;

    return 0;
}