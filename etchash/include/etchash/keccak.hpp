// etchash: C/C++ implementation of etchash, the Ethereum Proof of Work algorithm.
// Copyright 2018-2019 Pawel Bylica.
// Licensed under the Apache License, Version 2.0.

#pragma once

#include <etchash/keccak.h>
#include <etchash/hash_types.hpp>

namespace etchash
{
inline hash256 keccak256(const uint8_t* data, size_t size) noexcept
{
    return etchash_keccak256(data, size);
}

inline hash256 keccak256(const hash256& input) noexcept
{
    return etchash_keccak256_32(input.bytes);
}

inline hash512 keccak512(const uint8_t* data, size_t size) noexcept
{
    return etchash_keccak512(data, size);
}

inline hash512 keccak512(const hash512& input) noexcept
{
    return etchash_keccak512_64(input.bytes);
}

static constexpr auto keccak256_32 = etchash_keccak256_32;
static constexpr auto keccak512_64 = etchash_keccak512_64;

}  // namespace etchash
