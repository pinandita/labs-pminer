/* Copyright (C) 1883 Thomas Edison - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the GPLv3 license, which unfortunately won't be
 * written for another century.
 *
 * You should have received a copy of the LICENSE file with
 * this file.
 */

#include "ethash_cuda_miner_kernel_globals.h"

#include "ethash_cuda_miner_kernel.h"

#include "cuda_helper.h"

#define _PARALLEL_HASH 4

DEV_INLINE bool compute_hash(uint64_t nonce) {
    // sha3_512(header .. nonce)
    uint2 state[12];

    state[4] = vectorize(nonce);

    keccak_f1600_init(state);

    // Threads work together in this phase in groups of 8.
    const int thread_id = threadIdx.x & (THREADS_PER_HASH - 1);
    const int mix_idx = (thread_id & 3) * 2;

    for (int i = 0; i < THREADS_PER_HASH; i += _PARALLEL_HASH) {
        uint4 mix[_PARALLEL_HASH]{};
        uint32_t offset[_PARALLEL_HASH]{};
        uint32_t init0[_PARALLEL_HASH]{};

        // share init among threads
        for (int p = 0; p < _PARALLEL_HASH; p++) {
            uint2 shuffle[8]{};
            for (int j = 0; j < 8; j++) {
                shuffle[j].x = SHFL(state[j].x, i + p, THREADS_PER_HASH);
                shuffle[j].y = SHFL(state[j].y, i + p, THREADS_PER_HASH);
            }
            mix[p] = vectorize2(shuffle[mix_idx], shuffle[(mix_idx + 1)]);
            init0[p] = SHFL(shuffle[0].x, 0, THREADS_PER_HASH);
        }

        for (int a = 0; a < ACCESSES; a += 4) {
            int t = bfe(a, 2u, 3u);

            for (uint32_t b = 0; b < 4; b++) {
                for (int p = 0; p < _PARALLEL_HASH; p++) {
                    offset[p] = fnv(init0[p] ^ (a + b), ((uint32_t*)&mix[p])[b]) % d_dag_size;
                    offset[p] = SHFL(offset[p], t, THREADS_PER_HASH);
                    mix[p] = fnv4(mix[p], d_dag[offset[p]].uint4s[thread_id]);
                }
            }
        }

        for (int p = 0; p < _PARALLEL_HASH; p++) {
            uint2 shuffle[4]{};
            uint32_t thread_mix = fnv_reduce(mix[p]);

            // update mix across threads
            shuffle[0].x = SHFL(thread_mix, 0, THREADS_PER_HASH);
            shuffle[0].y = SHFL(thread_mix, 1, THREADS_PER_HASH);
            shuffle[1].x = SHFL(thread_mix, 2, THREADS_PER_HASH);
            shuffle[1].y = SHFL(thread_mix, 3, THREADS_PER_HASH);
            shuffle[2].x = SHFL(thread_mix, 4, THREADS_PER_HASH);
            shuffle[2].y = SHFL(thread_mix, 5, THREADS_PER_HASH);
            shuffle[3].x = SHFL(thread_mix, 6, THREADS_PER_HASH);
            shuffle[3].y = SHFL(thread_mix, 7, THREADS_PER_HASH);

            if ((i + p) == thread_id) {
                // move mix into state:
                state[8] = shuffle[0];
                state[9] = shuffle[1];
                state[10] = shuffle[2];
                state[11] = shuffle[3];
            }
        }
    }

    // keccak_256(keccak_512(header..nonce) .. mix);
    if (cuda_swab64(keccak_f1600_final(state)) > d_target)
        return true;

    return false;
}
