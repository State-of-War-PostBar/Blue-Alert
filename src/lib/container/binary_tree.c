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
bool
sowr_BinaryTreeNode_Find( const sowr_BinaryTreeNode *node, const void *elem, const sowr_BinaryTreeCmpFunc cmp )
{
    if (!node)
        return false;
    if (!cmp(node->data, elem))
        return true;
    return sowr_BinaryTreeNode_Find(node->left, elem, cmp) || sowr_BinaryTreeNode_Find(node->right, elem, cmp);
}

static
unsigned int
sowr_BinaryTreeNode_Height( const sowr_BinaryTreeNode *node )
{
    if (!node)
        return 0U;
    unsigned int left_height = 0U, right_height = 0U;
    left_height = sowr_BinaryTreeNode_Height(node->left);
    right_height = sowr_BinaryTreeNode_Height(node->right);

    return max(left_height, right_height) + 1U;
}

static
unsigned int
sowr_BinaryTreeNode_Width( const sowr_BinaryTreeNode *node, unsigned int level )
{
    if (!node)
        return 0U;
    if (level == 1U)
        return 1U;
    return sowr_BinaryTreeNode_Width(node->left, level - 1U) + sowr_BinaryTreeNode_Width(node->right, level - 1U);
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
sowr_BinaryTree_Create( size_t elem_size, sowr_BinaryTreeFreeFunc free_func )
{
    sowr_BinaryTree *tree = sowr_HeapAlloc(sizeof(sowr_BinaryTree));
    tree->elem_size = elem_size;
    tree->free_func = free_func;
    tree->head = NULL;
    tree->length = 0ULL;
    return tree;
}

sowr_BinaryTree
sowr_BinaryTree_CreateS( size_t elem_size, sowr_BinaryTreeFreeFunc free_func )
{
    sowr_BinaryTree tree =
    {
        .elem_size = elem_size,
        .free_func = free_func,
        .head = NULL,
        .length = 0ULL
    };
    return tree;
}

void
sowr_BinaryTree_Insert( sowr_BinaryTree *tree, const void *elem, const sowr_BinaryTreeCmpFunc cmp )
{
    sowr_BinaryTreeNode *node = sowr_HeapAlloc(sizeof(sowr_BinaryTreeNode));
    node->data = sowr_HeapAlloc(sizeof(tree->elem_size));
    memcpy(node->data, elem, tree->elem_size);
    node->left = node->right = NULL;

    if (!tree->length)
        tree->head = node;
    else
    {
        sowr_BinaryTreeNode *iter = tree->head;
        sowr_BinaryTreeNode *target = NULL;

        int result_r = 0;
        while (iter)
        {
            target = iter;
            int result = cmp(elem, iter->data);
            result_r = result;
            if (!result)
                return;
            else if (result < 0)
                iter = iter->left;
            else
                iter = iter->right;
        }

        if (result_r < 0)
            target->left = node;
        else
            target->right = node;
    }

    tree->length++;
}

bool
sowr_BinaryTree_Delete( sowr_BinaryTree *tree, const void *elem, const sowr_BinaryTreeCmpFunc cmp )
{
    sowr_BinaryTreeNode *iter = tree->head;
    sowr_BinaryTreeNode *previous = NULL;
    sowr_BinaryTreeNode *target = NULL;
    bool left_of_previous = false;

    while (iter)
    {
        int result = cmp(elem, iter->data);
        if (!result)
        {
            if (iter->left || iter->right)
            {
                if (!iter->left)
                {
                    // Only left child is empty, reattach right child
                    sowr_BinaryTreeNode *right = iter->right;

                    iter->data = right->data;
                    iter->left = right->left;
                    iter->right = right->right;

                    target = right;
                }
                else if (!iter->right)
                {
                    // Only right child is empty, reattach left child
                    sowr_BinaryTreeNode *left = iter->left;

                    iter->data = left->data;
                    iter->left = left->left;
                    iter->right = left->right;

                    target = left;
                }
                else
                {
                    // Neither left or right child is empty
                    sowr_BinaryTreeNode *target = iter;
                    sowr_BinaryTreeNode *leftest = iter->left;
                    while (leftest->right)
                    {
                        target = leftest;
                        leftest = leftest->right;
                    }
                    iter->data = leftest->data;
                    if (target != iter)
                        target->right = leftest->left;
                    else
                        target->left = leftest->left;

                    target = leftest;
                }
            }
            else
            {
                // Leaf is deleted
                target = iter;
                if (left_of_previous)
                    previous->left = NULL;
                else
                    previous->right = NULL;
            }
            // Delete the target
            if (tree->free_func)
                tree->free_func(target->data);
            sowr_HeapFree(target->data);
            sowr_HeapFree(target);
            if (tree->length > 1ULL)
                tree->length--;
            else
                tree->length = 0U;
            return true;
        }
        else if (result < 0)
        {
            previous = iter;
            left_of_previous = true;
            iter = iter->left;
        }
        else
        {
            previous = iter;
            left_of_previous = false;
            iter = iter->right;
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

bool
sowr_BinaryTree_Find( const sowr_BinaryTree *tree, const void *elem , const sowr_BinaryTreeCmpFunc cmp )
{
    if (!tree->length)
        return false;
    return sowr_BinaryTreeNode_Find(tree->head, elem, cmp);
}

void
sowr_BinaryTree_Clear( sowr_BinaryTree *tree )
{
    if (!tree->length)
        return;
    sowr_BinaryTreeNode_Clear(tree->head, tree->free_func);
    tree->length = 0ULL;
}

unsigned int
sowr_BinaryTree_Height( const sowr_BinaryTree *tree )
{
    if (!tree->length)
        return 0U;
    return sowr_BinaryTreeNode_Height(tree->head);
}

unsigned int
sowr_BinaryTree_Width( const sowr_BinaryTree *tree )
{
    if (!tree->length)
        return 0U;

    unsigned int max_width = 0U;
    unsigned int current_width = 0U;
    unsigned int height = sowr_BinaryTree_Height(tree);

    for (unsigned int i = 1U; i <= height; i++)
    {
        current_width = sowr_BinaryTreeNode_Width(tree->head, i);
        max_width = max(current_width, max_width);
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
