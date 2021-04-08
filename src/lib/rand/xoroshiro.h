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

#ifndef SOWR_LIB_RAND_XOROSHIRO_H
#define SOWR_LIB_RAND_XOROSHIRO_H

#include <pch.h>

///
/// \brief Initialize the rng
///
/// Initialize the Xoroshiro256** with a 128-bit seed.
///
/// \note States are independent for threads, so make sure to call this for every thread!
///
/// \param seed_hi Higher bits of the seed
/// \param seed_lo Lower bits of the seed
///
void
sowr_Rng_Xoroshiro_Init( uint64_t seed_hi, uint64_t seed_lo );

///
/// \brief Get next rng
///
/// Get next linear sequence. Xoroshiro256** is safe for 2^64 calls.
///
/// \return A random number
///
uint64_t
sowr_Rng_Xoroshiro_Next( void );

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
sowr_Rng_Xoroshiro_Ranged( uint64_t limit );

#endif // !SOWR_LIB_RAND_XOROSHIRO_H
