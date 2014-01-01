/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "animationdelayload.h"

#include "animatedsprite.h"

#include "resources/resourcemanager.h"

#include "debug.h"

AnimationDelayLoad::AnimationDelayLoad(const std::string &fileName,
                                       const int variant,
                                       AnimatedSprite *const sprite) :
    mFileName(fileName),
    mVariant(variant),
    mSprite(sprite),
    mAction(SpriteAction::STAND)
{
}

AnimationDelayLoad::~AnimationDelayLoad()
{
    if (mSprite)
    {
        mSprite->clearDelayLoad();
        mSprite = nullptr;
    }
}

void AnimationDelayLoad::clearSprite()
{
    mSprite = nullptr;
}

void AnimationDelayLoad::load()
{
    if (mSprite)
    {
        ResourceManager *const resman = ResourceManager::getInstance();
        SpriteDef *const s = resman->getSprite(mFileName, mVariant);
        if (!s)
            return;
        mSprite->setSprite(s);
        mSprite->play(mAction);
    }
}
