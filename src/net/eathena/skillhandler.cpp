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

#include "net/eathena/skillhandler.h"

#include "notifymanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/widgets/skillinfo.h"

#include "gui/windows/skilldialog.h"

#include "net/eathena/menu.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "resources/notifytypes.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

extern Net::SkillHandler *skillHandler;

namespace EAthena
{

SkillHandler::SkillHandler() :
    MessageHandler(),
    Ea::SkillHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_PLAYER_SKILLS,
        SMSG_SKILL_FAILED,
        SMSG_PLAYER_SKILL_UP,
        SMSG_PLAYER_SKILL_COOLDOWN,
        SMSG_PLAYER_SKILL_COOLDOWN_LIST,
        SMSG_SKILL_SNAP,
        SMSG_PLAYER_ADD_SKILL,
        SMSG_PLAYER_DELETE_SKILL,
        SMSG_PLAYER_UPDATE_SKILL,
        SMSG_SKILL_WARP_POINT,
        SMSG_SKILL_MEMO_MESSAGE,
        SMSG_PLAYER_SKILL_PRODUCE_MIX_LIST,
        SMSG_PLAYER_SKILL_PRODUCE_EFFECT,
        SMSG_SKILL_UNIT_UPDATE,
        SMSG_SKILL_ARROW_CREATE_LIST,
        SMSG_PLAYER_SKILL_AUTO_SPELLS,
        SMSG_SKILL_DEVOTION_EFFECT,
        SMSG_SKILL_ITEM_LIST_WINDOW,
        0
    };
    handledMessages = _messages;
    skillHandler = this;
}

void SkillHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_PLAYER_SKILLS:
            processPlayerSkills(msg);
            break;

        case SMSG_PLAYER_SKILL_UP:
            processPlayerSkillUp(msg);
            break;

        case SMSG_SKILL_FAILED:
            processSkillFailed(msg);
            break;

        case SMSG_PLAYER_SKILL_COOLDOWN:
            processSkillCoolDown(msg);
            break;

        case SMSG_PLAYER_SKILL_COOLDOWN_LIST:
            processSkillCoolDownList(msg);
            break;

        case SMSG_SKILL_SNAP:
            processSkillSnap(msg);
            break;

        case SMSG_PLAYER_ADD_SKILL:
            processSkillAdd(msg);
            break;

        case SMSG_PLAYER_UPDATE_SKILL:
            processSkillUpdate(msg);
            break;

        case SMSG_PLAYER_DELETE_SKILL:
            processSkillDelete(msg);
            break;

        case SMSG_SKILL_WARP_POINT:
            processSkillWarpPoint(msg);
            break;

        case SMSG_SKILL_MEMO_MESSAGE:
            processSkillMemoMessage(msg);
            break;

        case SMSG_PLAYER_SKILL_PRODUCE_MIX_LIST:
            processSkillProduceMixList(msg);
            break;

        case SMSG_PLAYER_SKILL_PRODUCE_EFFECT:
            processSkillProduceEffect(msg);
            break;

        case SMSG_SKILL_UNIT_UPDATE:
            processSkillUnitUpdate(msg);
            break;

        case SMSG_SKILL_ARROW_CREATE_LIST:
            processSkillArrowCreateList(msg);
            break;

        case SMSG_PLAYER_SKILL_AUTO_SPELLS:
            processSkillAutoSpells(msg);
            break;

        case SMSG_SKILL_DEVOTION_EFFECT:
            processSkillDevotionEffect(msg);
            break;

        case SMSG_SKILL_ITEM_LIST_WINDOW:
            processSkillItemListWindow(msg);
            break;

        default:
            break;
    }
}

void SkillHandler::useBeing(const int id, const int level,
                            const BeingId beingId) const
{
    createOutPacket(CMSG_SKILL_USE_BEING);
    outMsg.writeInt16(static_cast<int16_t>(level), "skill level");
    outMsg.writeInt16(static_cast<int16_t>(id), "skill id");
    outMsg.writeInt32(toInt(beingId, int), "target id");
}

void SkillHandler::usePos(const int id, const int level,
                          const int x, const int y) const
{
    createOutPacket(CMSG_SKILL_USE_POSITION);
    outMsg.writeInt16(static_cast<int16_t>(level), "skill level");
    outMsg.writeInt16(static_cast<int16_t>(id), "skill id");
    outMsg.writeInt16(static_cast<int16_t>(x), "x");
    outMsg.writeInt16(static_cast<int16_t>(y), "y");
}

void SkillHandler::usePos(const int id, const int level,
                          const int x, const int y,
                          const std::string &text) const
{
    createOutPacket(CMSG_SKILL_USE_POSITION_MORE);
    outMsg.writeInt16(static_cast<int16_t>(level), "level");
    outMsg.writeInt16(static_cast<int16_t>(id), "id");
    outMsg.writeInt16(static_cast<int16_t>(x), "x");
    outMsg.writeInt16(static_cast<int16_t>(y), "y");
    outMsg.writeString(text, 80, "text");
}

