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

#include "unicode.h"

#include "bytes.h"

size_t
sowr_Unicode_CountUTF8CodePoints( const unsigned char *str )
{
    size_t length = 0ULL;
    while (*str)
        length += (*str++ & 0xc0) != 0x80;
    return length; 
}

size_t
sowr_Unicode_CountUTF16CodePoints( const unsigned char *str )
{
    size_t length = 0ULL;
    sowr_UTF16Sequence seq = sowr_Unicode_NextUTF16Sequence(str);
    while (!seq.terminator)
    {
        length++;
        str += seq.length;
        seq = sowr_Unicode_NextUTF16Sequence(str);
    }
    return length;
}

sowr_UTF8Sequence
sowr_Unicode_NextUTF8Sequence( const unsigned char *str )
{
    size_t length = 0ULL;
    unsigned char ch = 0;
    if (str && (ch = *str))
    {
        // We check for the 0 mark after 1... in the byte.
        if (!(ch & ~0x7f))
            // If the byte becomes 0(0000000) after we turn off latter 7 bits, it's in ASCII range [0, 127].
            length = 1ULL;
        else
        {
            ch = (*str) & ~0x1f;
            // Checking the first 1... bit seems unnecessary, change later if bugs occur.
            if (/* ch & 0xc0 && */ ~(ch & ~0xdf) & 0x20)
                // Matches 110xxxxx byte head.
                length = 2ULL;
            else
            {
                ch = (*str) & ~0xf;
                if (/* ch & 0xe0 && */ ~(ch & ~0xef) & 0x10)
                    // Matches 1110xxxx byte head.
                    length = 3ULL;
                else
                    // We are not supporting utf-8 longer than 4 bytes.
                    length = 4ULL;
            }
        }
    }

    return (sowr_UTF8Sequence)
    {
        .length = length,
        .ptr = str
    };
}

sowr_UTF16Sequence
sowr_Unicode_NextUTF16Sequence( const unsigned char *str )
{
    uint16_t *first_two = (uint16_t *)str;

    if (*first_two <= 0xd7ff || (*first_two >= 0xe000 && *first_two <= 0xffff))
    {
        // Two bytes utf-16
        return (sowr_UTF16Sequence)
        {
            .length = 2ULL,
            .ptr = str,
            .terminator = (!*first_two)
        };
    }
    else
        // Four bytes utf-16
        return (sowr_UTF16Sequence)
        {
            .length = 4ULL,
            .ptr = str,
            .terminator = false
        };
}

sowr_Unicode
sowr_Unicode_DecodeUTF8Sequence( const sowr_UTF8Sequence *seq )
{
    if (!seq || !seq->ptr)
        return 0U;

    sowr_Unicode code = 0U;
    if (seq->length == 1ULL)
        code = *(seq->ptr);
    // Don't ask me why I don't use loops or pointers to optimize this,
    // I encountered some weird bug!!!!
    else if (seq->length == 2ULL)
    {
        unsigned char bytes[2] = { 0U };
        bytes[0] = *(seq->ptr) & ~0xc0;
        bytes[1] = *(seq->ptr + sizeof(uint8_t)) & ~0x80;
        code += bytes[0];
        code <<= 6;
        code += bytes[1];
    }
    else if (seq->length == 3ULL)
    {
        unsigned char bytes[3] = { 0U };
        bytes[0] = *(seq->ptr) & ~0xe0;
        bytes[1] = *(seq->ptr + sizeof(uint8_t)) & ~0x80;
        bytes[2] = *(seq->ptr + sizeof(uint8_t) * 2ULL) & ~0x80;
        code += bytes[0];
        code <<= 6;
        code += bytes[1];
        code <<= 6;
        code += bytes[2];
    }
    else
    {
        unsigned char bytes[4] = { 0U };
        bytes[0] = *(seq->ptr) & ~0xf0;
        bytes[1] = *(seq->ptr + sizeof(uint8_t)) & ~0x80;
        bytes[2] = *(seq->ptr + sizeof(uint8_t) * 2ULL) & ~0x80;
        bytes[3] = *(seq->ptr + sizeof(uint8_t) * 3ULL) & ~0x80;
        code += bytes[0];
        code <<= 6;
        code += bytes[1];
        code <<= 6;
        code += bytes[2];
        code <<= 6;
        code += bytes[3];
    }

    return code;
}

sowr_Unicode
sowr_Unicode_DecodeUTF16Sequence( const sowr_UTF16Sequence *seq )
{
    if (!seq || !seq->ptr)
        return 0U;

    sowr_Unicode code = 0U;
    if (seq->length == 2ULL)
    {
        uint16_t *bytes = (uint16_t *)(seq->ptr);
        code += *bytes;
    }
    else
    {
        uint16_t *first_two = (uint16_t *)(seq->ptr), *last_two = (uint16_t *)(seq->ptr + sizeof(uint16_t));
        code = ((((*first_two) & 0x03ff) << 10) | ((*last_two) & 0x03ff)) + 0x10000;
    }

    return code;
}

inline
size_t
sowr_Unicode_UTF8LengthOfCodePoint( sowr_Unicode cp )
{
    if (cp < 0x7f)
        return 1ULL;
    else if (cp < 0x7ff)
        return 2ULL;
    else if (cp <= 0xffff)
        return 3ULL;
    else
        return 4ULL;
}

inline
size_t
sowr_Unicode_UTF16LengthOfCodePoint( sowr_Unicode cp )
{
    if (cp <= 0xffff)
        return 2ULL;
    else
        return 4ULL;
}

