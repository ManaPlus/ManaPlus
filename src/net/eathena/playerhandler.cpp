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

#include "net/eathena/playerhandler.h"

#include "units.h"
#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "const/net/nostat.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/statuswindow.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"
#include "net/eathena/inventoryhandler.h"
#include "net/eathena/sp.h"

#include "debug.h"

extern Net::PlayerHandler *playerHandler;
extern int serverVersion;

namespace EAthena
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
                                      const int amount) const
{
    if (attr >= Attributes::STR && attr <= Attributes::LUK)
    {
        createOutPacket(CMSG_STAT_UPDATE_REQUEST);
        outMsg.writeInt16(CAST_S16(attr), "attribute id");
        outMsg.writeInt8(CAST_S8(amount), "increase");
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
    EAthena::InventoryHandler *const handler =
        static_cast<EAthena::InventoryHandler*>(inventoryHandler);
    if (handler)
        handler->pushPickup(floorItem->getId());
}

void PlayerHandler::setDirection(const unsigned char direction) const
{
    createOutPacket(CMSG_PLAYER_CHANGE_DIR);
    outMsg.writeInt8(0, "head direction");
    outMsg.writeInt8(0, "unused");
    outMsg.writeInt8(MessageOut::toServerDirection(direction),
        "player direction");
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
    unsigned char type;
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

void PlayerHandler::updateStatus(const uint8_t status) const
{
    createOutPacket(CMSG_SET_STATUS);
    outMsg.writeInt8(status, "status");
    outMsg.writeInt8(0, "unused");
}

void PlayerHandler::setShortcut(const int idx,
                                const uint8_t type,
                                const int id,
                                const int level) const
{
    createOutPacket(CMSG_SET_SHORTCUTS);
    outMsg.writeInt16(CAST_S16(idx), "index");
    outMsg.writeInt8(CAST_S8(type), "type");
    outMsg.writeInt32(id, "id");
    outMsg.writeInt16(CAST_S16(level), "level");
}

void PlayerHandler::shortcutShiftRow(const int row) const
{
    if (serverVersion != 0 && serverVersion < 11)
        return;
    createOutPacket(CMSG_SHORTCUTS_ROW_SHIFT);
    outMsg.writeInt8(CAST_S8(row), "row");
}

void PlayerHandler::removeOption() const
{
    createOutPacket(CMSG_REMOVE_OPTION);
}

void PlayerHandler::changeCart(const int type) const
{
    createOutPacket(CMSG_CHANGE_CART);
    outMsg.writeInt16(CAST_S16(type), "type");
}

void PlayerHandler::setMemo() const
{
    createOutPacket(CMSG_PLAYER_SET_MEMO);
}

void PlayerHandler::doriDori() const
{
    createOutPacket(CMSG_DORI_DORI);
}

void PlayerHandler::explosionSpirits() const
{
    createOutPacket(CMSG_EXPLOSION_SPIRITS);
}

void PlayerHandler::requestPvpInfo() const
{
    createOutPacket(CMSG_PVP_INFO);
    outMsg.writeInt32(0, "char id");
    outMsg.writeInt32(0, "account id");
}

void PlayerHandler::revive() const
{
    createOutPacket(CMSG_PLAYER_AUTO_REVIVE);
}

void PlayerHandler::setViewEquipment(const bool allow) const
{
    createOutPacket(CMSG_PLAYER_SET_EQUIPMENT_VISIBLE);
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(allow ? 1 : 0, "allow");
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
        case Sp::SPEED:
            localPlayer->setWalkSpeed(base);
            PlayerInfo::setStatBase(Attributes::WALK_SPEED, base);
            PlayerInfo::setStatMod(Attributes::WALK_SPEED, 0);
            break;
        case Sp::BASEEXP:
            PlayerInfo::setAttribute(Attributes::EXP, base);
            break;
        case Sp::JOBEXP:
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
        case Sp::MAXHP:
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
        case Sp::SP:
            PlayerInfo::setAttribute(Attributes::MP, base);
            break;
        case Sp::MAXSP:
            PlayerInfo::setAttribute(Attributes::MAX_MP, base);
            break;
        case Sp::STATUSPOINT:
            PlayerInfo::setAttribute(Attributes::CHAR_POINTS, base);
            break;
        case Sp::BASELEVEL:
            PlayerInfo::setAttribute(Attributes::LEVEL, base);
            if (localPlayer)
            {
                localPlayer->setLevel(base);
                localPlayer->updateName();
            }
            break;
        case Sp::SKILLPOINT:
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
        case Sp::ZENY:
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
        case Sp::NEXTBASEEXP:
            PlayerInfo::setAttribute(Attributes::EXP_NEEDED, base);
            break;
        // ++ here used in wrong way. Need like Sp::NEXTBASEEXP?
        case Sp::NEXTJOBEXP:
            PlayerInfo::setStatExperience(Attributes::JOB,
                PlayerInfo::getStatExperience(Attributes::JOB).first, base);
            break;
        case Sp::WEIGHT:
            PlayerInfo::setAttribute(Attributes::TOTAL_WEIGHT, base);
            break;
        case Sp::MAXWEIGHT:
            PlayerInfo::setAttribute(Attributes::MAX_WEIGHT, base);
            break;
        case Sp::USTR:
            statusWindow->setPointsNeeded(Attributes::STR, base);
            break;
        case Sp::UAGI:
            statusWindow->setPointsNeeded(Attributes::AGI, base);
            break;
        case Sp::UVIT:
            statusWindow->setPointsNeeded(Attributes::VIT, base);
            break;
        case Sp::UINT:
            statusWindow->setPointsNeeded(Attributes::INT, base);
            break;
        case Sp::UDEX:
            statusWindow->setPointsNeeded(Attributes::DEX, base);
            break;
        case Sp::ULUK:
            statusWindow->setPointsNeeded(Attributes::LUK, base);
            break;

        case Sp::ATK1:
            PlayerInfo::setStatBase(Attributes::ATK, base);
            PlayerInfo::updateAttrs();
            break;
        case Sp::ATK2:
            PlayerInfo::setStatMod(Attributes::ATK, base);
            PlayerInfo::updateAttrs();
            break;
        case Sp::MATK1:
            PlayerInfo::setStatBase(Attributes::MATK, base);
            break;
        case Sp::MATK2:
            PlayerInfo::setStatMod(Attributes::MATK, base);
            break;
        case Sp::DEF1:
            PlayerInfo::setStatBase(Attributes::DEF, base);
            break;
        case Sp::DEF2:
            PlayerInfo::setStatMod(Attributes::DEF, base);
            break;
        case Sp::MDEF1:
            PlayerInfo::setStatBase(Attributes::MDEF, base);
            break;
        case Sp::MDEF2:
            PlayerInfo::setStatMod(Attributes::MDEF, base);
            break;
        case Sp::HIT:
            PlayerInfo::setStatBase(Attributes::HIT, base);
            break;
        case Sp::FLEE1:
            PlayerInfo::setStatBase(Attributes::FLEE, base);
            break;
        case Sp::FLEE2:
            PlayerInfo::setStatMod(Attributes::FLEE, base);
            break;
        case Sp::CRITICAL:
            PlayerInfo::setStatBase(Attributes::CRIT, base);
            break;
        case Sp::ASPD:
            localPlayer->setAttackSpeed(base);
            PlayerInfo::setStatBase(Attributes::ATTACK_DELAY, base);
            PlayerInfo::setStatMod(Attributes::ATTACK_DELAY, 0);
            PlayerInfo::updateAttrs();
            break;
        case Sp::JOBLEVEL:
            PlayerInfo::setStatBase(Attributes::JOB, base);
            break;

        default:
            UNIMPLIMENTEDPACKETFIELD(type);
            break;
    }
}

#undef setStatComplex

}  // namespace EAthena