void SkillHandler::useMap(const int id, const std::string &map) const
{
    createOutPacket(CMSG_SKILL_USE_MAP);
    outMsg.writeInt16(static_cast<int16_t>(id), "skill id");
    outMsg.writeString(map, 16, "map name");
}

void SkillHandler::processPlayerSkills(Net::MessageIn &msg)
{
    msg.readInt16("len");
    const int skillCount = (msg.getLength() - 4) / 37;
    int updateSkill = 0;

    if (skillDialog)
        skillDialog->hideSkills(SkillOwner::Player);
    for (int k = 0; k < skillCount; k++)
    {
        const int skillId = msg.readInt16("skill id");
        const SkillType::SkillType inf = static_cast<SkillType::SkillType>(
            msg.readInt32("inf"));
        const int level = msg.readInt16("skill level");
        const int sp = msg.readInt16("sp");
        const int range = msg.readInt16("range");
        const std::string name = msg.readString(24, "skill name");
        const Modifiable up = fromBool(msg.readUInt8("up flag"), Modifiable);
        const int oldLevel = PlayerInfo::getSkillLevel(skillId);
        if (oldLevel && oldLevel != level)
            updateSkill = skillId;
        PlayerInfo::setSkillLevel(skillId, level);
        if (skillDialog)
        {
            if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
            {
                skillDialog->addSkill(SkillOwner::Player,
                    skillId, name, level, range, up, inf, sp);
            }
        }
    }
    if (skillDialog)
    {
        skillDialog->update();
        if (updateSkill)
            skillDialog->playUpdateEffect(updateSkill);
    }
}

void SkillHandler::processSkillAdd(Net::MessageIn &msg)
{
    int updateSkill = 0;
    const int skillId = msg.readInt16("skill id");
    const SkillType::SkillType inf = static_cast<SkillType::SkillType>(
        msg.readInt32("inf"));
    const int level = msg.readInt16("skill level");
    const int sp = msg.readInt16("sp");
    const int range = msg.readInt16("range");
    const std::string name = msg.readString(24, "skill name");
    const Modifiable up = fromBool(msg.readUInt8("up flag"), Modifiable);
    const int oldLevel = PlayerInfo::getSkillLevel(skillId);
    if (oldLevel && oldLevel != level)
        updateSkill = skillId;
    PlayerInfo::setSkillLevel(skillId, level);
    if (skillDialog)
    {
        if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
        {
            skillDialog->addSkill(SkillOwner::Player,
                skillId, name, level, range, up, inf, sp);
        }
        skillDialog->update();
        if (updateSkill)
            skillDialog->playUpdateEffect(updateSkill);
    }
}

void SkillHandler::processSkillUpdate(Net::MessageIn &msg)
{
    int updateSkill = 0;
    const int skillId = msg.readInt16("skill id");
    const SkillType::SkillType inf = static_cast<SkillType::SkillType>(
        msg.readInt32("inf"));
    const int level = msg.readInt16("skill level");
    const int sp = msg.readInt16("sp");
    const int range = msg.readInt16("range");
    const Modifiable up = fromBool(msg.readUInt8("up flag"), Modifiable);
    const int oldLevel = PlayerInfo::getSkillLevel(skillId);
    if (oldLevel && oldLevel != level)
        updateSkill = skillId;
    PlayerInfo::setSkillLevel(skillId, level);
    if (skillDialog)
    {
        if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
        {
            skillDialog->addSkill(SkillOwner::Player,
                skillId, "", level, range, up, inf, sp);
        }
        skillDialog->update();
        if (updateSkill)
            skillDialog->playUpdateEffect(updateSkill);
    }
}

void SkillHandler::processSkillDelete(Net::MessageIn &msg)
{
    int updateSkill = 0;
    const int skillId = msg.readInt16("skill id");
    const int oldLevel = PlayerInfo::getSkillLevel(skillId);
    if (oldLevel != 0)
        updateSkill = skillId;
    PlayerInfo::setSkillLevel(skillId, 0);
    if (skillDialog)
    {
        skillDialog->removeSkill(skillId);
        skillDialog->update();
        if (updateSkill)
            skillDialog->playRemoveEffect(updateSkill);
    }
}

void SkillHandler::processSkillCoolDown(Net::MessageIn &msg)
{
    const int skillId = msg.readInt16("skill id");
    const int duration = msg.readInt32("duration");
    if (skillDialog)
        skillDialog->setSkillDuration(SkillOwner::Player, skillId, duration);
}