void
sowr_Unicode_EncodeCodePointUTF8( sowr_Unicode cp, unsigned char *output )
{
    size_t length = sowr_Unicode_UTF8LengthOfCodePoint(cp);
    if (length == 1ULL)
    {
        unsigned char ch = (unsigned char)cp;
        memcpy(output, &ch, sizeof(uint8_t));
    }
    // Don't ask me why I don't use loops or pointers to optimize this,
    // I encountered some weird bug!!!!
    else if (length == 2ULL)
    {
        unsigned char bytes[2] = { 0U };
        bytes[0] = (cp >> 6) | 0xc0;
        bytes[1] = (cp & ~0xc0) | 0x80;
        memcpy(output, bytes, sizeof(uint16_t));
    }
    else if (length == 3ULL)
    {
        unsigned char bytes[3] = { 0U };
        bytes[0] = (cp >> 12) | 0xe0;
        bytes[1] = ((cp >> 6) & ~0xc0) | 0x80;
        bytes[2] = (cp & ~0xc0) | 0x80;
        memcpy(output, bytes, sizeof(uint16_t) + sizeof(uint8_t));
    }
    else
    {
        unsigned char bytes[4] = { 0U };
        bytes[0] = (cp >> 18) | 0xf0;
        bytes[1] = ((cp >> 12) & ~0xc0) | 0x80;
        bytes[2] = ((cp >> 6) & ~0xc0) | 0x80;
        bytes[3] = (cp & ~0xc0) | 0x80;
        memcpy(output, bytes, sizeof(uint32_t));
    }
}

void
sowr_Unicode_EncodeCodePointUTF16( sowr_Unicode cp, unsigned char *output )
{
    size_t length = sowr_Unicode_UTF16LengthOfCodePoint(cp);
    if (length == 2ULL)
    {
        uint16_t code = cp;
        memcpy(output, &code, sizeof(uint16_t));
    }
    else
    {
        cp -= 0x10000U;
        uint16_t first_two = 0U, last_two = 0U;
        first_two = (cp >> 10) | 0xd800;           // First 10 bits
        last_two = (cp & 0x3ff) | 0xdc00;          // Last 10 bits
        memcpy(output, &first_two, sizeof(uint16_t));
        memcpy(output + sizeof(uint16_t), &last_two, sizeof(uint16_t));
    }
}

void
sowr_Unicode_UTF16LE2BE( unsigned char *data )
{
    uint16_t bytes = 0U;
    while (true)
    {
        bytes = *((uint8_t *)(data + sizeof(uint8_t)));
        bytes <<= 8;
        bytes += *((uint8_t *)data);
        if (!bytes)
            break;
        sowr_SwapEndian(sizeof(uint16_t), data);
        data += sizeof(uint16_t);
    }
}

void
sowr_Unicode_UTF16BE2LE( unsigned char *data )
{
    uint16_t bytes = 0U;
    while (true)
    {
        bytes = *((uint16_t *)data);
        if (!bytes)
            break;
        sowr_SwapEndian(sizeof(uint16_t), data);
        data += sizeof(uint16_t);
    }
}

void
sowr_Unicode_DecodeUTF8String( const unsigned char *str, sowr_Vector *output )
{
    sowr_UTF8Sequence seq = sowr_Unicode_NextUTF8Sequence(str);
    while (seq.length)
    {
        sowr_Unicode cp = sowr_Unicode_DecodeUTF8Sequence(&seq);
        sowr_Vector_Push(output, &cp);
        str += seq.length;
        seq = sowr_Unicode_NextUTF8Sequence(str);
    }
}

void
sowr_Unicode_DecodeUTF16String( const unsigned char *str, sowr_Vector *output )
{
    sowr_UTF16Sequence seq = sowr_Unicode_NextUTF16Sequence(str);
    while (!seq.terminator)
    {
        sowr_Unicode cp = sowr_Unicode_DecodeUTF16Sequence(&seq);
        sowr_Vector_Push(output, &cp);
        str += seq.length;
        seq = sowr_Unicode_NextUTF16Sequence(str);
    }
}

void
sowr_Unicode_UTF8ToUTF16( const unsigned char *str, sowr_Vector *output )
{
    sowr_UTF8Sequence seq = sowr_Unicode_NextUTF8Sequence(str);
    unsigned char buffer[4] = { 0 };
    while (seq.length)
    {
        sowr_Unicode cp = sowr_Unicode_DecodeUTF8Sequence(&seq);
        size_t cp_len = sowr_Unicode_UTF16LengthOfCodePoint(cp);
        sowr_Unicode_EncodeCodePointUTF16(cp, buffer);
        for (size_t i = 0ULL; i < cp_len; i++)
            sowr_Vector_Push(output, buffer + i);
        str += seq.length;
        seq = sowr_Unicode_NextUTF8Sequence(str);
    }
    unsigned char c_0 = 0U;
    sowr_Vector_Push(output, &c_0);
    sowr_Vector_Push(output, &c_0);
}

void
sowr_Unicode_UTF16ToUTF8( const unsigned char *str, sowr_Vector *output )
{
    sowr_UTF16Sequence seq = sowr_Unicode_NextUTF16Sequence(str);
    unsigned char buffer[4] = { 0 };
    while (!seq.terminator)
    {
        sowr_Unicode cp = sowr_Unicode_DecodeUTF16Sequence(&seq);
        size_t cp_len = sowr_Unicode_UTF8LengthOfCodePoint(cp);
        sowr_Unicode_EncodeCodePointUTF8(cp, buffer);
        for (size_t i = 0ULL; i < cp_len; i++)
            sowr_Vector_Push(output, buffer + i);
        str += seq.length;
        seq = sowr_Unicode_NextUTF16Sequence(str);
    }
    unsigned char c_0 = 0U;
    sowr_Vector_Push(output, &c_0);
}
