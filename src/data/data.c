/*************************************************************************************************
*                                                                                                *
*                                    State of War: Remastered                                    *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
* A free, open-source software project recreating an old game.                                   *
* (c) 2017 - 2020 State of War Baidu Postbar, all rights reserved.                               *
*                                                                                                *
* State of War: Remastered is a free software. You can freely do whatever you want with it       *
* under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)         *
* published by mhtvsSFrpHdE.                                                                     *
* By the time this line is written, the version of the license document is 1, but you may use    *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
* State of War: Remastered is created intended to be useful, but without any warranty.           *
* For more information, please forward to the license.                                           *
*                                                                                                *
* You should have received a copy of the license along with the source code of this              *
* program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.    *
*                                                                                                *
* Mission is successfully completed.                                                             *
*                                                                                                *
**************************************************************************************************/

#include "data.h"

// Temporary "implementation".
// Will change!
sowr_HashVal
sowr_GetStrHash(const char *str)
{
    sowr_HashVal res = 0;
    size_t len = strlen(str);

    int i = 0;
    for (i = 0; i < len / 4; i++)
        res += (str[i] * str[i] >> 1) * 0x1070601686fULL;
    for (i = len / 4; i < len / 2; i++)
        res += (str[i] * str[i] >> 1) * 0x405236496fULL;
    for (i = len / 2; i < len * 3 / 4; i++)
        res += (str[i] * str[i] >> 1) * 0x1966822847fULL;
    for (i = len * 3 / 4; i < len; i++)
        res += (str[i] * str[i] >> 1) * 0x732464301fULL;

    res = res * 0x666666666ULL / 0233333333ULL;

    return res;
}
