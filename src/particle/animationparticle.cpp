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

#include "particle/animationparticle.h"

#include "simpleanimation.h"

#include "debug.h"

AnimationParticle::AnimationParticle(Animation *const animation) :
    ImageParticle(nullptr),
    mAnimation(new SimpleAnimation(animation))
{
}

AnimationParticle::AnimationParticle(XmlNodePtrConst animationNode,
                                     const std::string& dyePalettes):
    ImageParticle(nullptr),
    mAnimation(new SimpleAnimation(animationNode, dyePalettes))
{
}

AnimationParticle::~AnimationParticle()
{
    delete mAnimation;
    mAnimation = nullptr;
    mImage = nullptr;
}

bool AnimationParticle::update()
{
    if (mAnimation)
    {
        mAnimation->update(10);  // particle engine is updated every 10ms
        mImage = mAnimation->getCurrentImage();
    }
    return Particle::update();
}
