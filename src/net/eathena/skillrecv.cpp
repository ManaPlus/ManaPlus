/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "net/eathena/skillrecv.h"

#include "notifymanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "const/net/skill.h"

#include "enums/resources/notifytypes.h"

#include "gui/widgets/createwidget.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/textselectdialog.h"

#include "listeners/skillwarplistener.h"

#include "net/messagein.h"

#include "net/eathena/menu.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "resources/skill/skillinfo.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

static const unsigned int RFAIL                = 10;
static const unsigned int RFAIL_SUMMON         = 19;
static const unsigned int RFAIL_NEED_ITEM      = 71;
static const unsigned int RFAIL_NEED_EQUIPMENT = 72;
static const unsigned int RFAIL_SPIRITS        = 74;

extern int serverVersion;
extern int itemIdLen;

namespace EAthena
{

void SkillRecv::processPlayerSkills(Net::MessageIn &msg)
{
    msg.readInt16("len");
    const int sz = (serverVersion > 0) ? 41 : 37;
    const int skillCount = (msg.getLength() - 4) / sz;
    int updateSkill = 0;

    if (skillDialog != nullptr)
        skillDialog->hideSkills(SkillOwner::Player);
    for (int k = 0; k < skillCount; k++)
    {
        const int skillId = msg.readInt16("skill id");
        const SkillType::SkillType inf = static_cast<SkillType::SkillType>(
            msg.readInt32("inf"));
        if (serverVersion > 0)
            msg.readInt32("inf2");
        const int level = msg.readInt16("skill level");
        const int sp = msg.readInt16("sp");
        const int range = msg.readInt16("range");
        const std::string name = msg.readString(24, "skill name");
        const Modifiable up = fromBool(msg.readUInt8("up flag"), Modifiable);
        const int oldLevel = PlayerInfo::getSkillLevel(skillId);
        if ((oldLevel != 0) && oldLevel != level)
            updateSkill = skillId;
        PlayerInfo::setSkillLevel(skillId, level);
        if (skillDialog != nullptr)
        {
            if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
            {
                skillDialog->addSkill(SkillOwner::Player,
                    skillId, name, level, range, up, inf, sp);
            }
        }
    }
    if (skillDialog != nullptr)
    {
        skillDialog->update();
        skillDialog->updateModelsHidden();
        if (updateSkill != 0)
            skillDialog->playUpdateEffect(updateSkill);
    }
}

void SkillRecv::processSkillAdd(Net::MessageIn &msg)
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
    if ((oldLevel != 0) && oldLevel != level)
        updateSkill = skillId;
    PlayerInfo::setSkillLevel(skillId, level);
    if (skillDialog != nullptr)
    {
        if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
        {
            skillDialog->addSkill(SkillOwner::Player,
                skillId, name, level, range, up, inf, sp);
        }
        skillDialog->update();
        if (updateSkill != 0)
            skillDialog->playUpdateEffect(updateSkill);
    }
}

void SkillRecv::processSkillAdd2(Net::MessageIn &msg)
{
    int updateSkill = 0;
    msg.readInt16("len");  // for now unused
    const int skillId = msg.readInt16("skill id");
    const SkillType::SkillType inf = static_cast<SkillType::SkillType>(
        msg.readInt32("inf"));
    msg.readInt32("inf2");
    const int level = msg.readInt16("skill level");
    const int sp = msg.readInt16("sp");
    const int range = msg.readInt16("range");
    const std::string name = msg.readString(24, "skill name");
    const Modifiable up = fromBool(msg.readUInt8("up flag"), Modifiable);
    const int oldLevel = PlayerInfo::getSkillLevel(skillId);
    if ((oldLevel != 0) && oldLevel != level)
        updateSkill = skillId;
    PlayerInfo::setSkillLevel(skillId, level);
    if (skillDialog != nullptr)
    {
        if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
        {
            skillDialog->addSkill(SkillOwner::Player,
                skillId, name, level, range, up, inf, sp);
        }
        skillDialog->update();
        if (updateSkill != 0)
            skillDialog->playUpdateEffect(updateSkill);
    }
}

void SkillRecv::processSkillUpdate(Net::MessageIn &msg)
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
    if ((oldLevel != 0) && oldLevel != level)
        updateSkill = skillId;
    PlayerInfo::setSkillLevel(skillId, level);
    if (skillDialog != nullptr)
    {
        if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
        {
            skillDialog->addSkill(SkillOwner::Player,
                skillId, "", level, range, up, inf, sp);
        }
        skillDialog->update();
        if (updateSkill != 0)
            skillDialog->playUpdateEffect(updateSkill);
    }
}

