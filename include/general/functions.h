#pragma once
#include "../tfhe.h"

// Conversion between Torus32 and double
Torus32 dtot32(double message);
double t32tod(Torus32 message);

// | frac(x) |
double absfrac(double d);

/**
 * @brief   Add gaussian noise with signa as standard deviation to the message
 * 
 * @param message   'Clean' Message
 * @param sigma     Std deviation for gaussian distribution
 * @return Torus32  Noisy message
 */
Torus32 addGaussianNoise(Torus32 *message, double sigma);

/**
 * @brief           Return aproximated phase using Msize val
 * 
 * @param phase     Calculated phase, needed aproximation
 * @param Msize     Message space we are working on
 * @return          Aproximated phase - the expected result with some small noise
 */
Torus32 approxPhase(Torus32 phase, int32_t Msize);

/**
 * @brief 
 * 
 * @param message   Message which we want to encrypt 
 * @param Msize     Message space, neccesary for aproximate the phase
 * @return Torus32 
 */
Torus32 switchToTorus32(int32_t message, int32_t Msize);
