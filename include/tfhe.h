/**
 * @file tfhe.h
 * @author Konrad Konicki
 * @brief Just include this file to have access to all tfhe functions
 * @version 0.1
 * @date 2021-07-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

// General variables
#include "tfhe_core.h"

// Polynomials
#include "general/functions.h"
#include "general/polyfunctions.h"

// LWE functions (Key generation, Encryption, Decryption)
#include "lwe/LWEKey.h"
#include "lwe/functions.h"

// TLWE functions
#include "tlwe/functions.h"
#include "tlwe/TLWEParams.h"
#include "tlwe/TLWEKey.h"
#include "tlwe/TLWESample.h"

// TGSW functions
#include "tgsw/functions.h"
#include "tgsw/TGSWParams.h"
#include "tgsw/TGSWKey.h"
#include "tgsw/TGSWSample.h"
#include "tgsw/TGSWSampleFft.h"

// Bootstrapping and KeySwitch structures
#include "general/blocks.h"
