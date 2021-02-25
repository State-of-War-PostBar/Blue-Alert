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

#ifndef SOWR_LIB_CONTAINER_STRING_H
#define SOWR_LIB_CONTAINER_STRING_H

#include <pch.h>

typedef struct sowr_String
{
    size_t capacity;
    size_t length;
    char *ptr;
} sowr_String;

///
/// \brief Create a string
///
/// Create a string ready to use.
///
/// \return Created string
///
sowr_String *
sowr_String_Create( void );

///
/// \brief Create a string
///
/// Create a string on stack, ready to use.<BR />
/// The created string must be destroyed by \a sowr_String_DestroyS().
///
/// \return Created string
///
sowr_String
sowr_String_CreateS( void );

///
/// \brief Duplicate a string
///
/// Create a string from an existing one.
///
/// \param original String to duplicate from
///
/// \return Duplicated string
///
sowr_String *
sowr_String_From( const char *original );

///
/// \brief Duplicate a string
///
/// Create a string from an existing one on stack.<BR />
/// <B>The created string must be destroyed by \a sowr_String_DestroyS().</B>
///
/// \param original String to duplicate from
///
/// \return Duplicated string
///
sowr_String
sowr_String_FromS( const char *original );

///
/// \brief Expand the string capacity
///
/// Expand the string, usually doubling its capacity unless the length is 0.
///
/// \param str String to expand
///
void
sowr_String_Expand( sowr_String *str );

///
/// \brief Expand the string capacity
///
/// Expand the string until its capacity reaches the set limit.
///
/// \param str String to expand
/// \param new_size Target size for expanding
///
void
sowr_String_ExpandUntil( sowr_String *str, size_t new_size );

///
/// \brief Expand the string
///
/// Expand the string until its capacity reaches the set limit.<BR />
/// This function only allocate once instead of possible multiple doubling
/// by \a sowr_String_ExpandUntil().
///
/// \param str String to expand
/// \param new_size Target size for expanding
///
void
sowr_String_ExpandUntilOnce( sowr_String *str, size_t new_size );

///
/// \brief Shrink the string
///
/// Shrink the string to just enough to fit its contents and the null terminator (if any).
///
/// \param str String to shrink
///
void
sowr_String_ShrinkToFit( sowr_String *str );

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
sowr_String_First( const sowr_String *str );

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
sowr_String_Last( const sowr_String *str );

///
/// \brief Push a character
///
/// Push a character into the string.
///
/// \param str String
/// \param data Pushing character
///
void
sowr_String_PushC( sowr_String *str, char data );

///
/// \brief Push a string
///
/// Push a string into the string.
///
/// \param str String
/// \param data Pushing string (there is ambiguity... lol)
///
void
sowr_String_PushS( sowr_String *str, const char *data );

///
/// \brief Push a character
///
/// Push a character into the front of string.
///
/// \param str String
/// \param data Pushing character
///
void
sowr_String_PushCFront( sowr_String *str, char data );

///
/// \brief Push a string
///
/// Push a string into the front of string.
///
/// \param str String
/// \param data Pushing string (there is ambiguity... lol)
///
void
sowr_String_PushFrontS( sowr_String *str, const char *data );

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
sowr_String_InsertC( sowr_String *str, size_t index, char data );

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
sowr_String_InsertS( sowr_String *str, size_t index, const char *data );

///
/// \brief Pop from a string
///
/// Pop a character from the end of string.
///
/// \param str String
///
void
sowr_String_Pop( sowr_String *str );

///
/// \brief Pop from a string
///
/// Pop certain amount of characters from the end of string.
///
/// \param str String
/// \param num Amount of characters to pop
///
void
sowr_String_PopN( sowr_String *str, size_t num );

///
/// \brief Get a substring
///
/// Get a substring, the string before numth character is cut.
///
/// \param str String
/// \param num Index of substring
///
void
sowr_String_Sub( sowr_String *str, size_t num );

///
/// \brief Get a reserve string
///
/// Get a reserve string, the string after the numth character is cut.
///
/// \param str String
/// \param num Index of reserve
///
void
sowr_String_Res( sowr_String *str, size_t num );

///
/// \brief Clear a string
///
/// Clear a string's length.<BR />
///
/// \param str String to clear
///
void
sowr_String_Clear( sowr_String *str );

///
/// \brief Destroy a string
///
/// Destroy a string.
///
/// \param str String to destroy
///
void
sowr_String_Destroy( sowr_String *str );

///
/// \brief Destroy a string
///
/// Destroy a string created by \a sowr_String_CreateS().
///
/// \param str String to destroy
///
void
sowr_String_DestroyS( sowr_String *str );

#endif // !SOWR_LIB_CONTAINER_STRING_H
