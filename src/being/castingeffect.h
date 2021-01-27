/*
 *  The ManaPlus Client
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

#ifndef BEING_CASTINGEFFECT_H
#define BEING_CASTINGEFFECT_H

#include "being/actor.h"

#include "enums/being/actortype.h"

#include "localconsts.h"

class AnimatedSprite;

class CastingEffect final : public Actor
{
    public:
        CastingEffect(const int skillId,
                      const int skillLevel,
                      const std::string &animation,
                      const int x,
                      const int y,
                      const int range);

        A_DELETE_COPY(CastingEffect)

        ~CastingEffect() override final;

        virtual ActorTypeT getType() const noexcept2 A_WARN_UNUSED
        { return ActorType::Unknown; }

        void draw(Graphics *const graphics,
                  const int offsetX,
                  const int offsetY) const override final A_NONNULL(2);

        int getSortPixelY() const restrict2 override A_WARN_UNUSED
        { return mPixelY - mYDiff; }

        void update(const int time);

        bool isTerminated() const;

        float getAlpha() const override final A_WARN_UNUSED
        { return 1.0F; }

        void setAlpha(const float alpha A_UNUSED) override final
        { }

    protected:
        AnimatedSprite *mSprite;
        int mRectX;
        int mRectY;
        int mRectSize;
        int mAnimationX;
        int mAnimationY;
};

#endif  // BEING_CASTINGEFFECT_H
