/*************************************************************************************************
*                                                                                                *
*                                         [ Blue Alert ]                                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                              A free, open-source indie RTS game.                               *
*               (ɔ) 2017 - 2022 State of War Baidu Postbar, some rights reserved.                *
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

#ifndef BLRT_LIB_CONTAINER_HASH_MAP_H
#define BLRT_LIB_CONTAINER_HASH_MAP_H

#include <pch.h>

#include "string.h"
#include "vector.h"
#include "../data/hash.h"

typedef void (*blrt_HashMapWalkFunc)( void * );
typedef void (*blrt_HashMapFreeFunc)( void * );

typedef struct blrt_HashMapValue
{
    blrt_Hash index_hash;
    size_t data_size;
    blrt_HashMapFreeFunc free_func;
    void *data;
} blrt_HashMapValue;

typedef struct blrt_HashMap
{
    size_t buckets_count;
    size_t length;
    blrt_Vector buckets;
    blrt_HashMapFreeFunc free_func;
} blrt_HashMap;

///
/// \brief Create a hashmap
///
/// Create a hashmap, with default buckets, ready to use.
///
/// \param free_func Function to call when freeing an element
///
/// \return A hashmap
///
blrt_HashMap *
blrt_HashMap_Create( blrt_HashMapFreeFunc free_func );

///
/// \brief Create a hashmap
///
/// Create a hashmap by stack, with default buckets, ready to use.<BR />
/// <B>The created hashmap must be destroyed with \a blrt_HashMap_DestroyS().</B>
///
/// \param free_func Function to call when freeing an element
///
/// \return A hashmap
///
blrt_HashMap
blrt_HashMap_CreateS( blrt_HashMapFreeFunc free_func );

///
/// \brief Create a hashmap
///
/// Create a hashmap with custom number of buckets, ready to use.
///
/// \param buckets_count Suggested buckets count
/// \param free_func Function to call when freeing an element
///
/// \return A hashmap
///
blrt_HashMap *
blrt_HashMap_Create_SuggestBucketsCount( size_t buckets_count, blrt_HashMapFreeFunc free_func );

///
/// \brief Create a hashmap
///
/// Create a hashmap with custom number of buckets by stack, ready to use.<BR />
/// <B>The created hashmap must be destroyed with \a blrt_HashMap_DestroyS().</B>
///
/// \param buckets_count Suggested buckets count
/// \param free_func Function to call when freeing an element
///
/// \return A hashmap
///
blrt_HashMap
blrt_HashMap_Create_SuggestBucketsCountS( size_t buckets_count, blrt_HashMapFreeFunc free_func );

///
/// \brief Insert an element to hashmap
///
/// Use key and value to insert element to hashmap, override the old value if key is identical.
///
/// \param map Map to insert
/// \param index_length Length of index in byte
/// \param index Index
/// \param val_length Length of value in byte
/// \param value Value to insert
///
void
blrt_HashMap_Insert( blrt_HashMap *map, size_t index_length, const char *index, size_t val_length, const void *value );

///
/// \brief Insert an element to hashmap
///
/// Use key and value to insert element to hashmap, override the old value if key is identical.<BR />
/// The key and value are understood to be null-terminated strings.
///
/// \param map Map to insert
/// \param index Index
/// \param value Value to insert
///
void
blrt_HashMap_InsertCC( blrt_HashMap *map, const char *index, const char *value );

///
/// \brief Insert an element to hashmap
///
/// Use key and value to insert element to hashmap, override the old value if key is identical.<BR />
/// The key is understood to be null-terminated string.
///
/// \param map Map to insert
/// \param index Index
/// \param val_length Length of value in byte
/// \param value Value to insert
///
void
blrt_HashMap_InsertCV( blrt_HashMap *map, const char *index, size_t val_length, const void *value );

///
/// \brief Insert an element to hashmap
///
/// Use key and value to insert element to hashmap, override the old value if key is identical.<BR />
///
/// \param map Map to insert
/// \param index Index
/// \param value Value to insert
///
void
blrt_HashMap_InsertSS( blrt_HashMap *map, const blrt_String *index, const blrt_String *value );

///
/// \brief Insert an element to hashmap
///
/// Use key and value to insert element to hashmap, override the old value if key is identical.<BR />
/// The  value is understood to be null-terminated string.
///
/// \param map Map to insert
/// \param index Index
/// \param val_length Length of value in byte
/// \param value Value to insert
///
void
blrt_HashMap_InsertSV( blrt_HashMap *map, const blrt_String *index, size_t val_length, const void *value );

///
/// \brief Get a value from the map
///
/// Use key to get a value from the map.
///
/// \param map Map to get
/// \param index_length Length of index in byte
/// \param index Index
///
/// \return The value from key, NULL if not found
///
blrt_HashMapValue *
blrt_HashMap_Get( blrt_HashMap *map, size_t index_length, const char *index );

///
/// \brief Get a value from the map
///
/// Use key to get a value from the map.<BR />
/// The key is understood to be null-terminated string.
///
/// \param map Map to get
/// \param index Index
///
/// \return The value from key, NULL if not found
///
blrt_HashMapValue *
blrt_HashMap_GetC( blrt_HashMap *map, const char *index );


///
/// \brief Get a value from the map
///
/// Use string index to get a value from the map.
///
/// \param map Map to get
/// \param index Index
///
/// \return The value from index, NULL if not found
///
blrt_HashMapValue *
blrt_HashMap_GetS( blrt_HashMap *map, const blrt_String *index );

///
/// \brief Walk the map
///
/// Walk the map with given function.
///
/// \param map Map to walk
/// \param func Function to use
///
void
blrt_HashMap_Walk( blrt_HashMap *map, const blrt_HashMapWalkFunc func );

///
/// \brief Delete a value from the map
///
/// Delete a value by index from the map.
///
/// \param map Map to operate
/// \param index_length Length of index in byte
/// \param index Index
///
void
blrt_HashMap_Delete( blrt_HashMap *map, size_t index_length, const char *index );

///
/// \brief Delete a value from the map
///
/// Delete a value by index from the map.<BR />
/// The index is understood to be null-terminated string.
///
/// \param map Map to operate
/// \param index Index
///
void
blrt_HashMap_DeleteC( blrt_HashMap *map, const char *index );

///
/// \brief Delete a value from the map
///
/// Delete a value by index from the map using string as index.
///
/// \param map Map to operate
/// \param index Index
///
void
blrt_HashMap_DeleteS( blrt_HashMap *map, const blrt_String *index );

///
/// \brief Clear the hashmap
///
/// Clear the contents of the hashmap.
///
/// \param map Map to clear
///
void
blrt_HashMap_Clear( blrt_HashMap *map );

///
/// \brief Destroy the hashmap
///
/// Destroy the hashmap.
///
/// \param map Map to destroy
///
void
blrt_HashMap_Destroy( blrt_HashMap *map );

///
/// \brief Destroy the hashmap
///
/// Destroy the hashmap created by \a blrt_HashMap_CreateS() or \a blrt_HashMap_Create_SuggestBucketsCountS().
///
/// \param map Map to destroy
///
void
blrt_HashMap_DestroyS( blrt_HashMap *map );

#endif //!BLRT_LIB_CONTAINER_HASH_MAP_H
