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

#ifndef SOWR_LIB_TYPE_GENERIC_H
#define SOWR_LIB_TYPE_GENERIC_H

#include <pch.h>

#include "ptr.h"

typedef enum sowr_Type
{
    SOWR_TYPE_BOOL = 1,
    SOWR_TYPE_CHAR,
    SOWR_TYPE_UCHAR,
    SOWR_TYPE_SHORT,
    SOWR_TYPE_USHORT,
    SOWR_TYPE_INT,
    SOWR_TYPE_UINT,
    SOWR_TYPE_LONG,
    SOWR_TYPE_ULONG,
    SOWR_TYPE_LONGLONG,
    SOWR_TYPE_ULONGLONG,
    SOWR_TYPE_FLOAT,
    SOWR_TYPE_DOUBLE,
    SOWR_TYPE_LDOUBLE,
//  SOWR_TYPE_CHAR8,
    SOWR_TYPE_CHAR16,
    SOWR_TYPE_CHAR32,
    SOWR_TYPE_INT_8,
    SOWR_TYPE_UINT_8,
    SOWR_TYPE_INT_16,
    SOWR_TYPE_UINT_16,
    SOWR_TYPE_INT_32,
    SOWR_TYPE_UINT_32,
    SOWR_TYPE_INT_64,
    SOWR_TYPE_UINT_64,
    SOWR_TYPE_INT_LEAST_8,
    SOWR_TYPE_UINT_LEAST_8,
    SOWR_TYPE_INT_LEAST_16,
    SOWR_TYPE_UINT_LEAST_16,
    SOWR_TYPE_INT_LEAST_32,
    SOWR_TYPE_UINT_LEAST_32,
    SOWR_TYPE_INT_LEAST_64,
    SOWR_TYPE_UINT_LEAST_64,
    SOWR_TYPE_INT_FAST_8,
    SOWR_TYPE_UINT_FAST_8,
    SOWR_TYPE_INT_FAST_16,
    SOWR_TYPE_UINT_FAST_16,
    SOWR_TYPE_INT_FAST_32,
    SOWR_TYPE_UINT_FAST_32,
    SOWR_TYPE_INT_FAST_64,
    SOWR_TYPE_UINT_FAST_64,
    SOWR_TYPE_STRING,
    SOWR_TYPE_FUNC_PTR,
    SOWR_TYPE_VOID_PTR,
    SOWR_TYPE_PTR,
    SOWR_TYPE_ARRAY,
    SOWR_TYPE_SIZE,
    SOWR_TYPE_PTR_DIFF,
    SOWR_TYPE_INT_MAX,
    SOWR_TYPE_UINT_MAX,
    SOWR_TYPE_STRUCT,
    SOWR_TYPE_UNION,
    SOWR_TYPE_FUNCTION,
    SOWR_TYPE_VOID,
    SOWR_TYPE_UNKNOWN = 0
} sowr_Type;

extern const char *const SOWR_TYPENAMES[];

typedef union sowr_GenericDataRegular
{
    bool as_bool;
    char as_char;
    unsigned char as_uchar;
    short as_short;
    unsigned short as_ushort;
    int as_int;
    unsigned int as_uint;
    long as_long;
    unsigned long as_ulong;
    long long as_long_long;
    unsigned long long as_ulong_long;
//  char8_t as_char8;
    char16_t as_char16;
    char32_t as_char32;
    int8_t as_int_8;
    uint8_t as_uint_8;
    int16_t as_int_16;
    uint16_t as_uint_16;
    int32_t as_int_32;
    uint32_t as_uint_32;
    int64_t as_int_64;
    uint64_t as_uint_64;
    int_least8_t as_int_least8;
    uint_least8_t as_uint_least8;
    int_least16_t as_int_least16;
    uint_least16_t as_uint_least16;
    int_least32_t as_int_least32;
    uint_least32_t as_uint_least32;
    int_least64_t as_int_least64;
    uint_least64_t as_uint_least64;
    int_fast8_t as_int_fast8;
    uint_fast8_t as_uint_fast8;
    int_fast16_t as_int_fast16;
    uint_fast16_t as_uint_fast16;
    int_fast32_t as_int_fast32;
    uint_fast32_t as_uint_fast32;
    int_fast64_t as_int_fast64;
    uint_fast64_t as_uint_fast64;
    char *as_string;
    void *as_func_ptr;
    void *as_void_ptr;
    sowr_Ptr as_ptr;
    void *as_arr;
    size_t as_size;
    ptrdiff_t as_ptr_diff;
    intmax_t as_int_max;
    uintmax_t as_uint_max;
    sowr_Ptr unknown;
} sowr_GenericDataRegular;

