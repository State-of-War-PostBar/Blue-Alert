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

#ifndef BLRT_LIB_BRM_BRM_H
#define BLRT_LIB_BRM_BRM_H

#include <pch.h>

#include "../container/radix_tree.h"

typedef struct blrt_Brm
{
    blrt_RadixTree contents;
} blrt_Brm;

typedef enum blrt_Brm_DataType
{
    BLRT_BRM_FLAG,
    BLRT_BRM_PAIR
} blrt_Brm_DataType;

typedef struct blrt_BrmData
{
    blrt_Brm_DataType data_type;
    blrt_String data;
} blrt_BrmData;

///
/// \brief Load a brm
///
/// Load a brm from source.
///
/// \param str Source of the brm
///
/// \return Loaded brm
///
blrt_Brm
blrt_Brm_Load( const char *str );

///
/// \brief Destroy a brm
///
/// Destroy a brm, releasing resources.
///
/// \param brm Brm to destroy
///
void
blrt_Brm_Destroy( blrt_Brm *brm );

///
/// \brief Check a flag in brm
///
/// Check if a brm contains a flag.
///
/// \param brm Brm to check
/// \param flag Flag to check
///
/// \return If the flag exists
///
bool
blrt_Brm_HasFlag( const blrt_Brm *brm, const char *flag );

///
/// \brief Get a value from brm
///
/// Get a value from brm.
///
/// \param brm Brm to check
/// \param key Key for retrieving
///
/// \return Value if exists, NULL otherwise
///
char *
blrt_Brm_GetValue( const blrt_Brm *brm, const char *key );

#endif // !BLRT_LIB_BRM_BRM_H
