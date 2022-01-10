/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef NET_EATHENA_BEINGRECV_H
#define NET_EATHENA_BEINGRECV_H

#include "enums/net/beingtype.h"

#include "enums/resources/skill/skilltype2.h"

#include "enums/simpletypes/beingid.h"

#if defined(__GXX_EXPERIMENTAL_CXX0X__)
#if defined(__APPLE__)
#include <tr1/cstdint>
#endif  // defined(__APPLE__)
#endif  // defined(__GXX_EXPERIMENTAL_CXX0X__)

#include "localconsts.h"

namespace Net
{
    class MessageIn;
}  // namespace Net

class Being;

namespace EAthena
{
    namespace BeingRecv
    {
        void processBeingChangeLook2(Net::MessageIn &msg);
        void processBeingChangeLookCards(Net::MessageIn &msg);
        void processBeingVisible(Net::MessageIn &msg);
        void processBeingMove(Net::MessageIn &msg);
        void processBeingSpawn(Net::MessageIn &msg);
        void processMapTypeProperty(Net::MessageIn &msg);
        void processMapType(Net::MessageIn &msg);
        void processSkillCasting(Net::MessageIn &msg);
        void processSkillCasting2(Net::MessageIn &msg);
        void processSkillCasting3(Net::MessageIn &msg);
        void processSkillCastingEvol(Net::MessageIn &msg);
        void processBeingStatusChange(Net::MessageIn &msg);
        void processBeingMove2(Net::MessageIn &msg);
        void processBeingAction2(Net::MessageIn &msg);
        void processMonsterHp(Net::MessageIn &msg);
        void processBeingHp(Net::MessageIn &msg);
        void processSkillAutoCast(Net::MessageIn &msg);
        void processRanksList1(Net::MessageIn &msg);
        void processRanksList2(Net::MessageIn &msg);
        void processBeingChangeDirection(Net::MessageIn &msg);
        void processBeingChangeLookContinue(const Net::MessageIn &msg,
                                            Being *const dstBeing,
                                            const uint8_t type,
                                            const int id,
                                            const int id2,
                                            const int *const cards)
                                            A_NONNULL(2);
        void processBeingSpecialEffect(Net::MessageIn &msg);
        void processBeingSpecialEffectNum(Net::MessageIn &msg);
        void processBeingHatEffects(Net::MessageIn &msg);
        void processBeingRemoveSpecialEffect(Net::MessageIn &msg);
        void processBeingSoundEffect(Net::MessageIn &msg);
        void processSkillGroundNoDamage(Net::MessageIn &msg);
        void processSkillEntry(Net::MessageIn &msg);
        void processPlayerStatusChange(Net::MessageIn &msg);
        void processBeingResurrect(Net::MessageIn &msg);
        void processNameResponseTitle(Net::MessageIn &msg);
        void processPlayerGuilPartyInfo(Net::MessageIn &msg);
        void processPlayerGuilPartyInfo2(Net::MessageIn &msg);
        void processBeingRemoveSkill(Net::MessageIn &msg);
        void processBeingFakeName(Net::MessageIn &msg);
        void processBeingStatUpdate1(Net::MessageIn &msg);
        void processPlayerStatusChange2(Net::MessageIn &msg);
        void processBeingSelfEffect(Net::MessageIn &msg);
        void processMobInfo(Net::MessageIn &msg);
        void processBeingAttrs(Net::MessageIn &msg);
        void processMonsterInfo(Net::MessageIn &msg);
        void processClassChange(Net::MessageIn &msg);
        void processSpiritBalls(Net::MessageIn &msg);
        void processBladeStop(Net::MessageIn &msg);
        void processComboDelay(Net::MessageIn &msg);
        void processWddingEffect(Net::MessageIn &msg);
        void processBeingSlide(Net::MessageIn &msg);
        void processStarsKill(Net::MessageIn &msg);
        void processBlacksmithRanksList(Net::MessageIn &msg);
        void processAlchemistRanksList(Net::MessageIn &msg);
        void processTaekwonRanksList(Net::MessageIn &msg);
        void processPkRanksList(Net::MessageIn &msg);
        void processGladiatorFeelRequest(Net::MessageIn &msg);
        void processBossMapInfo(Net::MessageIn &msg);
        void processBeingFont(Net::MessageIn &msg);
        void processBeingMilleniumShield(Net::MessageIn &msg);
        void processBeingCharm(Net::MessageIn &msg);
        void processBeingViewEquipment(Net::MessageIn &msg);
        void processBeingViewEquipment2(Net::MessageIn &msg);
        void processPvpSet(Net::MessageIn &msg);
        void processNameResponse2(Net::MessageIn &msg);
        void processSkillCancel(Net::MessageIn &msg);
        void processSolveCharName(Net::MessageIn &msg);
        void processGraffiti(Net::MessageIn &msg);
        void processSkillDamage(Net::MessageIn &msg);
        void processNavigateTo(Net::MessageIn &msg);

        Being *createBeing2(Net::MessageIn &msg,
                            const BeingId id,
                            const int32_t job,
                            const BeingTypeT beingType);
        void applyPlayerAction(Net::MessageIn &msg,
                               Being *const being,
                               const uint8_t type);
        void processSkillCastingContinue(Net::MessageIn &msg,
                                         const BeingId srcId,
                                         const BeingId dstId,
                                         const int dstX,
                                         const int dstY,
                                         const int skillId,
                                         const int skillLevel,
                                         const int range,
                                         const SkillType2::SkillType2 inf2,
                                         const int castTime);
    }  // namespace BeingRecv
}  // namespace EAthena

#endif  // NET_EATHENA_BEINGRECV_H
