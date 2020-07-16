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

#ifndef SOWR_LIB_CONTAINER_TRIE_H
#define SOWR_LIB_CONTAINER_TRIE_H

#include <pch.h>

#include "string.h"

typedef void (*sowr_TrieFreeFunc)( void * );

typedef struct sowr_TrieNode
{
    size_t children;
    size_t data_size;
    void *data;
    struct sowr_TrieNode *characters[CHAR_MAX];
} sowr_TrieNode;

typedef struct sowr_Trie
{
    sowr_TrieFreeFunc free_func;
    sowr_TrieNode head;
} sowr_Trie;

///
/// \brief Create a trie
///
/// Create a trie ready to use.
///
/// \param free_func Function to call when the trie frees an element
///
/// \return Created trie
///
sowr_Trie *
sowr_Trie_Create( sowr_TrieFreeFunc free_func );

///
/// \brief Create a trie
///
/// Create a trie by stack ready to use.<BR />
/// <B>The created trie must be destroyed with \a sowr_Trie_DestroyS().</B>
///
/// \param free_func Function to call when the trie frees an element
///
/// \return Created trie
///
sowr_Trie
sowr_Trie_CreateS( sowr_TrieFreeFunc free_func );

///
/// \brief Clear the trie
///
/// Clear the contents of the trie.
///
/// \param trie Trie to clear
///
void
sowr_Trie_Clear( sowr_Trie *trie );

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
sowr_Trie_Insert( sowr_Trie *trie, const char *index, size_t data_size, const void *data );

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
sowr_Trie_InsertS( sowr_Trie *trie, const sowr_String *index, size_t data_size, const void *data );

///
/// \brief Get an element from trie
///
/// Get an element from the trie.
///
/// \param trie Trie to operate
/// \param index String index for the element
///
/// \return The trie node containing data if found, NULL otherwise.
///
sowr_TrieNode *
sowr_Trie_Get( sowr_Trie *trie, const char *index );

///
/// \brief Get an element from trie
///
/// Get an element from the trie.
///
/// \param trie Trie to operate
/// \param index String index for the element
///
/// \return The trie node containing data if found, NULL otherwise.
///
sowr_TrieNode *
sowr_Trie_GetS( sowr_Trie *trie, const sowr_String *index );

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
sowr_Trie_Delete( sowr_Trie *trie, const char *index );

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
sowr_Trie_DeleteS( sowr_Trie *trie, const sowr_String *index );

///
/// \brief Destroy the trie
///
/// Destroy the trie.
///
/// \param trie Trie to destroy
///
void
sowr_Trie_Destroy( sowr_Trie *trie );

///
/// \brief Destroy the trie
///
/// Destroy the trie created by \a sowr_Trie_CreateS().
///
/// \param trie Trie to destroy
///
void
sowr_Trie_DestroyS( sowr_Trie *trie );

#endif //!SOWR_LIB_CONTAINER_TRIE_H
