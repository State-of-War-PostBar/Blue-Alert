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

#ifndef SOWR_LIB_TEXT_COMPOSE_H
#define SOWR_LIB_TEXT_COMPOSE_H

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
sowr_StringCompose_IToA( sowr_String *output, intmax_t i, unsigned int base );

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
sowr_StringCompose_UToA( sowr_String *output, uintmax_t u, unsigned int base );

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
sowr_StringCompose_FToA( sowr_String *output, float f, size_t max_significant );

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
sowr_StringCompose_DToA( sowr_String *output, double d, size_t max_significant );

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
sowr_StringCompose_LDToA( sowr_String *output, long double ld, size_t max_significant );

// Wait for C2X to add the floating extension for LDToA.

///
/// \brief Compose string
///
/// Compose a string by given parameters.<br />
/// Make sure the parameters are of type \a sowr_GenericType.
///
/// \param output Output for the string
/// \param count Number of generic parameters
///
void
sowr_StringCompose_Compose( sowr_String *output, size_t count, ... );

///
/// \brief Compose string
///
/// Compose a string by given parameters.<br />
/// Make sure the parameters are of type \a sowr_GenericType.
///
/// \param output Output for the string
/// \param count Number of generic parameters
/// \param args Actual arguments
///
void
sowr_StringCompose_ComposeV( sowr_String *output, size_t count, va_list *args );

#endif // !SOWR_LIB_TEXT_COMPOSE_H
