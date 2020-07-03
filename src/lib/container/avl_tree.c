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

#include "avl_tree.h"

#include "stack.h"
#include "../memory/heap_memory.h"

static
void
sowr_AvlTreeNode_Walk( sowr_AvlTreeNode *node, sowr_AvlTreeWalkFunc func )
{
    if (!node)
        return;

    sowr_AvlTreeNode_Walk(node->left, func);
    func(node->data);
    sowr_AvlTreeNode_Walk(node->right, func);
}

static
size_t
sowr_AvlTreeNode_Height( const sowr_AvlTreeNode *node )
{
    if (!node)
        return 0ULL;

    size_t left_height = 0ULL, right_height = 0ULL;
    left_height = sowr_AvlTreeNode_Height(node->left);
    right_height = sowr_AvlTreeNode_Height(node->right);
    return (size_t)(fmaxl(left_height, right_height)) + 1ULL;
}

static
void
sowr_AvlTreeNode_UpdateHeight( void *node )
{
    sowr_AvlTreeNode *ptr = node;
    ptr->height = sowr_AvlTreeNode_Height(ptr);
}

static
void
sowr_AvlTreeNode_UpdateHeightP( void *nodep )
{
    sowr_AvlTreeNode *ptr = *((sowr_AvlTreeNode **)nodep);
    ptr->height = sowr_AvlTreeNode_Height(ptr);
}

static
void
sowr_AvlTreeNode_Insert( sowr_AvlTreeNode *node, size_t data_size, const void *data, sowr_AvlTreeCmpFunc cmp_func )
{
    if (!node)
    {
        sowr_AvlTreeNode *new_node = sowr_HeapAlloc(sizeof(sowr_AvlTreeNode));
        new_node->data_size = data_size;
        new_node->data = sowr_HeapAlloc(sizeof(data_size));
        memcpy(new_node->data, data, data_size);
        new_node->left = new_node->right = NULL;
        new_node->height = 1;
    }
    else
    {
        int result = cmp_func(node->data, data);
        if (!result)
            return;
        if (result < 0)
        {
            if (node->left)
                sowr_AvlTreeNode_Insert(node->left, data_size, data, cmp_func);
            else
            {
                sowr_AvlTreeNode *new_node = sowr_HeapAlloc(sizeof(sowr_AvlTreeNode));
                new_node->data_size = data_size;
                new_node->data = sowr_HeapAlloc(sizeof(data_size));
                memcpy(new_node->data, data, data_size);
                new_node->left = new_node->right = NULL;
                new_node->height = 1;

                node->left = new_node;
            }
        }
        else
        {
            if (node->right)
                sowr_AvlTreeNode_Insert(node->right, data_size, data, cmp_func);
            else
            {
                sowr_AvlTreeNode *new_node = sowr_HeapAlloc(sizeof(sowr_AvlTreeNode));
                new_node->data_size = data_size;
                new_node->data = sowr_HeapAlloc(sizeof(data_size));
                memcpy(new_node->data, data, data_size);
                new_node->left = new_node->right = NULL;
                new_node->height = 1;

                node->right = new_node;
            }
        }
        sowr_AvlTreeNode_UpdateHeight(node);
    }
}

static
int
sowr_AvlTreeNode_BalanceFactor( const sowr_AvlTreeNode *node )
{
    if (node)
        return (node->left ? node->left->height : 0) - (node->right ? node->right->height : 0);
    return -1;
}

static
sowr_AvlTreeNode *
sowr_AvlTreeNode_RotateLeft( sowr_AvlTreeNode *node )
{
    if (!node->right)
        return node;

    sowr_AvlTreeNode *right = node->right;
    node->right = right->left ? right->left : NULL;
    right->left = node;

    right->height = (size_t)(fmaxl(sowr_AvlTreeNode_Height(right->left), sowr_AvlTreeNode_Height(right->right))) + 1ULL;
    node->height = (size_t)(fmaxl(sowr_AvlTreeNode_Height(node->left), sowr_AvlTreeNode_Height(node->right))) + 1ULL;

    return right;
}

static
sowr_AvlTreeNode *
sowr_AvlTreeNode_RotateRight( sowr_AvlTreeNode *node )
{
    if (!node->left)
        return node;

    sowr_AvlTreeNode *left = node->left;
    node->left = left->right ? left->right : NULL;
    left->right = node;

    left->height = (size_t)(fmaxl(sowr_AvlTreeNode_Height(left->left), sowr_AvlTreeNode_Height(left->right))) + 1ULL;
    node->height = (size_t)(fmaxl(sowr_AvlTreeNode_Height(node->left), sowr_AvlTreeNode_Height(node->right))) + 1ULL;

    return left;
}

static
sowr_AvlTreeNode *
sowr_AvlTreeNode_Rebalance( sowr_AvlTreeNode *node )
{
    int factor = sowr_AvlTreeNode_BalanceFactor(node);
    if (factor > 1 && sowr_AvlTreeNode_BalanceFactor(node->left) > 0)
        return sowr_AvlTreeNode_RotateRight(node);
    else if (factor > 1 && sowr_AvlTreeNode_BalanceFactor(node->left) <= 0)
    {
        node->left = sowr_AvlTreeNode_RotateLeft(node->left);
        return sowr_AvlTreeNode_RotateRight(node);
    }
    else if (factor < -1 && sowr_AvlTreeNode_BalanceFactor(node->right) <= 0)
        return sowr_AvlTreeNode_RotateLeft(node);
    else if (factor < -1 && sowr_AvlTreeNode_BalanceFactor(node->right) > 0)
    {
        node->right = sowr_AvlTreeNode_RotateRight(node->right);
        return sowr_AvlTreeNode_RotateLeft(node);
    }
    else
        return node;
}

