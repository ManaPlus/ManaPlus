/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef GAMEMODIFIERS_H
#define GAMEMODIFIERS_H

#include <string>

#include "localconsts.h"

#define declModifier(name1) \
    static void change##name1(const bool forward); \
    static std::string get##name1##String(); \
    static const unsigned m##name1##Size; \
    static const char *const m##name1##Strings[];

class GameModifiers final
{
    public:
        A_DELETE_COPY(GameModifiers)

        static void init();

        static void changeMode(unsigned *restrict const var,
                               const unsigned limit,
                               const char *restrict const conf,
                               std::string (*const func)(),
                               const unsigned def,
                               const bool save,
                               const bool forward);

        declModifier(MoveType)
        declModifier(CrazyMoveType)
        declModifier(MoveToTargetType)
        declModifier(FollowMode)
        declModifier(AttackWeaponType)
        declModifier(AttackType)
        declModifier(TargetingType)
        declModifier(QuickDropCounter)
        declModifier(PickUpType)
        declModifier(MagicAttackType)
        declModifier(PvpAttackType)
        declModifier(ImitationMode)
        declModifier(GameModifiers)
        declModifier(MapDrawType)
        declModifier(CameraMode)
        declModifier(AwayMode)

        static void setQuickDropCounter(const int n);

        static void resetModifiers();

    protected:
        static const char *getVarItem(const char *const *const arr,
                                      const unsigned index,
                                      const unsigned sz)
                                      A_WARN_UNUSED A_NONNULL(1);
};

#undef declModifier

#endif  // GAMEMODIFIERS_H
