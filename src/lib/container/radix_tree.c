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

#include "radix_tree.h"

#include "../memory/heap_memory.h"

static
blrt_RadixTreeNode *
blrt_RadixTreeNode_Gen( void )
{
    blrt_RadixTreeNode *node = blrt_HeapZeroAlloc(sizeof(blrt_RadixTreeNode));
    node->key = blrt_String_CreateS();
    node->full_key = blrt_String_CreateS();
    return node;
}

static
void
blrt_RadixTreeNode_DeleteAfter( blrt_RadixTreeNode *node, blrt_RadixTreeFreeFunc free_func )
{
    if (!node)
        return;

    if (node->children)
        for (size_t i = 0ULL; i < CHAR_MAX; i++)
            if (node->characters[i])
            {
                blrt_RadixTreeNode_DeleteAfter(node->characters[i], free_func);
                node->characters[i] = NULL;
                node->children--;
            }

    if (node->data)
    {
        if (free_func)
            free_func(node->data);
        blrt_HeapFree(node->data);
    }
    blrt_String_DestroyS(&(node->key));
    blrt_String_DestroyS(&(node->full_key));
    blrt_HeapFree(node);
}

static
void
blrt_RadixTreeNode_TransferChildren( blrt_RadixTreeNode *target, blrt_RadixTreeNode *origin )
{
    if (!target || !origin)
        return;

    target->children = origin->children;
    origin->children = 0ULL;

    for (size_t i = 0ULL; i < CHAR_MAX; i++)
    {
        target->characters[i] = origin->characters[i];
        origin->characters[i] = NULL;
    }
}

blrt_RadixTree *
blrt_RadixTree_Create( blrt_RadixTreeFreeFunc free_func )
{
    blrt_RadixTreeNode head =
    {
        .children = 0ULL,
        .data_size = 0ULL,
        .data = NULL,
        .key = {},
        .full_key = {},
        .characters = { NULL }
    };
    blrt_RadixTree *tree = blrt_HeapAlloc(sizeof(blrt_RadixTree));
    tree->free_func = free_func;
    tree->head = head;
    return tree;
}

blrt_RadixTree
blrt_RadixTree_CreateS( blrt_RadixTreeFreeFunc free_func )
{
    blrt_RadixTreeNode head =
    {
        .children = 0ULL,
        .data_size = 0ULL,
        .data = NULL,
        .key = {},
        .full_key = {},
        .characters = { NULL }
    };
    blrt_RadixTree tree =
    {
        .free_func = free_func,
        .head = head
    };
    return tree;
}

void
blrt_RadixTree_Clear( blrt_RadixTree *tree )
{
    if (!tree)
        return;

    if (tree->head.children)
        for (size_t i = 0ULL; i < CHAR_MAX; i++)
            blrt_RadixTreeNode_DeleteAfter(tree->head.characters[i], tree->free_func);
    tree->head.children = 0ULL;
}

void
blrt_RadixTree_Insert( blrt_RadixTree *tree, const char *index, size_t data_size, const void *data )
{
    if (!tree)
        return;

    const char *index_o = index;
    blrt_RadixTreeNode *iter = &(tree->head);
    size_t ch = (size_t)*index_o;
    while (ch)
    {
        if (!iter->characters[ch])
        {
            blrt_RadixTreeNode *node = blrt_RadixTreeNode_Gen();
            blrt_String_PushS(&(node->key), index_o);
            blrt_String_PushS(&(node->full_key), index);
            node->data_size = data_size;
            node->data = blrt_HeapAlloc(data_size);
            memcpy(node->data, data, data_size);
            iter->characters[ch] = node;
            iter->children++;
            return;
        }
        else
        {
            blrt_RadixTreeNode *current = iter->characters[ch];
            size_t compared = 0ULL;
            const char *index_r = index_o;
            const char *target = current->key.ptr;

            // Eat up the target key and index string.
            while (*index_r == *target && *index_r && *target)     // We cannot use *index_r++ == *target++ here!
                compared++, index_r++, target++;

            // If the target string eat up the index string, split target string and get its content to a new lower node
            if (*target && !*index_r)
            {
                blrt_RadixTreeNode *new_node = blrt_RadixTreeNode_Gen();
                blrt_String_PushS(&(new_node->key), target);
                if (current->data)
                {
                    new_node->data_size = current->data_size;
                    new_node->data = blrt_HeapAlloc(new_node->data_size);
                    memcpy(new_node->data, current->data, new_node->data_size);
                }
                blrt_RadixTreeNode_TransferChildren(new_node, current);

                if (current->data_size != data_size)
                {
                    current->data_size = data_size;
                    current->data = current->data ? blrt_ReAlloc(data_size, current->data) : blrt_HeapAlloc(data_size);
                }
                memcpy(current->data, data, data_size);
                current->characters[(size_t)*target] = new_node;
                current->children++;
                blrt_String_Res(&(current->key), compared);
                blrt_String_ShrinkToFit(&(current->key));
                return;
            }

            // If the index string eat up the target string, target node remains unchanged and proceed search for index node to its lower children
            else if (!*target && *index_r)
            {
                ch = (size_t)*index_r;
                index_o = index_r;
                iter = current;
                continue;
            }

            // If both are eaten up, the index is overlapped, so we override old content or establish new content
            else if (!*target && !*index_r)
            {
                if (current->data)
                {
                    if (tree->free_func)
                        tree->free_func(current->data);
                    if (current->data_size != data_size)
                    {
                        current->data_size = data_size;
                        current->data = blrt_ReAlloc(data_size, current->data);
                    }
                    memcpy(current->data, data, data_size);
                }
                else
                {
                    current->data_size = data_size;
                    current->data = blrt_HeapAlloc(data_size);
                    memcpy(current->data, data, data_size);
                }
                return;
            }

            // If both have remaining parts, split from their common parts and turn them into different nodes
            else/* if (*target && *index_r) */
            {
                // Create new node to hold the remaining key of the target node
                blrt_RadixTreeNode *new_node = blrt_RadixTreeNode_Gen();
                blrt_String_PushS(&(new_node->key), target);
                blrt_String_PushS(&(new_node->full_key), current->full_key.ptr);
                if (current->data)
                {
                    new_node->data_size = current->data_size;
                    new_node->data = blrt_HeapAlloc(current->data_size);
                    memcpy(new_node->data, current->data, current->data_size);
                    blrt_HeapFree(current->data);
                    current->data = NULL;
                    current->data_size = 0ULL;
                }
                blrt_RadixTreeNode_TransferChildren(new_node, current);
                current->characters[(size_t)*target] = new_node;
                current->children++;
                blrt_String_Res(&(current->key), compared);

                ch = (size_t)*index_r;
                index_o = index_r;
                iter = current;
            }
        }
    }
}

