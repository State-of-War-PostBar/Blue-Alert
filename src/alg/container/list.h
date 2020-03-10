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

#ifndef _SOWR_LIST_H_
#define _SOWR_LIST_H_

#include <pch.h>

#include "../../memory/heap_memory.h"

#define SOWR_DEF_LIST_OF_TYPE(type_name)                                                         \
typedef void (*sowr_List_##type_name##_WalkFunc)(type_name *);                                   \
typedef struct sowr_List_##type_name##_t sowr_List_##type_name##_node, *sowr_List_##type_name;   \
struct sowr_List_##type_name##_t                                                                 \
{                                                                                                \
    type_name data;                                                                              \
    sowr_List_##type_name##_node *before, *next;                                                 \
};                                                                                               \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_Init();                                                                  \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_GetFirstInList(const sowr_List_##type_name);                             \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_GetLastInList(const sowr_List_##type_name);                              \
                                                                                                 \
size_t                                                                                           \
sowr_List_##type_name##_GetListSizeAfter(const sowr_List_##type_name);                           \
                                                                                                 \
size_t                                                                                           \
sowr_List_##type_name##_GetListSizeBefore(const sowr_List_##type_name);                          \
                                                                                                 \
bool                                                                                             \
sowr_List_##type_name##_IsEmpty(const sowr_List_##type_name);                                    \
                                                                                                 \
void                                                                                             \
sowr_List_##type_name##_Walk(const sowr_List_##type_name, const sowr_List_##type_name##_WalkFunc);\
                                                                                                 \
void                                                                                             \
sowr_List_##type_name##_ClearThisAndAfter(sowr_List_##type_name);                                \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_Push(const sowr_List_##type_name, const type_name *);                    \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_Pop(sowr_List_##type_name, type_name *);                                 \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_PushFront(const sowr_List_##type_name, const type_name *);               \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_PopFront(sowr_List_##type_name, type_name *);                            \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_DestroyNode(sowr_List_##type_name);                                      \
                                                                                                 \
void                                                                                             \
sowr_List_##type_name##_MovePtrFront(sowr_List_##type_name *, size_t);                           \
                                                                                                 \
void                                                                                             \
sowr_List_##type_name##_MovePtrBack(sowr_List_##type_name *, size_t);                            \
                                                                                                 \
void                                                                                             \
sowr_List_##type_name##_MovePtrToFirst(sowr_List_##type_name *);                                 \
                                                                                                 \
void                                                                                             \
sowr_List_##type_name##_MovePtrToLast(sowr_List_##type_name *);

#define SOWR_DEFINE_LIST_FUNCS_OF_TYPE(type_name)                                                \
inline                                                                                           \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_Init()                                                                   \
{                                                                                                \
    return NULL;                                                                                 \
}                                                                                                \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_GetFirstInList(const sowr_List_##type_name node)                         \
{                                                                                                \
    if (!node)                                                                                   \
    {                                                                                            \
        return NULL;                                                                             \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        sowr_List_##type_name result = node;                                                     \
        for (sowr_List_##type_name iter = node; iter; iter = iter->before)                       \
            result = iter;                                                                       \
        return result;                                                                           \
    }                                                                                            \
}                                                                                                \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_GetLastInList(const sowr_List_##type_name node)                          \
{                                                                                                \
    if (!node)                                                                                   \
    {                                                                                            \
        return NULL;                                                                             \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        sowr_List_##type_name result = node;                                                     \
        for (sowr_List_##type_name iter = node; iter; iter = iter->next)                         \
            result = iter;                                                                       \
        return result;                                                                           \
    }                                                                                            \
}                                                                                                \
                                                                                                 \
size_t                                                                                           \
sowr_List_##type_name##_GetListSizeBefore(const sowr_List_##type_name node)                      \
{                                                                                                \
    if (!node)                                                                                   \
    {                                                                                            \
        return 0;                                                                                \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        size_t result = 0;                                                                       \
        for (sowr_List_##type_name iter = node; iter; iter = iter->before)                       \
            result++;                                                                            \
        return result - 1;                                                                       \
    }                                                                                            \
}                                                                                                \
                                                                                                 \
size_t                                                                                           \
sowr_List_##type_name##_GetListSizeAfter(const sowr_List_##type_name node)                       \
{                                                                                                \
    if (!node)                                                                                   \
    {                                                                                            \
        return 0;                                                                                \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        size_t result = 0;                                                                       \
        for (sowr_List_##type_name iter = node; iter; iter = iter->next)                         \
            result++;                                                                            \
        return result - 1;                                                                       \
    }                                                                                            \
}                                                                                                \
                                                                                                 \
inline                                                                                           \
bool                                                                                             \
sowr_List_##type_name##_IsListEmpty(const sowr_List_##type_name node)                            \
{                                                                                                \
    return node == NULL ? true : false;                                                          \
}                                                                                                \
                                                                                                 \
void                                                                                             \
sowr_List_##type_name##_Walk(const sowr_List_##type_name node, const sowr_List_##type_name##_WalkFunc func)\
{                                                                                                \
    for (sowr_List_##type_name iter = node; iter; iter = iter->next)                             \
        func(&(iter->data));                                                                     \
}                                                                                                \
void                                                                                             \
sowr_List_##type_name##_ClearThisAndAfter(sowr_List_##type_name node)                            \
{                                                                                                \
    if (node)                                                                                    \
    {                                                                                            \
        sowr_List_##type_name iter = node;                                                       \
        sowr_List_##type_name##_MovePtrToLast(&iter);                                            \
        while (iter->before && iter != node)                                                     \
        {                                                                                        \
            iter = iter->before;                                                                 \
            sowr_HeapFree(iter->next);                                                           \
            iter->next = NULL;                                                                   \
        }                                                                                        \
        if (node->before)                                                                        \
        {                                                                                        \
            node->before->next = NULL;                                                           \
        }                                                                                        \
        sowr_HeapFree(node);                                                                     \
    }                                                                                            \
}                                                                                                \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_Push(const sowr_List_##type_name node, const type_name *data)            \
{                                                                                                \
    sowr_List_##type_name iter = node;                                                           \
    sowr_List_##type_name##_MovePtrToLast(&iter);                                                \
    if (!iter)                                                                                   \
    {                                                                                            \
        iter = sowr_HeapAlloc(sizeof(sowr_List_##type_name##_node));                             \
        memcpy(&(iter->data), data, sizeof(type_name));                                          \
        iter->before = NULL;                                                                     \
        iter->next = NULL;                                                                       \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        sowr_List_##type_name before = iter;                                                     \
        iter = sowr_HeapAlloc(sizeof(sowr_List_##type_name##_node));                             \
        memcpy(&(iter->data), data, sizeof(type_name));                                          \
        iter->before = before;                                                                   \
        iter->next = NULL;                                                                       \
        before->next = iter;                                                                     \
    }                                                                                            \
    return iter;                                                                                 \
}                                                                                                \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_Pop(sowr_List_##type_name node, type_name *recv)                         \
{                                                                                                \
    sowr_List_##type_name iter = node;                                                           \
    sowr_List_##type_name##_MovePtrToLast(&iter);                                                \
    if (!iter)                                                                                   \
    {                                                                                            \
        recv = NULL;                                                                             \
        return NULL;                                                                             \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        sowr_List_##type_name before = iter->before;                                             \
        if (before)                                                                              \
        {                                                                                        \
            before->next = NULL;                                                                 \
        }                                                                                        \
        memcpy(recv, &(iter->data), sizeof(type_name));                                          \
        sowr_HeapFree(iter);                                                                     \
        return before;                                                                           \
    }                                                                                            \
}                                                                                                \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_PushFront(const sowr_List_##type_name node, const type_name *data)       \
{                                                                                                \
    sowr_List_##type_name iter = node;                                                           \
    sowr_List_##type_name##_MovePtrToFirst(&iter);                                               \
    if (!iter)                                                                                   \
    {                                                                                            \
        iter = sowr_HeapAlloc(sizeof(sowr_List_##type_name##_node));                             \
        memcpy(&(iter->data), data, sizeof(type_name));                                          \
        iter->before = NULL;                                                                     \
        iter->next = NULL;                                                                       \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        sowr_List_##type_name next = iter;                                                       \
        iter = sowr_HeapAlloc(sizeof(sowr_List_##type_name##_node));                             \
        memcpy(&(iter->data), data, sizeof(type_name));                                          \
        iter->before = NULL;                                                                     \
        iter->next = next;                                                                       \
        next->before = iter;                                                                     \
    }                                                                                            \
    return iter;                                                                                 \
}                                                                                                \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_PopFront(sowr_List_##type_name node, type_name *recv)                    \
{                                                                                                \
    sowr_List_##type_name iter = node;                                                           \
    sowr_List_##type_name##_MovePtrToFirst(&iter);                                               \
    if (!iter)                                                                                   \
    {                                                                                            \
        recv = NULL;                                                                             \
        return NULL;                                                                             \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        sowr_List_##type_name next = iter->next;                                                 \
        if (next)                                                                                \
        {                                                                                        \
            next->before = NULL;                                                                 \
        }                                                                                        \
        memcpy(recv, &(iter->data), sizeof(type_name));                                          \
        sowr_HeapFree(iter);                                                                     \
        return next;                                                                             \
    }                                                                                            \
}                                                                                                \
                                                                                                 \
sowr_List_##type_name                                                                            \
sowr_List_##type_name##_DestroyNode(sowr_List_##type_name node)                                  \
{                                                                                                \
    sowr_List_##type_name ret = NULL;                                                            \
    if (node)                                                                                    \
    {                                                                                            \
        if (node->before && node->next)                                                          \
        {                                                                                        \
            ret = node->before;                                                                  \
            node->before->next = node->next;                                                     \
            node->next->before = node->before;                                                   \
        }                                                                                        \
        else if (node->before)                                                                   \
        {                                                                                        \
            ret = node->before;                                                                  \
            node->before->next = NULL;                                                           \
        }                                                                                        \
        else if (node->next)                                                                     \
        {                                                                                        \
            ret = node->next;                                                                    \
            node->next->before = NULL;                                                           \
        }                                                                                        \
        sowr_HeapFree(node);                                                                     \
    }                                                                                            \
    return ret;                                                                                  \
}                                                                                                \
                                                                                                 \
void                                                                                             \
sowr_List_##type_name##_MovePtrFront(sowr_List_##type_name *node, size_t magnitude)              \
{                                                                                                \
    size_t count = 0;                                                                            \
    sowr_List_##type_name iter = *node;                                                          \
    while (iter && count < magnitude)                                                            \
    {                                                                                            \
        *node = iter;                                                                            \
        count++;                                                                                 \
        iter = iter->before;                                                                     \
    }                                                                                            \
}                                                                                                \
                                                                                                 \
void                                                                                             \
sowr_List_##type_name##_MovePtrBack(sowr_List_##type_name *node, size_t magnitude)               \
{                                                                                                \
    size_t count = 0;                                                                            \
    sowr_List_##type_name iter = *node;                                                          \
    while (iter && count < magnitude)                                                            \
    {                                                                                            \
        *node = iter;                                                                            \
        count++;                                                                                 \
        iter = iter->next;                                                                       \
    }                                                                                            \
}                                                                                                \
                                                                                                 \
void                                                                                             \
sowr_List_##type_name##_MovePtrToFirst(sowr_List_##type_name *node)                              \
{                                                                                                \
    for (sowr_List_##type_name iter = *node; iter; iter = iter->before)                          \
        *node = iter;                                                                            \
}                                                                                                \
                                                                                                 \
void                                                                                             \
sowr_List_##type_name##_MovePtrToLast(sowr_List_##type_name *node)                               \
{                                                                                                \
    for (sowr_List_##type_name iter = *node; iter; iter = iter->next)                            \
        *node = iter;                                                                            \
}

#endif
