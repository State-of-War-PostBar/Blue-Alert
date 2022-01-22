/*************************************************************************************************
*                                                                                                *
*                                         [ Blue Alert ]                                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                              A free, open-source indie RTS game.                               *
*               (ɔ) 2017 - 2022 State of War Baidu Postbar, some rights reserved.                *
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

#include "bytes.h"

bool
blrt_IsLittleEndian( void )
{
    static const short num = 1;
    static const signed char *const ptr = (const signed char *const) &num;
    return ptr[0] == 1;
}

inline
void
blrt_Swap( size_t sz, unsigned char *b_l, unsigned char *b_r )
{
    if (!sz)
        return;

    unsigned char b_lr[sz], b_rl[sz];
    memcpy(b_lr, b_l, sz);
    memcpy(b_rl, b_r, sz);
    memcpy(b_l, b_rl, sz);
    memcpy(b_r, b_lr, sz);
}

inline
void
blrt_SwapEndian( size_t length, unsigned char *bytes )
{
    unsigned char byte1 = 0, byte2 = 0;
    for (size_t i = 0ULL, j = length - 1ULL; i < j; i++, j--)
    {
        byte1 = bytes[i];
        byte2 = bytes[j];
        bytes[i] = byte2;
        bytes[j] = byte1;
    }
}

inline
uint16_t
blrt_SwapEndian16( uint16_t val )
{
    return (val << 010U) | (val >> 010U);
}

inline
uint32_t
blrt_SwapEndian32( uint32_t val )
{
    return (val & 0xff000000U) >>  030U |
            (val & 0x00ff0000U) >> 010U |
            (val & 0x0000ff00U) << 010U |
            (val & 0x000000ffU) << 030U;
}

inline
uint64_t
blrt_SwapEndian64( uint64_t val )
{
    return (val & 0xff00000000000000ULL) >>  070U |
            (val & 0x00ff000000000000ULL) >> 050U |
            (val & 0x0000ff0000000000ULL) >> 030U |
            (val & 0x000000ff00000000ULL) >> 010U |
            (val & 0x00000000ff000000ULL) << 010U |
            (val & 0x0000000000ff0000ULL) << 030U |
            (val & 0x000000000000ff00ULL) << 050U |
            (val & 0x00000000000000ffULL) << 070U;
}

inline
void
blrt_FlipAtomicBool( atomic_bool *val )
{
    bool old = false;
    do
    {
        old = atomic_load(val);
    } while (!atomic_compare_exchange_strong(val, &old, !old));
}

inline
uint32_t
blrt_RotateLeft32( uint32_t x, unsigned int n )
{
    return (x << n) | (x >> (32 - n));
}

inline
uint32_t
blrt_RotateRight32( uint32_t x, unsigned int n )
{
    return (x >> n) | (x << (32 - n));
}

inline
uint64_t
blrt_RotateLeft64( uint64_t x, unsigned int n )
{
    return (x << n) | (x >> (64 - n));
}

inline
uint64_t
blrt_RotateRight64( uint64_t x, unsigned int n )
{
    return (x >> n) | (x << (64 - n));
}
