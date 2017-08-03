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

#ifndef NET_EATHENA_NPCHANDLER_H
#define NET_EATHENA_NPCHANDLER_H

#include "net/ea/npchandler.h"

namespace EAthena
{

class NpcHandler final : public Ea::NpcHandler
{
    public:
        NpcHandler();

        A_DELETE_COPY(NpcHandler)

        ~NpcHandler();

        void talk(const Being *const being) const override final;

        void nextDialog(const BeingId npcId) const override final;

        void closeDialog(const BeingId npcId) override final;

        void listInput(const BeingId npcId,
                       const unsigned char value) const override final;

        void integerInput(const BeingId npcId,
                          const int value) const override final;

        void stringInput(const BeingId npcId,
                         const std::string &value) const override final;

        void buy(const Being *const being) const override final;

        void buy(const BeingId beingId) const override final;

        void sell(const BeingId beingId) const override final;

        void buyItem(const BeingId beingId,
                     const int itemId,
                     const ItemColor color,
                     const int amount) const override final;

        void buyItems(STD_VECTOR<ShopItem*> &items) const
                      override final;

        void sellItem(const BeingId beingId,
                      const int itemId,
                      const int amount) const override final;

        void sellItems(STD_VECTOR<ShopItem*> &items) const override final;

        void completeProgressBar() const override final;

        void produceMix(const int nameId,
                        const int materialId1,
                        const int materialId2,
                        const int materialId3) const override final;

        void cooking(const CookingTypeT type,
                     const int nameId) const override final;

        void repair(const int index) const override final;

        void refine(const int index) const override final;

        BeingId getNpc(Net::MessageIn &msg,
                       const NpcActionT action) override final;

        void identify(const int index) const override final;

        void selectArrow(const int nameId) const override final;

        void selectAutoSpell(const int skillId) const override final;
};

}  // namespace EAthena

#endif  // NET_EATHENA_NPCHANDLER_H
