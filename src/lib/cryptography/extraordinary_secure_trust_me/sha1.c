/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (ɔ) 2017 - 2022 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*    State of War: Remastered is a free software. You can freely do whatever you want with it    *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                  published by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.                  *
*                                                                                                *
*  By the time this line is written, the version of the license document is 1, but you may use   *
*                  any later version of the document released by mhtvsSFrpHdE.                   *
*                                                                                                *
*     State of War: Remastered is created, intended to be useful, but without any warranty.      *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*                 You should have received a copy of the license along with the                  *
*                        source code of this program. If not, please see                         *
*              <https://github.com/State-of-War-PostBar/sowr/blob/master/LICENSE>.               *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                        <https://github.com/State-of-War-PostBar/sowr>.                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#include "sha1.h"

#include "../../data/bytes.h"

static
uint32_t
sowr_SHA1_Ch( uint32_t x, uint32_t y, uint32_t z )
{
    return (x & y) ^ ((~x) & z);
}

static
uint32_t
sowr_SHA1_Parity( uint32_t x, uint32_t y, uint32_t z )
{
    return x ^ y ^ z;
}

static
uint32_t
sowr_SHA1_Maj( uint32_t x, uint32_t y, uint32_t z )
{
    return (x & y) ^ (x & z) ^ (y & z);
}

static
const uint32_t (*SOWR_SHA1_CYCLE_FUNC[])(uint32_t, uint32_t, uint32_t) =
{
    sowr_SHA1_Ch,
    sowr_SHA1_Parity,
    sowr_SHA1_Maj,
    sowr_SHA1_Parity
};

static
const uint32_t SOWR_SHA1_CYCLE_CONSTANT[] =
{
    0x5a827999,
    0x6ed9eba1,
    0x8f1bbcdc,
    0xca62c1d6
};

static
inline
void
sowr_SHA1_MainCycle( uint32_t (*hash_value)[5], const uint32_t (*data_block)[16] )
{
    bool little_endian = sowr_IsLittleEndian();
    uint32_t window[80];

    for (int i = 0; i < 16; i++)
        window[i] = little_endian ? sowr_SwapEndian32((*data_block)[i]): (*data_block)[i];
    for (int j = 16; j < 80; j++)
        window[j] = sowr_RotateLeft32(window[j - 3] ^ window[j - 8] ^ window[j - 14] ^ window[j - 16], 1);

    uint32_t a = (*hash_value)[0],
             b = (*hash_value)[1],
             c = (*hash_value)[2],
             d = (*hash_value)[3],
             e = (*hash_value)[4];
    uint32_t t = 0;

    for (int p = 0; p < 80; p++)
    {
        uint32_t f = SOWR_SHA1_CYCLE_FUNC[p / 20](b, c, d),
                 k = SOWR_SHA1_CYCLE_CONSTANT[p / 20];

        t = sowr_RotateLeft32(a, 5) + f + e + k + window[p];
        e = d;
        d = c;
        c = sowr_RotateLeft32(b, 30);
        b = a;
        a = t;
    }

    (*hash_value)[0] += a;
    (*hash_value)[1] += b;
    (*hash_value)[2] += c;
    (*hash_value)[3] += d;
    (*hash_value)[4] += e;
}

sowr_SHA1
sowr_SHA1_Generate( uint64_t length, const unsigned char *data )
{
    sowr_SHA1 result;
    bool little_endian = sowr_IsLittleEndian();
    size_t nonfill_cycles = length / 64;
    size_t leftover_size = length % 64;

    uint64_t length_bit = length * CHAR_BIT;
    if (little_endian)
        length_bit = sowr_SwapEndian64(length_bit);

    uint32_t hash_value[5] =
    {
        0x67452301,
        0xefcdab89,
        0x98badcfe,
        0x10325476,
        0xc3d2e1f0
    };

    const unsigned char *data_ptr = data;
    for (size_t i = 0; i < nonfill_cycles; i++)
    {
        sowr_SHA1_MainCycle(&hash_value, (uint32_t (*)[16]) data_ptr);
        data_ptr += 64 * sizeof(char);
    }

    uint32_t data_block[16] = { 0 };
    unsigned char *data_block_ptr = (unsigned char *) data_block;
    if (leftover_size > 55)
    {
        //memset(data_block_ptr, 0, 64 * sizeof(char));
        memcpy(data_block_ptr, data_ptr, leftover_size);
        memcpy(data_block_ptr + leftover_size, &(uint8_t){ 0x80 }, sizeof(uint8_t));

        sowr_SHA1_MainCycle(&hash_value, &data_block);

        memset(data_block_ptr, 0, 56 * sizeof(char));
        memcpy(data_block_ptr + 56 * sizeof(char), &length_bit, sizeof(uint64_t));

        sowr_SHA1_MainCycle(&hash_value, &data_block);
    }
    else
    {
        //memset(data_block_ptr, 0, 56 * sizeof(char));
        memcpy(data_block_ptr, data_ptr, leftover_size);
        memcpy(data_block_ptr + leftover_size, &(uint8_t){ 0x80 }, sizeof(uint8_t));
        memcpy(data_block_ptr + 56 * sizeof(char), &length_bit, sizeof(uint64_t));

        sowr_SHA1_MainCycle(&hash_value, &data_block);
    }

    for (unsigned int i = 0; i < 5; i++)
        result.dword[i] = little_endian ? sowr_SwapEndian32(hash_value[i]) : hash_value[i];

    return result;
}