void SkillHandler::processSkillCoolDownList(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 4) / 10;
    for (int f = 0; f < count; f ++)
    {
        const int skillId = msg.readInt16("skill id");
        msg.readInt32("total");
        const int duration = msg.readInt32("duration");
        if (skillDialog)
        {
            skillDialog->setSkillDuration(SkillOwner::Player,
                skillId, duration);
        }
    }
}

void SkillHandler::processSkillFailed(Net::MessageIn &msg)
{
    // Action failed (ex. sit because you have not reached the
    // right level)
    const int skillId = msg.readInt16("skill id");
    const int bskill  = msg.readInt32("btype");
    const signed char success = msg.readUInt8("success");
    const signed char reason  = msg.readUInt8("reason");
    if (success != static_cast<int>(SKILL_FAILED)
        && bskill == static_cast<int>(BSKILL_EMOTE))
    {
        logger->log("Action: %d/%d", bskill, success);
    }

    std::string txt;
    if (success == static_cast<int>(SKILL_FAILED) && bskill != 0)
    {
        if (localPlayer && bskill == static_cast<int>(BSKILL_EMOTE)
            && reason == static_cast<int>(RFAIL_SKILLDEP))
        {
            localPlayer->stopAdvert();
        }

        SkillInfo *const info = skillDialog->getSkill(bskill);
        if (info)
            txt = info->errorText;
        else
            txt = strprintf(_("Unknown skill error: %d"), bskill);
    }
    else
    {
        SkillInfo *const info = skillDialog->getSkill(skillId);
        if (info)
            txt = info->errorText + ".";
        else
            txt = strprintf(_("Unknown skill error: %d."), skillId);
    }

    txt.append(" ");
    switch (reason)
    {
        case RFAIL_SKILLDEP:
            // TRANSLATORS: error message
            txt.append(_("You have not yet reached a high enough lvl!"));
            break;
        case RFAIL_INSUFHP:
            // TRANSLATORS: error message
            txt.append(_("Insufficient HP!"));
            break;
        case RFAIL_INSUFSP:
            // TRANSLATORS: error message
            txt.append(_("Insufficient SP!"));
            break;
        case RFAIL_NOMEMO:
            // TRANSLATORS: error message
            txt.append(_("You have no memos!"));
            break;
        case RFAIL_SKILLDELAY:
            // TRANSLATORS: error message
            txt.append(_("You cannot do that right now!"));
            break;
        case RFAIL_ZENY:
            // TRANSLATORS: error message
            txt.append(_("Seems you need more money... ;-)"));
            break;
        case RFAIL_WEAPON:
            // TRANSLATORS: error message
            txt.append(_("You cannot use this skill with that "
                "kind of weapon!"));
            break;
        case RFAIL_REDGEM:
            // TRANSLATORS: error message
            txt.append(_("You need another red gem!"));
            break;
        case RFAIL_BLUEGEM:
            // TRANSLATORS: error message
            txt.append(_("You need another blue gem!"));
            break;
        case RFAIL_OVERWEIGHT:
                // TRANSLATORS: error message
            txt.append(_("You're carrying to much to do this!"));
            break;
        default:
            UNIMPLIMENTEDPACKET;
            break;
    }

    NotifyManager::notify(NotifyTypes::SKILL_FAIL_MESSAGE, txt);
}

void SkillHandler::processSkillSnap(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readBeingId("being id");
    msg.readInt16("x");
    msg.readInt16("y");
}

void SkillHandler::processSkillWarpPoint(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("skill id");
    msg.readString(16, "map name 1");
    msg.readString(16, "map name 2");
    msg.readString(16, "map name 3");
    msg.readString(16, "map name 4");
}

void SkillHandler::processSkillMemoMessage(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readUInt8("type");
}

void SkillHandler::processSkillProduceMixList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    const int count = (msg.readInt16("len") - 8) / 8;
    for (int f = 0; f < count; f ++)
    {
        msg.readInt16("item id");
        for (int d = 0; d < 3; d ++)
            msg.readInt16("material id");
    }
}

void SkillHandler::processSkillProduceEffect(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt16("flag");
    msg.readInt16("item id");
}

void SkillHandler::processSkillUnitUpdate(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readBeingId("being id");
}

void SkillHandler::processSkillArrowCreateList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    const int count = (msg.readInt16("len") - 4) / 2;
    for (int f = 0; f < count; f ++)
        msg.readInt16("item id");
}

void SkillHandler::processSkillAutoSpells(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    for (int f = 0; f < 7; f ++)
        msg.readInt32("skill id");

    menu = MenuType::AutoSpell;
}

void SkillHandler::processSkillDevotionEffect(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readBeingId("being id");
    for (int f = 0; f < 5; f ++)
        msg.readInt32("devotee id");
    msg.readInt16("range");
}

void SkillHandler::processSkillItemListWindow(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt32("skill level");
    msg.readInt32("unused");
}

}  // namespace EAthena
