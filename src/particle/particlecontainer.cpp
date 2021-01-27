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

#include "particle/particlecontainer.h"

#include "utils/delete2.h"

#include "debug.h"

ParticleContainer::ParticleContainer(ParticleContainer *const parent,
                                     const bool delParent) :
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
    if (mNext != nullptr)
        mNext->clear();
}

void ParticleContainer::moveTo(const float x, const float y)
{
    if (mNext != nullptr)
        mNext->moveTo(x, y);
}
