/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef RESOURCES_ATTACK_H
#define RESOURCES_ATTACK_H

#include <string>

#include "localconsts.h"

struct Attack final
{
    std::string mAction;
    std::string mSkyAction;
    std::string mWaterAction;
    std::string mRideAction;
    int mEffectId;
    int mHitEffectId;
    int mCriticalHitEffectId;
    int mMissEffectId;
    std::string mMissileParticle;

    Attack(const std::string &action,
           const std::string &skyAction,
           const std::string &waterAction,
           const std::string &rideAction,
           const int effectId,
           const int hitEffectId,
           const int criticalHitEffectId,
           const int missEffectId,
           const std::string &missileParticle) :
        mAction(action),
        mSkyAction(skyAction),
        mWaterAction(waterAction),
        mRideAction(rideAction),
        mEffectId(effectId),
        mHitEffectId(hitEffectId),
        mCriticalHitEffectId(criticalHitEffectId),
        mMissEffectId(missEffectId),
        mMissileParticle(missileParticle)
    {
    }

    A_DELETE_COPY(Attack)
};

#endif  // RESOURCES_ATTACK_H
