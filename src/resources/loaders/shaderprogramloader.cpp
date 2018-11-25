/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifdef USE_OPENGL

#include "utils/checkutils.h"

#include "render/shaders/shaderprogram.h"
#include "render/shaders/shadersmanager.h"

#include "resources/loaders/shaderprogramloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "debug.h"

namespace
{
    struct ShaderProgramLoader final
    {
        A_DEFAULT_COPY(ShaderProgramLoader)

        const std::string vertex;
        const std::string fragment;
        const bool isNewShader;

        static Resource *load(const void *const v)
        {
            if (v == nullptr)
                return nullptr;

            const ShaderProgramLoader *const rl
                = static_cast<const ShaderProgramLoader *>(v);
            ShaderProgram *const resource = shaders.createProgram(
                rl->vertex,
                rl->fragment,
                rl->isNewShader);
            if (resource == nullptr)
                reportAlways("Shader program creation error")
            return resource;
        }
    };
}  // namespace

Resource *Loader::getShaderProgram(const std::string &vertex,
                                   const std::string &fragment,
                                   const bool isNewShader)
{
    ShaderProgramLoader rl = { vertex, fragment, isNewShader };
    return ResourceManager::get("program_" + vertex + " + " + fragment,
        ShaderProgramLoader::load, &rl);
}

#endif  // USE_OPENGL
