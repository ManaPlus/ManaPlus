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

#include "net/eathena/homunculusrecv.h"

#include "actormanager.h"
#include "notifymanager.h"

#include "being/homunculusinfo.h"
#include "being/playerinfo.h"
#include "being/localplayer.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/skilldialog.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "net/messagein.h"

#include "utils/stringutils.h"

#include "debug.h"

namespace EAthena
{

void HomunculusRecv::processHomunculusSkills(Net::MessageIn &msg)
{
    if (skillDialog != nullptr)
        skillDialog->hideSkills(SkillOwner::Homunculus);

    const int count = (msg.readInt16("len") - 4) / 37;
    for (int f = 0; f < count; f ++)
    {
        const int skillId = msg.readInt16("skill id");
        const SkillType::SkillType inf = static_cast<SkillType::SkillType>(
            msg.readInt16("inf"));
        msg.readInt16("unused");
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
                skillDialog->addSkill(SkillOwner::Homunculus,
                    skillId, name, level, range, up, inf, sp);
            }
        }
    }
    if (skillDialog != nullptr)
        skillDialog->updateModels();
}

void HomunculusRecv::processHomunculusData(Net::MessageIn &msg)
{
    msg.readUInt8("unused");
    const int cmd = msg.readUInt8("state");
    const BeingId id = msg.readBeingId("homunculus id");
    Being *const dstBeing = actorManager->findBeing(id);
    const int data = msg.readInt32("data");
    if (cmd == 0)  // pre init
    {
        HomunculusInfo *const info = new HomunculusInfo;
        info->id = id;
        PlayerInfo::setHomunculus(info);
        PlayerInfo::setHomunculusBeing(dstBeing);
        return;
    }
    HomunculusInfo *const info = PlayerInfo::getHomunculus();
    if (info == nullptr)
        return;
    switch (cmd)
    {
        case 1:  // intimacy
            info->intimacy = data;
            break;
        case 2:  // hunger
            info->hungry = data;
            break;
        case 3:  // accesory
            info->equip = data;
            break;
        default:
            break;
    }
}

void HomunculusRecv::processHomunculusInfo(Net::MessageIn &msg)
{
    const std::string name = msg.readString(24, "name");
    msg.readUInt8("flags");  // 0x01 - renamed, 0x02 - vaporize, 0x04 - alive
    const int level = msg.readInt16("level");
    PlayerInfo::setStatBase(Attributes::HOMUN_LEVEL,
        level,
        Notify_true);
    const int hungry = msg.readInt16("hungry");
    const int intimacy = msg.readInt16("intimacy");
    const int equip = msg.readInt16("equip");
    PlayerInfo::setStatBase(Attributes::HOMUN_ATK,
        msg.readInt16("atk"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::HOMUN_MATK,
        msg.readInt16("matk"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::HOMUN_HIT,
        msg.readInt16("hit"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::HOMUN_CRIT,
        msg.readInt16("luk/3 or crit/10"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::HOMUN_DEF,
        msg.readInt16("def"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::HOMUN_MDEF,
        msg.readInt16("mdef"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::HOMUN_FLEE,
        msg.readInt16("flee"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::HOMUN_ATTACK_DELAY,
        msg.readInt16("attack speed"),
        Notify_true);
    if (msg.getVersion() >= 20150513)
    {
        PlayerInfo::setStatBase(Attributes::HOMUN_HP,
            msg.readInt32("hp"),
            Notify_true);
        PlayerInfo::setStatBase(Attributes::HOMUN_MAX_HP,
            msg.readInt32("max hp"),
            Notify_true);
    }
    else
    {
        PlayerInfo::setStatBase(Attributes::HOMUN_HP,
            msg.readInt16("hp"),
            Notify_true);
        PlayerInfo::setStatBase(Attributes::HOMUN_MAX_HP,
            msg.readInt16("max hp"),
            Notify_true);
    }
    PlayerInfo::setStatBase(Attributes::HOMUN_MP,
        msg.readInt16("sp"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::HOMUN_MAX_MP,
        msg.readInt16("max sp"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::HOMUN_EXP,
        msg.readInt32("exp"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::HOMUN_EXP_NEEDED,
        msg.readInt32("next exp"),
        Notify_true);
    PlayerInfo::setStatBase(Attributes::HOMUN_SKILL_POINTS,
        msg.readInt16("skill points"),
        Notify_true);
    const int range = msg.readInt16("attack range");
    PlayerInfo::setStatBase(Attributes::HOMUN_ATTACK_RANGE,
        range,
        Notify_true);

    PlayerInfo::updateAttrs();
    HomunculusInfo *const info = PlayerInfo::getHomunculus();
    if (info == nullptr)
        return;
    Being *const dstBeing = actorManager->findBeing(info->id);

    info->name = name;
    info->level = level;
    info->range = range;
    info->hungry = hungry;
    info->intimacy = intimacy;
    info->equip = equip;
    PlayerInfo::setHomunculusBeing(dstBeing);
}

void HomunculusRecv::processHomunculusSkillUp(Net::MessageIn &msg)
{
    const int skillId = msg.readInt16("skill id");
    const int level = msg.readInt16("level");
    const int sp = msg.readInt16("sp");
    const int range = msg.readInt16("range");
    const Modifiable up = fromBool(msg.readUInt8("up flag"), Modifiable);

    if (skillDialog != nullptr && PlayerInfo::getSkillLevel(skillId) != level)
        skillDialog->playUpdateEffect(skillId);
    PlayerInfo::setSkillLevel(skillId, level);
    if (skillDialog != nullptr)
    {
        if (!skillDialog->updateSkill(skillId, range,
            up, SkillType::Unknown, sp))
        {
            skillDialog->addSkill(SkillOwner::Homunculus,
                skillId, "", level,
                range, up, SkillType::Unknown, sp);
        }
    }
}

void HomunculusRecv::processHomunculusFood(Net::MessageIn &msg)
{
    const int flag = msg.readUInt8("fail");
    const int itemId = msg.readInt16("food id");
    if (flag != 0)
    {
        NotifyManager::notify(NotifyTypes::HOMUNCULUS_FEED_OK);
    }
    else
    {
        const std::string name = strprintf("[@@%d|%s@@]", itemId,
            ItemDB::get(itemId).getName().c_str());
        NotifyManager::notify(NotifyTypes::HOMUNCULUS_FEED_FAIL, name);
    }
}

void HomunculusRecv::processHomunculusExp(Net::MessageIn &msg)
{
    const int exp = msg.readInt32("exp");
    msg.readInt32("unused");
    if (localPlayer != nullptr)
        localPlayer->addHomunXpMessage(exp);
}

}  // namespace EAthena
