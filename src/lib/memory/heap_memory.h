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

#ifndef BLRT_LIB_MEMORY_HEAP_MEMORY_H
#define BLRT_LIB_MEMORY_HEAP_MEMORY_H

#include <pch.h>

///
/// \brief Allocate memory in the heap
///
/// Allocate memory in the heap.<BR />
/// If the allocation fails, it terminates the program.
///
/// \param size Size in bytes of the allocation
///
/// \return Pointer to the allocated memory
///
void *
blrt_HeapAlloc( size_t size );

///
/// \brief Allocate memory in the heap with alignment
///
/// Allocate memory in the heap with alignment. See documents for \a aligned_alloc().<BR />
/// If the allocation fails, it terminates the program.
///
/// \param alignment Size in bytes of the alignment
/// \param size Size in bytes of the allocation
///
/// \return Pointer to the allocated memory
///
void *
blrt_HeapAlignedAlloc( size_t alignment, size_t size );

///
/// \brief Allocate memory in the heap with zeroing out
///
/// Allocate memory in the heap, zeroing out all the contents.<BR />
/// If the allocation fails, it terminates the program.
///
/// \param size Size in bytes of the allocation
///
/// \return Pointer to the allocated memory
///
void *
blrt_HeapZeroAlloc( size_t size );

///
/// \brief Free the allocated memory
///
/// Free the allocated memory.
///
/// \param ptr Pointer to the freeing memory
///
void
blrt_HeapFree( void *ptr );

///
/// \brief Reallocate the allocated memory with a new size.
///
/// Reallocate the allocated memory with a new size.<BR />
/// If the allocation fails, it terminates the program.
///
/// \note Keep in mind that the old pointer will not be safe to use anymore.
///
/// \param size Size in bytes of the allocation
/// \param ptr Pointer to the reallocating memory
///
/// \return Pointer to the allocated memory
///
void *
blrt_ReAlloc( size_t size, void *ptr );

#endif // !BLRT_LIB_MEMORY_HEAP_MEMORY_H
