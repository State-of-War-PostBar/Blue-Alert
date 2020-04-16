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

#include "hash_map.h"

#include "../memory/heap_memory.h"

static const size_t SOWR_HASH_MAP_DEFAULT_BUCKETS_COUNT = 16ULL;

static
bool
sowr_HashValCmpIndexFunc(const void *left, const void *right)
{
    if (((sowr_HashMapValue *)left)->index_hash == ((sowr_HashMapValue *)right)->index_hash)
        return true;
    return false;
}

static
bool
sowr_HashValCmpHashToDataFunc(const void *left_val, const void *right_hash)
{
    return ((sowr_HashMapValue *)left_val)->index_hash == *(sowr_HashVal *)right_hash;
}

static
void
sowr_HashMapValueFreeFunc(void *data)
{
    sowr_HeapFree(((sowr_HashMapValue *)data)->data);
}

inline
sowr_HashMap *
sowr_HashMap_Create(void)
{
    return sowr_HashMap_Create_SuggestBucketsCount(SOWR_HASH_MAP_DEFAULT_BUCKETS_COUNT);
}

inline
sowr_HashMap
sowr_HashMap_CreateS(void)
{
    return sowr_HashMap_Create_SuggestBucketsCountS(SOWR_HASH_MAP_DEFAULT_BUCKETS_COUNT);
}

sowr_HashMap *
sowr_HashMap_Create_SuggestBucketsCount(size_t buckets_count)
{
    sowr_HashMap *map = sowr_HeapAlloc(sizeof(sowr_HashMap));

    map->buckets = sowr_Vector_CreateS(sizeof(sowr_LinkedList), sowr_LinkedList_DestroyS);
    sowr_Vector_ExpandUntil(&(map->buckets), buckets_count);
    for (size_t i = 0ULL; i < buckets_count; i++)
    {
        sowr_LinkedList slot = sowr_LinkedList_CreateS(sizeof(sowr_HashMapValue), sowr_HashMapValueFreeFunc);
        sowr_Vector_Push(&(map->buckets), &slot);
    }

    map->buckets_count = buckets_count;
    map->length = 0ULL;

    return map;
}

sowr_HashMap
sowr_HashMap_Create_SuggestBucketsCountS(size_t buckets_count)
{
    sowr_HashMap map = {};

    map.buckets = sowr_Vector_CreateS(sizeof(sowr_LinkedList), sowr_LinkedList_DestroyS);
    sowr_Vector_ExpandUntil(&(map.buckets), buckets_count);
    for (size_t i = 0ULL; i < buckets_count; i++)
    {
        sowr_LinkedList slot = sowr_LinkedList_CreateS(sizeof(sowr_HashMapValue), sowr_HashMapValueFreeFunc);
        sowr_Vector_Push(&(map.buckets), &slot);
    }

    map.buckets_count = buckets_count;
    map.length = 0ULL;

    return map;
}

void
sowr_HashMap_Insert(sowr_HashMap *map, size_t index_length, const char *index, size_t val_length, const char *value)
{
    sowr_HashMapValue *block = sowr_HeapAlloc(sizeof(sowr_HashMapValue));
    block->data = sowr_HeapAlloc(sizeof(char) * val_length);
    memcpy(block->data, value, val_length);
    block->value_length = val_length;

    sowr_HashVal index_hash = sowr_GetHash(index_length, index);
    block->index_hash = index_hash;
    sowr_HashVal value_hash = sowr_GetHash(val_length, value);
    block->value_hash = value_hash;

    size_t slot = index_hash % map->buckets_count;
    sowr_LinkedList *bucket = sowr_Vector_PtrAt(&(map->buckets), slot);
    map->length = sowr_LinkedList_Delete(bucket, block, sowr_HashValCmpIndexFunc) ? map->length : map->length + 1ULL;
    sowr_LinkedList_Insert(bucket, block);
}

