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

#ifndef BLRT_LIB_DATA_UNICODE_H
#define BLRT_LIB_DATA_UNICODE_H

#include <pch.h>

#include "../container/vector.h"

typedef uint32_t blrt_Unicode;

typedef struct blrt_UTF8Sequence
{
    size_t length;
    const unsigned char *ptr;
} blrt_UTF8Sequence;

typedef struct blrt_UTF16Sequence
{
    size_t length;
    const unsigned char *ptr;
    bool terminator;
} blrt_UTF16Sequence;

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
blrt_Unicode_CountUTF8CodePoints( const unsigned char *str );

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
blrt_Unicode_CountUTF16CodePoints( const unsigned char *str );

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
blrt_UTF8Sequence
blrt_Unicode_NextUTF8Sequence( const unsigned char *str );

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
blrt_UTF16Sequence
blrt_Unicode_NextUTF16Sequence( const unsigned char *str );

///
/// \brief Decode a utf-8 sequence
///
/// Decode the utf-8 sequence into an unicode code point.
///
/// \param seq Utf-8 sequence to decode
///
/// \return Unicode code point of the sequence
///
blrt_Unicode
blrt_Unicode_DecodeUTF8Sequence( const blrt_UTF8Sequence *seq );

///
/// \brief Decode a utf-16 sequence
///
/// Decode the utf-16 sequence into an unicode code point.
///
/// \param seq Utf-16 sequence to decode
///
/// \return Unicode code point of the sequence
///
blrt_Unicode
blrt_Unicode_DecodeUTF16Sequence( const blrt_UTF16Sequence *seq );

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
blrt_Unicode_UTF8LengthOfCodePoint( blrt_Unicode cp );

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
blrt_Unicode_UTF16LengthOfCodePoint( blrt_Unicode cp );

///
/// \brief Encode an unicode code point
///
/// Encode an unicode code point to an utf-8 sequence.
///
/// \warning This function is unsafe. Make sure the output buffer has at least 4 bytes writable!
///
/// \param cp Code point
/// \param output Buffer for output
///
void
blrt_Unicode_EncodeCodePointUTF8( blrt_Unicode cp, unsigned char *output );

///
/// \brief Encode an unicode code point
///
/// Encode an unicode code point to an utf-16 sequence.
///
/// \warning This function is unsafe. Make sure the output buffer has at least 4 bytes writable!
///
/// \param cp Code point
/// \param output Buffer for output
///
void
blrt_Unicode_EncodeCodePointUTF16( blrt_Unicode cp, unsigned char *output );

///
/// \brief Decode an utf-8 string
///
/// Decode an utf-8 string into a vector of unicode code points.<BR />
/// <I>The string is assumed to be properly utf-8 encoded.</I>
///
/// \param str Utf-8 string
/// \param output Vector for output, it should be of blrt_Unicode type
///
void
blrt_Unicode_DecodeUTF8String( const unsigned char *str, blrt_Vector *output );

///
/// \brief Decode an utf-16 string
///
/// Decode an utf-16 string into a vector of unicode code points.<BR />
/// <I>The string is assumed to be properly utf-16 encoded.</I>
///
/// \param str Utf-16 string
/// \param output Vector for output, it should be of blrt_Unicode type
///
void
blrt_Unicode_DecodeUTF16String( const unsigned char *str, blrt_Vector *output );

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
blrt_Unicode_UTF8ToUTF16( const unsigned char *str, blrt_Vector *output );

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
blrt_Unicode_UTF16ToUTF8( const unsigned char *str, blrt_Vector *output );

#endif // !BLRT_LIB_DATA_UNICODE_H
