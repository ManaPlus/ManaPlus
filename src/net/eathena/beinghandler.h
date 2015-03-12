/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef NET_EATHENA_BEINGHANDLER_H
#define NET_EATHENA_BEINGHANDLER_H

#include "net/ea/beinghandler.h"

#include "net/eathena/beingtype.h"
#include "net/eathena/messagehandler.h"

namespace EAthena
{

class BeingHandler final : public MessageHandler, public Ea::BeingHandler
{
    public:
        explicit BeingHandler(const bool enableSync);

        A_DELETE_COPY(BeingHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void requestNameById(const int id) const override final;

        void undress(Being *const being) const override final;

        void requestRanks(const Rank::Rank rank) const override final;

    protected:
        static Being *createBeing2(const int id,
                                   const int16_t job,
                                   const BeingType::BeingType beingType);

        static void processBeingChangeLook2(Net::MessageIn &msg);

        static void processBeingVisible(Net::MessageIn &msg);

        static void processBeingMove(Net::MessageIn &msg);

        static void processBeingSpawn(Net::MessageIn &msg);

        static void processMapTypeProperty(Net::MessageIn &msg);

        static void processMapType(Net::MessageIn &msg);

        static void processSkillCasting(Net::MessageIn &msg);

        static void processBeingStatusChange(Net::MessageIn &msg);

        static void processBeingStatusChange2(Net::MessageIn &msg);

        static void processBeingMove2(Net::MessageIn &msg);

        static void processBeingAction2(Net::MessageIn &msg);

        static void processMonsterHp(Net::MessageIn &msg);

        static void processSkillAutoCast(Net::MessageIn &msg);

        static void processRanksList(Net::MessageIn &msg);

        static void processBeingChangeDirection(Net::MessageIn &msg);

        static void processBeingChangeLookContinue(Being *const dstBeing,
                                                   const uint8_t type,
                                                   const int id,
                                                   const int id2);

        static void processBeingSpecialEffect(Net::MessageIn &msg);

        static void processBeingSpecialEffectNum(Net::MessageIn &msg);

        static void processBeingSoundEffect(Net::MessageIn &msg);

        static void applyPlayerAction(Being *const being,
                                      const uint8_t type);

        void viewPlayerEquipment(const Being *const being);

        static void processSkillGroundNoDamage(Net::MessageIn &msg);

        static void processSkillEntry(Net::MessageIn &msg);

        static void processPlaterStatusChange(Net::MessageIn &msg);

        static void processPlaterStatusChangeNoTick(Net::MessageIn &msg);

        static void processBeingResurrect(Net::MessageIn &msg);

        static void processPlayerGuilPartyInfo(Net::MessageIn &msg);

        static void processBeingRemoveSkil(Net::MessageIn &msg);

        static void processBeingFakeName(Net::MessageIn &msg);

        static void processBeingStatUpdate1(Net::MessageIn &msg);

        static void processPlaterStatusChange2(Net::MessageIn &msg);

        static void processBeingSelfEffect(Net::MessageIn &msg);

        static void processMobInfo(Net::MessageIn &msg);

        static void processBeingAttrs(Net::MessageIn &msg);

        static void processMonsterInfo(Net::MessageIn &msg);

        static void processClassChange(Net::MessageIn &msg);

        static void processSpiritBalls(Net::MessageIn &msg);

        static void processSpiritBallSingle(Net::MessageIn &msg);

        static void processBladeStop(Net::MessageIn &msg);

        static void processComboDelay(Net::MessageIn &msg);

        static void processWddingEffect(Net::MessageIn &msg);

        static void processBeingSlide(Net::MessageIn &msg);

        static void processStarsKill(Net::MessageIn &msg);

        static void processBlacksmithRanksList(Net::MessageIn &msg);

        static void processAlchemistRanksList(Net::MessageIn &msg);

        static void processTaekwonRanksList(Net::MessageIn &msg);

        static void processPkRanksList(Net::MessageIn &msg);
};

}  // namespace EAthena

#endif  // NET_EATHENA_BEINGHANDLER_H
