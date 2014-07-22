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

        void changeMode(unsigned *restrict const var,
                        const unsigned limit,
                        const char *restrict const conf,
                        std::string (GameModifiers::*const func)(),
                        const unsigned def,
                        const bool save,
                        const bool forward);

        void changeMoveType(const bool forward);

        void changeCrazyMoveType(const bool forward);

        void changeMoveToTargetType(const bool forward);

        void changeFollowMode(const bool forward);

        void changeAttackWeaponType(const bool forward);

        void changeAttackType(const bool forward);

        void changeQuickDropCounter(const bool forward);

        void changePickUpType(const bool forward);

        void changeMagicAttackType(const bool forward);

        void changePvpAttackType(const bool forward);

        void changeImitationMode(const bool forward);

        void changeGameModifiers();

        void changeAwayMode();

        void setQuickDropCounter(const int n);

        std::string getMoveTypeString();

        std::string getCrazyMoveTypeString();

        std::string getMoveToTargetTypeString();

        std::string getFollowModeString();

        std::string getAttackWeaponTypeString();

        std::string getAttackTypeString();

        std::string getQuickDropCounterString();

        std::string getPickUpTypeString();

        std::string getMagicAttackString();

        std::string getPvpAttackTypeString();

        std::string getImitationModeString();

        std::string getGameModifiersString();

        std::string getMapDrawTypeString();

        std::string getAwayModeString();

    protected:
        static const char *getVarItem(const char *const *const arr,
                                      const unsigned index,
                                      const unsigned sz) A_WARN_UNUSED;
};

extern GameModifiers *modifiers;

#endif  // GAMEMODIFIERS_H
