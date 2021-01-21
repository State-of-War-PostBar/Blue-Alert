/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (c) 2017 - 2021 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*    State of War: Remastered is a free software. You can freely do whatever you want with it    *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                                   published by mhtvsSFrpHdE.                                   *
*                                                                                                *
*  By the time this line is written, the version of the license document is 1, but you may use   *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
*     State of War: Remastered is created, intended to be useful, but without any warranty.      *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*       You should have received a copy of the license along with the source code of this        *
*  program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.   *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                         https://github.com/State-of-War-PostBar/sowr.                          *
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#ifndef SOWR_LIB_SWM_SWM_H
#define SOWR_LIB_SWM_SWM_H

#include <pch.h>

#include "../container/radix_tree.h"

typedef struct sowr_Swm
{
    sowr_RadixTree contents;
} sowr_Swm;

typedef enum sowr_Swm_DataType
{
    SOWR_SWM_FLAG,
    SOWR_SWM_PAIR
} sowr_Swm_DataType;

typedef struct sowr_SwmData
{
    sowr_Swm_DataType data_type;
    sowr_String data;
} sowr_SwmData;

///
/// \brief Load a swm
///
/// Load a swm from source.
///
/// \param str Source of the swm
///
/// \return Loaded swm
///
sowr_Swm
sowr_Swm_Load( const char *str );

///
/// \brief Destroy a swm
///
/// Destroy a swm, releasing resources.
///
/// \param swm Swm to destroy
///
void
sowr_Swm_Destroy( sowr_Swm *swm );

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
sowr_Swm_HasFlag( const sowr_Swm *swm, const char *flag );

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
sowr_Swm_GetValue( const sowr_Swm *swm, const char *key );

#endif // !SOWR_LIB_SWM_SWM_H
