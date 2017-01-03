/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef PARTICLE_ANIMATIONPARTICLE_H
#define PARTICLE_ANIMATIONPARTICLE_H

#include "particle/imageparticle.h"

#include "utils/xml.h"

class Animation;

class AnimationParticle final : public ImageParticle
{
    public:
        explicit AnimationParticle(Animation *restrict const animation)
                                   A_NONNULL(2);

        explicit AnimationParticle(XmlNodePtrConst animationNode,
                                   const std::string &restrict dyePalettes
                                   = std::string());

        A_DELETE_COPY(AnimationParticle)
};

#endif  // PARTICLE_ANIMATIONPARTICLE_H
