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

#ifndef BLRT_LIB_TEXT_COMPOSE_H
#define BLRT_LIB_TEXT_COMPOSE_H

#include <pch.h>

#include "../container/string.h"

///
/// \brief Convert integer to string
///
/// Convert an integer into string.
///
/// \param output Output for the integer
/// \param i Integer to convert
/// \param base Base for the converted product
///
void
blrt_StringCompose_IToA( blrt_String *output, intmax_t i, unsigned int base );

///
/// \brief Convert unsigned integer to string
///
/// Convert an unsigned integer into string.
///
/// \param output Output for the integer
/// \param u Integer to convert
/// \param base Base for the converted product
///
void
blrt_StringCompose_UToA( blrt_String *output, uintmax_t u, unsigned int base );

///
/// \brief Convert floating point number to string
///
/// Convert a floating point number into string.
///
/// \param output Output for the floating
/// \param f Floating point number to convert
/// \param max_significant Maximum amount of characters after decimal point
///
void
blrt_StringCompose_FToA( blrt_String *output, float f, size_t max_significant );

///
/// \brief Convert floating point number to string
///
/// Convert a floating point number into string.
///
/// \param output Output for the floating
/// \param d Floating point number to convert
/// \param max_significant Maximum amount of characters after decimal point
///
void
blrt_StringCompose_DToA( blrt_String *output, double d, size_t max_significant );

///
/// \brief Convert floating point number to string
///
/// Convert a floating point number into string.
///
/// \param output Output for the floating
/// \param ld Floating point number to convert
/// \param max_significant Maximum amount of characters after decimal point
///
void
blrt_StringCompose_LDToA( blrt_String *output, long double ld, size_t max_significant );

// Wait for C2X to add the floating extension for LDToA.

///
/// \brief Compose string
///
/// Compose a string by given parameters.<br />
/// Make sure the parameters are of type \a blrt_GenericType.
///
/// \param output Output for the string
/// \param count Number of generic parameters
///
void
blrt_StringCompose_Compose( blrt_String *output, size_t count, ... );

///
/// \brief Compose string
///
/// Compose a string by given parameters.<br />
/// Make sure the parameters are of type \a blrt_GenericType.
///
/// \param output Output for the string
/// \param count Number of generic parameters
/// \param args Actual arguments
///
void
blrt_StringCompose_ComposeV( blrt_String *output, size_t count, va_list *args );

#endif // !BLRT_LIB_TEXT_COMPOSE_H
