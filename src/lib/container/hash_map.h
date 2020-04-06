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

#ifndef SOWR_LIB_CONTAINER_HASH_MAP_H
#define SOWR_LIB_CONTAINER_HASH_MAP_H

#include <pch.h>

#include "vector.h"
#include "linked_list.h"
#include "../hash/hash.h"

SOWR_DEF_VECTOR_OF_TYPE(sowr_Linked_List, sowr_VecLinkedList);

typedef struct
{
    char *data;
    size_t length;
    size_t hash;
} sowr_HashMapValue;

typedef struct
{
    sowr_VecLinkedList *buckets;
    size_t buckets_count;
    size_t length;
} sowr_HashMap;

extern const size_t SOWR_HASH_MAP_DEFAULT_BUCKETS_COUNT;

sowr_HashMap *
sowr_HashMap_Create();

sowr_HashMap *
sowr_HashMap_Create_SuggestBuckets(size_t);

void
sowr_HashMap_Insert_Mov(sowr_HashMap *, size_t, const char *, size_t, char *);

void
sowr_HashMap_Insert_Cpy(sowr_HashMap *, size_t, const char *, size_t, const char *);

sowr_HashMapValue *
sowr_HashMap_Get(sowr_HashMap *, size_t, const char *);

sowr_HashMapValue *
sowr_HashMap_Take(sowr_HashMap *, size_t, const char *);

void
sowr_HashMap_Delete(sowr_HashMap *, size_t, const char *);

void
sowr_HashMap_Clear(sowr_HashMap *);

void
sowr_HashMap_Destroy(sowr_HashMap *);

#endif //!SOWR_LIB_CONTAINER_HASH_MAP_H
