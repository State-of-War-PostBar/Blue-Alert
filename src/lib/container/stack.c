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

#include "stack.h"

inline
blrt_Stack *
blrt_Stack_Create ( size_t elem_size, blrt_StackFreeFunc free_func )
{
    return blrt_Vector_Create(elem_size, free_func);
}

inline
blrt_Stack
blrt_Stack_CreateS ( size_t elem_size, blrt_StackFreeFunc free_func )
{
    return blrt_Vector_CreateS(elem_size, free_func);
}

inline
void *
blrt_Stack_Last( const blrt_Stack *stack )
{
    return blrt_Vector_Last(stack);
}

inline
void *
blrt_Stack_Find( const blrt_Stack *stack, const void *data, blrt_StackCmpFunc cmp )
{
    return blrt_Vector_Find(stack, data, cmp);
}

inline
void
blrt_Stack_Expand( blrt_Stack *stack )
{
    blrt_Vector_Expand(stack);
}

inline
void
blrt_Stack_ExpandUntil( blrt_Stack *stack, size_t size )
{
    blrt_Vector_ExpandUntil(stack, size);
}

void
blrt_Stack_Eat( blrt_Stack *stack, blrt_StackEatFunc func )
{
    if (!stack->length)
        return;

    for (size_t i = stack->length - 1ULL; i > 0ULL; i--)
        func(blrt_Vector_PtrAt(stack, i));
    func(blrt_Vector_First(stack));
    blrt_Stack_Clear(stack);
}

inline
void
blrt_Stack_Clear( blrt_Stack *stack )
{
    blrt_Vector_Clear(stack);
}

inline
void
blrt_Stack_ShrinkToFit( blrt_Stack *stack )
{
    blrt_Vector_ShrinkToFit(stack);
}

inline
void
blrt_Stack_Push( blrt_Stack *stack, const void *elem )
{
    blrt_Vector_Push(stack, elem);
}

inline
void
blrt_Stack_Pop( blrt_Stack *stack, void *ptr_retrieve )
{
    blrt_Vector_Pop(stack, ptr_retrieve);
}

inline
void
blrt_Stack_Destroy( blrt_Stack *stack )
{
    blrt_Vector_Destroy(stack);
}

inline
void
blrt_Stack_DestroyS( blrt_Stack *stack )
{
    blrt_Vector_DestroyS(stack);
}
