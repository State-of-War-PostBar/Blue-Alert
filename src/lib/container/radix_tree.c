/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (c) 2017 - 2021 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*    State of War: Remastered is a free software. You can freely do whatever you want with it    *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                                   published by mhtvsSFrpHdE.                                   *
*                                                                                                *
*  By the time this line is written, the version of the license document is 1, but you may use   *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
*     State of War: Remastered is created, intended to be useful, but without any warranty.      *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*       You should have received a copy of the license along with the source code of this        *
*  program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.   *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                         https://github.com/State-of-War-PostBar/sowr.                          *
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#include "radix_tree.h"

#include "../memory/heap_memory.h"

static
sowr_RadixTreeNode *
sowr_RadixTreeNode_Gen( void )
{
    sowr_RadixTreeNode *node = sowr_HeapZeroAlloc(sizeof(sowr_RadixTreeNode));
    node->key = sowr_String_CreateS();
    node->full_key = sowr_String_CreateS();
    return node;
}

static
void
sowr_RadixTreeNode_DeleteAfter( sowr_RadixTreeNode *node, sowr_RadixTreeFreeFunc free_func )
{
    if (!node)
        return;

    if (node->children)
        for (size_t i = 0ULL; i < CHAR_MAX; i++)
            if (node->characters[i])
            {
                sowr_RadixTreeNode_DeleteAfter(node->characters[i], free_func);
                node->characters[i] = NULL;
                node->children--;
            }

    if (node->data)
    {
        if (free_func)
            free_func(node->data);
        sowr_HeapFree(node->data);
    }
    sowr_String_DestroyS(&(node->key));
    sowr_String_DestroyS(&(node->full_key));
    sowr_HeapFree(node);
}

static
void
sowr_RadixTreeNode_TransferChildren( sowr_RadixTreeNode *target, sowr_RadixTreeNode *origin )
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

sowr_RadixTree *
sowr_RadixTree_Create( sowr_RadixTreeFreeFunc free_func )
{
    sowr_RadixTreeNode head =
    {
        .children = 0ULL,
        .data_size = 0ULL,
        .data = NULL,
        .key = {},
        .full_key = {},
        .characters = { NULL }
    };
    sowr_RadixTree *tree = sowr_HeapAlloc(sizeof(sowr_RadixTree));
    tree->free_func = free_func;
    tree->head = head;
    return tree;
}

sowr_RadixTree
sowr_RadixTree_CreateS( sowr_RadixTreeFreeFunc free_func )
{
    sowr_RadixTreeNode head =
    {
        .children = 0ULL,
        .data_size = 0ULL,
        .data = NULL,
        .key = {},
        .full_key = {},
        .characters = { NULL }
    };
    sowr_RadixTree tree =
    {
        .free_func = free_func,
        .head = head
    };
    return tree;
}

void
sowr_RadixTree_Clear( sowr_RadixTree *tree )
{
    if (!tree)
        return;

    if (tree->head.children)
        for (size_t i = 0ULL; i < CHAR_MAX; i++)
            sowr_RadixTreeNode_DeleteAfter(tree->head.characters[i], tree->free_func);
    tree->head.children = 0ULL;
}

