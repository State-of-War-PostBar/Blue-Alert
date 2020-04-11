/*************************************************************************************************
*                                                                                                *
*                                    State of War: Remastered                                    *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
* A free, open-source software project recreating an old game.                                   *
* (c) 2017 - 2020 State of War Baidu Postbar, all rights reserved.                               *
*                                                                                                *
* State of War: Remastered is a free software. You can freely do whatever you want with it       *
* under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)         *
* published by mhtvsSFrpHdE.                                                                     *
* By the time this line is written, the version of the license document is 1, but you may use    *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
* State of War: Remastered is created intended to be useful, but without any warranty.           *
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
sowr_HashValCmpDataFunc(const void *left, const void *right)
{
    sowr_HashMapValue *rleft = (sowr_HashMapValue *)left, *rright = (sowr_HashMapValue *)right;
    if (rleft->hash != rright->hash)
        return false;
    if (rleft->length != rright->length)
        return false;
    if (strncmp(rleft->data, rright->data, rleft->length))
        return false;
    return true;
}

static
bool
sowr_HashValCmpHashToDataFunc(const void *left_val, const void *right_hash)
{
    sowr_HashMapValue *rleft = (sowr_HashMapValue *)left_val;
    sowr_HashVal *rright = (sowr_HashVal *)right_hash;
    return rleft->hash == *rright;
}

static
void
sowr_HashMapValueFreeFunc(void *data)
{
    sowr_HeapFree(((sowr_HashMapValue *)data)->data);
}

static
void
sowr_HashMapListFreeFunc(void *data)
{
    sowr_LinkedList_Clear((sowr_Linked_List *)data);
}

static
void
sowr_HashMap_ClearListFunc(void *list)
{
    sowr_LinkedList_Clear((sowr_Linked_List *)list);
}

inline
sowr_HashMap *
sowr_HashMap_Create()
{
    return sowr_HashMap_Create_SuggestBuckets(SOWR_HASH_MAP_DEFAULT_BUCKETS_COUNT);
}

sowr_HashMap *
sowr_HashMap_Create_SuggestBuckets(size_t buckets_count)
{
    sowr_HashMap *map = sowr_HeapAlloc(sizeof(sowr_HashMap));

    sowr_Vector *buckets = sowr_Vector_Create(sizeof(sowr_Linked_List), sowr_HashMapListFreeFunc);
    sowr_Vector_ExpandUntil(buckets, buckets_count);

    for (size_t i = 0; i < buckets_count; i++)
    {
        sowr_Linked_List *slot = sowr_LinkedList_Create(sizeof(sowr_HashMapValue), sowr_HashMapValueFreeFunc);
        sowr_Vector_Push(buckets, slot);
    }

    map->buckets = buckets;
    map->buckets_count = buckets_count;
    map->length = 0;

    return map;
}

void
sowr_HashMap_Insert(sowr_HashMap *map, size_t index_length, const char *index, size_t val_length, const char *value)
{
    sowr_HashMapValue *block = sowr_HeapAlloc(sizeof(sowr_HashMapValue));
    block->data = sowr_HeapAlloc(sizeof(char) * val_length);
    memcpy(block->data, value, val_length);
    block->length = val_length;

    sowr_HashVal hash = sowr_GetHash(index_length, index);
    block->hash = hash;

    size_t slot = hash % map->buckets_count;
    sowr_Linked_List *bucket = sowr_Vector_PtrAt(map->buckets, slot);
    map->length = sowr_LinkedList_Delete(bucket, block, sowr_HashValCmpDataFunc) ? map->length : map->length + 1;
    sowr_LinkedList_Insert(bucket, block);
}

inline
void
sowr_HashMap_InsertS(sowr_HashMap *map, const char *index, const char *value)
{
    sowr_HashMap_Insert(map, strlen(index) + 1, index, strlen(value) + 1, value);
}

sowr_HashMapValue *
sowr_HashMap_Get(sowr_HashMap *map, size_t index_length, const char *index)
{
    sowr_HashVal hash = sowr_GetHash(index_length, index);
    size_t slot = hash % map->buckets_count;
    sowr_Linked_List *bucket = sowr_Vector_PtrAt(map->buckets, slot);

    switch (bucket->length)
    {
        case 0:
            return NULL;
        case 1:
            return (sowr_HashMapValue *)bucket->next->data;
        default:
            return (sowr_HashMapValue *)sowr_LinkedList_Find(bucket, &hash, sowr_HashValCmpHashToDataFunc)->data;
    }
}

inline
sowr_HashMapValue *
sowr_HashMap_GetS(sowr_HashMap *map, const char *index)
{
    return sowr_HashMap_Get(map, strlen(index) + 1, index);
}

void
sowr_HashMap_Walk(sowr_HashMap *map, const sowr_HashMapWalkFunc func)
{
    if (!map->length)
        return;

    for (size_t i = 0; i < map->buckets_count; i++)
        sowr_LinkedList_Walk(sowr_Vector_PtrAt(map->buckets, i), func);
}

sowr_HashMapValue *
sowr_HashMap_Take(sowr_HashMap *map, size_t index_length, const char *index)
{
    sowr_HashVal hash = sowr_GetHash(index_length, index);
    size_t slot = hash % map->buckets_count;
    sowr_Linked_List *bucket = sowr_Vector_PtrAt(map->buckets, slot);

    if (!bucket->length)
        return NULL;

    switch (bucket->length)
    {
        case 1:
        {
            sowr_HashMapValue *new_val;
            sowr_LinkedList_PopNF(bucket, (void **)&new_val);
            map->length--;
            return new_val;
        }
        default:
        {
            sowr_HashMapValue *new_val;
            sowr_LinkedList_DeleteNF(bucket, &hash, sowr_HashValCmpHashToDataFunc, (void **)&new_val);
            map->length--;
            return new_val;
        }
    }
}

sowr_HashMapValue *
sowr_HashMap_TakeS(sowr_HashMap *map, const char *index)
{
    return sowr_HashMap_Take(map, strlen(index) + 1, index);
}

void
sowr_HashMap_Delete(sowr_HashMap *map, size_t index_length, const char *index)
{
    sowr_HashVal hash = sowr_GetHash(index_length, index);
    size_t slot = hash % map->buckets_count;
    sowr_Linked_List *bucket = sowr_Vector_PtrAt(map->buckets, slot);

    if (!bucket->length)
        return;

    switch (bucket->length)
    {
        case 1:
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
sowr_HashMap_DeleteS(sowr_HashMap *map, const char *index)
{
    sowr_HashMap_Delete(map, strlen(index) + 1, index);
}

void
sowr_HashMap_Clear(sowr_HashMap *map)
{
    sowr_Vector_Walk(map->buckets, sowr_HashMap_ClearListFunc);
}

void
sowr_HashMap_Destroy(sowr_HashMap *map)
{
    sowr_Vector_Destroy(map->buckets);
    sowr_HeapFree(map);
}
