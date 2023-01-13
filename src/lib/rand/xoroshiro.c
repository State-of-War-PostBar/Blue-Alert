/*************************************************************************************************
*                                                                                                *
*                                         [ Blue Alert ]                                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                              A free, open-source indie RTS game.                               *
*               (ɔ) 2017 - 2023 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*           Blue Alert is a free software. You can freely do whatever you want with it           *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                  published by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.                  *
*                                                                                                *
*            Blue Alert is created, intended to be useful, but without any warranty.             *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*                 You should have received a copy of the license along with the                  *
*                        source code of this program. If not, please see                         *
*           <https://github.com/State-of-War-PostBar/Blue-Alert/blob/master/LICENSE>.            *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                     <https://github.com/State-of-War-PostBar/Blue-Alert>.                      *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#include "xoroshiro.h"

#include "../data/bytes.h"

static const uint64_t BLRT_RNG_XOROSHIRO_SEED_SALT = 0x1070601686ULL;

thread_local static uint64_t blrt_rng_xoroshiro_space[4];

void
blrt_Rng_Xoroshiro_Init( uint64_t seed_high, uint64_t seed_low )
{
    uint64_t seed_2 = 1ULL, seed_3 = 1ULL;
    seed_high += BLRT_RNG_XOROSHIRO_SEED_SALT;
    seed_2 = blrt_SwapEndian64(seed_high) ^ seed_low;
    seed_3 = blrt_SwapEndian64(seed_low) ^ seed_high;
    seed_low += BLRT_RNG_XOROSHIRO_SEED_SALT;
    blrt_rng_xoroshiro_space[0] = seed_high;
    blrt_rng_xoroshiro_space[1] = seed_2;
    blrt_rng_xoroshiro_space[2] = seed_3;
    blrt_rng_xoroshiro_space[3] = seed_low;
}

uint64_t
blrt_Rng_Xoroshiro_Next( void )
{
    uint64_t result = blrt_RotateLeft64(blrt_rng_xoroshiro_space[1] * 5, 7) * 9;
    uint64_t temp = blrt_rng_xoroshiro_space[1] << 17;

    blrt_rng_xoroshiro_space[2] ^= blrt_rng_xoroshiro_space[0];
    blrt_rng_xoroshiro_space[3] ^= blrt_rng_xoroshiro_space[1];
    blrt_rng_xoroshiro_space[1] ^= blrt_rng_xoroshiro_space[2];
    blrt_rng_xoroshiro_space[0] ^= blrt_rng_xoroshiro_space[3];

    blrt_rng_xoroshiro_space[2] ^= temp;
    blrt_rng_xoroshiro_space[3] = blrt_RotateLeft64(blrt_rng_xoroshiro_space[3], 45);

    return result;
}

uint64_t
blrt_Rng_Xoroshiro_Ranged( uint64_t limit )
{
    return blrt_Rng_Xoroshiro_Next() % limit;
}
