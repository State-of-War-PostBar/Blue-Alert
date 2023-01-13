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

#include "compose.h"

#include "../data/bytes.h"
#include "../type/generic.h"

void
blrt_StringCompose_IToA( blrt_String *output, intmax_t i, unsigned int base )
{
    if (i < 0)
        blrt_String_PushC(output, '-');
    blrt_StringCompose_UToA(output, imaxabs(i), base);
}

void
blrt_StringCompose_UToA( blrt_String *output, uintmax_t u, unsigned int base )
{
    if (!u)
    {
        blrt_String_PushC(output, '0');
        return;
    }

    if (base < 2 || base > 36)
    {
        blrt_String_PushC(output, '?');
        return;
    }

    size_t current_len = output->length, after_len = output->length;
    while (u)
    {
        uintmax_t rem = u % base;
        blrt_String_PushC(output, rem > 9 ? rem - 10 + 'a' : rem + '0');
        u /= base;
        after_len++;
    }

    blrt_SwapEndian(after_len - current_len, (unsigned char *)(output->ptr + current_len));
}

void
blrt_StringCompose_FToA( blrt_String *output, float f, size_t max_significant )
{
    switch (fpclassify(f))
    {
        case FP_INFINITE:
        {
            blrt_String_PushS(output, "INF");
            break;
        }
        case FP_NAN:
        {
            blrt_String_PushS(output, "NAN");
            break;
        }
        case FP_ZERO:
        {
            blrt_String_PushS(output, "0.0");
            break;
        }
        case FP_SUBNORMAL:
        {
            if (signbit(f))
                blrt_String_PushC(output, '-');
            blrt_String_PushS(output, "(Subnormal ~");
            blrt_String_PushS(output, BLRT_STRINGIFY(FLT_MIN));
            blrt_String_PushC(output, ')');
            break;
        }
        case FP_NORMAL:
        {
            if (signbit(f))
            {
                blrt_String_PushC(output, '-');
                f = fabsf(f);
            }

            uintmax_t integer_part = 0U;
            float buf = 0.0f;

            f = modff(f, &buf);
            integer_part = (uintmax_t)buf;
            blrt_StringCompose_UToA(output, integer_part, 10U);
            blrt_String_PushC(output, '.');

            // Comparing eplisoned floating is meaningless for this program.
            if (!f)
            {
                blrt_String_PushC(output, '0');
                break;
            }

            while (f && max_significant)
            {
                f *= 10.0f;
                f = modff(f, &buf);
                integer_part = (uintmax_t)buf;
                blrt_StringCompose_UToA(output, integer_part, 10U);
                max_significant--;
            }
            break;
        }
        default:
            blrt_String_PushC(output, '?');
    }
}

void
blrt_StringCompose_DToA( blrt_String *output, double d, size_t max_significant )
{
    switch (fpclassify(d))
    {
        case FP_INFINITE:
        {
            blrt_String_PushS(output, "INF");
            break;
        }
        case FP_NAN:
        {
            blrt_String_PushS(output, "NAN");
            break;
        }
        case FP_ZERO:
        {
            blrt_String_PushS(output, "0.00");
            break;
        }
        case FP_SUBNORMAL:
        {
            if (signbit(d))
                blrt_String_PushC(output, '-');
            blrt_String_PushS(output, "(Subnormal ~");
            blrt_String_PushS(output, BLRT_STRINGIFY(DBL_MIN));
            blrt_String_PushC(output, ')');
            break;
        }
        case FP_NORMAL:
        {
            if (signbit(d))
            {
                blrt_String_PushC(output, '-');
                d = fabs(d);
            }

            uintmax_t integer_part = 0U;
            double buf = 0.0f;

            d = modf(d, &buf);
            integer_part = (uintmax_t)buf;
            blrt_StringCompose_UToA(output, integer_part, 10U);
            blrt_String_PushC(output, '.');

            // Comparing with eplisoned floating is nearly meaningless in our program.
            if (!d)
            {
                blrt_String_PushC(output, '0');
                break;
            }

            while (d && max_significant)
            {
                d *= 10.0;
                d = modf(d, &buf);
                integer_part = (uintmax_t)buf;
                blrt_StringCompose_UToA(output, integer_part, 10U);
                max_significant--;
            }
            break;
        }
        default:
            blrt_String_PushC(output, '?');
    }
}

