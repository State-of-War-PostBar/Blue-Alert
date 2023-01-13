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

#include "hash_map.h"

#include "binary_tree.h"
#include "../memory/heap_memory.h"

static const size_t BLRT_HASH_MAP_DEFAULT_BUCKETS_COUNT = 16ULL;

static
int
blrt_CompareIndexHash( const void *left, const void *right )
{
    blrt_HashMapValue *l = (blrt_HashMapValue *)left, *r = (blrt_HashMapValue *)right;
    if (l->index_hash < r->index_hash)
        return 1;
    if (l->index_hash > r->index_hash)
        return -1;
    return 0;
}

static
int
blrt_CompareIndexHashToHash( const void *left_val, const void *right_hash )
{
    const blrt_HashMapValue *l = (const blrt_HashMapValue *)left_val;
    const blrt_Hash *r = (const blrt_Hash *)right_hash;
    if (l->index_hash < *r)
        return 1;
    if (l->index_hash > *r)
        return -1;
    return 0;
}

static
void
blrt_FreeHashMapValue( void *data )
{
    blrt_HashMapValue *value = data;
    if (value->free_func)
        value->free_func(value->data);
    blrt_HeapFree(value->data);
}

inline
blrt_HashMap *
blrt_HashMap_Create( blrt_HashMapFreeFunc free_func )
{
    return blrt_HashMap_Create_SuggestBucketsCount(BLRT_HASH_MAP_DEFAULT_BUCKETS_COUNT, free_func);
}

inline
blrt_HashMap
blrt_HashMap_CreateS( blrt_HashMapFreeFunc free_func )
{
    return blrt_HashMap_Create_SuggestBucketsCountS(BLRT_HASH_MAP_DEFAULT_BUCKETS_COUNT, free_func);
}

blrt_HashMap *
blrt_HashMap_Create_SuggestBucketsCount( size_t buckets_count, blrt_HashMapFreeFunc free_func )
{
    blrt_HashMap *map = blrt_HeapAlloc(sizeof(blrt_HashMap));

    map->buckets = blrt_Vector_CreateS(sizeof(blrt_BinaryTree), (blrt_VectorFreeFunc)blrt_BinaryTree_DestroyS);
    blrt_Vector_ExpandUntil(&(map->buckets), buckets_count);
    for (size_t i = 0ULL; i < buckets_count; i++)
    {
        blrt_BinaryTree tree = blrt_BinaryTree_CreateS(blrt_FreeHashMapValue);
        blrt_Vector_Push(&(map->buckets), &tree);
    }

    map->buckets_count = buckets_count;
    map->length = 0ULL;
    map->free_func = free_func;

    return map;
}

blrt_HashMap
blrt_HashMap_Create_SuggestBucketsCountS( size_t buckets_count, blrt_HashMapFreeFunc free_func )
{
    blrt_HashMap map;

    map.buckets = blrt_Vector_CreateS(sizeof(blrt_BinaryTree), (blrt_VectorFreeFunc)blrt_BinaryTree_DestroyS);
    blrt_Vector_ExpandUntil(&(map.buckets), buckets_count);
    for (size_t i = 0ULL; i < buckets_count; i++)
    {
        blrt_BinaryTree tree = blrt_BinaryTree_CreateS(blrt_FreeHashMapValue);
        blrt_Vector_Push(&(map.buckets), &tree);
    }

    map.buckets_count = buckets_count;
    map.length = 0ULL;
    map.free_func = free_func;

    return map;
}

void
blrt_HashMap_Insert( blrt_HashMap *map, size_t index_length, const char *index, size_t val_length, const void *value )
{
    blrt_HashMapValue block;
    block.data = blrt_HeapAlloc(val_length);
    block.data_size = val_length;
    memcpy(block.data, value, val_length);
    block.free_func = map->free_func;

    blrt_Hash index_hash = blrt_GetHash(index_length, (unsigned char *)index);
    block.index_hash = index_hash;

    size_t slot = index_hash % map->buckets_count;
    blrt_BinaryTree *bucket = blrt_Vector_PtrAt(&(map->buckets), slot);
    map->length = blrt_BinaryTree_Delete(bucket, &block, blrt_CompareIndexHash) ? map->length : map->length + 1ULL;
    blrt_BinaryTree_Insert(bucket, sizeof(blrt_HashMapValue), &block, blrt_CompareIndexHash);
}

