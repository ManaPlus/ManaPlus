/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef PARTICLE_PARTICLELIST_H
#define PARTICLE_PARTICLELIST_H

#include "particle/particlecontainer.h"

#include <list>

#include "localconsts.h"

class Particle;

/**
 * Linked list of particle effects.
 */
class ParticleList final : public ParticleContainer
{
    public:
        explicit ParticleList(ParticleContainer *const parent = nullptr,
                              const bool delParent = true);

        A_DELETE_COPY(ParticleList)

        ~ParticleList();

        /**
         * Takes control of and adds a particle
         */
        void addLocally(Particle *const particle);

        /**
         * `kills' and removes a particle
         */
        void removeLocally(const Particle *const particle);

        void clearLocally() override final;

        void moveTo(const float x, const float y) override final;

        size_t size() const
        { return mSize; }

    protected:
        std::list<Particle *> mElements;    /**< Contained particle effects */
        size_t mSize;
};

#endif  // PARTICLE_PARTICLELIST_H
