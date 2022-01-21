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

#include "binary_tree.h"

#include "../memory/heap_memory.h"

static
void
blrt_BinaryTreeNode_Walk( blrt_BinaryTreeNode *node, blrt_BinaryTreeWalkFunc func )
{
    if (!node)
        return;

    blrt_BinaryTreeNode_Walk(node->left, func);
    func(node->data);
    blrt_BinaryTreeNode_Walk(node->right, func);
}

static
size_t
blrt_BinaryTreeNode_Height( const blrt_BinaryTreeNode *node )
{
    if (!node)
        return 0ULL;

    size_t left_height = 0ULL, right_height = 0ULL;
    left_height = blrt_BinaryTreeNode_Height(node->left);
    right_height = blrt_BinaryTreeNode_Height(node->right);
    return (size_t)(fmaxl(left_height, right_height)) + 1ULL;
}

static
void
blrt_BinaryTreeNode_Clear( blrt_BinaryTreeNode *node, blrt_BinaryTreeFreeFunc free_func )
{
    if (!node)
        return;

    blrt_BinaryTreeNode_Clear(node->left, free_func);
    blrt_BinaryTreeNode_Clear(node->right, free_func);

    if (free_func)
        free_func(node->data);
    blrt_HeapFree(node->data);
    blrt_HeapFree(node);
}

blrt_BinaryTree *
blrt_BinaryTree_Create( blrt_BinaryTreeFreeFunc free_func )
{
    blrt_BinaryTree *tree = blrt_HeapAlloc(sizeof(blrt_BinaryTree));
    tree->free_func = free_func;
    tree->head = (blrt_BinaryTreeNode){ 0 };
    tree->length = 0ULL;
    return tree;
}

blrt_BinaryTree
blrt_BinaryTree_CreateS( blrt_BinaryTreeFreeFunc free_func )
{
    blrt_BinaryTree tree =
    {
        .free_func = free_func,
        .head = (blrt_BinaryTreeNode){ 0 },
        .length = 0ULL
    };
    return tree;
}

void
blrt_BinaryTree_Insert( blrt_BinaryTree *tree, size_t data_size, const void *data, blrt_BinaryTreeCmpFunc cmp_func )
{
    if (!tree->length)
    {
        tree->head.data_size = data_size;
        tree->head.data = blrt_HeapAlloc(data_size);
        memcpy(tree->head.data, data, data_size);
        tree->head.left = tree->head.right = NULL;
    }
    else
    {
        blrt_BinaryTreeNode *iter = &(tree->head), *previous = NULL;
        int result = 0;
        while (iter)
        {
            previous = iter;
            result = cmp_func(iter->data, data);
            if (!result)
                return;
            else if (result < 0)
                iter = iter->left;
            else
                iter = iter->right;
        }

        blrt_BinaryTreeNode *node = blrt_HeapAlloc(sizeof(blrt_BinaryTreeNode));
        node->data_size = data_size;
        node->data = blrt_HeapAlloc(data_size);
        memcpy(node->data, data, data_size);
        node->left = node->right = NULL;

        if (result < 0)
            previous->left = node;
        else
            previous->right = node;
    }
    tree->length++;
}

bool
blrt_BinaryTree_Delete( blrt_BinaryTree *tree, const void *data, blrt_BinaryTreeCmpFunc cmp_func )
{
    if (!tree->length)
        return false;

    blrt_BinaryTreeNode *iter = &(tree->head);
    blrt_BinaryTreeNode *previous = NULL;
    bool left_of_previous = true;
    int result = 0;
    while (iter)
    {
        result = cmp_func(iter->data, data);
        if (!result)
        {
            if (iter->left && iter->right)
            {
                // Node has two children
                // Find it successor (minimum after the node) and presuccessor (parent of successor).
                blrt_BinaryTreeNode *successor = iter->right, *presuccessor = iter;
                while (successor->left)
                {
                    presuccessor = successor;
                    successor = successor->left;
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
                    // No previous node
                    tree->head = *successor;
                successor->left = iter->left;
                successor->right = iter != presuccessor ? iter->right : successor->right;
            }
            else if (iter->left || iter->right)
            {
                // Node has one child, put it as node's parent's child
                blrt_BinaryTreeNode *child = iter->left ? iter->left : iter->right;
                if (previous)
                {
                    if (left_of_previous)
                        previous->left = child;
                    else
                        previous->right = child;
                }
                else
                {
                    tree->head = *child;
                }
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
                {
                    if (tree->free_func)
                        tree->free_func(tree->head.data);
                    blrt_HeapFree(tree->head.data);
                    tree->head = (blrt_BinaryTreeNode){ 0 };
                }
            }

            if (iter != &(tree->head))
            {
                if (tree->free_func)
                    tree->free_func(iter->data);
                blrt_HeapFree(iter->data);
                blrt_HeapFree(iter);
            }
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
blrt_BinaryTree_Walk( blrt_BinaryTree *tree, blrt_BinaryTreeWalkFunc func )
{
    if (!tree->length)
        return;

    blrt_BinaryTreeNode_Walk(&(tree->head), func);
}

blrt_BinaryTreeNode *
blrt_BinaryTree_Find( const blrt_BinaryTree *tree, const void *data, blrt_BinaryTreeCmpFunc cmp_func )
{
    if (!tree->length)
        return NULL;

    const blrt_BinaryTreeNode *iter = &(tree->head);
    int result = 0;
    while (iter)
    {
        result = cmp_func(iter->data, data);
        if (!result)
            return (blrt_BinaryTreeNode *)iter;
        else if (result < 0)
            iter = iter->left;
        else
            iter = iter->right;
    }

    return NULL;
}

void
blrt_BinaryTree_Clear( blrt_BinaryTree *tree )
{
    if (!tree->length)
        return;

    blrt_BinaryTreeNode_Clear(tree->head.left, tree->free_func);
    blrt_BinaryTreeNode_Clear(tree->head.right, tree->free_func);

    if (tree->head.data_size)
    {
        if (tree->free_func)
            tree->free_func(tree->head.data);
        blrt_HeapFree(tree->head.data);
    }

    tree->head = (blrt_BinaryTreeNode){ 0 };
    tree->length = 0ULL;
}

size_t
blrt_BinaryTree_Height( const blrt_BinaryTree *tree )
{
    if (!tree->length)
        return 0ULL;

    return blrt_BinaryTreeNode_Height(&(tree->head));
}

void
blrt_BinaryTree_Destroy( blrt_BinaryTree *tree )
{
    blrt_BinaryTree_Clear(tree);
    blrt_HeapFree(tree);
}

void
blrt_BinaryTree_DestroyS( blrt_BinaryTree *tree )
{
    blrt_BinaryTree_Clear(tree);
}
