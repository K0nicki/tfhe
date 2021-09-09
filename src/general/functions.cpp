#include "../../include/general/functions.h"

static const int64_t _b32 = int64_t(1) << 32; // 2^32
static const double _b32_double = _b32; // 2^32

double absfrac(double d) { return abs(d - rint(d)); }

double t32tod(Torus32 message)
{
    // Return message
    return double(message) / _b32_double;
}

Torus32 dtot32(double message)
{
    return int32_t(int64_t((message - int64_t(message)) * _b32));
}

Torus32 addGaussianNoise(Torus32 *message, double sigma)
{
    std::normal_distribution<double> distrib(0., sigma);
    std::default_random_engine gen;

    double err{distrib(gen)};
    return *message + dtot32(err);
}

Torus32 approxPhase(Torus32 phase, int32_t Msize)
{
    uint64_t interval{((UINT64_C(1) << 63) / Msize) * 2};
    uint64_t intervalHalf{(interval / 2)};

    // Expand message from 32bits to 64bits
    uint64_t phase64{((uint64_t(phase) << 32) + intervalHalf)};
    // Round the phase to the nearest possible multiple of interval. The difference was decause of the noise so we can get rid of it
    phase64 -= (phase64 % interval);

    // Rescale to 32bits
    return int32_t(phase64 >> 32);
    /**
 * @Questions
 * Why we need expand phase to 64 bits? Why we can't do it on 32 bits space?
 * Because it was uint.
 */
}

Torus32 switchToTorus32(int32_t message, int32_t Msize)
{
    uint64_t interval{((UINT64_C(1) << 63) / Msize)*2};
    uint64_t phase64{message * interval};

    // floor to the nearest possible multiples of interval
    return phase64 >> 32;
}