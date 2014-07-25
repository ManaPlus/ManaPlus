/*
 *  The ManaPlus Client
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

#ifndef GAMEMODIFIERS_H
#define GAMEMODIFIERS_H

#include <string>

#include "localconsts.h"

class GameModifiers final
{
    public:
        GameModifiers();

        A_DELETE_COPY(GameModifiers)

        ~GameModifiers();

        static void changeMode(unsigned *restrict const var,
                               const unsigned limit,
                               const char *restrict const conf,
                               std::string (*const func)(),
                               const unsigned def,
                               const bool save,
                               const bool forward);

        static void changeMoveType(const bool forward);

        static void changeCrazyMoveType(const bool forward);

        static void changeMoveToTargetType(const bool forward);

        static void changeFollowMode(const bool forward);

        static void changeAttackWeaponType(const bool forward);

        static void changeAttackType(const bool forward);

        static void changeQuickDropCounter(const bool forward);

        static void changePickUpType(const bool forward);

        static void changeMagicAttackType(const bool forward);

        static void changePvpAttackType(const bool forward);

        static void changeImitationMode(const bool forward);

        static void changeGameModifiers();

        static void changeAwayMode();

        static void setQuickDropCounter(const int n);

        static std::string getMoveTypeString();

        static std::string getCrazyMoveTypeString();

        static std::string getMoveToTargetTypeString();

        static std::string getFollowModeString();

        static std::string getAttackWeaponTypeString();

        static std::string getAttackTypeString();

        static std::string getQuickDropCounterString();

        static std::string getPickUpTypeString();

        static std::string getMagicAttackTypeString();

        static std::string getPvpAttackTypeString();

        static std::string getImitationModeString();

        static std::string getGameModifiersString();

        static std::string getMapDrawTypeString();

        static std::string getAwayModeString();

        static std::string getCameraModeString();

        static void resetModifiers();

    protected:
        static const char *getVarItem(const char *const *const arr,
                                      const unsigned index,
                                      const unsigned sz) A_WARN_UNUSED;
};

#endif  // GAMEMODIFIERS_H