inline
void
blrt_HashMap_InsertCC( blrt_HashMap *map, const char *index, const char *value )
{
    blrt_HashMap_Insert(map, strlen(index), index, strlen(value) + 1ULL, value);
}

inline
void
blrt_HashMap_InsertCV( blrt_HashMap *map, const char *index, size_t val_length, const void *value )
{
    blrt_HashMap_Insert(map, strlen(index), index, val_length, value);
}

inline
void
blrt_HashMap_InsertSS( blrt_HashMap *map, const blrt_String *index, const blrt_String *value )
{
    blrt_HashMap_Insert(map, index->length, index->ptr, value->length + 1ULL, value->ptr);
}

inline
void
blrt_HashMap_InsertSV( blrt_HashMap *map, const blrt_String *index, size_t val_length, const void *value )
{
    blrt_HashMap_Insert(map, index->length, index->ptr, val_length, value);
}

blrt_HashMapValue *
blrt_HashMap_Get( blrt_HashMap *map, size_t index_length, const char *index )
{
    if (!map->length)
        return NULL;

    blrt_Hash hash = blrt_GetHash(index_length, (unsigned char *)index);
    size_t slot = hash % map->buckets_count;
    blrt_BinaryTree *bucket = blrt_Vector_PtrAt(&(map->buckets), slot);

    switch (bucket->length)
    {
        case 0ULL:
            return NULL;
        default:
        {
            blrt_BinaryTreeNode *result = blrt_BinaryTree_Find(bucket, &hash, blrt_CompareIndexHashToHash);
            return result ? (blrt_HashMapValue *)(result->data) : NULL;
        }
    }
}

inline
blrt_HashMapValue *
blrt_HashMap_GetC( blrt_HashMap *map, const char *index )
{
    return blrt_HashMap_Get(map, sizeof(index), index);
}

inline
blrt_HashMapValue *
blrt_HashMap_GetS( blrt_HashMap *map, const blrt_String *index )
{
    return blrt_HashMap_Get(map, index->length, index->ptr);
}

void
blrt_HashMap_Walk( blrt_HashMap *map, const blrt_HashMapWalkFunc func )
{
    if (!map->length)
        return;

    for (size_t i = 0ULL; i < map->buckets_count; i++)
        blrt_BinaryTree_Walk(blrt_Vector_PtrAt(&(map->buckets), i), func);
}

void
blrt_HashMap_Delete( blrt_HashMap *map, size_t index_length, const char *index )
{
    if (!map->length)
        return;

    blrt_Hash hash = blrt_GetHash(index_length, (unsigned char *)index);
    size_t slot = hash % map->buckets_count;
    blrt_BinaryTree *tree = blrt_Vector_PtrAt(&(map->buckets), slot);

    switch (tree->length)
    {
        case 0ULL:
            return;
        default:
            if (blrt_BinaryTree_Delete(tree, &hash, blrt_CompareIndexHashToHash))
                map->length--;
    }
}

inline
void
blrt_HashMap_DeleteC( blrt_HashMap *map, const char *index )
{
    blrt_HashMap_Delete(map, strlen(index), index);
}

inline
void
blrt_HashMap_DeleteS( blrt_HashMap *map, const blrt_String *index )
{
    blrt_HashMap_Delete(map, index->length, index->ptr);
}

inline
void
blrt_HashMap_Clear( blrt_HashMap *map )
{
    blrt_Vector_Walk(&(map->buckets), (blrt_VectorWalkFunc)blrt_BinaryTree_Clear);
}

void
blrt_HashMap_Destroy( blrt_HashMap *map )
{
    blrt_Vector_DestroyS(&(map->buckets));
    blrt_HeapFree(map);
}

void
blrt_HashMap_DestroyS( blrt_HashMap *map )
{
    blrt_Vector_DestroyS(&(map->buckets));
}
