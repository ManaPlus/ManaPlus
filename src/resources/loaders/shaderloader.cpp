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

#include "render/shaders/shader.h"
#include "render/shaders/shadersmanager.h"

#include "resources/loaders/shaderloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "debug.h"

namespace
{
    struct ShaderLoader final
    {
        A_DEFAULT_COPY(ShaderLoader)

        const std::string name;
        const unsigned int type;

        static Resource *load(const void *const v)
        {
            if (v == nullptr)
                return nullptr;

            const ShaderLoader *const rl
                = static_cast<const ShaderLoader *>(v);
            Shader *const resource = shaders.createShader(rl->type, rl->name);
            if (resource == nullptr)
                reportAlways("Shader creation error: %s", rl->name.c_str());
            return resource;
        }
    };
}  // namespace

Resource *Loader::getShader(const unsigned int type,
                            const std::string &name)
{
    ShaderLoader rl = { name, type };
    return ResourceManager::get("shader_" + name, ShaderLoader::load, &rl);
}

#endif  // USE_OPENGL
