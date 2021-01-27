/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "particle/particle.h"
#include "particle/particlevector.h"

#include "debug.h"

ParticleVector::ParticleVector(ParticleContainer *const parent,
                               const bool delParent) :
    ParticleContainer(parent, delParent),
    mIndexedElements()
{}

ParticleVector::~ParticleVector()
{}

void ParticleVector::setLocally(const int index, Particle *const particle)
{
    if (index < 0)
        return;

    delLocally(index);

    if (mIndexedElements.size() <= CAST_SIZE(index))
        mIndexedElements.resize(index + 1, nullptr);

    if (particle != nullptr)
        particle->disableAutoDelete();
    mIndexedElements[index] = particle;
}

void ParticleVector::delLocally(const int index)
{
    if (index < 0)
        return;

    if (mIndexedElements.size() <= CAST_SIZE(index))
        return;

    Particle *const p = mIndexedElements[index];
    if (p != nullptr)
    {
        mIndexedElements[index] = nullptr;
        p->kill();
    }
}

void ParticleVector::clearLocally()
{
    for (unsigned int i = 0;
         i < CAST_U32(mIndexedElements.size());
         i++)
    {
        delLocally(i);
    }
}

void ParticleVector::moveTo(const float x, const float y)
{
    ParticleContainer::moveTo(x, y);

    for (STD_VECTOR<Particle *>::iterator it = mIndexedElements.begin();
         it != mIndexedElements.end(); ++it)
    {
        Particle *const p = *it;
        if (p != nullptr)
        {
            p->moveTo(x, y);

            if (p->isExtinct())
            {
                p->kill();
                *it = nullptr;
            }
        }
    }
}

size_t ParticleVector::usedSize() const
{
    size_t cnt = 0;
    for (STD_VECTOR<Particle *>::const_iterator it = mIndexedElements.begin();
         it != mIndexedElements.end(); ++it)
    {
        if (*it != nullptr)
            cnt ++;
    }
    return cnt;
}
