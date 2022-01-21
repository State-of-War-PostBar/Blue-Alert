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

#ifndef BLRT_LIB_SWM_SWM_H
#define BLRT_LIB_SWM_SWM_H

#include <pch.h>

#include "../container/radix_tree.h"

typedef struct blrt_Swm
{
    blrt_RadixTree contents;
} blrt_Swm;

typedef enum blrt_Swm_DataType
{
    BLRT_SWM_FLAG,
    BLRT_SWM_PAIR
} blrt_Swm_DataType;

typedef struct blrt_SwmData
{
    blrt_Swm_DataType data_type;
    blrt_String data;
} blrt_SwmData;

///
/// \brief Load a swm
///
/// Load a swm from source.
///
/// \param str Source of the swm
///
/// \return Loaded swm
///
blrt_Swm
blrt_Swm_Load( const char *str );

///
/// \brief Destroy a swm
///
/// Destroy a swm, releasing resources.
///
/// \param swm Swm to destroy
///
void
blrt_Swm_Destroy( blrt_Swm *swm );

///
/// \brief Check a flag in swm
///
/// Check if a swm contains a flag.
///
/// \param swm Swm to check
/// \param flag Flag to check
///
/// \return If the flag exists
///
bool
blrt_Swm_HasFlag( const blrt_Swm *swm, const char *flag );

///
/// \brief Get a value from swm
///
/// Get a value from swm.
///
/// \param swm Swm to check
/// \param key Key for retrieving
///
/// \return Value if exists, NULL otherwise
///
char *
blrt_Swm_GetValue( const blrt_Swm *swm, const char *key );

#endif // !BLRT_LIB_SWM_SWM_H