void SkillRecv::processSkillUpdate2(Net::MessageIn &msg)
{
    int updateSkill = 0;
    msg.readInt16("len");  // for now unused
    const int skillId = msg.readInt16("skill id");
    const SkillType::SkillType inf = static_cast<SkillType::SkillType>(
        msg.readInt32("inf"));
    msg.readInt32("inf2");
    const int level = msg.readInt16("skill level");
    const int sp = msg.readInt16("sp");
    const int range = msg.readInt16("range");
    const Modifiable up = fromBool(msg.readUInt8("up flag"), Modifiable);
    const int oldLevel = PlayerInfo::getSkillLevel(skillId);
    if ((oldLevel != 0) && oldLevel != level)
        updateSkill = skillId;
    PlayerInfo::setSkillLevel(skillId, level);
    if (skillDialog != nullptr)
    {
        if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
        {
            skillDialog->addSkill(SkillOwner::Player,
                skillId, "", level, range, up, inf, sp);
        }
        skillDialog->update();
        if (updateSkill != 0)
            skillDialog->playUpdateEffect(updateSkill);
    }
}

void SkillRecv::processSkillDelete(Net::MessageIn &msg)
{
    int updateSkill = 0;
    const int skillId = msg.readInt16("skill id");
    const int oldLevel = PlayerInfo::getSkillLevel(skillId);
    if (oldLevel != 0)
        updateSkill = skillId;
    PlayerInfo::setSkillLevel(skillId, 0);
    if (skillDialog != nullptr)
    {
        skillDialog->removeSkill(skillId);
        skillDialog->update();
        if (updateSkill != 0)
            skillDialog->playRemoveEffect(updateSkill);
    }
}

void SkillRecv::processSkillCoolDown(Net::MessageIn &msg)
{
    const int skillId = msg.readInt16("skill id");
    const int duration = msg.readInt32("duration");
    if (skillDialog != nullptr)
        skillDialog->setSkillDuration(SkillOwner::Player, skillId, duration);
}

void SkillRecv::processSkillCoolDownList(Net::MessageIn &msg)
{
    int packetLen;
    if (msg.getVersion() >= 20120604)
        packetLen = 10;
    else
        packetLen = 6;
    const int count = (msg.readInt16("len") - 4) / packetLen;
    for (int f = 0; f < count; f ++)
    {
        const int skillId = msg.readInt16("skill id");
        if (msg.getVersion() >= 20120604)
            msg.readInt32("total");
        const int duration = msg.readInt32("duration");
        if (skillDialog != nullptr)
        {
            skillDialog->setSkillDuration(SkillOwner::Player,
                skillId, duration);
        }
    }
}

void SkillRecv::processSkillFailed(Net::MessageIn &msg)
{
    // Action failed (ex. sit because you have not reached the
    // right level)
    const int skillId = msg.readInt16("skill id");
    const int bskill  = msg.readItemId("btype");
    const int itemId  = msg.readItemId("item id");
    const signed char success = msg.readUInt8("success");
    const signed char reason  = msg.readUInt8("reason");
    if (success != CAST_S32(SKILL_FAILED)
        && bskill == CAST_S32(BSKILL_EMOTE))
    {
        logger->log("Action: %d/%d", bskill, success);
    }

    if (localPlayer != nullptr)
        localPlayer->stopCast(true);
    std::string txt;
    if (success == CAST_S32(SKILL_FAILED) && bskill != 0)
    {
        if ((localPlayer != nullptr) && bskill == CAST_S32(BSKILL_EMOTE)
            && reason == CAST_S32(RFAIL_SKILLDEP))
        {
            localPlayer->stopAdvert();
        }

        const SkillInfo *const info = skillDialog->getSkill(bskill);
        if (info != nullptr)
        {
            txt = info->errorText;
        }
        else
        {
            // TRANSLATORS: skill error message
            txt = strprintf(_("Unknown skill error: %d"), bskill);
        }
    }
    else
    {
        const SkillInfo *const info = skillDialog->getSkill(skillId);
        if (info != nullptr)
        {
            txt = info->errorText + ".";
        }
        else
        {
            // TRANSLATORS: skill error message
            txt = strprintf(_("Unknown skill error: %d."), skillId);
        }
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
        case RFAIL_SUMMON:
            // TRANSLATORS: error message
            txt.append(_("Fail summon."));
            break;
        case RFAIL_SPIRITS:
            // TRANSLATORS: error message
            txt.append(_("Need spirits."));
            break;
        case RFAIL_NEED_EQUIPMENT:
        {
            const int amount = bskill;
            const ItemInfo &info = ItemDB::get(itemId);
            if (amount == 1)
            {
                // TRANSLATORS: skill error message
                txt.append(strprintf(_("Need equipment %s."),
                    info.getLink().c_str()));
            }
            else
            {
                // TRANSLATORS: skill error message
                txt.append(strprintf(_("Need equipment %s and amount %d"),
                    info.getLink().c_str(),
                    amount));
            }
            break;
        }
        case RFAIL_NEED_ITEM:
        {
            const int amount = bskill;
            const ItemInfo &info = ItemDB::get(itemId);
            if (amount == 1)
            {
                // TRANSLATORS: skill error message
                txt.append(strprintf(_("Need item %s."),
                    info.getLink().c_str()));
            }
            else
            {
                // TRANSLATORS: skill error message
                txt.append(strprintf(_("Need item %s and amount %d"),
                    info.getLink().c_str(),
                    amount));
            }
            break;
        }
        case RFAIL:
        {
            // TRANSLATORS: error message
            txt.append(_("Skill failed!"));
            break;
        }

        default:
            UNIMPLEMENTEDPACKETFIELD(reason);
            break;
    }
    if (debugChatTab != nullptr)
        debugChatTab->chatLog("SKILL: "
            + txt,
            ChatMsgType::BY_SERVER,
            IgnoreRecord_false,
            TryRemoveColors_true);
}

