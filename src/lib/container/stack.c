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

#include "stack.h"

inline
sowr_Stack *
sowr_Stack_Create ( size_t elem_size, sowr_StackFreeFunc free_func )
{
    return sowr_Vector_Create(elem_size, free_func);
}

inline
sowr_Stack
sowr_Stack_CreateS ( size_t elem_size, sowr_StackFreeFunc free_func )
{
    return sowr_Vector_CreateS(elem_size, free_func);
}

inline
void *
sowr_Stack_Last( const sowr_Stack *stack )
{
    return sowr_Vector_Last(stack);
}

inline
void
sowr_Stack_Expand( sowr_Stack *stack )
{
    sowr_Vector_Expand(stack);
}

inline
void
sowr_Stack_ExpandUntil( sowr_Stack *stack, size_t size )
{
    sowr_Vector_ExpandUntil(stack, size);
}

void
sowr_Stack_Eat( sowr_Stack *stack, sowr_StackEatFunc func )
{
    if (!stack->length)
        return;

    for (size_t i = stack->length - 1ULL; i > 0ULL; i--)
        func(sowr_Vector_PtrAt(stack, i));
    func(sowr_Vector_First(stack));
    sowr_Stack_Clear(stack);
}

inline
void
sowr_Stack_Clear( sowr_Stack *stack )
{
    sowr_Vector_Clear(stack);
}

inline
void
sowr_Stack_ShrinkToFit( sowr_Stack *stack )
{
    sowr_Vector_ShrinkToFit(stack);
}

inline
void
sowr_Stack_Push( sowr_Stack *stack, const void *elem )
{
    sowr_Vector_Push(stack, elem);
}

inline
void
sowr_Stack_Pop( sowr_Stack *stack, void *ptr_retrieve )
{
    sowr_Vector_Pop(stack, ptr_retrieve);
}

inline
void
sowr_Stack_Destroy( sowr_Stack *stack )
{
    sowr_Vector_Destroy(stack);
}

inline
void
sowr_Stack_DestroyS( sowr_Stack *stack )
{
    sowr_Vector_DestroyS(stack);
}
