/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef PARTICLE_PARTICLEINFO_H
#define PARTICLE_PARTICLEINFO_H

#include "particle/particle.h"

#include <string>
#include <vector>

struct ParticleInfo
{
    ParticleInfo() :
        files(),
        particles()
    {
    }

    std::vector<std::string> files;
    std::vector<Particle*> particles;
};

#endif  // PARTICLE_PARTICLEINFO_H
