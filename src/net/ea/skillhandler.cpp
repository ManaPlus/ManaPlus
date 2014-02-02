/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "net/ea/skillhandler.h"

#include "logger.h"
#include "notifications.h"
#include "notifymanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/windows/skilldialog.h"

#include "utils/stringutils.h"

#include "debug.h"

/** job dependend identifiers (?)  */
static const unsigned int SKILL_BASIC =       0x0001;
static const unsigned int SKILL_WARP =        0x001b;
static const unsigned int SKILL_STEAL =       0x0032;
static const unsigned int SKILL_ENVENOM =     0x0034;

/** basic skills identifiers       */
static const unsigned int BSKILL_TRADE =      0x0000;
static const unsigned int BSKILL_EMOTE =      0x0001;
static const unsigned int BSKILL_SIT =        0x0002;
static const unsigned int BSKILL_CREATECHAT = 0x0003;
static const unsigned int BSKILL_JOINPARTY =  0x0004;
static const unsigned int BSKILL_SHOUT =      0x0005;

/** reasons why action failed      */
static const unsigned int RFAIL_SKILLDEP =    0x00;
static const unsigned int RFAIL_INSUFSP =     0x01;
static const unsigned int RFAIL_INSUFHP =     0x02;
static const unsigned int RFAIL_NOMEMO =      0x03;
static const unsigned int RFAIL_SKILLDELAY =  0x04;
static const unsigned int RFAIL_ZENY =        0x05;
static const unsigned int RFAIL_WEAPON =      0x06;
static const unsigned int RFAIL_REDGEM =      0x07;
static const unsigned int RFAIL_BLUEGEM =     0x08;
static const unsigned int RFAIL_OVERWEIGHT =  0x09;

/** should always be zero if failed */
static const unsigned int SKILL_FAILED =      0x00;

namespace Ea
{

SkillHandler::SkillHandler()
{
}

void SkillHandler::processPlayerSkills(Net::MessageIn &msg)
{
    msg.readInt16();  // length
    const int skillCount = (msg.getLength() - 4) / 37;
    int updateSkill = 0;

    for (int k = 0; k < skillCount; k++)
    {
        const int skillId = msg.readInt16();
        msg.readInt16();  // target type
        msg.skip(2);  // skill pool flags
        const int level = msg.readInt16();
        msg.readInt16();  // sp
        const int range = msg.readInt16();
        msg.skip(24);  // 0 unused
        const int up = msg.readInt8();
        const int oldLevel = PlayerInfo::getSkillLevel(skillId);
        if (oldLevel && oldLevel != level)
            updateSkill = skillId;
        PlayerInfo::setSkillLevel(skillId, level);
        if (skillDialog)
        {
            if (!skillDialog->updateSkill(skillId, range, up))
                skillDialog->addSkill(skillId, level, range, up);
        }
    }
    if (updateSkill && skillDialog)
        skillDialog->playUpdateEffect(updateSkill);
}

void SkillHandler::processPlayerSkillUp(Net::MessageIn &msg)
{
    const int skillId = msg.readInt16();
    const int level = msg.readInt16();
    msg.readInt16();  // sp
    const int range = msg.readInt16();
    const int up = msg.readInt8();

    if (skillDialog && PlayerInfo::getSkillLevel(skillId) != level)
        skillDialog->playUpdateEffect(skillId);
    PlayerInfo::setSkillLevel(skillId, level);
    if (skillDialog)
    {
        if (!skillDialog->updateSkill(skillId, range, up))
            skillDialog->addSkill(skillId, level, range, up);
    }
}

void SkillHandler::processSkillFailed(Net::MessageIn &msg)
{
    // Action failed (ex. sit because you have not reached the
    // right level)
    const int skillId   = msg.readInt16();
    const int16_t bskill  = msg.readInt16();
    msg.readInt16();  // btype
    const signed char success = msg.readInt8();
    const signed char reason  = msg.readInt8();
    if (success != static_cast<int>(SKILL_FAILED)
        && bskill == static_cast<int>(BSKILL_EMOTE))
    {
        logger->log("Action: %d/%d", bskill, success);
    }

    std::string txt;
    if (success == static_cast<int>(SKILL_FAILED)
        && skillId == static_cast<int>(SKILL_BASIC))
    {
        if (player_node && bskill == static_cast<int>(BSKILL_EMOTE)
            && reason == static_cast<int>(RFAIL_SKILLDEP))
        {
            player_node->stopAdvert();
        }

        switch (bskill)
        {
            case BSKILL_TRADE:
                // TRANSLATORS: error message
                txt = _("Trade failed!");
                break;
            case BSKILL_EMOTE:
                // TRANSLATORS: error message
                txt = _("Emote failed!");
                break;
            case BSKILL_SIT:
                // TRANSLATORS: error message
                txt = _("Sit failed!");
                break;
            case BSKILL_CREATECHAT:
                // TRANSLATORS: error message
                txt = _("Chat creating failed!");
                break;
            case BSKILL_JOINPARTY:
                // TRANSLATORS: error message
                txt = _("Could not join party!");
                break;
            case BSKILL_SHOUT:
                // TRANSLATORS: error message
                txt = _("Cannot shout!");
                break;
            default:
                logger->log("QQQ SMSG_SKILL_FAILED: bskill "
                            + toString(bskill));
                break;
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
                // TRANSLATORS: error message
                txt.append(_("Huh? What's that?"));
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
                // TRANSLATORS: error message
                txt = _("Warp failed...");
                break;
            case SKILL_STEAL :
                // TRANSLATORS: error message
                txt = _("Could not steal anything...");
                break;
            case SKILL_ENVENOM :
                // TRANSLATORS: error message
                txt = _("Poison had no effect...");
                break;
            default:
                logger->log("QQQ SMSG_SKILL_FAILED: skillId "
                            + toString(skillId));
                break;
        }
    }

    NotifyManager::notify(NotifyManager::SKILL_FAIL_MESSAGE, txt);
}
}  // namespace Ea
