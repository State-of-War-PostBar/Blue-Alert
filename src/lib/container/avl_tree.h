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

#ifndef SOWR_LIB_CONTAINER_AVL_TREE_H
#define SOWR_LIB_CONTAINER_AVL_TREE_H

#include <pch.h>

typedef void (*sowr_AvlTreeFreeFunc)( void * );
typedef void (*sowr_AvlTreeWalkFunc)( void * );
typedef int  (*sowr_AvlTreeCmpFunc)( const void *, const void * );

typedef struct sowr_AvlTreeNode
{
    size_t data_size;
    void *data;
    struct sowr_AvlTreeNode *left, *right;
    int height;
} sowr_AvlTreeNode;

typedef struct sowr_AvlTree
{
    size_t length;
    sowr_AvlTreeFreeFunc free_func;
    sowr_AvlTreeCmpFunc cmp_func;
    sowr_AvlTreeNode *head;
} sowr_AvlTree;

///
/// \brief Create an avl tree
///
/// Create an avl tree ready to use.
///
/// \param free_func Function to call when freeing an element
/// \param cmp_func Function for comparision
///
/// \return Created tree
///
sowr_AvlTree *
sowr_AvlTree_Create( sowr_AvlTreeFreeFunc free_func, sowr_AvlTreeCmpFunc cmp_func );

///
/// \brief Create an avl tree
///
/// Create an avl tree on stack ready to use.<BR />
/// <B>The created tree must be destroyed with \a sowr_AvlTree_DestroyS().</B>
///
/// \param free_func Function to call when freeing an element
/// \param cmp_func Function for comparision
///
/// \return Created tree
///
sowr_AvlTree
sowr_AvlTree_CreateS( sowr_AvlTreeFreeFunc free_func, sowr_AvlTreeCmpFunc cmp_func );

///
/// \brief Insert data to avl tree
///
/// Insert some data to the avl tree.
///
/// \param tree Tree to insert
/// \param data_size Size of data
/// \param data Data to insert
///
void
sowr_AvlTree_Insert( sowr_AvlTree *tree, size_t data_size, const void *data );

///
/// \brief Delete element in tree
///
/// Delete an element in the avl tree.
///
/// \param tree Tree to delete from
/// \param data Data to delete
///
/// \return If anything is deleted
///
bool
sowr_AvlTree_Delete( sowr_AvlTree *tree, const void *data );

///
/// \brief Walk the avl tree
///
/// Walk the avl tree, from lowest to highest.
///
/// \param tree Tree to walk
/// \param func Function for walking
///
void
sowr_AvlTree_Walk( sowr_AvlTree *tree, sowr_AvlTreeWalkFunc func );

///
/// \brief Find an element in tree
///
/// Look up an element in the avl tree.
///
/// \param tree Tree to look for
/// \param data Data to look for
///
/// \return The node if found
///
sowr_AvlTreeNode *
sowr_AvlTree_Find( const sowr_AvlTree *tree, const void *data );

///
/// \brief Clear a tree
///
/// Clearing out an avl tree, freeing all its elements.
///
/// \param tree Tree to clear
///
void
sowr_AvlTree_Clear( sowr_AvlTree *tree );

///
/// \brief Get the height of tree
///
/// Get the height of the avl tree.
///
/// \param tree Tree to evaluate
///
/// \return Accumulated height
///
size_t
sowr_AvlTree_Height( const sowr_AvlTree *tree );

///
/// \brief Destroy a tree
///
/// Destroy an avl tree.
///
/// \param tree tree to destroy
///
void
sowr_AvlTree_Destroy( sowr_AvlTree *tree );

/// \brief Destroy a tree
///
/// Destroy an avl tree created by \a sowr_AvlTree_CreateS().
///
/// \param tree Tree to destroy
///
void
sowr_AvlTree_DestroyS( sowr_AvlTree *tree );

#endif // !SOWR_LIB_CONTAINER_AVL_TREE_H
