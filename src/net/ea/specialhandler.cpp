/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "net/ea/specialhandler.h"

#include "logger.h"
#include "localplayer.h"
#include "playerinfo.h"

#include "gui/skilldialog.h"

#include "gui/widgets/chattab.h"

#include "net/messageout.h"

#include "utils/gettext.h"

#include "debug.h"

/** job dependend identifiers (?)  */
#define SKILL_BASIC       0x0001
#define SKILL_WARP        0x001b
#define SKILL_STEAL       0x0032
#define SKILL_ENVENOM     0x0034

/** basic skills identifiers       */
#define BSKILL_TRADE      0x0000
#define BSKILL_EMOTE      0x0001
#define BSKILL_SIT        0x0002
#define BSKILL_CREATECHAT 0x0003
#define BSKILL_JOINPARTY  0x0004
#define BSKILL_SHOUT      0x0005

/** reasons why action failed      */
#define RFAIL_SKILLDEP    0x00
#define RFAIL_INSUFSP     0x01
#define RFAIL_INSUFHP     0x02
#define RFAIL_NOMEMO      0x03
#define RFAIL_SKILLDELAY  0x04
#define RFAIL_ZENY        0x05
#define RFAIL_WEAPON      0x06
#define RFAIL_REDGEM      0x07
#define RFAIL_BLUEGEM     0x08
#define RFAIL_OVERWEIGHT  0x09

/** should always be zero if failed */
#define SKILL_FAILED      0x00

namespace Ea
{

SpecialHandler::SpecialHandler()
{
}

void SpecialHandler::use(int id A_UNUSED)
{
    // TODO
}

void SpecialHandler::processPlayerSkills(Net::MessageIn &msg)
{
    msg.readInt16();  // length
    const int skillCount = (msg.getLength() - 4) / 37;

    for (int k = 0; k < skillCount; k++)
    {
        int skillId = msg.readInt16();
        msg.readInt16();  // target type
        msg.skip(2);  // skill pool flags
        int level = msg.readInt16();
        msg.readInt16(); // sp
        int range = msg.readInt16();
        msg.skip(24); // 0 unused
        int up = msg.readInt8();

        PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
            skillId), level);
        if (skillDialog)
        {
            if (!skillDialog->updateSkill(skillId, range, up))
                skillDialog->addSkill(skillId, level, range, up);
        }
    }
}

void SpecialHandler::processPlayerSkillUp(Net::MessageIn &msg)
{
    const int skillId = msg.readInt16();
    const int level = msg.readInt16();
    msg.readInt16(); // sp
    int range = msg.readInt16();
    int up = msg.readInt8();

    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
        skillId), level);
    if (skillDialog)
    {
        if (!skillDialog->updateSkill(skillId, range, up))
            skillDialog->addSkill(skillId, level, range, up);
    }
}

void SpecialHandler::processSkillFailed(Net::MessageIn &msg)
{
    // Action failed (ex. sit because you have not reached the
    // right level)
    const int skillId   = msg.readInt16();
    const short bskill  = msg.readInt16();
    msg.readInt16(); // btype
    char success = msg.readInt8();
    char reason  = msg.readInt8();
    if (success != SKILL_FAILED && bskill == BSKILL_EMOTE)
        logger->log("Action: %d/%d", bskill, success);

    std::string txt;
    if (success == SKILL_FAILED && skillId == SKILL_BASIC)
    {
        if (player_node && bskill == BSKILL_EMOTE && reason == RFAIL_SKILLDEP)
            player_node->stopAdvert();

        switch (bskill)
        {
            case BSKILL_TRADE:
                txt = _("Trade failed!");
                break;
            case BSKILL_EMOTE:
                txt = _("Emote failed!");
                break;
            case BSKILL_SIT:
                txt = _("Sit failed!");
                break;
            case BSKILL_CREATECHAT:
                txt = _("Chat creating failed!");
                break;
            case BSKILL_JOINPARTY:
                txt = _("Could not join party!");
                break;
            case BSKILL_SHOUT:
                txt = _("Cannot shout!");
                break;
            default:
                logger->log("QQQ SMSG_SKILL_FAILED: bskill "
                            + toString(bskill));
                break;
        }

        txt += " ";

        switch (reason)
        {
            case RFAIL_SKILLDEP:
                txt += _("You have not yet reached a high enough lvl!");
                break;
            case RFAIL_INSUFHP:
                txt += _("Insufficient HP!");
                break;
            case RFAIL_INSUFSP:
                txt += _("Insufficient SP!");
                break;
            case RFAIL_NOMEMO:
                txt += _("You have no memos!");
                break;
            case RFAIL_SKILLDELAY:
                txt += _("You cannot do that right now!");
                break;
            case RFAIL_ZENY:
                txt += _("Seems you need more money... ;-)");
                break;
            case RFAIL_WEAPON:
                txt += _("You cannot use this skill with that "
                    "kind of weapon!");
                break;
            case RFAIL_REDGEM:
                txt += _("You need another red gem!");
                break;
            case RFAIL_BLUEGEM:
                txt += _("You need another blue gem!");
                break;
            case RFAIL_OVERWEIGHT:
                txt += _("You're carrying to much to do this!");
                break;
            default:
                txt += _("Huh? What's that?");
                logger->log("QQQ SMSG_SKILL_FAILED: reason "
                            + toString(reason));
                break;
        }
    }
    else
    {
        switch (skillId)
        {
            case SKILL_WARP :
                txt = _("Warp failed...");
                break;
            case SKILL_STEAL :
                txt = _("Could not steal anything...");
                break;
            case SKILL_ENVENOM :
                txt = _("Poison had no effect...");
                break;
            default:
                logger->log("QQQ SMSG_SKILL_FAILED: skillId "
                            + toString(skillId));
                break;
        }
    }

    if (localChatTab)
        localChatTab->chatLog(txt);
}
} // namespace Ea
