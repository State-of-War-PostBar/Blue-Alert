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

#include "cstring.h"

#include "../type/generic.h"

void
sowr_CString_Compose( sowr_String *output, size_t count, ... )
{
    va_list args;
    va_start(args, count);
    sowr_CString_ComposeV(output, count, &args);
    va_end(args);
}

void
sowr_CString_ComposeV( sowr_String *output, size_t count, va_list *args )
{
    va_list args_cpy;
    va_copy(args_cpy, *args);

    for (size_t i = 0ULL; i < count; i++)
    {
        sowr_GenericType data = va_arg(args_cpy, sowr_GenericType);
        switch (data.type_name)
        {
            case (SOWR_TYPE_STRING):
            {
                sowr_String_PushS(output, data.data.reg.as_string);
                break;
            }

            // TODO: Other data types

            default:
                sowr_String_PushC(output, '?');
        }
    }

    va_end(args_cpy);
}
