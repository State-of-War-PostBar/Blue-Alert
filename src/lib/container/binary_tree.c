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
sowr_BinaryTreeNode_Walk( sowr_BinaryTreeNode *node, sowr_BinaryTreeWalkFunc func )
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
void
sowr_BinaryTreeNode_Clear( sowr_BinaryTreeNode *node, sowr_BinaryTreeFreeFunc free_func )
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
sowr_BinaryTree_Create( sowr_BinaryTreeFreeFunc free_func, sowr_BinaryTreeCmpFunc cmp_func )
{
    sowr_BinaryTree *tree = sowr_HeapAlloc(sizeof(sowr_BinaryTree));
    tree->free_func = free_func;
    tree->cmp_func = cmp_func;
    tree->head = NULL;
    tree->length = 0ULL;
    return tree;
}

sowr_BinaryTree
sowr_BinaryTree_CreateS( sowr_BinaryTreeFreeFunc free_func, sowr_BinaryTreeCmpFunc cmp_func )
{
    sowr_BinaryTree tree =
    {
        .free_func = free_func,
        .cmp_func = cmp_func,
        .head = NULL,
        .length = 0ULL
    };
    return tree;
}

void
sowr_BinaryTree_Insert( sowr_BinaryTree *tree, size_t data_size, const void *data )
{
    if (!tree->length)
    {
        sowr_BinaryTreeNode *node = sowr_HeapAlloc(sizeof(sowr_BinaryTreeNode));
        node->data = sowr_HeapAlloc(data_size);
        memcpy(node->data, data, data_size);
        node->left = node->right = NULL;
        tree->head = node;
    }
    else
    {
        sowr_BinaryTreeNode *iter = tree->head;
        sowr_BinaryTreeNode *target = NULL;

        int result = 0;
        while (iter)
        {
            target = iter;
            result = tree->cmp_func(iter->data, data);
            if (!result)
                return;
            else if (result < 0)
                iter = iter->left;
            else
                iter = iter->right;
        }

        sowr_BinaryTreeNode *node = sowr_HeapAlloc(sizeof(sowr_BinaryTreeNode));
        node->data = sowr_HeapAlloc(data_size);
        memcpy(node->data, data, data_size);
        node->left = node->right = NULL;

        if (result < 0)
            target->left = node;
        else
            target->right = node;
    }
    tree->length++;
}

bool
sowr_BinaryTree_Delete( sowr_BinaryTree *tree, const void *data )
{
    if (!tree->length)
        return false;

    sowr_BinaryTreeNode *iter = tree->head;
    sowr_BinaryTreeNode *previous = NULL;
    bool left_of_previous = true;
    int result = 0;
    while (iter)
    {
        result = tree->cmp_func(iter->data, data);
        if (!result)
        {
            if (iter->left && iter->right)
            {
                // Node has two children
                // Find it successor (minimum after the node) and presuccessor (parent of successor).
                sowr_BinaryTreeNode *successor = iter, *presuccessor = iter;
                while (iter->left)
                {
                    presuccessor = successor;
                    iter = iter->left;
                }

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
                // Node has one child, put it as node's parent's child
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
                // Node is leaf, just delete it
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
sowr_BinaryTree_Walk( sowr_BinaryTree *tree, sowr_BinaryTreeWalkFunc func )
{
    if (!tree->length)
        return;

    sowr_BinaryTreeNode_Walk(tree->head, func);
}

sowr_BinaryTreeNode *
sowr_BinaryTree_Find( const sowr_BinaryTree *tree, const void *data )
{
    if (!tree->length)
        return NULL;

    sowr_BinaryTreeNode *iter = tree->head;
    int result = 0;
    while (iter)
    {
        result = tree->cmp_func(iter->data, data);
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
