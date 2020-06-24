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

#include "binary_tree.h"

#include "../memory/heap_memory.h"

static
void
sowr_BinaryTreeNode_Walk( sowr_BinaryTreeNode *node, const sowr_BinaryTreeWalkFunc func )
{
    if (!node)
        return;
    sowr_BinaryTreeNode_Walk(node->left, func);
    func(node->data);
    sowr_BinaryTreeNode_Walk(node->right, func);
}

static
size_t
sowr_BinaryTreeNode_Height( const sowr_BinaryTreeNode *node )
{
    if (!node)
        return 0ULL;
    size_t left_height = 0ULL, right_height = 0ULL;
    left_height = sowr_BinaryTreeNode_Height(node->left);
    right_height = sowr_BinaryTreeNode_Height(node->right);

    return (left_height ^ ((left_height ^ right_height) & -(left_height < right_height))) + 1ULL;
}

static
size_t
sowr_BinaryTreeNode_Width( const sowr_BinaryTreeNode *node, size_t level )
{
    if (!node)
        return 0ULL;
    if (level == 1ULL)
        return 1ULL;
    return sowr_BinaryTreeNode_Width(node->left, level - 1ULL) + sowr_BinaryTreeNode_Width(node->right, level - 1ULL);
}

static
sowr_BinaryTreeNode *
sowr_BinaryTreeNode_MinAfter( const sowr_BinaryTreeNode *node )
{
    if (!node)
        return NULL;
    if (!node->left)
        return (sowr_BinaryTreeNode *)node;
    return sowr_BinaryTreeNode_MinAfter(node->left);
}

static
sowr_BinaryTreeNode *
sowr_BinaryTreeNode_ParentOf( const sowr_BinaryTreeNode *head, const sowr_BinaryTreeNode *node, const sowr_BinaryTreeCmpFunc cmp )
{
    if (!head || !node)
        return NULL;

    const sowr_BinaryTreeNode *iter = head;
    const sowr_BinaryTreeNode *previous = NULL;
    int result = 0;
    while (iter)
    {
        result = cmp(node->data, iter->data);
        if (!result)
            return (sowr_BinaryTreeNode *)previous;
        else
        {
            previous = iter;
            if (result < 0)
                iter = iter->left;
            else
                iter = iter->right;
        }
    }
    return NULL;
}

static
void
sowr_BinaryTreeNode_Clear( sowr_BinaryTreeNode *node, const sowr_BinaryTreeFreeFunc free_func )
{
    if (!node)
        return;
    sowr_BinaryTreeNode_Clear(node->left, free_func);
    sowr_BinaryTreeNode_Clear(node->right, free_func);

    if (free_func)
        free_func(node->data);
    sowr_HeapFree(node->data);
    sowr_HeapFree(node);
}

sowr_BinaryTree *
sowr_BinaryTree_Create( size_t elem_size, const sowr_BinaryTreeFreeFunc free_func, const sowr_BinaryTreeCmpFunc cmp_func )
{
    sowr_BinaryTree *tree = sowr_HeapAlloc(sizeof(sowr_BinaryTree));
    tree->elem_size = elem_size;
    tree->free_func = free_func;
    tree->cmp_func = cmp_func;
    tree->head = NULL;
    tree->length = 0ULL;
    return tree;
}

sowr_BinaryTree
sowr_BinaryTree_CreateS( size_t elem_size, const sowr_BinaryTreeFreeFunc free_func, const sowr_BinaryTreeCmpFunc cmp_func )
{
    sowr_BinaryTree tree =
    {
        .elem_size = elem_size,
        .free_func = free_func,
        .cmp_func = cmp_func,
        .head = NULL,
        .length = 0ULL
    };
    return tree;
}

void
sowr_BinaryTree_Insert( sowr_BinaryTree *tree, const void *elem )
{
    if (!tree->length)
    {
        sowr_BinaryTreeNode *node = sowr_HeapAlloc(sizeof(sowr_BinaryTreeNode));
        node->data = sowr_HeapAlloc(sizeof(tree->elem_size));
        memcpy(node->data, elem, tree->elem_size);
        node->left = node->right = NULL;
        tree->head = node;
    }
    else
    {
        sowr_BinaryTreeNode *iter = tree->head;
        sowr_BinaryTreeNode *target = NULL;

        int result_r = 0;
        while (iter)
        {
            target = iter;
            int result = tree->cmp_func(elem, iter->data);
            result_r = result;
            if (!result)
                return;         // Duplicate data, do nothing
            else if (result < 0)
                iter = iter->left;
            else
                iter = iter->right;
        }

        sowr_BinaryTreeNode *node = sowr_HeapAlloc(sizeof(sowr_BinaryTreeNode));
        node->data = sowr_HeapAlloc(sizeof(tree->elem_size));
        memcpy(node->data, elem, tree->elem_size);
        node->left = node->right = NULL;

        if (result_r < 0)
            target->left = node;
        else
            target->right = node;
    }

    tree->length++;
}

