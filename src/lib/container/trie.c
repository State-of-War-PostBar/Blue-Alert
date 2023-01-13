/*************************************************************************************************
*                                                                                                *
*                                         [ Blue Alert ]                                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                              A free, open-source indie RTS game.                               *
*               (ɔ) 2017 - 2023 State of War Baidu Postbar, some rights reserved.                *
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

#include "trie.h"

#include "../memory/heap_memory.h"

static
inline
blrt_TrieNode *
blrt_TrieNode_Gen( void )
{
    return blrt_HeapZeroAlloc(sizeof(blrt_TrieNode));
}

static
void
blrt_TrieNode_DeleteAfter( blrt_TrieNode *node, blrt_TrieFreeFunc free_func )
{
    if (!node)
        return;

    if (node->children)
        for (size_t i = 0ULL; i < CHAR_MAX; i++)
            if (node->characters[i])
            {
                blrt_TrieNode_DeleteAfter(node->characters[i], free_func);
                node->characters[i] = NULL;
                node->children--;
            }

    if (node->data)
    {
        if (free_func)
            free_func(node->data);
        blrt_HeapFree(node->data);
    }
    blrt_HeapFree(node);
}

blrt_Trie *
blrt_Trie_Create( blrt_TrieFreeFunc free_func )
{
    blrt_TrieNode head;
    head.data_size = 0ULL;
    head.data = NULL;
    head.children = 0ULL;
    for (size_t i = 0ULL; i < CHAR_MAX; i++)
        head.characters[i] = NULL;

    blrt_Trie *trie = blrt_HeapAlloc(sizeof(blrt_Trie));
    trie->free_func = free_func;
    trie->head = head;

    return trie;
}

blrt_Trie
blrt_Trie_CreateS( blrt_TrieFreeFunc free_func )
{
    blrt_TrieNode head;
    head.data_size = 0ULL;
    head.data = NULL;
    head.children = 0ULL;
    for (size_t i = 0ULL; i < CHAR_MAX; i++)
        head.characters[i] = NULL;

    blrt_Trie trie =
    {
        .free_func = free_func,
        .head = head
    };

    return trie;
}

void
blrt_Trie_Clear( blrt_Trie *trie )
{
    if (!trie)
        return;

    if (trie->head.children)
        for (size_t i = 0ULL; i < CHAR_MAX; i++)
            blrt_TrieNode_DeleteAfter(trie->head.characters[i], trie->free_func);
    trie->head.children = 0ULL;
}

void
blrt_Trie_Insert( blrt_Trie *trie, const char *index, size_t data_size, const void *data )
{
    if (!trie)
        return;

    blrt_TrieNode *iter = &(trie->head);
    size_t ch = 0ULL;
    while (*index)
    {
        ch = (size_t)(*index);
        if (!iter->characters[ch])
        {
            iter->characters[ch] = blrt_TrieNode_Gen();
            iter->children++;
        }

        iter = iter->characters[ch];
        index++;
    }

    if (!iter->data)
    {
        iter->data = blrt_HeapAlloc(data_size);
        memcpy(iter->data, data, data_size);
    }
    else
    {
        if (trie->free_func)
            trie->free_func(iter->data);
        memcpy(iter->data, data, data_size);
    }
}

inline
void
blrt_Trie_InsertS( blrt_Trie *trie, const blrt_String *index, size_t data_size, const void *data )
{
    blrt_Trie_Insert(trie, index->ptr, data_size, data);
}

blrt_TrieNode *
blrt_Trie_Get( blrt_Trie *trie, const char *index )
{
    if (!trie)
        return NULL;

    blrt_TrieNode *iter = &(trie->head);
    while (*index)
    {
        iter = iter->characters[(size_t)*index];
        if (!iter)
            return NULL;
        index++;
    }

    return iter;
}

inline
blrt_TrieNode *
blrt_Trie_GetS( blrt_Trie *trie, const blrt_String *index )
{
    return blrt_Trie_Get(trie, index->ptr);
}

bool
blrt_Trie_Delete( blrt_Trie *trie, const char *index )
{
    if (!trie)
        return false;

    blrt_TrieNode *iter = &(trie->head);
    while (*index)
    {
        iter = iter->characters[(size_t)(*index)];
        if (!iter)
            return false;
        index++;
    }

    if (iter->data)
    {
        if (trie->free_func)
            trie->free_func(iter->data);
        blrt_HeapFree(iter->data);
    }
    iter->data = NULL;

    return true;
}

inline
bool
blrt_Trie_DeleteS( blrt_Trie *trie, const blrt_String *index )
{
    return blrt_Trie_Delete(trie, index->ptr);
}

void
blrt_Trie_Destroy( blrt_Trie *trie )
{
    blrt_Trie_Clear(trie);
    blrt_HeapFree(trie);
}

void
blrt_Trie_DestroyS( blrt_Trie *trie )
{
    blrt_Trie_Clear(trie);
}
