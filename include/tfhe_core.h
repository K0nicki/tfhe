/**
 * @file tfhe_core.h
 * @author Konrad Konicki
 * @brief This file contains the global tfhe variables (???)
 * @date 2021-07-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdint.h>

/**
 * The main idea is that we can represent the x number as real Torus element
 * by int(2^32*x) mod 2^32
 * 
 * Pros:
 *  - Add, Sub are natively supported
 *  - Modulo 1 is mapped to mod 2^32
 * 
 */
typedef int32_t Torus32;