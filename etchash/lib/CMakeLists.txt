# etchash: C/C++ implementation of etchash, the Ethereum Proof of Work algorithm.
# Copyright 2019 Pawel Bylica.
# Licensed under the Apache License, Version 2.0.

add_subdirectory(keccak)
add_subdirectory(etchash)

get_filename_component(ETCHASH_PRIVATE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
set(ETCHASH_PRIVATE_INCLUDE_DIR ${ETCHASH_PRIVATE_INCLUDE_DIR} PARENT_SCOPE)
