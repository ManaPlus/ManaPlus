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

#include "enums/resources/notifytypes.h"

#include "gui/widgets/skillinfo.h"

#include "gui/windows/skilldialog.h"

#include "net/ea/skillrecv.h"

#include "net/eathena/menu.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"
#include "net/eathena/skillrecv.h"

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
            SkillRecv::processPlayerSkills(msg);
            break;

        case SMSG_PLAYER_SKILL_UP:
            Ea::SkillRecv::processPlayerSkillUp(msg);
            break;

        case SMSG_SKILL_FAILED:
            SkillRecv::processSkillFailed(msg);
            break;

        case SMSG_PLAYER_SKILL_COOLDOWN:
            SkillRecv::processSkillCoolDown(msg);
            break;

        case SMSG_PLAYER_SKILL_COOLDOWN_LIST:
            SkillRecv::processSkillCoolDownList(msg);
            break;

        case SMSG_SKILL_SNAP:
            SkillRecv::processSkillSnap(msg);
            break;

        case SMSG_PLAYER_ADD_SKILL:
            SkillRecv::processSkillAdd(msg);
            break;

        case SMSG_PLAYER_UPDATE_SKILL:
            SkillRecv::processSkillUpdate(msg);
            break;

        case SMSG_PLAYER_DELETE_SKILL:
            SkillRecv::processSkillDelete(msg);
            break;

        case SMSG_SKILL_WARP_POINT:
            SkillRecv::processSkillWarpPoint(msg);
            break;

        case SMSG_SKILL_MEMO_MESSAGE:
            SkillRecv::processSkillMemoMessage(msg);
            break;

        case SMSG_PLAYER_SKILL_PRODUCE_MIX_LIST:
            SkillRecv::processSkillProduceMixList(msg);
            break;

        case SMSG_PLAYER_SKILL_PRODUCE_EFFECT:
            SkillRecv::processSkillProduceEffect(msg);
            break;

        case SMSG_SKILL_UNIT_UPDATE:
            SkillRecv::processSkillUnitUpdate(msg);
            break;

        case SMSG_SKILL_ARROW_CREATE_LIST:
            SkillRecv::processSkillArrowCreateList(msg);
            break;

        case SMSG_PLAYER_SKILL_AUTO_SPELLS:
            SkillRecv::processSkillAutoSpells(msg);
            break;

        case SMSG_SKILL_DEVOTION_EFFECT:
            SkillRecv::processSkillDevotionEffect(msg);
            break;

        case SMSG_SKILL_ITEM_LIST_WINDOW:
            SkillRecv::processSkillItemListWindow(msg);
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

}  // namespace EAthena
