/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (c) 2017 - 2021 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*    State of War: Remastered is a free software. You can freely do whatever you want with it    *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                                   published by mhtvsSFrpHdE.                                   *
*                                                                                                *
*  By the time this line is written, the version of the license document is 1, but you may use   *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
*     State of War: Remastered is created, intended to be useful, but without any warranty.      *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*       You should have received a copy of the license along with the source code of this        *
*  program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.   *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                         https://github.com/State-of-War-PostBar/sowr.                          *
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#include "xoroshiro.h"

#include "../data/bytes.h"

static const uint64_t SOWR_RNG_XOROSHIRO_SEED_SALT = 0xacbcdc10706ULL;

thread_local static uint64_t sowr_rng_xoroshiro_space[4];

static inline uint64_t rotate(uint64_t x, int k)
{
    return (x << k) | (x >> (64 - k));
}

void
sowr_Rng_Xoroshiro_Init( uint64_t seed_hi, uint64_t seed_lo )
{
    uint64_t seed_2 = 0ULL, seed_3 = 0ULL;
    seed_hi ^= SOWR_RNG_XOROSHIRO_SEED_SALT;
    seed_2 = sowr_SwapEndian64(seed_hi) ^ seed_lo;
    seed_3 = sowr_SwapEndian64(seed_lo) ^ seed_hi;
    seed_lo ^= SOWR_RNG_XOROSHIRO_SEED_SALT;
    sowr_rng_xoroshiro_space[0] = seed_hi;
    sowr_rng_xoroshiro_space[1] = seed_2;
    sowr_rng_xoroshiro_space[2] = seed_3;
    sowr_rng_xoroshiro_space[3] = seed_lo;
}

uint64_t
sowr_Rng_Xoroshiro_Next( void )
{
    uint64_t rresult = rotate(sowr_rng_xoroshiro_space[1] * 5, 7) * 9;
    uint64_t temp = sowr_rng_xoroshiro_space[1] << 17;

    sowr_rng_xoroshiro_space[2] ^= sowr_rng_xoroshiro_space[0];
    sowr_rng_xoroshiro_space[3] ^= sowr_rng_xoroshiro_space[1];
    sowr_rng_xoroshiro_space[1] ^= sowr_rng_xoroshiro_space[2];
    sowr_rng_xoroshiro_space[0] ^= sowr_rng_xoroshiro_space[3];

    sowr_rng_xoroshiro_space[2] ^= temp;
    sowr_rng_xoroshiro_space[3] = rotate(sowr_rng_xoroshiro_space[3], 45);

    return rresult;
}

uint64_t
sowr_Rng_Xoroshiro_Ranged( uint64_t limit )
{
    return sowr_Rng_Xoroshiro_Next() % limit;
}
