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

#include "particle/particlelist.h"

#include "utils/foreach.h"

#include "particle/particle.h"

#include "debug.h"

typedef std::list<Particle *>::iterator ParticleListIter;
typedef std::list<Particle *>::const_iterator ParticleListCIter;

ParticleList::ParticleList(ParticleContainer *const parent,
                           const bool delParent) :
    ParticleContainer(parent, delParent),
    mElements(),
    mSize(0U)
{}

ParticleList::~ParticleList()
{
}

void ParticleList::addLocally(Particle *const particle)
{
    if (particle != nullptr)
    {
        mElements.push_back(particle);
        mSize ++;
    }
}

void ParticleList::removeLocally(const Particle *const particle)
{
    for (std::list<Particle *>::iterator it = mElements.begin();
         it != mElements.end(); )
    {
        Particle *const p = *it;
        if (p == particle)
        {
            p->kill();
            p->prepareToDie();
            it = mElements.erase(it);
            mSize --;
        }
        else
        {
            ++it;
        }
    }
}

void ParticleList::clearLocally()
{
    FOR_EACH (ParticleListCIter, it, mElements)
    {
        (*it)->kill();
        (*it)->prepareToDie();
    }

    mElements.clear();
    mSize = 0U;
}

void ParticleList::moveTo(const float x, const float y)
{
    ParticleContainer::moveTo(x, y);

    for (std::list<Particle *>::iterator it = mElements.begin();
         it != mElements.end(); )
    {
        Particle *const p = *it;
        p->moveTo(x, y);
        if (p->isExtinct())
        {
            p->kill();
            it = mElements.erase(it);
            mSize --;
        }
        else
        {
            ++it;
        }
    }
}
