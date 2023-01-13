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

#ifndef BLRT_LIB_CONTAINER_RADIX_TREE_H
#define BLRT_LIB_CONTAINER_RADIX_TREE_H

#include <pch.h>

#include "hash_map.h"
#include "string.h"

typedef void (*blrt_RadixTreeFreeFunc)( void * );
typedef void (*blrt_RadixTreeWalkFunc)( void * );

typedef struct blrt_RadixTreeNode
{
    size_t children;
    size_t data_size;
    void *data;
    blrt_String key;
    blrt_String full_key;
    struct blrt_RadixTreeNode *characters[CHAR_MAX];
} blrt_RadixTreeNode;

typedef struct blrt_RadixTree
{
    blrt_RadixTreeFreeFunc free_func;
    blrt_RadixTreeNode head;
} blrt_RadixTree;

///
/// \brief Create a radix tree
///
/// Create a radix tree ready to use.
///
/// \param free_func Function to call when freeing elements
///
/// \return Created radix tree
///
blrt_RadixTree *
blrt_RadixTree_Create( blrt_RadixTreeFreeFunc free_func );

///
/// \brief Create a radix tree
///
/// Create a radix tree by stack ready to use.<BR />
/// <B>The created radix tree must be destroyed with \a blrt_RadixTree_DestroyS().</B>
///
/// \param free_func Function to call when freeing elements
///
/// \return Created radix tree
///
blrt_RadixTree
blrt_RadixTree_CreateS( blrt_RadixTreeFreeFunc free_func );

///
/// \brief Clear the radix tree
///
/// Clear the contents of the radix tree
///
/// \param tree Radix tree to clear
///
void
blrt_RadixTree_Clear( blrt_RadixTree *tree );

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
blrt_RadixTree_Insert( blrt_RadixTree *tree, const char *index, size_t data_size, const void *data );

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
blrt_RadixTree_InsertS( blrt_RadixTree *tree, const blrt_String *index, size_t data_size, const void *data );

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
blrt_RadixTreeNode *
blrt_RadixTree_Get( const blrt_RadixTree *tree, const char *index );

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
blrt_RadixTreeNode *
blrt_RadixTree_GetS( const blrt_RadixTree *tree, const blrt_String *index );

///
/// Walk the radix tree
///
/// Walk the radix tree.
///
/// \param node Node to start walking
/// \param func Function for walking
///
void
blrt_RadixTree_Walk( const blrt_RadixTreeNode *node, blrt_RadixTreeWalkFunc func );

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
blrt_RadixTree_ListAllChildren( const blrt_RadixTreeNode *node, blrt_HashMap *output );

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
blrt_RadixTree_Delete( blrt_RadixTree *tree, const char *index );

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
blrt_RadixTree_DeleteS( blrt_RadixTree *tree, const blrt_String *index );

///
/// \brief Destroy the radix tree
///
/// Destroy the radix tree.
///
/// \param tree Radix tree to destroy
///
void
blrt_RadixTree_Destroy( blrt_RadixTree *tree );

///
/// \brief Destroy the radix tree
///
/// Destroy the radix tree created by \a blrt_RadixTree_CreateS().
///
/// \param tree Radix tree to destroy
///
void
blrt_RadixTree_DestroyS( blrt_RadixTree *tree );

#endif // !BLRT_LIB_CONTAINER_RADIX_TREE_H
