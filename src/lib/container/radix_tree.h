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

#ifndef SOWR_LIB_CONTAINER_RADIX_TREE_H
#define SOWR_LIB_CONTAINER_RADIX_TREE_H

#include <pch.h>

#include "hash_map.h"
#include "string.h"

typedef void (*sowr_RadixTreeFreeFunc)( void * );

typedef struct sowr_RadixTreeNode
{
    size_t children;
    size_t data_size;
    void *data;
    sowr_String key;
    sowr_String full_key;
    struct sowr_RadixTreeNode *characters[CHAR_MAX];
} sowr_RadixTreeNode;

typedef struct sowr_RadixTree
{
    sowr_RadixTreeFreeFunc free_func;
    sowr_RadixTreeNode head;
} sowr_RadixTree;

///
/// \brief Create a radix tree
///
/// Create a radix tree ready to use.
///
/// \param free_func Function to call when freeing elements
///
/// \return Created radix tree
///
sowr_RadixTree *
sowr_RadixTree_Create( sowr_RadixTreeFreeFunc free_func );

///
/// \brief Create a radix tree
///
/// Create a radix tree by stack ready to use.<BR />
/// <B>The created radix tree must be destroyed with \a sowr_RadixTree_DestroyS().</B>
///
/// \param free_func Function to call when freeing elements
///
/// \return Created radix tree
///
sowr_RadixTree
sowr_RadixTree_CreateS( sowr_RadixTreeFreeFunc free_func );

///
/// \brief Clear the radix tree
///
/// Clear the contents of the radix tree
///
/// \param tree Radix tree to clear
///
void
sowr_RadixTree_Clear( sowr_RadixTree *tree );

///
/// \brief Insert an element to radix tree
///
/// Insert an element to the radix tree. It will override the old data if overlapped.
///
/// \param tree Radix tree to operate
/// \param index String index for the element
/// \param data_size Size of the data
/// \param data Data to insert
///
void
sowr_RadixTree_Insert( sowr_RadixTree *tree, const char *index, size_t data_size, const void *data );

///
/// \brief Insert an element to radix tree
///
/// Insert an element to the radix tree. It will override the old data if overlapped.
///
/// \param tree Radix tree to operate
/// \param index String index for the element
/// \param data_size Size of the data
/// \param data Data to insert
///
void
sowr_RadixTree_InsertS( sowr_RadixTree *tree, const sowr_String *index, size_t data_size, const void *data );

///
/// \brief Get an element from radix tree
///
/// Get an element from the radix tree.
///
/// \param tree Radix tree to operate
/// \param index String index for the element
///
/// \return The trie node containing data if found, NULL otherwise
///
sowr_RadixTreeNode *
sowr_RadixTree_Get( const sowr_RadixTree *tree, const char *index );

///
/// \brief Get an element from radix tree
///
/// Get an element from the radix tree.
///
/// \param tree Radix tree to operate
/// \param index String index for the element
///
/// \return The trie node containing data if found, NULL otherwise
///
sowr_RadixTreeNode *
sowr_RadixTree_GetS( const sowr_RadixTree *tree, const sowr_String *index );

///
/// \brief List all the children of a node
///
/// List all children of a radix tree node to a hash map, including the node itself.
///
/// \param node Node to operate
/// \param output Hash map for output. If this is NULL, only children counting will be returned
///
/// \return The number of children listed
///
size_t
sowr_RadixTree_ListAllChildren( const sowr_RadixTreeNode *node, sowr_HashMap *output );

///
/// \brief Delete an element from radix tree
///
/// Delete an element from the radix tree (the node is not deleted).
///
/// \param tree Radix tree to operate
/// \param index String index for the node
///
/// \return If anything is deleted
///
bool
sowr_RadixTree_Delete( sowr_RadixTree *tree, const char *index );

///
/// \brief Delete an element from radix tree
///
/// Delete an element from the radix tree (the node is not deleted).
///
/// \param tree Radix tree to operate
/// \param index String index for the node
///
/// \return If anything is deleted
///
bool
sowr_RadixTree_DeleteS( sowr_RadixTree *tree, const sowr_String *index );

///
/// \brief Destroy the radix tree
///
/// Destroy the radix tree.
///
/// \param tree Radix tree to destroy
///
void
sowr_RadixTree_Destroy( sowr_RadixTree *tree );

///
/// \brief Destroy the radix tree
///
/// Destroy the radix tree created by \a sowr_RadixTree_CreateS().
///
/// \param tree Radix tree to destroy
///
void
sowr_RadixTree_DestroyS( sowr_RadixTree *tree );

#endif // !SOWR_LIB_CONTAINER_RADIX_TREE_H
