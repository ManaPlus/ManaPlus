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

#ifndef RENDER_SHADER_H
#define RENDER_SHADER_H

#ifdef USE_OPENGL

#include "resources/resource.h"

class Shader final : public Resource
{
    public:
        Shader(const unsigned int id);

        ~Shader();

        A_DELETE_COPY(Shader)

    protected:
        unsigned int mShaderId;
};

#endif  // USE_OPENGL
#endif  // RENDER_SHADER_H