bool
sowr_BinaryTree_Delete( sowr_BinaryTree *tree, const void *elem )
{
    if (!tree->length)
        return false;

    sowr_BinaryTreeNode *iter = tree->head;
    sowr_BinaryTreeNode *previous = NULL;
    bool left_of_previous = true;
    int result = 0;
    while (iter)
    {
        result = tree->cmp_func(elem, iter->data);
        if (!result)
        {
            if (iter->left && iter->right)
            {
                // Node has two children
                sowr_BinaryTreeNode *successor = sowr_BinaryTreeNode_MinAfter(iter->right);
                sowr_BinaryTreeNode *presuccessor = sowr_BinaryTreeNode_ParentOf(tree->head, successor, tree->cmp_func);

                // If successor has a right child (it can never have a left child)
                // make it presuccessor's left child, unless its presuccessor is the deleting node itself.
                if (iter != presuccessor)
                    presuccessor->left = successor->right ? successor->right : NULL;
                // Otherwise (presuccessor is the node), we do nothing and proceed.

                // Switch the connection of the deleting node and the successor
                if (previous)
                {
                    if (left_of_previous)
                        previous->left = successor;
                    else
                        previous->right = successor;
                }
                else
                    tree->head = successor;
                successor->left = iter->left;
                successor->right = iter != presuccessor ? iter->right : successor->right;
            }
            else if (iter->left || iter->right)
            {
                // Node has one child
                sowr_BinaryTreeNode *child = iter->left ? iter->left : iter->right;
                if (previous)
                {
                    if (left_of_previous)
                        previous->left = child;
                    else
                        previous->right = child;
                }
                else
                    tree->head = child;
            }
            else
            {
                // Node is leaf
                if (previous)
                {
                    if (left_of_previous)
                        previous->left = NULL;
                    else
                        previous->right = NULL;
                }
                else
                    tree->head = NULL;
            }

            if (tree->free_func)
                tree->free_func(iter->data);
            sowr_HeapFree(iter->data);
            sowr_HeapFree(iter);
            tree->length--;
            return true;
        }
        else
        {
            previous = iter;
            if (result < 0)
            {
                left_of_previous = true;
                iter = iter->left;
            }
            else
            {
                left_of_previous = false;
                iter = iter->right;
            }
        }
    }
    return false;
}

void
sowr_BinaryTree_Walk( sowr_BinaryTree *tree, const sowr_BinaryTreeWalkFunc func )
{
    if (!tree->length)
        return;
    sowr_BinaryTreeNode_Walk(tree->head, func);
}

sowr_BinaryTreeNode *
sowr_BinaryTree_Find( const sowr_BinaryTree *tree, const void *elem )
{
    if (!tree->length)
        return NULL;

    sowr_BinaryTreeNode *iter = tree->head;
    int result = 0;
    while (iter)
    {
        result = tree->cmp_func(elem, iter->data);
        if (!result)
            return iter;
        else if (result < 0)
            iter = iter->left;
        else
            iter = iter->right;
    }

    return NULL;
}

void
sowr_BinaryTree_Clear( sowr_BinaryTree *tree )
{
    if (!tree->length)
        return;
    sowr_BinaryTreeNode_Clear(tree->head, tree->free_func);
    tree->length = 0ULL;
}

size_t
sowr_BinaryTree_Height( const sowr_BinaryTree *tree )
{
    if (!tree->length)
        return 0ULL;
    return sowr_BinaryTreeNode_Height(tree->head);
}

size_t
sowr_BinaryTree_Width( const sowr_BinaryTree *tree )
{
    if (!tree->length)
        return 0ULL;

    size_t max_width = 0ULL;
    size_t current_width = 0ULL;
    size_t height = sowr_BinaryTree_Height(tree);

    for (size_t i = 1ULL; i <= height; i++)
    {
        current_width = sowr_BinaryTreeNode_Width(tree->head, i);
        max_width = current_width ^ ((current_width ^ max_width) & -(current_width < max_width));
    }

    return max_width;
}

void
sowr_BinaryTree_Destroy( sowr_BinaryTree *tree )
{
    sowr_BinaryTree_Clear(tree);
    sowr_HeapFree(tree);
}

void
sowr_BinaryTree_DestroyS( sowr_BinaryTree *tree )
{
    sowr_BinaryTree_Clear(tree);
}
