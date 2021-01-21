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

#ifndef SOWR_LIB_DATA_UNICODE_H
#define SOWR_LIB_DATA_UNICODE_H

#include <pch.h>

#include "../container/vector.h"

typedef uint32_t sowr_Unicode;

typedef struct sowr_UTF8Sequence
{
    size_t length;
    const unsigned char *ptr;
} sowr_UTF8Sequence;

typedef struct sowr_UTF16Sequence
{
    size_t length;
    const unsigned char *ptr;
    bool terminator;
} sowr_UTF16Sequence;

///
/// \brief Count utf-8 code points
///
/// Count the total number of unicode code points in the utf-8 string.<BR />
/// <I>The string is assumed to be properly utf-8 encoded.</I>
///
/// \param str String to count
///
/// \return Number of unicode code points
///
size_t
sowr_Unicode_CountUTF8CodePoints( const unsigned char *str );

///
/// \brief Count utf-16 code points
///
/// Count the total number of unicode code points in the utf-16 string.<BR />
/// <I>The string is assumed to be properly utf-16 encoded.</I>
///
/// \param str String to count
///
/// \return Number of unicode code points
///
size_t
sowr_Unicode_CountUTF16CodePoints( const unsigned char *str );

///
/// \brief Get the next utf-8 sequence
///
/// Get the next utf-8 sequence in the string.<BR />
/// <I>The string is assumed to be properly utf-8 encoded.</I>
///
/// \param str String
///
/// \return Utf-8 sequence information
///
sowr_UTF8Sequence
sowr_Unicode_NextUTF8Sequence( const unsigned char *str );

///
/// \brief Get the next utf-16 sequence
///
/// Get the next utf-16 sequence in the string.<BR />
/// <I>The string is assumed to be properly utf-16 encoded.</I>
///
/// \param str String
///
/// \return Utf-16 sequence information
///
sowr_UTF16Sequence
sowr_Unicode_NextUTF16Sequence( const unsigned char *str );

///
/// \brief Decode a utf-8 sequence
///
/// Decode the utf-8 sequence into an unicode code point.
///
/// \param seq Utf-8 sequence to decode
///
/// \return Unicode code point of the sequence
///
sowr_Unicode
sowr_Unicode_DecodeUTF8Sequence( const sowr_UTF8Sequence *seq );

///
/// \brief Decode a utf-16 sequence
///
/// Decode the utf-16 sequence into an unicode code point.
///
/// \param seq Utf-16 sequence to decode
///
/// \return Unicode code point of the sequence
///
sowr_Unicode
sowr_Unicode_DecodeUTF16Sequence( const sowr_UTF16Sequence *seq );

///
/// \brief Length of unicode code point
///
/// Get the length of the utf-8 sequence of the unicode code point.
///
/// \param cp Code point
///
/// \return Length of utf-8 sequence
///
size_t
sowr_Unicode_UTF8LengthOfCodePoint( sowr_Unicode cp );

///
/// \brief Length of unicode code point
///
/// Get the length of the utf-16 sequence of the unicode code point.
///
/// \param cp Code point
///
/// \return Length of utf-16 sequence
///
size_t
sowr_Unicode_UTF16LengthOfCodePoint( sowr_Unicode cp );

///
/// \brief Encode an unicode code point
///
/// Encode an unicode code point to an utf-8 sequence.
///
/// \param cp Code point
/// \param output Buffer for output
///
void
sowr_Unicode_EncodeCodePointUTF8( sowr_Unicode cp, unsigned char *output );

///
/// \brief Encode an unicode code point
///
/// Encode an unicode code point to an utf-16 sequence.
///
/// \param cp Code point
/// \param output Buffer for output
///
void
sowr_Unicode_EncodeCodePointUTF16( sowr_Unicode cp, unsigned char *output );

///
/// \brief Swap endianness of utf-16
///
/// Swap little-endian utf-16 string to big-endian.
/// <I>The string is assumed to be properly utf-16 little-endian encoded.</I>
///
/// \param data Utf-16 string
///
void
sowr_Unicode_UTF16LE2BE( unsigned char *data );

///
/// \brief Swap endianness of utf-16
///
/// Swap big-endian utf-16 string to little-endian.
/// <I>The string is assumed to be properly utf-16 big-endian encoded.</I>
///
/// \param data Utf-16 string
///
void
sowr_Unicode_UTF16BE2LE( unsigned char *data );

///
/// \brief Decode an utf-8 string
///
/// Decode an utf-8 string into a vector of unicode code points.<BR />
/// <I>The string is assumed to be properly utf-8 encoded.</I>
///
/// \param str Utf-8 string
/// \param output Vector for output, it should be of sowr_Unicode type
///
void
sowr_Unicode_DecodeUTF8String( const unsigned char *str, sowr_Vector *output );

///
/// \brief Decode an utf-16 string
///
/// Decode an utf-16 string into a vector of unicode code points.<BR />
/// <I>The string is assumed to be properly utf-16 encoded.</I>
///
/// \param str Utf-16 string
/// \param output Vector for output, it should be of sowr_Unicode type
///
void
sowr_Unicode_DecodeUTF16String( const unsigned char *str, sowr_Vector *output );

///
/// \brief Convert utf-8 to utf-16
///
/// Convert an utf-8 string to an utf-16 string.<BR />
/// <I>The string is assumed to be properly utf-8 encoded.</I>
///
/// \param str UTF-8 string
/// \param output Vector for output, it should be of unsigned char type
///
void
sowr_Unicode_UTF8ToUTF16( const unsigned char *str, sowr_Vector *output );

///
/// \brief Convert utf-16 to utf-8
///
/// Convert an utf-16 string to an utf-8 string.<BR />
/// <I>The string is assumed to be properly utf-16 encoded.</I>
///
/// \param str UTF-16 string
/// \param output Vector for output, it should be of unsigned char type
///
void
sowr_Unicode_UTF16ToUTF8( const unsigned char *str, sowr_Vector *output );

#endif // !SOWR_LIB_DATA_UNICODE_H