inline
void
sowr_HashMap_InsertI(sowr_HashMap *map, const char *index, const char *value)
{
    sowr_HashMap_Insert(map, strlen(index) + 1ULL, index, strlen(value) + 1ULL, value);
}

sowr_HashMapValue *
sowr_HashMap_Get(sowr_HashMap *map, size_t index_length, const char *index)
{
    if (!map->length)
        return NULL;

    sowr_HashVal hash = sowr_GetHash(index_length, index);
    size_t slot = hash % map->buckets_count;
    sowr_LinkedList *bucket = sowr_Vector_PtrAt(&(map->buckets), slot);

    switch (bucket->length)
    {
        case 0ULL:
            return NULL;
        case 1ULL:
            return (sowr_HashMapValue *)bucket->head->data;
        default:
            return (sowr_HashMapValue *)sowr_LinkedList_Find(bucket, &hash, sowr_HashValCmpHashToDataFunc)->data;
    }
}

inline
sowr_HashMapValue *
sowr_HashMap_GetI(sowr_HashMap *map, const char *index)
{
    return sowr_HashMap_Get(map, strlen(index) + 1ULL, index);
}

void
sowr_HashMap_Walk(sowr_HashMap *map, const sowr_HashMapWalkFunc func)
{
    if (!map->length)
        return;

    for (size_t i = 0ULL; i < map->buckets_count; i++)
        sowr_LinkedList_Walk(sowr_Vector_PtrAt(&(map->buckets), i), func);
}

sowr_HashMapValue *
sowr_HashMap_Take(sowr_HashMap *map, size_t index_length, const char *index)
{
    if (!map->length)
        return NULL;

    sowr_HashVal hash = sowr_GetHash(index_length, index);
    size_t slot = hash % map->buckets_count;
    sowr_LinkedList *bucket = sowr_Vector_PtrAt(&(map->buckets), slot);

    switch (bucket->length)
    {
        case 0ULL:
            return NULL;
        case 1ULL:
        {
            sowr_HashMapValue *new_val;
            sowr_LinkedList_PopN(bucket, &new_val);
            map->length--;
            return new_val;
        }
        default:
        {
            sowr_HashMapValue *new_val;
            sowr_LinkedList_DeleteN(bucket, &hash, sowr_HashValCmpHashToDataFunc, &new_val);
            map->length--;
            return new_val;
        }
    }
}

sowr_HashMapValue *
sowr_HashMap_TakeI(sowr_HashMap *map, const char *index)
{
    return sowr_HashMap_Take(map, strlen(index) + 1ULL, index);
}

void
sowr_HashMap_Delete(sowr_HashMap *map, size_t index_length, const char *index)
{
    if (!map->length)
        return;

    sowr_HashVal hash = sowr_GetHash(index_length, index);
    size_t slot = hash % map->buckets_count;
    sowr_LinkedList *bucket = sowr_Vector_PtrAt(&(map->buckets), slot);

    switch (bucket->length)
    {
        case 0ULL:
            return;
        case 1ULL:
        {
            sowr_LinkedList_Pop(bucket);
            map->length--;
            break;
        }
        default:
        {
            sowr_LinkedList_Delete(bucket, &hash, sowr_HashValCmpHashToDataFunc);
            map->length--;
        }
    }
}

inline
void
sowr_HashMap_DeleteI(sowr_HashMap *map, const char *index)
{
    sowr_HashMap_Delete(map, strlen(index) + 1ULL, index);
}

inline
void
sowr_HashMap_Clear(sowr_HashMap *map)
{
    sowr_Vector_Walk(&(map->buckets), sowr_LinkedList_Clear);
}

void
sowr_HashMap_Destroy(sowr_HashMap *map)
{
    sowr_Vector_DestroyS(&(map->buckets));
    sowr_HeapFree(map);
}

void
sowr_HashMap_DestroyS(sowr_HashMap *map)
{
    sowr_Vector_DestroyS(&map->buckets);
}