void SkillRecv::processSkillWarpPoint(Net::MessageIn &msg)
{
    const int skillId = msg.readInt16("skill id");

    TextSelectDialog *const dialog = CREATEWIDGETR(TextSelectDialog,
        // TRANSLATORS: warp select window name
        _("Select warp target"),
        // TRANSLATORS: warp select button
        _("Warp"),
        AllowQuit_false);
    skillWarpListener.setDialog(dialog);
    skillWarpListener.setSkill(skillId);
    dialog->addActionListener(&skillWarpListener);
    for (int f = 0; f < 4; f ++)
        dialog->addText(msg.readString(16, "map name"));
}

void SkillRecv::processSkillWarpPoint2(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 6) / 16;
    const int skillId = msg.readInt16("skill id");

    TextSelectDialog *const dialog = CREATEWIDGETR(TextSelectDialog,
        // TRANSLATORS: warp select window name
        _("Select warp target"),
        // TRANSLATORS: warp select button
        _("Warp"),
        AllowQuit_false);
    skillWarpListener.setDialog(dialog);
    skillWarpListener.setSkill(skillId);
    dialog->addActionListener(&skillWarpListener);
    for (int f = 0; f < count; f ++)
        dialog->addText(msg.readString(16, "map name"));
}

void SkillRecv::processSkillMemoMessage(Net::MessageIn &msg)
{
    const int type = msg.readUInt8("type");
    switch (type)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::SKILL_MEMO_SAVED);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::SKILL_MEMO_ERROR_LEVEL);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::SKILL_MEMO_ERROR_SKILL);
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(type);
            break;
    }
}

void SkillRecv::processSkillProduceMixList(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    const int count = (msg.readInt16("len") - 4) / 4 * itemIdLen;
    for (int f = 0; f < count; f ++)
    {
        msg.readItemId("item id");
        for (int d = 0; d < 3; d ++)
            msg.readItemId("material id");
    }
}

void SkillRecv::processSkillProduceEffect(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    msg.readInt16("flag");
    msg.readItemId("item id");
}

void SkillRecv::processSkillUnitUpdate(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    msg.readBeingId("being id");
}

void SkillRecv::processSkillArrowCreateList(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    const int count = (msg.readInt16("len") - 4) / itemIdLen;
    for (int f = 0; f < count; f ++)
        msg.readItemId("item id");
}

void SkillRecv::processSkillAutoSpells1(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    for (int f = 0; f < 7; f ++)
        msg.readInt32("skill id");

    menu = MenuType::AutoSpell;
}

void SkillRecv::processSkillAutoSpells2(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    const int count = (msg.readInt16("len") - 4) / 4;
    for (int f = 0; f < count; f ++)
        msg.readInt32("skill id");

    menu = MenuType::AutoSpell;
}

void SkillRecv::processSkillDevotionEffect(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    msg.readBeingId("being id");
    for (int f = 0; f < 5; f ++)
        msg.readInt32("devotee id");
    msg.readInt16("range");
}

void SkillRecv::processSkillItemListWindow(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    msg.readInt32("skill level");
    msg.readInt32("unused");
}

void SkillRecv::processSkillScale(Net::MessageIn &msg)
{
    msg.readBeingId("being id");
    msg.readInt16("skill id");
    msg.readInt16("skill level");
    msg.readInt16("x");
    msg.readInt16("y");
    msg.readInt32("cast time");
}

}  // namespace EAthena
