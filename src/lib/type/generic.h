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

#ifndef BLRT_LIB_TYPE_GENERIC_H
#define BLRT_LIB_TYPE_GENERIC_H

#include <pch.h>

#ifdef _UINTPTR_T_DEFINED
    typedef uintptr_t blrt_Ptr;
#else
    typedef void *blrt_Ptr;
#endif

typedef enum blrt_Type
{
    BLRT_TYPE_BOOL = 1,
    BLRT_TYPE_CHAR,
    BLRT_TYPE_UCHAR,
    BLRT_TYPE_SHORT,
    BLRT_TYPE_USHORT,
    BLRT_TYPE_INT,
    BLRT_TYPE_UINT,
    BLRT_TYPE_LONG,
    BLRT_TYPE_ULONG,
    BLRT_TYPE_LONGLONG,
    BLRT_TYPE_ULONGLONG,
    BLRT_TYPE_FLOAT,
    BLRT_TYPE_DOUBLE,
    BLRT_TYPE_LDOUBLE,
//  BLRT_TYPE_CHAR8,
    BLRT_TYPE_CHAR16,
    BLRT_TYPE_CHAR32,
    BLRT_TYPE_INT_8,
    BLRT_TYPE_UINT_8,
    BLRT_TYPE_INT_16,
    BLRT_TYPE_UINT_16,
    BLRT_TYPE_INT_32,
    BLRT_TYPE_UINT_32,
    BLRT_TYPE_INT_64,
    BLRT_TYPE_UINT_64,
    BLRT_TYPE_INT_LEAST_8,
    BLRT_TYPE_UINT_LEAST_8,
    BLRT_TYPE_INT_LEAST_16,
    BLRT_TYPE_UINT_LEAST_16,
    BLRT_TYPE_INT_LEAST_32,
    BLRT_TYPE_UINT_LEAST_32,
    BLRT_TYPE_INT_LEAST_64,
    BLRT_TYPE_UINT_LEAST_64,
    BLRT_TYPE_INT_FAST_8,
    BLRT_TYPE_UINT_FAST_8,
    BLRT_TYPE_INT_FAST_16,
    BLRT_TYPE_UINT_FAST_16,
    BLRT_TYPE_INT_FAST_32,
    BLRT_TYPE_UINT_FAST_32,
    BLRT_TYPE_INT_FAST_64,
    BLRT_TYPE_UINT_FAST_64,
    BLRT_TYPE_STRING,
    BLRT_TYPE_FUNC_PTR,
    BLRT_TYPE_VOID_PTR,
    BLRT_TYPE_PTR,
    BLRT_TYPE_ARRAY,
    BLRT_TYPE_SIZE,
    BLRT_TYPE_PTR_DIFF,
    BLRT_TYPE_INT_MAX,
    BLRT_TYPE_UINT_MAX,
    BLRT_TYPE_STRUCT,
    BLRT_TYPE_UNION,
    BLRT_TYPE_FUNCTION,
    BLRT_TYPE_VOID,

    BLRT_TYPE_UNKNOWN = 0
} blrt_Type;

extern const char *const BLRT_TYPENAMES[];
#define BLRT_TYPENAME(T) BLRT_TYPENAMES[(T)]

typedef union blrt_GenericDataRegular
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
    blrt_Ptr as_ptr;
    void *as_arr;
    size_t as_size;
    ptrdiff_t as_ptr_diff;
    intmax_t as_int_max;
    uintmax_t as_uint_max;
    blrt_Ptr unknown;
} blrt_GenericDataRegular;

typedef union blrt_GenericDataFloat
{
    float as_float;
    double as_double;
    long double as_ldouble;
} blrt_GenericDataFloat;

typedef union blrt_GenericTypeData
{
    blrt_GenericDataRegular reg;
    blrt_GenericDataFloat flt;
} blrt_GenericTypeData;

typedef struct blrt_GenericType
{
    blrt_Type type;
    size_t data_size;
    blrt_GenericTypeData data;
} blrt_GenericType;

#define BLRT_MAKE_GENERIC(var) (blrt_GenericType)                                             \
                               {                                                              \
                                    .type = _Generic((var),                                   \
                                                     bool: BLRT_TYPE_BOOL,                    \
                                                     char: BLRT_TYPE_CHAR,                    \
                                                     unsigned char: BLRT_TYPE_UCHAR,          \
                                                     short: BLRT_TYPE_SHORT,                  \
                                                     unsigned short: BLRT_TYPE_USHORT,        \
                                                     int: BLRT_TYPE_INT,                      \
                                                     unsigned int: BLRT_TYPE_UINT,            \
                                                     long: BLRT_TYPE_LONG,                    \
                                                     unsigned long: BLRT_TYPE_ULONG,          \
                                                     long long: BLRT_TYPE_LONGLONG,           \
                                                     unsigned long long: BLRT_TYPE_ULONGLONG, \
                                                     char *: BLRT_TYPE_STRING,                \
                                                     const char *: BLRT_TYPE_STRING,          \
                                                     void *: BLRT_TYPE_VOID_PTR,              \
                                                     const void *: BLRT_TYPE_VOID_PTR,        \
                                                     default: BLRT_TYPE_UNKNOWN               \
                                                    ),                                        \
                                    .data_size = sizeof((var)),                               \
                                    .data = (blrt_GenericTypeData){ .reg = (blrt_GenericDataRegular) { .unknown =  (blrt_Ptr)(var) } } \
                               }

#define BLRT_MAKE_GENERIC_T(var, T) (blrt_GenericType)              \
                                    {                               \
                                        .type = (T),                \
                                        .data_size = sizeof((var)), \
                                        .data = (blrt_GenericTypeData){ .reg = (blrt_GenericDataRegular) { .unknown =  (blrt_Ptr)(var) } } \
                                    }

#define BLRT_MAKE_GENERIC_F(varf) (blrt_GenericType)                                 \
                                  {                                                  \
                                    .type = _Generic((varf),                         \
                                                     float: BLRT_TYPE_FLOAT,         \
                                                     double: BLRT_TYPE_DOUBLE,       \
                                                     long double: BLRT_TYPE_LDOUBLE, \
                                                     default: BLRT_TYPE_UNKNOWN      \
                                                    ),                               \
                                    .data_size = sizeof((varf)),                     \
                                    .data = (blrt_GenericTypeData){ .flt = _Generic((varf),                                                       \
                                                                                    float: (blrt_GenericDataFloat){ .as_float = (varf) },         \
                                                                                    double: (blrt_GenericDataFloat){ .as_double = (varf) },       \
                                                                                    long double: (blrt_GenericDataFloat){ .as_ldouble = (varf) }, \
                                                                                    default: (blrt_GenericDataFloat){ .as_float = (varf) }        \
                                                                                   )                                                              \
                                                                  }                  \
                                  }

#define BLRT_MAKE_GENERIC_P(var) (blrt_GenericType)          \
                                 {                           \
                                    .type = BLRT_TYPE_PTR,   \
                                    .data_size = sizeof(var) \
                                    .data = (blrt_GenericTypeData) { .reg = (blrt_GenericDataRegular) { .as_ptr = (blrt_Ptr)(var) } } \
                                 }

#define BLRT_MAKE_GENERIC_A(var, len) (blrt_GenericType)         \
                                      {                          \
                                        .type = BLRT_TYPE_ARRAY, \
                                        .data_size = len,        \
                                        .data = (blrt_GenericTypeData){ .reg = (blrt_GenericDataRegular) { .as_arr =  (void *)(var) } } \
                                      }

#endif // !BLRT_LIB_TYPE_GENERIC_H
