/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef PARTICLECONTAINER_H
#define PARTICLECONTAINER_H

#include <list>
#include <vector>

#include "localconsts.h"

class Particle;

/**
 * Set of particle effects.  May be stacked with other ParticleContainers.  All
 * operations herein affect such stacked containers, unless the operations end
 * in `Locally'.
 */
class ParticleContainer
{
public:
    /**
     * Constructs a new particle container and assumes responsibility for
     * its parent (for all operations defined herein, except when ending in `Locally')
     *
     * delParent means that the destructor should also free the parent.
     */
    explicit ParticleContainer(ParticleContainer *const parent = nullptr,
                               const bool delParent = true);

    A_DELETE_COPY(ParticleContainer)

    virtual ~ParticleContainer();

    /**
     * Kills and removes all particle effects
     */
    void clear();

    /**
     * Kills and removes all particle effects (only in this container)
     */
    virtual void clearLocally()
    { }

    /**
     * Sets the positions of all elements
     */
    virtual void moveTo(const float x, const float y);

protected:
    ParticleContainer *mNext;           /**< Contained container, if any */
    bool mDelParent;                    /**< Delete mNext in destructor */
};

/**
 * Linked list of particle effects.
 */
class ParticleList final : public ParticleContainer
{
public:
    explicit ParticleList(ParticleContainer *const parent = nullptr,
                          const bool delParent = true);

    A_DELETE_COPY(ParticleList)

    virtual ~ParticleList();

    /**
     * Takes control of and adds a particle
     */
    void addLocally(Particle *const particle);

    /**
     * `kills' and removes a particle
     */
    void removeLocally(const Particle *const particle);

    virtual void clearLocally() override;

    virtual void moveTo(const float x, const float y) override;

protected:
    std::list<Particle *> mElements;    /**< Contained particle effects */
};

/**
 * Particle container with indexing facilities
 */
class ParticleVector final : public ParticleContainer
{
public:
    explicit ParticleVector(ParticleContainer *const parent = nullptr,
                            const bool delParent = true);

    A_DELETE_COPY(ParticleVector)

    virtual ~ParticleVector();

    /**
     * Sets a particle at a specified index.  Kills the previous particle
     * there, if needed.
     */
    virtual void setLocally(const int index, Particle *const particle);

    /**
     * Removes a particle at a specified index
     */
    virtual void delLocally(const int index);

    virtual void clearLocally() override;

    virtual void moveTo(const float x, const float y) override;

protected:
    std::vector<Particle *> mIndexedElements;
};

#endif  // PARTICLECONTAINER_H
