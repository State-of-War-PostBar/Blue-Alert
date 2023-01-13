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

#ifndef BLRT_LIB_CONTAINER_VECTOR_H
#define BLRT_LIB_CONTAINER_VECTOR_H

#include <pch.h>

typedef void (*blrt_VectorFreeFunc)( void * );
typedef void (*blrt_VectorWalkFunc)( void * );
typedef bool (*blrt_VectorCmpFunc)( const void *, const void * );

typedef struct blrt_Vector
{
    size_t elem_size;
    size_t capacity;
    size_t length;
    blrt_VectorFreeFunc free_func;
    void *ptr;
} blrt_Vector;

///
/// \brief Create a vector
///
/// Create a vector.
///
/// \param elem_size Size of vector's elements
/// \param free_func Function to call when the vector frees an element
///
/// \return Created vector
///
blrt_Vector *
blrt_Vector_Create( size_t elem_size, blrt_VectorFreeFunc free_func );

///
/// \brief Create a vector
///
/// Create a vector by stack.<BR />
/// <B>The created vector must be freed by \a blrt_Vector_DestroyS().</B>
///
/// \param elem_size Size of vector's elements
/// \param free_func Function to call when the vector frees an element
///
/// \return Created vector
///
blrt_Vector
blrt_Vector_CreateS( size_t elem_size, blrt_VectorFreeFunc free_func );

///
/// \brief Get the first element
///
/// Get the first element of the vector.
///
/// \param vec Vector
///
/// \return Pointer to the first element
///
void *
blrt_Vector_First( const blrt_Vector *vec );

///
/// \brief Get the last element
///
/// Get the last element of the vector.
///
/// \param vec Vector
///
/// \return Pointer to the last element
///
void *
blrt_Vector_Last( const blrt_Vector *vec );

///
/// \brief Get the indexed element
///
/// Get the pointer to some place in the vector.<BR />
/// If the index is out of bound, it returns the last element of the vector, if any.
///
/// \param vec Vector
/// \param index Index to access
///
/// \return Pointer to the location after index
///
void *
blrt_Vector_PtrAt( const blrt_Vector *vec, size_t index );

///
/// \brief Find an element in the vector
///
/// Search for an element in the given vector.
///
/// \param vec Vector
/// \param data Data to search
/// \param cmp Comparision function
///
/// \return Pointer to the found element, NULL otherwise.
///
void *
blrt_Vector_Find( const blrt_Vector *vec, const void *data, blrt_VectorCmpFunc cmp );

///
/// \brief Expand the vector
///
/// Expand the vector, usually doubling its capacity unless the length is 0.
///
/// \param vec Vector to expand
///
void
blrt_Vector_Expand( blrt_Vector *vec );

///
/// \brief Expand the vector
///
/// Expand the vector until its capacity reaches the set limit.
///
/// \param vec Vector to expand
/// \param size Target size for expanding
///
void
blrt_Vector_ExpandUntil( blrt_Vector *vec, size_t size );

///
/// \brief Walkthrough the vector
///
/// Walk the vector by a function.
///
/// \param vec Vector to walk
/// \param func Function for walking
///
void
blrt_Vector_Walk( blrt_Vector *vec, blrt_VectorWalkFunc func );

///
/// \brief Clear out a vector
///
/// Clear the content of a vector. Its capacity is unchanged.
///
/// \param vec Vector to clear
///
void
blrt_Vector_Clear( blrt_Vector *vec );

///
/// \brief Shrink the vector
///
/// Shrink the vector to just enough to fit its contents.
///
/// \param vec Vector to shrink
///
void
blrt_Vector_ShrinkToFit( blrt_Vector *vec );

///
/// \brief Insert element to vector
///
/// Insert an element to vector.<BR />
/// If index is out of bound, it is understood to push the element to the end of vector.
///
/// \param vec Vector to insert
/// \param index Index to insert
/// \param elem Element to insert
///
void
blrt_Vector_Insert( blrt_Vector *vec, size_t index, const void *elem );

///
/// \brief Replace an element
///
/// Replace an element with a new one, the old one will be overriden.<BR />
/// If index is out of bound, it is understood to push the element to last of vector.
///
/// \param vec Vector to operate
/// \param index Index to replace
/// \param elem Element to replace
///
void
blrt_Vector_Replace( blrt_Vector *vec, size_t index, const void *elem );

///
/// \brief Delete an element
///
/// Delete an element in index.<BR />
/// If index is out of bound, it does nothing.
///
/// \param vec Vector to operate
/// \param index Index to delete
///
void
blrt_Vector_Delete( blrt_Vector *vec, size_t index );

///
/// \brief Take an element
///
/// Take an element out of the vector.<BR />
/// If index is out of bound, it is understood to pop the last element of the vector.
/// If ptr_retrieve is NULL the free function will be called.
///
/// \param vec Vector to operate
/// \param index Index to take out
/// \param ptr_retrieve Pointer to retrieve the result
///
void
blrt_Vector_Take( blrt_Vector *vec, size_t index, void *ptr_retrieve );

///
/// \brief Push an element
///
/// Push an element into the end of vector.
///
/// \param vec Vector to push into
/// \param elem Element to be pushed
///
void
blrt_Vector_Push( blrt_Vector *vec, const void *elem );

///
/// \brief Pop the last element of vector
///
/// Pop the last element of vector into the pointer.<BR />
/// If ptr_retrieve is NULL the free function will be called.
///
/// \param vec Vector to pop out
/// \param ptr_retrieve Pointer to retrieve the result
///
void
blrt_Vector_Pop( blrt_Vector *vec, void *ptr_retrieve );

///
/// \brief Push an element to front
///
/// Push an element into the front of vector.
///
/// \param vec Vector to push into
/// \param elem Element to be pushed
///
void
blrt_Vector_Push_Front( blrt_Vector *vec, const void *elem );

///
/// \brief Pop the first element of vector
///
/// Pop the first element of vector into the pointer.<BR />
/// Whether ptr_retrieve is NULL or not, the first element will be overriden.
///
/// \param vec Vector to pop out
/// \param ptr_retrieve Pointer to retrieve the result
///
void
blrt_Vector_Pop_Front( blrt_Vector *vec, void *ptr_retrieve );

///
/// \brief Destroy a vector
///
/// Destroy a vector, freeing all its elements.
///
/// \param vec Vector to destroy
///
void
blrt_Vector_Destroy( blrt_Vector *vec );

///
/// \brief Destroy a vector
///
/// Destroy a vector created by \a blrt_Vector_CreateS(), freeing all its elements.
///
/// \param vec Vector to destroy
///
void
blrt_Vector_DestroyS( blrt_Vector *vec );

#endif // !BLRT_LIB_CONTAINER_VECTOR_H
