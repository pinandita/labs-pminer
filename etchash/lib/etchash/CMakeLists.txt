# etchash: C/C++ implementation of etchash, the Ethereum Proof of Work algorithm.
# Copyright 2018-2019 Pawel Bylica.
# Licensed under the Apache License, Version 2.0.

add_library(etchash)
add_library(etchash::etchash ALIAS etchash)
target_link_libraries(etchash PRIVATE etchash::keccak)
target_include_directories(etchash PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_sources(etchash PRIVATE
    bit_manipulation.h
    builtins.h
    endianness.hpp
    ${PROJECT_SOURCE_DIR}/include/etchash/ethash.h
    ${PROJECT_SOURCE_DIR}/include/etchash/ethash.hpp
    ethash-internal.hpp
    ethash.cpp
    ${PROJECT_SOURCE_DIR}/include/etchash/hash_types.h
    managed.cpp
    kiss99.hpp
    primes.h
    primes.c
)


if(CABLE_COMPILER_GNULIKE AND NOT SANITIZE MATCHES undefined)
    target_compile_options(etchash PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
endif()
