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

#include "bytes.h"

bool
sowr_Endianness( void )
{
    const short num = 1;
    const signed char *const ptr = (const signed char *const) &num;
    return ptr[0] == 1;
}

void
sowr_SwapEndian( size_t length, unsigned char *bytes )
{
    // Do not "play smart" like using xor exchange.
    // It doesn't always help.
    unsigned char byte = 0;
    for (size_t i = 0ULL, j = length - 1ULL; i < j; i++, j--)
    {
        byte = bytes[i];
        bytes[i] = bytes[j];
        bytes[j] = byte;
    }
}

inline
uint16_t
sowr_SwapEndian16( uint16_t val )
{
    return (val << 010U) | (val >> 010U);
}

inline
uint32_t
sowr_SwapEndian32( uint32_t val )
{
    return (val & 0xff000000U) >>  030U |
            (val & 0x00ff0000U) >> 010U |
            (val & 0x0000ff00U) << 010U |
            (val & 0x000000ffU) << 030U;
}

inline
uint64_t
sowr_SwapEndian64( uint64_t val )
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