void
blrt_StringCompose_LDToA( blrt_String *output, long double ld, size_t max_significant )
{
    switch (fpclassify(ld))
    {
        case FP_INFINITE:
        {
            blrt_String_PushS(output, "INF");
            break;
        }
        case FP_NAN:
        {
            blrt_String_PushS(output, "NAN");
            break;
        }
        case FP_ZERO:
        {
            blrt_String_PushS(output, "0.000");
            break;
        }
        case FP_SUBNORMAL:
        {
            if (signbit(ld))
                blrt_String_PushC(output, '-');
            blrt_String_PushS(output, "(Subnormal ~");
            blrt_String_PushS(output, BLRT_STRINGIFY(LDBL_MIN));
            blrt_String_PushC(output, ')');
            break;
        }
        case FP_NORMAL:
        {
            if (signbit(ld))
            {
                blrt_String_PushC(output, '-');
                ld = fabsl(ld);
            }

            uintmax_t integer_part = 0U;
            long double buf = 0.0f;

            ld = modfl(ld, &buf);
            integer_part = (uintmax_t)buf;
            blrt_StringCompose_UToA(output, integer_part, 10U);
            blrt_String_PushC(output, '.');

            // Comparing with eplisoned floating is nearly meaningless in our program.
            if (!ld)
            {
                blrt_String_PushC(output, '0');
                break;
            }

            while (ld && max_significant)
            {
                ld *= 10.0L;
                ld = modfl(ld, &buf);
                integer_part = (uintmax_t)buf;
                blrt_StringCompose_UToA(output, integer_part, 10U);
                max_significant--;
            }
            break;
        }
        default:
            blrt_String_PushC(output, '?');
    }
}

void
blrt_StringCompose_Compose( blrt_String *output, size_t count, ... )
{
    va_list args;
    va_start(args, count);
    blrt_StringCompose_ComposeV(output, count, &args);
    va_end(args);
}

void
blrt_StringCompose_ComposeV( blrt_String *output, size_t count, va_list *args )
{
    va_list args_cpy;
    va_copy(args_cpy, *args);

    for (size_t i = 0ULL; i < count; i++)
    {
        blrt_GenericType data = va_arg(args_cpy, blrt_GenericType);
        switch (data.type)
        {
            case BLRT_TYPE_BOOL:
            {
                blrt_String_PushS(output, data.data.reg.as_bool ? "true" : "false");
                break;
            }
            case BLRT_TYPE_CHAR:
            {
                blrt_String_PushC(output, data.data.reg.as_char);
                break;
            }
            case BLRT_TYPE_UCHAR:
            {
                blrt_String_PushC(output, data.data.reg.as_uchar);
                break;
            }

            case BLRT_TYPE_SHORT: case BLRT_TYPE_INT: case BLRT_TYPE_LONG: case BLRT_TYPE_LONGLONG:
            case BLRT_TYPE_INT_8: case BLRT_TYPE_INT_16: case BLRT_TYPE_INT_32: case BLRT_TYPE_INT_64:
            case BLRT_TYPE_INT_LEAST_8: case BLRT_TYPE_INT_LEAST_16: case BLRT_TYPE_INT_LEAST_32: case BLRT_TYPE_INT_LEAST_64:
            case BLRT_TYPE_INT_FAST_8: case BLRT_TYPE_INT_FAST_16: case BLRT_TYPE_INT_FAST_32: case BLRT_TYPE_INT_FAST_64:
            case BLRT_TYPE_INT_MAX:
            {
                blrt_StringCompose_IToA(output, data.data.reg.as_int_max, 10U);
                break;
            }
            case BLRT_TYPE_USHORT: case BLRT_TYPE_UINT: case BLRT_TYPE_ULONG: case BLRT_TYPE_ULONGLONG:
            case BLRT_TYPE_UINT_8: case BLRT_TYPE_UINT_16: case BLRT_TYPE_UINT_32: case BLRT_TYPE_UINT_64:
            case BLRT_TYPE_UINT_LEAST_8: case BLRT_TYPE_UINT_LEAST_16: case BLRT_TYPE_UINT_LEAST_32: case BLRT_TYPE_UINT_LEAST_64:
            case BLRT_TYPE_UINT_FAST_8: case BLRT_TYPE_UINT_FAST_16: case BLRT_TYPE_UINT_FAST_32: case BLRT_TYPE_UINT_FAST_64:
            case BLRT_TYPE_SIZE: case BLRT_TYPE_PTR_DIFF: case BLRT_TYPE_UINT_MAX:
            {
                blrt_StringCompose_UToA(output, data.data.reg.as_uint_max, 10U);
                break;
            }

            case BLRT_TYPE_FLOAT:
            {
                blrt_StringCompose_FToA(output, data.data.flt.as_float, 5ULL);
                break;
            }
            case BLRT_TYPE_DOUBLE:
            {
                blrt_StringCompose_DToA(output, data.data.flt.as_double, 7ULL);
                break;
            }
            case BLRT_TYPE_LDOUBLE:
            {
                blrt_StringCompose_LDToA(output, data.data.flt.as_ldouble, 12ULL);
                break;
            }

            case BLRT_TYPE_FUNC_PTR:
            case BLRT_TYPE_VOID_PTR:
            case BLRT_TYPE_PTR:
            {
                blrt_String_PushS(output, "0x");
                blrt_StringCompose_UToA(output, (blrt_Ptr)(data.data.reg.as_ptr), 16U);
                break;
            }

            case BLRT_TYPE_STRING:
            {
                blrt_String_PushS(output, data.data.reg.as_string);
                break;
            }

            default:
                blrt_String_PushC(output, '?');
        }
    }

    va_end(args_cpy);
}
