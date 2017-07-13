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

#ifndef PARTICLE_PARTICLEVECTOR_H
#define PARTICLE_PARTICLEVECTOR_H

#include "particle/particlecontainer.h"

#include "utils/vector.h"

#include "localconsts.h"

class Particle;

/**
 * Particle container with indexing facilities
 */
class ParticleVector final : public ParticleContainer
{
    public:
        explicit ParticleVector(ParticleContainer *const parent = nullptr,
                                const bool delParent = true);

        A_DELETE_COPY(ParticleVector)

        ~ParticleVector();

        /**
         * Sets a particle at a specified index.  Kills the previous particle
         * there, if needed.
         */
        void setLocally(const int index, Particle *const particle);

        /**
         * Removes a particle at a specified index
         */
        void delLocally(const int index);

        void clearLocally() override final;

        void moveTo(const float x, const float y) override final;

        size_t size() const
        { return mIndexedElements.size(); }

        size_t usedSize() const;

    protected:
        STD_VECTOR<Particle *> mIndexedElements;
};

#endif  // PARTICLE_PARTICLEVECTOR_H
