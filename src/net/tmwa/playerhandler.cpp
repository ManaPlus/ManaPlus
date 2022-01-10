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

#include "net/tmwa/playerhandler.h"

#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "const/net/nostat.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/statuswindow.h"

#include "net/ea/inventoryhandler.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocolout.h"
#include "net/tmwa/sp.h"

#include "resources/db/unitsdb.h"

#include "debug.h"

namespace TmwAthena
{

PlayerHandler::PlayerHandler() :
    Ea::PlayerHandler()
{
    playerHandler = this;
}

PlayerHandler::~PlayerHandler()
{
    playerHandler = nullptr;
}

void PlayerHandler::attack(const BeingId id,
                           const Keep keep) const
{
    createOutPacket(CMSG_PLAYER_CHANGE_ACT);
    outMsg.writeBeingId(id, "target id");
    if (keep == Keep_true)
        outMsg.writeInt8(7, "action");
    else
        outMsg.writeInt8(0, "action");
}

void PlayerHandler::stopAttack() const
{
    createOutPacket(CMSG_PLAYER_STOP_ATTACK);
}

void PlayerHandler::emote(const uint8_t emoteId) const
{
    createOutPacket(CMSG_PLAYER_EMOTE);
    outMsg.writeInt8(emoteId, "emote id");
}

void PlayerHandler::increaseAttribute(const AttributesT attr,
                                      const int amount A_UNUSED) const
{
    if (attr >= Attributes::PLAYER_STR && attr <= Attributes::PLAYER_LUK)
    {
        createOutPacket(CMSG_STAT_UPDATE_REQUEST);
        outMsg.writeInt16(CAST_S16(attr), "attribute id");
        outMsg.writeInt8(1, "increment");
    }
}

void PlayerHandler::increaseSkill(const uint16_t skillId) const
{
    if (PlayerInfo::getAttribute(Attributes::PLAYER_SKILL_POINTS) <= 0)
        return;

    createOutPacket(CMSG_SKILL_LEVELUP_REQUEST);
    outMsg.writeInt16(skillId, "skill id");
}

void PlayerHandler::pickUp(const FloorItem *const floorItem) const
{
    if (floorItem == nullptr)
        return;

    createOutPacket(CMSG_ITEM_PICKUP);
    outMsg.writeBeingId(floorItem->getId(), "object id");
    Ea::InventoryHandler::pushPickup(floorItem->getId());
}

void PlayerHandler::setDirection(const unsigned char direction) const
{
    createOutPacket(CMSG_PLAYER_CHANGE_DIR);
    outMsg.writeInt16(0, "unused");
    outMsg.writeInt8(direction, "direction");
}

void PlayerHandler::setDestination(const int x, const int y,
                                   const int direction) const
{
    createOutPacket(CMSG_PLAYER_CHANGE_DEST);
    outMsg.writeCoordinates(CAST_U16(x),
        CAST_U16(y),
        CAST_U8(direction), "destination");
}

void PlayerHandler::changeAction(const BeingActionT &action) const
{
    char type;
    switch (action)
    {
        case BeingAction::SIT:
            type = 2;
            break;
        case BeingAction::STAND:
        case BeingAction::PRESTAND:
            type = 3;
            break;
        default:
        case BeingAction::MOVE:
        case BeingAction::ATTACK:
        case BeingAction::DEAD:
        case BeingAction::HURT:
        case BeingAction::SPAWN:
        case BeingAction::CAST:
            return;
    }

    createOutPacket(CMSG_PLAYER_CHANGE_ACT);
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt8(type, "action");
}

void PlayerHandler::respawn() const
{
    createOutPacket(CMSG_PLAYER_RESTART);
    outMsg.writeInt8(0, "action");
}

void PlayerHandler::requestOnlineList() const
{
    createOutPacket(CMSG_ONLINE_LIST);
}

void PlayerHandler::removeOption() const
{
}

void PlayerHandler::changeCart(const int type A_UNUSED) const
{
}

void PlayerHandler::setMemo() const
{
}

void PlayerHandler::updateStatus(const uint8_t status A_UNUSED) const
{
}

void PlayerHandler::setShortcut(const int idx A_UNUSED,
                                const int tab A_UNUSED,
                                const uint8_t type A_UNUSED,
                                const int id A_UNUSED,
                                const int level A_UNUSED) const
{
}

void PlayerHandler::doriDori() const
{
}

void PlayerHandler::explosionSpirits() const
{
}

void PlayerHandler::requestPvpInfo() const
{
}

void PlayerHandler::revive() const
{
}

void PlayerHandler::shortcutShiftRow(const int row A_UNUSED,
                                     const int tab A_UNUSED) const
{
}

void PlayerHandler::setConfigOption(const int id A_UNUSED,
                                    const int data A_UNUSED) const
{
}

#define setStatComplex(stat) \
    PlayerInfo::setStatBase(stat, CAST_S32(base), notify); \
    if (mod != NoStat) \
        PlayerInfo::setStatMod(stat, mod, Notify_true)

void PlayerHandler::setStat(Net::MessageIn &msg,
                            const int type,
                            const int64_t base,
                            const int mod,
                            const Notify notify) const
{
    switch (type)
    {
        case Sp::SPEED:
            localPlayer->setWalkSpeed(CAST_S32(base));
            PlayerInfo::setStatBase(Attributes::PLAYER_WALK_SPEED,
                CAST_S32(base),
                Notify_true);
            PlayerInfo::setStatMod(Attributes::PLAYER_WALK_SPEED,
                0,
                Notify_true);
            break;
        case Sp::BASEEXP:
            PlayerInfo::setAttribute(Attributes::PLAYER_EXP,
                base,
                Notify_true);
            break;
        case Sp::JOBEXP:
            PlayerInfo::setAttribute(Attributes::PLAYER_JOB_EXP,
                base,
                Notify_true);
            break;
        case Sp::KARMA:
            PlayerInfo::setStatBase(Attributes::PLAYER_KARMA,
                CAST_S32(base),
                Notify_true);
            PlayerInfo::setStatMod(Attributes::PLAYER_KARMA,
                0,
                Notify_true);
            break;
        case Sp::MANNER:
            PlayerInfo::setStatBase(Attributes::PLAYER_MANNER,
                CAST_S32(base),
                Notify_true);
            PlayerInfo::setStatMod(Attributes::PLAYER_MANNER,
                0,
                Notify_true);
            break;
        case Sp::HP:
            PlayerInfo::setAttribute(Attributes::PLAYER_HP,
                base,
                Notify_true);
            if (localPlayer->isInParty() && (Party::getParty(1) != nullptr))
            {
                PartyMember *const m = Party::getParty(1)
                    ->getMember(localPlayer->getId());
                if (m != nullptr)
                {
                    m->setHp(CAST_S32(base));
                    m->setMaxHp(PlayerInfo::getAttribute(
                        Attributes::PLAYER_MAX_HP));
                }
            }
            break;
        case Sp::MAXHP:
            PlayerInfo::setAttribute(Attributes::PLAYER_MAX_HP,
                base,
                Notify_true);

            if (localPlayer->isInParty() && (Party::getParty(1) != nullptr))
            {
                PartyMember *const m = Party::getParty(1)->getMember(
                    localPlayer->getId());
                if (m != nullptr)
                {
                    m->setHp(PlayerInfo::getAttribute(Attributes::PLAYER_HP));
                    m->setMaxHp(CAST_S32(base));
                }
            }
            break;
        case Sp::SP:
            PlayerInfo::setAttribute(Attributes::PLAYER_MP,
                base,
                Notify_true);
            break;
        case Sp::MAXSP:
            PlayerInfo::setAttribute(Attributes::PLAYER_MAX_MP,
                base,
                Notify_true);
            break;
        case Sp::STATUSPOINT:
            PlayerInfo::setAttribute(Attributes::PLAYER_CHAR_POINTS,
                base,
                Notify_true);
            break;
        case Sp::BASELEVEL:
            PlayerInfo::setAttribute(Attributes::PLAYER_BASE_LEVEL,
                base,
                Notify_true);
            if (localPlayer != nullptr)
            {
                localPlayer->setLevel(CAST_S32(base));
                localPlayer->updateName();
            }
            break;
        case Sp::SKILLPOINT:
            PlayerInfo::setAttribute(Attributes::PLAYER_SKILL_POINTS,
                base,
                Notify_true);
            if (skillDialog != nullptr)
                skillDialog->update();
            break;
        case Sp::STR:
            setStatComplex(Attributes::PLAYER_STR);
            break;
        case Sp::AGI:
            setStatComplex(Attributes::PLAYER_AGI);
            break;
        case Sp::VIT:
            setStatComplex(Attributes::PLAYER_VIT);
            break;
        case Sp::INT:
            setStatComplex(Attributes::PLAYER_INT);
            break;
        case Sp::DEX:
            setStatComplex(Attributes::PLAYER_DEX);
            break;
        case Sp::LUK:
            setStatComplex(Attributes::PLAYER_LUK);
            break;
        case Sp::ZENY:
        {
            const int oldMoney = PlayerInfo::getAttribute(Attributes::MONEY);
            const int newMoney = CAST_S32(base);
            if (newMoney > oldMoney)
            {
                NotifyManager::notify(NotifyTypes::MONEY_GET,
                    UnitsDb::formatCurrency(newMoney - oldMoney));
            }
            else if (newMoney < oldMoney)
            {
                NotifyManager::notify(NotifyTypes::MONEY_SPENT,
                    UnitsDb::formatCurrency(oldMoney - newMoney).c_str());
            }

            PlayerInfo::setAttribute(Attributes::MONEY,
                newMoney,
                Notify_true);
            break;
        }
        case Sp::NEXTBASEEXP:
            PlayerInfo::setAttribute(Attributes::PLAYER_EXP_NEEDED,
                base,
                Notify_true);
            break;
        case Sp::JOB_MOD:
            PlayerInfo::setAttribute(Attributes::PLAYER_JOB_EXP_NEEDED,
                base,
                Notify_true);
            break;
        case Sp::WEIGHT:
            PlayerInfo::setAttribute(Attributes::TOTAL_WEIGHT,
                base,
                Notify_true);
            break;
        case Sp::MAXWEIGHT:
            PlayerInfo::setAttribute(Attributes::MAX_WEIGHT,
                base,
                Notify_true);
            break;
        case Sp::USTR:
            statusWindow->setPointsNeeded(Attributes::PLAYER_STR,
                CAST_S32(base));
            break;
        case Sp::UAGI:
            statusWindow->setPointsNeeded(Attributes::PLAYER_AGI,
                CAST_S32(base));
            break;
        case Sp::UVIT:
            statusWindow->setPointsNeeded(Attributes::PLAYER_VIT,
                CAST_S32(base));
            break;
        case Sp::UINT:
            statusWindow->setPointsNeeded(Attributes::PLAYER_INT,
                CAST_S32(base));
            break;
        case Sp::UDEX:
            statusWindow->setPointsNeeded(Attributes::PLAYER_DEX,
                CAST_S32(base));
            break;
        case Sp::ULUK:
            statusWindow->setPointsNeeded(Attributes::PLAYER_LUK,
                CAST_S32(base));
            break;
        case Sp::ATK1:
            PlayerInfo::setStatBase(Attributes::PLAYER_ATK,
                CAST_S32(base),
                Notify_true);
            PlayerInfo::updateAttrs();
            break;
        case Sp::ATK2:
            PlayerInfo::setStatMod(Attributes::PLAYER_ATK,
                CAST_S32(base),
                Notify_true);
            PlayerInfo::updateAttrs();
            break;
        case Sp::MATK1:
            PlayerInfo::setStatBase(Attributes::PLAYER_MATK,
                CAST_S32(base),
                Notify_true);
            break;
        case Sp::MATK2:
            PlayerInfo::setStatMod(Attributes::PLAYER_MATK,
                CAST_S32(base),
                Notify_true);
            break;
        case Sp::DEF1:
            PlayerInfo::setStatBase(Attributes::PLAYER_DEF,
                CAST_S32(base),
                Notify_true);
            break;
        case Sp::DEF2:
            PlayerInfo::setStatMod(Attributes::PLAYER_DEF,
                CAST_S32(base),
                Notify_true);
            break;
        case Sp::MDEF1:
            PlayerInfo::setStatBase(Attributes::PLAYER_MDEF,
                CAST_S32(base),
                Notify_true);
            break;
        case Sp::MDEF2:
            PlayerInfo::setStatMod(Attributes::PLAYER_MDEF,
                CAST_S32(base),
                Notify_true);
            break;
        case Sp::HIT:
            PlayerInfo::setStatBase(Attributes::PLAYER_HIT,
                CAST_S32(base),
                Notify_true);
            break;
        case Sp::FLEE1:
            PlayerInfo::setStatBase(Attributes::PLAYER_FLEE,
                CAST_S32(base),
                Notify_true);
            break;
        case Sp::FLEE2:
            PlayerInfo::setStatMod(Attributes::PLAYER_FLEE,
                CAST_S32(base),
                Notify_true);
            break;
        case Sp::CRITICAL:
            PlayerInfo::setStatBase(Attributes::PLAYER_CRIT,
                CAST_S32(base),
                Notify_true);
            break;
        case Sp::ASPD:
            localPlayer->setAttackSpeed(CAST_S32(base));
            PlayerInfo::setStatBase(Attributes::PLAYER_ATTACK_DELAY,
                CAST_S32(base),
                Notify_true);
            PlayerInfo::setStatMod(Attributes::PLAYER_ATTACK_DELAY,
                0,
                Notify_true);
            PlayerInfo::updateAttrs();
            break;
        case Sp::JOBLEVEL:
            PlayerInfo::setAttribute(Attributes::PLAYER_JOB_LEVEL,
                base,
                Notify_true);
            break;
        case Sp::GM_LEVEL:
            localPlayer->setGroupId(CAST_S32(base));
            break;

        default:
            UNIMPLEMENTEDPACKETFIELD(type);
            break;
    }
}

#undef setStatComplex

void PlayerHandler::selectStyle(const int headColor A_UNUSED,
                                const int headStyle A_UNUSED,
                                const int bodyColor A_UNUSED,
                                const int topStyle A_UNUSED,
                                const int middleStyle A_UNUSED,
                                const int bottomStyle A_UNUSED,
                                const int bodyStyle A_UNUSED) const
{
}

void PlayerHandler::setTitle(const int titleId A_UNUSED) const
{
}

void PlayerHandler::closeStyleWindow() const
{
}

}  // namespace TmwAthena
