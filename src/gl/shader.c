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

#include "shader.h"

#include "../common/log.h"

#include <glad/glad.h>

sowr_gl_Shader
sowr_gl_ShaderFromSource(sowr_gl_ShaderType type, const char *src)
{
    sowr_gl_Shader id = 0;
    id = glCreateShader(type);
    glShaderSource(id, 1, &src, NULL);

    glCompileShader(id);
    int success = 0;
    char info_log[SOWR_DEFAULT_LOG_BUFFER_SIZE];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(id, SOWR_DEFAULT_LOG_BUFFER_SIZE, NULL, info_log);
        SOWR_LOG_ERROR("Failed to load an OpenGL shader!\n%s", info_log);
    }

    return id;
}

sowr_gl_ShaderProgram
sowr_gl_ProgramFromShaders(size_t count, const sowr_gl_Shader *shaders, bool release)
{
    sowr_gl_ShaderProgram id = 0;
    id = glCreateProgram();
    int i;

    for (i = 0; i < count; i++)
        glAttachShader(id, shaders[i]);

    int success = 0;
    char info_log[SOWR_DEFAULT_LOG_BUFFER_SIZE];

    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, SOWR_DEFAULT_LOG_BUFFER_SIZE, NULL, info_log);
        SOWR_LOG_ERROR("Failed to link an OpenGL shader program!\n%s\n", info_log);
    }

    glValidateProgram(id);
    glGetProgramiv(id, GL_VALIDATE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, SOWR_DEFAULT_LOG_BUFFER_SIZE, NULL, info_log);
        SOWR_LOG_ERROR("Failed to validate an OpenGL shader program!\n%s\n", info_log);
    }

    if (release)
        for (i = 0; i < count; i++)
            glDeleteShader(shaders[i]);

    return id;
}
