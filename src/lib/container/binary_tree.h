/*************************************************************************************************
*                                                                                                *
*                                    State of War: Remastered                                    *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
* A free, open-source software project recreating an old game.                                   *
* (c) 2017 - 2020 State of War Baidu Postbar, some rights reserved.                              *
*                                                                                                *
* State of War: Remastered is a free software. You can freely do whatever you want with it       *
* under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)         *
* published by mhtvsSFrpHdE.                                                                     *
* By the time this line is written, the version of the license document is 1, but you may use    *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
* State of War: Remastered is created, intended to be useful, but without any warranty.          *
* For more information, please forward to the license.                                           *
*                                                                                                *
* You should have received a copy of the license along with the source code of this              *
* program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.    *
*                                                                                                *
* Mission is successfully completed.                                                             *
*                                                                                                *
**************************************************************************************************/

#ifndef SOWR_LIB_CONTAINER_BINARY_TREE_H
#define SOWR_LIB_CONTAINER_BINARY_TREE_H

#include <pch.h>

#include "vector.h"

typedef void (*sowr_BinaryTreeFreeFunc)( void * );
typedef void (*sowr_BinaryTreeWalkFunc)( void * );
typedef int  (*sowr_BinaryTreeCmpFunc)( const void *, const void * );

typedef struct sowr_BinaryTreeNode
{
    void *data;
    struct sowr_BinaryTreeNode *left, *right;
} sowr_BinaryTreeNode;

typedef struct sowr_BinaryTree
{
    size_t elem_size;
    size_t length;
    sowr_BinaryTreeFreeFunc free_func;
    sowr_BinaryTreeCmpFunc cmp_func;
    sowr_BinaryTreeNode *head;
} sowr_BinaryTree;

///
/// \brief Create a binary tree
///
/// Create a binary tree ready to use.
///
/// \param elem_size Size of each element
/// \param free_func Function to call when freeing an element
///
/// \return Created tree
///
sowr_BinaryTree *
sowr_BinaryTree_Create( size_t elem_size, sowr_BinaryTreeFreeFunc free_func, sowr_BinaryTreeCmpFunc cmp_func );

///
/// \brief Create a binary tree
///
/// Create a binary tree on stack ready to use.
///
/// \param elem_size Size of each element
/// \param free_func Function to call when freeing an element
///
/// \return Created tree
///
sowr_BinaryTree
sowr_BinaryTree_CreateS( size_t elem_size, sowr_BinaryTreeFreeFunc free_func, sowr_BinaryTreeCmpFunc cmp_func );

///
/// \brief Insert element to binary tree
///
/// Insert an element to the binary tree.
///
/// \param tree Tree to insert
/// \param elem Element to insert
/// \param cmp Comparision function
///
void
sowr_BinaryTree_Insert( sowr_BinaryTree *tree, const void *elem );

///
/// \brief Delete element in tree
///
/// Delete an element in the binary tree.
///
/// \param tree Tree to delete from
/// \param elem Element to delete
/// \param cmp Comparision function
///
/// \return If anything is deleted
///
bool
sowr_BinaryTree_Delete( sowr_BinaryTree *tree, const void *elem );

///
/// \brief Walk the binary tree
///
/// Walk the binary tree, from lowest to highest.
///
/// \param tree Tree to walk
/// \param func Function for walking
///
void
sowr_BinaryTree_Walk( sowr_BinaryTree *tree, sowr_BinaryTreeWalkFunc func );

///
/// \brief Find an element in tree
///
/// Look up an element in the binary tree.
///
/// \param tree Tree to look for
/// \param elem Element to look for
/// \param cmp Comparision function
///
/// \return The node if found
///
sowr_BinaryTreeNode *
sowr_BinaryTree_Find( const sowr_BinaryTree *tree, const void *elem );

///
/// \brief Clear a tree
///
/// Clearing out an binary tree, freeing all its elements
///
/// \param tree Tree to clear
///
void
sowr_BinaryTree_Clear( sowr_BinaryTree *tree );

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
sowr_BinaryTree_Height( const sowr_BinaryTree *tree );

///
/// \brief Get the width of tree
///
/// Get the width of the binary tree.
///
/// \param tree Tree to evaluate
///
/// \return Accumulated width
///
size_t
sowr_BinaryTree_Width( const sowr_BinaryTree *tree );

///
/// \brief Destroy a tree
///
/// Destroy a binary tree.
///
/// \param tree tree to destroy
///
void
sowr_BinaryTree_Destroy( sowr_BinaryTree *tree );

/// \brief Destroy a tree
///
/// Destroy a binary tree created by \a sowr_BinaryTree_CreateS().
///
/// \param tree Tree to destroy
///
void
sowr_BinaryTree_DestroyS( sowr_BinaryTree *tree );

#endif //!SOWR_LIB_CONTAINER_BINARY_TREE_H