inline
void
blrt_RadixTree_InsertS( blrt_RadixTree *tree, const blrt_String *index, size_t data_size, const void *data )
{
    blrt_RadixTree_Insert(tree, index->ptr, data_size, data);
}

blrt_RadixTreeNode *
blrt_RadixTree_Get( const blrt_RadixTree *tree, const char *index )
{
    if (!tree)
        return NULL;

    const blrt_RadixTreeNode *iter = &(tree->head);
    size_t ch = (size_t)*index;
    while (ch)
    {
        if (!iter->characters[ch])
            return NULL;
        else
        {
            const blrt_RadixTreeNode *current = iter->characters[ch];
            const char *index_r = index;
            const char *target = current->key.ptr;

            while (*index_r == *target && *index_r && *target)
                index_r++, target++;

            if (!*target && !*index_r)
                return (blrt_RadixTreeNode *)current;
            else if (!*target && *index_r)
            {
                ch = (size_t)*index_r;
                index = index_r;
                iter = current;
                continue;
            }
            else
                return NULL;
        }
    }

    return NULL;
}

inline
blrt_RadixTreeNode *
blrt_RadixTree_GetS( const blrt_RadixTree *tree, const blrt_String *index )
{
    return blrt_RadixTree_Get(tree, index->ptr);
}

void
blrt_RadixTree_Walk( const blrt_RadixTreeNode *node, blrt_RadixTreeWalkFunc func )
{
    if (!node)
        return;

    for (size_t i = 0ULL; i < CHAR_MAX; i++)
        blrt_RadixTree_Walk(node->characters[i], func);

    if (node->data)
        func((void *)node);
}

size_t
blrt_RadixTree_ListAllChildren( const blrt_RadixTreeNode *node, blrt_HashMap *output )
{
    if (!node)
        return 0ULL;

    size_t children = 0ULL;

    for (size_t i = 0ULL; i < CHAR_MAX; i++)
        children += blrt_RadixTree_ListAllChildren(node->characters[i], output);

    if (node->data)
    {
        if (output)
            blrt_HashMap_InsertCV(output, node->full_key.ptr, node->data_size, node->data);
        children++;
    }

    return children;
}

bool
blrt_RadixTree_Delete( blrt_RadixTree *tree, const char *index )
{
    if (!tree)
        return false;

    blrt_RadixTreeNode *iter = &(tree->head);
    size_t ch = (size_t)*index;
    while (ch)
    {
        if (!iter->characters[ch])
            return false;

        else
        {
            blrt_RadixTreeNode *current = iter->characters[ch];
            const char *index_r = index;
            const char *target = current->key.ptr;

            while (*index_r == *target && *index_r && *target)
                index_r++, target++;

            if (!*target && !*index_r)
            {
                if (current->data)
                {
                    current->data_size = 0ULL;
                    blrt_HeapFree(current->data);
                    current->data = NULL;
                }
                return true;
            }
            else if (!*target && *index_r)
            {
                ch = (size_t)*index_r;
                index = index_r;
                iter = current;
                continue;
            }
            else
                return false;
        }
    }

    return false;
}

inline
bool
blrt_RadixTree_DeleteS( blrt_RadixTree *tree, const blrt_String *index )
{
    return blrt_RadixTree_Delete(tree, index->ptr);
}

void
blrt_RadixTree_Destroy( blrt_RadixTree *tree )
{
    blrt_RadixTree_Clear(tree);
    blrt_HeapFree(tree);
}

void
blrt_RadixTree_DestroyS( blrt_RadixTree *tree )
{
    blrt_RadixTree_Clear(tree);
}
