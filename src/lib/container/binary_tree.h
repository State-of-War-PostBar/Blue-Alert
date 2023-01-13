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

#ifndef BLRT_LIB_CONTAINER_BINARY_TREE_H
#define BLRT_LIB_CONTAINER_BINARY_TREE_H

#include <pch.h>

typedef void (*blrt_BinaryTreeFreeFunc)( void * );
typedef void (*blrt_BinaryTreeWalkFunc)( void * );
typedef int  (*blrt_BinaryTreeCmpFunc)( const void *, const void * );

typedef struct blrt_BinaryTreeNode
{
    size_t data_size;
    void *data;
    struct blrt_BinaryTreeNode *left, *right;
} blrt_BinaryTreeNode;

typedef struct blrt_BinaryTree
{
    size_t length;
    blrt_BinaryTreeFreeFunc free_func;
    blrt_BinaryTreeNode head;
} blrt_BinaryTree;

///
/// \brief Create a binary tree
///
/// Create a binary tree ready to use.
///
/// \param free_func Function to call when freeing an element
///
/// \return Created tree
///
blrt_BinaryTree *
blrt_BinaryTree_Create( blrt_BinaryTreeFreeFunc free_func );

///
/// \brief Create a binary tree
///
/// Create a binary tree on stack ready to use.
/// <B>The created tree must be destroyed with \a blrt_BinaryTree_DestroyS().</B>
///
/// \param free_func Function to call when freeing an element
///
/// \return Created tree
///
blrt_BinaryTree
blrt_BinaryTree_CreateS( blrt_BinaryTreeFreeFunc free_func );

///
/// \brief Insert data to binary tree
///
/// Insert some data to the binary tree.
///
/// \param tree Tree to insert
/// \param data_size Size of data
/// \param data Data to insert
/// \param cmp_func Function for comparision
///
void
blrt_BinaryTree_Insert( blrt_BinaryTree *tree, size_t data_size, const void *data, blrt_BinaryTreeCmpFunc cmp_func );

///
/// \brief Delete element in tree
///
/// Delete an element in the binary tree.
///
/// \param tree Tree to delete from
/// \param data Data to delete
/// \param cmp_func Function for comparision
///
/// \return If anything is deleted
///
bool
blrt_BinaryTree_Delete( blrt_BinaryTree *tree, const void *data, blrt_BinaryTreeCmpFunc cmp_func );

///
/// \brief Walk the binary tree
///
/// Walk the binary tree, from lowest to highest.
///
/// \param tree Tree to walk
/// \param func Function for walking
///
void
blrt_BinaryTree_Walk( blrt_BinaryTree *tree, blrt_BinaryTreeWalkFunc func );

///
/// \brief Find an element in tree
///
/// Look up an element in the binary tree.
///
/// \param tree Tree to look for
/// \param data Data to look for
/// \param cmp_func Function for comparision
///
/// \return The node if found
///
blrt_BinaryTreeNode *
blrt_BinaryTree_Find( const blrt_BinaryTree *tree, const void *data, blrt_BinaryTreeCmpFunc cmp_func );

///
/// \brief Clear a tree
///
/// Clearing out a binary tree, freeing all its elements.
///
/// \param tree Tree to clear
///
void
blrt_BinaryTree_Clear( blrt_BinaryTree *tree );

///
/// \brief Get the height of tree
///
/// Get the height of the binary tree.
///
/// \param tree Tree to evaluate
///
/// \return Accumulated height
///
size_t
blrt_BinaryTree_Height( const blrt_BinaryTree *tree );

///
/// \brief Destroy a tree
///
/// Destroy a binary tree.
///
/// \param tree tree to destroy
///
void
blrt_BinaryTree_Destroy( blrt_BinaryTree *tree );

/// \brief Destroy a tree
///
/// Destroy a binary tree created by \a blrt_BinaryTree_CreateS().
///
/// \param tree Tree to destroy
///
void
blrt_BinaryTree_DestroyS( blrt_BinaryTree *tree );

#endif //!BLRT_LIB_CONTAINER_BINARY_TREE_H
