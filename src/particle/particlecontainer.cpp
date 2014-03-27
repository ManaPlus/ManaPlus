/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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
#include "particle/particlecontainer.h"

#include "utils/delete2.h"

#include "debug.h"

typedef std::list<Particle *>::iterator ParticleListIter;
typedef std::list<Particle *>::const_iterator ParticleListCIter;

ParticleContainer::ParticleContainer(ParticleContainer *const parent,
                                     const bool delParent):
    mNext(parent),
    mDelParent(delParent)
{
}

ParticleContainer::~ParticleContainer()
{
    // +++ call virtul method in destructor
    clearLocally();
    if (mDelParent)
        delete2(mNext)
}

void ParticleContainer::clear()
{
    clearLocally();
    if (mNext)
        mNext->clear();
}

void ParticleContainer::moveTo(const float x, const float y)
{
    if (mNext)
        mNext->moveTo(x, y);
}

// -- particle list ----------------------------------------

ParticleList::ParticleList(ParticleContainer *const parent,
                           const bool delParent) :
    ParticleContainer(parent, delParent),
    mElements()
{}

ParticleList::~ParticleList()
{}

void ParticleList::addLocally(Particle *const particle)
{
    if (particle)
    {
        // The effect may not die without the beings permission or we segfault
        particle->disableAutoDelete();
        mElements.push_back(particle);
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
            it = mElements.erase(it);
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
        (*it)->kill();

    mElements.clear();
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
        }
        else
        {
            ++it;
        }
    }
}

// -- particle vector ----------------------------------------

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

    if (mIndexedElements.size() <= static_cast<unsigned>(index))
        mIndexedElements.resize(index + 1, nullptr);

    if (particle)
        particle->disableAutoDelete();
    mIndexedElements[index] = particle;
}

void ParticleVector::delLocally(const int index)
{
    if (index < 0)
        return;

    if (mIndexedElements.size() <= static_cast<unsigned>(index))
        return;

    Particle *const p = mIndexedElements[index];
    if (p)
    {
        mIndexedElements[index] = nullptr;
        p->kill();
    }
}

void ParticleVector::clearLocally()
{
    for (unsigned int i = 0; i < mIndexedElements.size(); i++)
        delLocally(i);
}

void ParticleVector::moveTo(const float x, const float y)
{
    ParticleContainer::moveTo(x, y);

    for (std::vector<Particle *>::iterator it = mIndexedElements.begin();
         it != mIndexedElements.end(); ++it)
    {
        Particle *const p = *it;
        if (p)
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
