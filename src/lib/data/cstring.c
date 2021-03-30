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

#include "bytes.h"
#include "../type/generic.h"

void
sowr_CString_IToA( sowr_String *output, intmax_t i, unsigned int base )
{
    if (i < 0)
        sowr_String_PushC(output, '-');
    sowr_CString_UToA(output, imaxabs(i), base);
}

void
sowr_CString_UToA( sowr_String *output, uintmax_t u, unsigned int base )
{
    if (!u)
    {
        sowr_String_PushC(output, '0');
        return;
    }

    if (base < 2 || base > 36)
    {
        sowr_String_PushC(output, '?');
        return;
    }

    size_t current_len = output->length, after_len = output->length;
    while (u)
    {
        uintmax_t rem = u % base;
        sowr_String_PushC(output, rem > 9 ? rem - 10 + 'a' : rem + '0');
        u /= base;
        after_len++;
    }

    sowr_SwapEndian(after_len - current_len, (unsigned char *)(output->ptr + current_len));
}

void
sowr_CString_FToA( sowr_String *output, float f, size_t max_significant )
{
    switch (fpclassify(f))
    {
        case FP_INFINITE:
        {
            sowr_String_PushS(output, "INF");
            break;
        }
        case FP_NAN:
        {
            sowr_String_PushS(output, "NAN");
            break;
        }
        case FP_ZERO:
        {
            sowr_String_PushS(output, "0.0");
            break;
        }
        case FP_SUBNORMAL:
        {
            if (signbit(f))
                sowr_String_PushC(output, '-');
            sowr_String_PushS(output, "(~");
            sowr_String_PushS(output, SOWR_STRINGIFY(FLT_MIN));
            sowr_String_PushC(output, ')');
            break;
        }
        case FP_NORMAL:
        {
            if (signbit(f))
            {
                sowr_String_PushC(output, '-');
                f = fabsf(f);
            }

            uintmax_t integer_part = 0U;
            float buf = 0.0f;

            f = modff(f, &buf);
            integer_part = (uintmax_t)buf;
            sowr_CString_UToA(output, integer_part, 10U);
            sowr_String_PushC(output, '.');

            // Comparing with eplisoned floating is nearly meaningless in our program.
            if (!f)
            {
                sowr_String_PushC(output, '0');
                break;
            }

            while (f && max_significant)
            {
                f *= 10.0f;
                f = modff(f, &buf);
                integer_part = (uintmax_t)buf;
                sowr_CString_UToA(output, integer_part, 10U);
                max_significant--;
            }
            break;
        }
        default:
            sowr_String_PushC(output, '?');
    }
}

void
sowr_CString_DToA( sowr_String *output, double d, size_t max_significant )
{
    switch (fpclassify(d))
    {
        case FP_INFINITE:
        {
            sowr_String_PushS(output, "INF");
            break;
        }
        case FP_NAN:
        {
            sowr_String_PushS(output, "NAN");
            break;
        }
        case FP_ZERO:
        {
            sowr_String_PushS(output, "0.00");
            break;
        }
        case FP_SUBNORMAL:
        {
            if (signbit(d))
                sowr_String_PushC(output, '-');
            sowr_String_PushS(output, "(~");
            sowr_String_PushS(output, SOWR_STRINGIFY(DBL_MIN));
            sowr_String_PushC(output, ')');
            break;
        }
        case FP_NORMAL:
        {
            if (signbit(d))
            {
                sowr_String_PushC(output, '-');
                d = fabs(d);
            }

            uintmax_t integer_part = 0U;
            double buf = 0.0f;

            d = modf(d, &buf);
            integer_part = (uintmax_t)buf;
            sowr_CString_UToA(output, integer_part, 10U);
            sowr_String_PushC(output, '.');

            // Comparing with eplisoned floating is nearly meaningless in our program.
            if (!d)
            {
                sowr_String_PushC(output, '0');
                break;
            }

            while (d && max_significant)
            {
                d *= 10.0;
                d = modf(d, &buf);
                integer_part = (uintmax_t)buf;
                sowr_CString_UToA(output, integer_part, 10U);
                max_significant--;
            }
            break;
        }
        default:
            sowr_String_PushC(output, '?');
    }
}

