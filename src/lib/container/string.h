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

#ifndef BLRT_LIB_CONTAINER_STRING_H
#define BLRT_LIB_CONTAINER_STRING_H

#include <pch.h>

typedef struct blrt_String
{
    size_t capacity;
    size_t length;
    char *ptr;
} blrt_String;

///
/// \brief Create a string
///
/// Create a string ready to use.
///
/// \return Created string
///
blrt_String *
blrt_String_Create( void );

///
/// \brief Create a string
///
/// Create a string on stack, ready to use.<BR />
/// The created string must be destroyed by \a blrt_String_DestroyS().
///
/// \return Created string
///
blrt_String
blrt_String_CreateS( void );

///
/// \brief Duplicate a string
///
/// Create a string from an existing one.
///
/// \param original String to duplicate from
///
/// \return Duplicated string
///
blrt_String *
blrt_String_From( const char *original );

///
/// \brief Duplicate a string
///
/// Create a string from an existing one on stack.<BR />
/// <B>The created string must be destroyed by \a blrt_String_DestroyS().</B>
///
/// \param original String to duplicate from
///
/// \return Duplicated string
///
blrt_String
blrt_String_FromS( const char *original );

///
/// \brief Expand the string capacity
///
/// Expand the string, usually doubling its capacity unless the length is 0.
///
/// \param str String to expand
///
void
blrt_String_Expand( blrt_String *str );

///
/// \brief Expand the string capacity
///
/// Expand the string until its capacity reaches the set limit.
///
/// \param str String to expand
/// \param new_size Target size for expanding
///
void
blrt_String_ExpandUntil( blrt_String *str, size_t new_size );

///
/// \brief Expand the string
///
/// Expand the string until its capacity reaches the set limit.<BR />
/// This function only allocate once instead of possible multiple doubling
/// by \a blrt_String_ExpandUntil().
///
/// \param str String to expand
/// \param new_size Target size for expanding
///
void
blrt_String_ExpandUntilOnce( blrt_String *str, size_t new_size );

///
/// \brief Shrink the string
///
/// Shrink the string to just enough to fit its contents and the null terminator (if any).
///
/// \param str String to shrink
///
void
blrt_String_ShrinkToFit( blrt_String *str );

///
/// \brief Get the first character
///
/// Get the first character of the string.
///
/// \param str String
///
/// \return Pointer to the first character, NULL if string is empty
///
char *
blrt_String_First( const blrt_String *str );

///
/// \brief Get the last character
///
/// Get the last character of the string.<BR />
/// <B>This <I>"last character"</I> is not the null terminator!!!</B>
///
/// \param str String
///
/// \return Pointer to the last character, NULL if string is empty
///
char *
blrt_String_Last( const blrt_String *str );

///
/// \brief Push a character
///
/// Push a character into the string.
///
/// \param str String
/// \param data Pushing character
///
void
blrt_String_PushC( blrt_String *str, char data );

///
/// \brief Push a string
///
/// Push a string into the string.
///
/// \param str String
/// \param data Pushing string (there is ambiguity... lol)
///
void
blrt_String_PushS( blrt_String *str, const char *data );

///
/// \brief Push a character
///
/// Push a character into the front of string.
///
/// \param str String
/// \param data Pushing character
///
void
blrt_String_PushCFront( blrt_String *str, char data );

///
/// \brief Push a string
///
/// Push a string into the front of string.
///
/// \param str String
/// \param data Pushing string (there is ambiguity... lol)
///
void
blrt_String_PushFrontS( blrt_String *str, const char *data );

///
/// \brief Insert into string
///
/// Insert a character into the middle of string.
///
/// \param str String
/// \param index Index to push into (the index character later claims)
/// \param data Character to push
///
void
blrt_String_InsertC( blrt_String *str, size_t index, char data );

///
/// \brief Insert into string
///
/// Insert a string into the middle of string.
///
/// \param str String
/// \param index Index to push into (the index string segment later claims)
/// \param data String segment to push
///
void
blrt_String_InsertS( blrt_String *str, size_t index, const char *data );

///
/// \brief Pop from a string
///
/// Pop a character from the end of string.
///
/// \param str String
///
void
blrt_String_Pop( blrt_String *str );

///
/// \brief Pop from a string
///
/// Pop certain amount of characters from the end of string.
///
/// \param str String
/// \param num Amount of characters to pop
///
void
blrt_String_PopN( blrt_String *str, size_t num );

///
/// \brief Get a substring
///
/// Get a substring, the string before numth character is cut.
///
/// \param str String
/// \param num Index of substring
///
void
blrt_String_Sub( blrt_String *str, size_t num );

///
/// \brief Get a reserve string
///
/// Get a reserve string, the string after the numth character is cut.
///
/// \param str String
/// \param num Index of reserve
///
void
blrt_String_Res( blrt_String *str, size_t num );

///
/// \brief Clear a string
///
/// Clear a string's length.<BR />
///
/// \param str String to clear
///
void
blrt_String_Clear( blrt_String *str );

///
/// \brief Destroy a string
///
/// Destroy a string.
///
/// \param str String to destroy
///
void
blrt_String_Destroy( blrt_String *str );

///
/// \brief Destroy a string
///
/// Destroy a string created by \a blrt_String_CreateS().
///
/// \param str String to destroy
///
void
blrt_String_DestroyS( blrt_String *str );

#endif // !BLRT_LIB_CONTAINER_STRING_H
