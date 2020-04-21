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

#include "filesystem.h"

#include "../memory/heap_memory.h"

#ifdef SOWR_TARGET_WINDOWS
    #include <io.h>
    #include <direct.h>
#else
    #include <unistd.h>
    #include <sys/dir.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

inline
bool
sowr_PathExists(const char *path)
{
#ifdef SOWR_TARGET_WINDOWS
    return _access(path, 0) == 0;
#else
    return access(path, 0) == 0;
#endif
}

inline
bool
sowr_CreateDirectory(const char *path)
{
#ifdef SOWR_TARGET_WINDOWS
    return _mkdir(path);
#else
    return mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

size_t
sowr_GetFileSize(FILE *file)
{
#ifdef SOWR_TARGET_WINDOWS
    fseeko(file, 0L, SEEK_END);
    size_t size = ftello(file);
    rewind(file);
    return size;
#else
    struct stat64 st;
    fstat64(fileno(file), &st);
    return st.st_size;
#endif
}
