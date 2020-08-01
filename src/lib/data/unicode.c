/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (c) 2017 - 2020 State of War Baidu Postbar, some rights reserved.                *
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

inline
size_t
sowr_UC_CountUTF8CodePoints( const char *str )
{
    size_t length = 0ULL;
    while (*str)
        length += (*str++ & 0xc0) != 0x80;
    return length; 
}

sowr_UTF8Sequence
sowr_UC_NextUTF8Sequence( const char *str )
{
    size_t length = 0ULL;;
    char ch = 0;
    if (str && (ch = *str))
    {
        // We check for the 0 mark after 1... in the byte.
        if (!(ch & ~0x7f))
            // If the byte becomes 0(0000000) after we turn off latter 7 bits, it's in ASCII range [0, 127].
            length = 1ULL;
        else
        {
            ch = (*str) & ~0x1f;
            // TODO Checking the first 1... bits seems unnecessary, change later if bugs occur.
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

sowr_Unicode
sowr_UC_DecodeUTF8Sequence( const sowr_UTF8Sequence *seq )
{
    if (!seq || !seq->ptr)
        return 0U;

    sowr_Unicode code = 0U;
    if (seq->length == 1ULL)
        code = *(seq->ptr);
    // Don't ask me why I don't use loops to optimize this,
    // I encountered some weird bug!!!!
    else if (seq->length == 2ULL)
    {
        char bytes[2] = { 0 };
        bytes[0] = *(seq->ptr) & ~0xc0;
        bytes[1] = *(seq->ptr + 1) & ~0x80;
        code += bytes[0];
        code <<= 6;
        code += bytes[1];
    }
    else if (seq->length == 3ULL)
    {
        char bytes[3] = { 0 };
        bytes[0] = *(seq->ptr) & ~0xe0;
        bytes[1] = *(seq->ptr + 1) & ~0x80;
        bytes[2] = *(seq->ptr + 2) & ~0x80;
        code += bytes[0];
        code <<= 6;
        code += bytes[1];
        code <<= 6;
        code += bytes[2];
    }
    else
    {
        char bytes[4] = { 0 };
        bytes[0] = *(seq->ptr) & ~0xf0;
        bytes[1] = *(seq->ptr + 1) & ~0x80;
        bytes[2] = *(seq->ptr + 2) & ~0x80;
        bytes[3] = *(seq->ptr + 3) & ~0x80;
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

inline
size_t
sowr_UC_UTF8LengthOfCodePoint( sowr_Unicode cp )
{
    if (cp < 0x7fU)
        return 1ULL;
    else if (cp < 0x7ffU)
        return 2ULL;
    else if (cp <= 0xffffU)
        return 3ULL;
    else
        return 4ULL;
}

void
sowr_UC_EncodeCodePointUTF8( sowr_Unicode cp, char *output )
{
    size_t length = sowr_UC_UTF8LengthOfCodePoint(cp);
    if (!length)
        return;
    else if (length == 1ULL)
    {
        char ch = (char)cp;
        memcpy(output, &ch, sizeof(char));
    }
    // Don't ask me why I don't use loops to optimize this,
    // I encountered some weird bug!!!!
    else if (length == 2ULL)
    {
        char bytes[2] = { 0 };
        bytes[0] = (cp >> 6) | 0xc0;
        bytes[1] = (cp & ~0xc0) | 0x80;
        memcpy(output, bytes, sizeof(char) * 2ULL);
    }
    else if (length == 3ULL)
    {
        char bytes[3] = { 0 };
        bytes[0] = (cp >> 12) | 0xe0;
        bytes[1] = ((cp >> 6) & ~0xc0) | 0x80;
        bytes[2] = (cp & ~0xc0) | 0x80;
        memcpy(output, bytes, sizeof(char) * 3ULL);
    }
    else
    {
        char bytes[4] = { 0 };
        bytes[0] = (cp >> 18) | 0xf0;
        bytes[1] = ((cp >> 12) & ~0xc0) | 0x80;
        bytes[2] = ((cp >> 6) & ~0xc0) | 0x80;
        bytes[3] = (cp & ~0xc0) | 0x80;
        memcpy(output, bytes, sizeof(char) * 4ULL);
    }
}

void
sowr_UC_DecodeUTF8String( const char *str, sowr_Vector *output )
{
    sowr_UTF8Sequence seq = sowr_UC_NextUTF8Sequence(str);
    while (seq.length)
    {
        sowr_Unicode cp = sowr_UC_DecodeUTF8Sequence(&seq);
        sowr_Vector_Push(output, &cp);
        str += seq.length;
        seq = sowr_UC_NextUTF8Sequence(str);
    }
    sowr_Unicode c_0 = '\0';
    sowr_Vector_Push(output, &c_0);
}