static
void
sowr_AvlTreeNode_Clear( sowr_AvlTreeNode *node, sowr_AvlTreeFreeFunc free_func )
{
    if (!node)
        return;

    sowr_AvlTreeNode_Clear(node->left, free_func);
    sowr_AvlTreeNode_Clear(node->right, free_func);

    if (free_func)
        free_func(node->data);
    sowr_HeapFree(node->data);
    sowr_HeapFree(node);
}

sowr_AvlTree *
sowr_AvlTree_Create( sowr_AvlTreeFreeFunc free_func, sowr_AvlTreeCmpFunc cmp_func )
{
    sowr_AvlTree *tree = sowr_HeapAlloc(sizeof(sowr_AvlTree));
    tree->free_func = free_func;
    tree->cmp_func = cmp_func;
    tree->head = NULL;
    tree->length = 0ULL;
    return tree;
}

sowr_AvlTree
sowr_AvlTree_CreateS( sowr_AvlTreeFreeFunc free_func, sowr_AvlTreeCmpFunc cmp_func )
{
    sowr_AvlTree tree =
    {
        .free_func = free_func,
        .cmp_func = cmp_func,
        .head = NULL,
        .length = 0ULL
    };
    return tree;
}

void
sowr_AvlTree_Insert( sowr_AvlTree *tree, size_t data_size, const void *data )
{
    if (!tree->length)
    {
        sowr_AvlTreeNode *node = sowr_HeapAlloc(sizeof(sowr_AvlTreeNode));
        node->data_size = data_size;
        node->data = sowr_HeapAlloc(data_size);
        memcpy(node->data, data, data_size);
        node->left = node->right = NULL;
        node->height = 0;
        tree->head = node;
    }
    else
        sowr_AvlTreeNode_Insert(tree->head, data_size, data, tree->cmp_func);

    tree->head = sowr_AvlTreeNode_Rebalance(tree->head);
    tree->length++;
}

bool
sowr_AvlTree_Delete( sowr_AvlTree *tree, const void *data )
{
    if (!tree->length)
        return false;

    sowr_AvlTreeNode *iter = tree->head;
    sowr_AvlTreeNode *previous = NULL;
    sowr_Stack stack = sowr_Stack_CreateS(sizeof(sowr_AvlTreeNode *), NULL);
    bool left_of_previous = true;
    int result = 0;
    while (iter)
    {
        sowr_Stack_Push(&stack, &iter);
        result = tree->cmp_func(iter->data, data);
        if (!result)
        {
            sowr_Stack_Pop(&stack, NULL);
            if (iter->left && iter->right)
            {
                sowr_AvlTreeNode *successor = iter->right, *presuccessor = iter;
                while (successor->left)
                {
                    presuccessor = successor;
                    successor = successor->left;
                }

                if (iter != presuccessor)
                    presuccessor->left = successor->right ? successor->right : NULL;

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
                sowr_AvlTreeNode *child = iter->left ? iter->left : iter->right;
                if (previous)
                {
                    if (left_of_previous)
                        previous->left = child;
                    else
                        previous->right = child;
                }
            }
            else
            {
                if (previous)
                {
                    if (left_of_previous)
                        previous->left = NULL;
                    else
                        previous->right = NULL;
                }
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
            if (previous < 0)
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
    sowr_Stack_Eat(&stack, sowr_AvlTreeNode_UpdateHeightP);
    tree->head = sowr_AvlTreeNode_Rebalance(tree->head);
    sowr_Stack_DestroyS(&stack);
    return false;
}

void
sowr_AvlTree_Walk( sowr_AvlTree *tree, sowr_AvlTreeWalkFunc func )
{
    if (!tree->length)
        return;

    sowr_AvlTreeNode_Walk(tree->head, func);
}

sowr_AvlTreeNode *
sowr_AvlTree_Find( const sowr_AvlTree *tree, const void *data )
{
    if (!tree->length)
        return NULL;

    sowr_AvlTreeNode *iter = tree->head;
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
sowr_AvlTree_Clear( sowr_AvlTree *tree )
{
    if (!tree->length)
        return;

    sowr_AvlTreeNode_Clear(tree->head, tree->free_func);
    tree->length = 0ULL;
}

size_t
sowr_AvlTree_Height( const sowr_AvlTree *tree )
{
    if (!tree->length)
        return 0ULL;

    return sowr_AvlTreeNode_Height(tree->head);
}

void
sowr_AvlTree_Destroy( sowr_AvlTree *tree )
{
    sowr_AvlTree_Clear(tree);
    sowr_HeapFree(tree);
}

void
sowr_AvlTree_DestroyS( sowr_AvlTree *tree )
{
    sowr_AvlTree_Clear(tree);
}