void
sowr_CString_LDToA( sowr_String *output, long double ld, size_t max_significant )
{
    switch (fpclassify(ld))
    {
        case FP_INFINITE:
        {
            sowr_String_PushS(output, "INF");
            break;
        }
        case FP_NAN:
        {
            sowr_String_PushS(output, "NAN");
            break;
        }
        case FP_ZERO:
        {
            sowr_String_PushS(output, "0.000");
        }
        case FP_SUBNORMAL:
        {
            if (signbit(ld))
                sowr_String_PushC(output, '-');
            sowr_String_PushS(output, "(~");
            sowr_String_PushS(output, SOWR_STRINGIFY(LDBL_MIN));
            sowr_String_PushC(output, ')');
            break;
        }
        case FP_NORMAL:
        {
            if (signbit(ld))
            {
                sowr_String_PushC(output, '-');
                ld = fabsl(ld);
            }

            uintmax_t integer_part = 0U;
            long double buf = 0.0f;

            ld = modfl(ld, &buf);
            integer_part = (uintmax_t)buf;
            sowr_CString_UToA(output, integer_part, 10U);
            sowr_String_PushC(output, '.');

            // Comparing with eplisoned floating is nearly meaningless in our program.
            if (!ld)
            {
                sowr_String_PushC(output, '0');
                break;
            }

            while (ld && max_significant)
            {
                ld *= 10.0L;
                ld = modfl(ld, &buf);
                integer_part = (uintmax_t)buf;
                sowr_CString_UToA(output, integer_part, 10U);
                max_significant--;
            }
            break;
        }
        default:
            sowr_String_PushC(output, '?');
    }
}

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
        switch (data.type)
        {
            case SOWR_TYPE_BOOL:
            {
                sowr_String_PushS(output, data.data.reg.as_bool ? "true" : "false");
                break;
            }
            case SOWR_TYPE_CHAR:
            {
                sowr_String_PushC(output, data.data.reg.as_char);
                break;
            }
            case SOWR_TYPE_UCHAR:
            {
                sowr_String_PushC(output, data.data.reg.as_uchar);
                break;
            }

            case SOWR_TYPE_SHORT: case SOWR_TYPE_INT: case SOWR_TYPE_LONG: case SOWR_TYPE_LONGLONG:
            case SOWR_TYPE_INT_8: case SOWR_TYPE_INT_16: case SOWR_TYPE_INT_32: case SOWR_TYPE_INT_64:
            case SOWR_TYPE_INT_LEAST_8: case SOWR_TYPE_INT_LEAST_16: case SOWR_TYPE_INT_LEAST_32: case SOWR_TYPE_INT_LEAST_64:
            case SOWR_TYPE_INT_FAST_8: case SOWR_TYPE_INT_FAST_16: case SOWR_TYPE_INT_FAST_32: case SOWR_TYPE_INT_FAST_64:
            case SOWR_TYPE_INT_MAX:
            {
                sowr_CString_IToA(output, data.data.reg.as_int_max, 10U);
                break;
            }
            case SOWR_TYPE_USHORT: case SOWR_TYPE_UINT: case SOWR_TYPE_ULONG: case SOWR_TYPE_ULONGLONG:
            case SOWR_TYPE_UINT_8: case SOWR_TYPE_UINT_16: case SOWR_TYPE_UINT_32: case SOWR_TYPE_UINT_64:
            case SOWR_TYPE_UINT_LEAST_8: case SOWR_TYPE_UINT_LEAST_16: case SOWR_TYPE_UINT_LEAST_32: case SOWR_TYPE_UINT_LEAST_64:
            case SOWR_TYPE_UINT_FAST_8: case SOWR_TYPE_UINT_FAST_16: case SOWR_TYPE_UINT_FAST_32: case SOWR_TYPE_UINT_FAST_64:
            case SOWR_TYPE_SIZE: case SOWR_TYPE_PTR_DIFF: case SOWR_TYPE_UINT_MAX:
            {
                sowr_CString_UToA(output, data.data.reg.as_uint_max, 10U);
                break;
            }

            case SOWR_TYPE_FLOAT:
            {
                sowr_CString_FToA(output, data.data.flt.as_float, 5ULL);
                break;
            }
            case SOWR_TYPE_DOUBLE:
            {
                sowr_CString_DToA(output, data.data.flt.as_double, 7ULL);
                break;
            }
            case SOWR_TYPE_LDOUBLE:
            {
                sowr_CString_LDToA(output, data.data.flt.as_ldouble, 12ULL);
                break;
            }

            case SOWR_TYPE_FUNC_PTR:
            case SOWR_TYPE_VOID_PTR:
            case SOWR_TYPE_PTR:
            {
                sowr_String_PushS(output, "0x");
                sowr_CString_UToA(output, (sowr_Ptr)(data.data.reg.as_ptr), 16U);
                break;
            }

            case SOWR_TYPE_STRING:
            {
                sowr_String_PushS(output, data.data.reg.as_string);
                break;
            }

            default:
                sowr_String_PushC(output, '?');
        }
    }

    va_end(args_cpy);
}
