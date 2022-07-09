/* ethash: C/C++ implementation of Ethash, the Ethereum Proof of Work algorithm.
 * Copyright 2018-2019 Pawel Bylica.
 * Licensed under the Apache License, Version 2.0.
 */

#pragma once

#include <etchash/hash_types.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The Ethash algorithm revision implemented as specified in the Ethash spec
 * https://github.com/ethereum/wiki/wiki/Ethash.
 */
#define ETCHASH_REVISION "23"

#define ETHASH_EPOCH_LENGTH 30000
#define ETHASH_LIGHT_CACHE_ITEM_SIZE 64
#define ETHASH_FULL_DATASET_ITEM_SIZE 128
#define ETHASH_NUM_DATASET_ACCESSES 64

#define ETCHASH_EPOCH_LENGTH 60000
#define ETCHASH_ACTIVATION_BLOCK 11700000


struct etchash_epoch_context
{
    const int epoch_number;
    const int light_cache_num_items;
    const union etchash_hash512* const light_cache;
    const uint32_t* const l1_cache;
    const int full_dataset_num_items;
};


struct etchash_epoch_context_full;


struct etchash_result
{
    union etchash_hash256 final_hash;
    union etchash_hash256 mix_hash;
};


/**
 * Calculates the number of items in the light cache for given epoch.
 *
 * This function will search for a prime number matching the criteria given
 * by the Ethash so the execution time is not constant. It takes ~ 0.01 ms.
 *
 * @param epoch_number  The epoch number.
 * @return              The number items in the light cache.
 */
int etchash_calculate_light_cache_num_items(int epoch_number) NOEXCEPT;


/**
 * Calculates the number of items in the full dataset for given epoch.
 *
 * This function will search for a prime number matching the criteria given
 * by the Ethash so the execution time is not constant. It takes ~ 0.05 ms.
 *
 * @param epoch_number  The epoch number.
 * @return              The number items in the full dataset.
 */
int etchash_calculate_full_dataset_num_items(int epoch_number) NOEXCEPT;

/**
 * Calculates the epoch seed hash.
 * @param epoch_number  The epoch number.
 * @return              The epoch seed hash.
 */
union etchash_hash256 etchash_calculate_epoch_seed(int epoch_number) NOEXCEPT;


struct etchash_epoch_context* etchash_create_epoch_context(int epoch_number) NOEXCEPT;

/**
 * Creates the epoch context with the full dataset initialized.
 *
 * The memory for the full dataset is only allocated and marked as "not-generated".
 * The items of the full dataset are generated on the fly when hit for the first time.
 *
 * The memory allocated in the context MUST be freed with ethash_destroy_epoch_context_full().
 *
 * @param epoch_number  The epoch number.
 * @return  Pointer to the context or null in case of memory allocation failure.
 */
struct etchash_epoch_context_full* etchash_create_epoch_context_full(int epoch_number) NOEXCEPT;

void etchash_destroy_epoch_context(struct etchash_epoch_context* context) NOEXCEPT;

void etchash_destroy_epoch_context_full(struct etchash_epoch_context_full* context) NOEXCEPT;


/**
 * Get global shared epoch context.
 */
const struct etchash_epoch_context* etchash_get_global_epoch_context(int epoch_number) NOEXCEPT;

/**
 * Get global shared epoch context with full dataset initialized.
 */
const struct etchash_epoch_context_full* etchash_get_global_epoch_context_full(
    int epoch_number) NOEXCEPT;


struct etchash_result etchash_hash(const struct etchash_epoch_context* context,
    const union etchash_hash256* header_hash, uint64_t nonce) NOEXCEPT;

bool etchash_verify(const struct etchash_epoch_context* context,
    const union etchash_hash256* header_hash, const union etchash_hash256* mix_hash, uint64_t nonce,
    const union etchash_hash256* boundary) NOEXCEPT;

bool etchash_verify_final_hash(const union etchash_hash256* header_hash,
    const union etchash_hash256* mix_hash, uint64_t nonce,
    const union etchash_hash256* boundary) NOEXCEPT;

#ifdef __cplusplus
}
#endif
