/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_NPCHANDLER_H
#define NET_NPCHANDLER_H

#include <iosfwd>

#include "being/cookingtype.h"

#include "localconsts.h"

namespace Net
{

class NpcHandler notfinal
{
    public:
        virtual ~NpcHandler()
        { }

        virtual void talk(const int npcId) const = 0;

        virtual void nextDialog(const int npcId) const = 0;

        virtual void closeDialog(const int npcId) = 0;

        virtual void listInput(const int npcId,
                               const unsigned char value) const = 0;

        virtual void integerInput(const int npcId, const int value) const = 0;

        virtual void stringInput(const int npcId,
                                 const std::string &value) const = 0;

        virtual void sendLetter(const int npcId, const std::string &recipient,
                                const std::string &text) const = 0;

        virtual void startShopping(const int beingId) const = 0;

        virtual void buy(const int beingId) const = 0;

        virtual void sell(const int beingId) const = 0;

        virtual void buyItem(const int beingId, const int itemId,
                             const unsigned char color,
                             const int amount) const = 0;

        virtual void sellItem(const int beingId, const int itemId,
                              const int amount) const = 0;

        virtual void endShopping(const int beingId) const = 0;

        virtual void completeProgressBar() const = 0;

        virtual void produceMix(const int nameId,
                                const int materialId1,
                                const int materialId2,
                                const int materialId3) const  = 0;

        virtual void cooking(const CookingType::Type type,
                             const int nameId) const = 0;

        virtual void repair(const int index) const = 0;

        virtual void refine(const int index) const = 0;

        virtual void identify(const int index) const = 0;

        virtual void selectArrow(const int nameId) const = 0;

        virtual void selectAutoSpell(const int skillId) const = 0;
};

}  // namespace Net

extern Net::NpcHandler *npcHandler;

#endif  // NET_NPCHANDLER_H