void
sowr_RadixTree_Insert( sowr_RadixTree *tree, const char *index, size_t data_size, const void *data )
{
    if (!tree)
        return;

    const char *index_o = index;
    sowr_RadixTreeNode *iter = &(tree->head);
    size_t ch = (size_t)*index_o;
    while (ch)
    {
        if (!iter->characters[ch])
        {
            sowr_RadixTreeNode *node = sowr_RadixTreeNode_Gen();
            sowr_String_PushS(&(node->key), index_o);
            sowr_String_PushS(&(node->full_key), index);
            node->data_size = data_size;
            node->data = sowr_HeapAlloc(data_size);
            memcpy(node->data, data, data_size);
            iter->characters[ch] = node;
            iter->children++;
            return;
        }
        else
        {
            sowr_RadixTreeNode *current = iter->characters[ch];
            size_t compared = 0ULL;
            const char *index_r = index_o;
            const char *target = current->key.ptr;

            // Eat up the target key and index string.
            while (*index_r == *target && *index_r && *target)     // We cannot use *index_r++ == *target++ here!
                compared++, index_r++, target++;

            // If the target string eat up the index string, split target string and get its content to a new lower node
            if (*target && !*index_r)
            {
                sowr_RadixTreeNode *new_node = sowr_RadixTreeNode_Gen();
                sowr_String_PushS(&(new_node->key), target);
                if (current->data)
                {
                    new_node->data_size = current->data_size;
                    new_node->data = sowr_HeapAlloc(new_node->data_size);
                    memcpy(new_node->data, current->data, new_node->data_size);
                }
                sowr_RadixTreeNode_TransferChildren(new_node, current);

                if (current->data_size != data_size)
                {
                    current->data_size = data_size;
                    current->data = current->data ? sowr_ReAlloc(data_size, current->data) : sowr_HeapAlloc(data_size);
                }
                memcpy(current->data, data, data_size);
                current->characters[(size_t)*target] = new_node;
                current->children++;
                sowr_String_Res(&(current->key), compared);
                sowr_String_ShrinkToFit(&(current->key));
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
                        current->data = sowr_ReAlloc(data_size, current->data);
                    }
                    memcpy(current->data, data, data_size);
                }
                else
                {
                    current->data_size = data_size;
                    current->data = sowr_HeapAlloc(data_size);
                    memcpy(current->data, data, data_size);
                }
                return;
            }

            // If both have remaining parts, split from their common parts and turn them into different nodes
            else/* if (*target && *index_r) */
            {
                // Create new node to hold the remaining key of the target node
                sowr_RadixTreeNode *new_node = sowr_RadixTreeNode_Gen();
                sowr_String_PushS(&(new_node->key), target);
                sowr_String_PushS(&(new_node->full_key), current->full_key.ptr);
                if (current->data)
                {
                    new_node->data_size = current->data_size;
                    new_node->data = sowr_HeapAlloc(current->data_size);
                    memcpy(new_node->data, current->data, current->data_size);
                    sowr_HeapFree(current->data);
                    current->data = NULL;
                    current->data_size = 0ULL;
                }
                sowr_RadixTreeNode_TransferChildren(new_node, current);
                current->characters[(size_t)*target] = new_node;
                current->children++;
                sowr_String_Res(&(current->key), compared);

                ch = (size_t)*index_r;
                index_o = index_r;
                iter = current;
            }
        }
    }
}

inline
void
sowr_RadixTree_InsertS( sowr_RadixTree *tree, const sowr_String *index, size_t data_size, const void *data )
{
    sowr_RadixTree_Insert(tree, index->ptr, data_size, data);
}

sowr_RadixTreeNode *
sowr_RadixTree_Get( const sowr_RadixTree *tree, const char *index )
{
    if (!tree)
        return NULL;

    const sowr_RadixTreeNode *iter = &(tree->head);
    size_t ch = (size_t)*index;
    while (ch)
    {
        if (!iter->characters[ch])
            return NULL;
        else
        {
            const sowr_RadixTreeNode *current = iter->characters[ch];
            const char *index_r = index;
            const char *target = current->key.ptr;

            while (*index_r == *target && *index_r && *target)
                index_r++, target++;

            if (!*target && !*index_r)
                return (sowr_RadixTreeNode *)current;
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
sowr_RadixTreeNode *
sowr_RadixTree_GetS( const sowr_RadixTree *tree, const sowr_String *index )
{
    return sowr_RadixTree_Get(tree, index->ptr);
}

size_t
sowr_RadixTree_ListAllChildren( const sowr_RadixTreeNode *node, sowr_HashMap *output )
{
    if (!node)
        return 0ULL;

    size_t children = 0ULL;

    for (size_t i = 0ULL; i < CHAR_MAX; i++)
        children += sowr_RadixTree_ListAllChildren(node->characters[i], output);

    if (node->data)
    {
        if (output)
            sowr_HashMap_InsertCV(output, node->full_key.ptr, node->data_size, node->data);
        children++;
    }

    return children;
}

bool
sowr_RadixTree_Delete( sowr_RadixTree *tree, const char *index )
{
    if (!tree)
        return false;

    sowr_RadixTreeNode *iter = &(tree->head);
    size_t ch = (size_t)*index;
    while (ch)
    {
        if (!iter->characters[ch])
            return false;

        else
        {
            sowr_RadixTreeNode *current = iter->characters[ch];
            const char *index_r = index;
            const char *target = current->key.ptr;

            while (*index_r == *target && *index_r && *target)
                index_r++, target++;

            if (!*target && !*index_r)
            {
                if (current->data)
                {
                    current->data_size = 0ULL;
                    sowr_HeapFree(current->data);
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
sowr_RadixTree_DeleteS( sowr_RadixTree *tree, const sowr_String *index )
{
    return sowr_RadixTree_Delete(tree, index->ptr);
}

void
sowr_RadixTree_Destroy( sowr_RadixTree *tree )
{
    sowr_RadixTree_Clear(tree);
    sowr_HeapFree(tree);
}

void
sowr_RadixTree_DestroyS( sowr_RadixTree *tree )
{
    sowr_RadixTree_Clear(tree);
}
