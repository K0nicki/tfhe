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

// LWE functions (Key generation, Encryption, Decryption)
#include "lwe/LWEKey.h"
#include "lwe/functions.h"

#include "general/functions.h"
#include "general/polyfunctions.h"

// TLWE functions
#include "tlwe/functions.h"
