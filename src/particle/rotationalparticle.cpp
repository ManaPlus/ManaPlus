/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
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

#include "particle/rotationalparticle.h"

#include "simpleanimation.h"

#include "utils/delete2.h"

#include <cmath>

#include "debug.h"

static const double PI = M_PI;
static const float PI2 = 2 * M_PI;

RotationalParticle::RotationalParticle(Animation *const animation) :
    ImageParticle(nullptr),
    mAnimation(new SimpleAnimation(animation))
{
}

RotationalParticle::RotationalParticle(const XmlNodePtr animationNode,
                                       const std::string& dyePalettes):
    ImageParticle(nullptr),
    mAnimation(new SimpleAnimation(animationNode, dyePalettes))
{
}

RotationalParticle::~RotationalParticle()
{
    delete2(mAnimation);
    mImage = nullptr;
}

bool RotationalParticle::update()
{
    if (!mAnimation)
        return false;

    // TODO: cache velocities to avoid spamming atan2()

    const int size = mAnimation->getLength();
    if (!size)
        return false;

    float rad = static_cast<float>(atan2(mVelocity.x, mVelocity.y));
    if (rad < 0)
        rad = PI2 + rad;

    const float range = static_cast<const float>(PI / size);

    // Determines which frame the particle should play
    if (rad < range || rad > PI2 - range)
    {
        mAnimation->setFrame(0);
    }
    else
    {
        for (int c = 1; c < size; c++)
        {
            if (((static_cast<float>(c) * (2 * range)) - range) < rad
                && rad < ((static_cast<float>(c) * (2 * range)) + range))
            {
                mAnimation->setFrame(c);
                break;
            }
        }
    }

    mImage = mAnimation->getCurrentImage();

    return Particle::update();
}
