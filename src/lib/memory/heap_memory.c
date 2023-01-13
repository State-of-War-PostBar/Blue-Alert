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

#include "heap_memory.h"

void *
blrt_HeapAlloc( size_t size )
{
    void *ptr = malloc(size);
    if (!ptr)
    {
        perror("Failed to allocate memory");
        abort();
    }
    return ptr;
}

void *
blrt_HeapAlignedAlloc( size_t alignment, size_t size )
{
#ifdef BLRT_TARGET_WINDOWS
    void *ptr = _aligned_malloc(size, alignment);
#else
    void *ptr = aligned_alloc(alignment, size);
#endif
    if (!ptr)
    {
        perror("Failed to allocate aligned memory");
        abort();
    }
    return ptr;
}

void *
blrt_HeapZeroAlloc( size_t size )
{
    void *ptr = calloc(1ULL, size);
    if (!ptr)
    {
        perror("Failed to allocate memory with zeroing out");
        abort();
    }
    return ptr;
}

void
blrt_HeapFree( void *ptr )
{
    free(ptr);
}

void *
blrt_ReAlloc( size_t size, void *ptr )
{
    void *new_memory = realloc(ptr, size);
    if (!new_memory)
    {
        perror("Failed to reallocate memory");
        abort();
    }
    return new_memory;
}
