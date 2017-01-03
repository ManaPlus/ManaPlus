/*
 *  The ManaPlus Client
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

#include "being/castingeffect.h"

#include "configuration.h"

#include "const/resources/map/map.h"

#include "gui/userpalette.h"

#include "render/graphics.h"

#include "resources/sprite/animatedsprite.h"

#include "utils/checkutils.h"

#include "debug.h"

CastingEffect::CastingEffect(const int skillId,
                             const int skillLevel,
                             const std::string &animation,
                             const int x,
                             const int y,
                             const int range) :
    Actor(),
    mSprite(animation.empty() ? nullptr :
        AnimatedSprite::load(paths.getStringValue("sprites") + animation)),
    mRectX((x - range) * mapTileSize),
    mRectY((y - range) * mapTileSize),
    mRectSize(range * mapTileSize * 2 + mapTileSize),
    mAnimationX(mRectX + (mRectSize - (mSprite ?
        mSprite->getWidth() : 0)) / 2),
    mAnimationY(mRectY + (mRectSize - (mSprite ?
        mSprite->getHeight() : 0)) / 2)
{
    mPixelX = x * mapTileSize;
    mPixelY = y * mapTileSize;
    mYDiff = range * mapTileSize + 31;
    if (!mSprite)
    {
        reportAlways("Skill %d/%d casting animation '%s' load failed",
            skillId,
            skillLevel,
            animation.c_str());
    }
}

CastingEffect::~CastingEffect()
{
}

void CastingEffect::draw(Graphics *const graphics,
                         const int offsetX,
                         const int offsetY) const
{
    graphics->setColor(userPalette->getColorWithAlpha(
        UserColorId::ATTACK_RANGE_BORDER));

    graphics->drawRectangle(Rect(
        mRectX + offsetX,
        mRectY + offsetY,
        mRectSize,
        mRectSize));
    if (mSprite)
    {
        mSprite->draw(graphics,
            mAnimationX + offsetX,
            mAnimationY + offsetY);
    }
}

void CastingEffect::update(const int time)
{
    if (mSprite)
        mSprite->update(time);
}

bool CastingEffect::isTerminated() const
{
    if (mSprite)
        return mSprite->isTerminated();
    return false;
}
