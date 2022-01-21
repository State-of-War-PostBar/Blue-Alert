/*************************************************************************************************
*                                                                                                *
*                                         [ Blue Alert ]                                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                              A free, open-source indie RTS game.                               *
*               (ɔ) 2017 - 2022 State of War Baidu Postbar, some rights reserved.                *
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

#ifndef BLRT_LIB_CONTAINER_TRIE_H
#define BLRT_LIB_CONTAINER_TRIE_H

#include <pch.h>

#include "string.h"

typedef void (*blrt_TrieFreeFunc)( void * );

typedef struct blrt_TrieNode
{
    size_t children;
    size_t data_size;
    void *data;
    struct blrt_TrieNode *characters[CHAR_MAX];
} blrt_TrieNode;

typedef struct blrt_Trie
{
    blrt_TrieFreeFunc free_func;
    blrt_TrieNode head;
} blrt_Trie;

///
/// \brief Create a trie
///
/// Create a trie ready to use.
///
/// \param free_func Function to call when the trie frees an element
///
/// \return Created trie
///
blrt_Trie *
blrt_Trie_Create( blrt_TrieFreeFunc free_func );

///
/// \brief Create a trie
///
/// Create a trie by stack ready to use.<BR />
/// <B>The created trie must be destroyed with \a blrt_Trie_DestroyS().</B>
///
/// \param free_func Function to call when the trie frees an element
///
/// \return Created trie
///
blrt_Trie
blrt_Trie_CreateS( blrt_TrieFreeFunc free_func );

///
/// \brief Clear the trie
///
/// Clear the contents of the trie.
///
/// \param trie Trie to clear
///
void
blrt_Trie_Clear( blrt_Trie *trie );

///
/// \brief Insert an element to trie
///
/// Insert an element to the trie. It will override the old data if overlapped.
///
/// \param trie Trie to operate
/// \param index String index for the element
/// \param data_size Size of the data
/// \param data Data to insert
///
void
blrt_Trie_Insert( blrt_Trie *trie, const char *index, size_t data_size, const void *data );

///
/// \brief Insert an element to trie
///
/// Insert an element to the trie. It will override the old data if overlapped.
///
/// \param trie Trie to operate
/// \param index String index for the element
/// \param data_size Size of the data
/// \param data Data to insert
///
void
blrt_Trie_InsertS( blrt_Trie *trie, const blrt_String *index, size_t data_size, const void *data );

///
/// \brief Get an element from trie
///
/// Get an element from the trie.
///
/// \param trie Trie to operate
/// \param index String index for the element
///
/// \return The trie node containing data if found, NULL otherwise
///
blrt_TrieNode *
blrt_Trie_Get( blrt_Trie *trie, const char *index );

///
/// \brief Get an element from trie
///
/// Get an element from the trie.
///
/// \param trie Trie to operate
/// \param index String index for the element
///
/// \return The trie node containing data if found, NULL otherwise
///
blrt_TrieNode *
blrt_Trie_GetS( blrt_Trie *trie, const blrt_String *index );

///
/// \brief Delete an element from trie
///
/// Delete an element from the trie (the node is not deleted).
///
/// \param trie Trie to operate
/// \param index String index for the node
///
/// \return If anything is deleted
///
bool
blrt_Trie_Delete( blrt_Trie *trie, const char *index );

///
/// \brief Delete an element from trie
///
/// Delete an element from the trie (the node is not deleted).
///
/// \param trie Trie to operate
/// \param index String index for the node
///
/// \return If anything is deleted
///
bool
blrt_Trie_DeleteS( blrt_Trie *trie, const blrt_String *index );

///
/// \brief Destroy the trie
///
/// Destroy the trie.
///
/// \param trie Trie to destroy
///
void
blrt_Trie_Destroy( blrt_Trie *trie );

///
/// \brief Destroy the trie
///
/// Destroy the trie created by \a blrt_Trie_CreateS().
///
/// \param trie Trie to destroy
///
void
blrt_Trie_DestroyS( blrt_Trie *trie );

#endif //!BLRT_LIB_CONTAINER_TRIE_H
