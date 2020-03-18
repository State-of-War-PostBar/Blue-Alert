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

#include "heap_memory.h"

#include "../common/log.h"

inline
void *
sowr_HeapAlloc(size_t size)
{
    void *ptr = malloc(size);
#ifdef SOWR_BUILD_DEBUG
    if (!ptr)
    {
        SOWR_LOG_FATAL("Failed to allocate %zu bytes of memory, program failing.", size);
        abort();
    }
#endif
    return ptr;
}

inline
void *
sowr_HeapAlignedAlloc(size_t size, size_t alignment)
{
#ifdef SOWR_TARGET_WINDOWS
    void *ptr = _aligned_malloc(size, alignment);
#else
    void *ptr = aligned_alloc(alignment, size);
#endif
#ifdef SOWR_BUILD_DEBUG
    if (!ptr)
    {
        SOWR_LOG_FATAL("Failed to aligned allocating %zu bytes of memory, program failing.", size);
        abort();
    }
#endif
    return ptr;
}

inline
void *
sowr_HeapZeroAlloc(size_t size)
{
    void *ptr = calloc(1, size);
#ifdef SOWR_BUILD_DEBUG
    if (!ptr)
    {
        SOWR_LOG_FATAL("Failed to initialize allocate %zu bytes of memory, program failing.", size);
        abort();
    }
#endif
    return ptr;
}

inline
void
sowr_HeapFree(void *ptr)
{
    free(ptr);
}

inline
void *
sowr_ReAlloc(size_t size, void *ptr)
{
    void *new = realloc(ptr, size);
#ifdef SOWR_BUILD_DEBUG
    if (!new)
    {
        SOWR_LOG_FATAL("Failed to reallocate %zu bytes of memory at %p, program failing.", ptr, size);
        abort();
    }
#endif
    return new;
}
