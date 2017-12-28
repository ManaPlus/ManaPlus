/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2017  The ManaPlus Developers
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

#ifndef RENDER_SHADERS_SHADER_H
#define RENDER_SHADERS_SHADER_H

#ifdef USE_OPENGL

#include "resources/resource.h"

class Shader final : public Resource
{
    public:
        explicit Shader(const unsigned int id);

        ~Shader() override final;

        A_DELETE_COPY(Shader)

        unsigned int getShaderId() const noexcept2
        { return mShaderId; }

    protected:
        unsigned int mShaderId;
};

#endif  // USE_OPENGL
#endif  // RENDER_SHADERS_SHADER_H
