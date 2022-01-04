/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (ɔ) 2017 - 2022 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*    State of War: Remastered is a free software. You can freely do whatever you want with it    *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                  published by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.                  *
*                                                                                                *
*  By the time this line is written, the version of the license document is 1, but you may use   *
*                  any later version of the document released by mhtvsSFrpHdE.                   *
*                                                                                                *
*     State of War: Remastered is created, intended to be useful, but without any warranty.      *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*                 You should have received a copy of the license along with the                  *
*                        source code of this program. If not, please see                         *
*              <https://github.com/State-of-War-PostBar/sowr/blob/master/LICENSE>.               *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                        <https://github.com/State-of-War-PostBar/sowr>.                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#ifndef SOWR_LIB_MATH_VEC_H
#define SOWR_LIB_MATH_VEC_H

#include <pch.h>

typedef union sowr_Vec2
{
    struct
    {
        float x, y;
    };
    struct
    {
        float u, v;
    };
    struct
    {
        float w, h;
    };

    float data[2];
} sowr_Vec2;

typedef union sowr_Vec2D
{
    struct
    {
        double x, y;
    };
    struct
    {
        double u, v;
    };
    struct
    {
        double w, h;
    };

    double data[2];
} sowr_Vec2D;

typedef union sowr_Vec2I
{
    struct
    {
        int x, y;
    };
    struct
    {
        int u, v;
    };
    struct
    {
        int w, h;
    };

    int data[2];
} sowr_Vec2I;

typedef union sowr_Vec2U
{
    struct
    {
        unsigned int x, y;
    };
    struct
    {
        unsigned int u, v;
    };
    struct
    {
        unsigned int w, h;
    };

    unsigned int data[2];
} sowr_Vec2U;

typedef union sowr_Vec3
{
    struct
    {
        float x, y, z;
    };
    struct
    {
        float u, v, ignored_0;
    };
    struct
    {
        float r, g, b;
    };

    float data[3];
} sowr_Vec3;

typedef union sowr_Vec3D
{
    struct
    {
        double x, y, z;
    };
    struct
    {
        double u, v, ignored_0;
    };
    struct
    {
        double r, g, b;
    };

    double data[3];
} sowr_Vec3D;

typedef union sowr_Vec3I
{
    struct
    {
        int x, y, z;
    };
    struct
    {
        int u, v, ignored_0;
    };
    struct
    {
        int r, g, b;
    };

    int data[3];
} sowr_Vec3I;

typedef union sowr_Vec3U
{
    struct
    {
        unsigned int x, y, z;
    };
    struct
    {
        unsigned int u, v, ignored_0;
    };
    struct
    {
        unsigned int r, g, b;
    };

    unsigned int data[3];
} sowr_Vec3U;

typedef union sowr_Vec4
{
    struct
    {
        float x, y, z, w;
    };
    struct
    {
        float u, v, s, t;
    };
    struct
    {
        float r, g, b, a;
    };

    float data[4];
} sowr_Vec4;

typedef union sowr_Vec4D
{
    struct
    {
        double x, y, z, w;
    };
    struct
    {
        double u, v, s, t;
    };
    struct
    {
        double r, g, b, a;
    };

    double data[4];
} sowr_Vec4D;

typedef union sowr_Vec4I
{
    struct
    {
        int x, y, z, w;
    };
    struct
    {
        int u, v, s, t;
    };
    struct
    {
        int r, g, b, a;
    };

    int data[4];
} sowr_Vec4I;

typedef union sowr_Vec4U
{
    struct
    {
        unsigned int x, y, z, w;
    };
    struct
    {
        unsigned int u, v, s, t;
    };
    struct
    {
        unsigned int r, g, b, a;
    };

    unsigned int data[4];
} sowr_Vec4U;

#endif // !SOWR_LIB_MATH_VEC_H
