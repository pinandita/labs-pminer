/* etchash: C/C++ implementation of etchash, the Ethereum Proof of Work algorithm.
 * Copyright 2019 Pawel Bylica.
 * Licensed under the Apache License, Version 2.0.
 */

#pragma once

/** The etchash library version. */
#define ETCHASH_VERSION "0.6.0"

#ifdef __cplusplus
namespace etchash
{
/// The etchash library version.
constexpr auto version = ETCHASH_VERSION;

}  // namespace etchash
#endif
