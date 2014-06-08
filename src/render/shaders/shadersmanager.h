/*
 *  The ManaPlus Client
 *  Copyright (C) 2014  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RENDER_SHADERSMANAGER_H
#define RENDER_SHADERSMANAGER_H

#ifdef USE_OPENGL

#include <string>

#include "localconsts.h"

class Shader;
class ShaderProgram;

class ShadersManager final
{
    public:
        Shader *createShader(const unsigned int type,
                             const std::string &fileName) A_WARN_UNUSED;

        ShaderProgram *createProgram(const std::string &vertex,
                                     const std::string &fragment)
                                     A_WARN_UNUSED;
};

extern ShadersManager shaders;

#endif  // USE_OPENGL
#endif  // RENDER_SHADERSMANAGER_H
