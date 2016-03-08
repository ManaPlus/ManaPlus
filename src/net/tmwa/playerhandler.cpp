/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "units.h"
#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "const/net/nostat.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/statuswindow.h"

#include "net/ea/eaprotocol.h"

#include "net/tmwa/inventoryhandler.h"
#include "net/tmwa/messageout.h"
#include "net/tmwa/protocolout.h"
#include "net/tmwa/sp.h"

#include "debug.h"

extern Net::PlayerHandler *playerHandler;

namespace TmwAthena
{

PlayerHandler::PlayerHandler() :
    Ea::PlayerHandler()
{
    playerHandler = this;
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
    if (attr >= Attributes::STR && attr <= Attributes::LUK)
    {
        createOutPacket(CMSG_STAT_UPDATE_REQUEST);
        outMsg.writeInt16(CAST_S16(attr), "attribute id");
        outMsg.writeInt8(1, "increment");
    }
}

void PlayerHandler::increaseSkill(const uint16_t skillId) const
{
    if (PlayerInfo::getAttribute(Attributes::SKILL_POINTS) <= 0)
        return;

    createOutPacket(CMSG_SKILL_LEVELUP_REQUEST);
    outMsg.writeInt16(skillId, "skill id");
}

void PlayerHandler::pickUp(const FloorItem *const floorItem) const
{
    if (!floorItem)
        return;

    createOutPacket(CMSG_ITEM_PICKUP);
    outMsg.writeBeingId(floorItem->getId(), "object id");
    TmwAthena::InventoryHandler *const handler =
        static_cast<TmwAthena::InventoryHandler*>(inventoryHandler);
    if (handler)
        handler->pushPickup(floorItem->getId());
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

void PlayerHandler::shortcutShiftRow(const int row A_UNUSED) const
{
}

void PlayerHandler::setViewEquipment(const bool allow A_UNUSED) const
{
}

#define setStatComplex(stat) \
    PlayerInfo::setStatBase(stat, base, notify); \
    if (mod != NoStat) \
        PlayerInfo::setStatMod(stat, mod)

void PlayerHandler::setStat(Net::MessageIn &msg,
                            const int type,
                            const int base,
                            const int mod,
                            const Notify notify) const
{
    switch (type)
    {
        case Sp::WALK_SPEED:
            localPlayer->setWalkSpeed(base);
            PlayerInfo::setStatBase(Attributes::WALK_SPEED, base);
            PlayerInfo::setStatMod(Attributes::WALK_SPEED, 0);
            break;
        case Sp::EXP:
            PlayerInfo::setAttribute(Attributes::EXP, base);
            break;
        case Sp::JOB_EXP:
            PlayerInfo::setStatExperience(Attributes::JOB, base,
                PlayerInfo::getStatExperience(Attributes::JOB).second);
            break;
        case Sp::KARMA:
            PlayerInfo::setStatBase(Attributes::KARMA, base);
            PlayerInfo::setStatMod(Attributes::KARMA, 0);
            break;
        case Sp::MANNER:
            PlayerInfo::setStatBase(Attributes::MANNER, base);
            PlayerInfo::setStatMod(Attributes::MANNER, 0);
            break;
        case Sp::HP:
            PlayerInfo::setAttribute(Attributes::HP, base);
            if (localPlayer->isInParty() && Party::getParty(1))
            {
                PartyMember *const m = Party::getParty(1)
                    ->getMember(localPlayer->getId());
                if (m)
                {
                    m->setHp(base);
                    m->setMaxHp(PlayerInfo::getAttribute(Attributes::MAX_HP));
                }
            }
            break;
        case Sp::MAX_HP:
            PlayerInfo::setAttribute(Attributes::MAX_HP, base);

            if (localPlayer->isInParty() && Party::getParty(1))
            {
                PartyMember *const m = Party::getParty(1)->getMember(
                    localPlayer->getId());
                if (m)
                {
                    m->setHp(PlayerInfo::getAttribute(Attributes::HP));
                    m->setMaxHp(base);
                }
            }
            break;
        case Sp::MP:
            PlayerInfo::setAttribute(Attributes::MP, base);
            break;
        case Sp::MAX_MP:
            PlayerInfo::setAttribute(Attributes::MAX_MP, base);
            break;
        case Sp::CHAR_POINTS:
            PlayerInfo::setAttribute(Attributes::CHAR_POINTS, base);
            break;
        case Sp::LEVEL:
            PlayerInfo::setAttribute(Attributes::LEVEL, base);
            if (localPlayer)
            {
                localPlayer->setLevel(base);
                localPlayer->updateName();
            }
            break;
        case Sp::SKILL_POINTS:
            PlayerInfo::setAttribute(Attributes::SKILL_POINTS, base);
            if (skillDialog)
                skillDialog->update();
            break;
        case Sp::STR:
            setStatComplex(Attributes::STR);
            break;
        case Sp::AGI:
            setStatComplex(Attributes::AGI);
            break;
        case Sp::VIT:
            setStatComplex(Attributes::VIT);
            break;
        case Sp::INT:
            setStatComplex(Attributes::INT);
            break;
        case Sp::DEX:
            setStatComplex(Attributes::DEX);
            break;
        case Sp::LUK:
            setStatComplex(Attributes::LUK);
            break;
        case Sp::MONEY:
        {
            const int oldMoney = PlayerInfo::getAttribute(Attributes::MONEY);
            const int newMoney = base;
            if (newMoney > oldMoney)
            {
                NotifyManager::notify(NotifyTypes::MONEY_GET,
                    Units::formatCurrency(newMoney - oldMoney));
            }
            else if (newMoney < oldMoney)
            {
                NotifyManager::notify(NotifyTypes::MONEY_SPENT,
                    Units::formatCurrency(oldMoney - newMoney).c_str());
            }

            PlayerInfo::setAttribute(Attributes::MONEY, newMoney);
            break;
        }
        case Sp::EXP_NEEDED:
            PlayerInfo::setAttribute(Attributes::EXP_NEEDED, base);
            break;
        case Sp::JOB_MOD:
            PlayerInfo::setStatExperience(Attributes::JOB,
                PlayerInfo::getStatExperience(Attributes::JOB).first, base);
            break;
        case Sp::TOTAL_WEIGHT:
            PlayerInfo::setAttribute(Attributes::TOTAL_WEIGHT, base);
            break;
        case Sp::MAX_WEIGHT:
            PlayerInfo::setAttribute(Attributes::MAX_WEIGHT, base);
            break;
        case Sp::STR_NEEDED:
            statusWindow->setPointsNeeded(Attributes::STR, base);
            break;
        case Sp::AGI_NEEDED:
            statusWindow->setPointsNeeded(Attributes::AGI, base);
            break;
        case Sp::VIT_NEEDED:
            statusWindow->setPointsNeeded(Attributes::VIT, base);
            break;
        case Sp::INT_NEEDED:
            statusWindow->setPointsNeeded(Attributes::INT, base);
            break;
        case Sp::DEX_NEEDED:
            statusWindow->setPointsNeeded(Attributes::DEX, base);
            break;
        case Sp::LUK_NEEDED:
            statusWindow->setPointsNeeded(Attributes::LUK, base);
            break;

        case Sp::ATK:
            PlayerInfo::setStatBase(Attributes::ATK, base);
            PlayerInfo::updateAttrs();
            break;
        case Sp::ATK_MOD:
            PlayerInfo::setStatMod(Attributes::ATK, base);
            PlayerInfo::updateAttrs();
            break;
        case Sp::MATK:
            PlayerInfo::setStatBase(Attributes::MATK, base);
            break;
        case Sp::MATK_MOD:
            PlayerInfo::setStatMod(Attributes::MATK, base);
            break;
        case Sp::DEF:
            PlayerInfo::setStatBase(Attributes::DEF, base);
            break;
        case Sp::DEF_MOD:
            PlayerInfo::setStatMod(Attributes::DEF, base);
            break;
        case Sp::MDEF:
            PlayerInfo::setStatBase(Attributes::MDEF, base);
            break;
        case Sp::MDEF_MOD:
            PlayerInfo::setStatMod(Attributes::MDEF, base);
            break;
        case Sp::HIT:
            PlayerInfo::setStatBase(Attributes::HIT, base);
            break;
        case Sp::FLEE:
            PlayerInfo::setStatBase(Attributes::FLEE, base);
            break;
        case Sp::FLEE_MOD:
            PlayerInfo::setStatMod(Attributes::FLEE, base);
            break;
        case Sp::CRIT:
            PlayerInfo::setStatBase(Attributes::CRIT, base);
            break;
        case Sp::ATTACK_DELAY:
            localPlayer->setAttackSpeed(base);
            PlayerInfo::setStatBase(Attributes::ATTACK_DELAY, base);
            PlayerInfo::setStatMod(Attributes::ATTACK_DELAY, 0);
            PlayerInfo::updateAttrs();
            break;
        case Sp::JOB:
            PlayerInfo::setStatBase(Attributes::JOB, base);
            break;
        case Sp::GM_LEVEL:
            localPlayer->setGMLevel(base);
            break;

        default:
            UNIMPLIMENTEDPACKET;
            break;
    }

}

#undef setStatComplex

}  // namespace TmwAthena
