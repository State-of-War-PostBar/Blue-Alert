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

#include "sha256.h"

#include "../data/bytes.h"
#include "../memory/heap_memory.h"

static
const uint32_t SOWR_SHA256_K[] =
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static
inline
uint32_t
sowr_SHA256_Ch( uint32_t x, uint32_t y, uint32_t z )
{
    return (x & y) ^ ((~x) & z);
}

static
inline
uint32_t
sowr_SHA256_Maj( uint32_t x, uint32_t y, uint32_t z )
{
    return (x & y) ^ (x & z) ^ (y & z);
}

static
inline
uint32_t
sowr_SHA256_Sig0( uint32_t x )
{
    return sowr_RotateRight32(x, 2) ^ sowr_RotateRight32(x, 13) ^ sowr_RotateRight32(x, 22);
}

static
inline
uint32_t
sowr_SHA256_Sig1( uint32_t x )
{
    return sowr_RotateRight32(x, 6) ^ sowr_RotateRight32(x, 11) ^ sowr_RotateRight32(x, 25);
}

static
inline
uint32_t
sowr_SHA256_sig0( uint32_t x )
{
    return sowr_RotateRight32(x, 7) ^ sowr_RotateRight32(x, 18) ^ (x >> 3);
}

static
inline
uint32_t
sowr_SHA256_sig1( uint32_t x )
{
    return sowr_RotateRight32(x, 17) ^ sowr_RotateRight32(x, 19) ^ (x >> 10);
}

static
inline
void
sowr_SHA256_MainCycle( uint32_t (*hash_value)[8], uint32_t (*data_block)[16] )
{
    bool little_endian = sowr_IsLittleEndian();
    uint32_t window[64];

    for (int i = 0; i < 16; i++)
        window[i] = little_endian ? sowr_SwapEndian32((*data_block)[i]) : (*data_block)[i];
    for (int j = 16; j < 64; j++)
        window[j] = sowr_SHA256_sig1(window[j - 2]) + window[j - 7] + sowr_SHA256_sig0(window[j - 15]) + window[j - 16];

    uint32_t a = (*hash_value)[0],
             b = (*hash_value)[1],
             c = (*hash_value)[2],
             d = (*hash_value)[3],
             e = (*hash_value)[4],
             f = (*hash_value)[5],
             g = (*hash_value)[6],
             h = (*hash_value)[7];
    uint32_t t1 = 0, t2 = 0;

    for (int k = 0; k < 64; k++)
    {
        t1 = h + sowr_SHA256_Sig1(e) + sowr_SHA256_Ch(e, f, g) + SOWR_SHA256_K[k] + window[k];
        t2 = sowr_SHA256_Sig0(a) + sowr_SHA256_Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    (*hash_value)[0] += a;
    (*hash_value)[1] += b;
    (*hash_value)[2] += c;
    (*hash_value)[3] += d;
    (*hash_value)[4] += e;
    (*hash_value)[5] += f;
    (*hash_value)[6] += g;
    (*hash_value)[7] += h;
}

sowr_SHA256
sowr_SHA256_Generate( uint64_t length, const unsigned char *data )
{
    sowr_SHA256 result;
    bool little_endian = sowr_IsLittleEndian();
    size_t nonfill_cycles = length / 64;
    size_t leftover_size = length % 64;

    uint64_t length_bit = length * CHAR_BIT;
    if (little_endian)
        length_bit = sowr_SwapEndian64(length_bit);

    uint32_t hash_value[8] =
    {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19
    };

    const unsigned char *data_ptr = data;
    for (size_t i = 0; i < nonfill_cycles; i++)
    {
        sowr_SHA256_MainCycle(&hash_value, (uint32_t (*)[16]) data_ptr);
        data_ptr += 64 * sizeof(char);
    }

    uint32_t data_block[16] = { 0 };
    unsigned char *data_block_ptr = (unsigned char *) data_block;
    if (leftover_size > 55)
    {
        //memset(data_block_ptr, 0, 64 * sizeof(char));
        memcpy(data_block_ptr, data_ptr, leftover_size);
        memcpy(data_block_ptr + leftover_size, &(uint8_t){ 0x80 }, sizeof(uint8_t));

        sowr_SHA256_MainCycle(&hash_value, &data_block);

        memset(data_block_ptr, 0, 56 * sizeof(char));
        memcpy(data_block_ptr + 56 * sizeof(char), &length_bit, sizeof(uint64_t));

        sowr_SHA256_MainCycle(&hash_value, &data_block);
    }
    else
    {
        //memset(data_block_ptr, 0, 64 * sizeof(char));
        memcpy(data_block_ptr, data_ptr, leftover_size);
        memcpy(data_block_ptr + leftover_size, &(uint8_t){ 0x80 }, sizeof(uint8_t));
        memcpy(data_block_ptr + 56 * sizeof(char), &length_bit, sizeof(uint64_t));

        sowr_SHA256_MainCycle(&hash_value, &data_block);
    }

    for (int i = 0; i < 8; i++)
        result.dword[i] = little_endian ? sowr_SwapEndian32(hash_value[i]) : hash_value[i];

    return result;
}
