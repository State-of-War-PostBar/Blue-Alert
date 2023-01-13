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

#include "aes256.h"

#include "../../../data/bytes.h"

typedef union blrt_AES256_Block
{
    uint32_t word[4];
    uint8_t matrix[4][4];
    uint8_t vec[16];
} blrt_AES256_Block;

typedef union blrt_AES256_ExpandedKey
{
    blrt_AES256_Block block[15];
    uint32_t word[4 * 15];
} blrt_AES256_ExpandedKey;

static
const uint8_t BLRT_AES256_S[16][16] =
{
    { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
    { 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
    { 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
    { 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
    { 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
    { 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
    { 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
    { 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
    { 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
    { 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
    { 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
    { 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
    { 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
    { 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
    { 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf },
    { 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
};

static
const uint8_t BLRT_AES256_IS[16][16] =
{
    { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb },
    { 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb },
    { 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e },
    { 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25 },
    { 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92 },
    { 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84 },
    { 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06 },
    { 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b },
    { 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73 },
    { 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e },
    { 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b },
    { 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4 },
    { 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f },
    { 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef },
    { 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61 },
    { 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d }
};

static
const uint8_t BLRT_AES256_RCON[16] =
{
    0x00,
    0x01, 0x02,
    0x04, 0x08,
    0x10, 0x20,
    0x40, 0x80,
    0x1b, 0x36,
    0x6c, 0xd8,
    0xab, 0xed,
    0x9a
};

static
const uint8_t BLRT_AES256_GF[4][4] =
{
    { 0x02, 0x03, 0x01, 0x01 },
    { 0x01, 0x02, 0x03, 0x01 },
    { 0x01, 0x01, 0x02, 0x03 },
    { 0x03, 0x01, 0x01, 0x02 }
};

static
const uint8_t BLRT_AES256_IGF[4][4] =
{
    { 0x0e, 0x0b, 0x0d, 0x09 },
    { 0x09, 0x0e, 0x0b, 0x0d },
    { 0x0d, 0x09, 0x0e, 0x0b },
    { 0x0b, 0x0d, 0x09, 0x0e }
};

static
uint8_t
blrt_AES256_GFMul( uint8_t a, uint8_t b )
{
    uint8_t p = 0;

    for (int i = 0; i < 8 && a && b; i++)
    {
        if (b & 0x1)
            p ^= a;
        b >>= 1;

        bool carry = a & 0x80;
        a <<= 1;
        a &= 0xfe;
        if (carry)
            a ^= 0x1b;
    }

    return p;
}

static
uint32_t
blrt_AES256_SubWord( uint32_t data )
{
    return (uint32_t) BLRT_AES256_S[(data & 0xf0000000) >> 28][(data & 0x0f000000) >> 24] << 24 |
           (uint32_t) BLRT_AES256_S[(data & 0x00f00000) >> 20][(data & 0x000f0000) >> 16] << 16 |
           (uint16_t) BLRT_AES256_S[(data & 0x0000f000) >> 12][(data & 0x00000f00) >> 8] << 8 |
           BLRT_AES256_S[(data & 0x000000f0) >> 4][(data & 0x0000000f)];
}


static
uint32_t
blrt_AES256_InverseSubWord( uint32_t data )
{
    return (uint32_t) BLRT_AES256_IS[(data & 0xf0000000) >> 28][(data & 0x0f000000) >> 24] << 24 |
           (uint32_t) BLRT_AES256_IS[(data & 0x00f00000) >> 20][(data & 0x000f0000) >> 16] << 16 |
           (uint16_t) BLRT_AES256_IS[(data & 0x0000f000) >> 12][(data & 0x00000f00) >> 8] << 8 |
           BLRT_AES256_IS[(data & 0x000000f0) >> 4][(data & 0x0000000f)];
}

static
void
blrt_AES256_ShiftRows( const blrt_AES256_Block *data, blrt_AES256_Block *output )
{
    blrt_AES256_Block result;

    result.matrix[0][0] = data->matrix[0][0];
    result.matrix[0][1] = data->matrix[1][1];
    result.matrix[0][2] = data->matrix[2][2];
    result.matrix[0][3] = data->matrix[3][3];

    result.matrix[1][0] = data->matrix[1][0];
    result.matrix[1][1] = data->matrix[2][1];
    result.matrix[1][2] = data->matrix[3][2];
    result.matrix[1][3] = data->matrix[0][3];

    result.matrix[2][0] = data->matrix[2][0];
    result.matrix[2][1] = data->matrix[3][1];
    result.matrix[2][2] = data->matrix[0][2];
    result.matrix[2][3] = data->matrix[1][3];

    result.matrix[3][0] = data->matrix[3][0];
    result.matrix[3][1] = data->matrix[0][1];
    result.matrix[3][2] = data->matrix[1][2];
    result.matrix[3][3] = data->matrix[2][3];

    *output = result;
}

static
void
blrt_AES256_InverseShiftRows( const blrt_AES256_Block *data, blrt_AES256_Block *output )
{
    blrt_AES256_Block result;

    result.matrix[0][0] = data->matrix[0][0];
    result.matrix[0][1] = data->matrix[3][1];
    result.matrix[0][2] = data->matrix[2][2];
    result.matrix[0][3] = data->matrix[1][3];

    result.matrix[1][0] = data->matrix[1][0];
    result.matrix[1][1] = data->matrix[0][1];
    result.matrix[1][2] = data->matrix[3][2];
    result.matrix[1][3] = data->matrix[2][3];

    result.matrix[2][0] = data->matrix[2][0];
    result.matrix[2][1] = data->matrix[1][1];
    result.matrix[2][2] = data->matrix[0][2];
    result.matrix[2][3] = data->matrix[3][3];

    result.matrix[3][0] = data->matrix[3][0];
    result.matrix[3][1] = data->matrix[2][1];
    result.matrix[3][2] = data->matrix[1][2];
    result.matrix[3][3] = data->matrix[0][3];

    *output = result;
}

static
void
blrt_AES256_MixColumns( const blrt_AES256_Block *data, blrt_AES256_Block *output )
{
    blrt_AES256_Block result;

    for (int i = 0; i < 4; i++)
    {
        result.matrix[i][0] = blrt_AES256_GFMul(data->matrix[i][0], BLRT_AES256_GF[0][0]) ^ blrt_AES256_GFMul(data->matrix[i][1], BLRT_AES256_GF[0][1]) ^
                              blrt_AES256_GFMul(data->matrix[i][2], BLRT_AES256_GF[0][2]) ^ blrt_AES256_GFMul(data->matrix[i][3], BLRT_AES256_GF[0][3]);
        result.matrix[i][1] = blrt_AES256_GFMul(data->matrix[i][0], BLRT_AES256_GF[1][0]) ^ blrt_AES256_GFMul(data->matrix[i][1], BLRT_AES256_GF[1][1]) ^
                              blrt_AES256_GFMul(data->matrix[i][2], BLRT_AES256_GF[1][2]) ^ blrt_AES256_GFMul(data->matrix[i][3], BLRT_AES256_GF[1][3]);
        result.matrix[i][2] = blrt_AES256_GFMul(data->matrix[i][0], BLRT_AES256_GF[2][0]) ^ blrt_AES256_GFMul(data->matrix[i][1], BLRT_AES256_GF[2][1]) ^
                              blrt_AES256_GFMul(data->matrix[i][2], BLRT_AES256_GF[2][2]) ^ blrt_AES256_GFMul(data->matrix[i][3], BLRT_AES256_GF[2][3]);
        result.matrix[i][3] = blrt_AES256_GFMul(data->matrix[i][0], BLRT_AES256_GF[3][0]) ^ blrt_AES256_GFMul(data->matrix[i][1], BLRT_AES256_GF[3][1]) ^
                              blrt_AES256_GFMul(data->matrix[i][2], BLRT_AES256_GF[3][2]) ^ blrt_AES256_GFMul(data->matrix[i][3], BLRT_AES256_GF[3][3]);
    }

    *output = result;
}

static
void
blrt_AES256_InverseMixColumns( const blrt_AES256_Block *data, blrt_AES256_Block *output )
{
    blrt_AES256_Block result;

    for (int i = 0; i < 4; i++)
    {
        result.matrix[i][0] = blrt_AES256_GFMul(data->matrix[i][0], BLRT_AES256_IGF[0][0]) ^ blrt_AES256_GFMul(data->matrix[i][1], BLRT_AES256_IGF[0][1]) ^
                              blrt_AES256_GFMul(data->matrix[i][2], BLRT_AES256_IGF[0][2]) ^ blrt_AES256_GFMul(data->matrix[i][3], BLRT_AES256_IGF[0][3]);
        result.matrix[i][1] = blrt_AES256_GFMul(data->matrix[i][0], BLRT_AES256_IGF[1][0]) ^ blrt_AES256_GFMul(data->matrix[i][1], BLRT_AES256_IGF[1][1]) ^
                              blrt_AES256_GFMul(data->matrix[i][2], BLRT_AES256_IGF[1][2]) ^ blrt_AES256_GFMul(data->matrix[i][3], BLRT_AES256_IGF[1][3]);
        result.matrix[i][2] = blrt_AES256_GFMul(data->matrix[i][0], BLRT_AES256_IGF[2][0]) ^ blrt_AES256_GFMul(data->matrix[i][1], BLRT_AES256_IGF[2][1]) ^
                              blrt_AES256_GFMul(data->matrix[i][2], BLRT_AES256_IGF[2][2]) ^ blrt_AES256_GFMul(data->matrix[i][3], BLRT_AES256_IGF[2][3]);
        result.matrix[i][3] = blrt_AES256_GFMul(data->matrix[i][0], BLRT_AES256_IGF[3][0]) ^ blrt_AES256_GFMul(data->matrix[i][1], BLRT_AES256_IGF[3][1]) ^
                              blrt_AES256_GFMul(data->matrix[i][2], BLRT_AES256_IGF[3][2]) ^ blrt_AES256_GFMul(data->matrix[i][3], BLRT_AES256_IGF[3][3]);
    }

    *output = result;
}

static
void
blrt_AES256_AddRoundKey( const blrt_AES256_Block *data, const blrt_AES256_Block *key, blrt_AES256_Block *output )
{
    for (int i = 0; i < 4; i++)
        output->word[i] = data->word[i] ^ key->word[i];
}

static
blrt_AES256_ExpandedKey
blrt_AES256_ExpandKey( const unsigned char key[32] )
{
    blrt_AES256_ExpandedKey result;
    memcpy(&result, key, 32 * sizeof(unsigned char));

    uint32_t temp = 0;
    for (int i = 8; i < 8 * 15; i++)
    {
        if (i % 8 == 0)
        {
            temp = result.word[i - 1];
            temp = blrt_RotateLeft32(temp, 24);
            temp = blrt_AES256_SubWord(temp);
            temp = temp ^ BLRT_AES256_RCON[i / 8];
            temp = result.word[i - 8] ^ temp;
            result.word[i] = temp;
        }
        else if (i % 4 == 0)
        {
            temp = result.word[i - 1];
            temp = blrt_AES256_SubWord(temp);
            temp ^= result.word[i - 8];
            result.word[i] = temp;
        }
        else
        {
            temp = result.word[i - 1];
            temp ^= result.word[i - 8];
            result.word[i] = temp;
        }
    }

    return result;
}

static
void
blrt_AES256_EncryptBlock( const unsigned char raw_data[16], const blrt_AES256_ExpandedKey *expanded_key, unsigned char encrypted_output[16] )
{
    const blrt_AES256_Block *data_ptr = (blrt_AES256_Block *) raw_data;
    blrt_AES256_Block *output_ptr = (blrt_AES256_Block *) encrypted_output;

    blrt_AES256_AddRoundKey(data_ptr, &(expanded_key->block[0]), output_ptr);

    for (int i = 1; i < 14; i++)
    {
        for (int j = 0; j < 4; j++)
            output_ptr->word[j] = blrt_AES256_SubWord(output_ptr->word[j]);
        blrt_AES256_ShiftRows(output_ptr, output_ptr);
        blrt_AES256_MixColumns(output_ptr, output_ptr);
        blrt_AES256_AddRoundKey(output_ptr, &(expanded_key->block[i]), output_ptr);
    }

    for (int i = 0; i < 4; i++)
        output_ptr->word[i] = blrt_AES256_SubWord(output_ptr->word[i]);
    blrt_AES256_ShiftRows(output_ptr, output_ptr);
    blrt_AES256_AddRoundKey(output_ptr, &(expanded_key->block[14]), output_ptr);
}

static
void
blrt_AES256_DecryptBlock( const unsigned char encrypted_data[16], const blrt_AES256_ExpandedKey *expanded_key, unsigned char decrypted_output[16] )
{
    const blrt_AES256_Block *data_ptr = (blrt_AES256_Block *) encrypted_data;
    blrt_AES256_Block *output_ptr = (blrt_AES256_Block *) decrypted_output;

    blrt_AES256_AddRoundKey(data_ptr, &(expanded_key->block[14]), output_ptr);

    for (int i = 13; i > 0; i--)
    {
        blrt_AES256_InverseShiftRows(output_ptr, output_ptr);
        for (int j = 0; j < 4; j++)
            output_ptr->word[j] = blrt_AES256_InverseSubWord(output_ptr->word[j]);
        blrt_AES256_AddRoundKey(output_ptr, &(expanded_key->block[i]), output_ptr);
        blrt_AES256_InverseMixColumns(output_ptr, output_ptr);
    }

    blrt_AES256_InverseShiftRows(output_ptr, output_ptr);
    for (int j = 0; j < 4; j++)
        output_ptr->word[j] = blrt_AES256_InverseSubWord(output_ptr->word[j]);
    blrt_AES256_AddRoundKey(output_ptr, &(expanded_key->block[0]), output_ptr);
}

inline
size_t
blrt_AES256_LengthAfterPadding( size_t length )
{
    return length + 16 - length % 16;
}

size_t
blrt_AES256_Encrypt( size_t length, const unsigned char *raw_data, const unsigned char key[32], unsigned char *encrypted_output )
{
    size_t nonfill_cycles = length / 16;
    size_t leftover_size = length % 16;
    size_t real_length = length;
    uint8_t padding_size = 16 - leftover_size;

    blrt_AES256_ExpandedKey expanded_key = blrt_AES256_ExpandKey(key);

    const unsigned char *data_ptr = raw_data;
    unsigned char *output_ptr = encrypted_output;
    for (size_t i = 0; i < nonfill_cycles; i++)
    {
        blrt_AES256_EncryptBlock(data_ptr, &expanded_key, output_ptr);
        data_ptr += 16;
        output_ptr += 16;
    }

    unsigned char window[16];
    if (leftover_size)
    {
        memcpy(window, data_ptr, leftover_size);
        for (int i = leftover_size; i < 16; i++)
            window[i] = padding_size;

        blrt_AES256_EncryptBlock(window, &expanded_key, output_ptr);

        real_length += 16 - leftover_size;
    }
    else
    {
        for (int i = 0; i < 16; i++)
            window[i] = padding_size;
        blrt_AES256_EncryptBlock(window, &expanded_key, output_ptr);

        real_length += 16;
    }

    return real_length;
}

size_t
blrt_AES256_Decrypt( size_t length, const unsigned char *encryted_data, const unsigned char key[32], unsigned char *decrypted_output )
{
    if (length < 16)
        return 0;

    size_t cycles = length / 16;

    blrt_AES256_ExpandedKey expanded_key = blrt_AES256_ExpandKey(key);

    for (size_t i = 0; i < cycles; i++)
        blrt_AES256_DecryptBlock(encryted_data + i * 16, &expanded_key, decrypted_output + i * 16);

    return 16 * (cycles - 1) + (16 - decrypted_output[length - 1]);
}