typedef union sowr_GenericDataFloat
{
    float as_float;
    double as_double;
    long double as_ldouble;
} sowr_GenericDataFloat;

typedef union sowr_GenericTypeData
{
    sowr_GenericDataRegular reg;
    sowr_GenericDataFloat flt;
} sowr_GenericTypeData;

typedef struct sowr_GenericType
{
    sowr_Type type_name;
    size_t data_size;
    sowr_GenericTypeData data;
} sowr_GenericType;

#define SOWR_MAKE_GENERIC(var) (sowr_GenericType)                                                  \
                               {                                                                   \
                                    .type_name = _Generic((var),                                   \
                                                          bool: SOWR_TYPE_BOOL,                    \
                                                          char: SOWR_TYPE_CHAR,                    \
                                                          unsigned char: SOWR_TYPE_UCHAR,          \
                                                          short: SOWR_TYPE_SHORT,                  \
                                                          unsigned short: SOWR_TYPE_USHORT,        \
                                                          int: SOWR_TYPE_INT,                      \
                                                          unsigned int: SOWR_TYPE_UINT,            \
                                                          long: SOWR_TYPE_LONG,                    \
                                                          unsigned long: SOWR_TYPE_ULONG,          \
                                                          long long: SOWR_TYPE_LONGLONG,           \
                                                          unsigned long long: SOWR_TYPE_ULONGLONG, \
                                                          char *: SOWR_TYPE_STRING,                \
                                                          const char *: SOWR_TYPE_STRING,          \
                                                          void *: SOWR_TYPE_VOID_PTR,              \
                                                          const void *: SOWR_TYPE_VOID_PTR,        \
                                                          default: SOWR_TYPE_UNKNOWN               \
                                                         ),                                        \
                                    .data_size = sizeof((var)),                                    \
                                    .data = (sowr_GenericTypeData){ .reg = (sowr_GenericDataRegular) { .unknown =  SOWR_TO_WIDER_IF_PTR((var)) } } \
                               }

#define SOWR_MAKE_GENERIC_T(var, T) (sowr_GenericType)                                               \
                                    {                                                                \
                                        .type_name = (T),                                            \
                                        .data_size = sizeof((var)),                                  \
                                        .data = (sowr_GenericTypeData){ .reg = (sowr_GenericDataRegular) { .unknown =  SOWR_TO_WIDER_IF_PTR((var)) } } \
                                    }

#define SOWR_MAKE_GENERIC_F(varf) (sowr_GenericType)                                      \
                                  {                                                       \
                                    .type_name = _Generic((varf),                         \
                                                          float: SOWR_TYPE_FLOAT,         \
                                                          double: SOWR_TYPE_DOUBLE,       \
                                                          long double: SOWR_TYPE_LDOUBLE, \
                                                          default: SOWR_TYPE_UNKNOWN      \
                                                         ),                               \
                                    .data_size = sizeof((varf)),                          \
                                    .data = (sowr_GenericTypeData){ .flt = _Generic((varf),                                                       \
                                                                                    float: (sowr_GenericDataFloat){ .as_float = (varf) },         \
                                                                                    double: (sowr_GenericDataFloat){ .as_double = (varf) },       \
                                                                                    long double: (sowr_GenericDataFloat){ .as_ldouble = (varf) }, \
                                                                                    default: (sowr_GenericDataFloat){ .as_float = (varf) }        \
                                                                                   )                                                              \
                                                                  }                         \
                                  }

#define SOWR_MAKE_GENERIC_A(var, len) (sowr_GenericType)              \
                                      {                               \
                                        .type_name = SOWR_TYPE_ARRAY, \
                                        .data_size = len,             \
                                        .data = (sowr_GenericTypeData){ .reg = (sowr_GenericDataRegular) { .as_arr =  SOWR_TO_WIDER_IF_PTR((var)) } } \
                                      }

#endif // !SOWR_LIB_TYPE_GENERIC_H
