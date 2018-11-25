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

#include "net/eathena/mercenaryrecv.h"

#include "actormanager.h"
#include "notifymanager.h"

#include "being/localplayer.h"
#include "being/mercenaryinfo.h"
#include "being/playerinfo.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/skilldialog.h"

#include "net/messagein.h"

#include "net/eathena/sp.h"

#include "utils/checkutils.h"

#include "debug.h"

namespace EAthena
{

#define setMercStat(sp, stat) \
        case sp: \
            PlayerInfo::setStatBase(stat, \
                val, \
                Notify_true); \
            break

void MercenaryRecv::processMercenaryUpdate(Net::MessageIn &msg)
{
    const int sp = msg.readInt16("type");
    const int val = msg.readInt32("value");
    switch (sp)
    {
        setMercStat(Sp::ATK1, Attributes::MERC_ATK);
        setMercStat(Sp::MATK1, Attributes::MERC_MATK);
        setMercStat(Sp::HIT, Attributes::MERC_HIT);
        setMercStat(Sp::CRITICAL, Attributes::MERC_CRIT);
        setMercStat(Sp::DEF1, Attributes::MERC_DEF);
        setMercStat(Sp::MDEF1, Attributes::MERC_MDEF);
        setMercStat(Sp::MERCFLEE, Attributes::MERC_FLEE);
        setMercStat(Sp::ASPD, Attributes::MERC_ATTACK_DELAY);
        setMercStat(Sp::HP, Attributes::MERC_HP);
        setMercStat(Sp::MAXHP, Attributes::MERC_MAX_HP);
        setMercStat(Sp::SP, Attributes::MERC_MP);
        setMercStat(Sp::MAXSP, Attributes::MERC_MAX_MP);
        setMercStat(Sp::MERCKILLS, Attributes::MERC_KILLS);
        setMercStat(Sp::MERCFAITH, Attributes::MERC_FAITH);
        default:
            reportAlways("Unknown mercenary stat %d",
                sp)
            break;
    }
}

void MercenaryRecv::processMercenaryInfo(Net::MessageIn &msg)
{
    if (actorManager == nullptr)
        return;
    // +++ need create if need mercenary being and update stats
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));
    PlayerInfo::setStatBase(Attributes::MERC_ATK,
        msg.readInt16("atk"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_MATK,
        msg.readInt16("matk"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_HIT,
        msg.readInt16("hit"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_CRIT,
        msg.readInt16("crit/10"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_DEF,
        msg.readInt16("def"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_MDEF,
        msg.readInt16("mdef"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_FLEE,
        msg.readInt16("flee"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_ATTACK_DELAY,
        msg.readInt16("attack speed"),
        Notify_true);
    const std::string name = msg.readString(24, "name");
    const int level = msg.readInt16("level");
    PlayerInfo::setStatBase(Attributes::MERC_LEVEL,
        level,
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_HP,
        msg.readInt32("hp"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_MAX_HP,
        msg.readInt32("max hp"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_MP,
        msg.readInt32("sp"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_MAX_MP,
        msg.readInt32("max sp"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_EXPIRE,
        msg.readInt32("expire time"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_FAITH,
        msg.readInt16("faith"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_CALLS,
        msg.readInt32("calls"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::MERC_KILLS,
        msg.readInt32("kills"),
        Notify_true);
    const int range = msg.readInt16("attack range");
    PlayerInfo::setStatBase(Attributes::MERC_ATTACK_RANGE,
        range,
        Notify_true);
    PlayerInfo::updateAttrs();

    if ((dstBeing != nullptr) && (localPlayer != nullptr))
    {
        MercenaryInfo *const mercenary = new MercenaryInfo;
        mercenary->id = dstBeing->getId();
        mercenary->name = name;
        mercenary->level = level;
        mercenary->range = range;
        PlayerInfo::setMercenary(mercenary);
        PlayerInfo::setMercenaryBeing(dstBeing);
    }
}

void MercenaryRecv::processMercenarySkills(Net::MessageIn &msg)
{
    if (skillDialog != nullptr)
        skillDialog->hideSkills(SkillOwner::Mercenary);
    const int count = (msg.readInt16("len") - 4) / 37;
    for (int f = 0; f < count; f ++)
    {
        const int skillId = msg.readInt16("skill id");
        const SkillType::SkillType inf = static_cast<SkillType::SkillType>(
            msg.readInt32("inf"));
        const int level = msg.readInt16("skill level");
        const int sp = msg.readInt16("sp");
        const int range = msg.readInt16("range");
        const std::string name = msg.readString(24, "skill name");
        const Modifiable up = fromBool(msg.readUInt8("up flag"), Modifiable);
        PlayerInfo::setSkillLevel(skillId, level);
        if (skillDialog != nullptr)
        {
            if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
            {
                skillDialog->addSkill(SkillOwner::Mercenary,
                    skillId, name, level, range, up, inf, sp);
            }
        }
    }
    if (skillDialog != nullptr)
        skillDialog->updateModels();
}

void MercenaryRecv::handleMercenaryMessage(const int cmd)
{
    PlayerInfo::setMercenary(nullptr);
    if (skillDialog != nullptr)
    {
        skillDialog->hideSkills(SkillOwner::Mercenary);
        skillDialog->updateModels();
    }

    switch (cmd)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::MERCENARY_EXPIRED);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::MERCENARY_KILLED);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::MERCENARY_FIRED);
            break;
        case 3:
            NotifyManager::notify(NotifyTypes::MERCENARY_RUN);
            break;
        default:
            NotifyManager::notify(NotifyTypes::MERCENARY_UNKNOWN);
            break;
    }
}

}  // namespace EAthena
