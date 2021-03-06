/* etchash: C/C++ implementation of etchash, the Ethereum Proof of Work algorithm.
 * Copyright 2018-2019 Pawel Bylica.
 * Licensed under the Apache License, Version 2.0.
 */

#pragma once

#include <etchash/hash_types.h>

#include <stddef.h>

#ifdef __cplusplus
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The Keccak-f[800] function.
 *
 * The implementation of the Keccak-f function with 800-bit width of the permutation (b).
 * The size of the state is also 800 bit what gives 25 32-bit words.
 *
 * @param state  The state of 25 32-bit words on which the permutation is to be performed.
 */
void etchash_keccakf800(uint32_t state[25]) NOEXCEPT;

union etchash_hash256 etchash_keccak256(const uint8_t* data, size_t size) NOEXCEPT;
union etchash_hash256 etchash_keccak256_32(const uint8_t data[32]) NOEXCEPT;
union etchash_hash512 etchash_keccak512(const uint8_t* data, size_t size) NOEXCEPT;
union etchash_hash512 etchash_keccak512_64(const uint8_t data[64]) NOEXCEPT;

#ifdef __cplusplus
}
#endif
