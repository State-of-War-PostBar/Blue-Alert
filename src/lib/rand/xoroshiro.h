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

#ifndef BLRT_LIB_RAND_XOROSHIRO_H
#define BLRT_LIB_RAND_XOROSHIRO_H

#include <pch.h>

/*
    Xoroshiro 256** PRNG
    By Vigna, Sebastiano / Blackman, David

    Credit: https://prng.di.unimi.it
*/

///
/// \brief Initialize the rng
///
/// Initialize the Xoroshiro256** with a 128-bit seed.
///
/// \note States are independent for threads, so make sure to call this for every thread!
///
/// \param seed_high Higher bits of the seed
/// \param seed_low Lower bits of the seed
///
void
blrt_Rng_Xoroshiro_Init( uint64_t seed_high, uint64_t seed_low );

///
/// \brief Get next rng
///
/// Get next linear sequence. Xoroshiro256** is safe for 2^64 calls.
///
/// \return A random number
///
uint64_t
blrt_Rng_Xoroshiro_Next( void );

///
/// \brief Get a ranged rng
///
/// Get a rng in [0, limit). <i>Be wary of % bias.</i>
///
/// \param limit Max of the number
///
/// \return A random number
///
uint64_t
blrt_Rng_Xoroshiro_Ranged( uint64_t limit );

#endif // !BLRT_LIB_RAND_XOROSHIRO_H
