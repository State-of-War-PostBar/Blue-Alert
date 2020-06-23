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

#include "trie.h"

#include "../memory/heap_memory.h"

static
sowr_TrieNode *
sowr_TrieNode_Gen()
{
    sowr_TrieNode *node = sowr_HeapAlloc(sizeof(sowr_TrieNode));
    node->data = NULL;
    node->leaf = false;
    for (size_t i = 0ULL; i < SOWR_TRIE_CHARACTERS; i++)
        node->characters[i] = NULL;

    return node;
}

static
void
sowr_TrieNode_DeleteAfter( sowr_TrieNode *node, const sowr_TrieFreeFunc free_func )
{
    if (!node)
        return;

    for (size_t i = 0ULL; i < SOWR_TRIE_CHARACTERS; i++)
        if (node->characters[i])
            sowr_TrieNode_DeleteAfter(node->characters[i], free_func);

    if (node->data && free_func)
    {
        free_func(node->data);
        sowr_HeapFree(node->data);
    }
    sowr_HeapFree(node);
}

sowr_Trie *
sowr_Trie_Create( size_t elem_size, const sowr_TrieFreeFunc free_func )
{
    sowr_TrieNode head;
    head.data = NULL;
    head.leaf = false;
    for (size_t i = 0ULL; i < SOWR_TRIE_CHARACTERS; i++)
        head.characters[i] = NULL;

    sowr_Trie *trie = sowr_HeapAlloc(sizeof(sowr_Trie));
    trie->elem_size = elem_size;
    trie->free_func = free_func;
    trie->head = head;

    return trie;
}

sowr_Trie
sowr_Trie_CreateS( size_t elem_size, const sowr_TrieFreeFunc free_func )
{
    sowr_TrieNode head;
    head.data = NULL;
    head.leaf = false;
    for (size_t i = 0ULL; i < SOWR_TRIE_CHARACTERS; i++)
        head.characters[i] = NULL;

    sowr_Trie trie =
    {
        .elem_size = elem_size,
        .free_func = free_func,
        .head = head
    };

    return trie;
}

void
sowr_Trie_Clear( sowr_Trie *trie )
{
    if (!trie)
        return;

    for (size_t i = 0ULL; i < SOWR_TRIE_CHARACTERS; i++)
        sowr_TrieNode_DeleteAfter(trie->head.characters[i], trie->free_func);
}

void
sowr_Trie_Insert( sowr_Trie *trie, const char *index, const void *data )
{
    if (!trie)
        return;

    sowr_TrieNode *iter = &(trie->head);
    size_t ch = 0ULL;
    while (*index)
    {
        ch = *index;
        if (!iter->characters[ch])
            iter->characters[ch] = sowr_TrieNode_Gen();

        iter = iter->characters[ch];
        index++;
    }

    if (!iter->data)
    {
        iter->data = sowr_HeapAlloc(trie->elem_size);
        memcpy(iter->data, data, trie->elem_size);
        iter->leaf = true;
    }
    else
    {
        if (trie->free_func)
            trie->free_func(iter->data);
        memcpy(iter->data, data, trie->elem_size);
        iter->leaf = true;
    }
}

sowr_TrieNode *
sowr_Trie_Get( sowr_Trie *trie, const char *index )
{
    if (!trie)
        return NULL;

    sowr_TrieNode *iter = &(trie->head);
    while (*index)
    {
        iter = iter->characters[(size_t)*index];
        if (!iter)
            return NULL;
        index++;
    }

    return iter;
}

bool
sowr_Trie_Delete( sowr_Trie *trie, const char *index )
{
    if (!trie)
        return false;

    sowr_TrieNode *iter = &(trie->head);
    sowr_TrieNode **previous = NULL;
    while (*index)
    {
        previous = &(iter->characters[(size_t)*index]);
        iter = iter->characters[(size_t)*index];
        if (!iter)
            return false;
        index++;
    }

    if (iter->data)
    {
        if (trie->free_func)
            trie->free_func(iter->data);
        sowr_HeapFree(iter->data);
    }
    sowr_HeapFree(iter);
    *previous = NULL;

    return true;
}

void
sowr_Trie_Destroy( sowr_Trie *trie )
{
    sowr_Trie_Clear(trie);
    sowr_HeapFree(trie);
}

void
sowr_Trie_DestroyS( sowr_Trie *trie )
{
    sowr_Trie_Clear(trie);
}
