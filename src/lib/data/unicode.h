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

#ifndef SOWR_LIB_DATA_UNICODE_H
#define SOWR_LIB_DATA_UNICODE_H

#include <pch.h>

#include "../container/vector.h"

typedef struct sowr_UTF8Sequence
{
    size_t length;
    const char *ptr;
} sowr_UTF8Sequence;

typedef uint32_t sowr_Unicode;

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
sowr_UC_CountUTF8CodePoints( const char *str );

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
sowr_UC_NextUTF8Sequence( const char *str );

///
/// \brief Decode a utf-8 sequence
///
/// Decode the utf-8 sequence into an utf-8 code point.
///
/// \param seq Utf-8 sequence to decode
///
/// \return Unicode code point of the sequence
///
sowr_Unicode
sowr_UC_DecodeUTF8Sequence( const sowr_UTF8Sequence *seq );

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
sowr_UC_UTF8LengthOfCodePoint( sowr_Unicode cp );

///
/// \brief Encode an unicode code point
///
/// Encode an unicode code point to an utf-8 sequence.
///
/// \param cp Code point
/// \param output Buffer for output
///
void
sowr_UC_EncodeCodePointUTF8( sowr_Unicode cp, char *output );

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
sowr_UC_DecodeUTF8String( const char *str, sowr_Vector *output );

#endif // !SOWR_LIB_DATA_UNICODE_H
