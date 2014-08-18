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

#include "net/ea/playerhandler.h"

#include "configuration.h"
#include "game.h"
#include "party.h"
#include "notifymanager.h"
#include "units.h"

#include "being/attributes.h"
#include "being/localplayer.h"

#include "gui/viewport.h"

#include "gui/windows/okdialog.h"
#include "gui/windows/skilldialog.h"
#include "gui/windows/statuswindow.h"

#include "resources/notifytypes.h"

#include "resources/db/deaddb.h"

#include "resources/map/map.h"

#include "listeners/playerdeathlistener.h"
#include "listeners/weightlistener.h"

#include "net/messagein.h"

#include "net/ea/eaprotocol.h"

#include "utils/gettext.h"

#include "debug.h"

extern int weightNoticeTime;

// Max. distance we are willing to scroll after a teleport;
// everything beyond will reset the port hard.
static const int MAP_TELEPORT_SCROLL_DISTANCE = 8;

namespace
{
    WeightListener weightListener;
    PlayerDeathListener deathListener;
}  // anonymous namespace

namespace Ea
{

PlayerHandler::PlayerHandler()
{
}

void PlayerHandler::decreaseAttribute(const int attr A_UNUSED) const
{
}

void PlayerHandler::ignorePlayer(const std::string &player A_UNUSED,
                                 const bool ignore A_UNUSED) const
{
}

void PlayerHandler::ignoreAll(const bool ignore A_UNUSED) const
{
}

bool PlayerHandler::canCorrectAttributes() const
{
    return false;
}

Vector PlayerHandler::getDefaultWalkSpeed() const
{
    // Return an normalized speed for any side
    // as the offset is calculated elsewhere.
    return Vector(150, 150, 0);
}

void PlayerHandler::processWalkResponse(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processWalkResponse")
    /*
      * This client assumes that all walk messages succeed,
      * and that the server will send a correction notice
      * otherwise.
      */
    uint16_t srcX, srcY, dstX, dstY;
    msg.readInt32();  // tick
    msg.readCoordinatePair(srcX, srcY, dstX, dstY);
    if (localPlayer)
        localPlayer->setRealPos(dstX, dstY);
    BLOCK_END("PlayerHandler::processWalkResponse")
}

void PlayerHandler::processPlayerWarp(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processPlayerWarp")
    std::string mapPath = msg.readString(16);
    int x = msg.readInt16();
    int y = msg.readInt16();

    logger->log("Warping to %s (%d, %d)", mapPath.c_str(), x, y);

    if (!localPlayer)
        logger->log1("SMSG_PLAYER_WARP localPlayer null");

    /*
      * We must clear the local player's target *before* the call
      * to changeMap, as it deletes all beings.
      */
    if (localPlayer)
        localPlayer->stopAttack();

    Game *const game = Game::instance();

    const std::string &currentMapName = game->getCurrentMapName();
    const bool sameMap = (currentMapName == mapPath);

    // Switch the actual map, deleting the previous one if necessary
    mapPath = mapPath.substr(0, mapPath.rfind("."));
    game->changeMap(mapPath);

    int scrollOffsetX = 0;
    int scrollOffsetY = 0;

    if (localPlayer)
    {
        const Map *const map = game->getCurrentMap();
        if (map)
        {
            if (x >= map->getWidth())
                x = map->getWidth() - 1;
            if (y >= map->getHeight())
                y = map->getHeight() - 1;
            if (x < 0)
                x = 0;
            if (y < 0)
                y = 0;
            /* Scroll if neccessary */
            if (!sameMap
                || (abs(x - localPlayer->getTileX())
                > MAP_TELEPORT_SCROLL_DISTANCE)
                || (abs(y - localPlayer->getTileY())
                > MAP_TELEPORT_SCROLL_DISTANCE))
            {
                scrollOffsetX = (x - localPlayer->getTileX())
                    * map->getTileWidth();
                scrollOffsetY = (y - localPlayer->getTileY())
                    * map->getTileHeight();
            }
        }

        localPlayer->setAction(BeingAction::STAND, 0);
        localPlayer->setTileCoords(x, y);
        localPlayer->updatePets();
        localPlayer->navigateClean();
    }

    logger->log("Adjust scrolling by %d:%d", scrollOffsetX, scrollOffsetY);

    if (viewport)
    {
        viewport->returnCamera();
        viewport->scrollBy(scrollOffsetX, scrollOffsetY);
    }
    BLOCK_END("PlayerHandler::processPlayerWarp")
}

void PlayerHandler::processPlayerStatUpdate1(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate1")
    const int type = msg.readInt16();
    const int value = msg.readInt32();
    if (!localPlayer)
    {
        BLOCK_END("PlayerHandler::processPlayerStatUpdate1")
        return;
    }

    switch (type)
    {
        case 0x0000:
            localPlayer->setWalkSpeed(Vector(static_cast<float>(
                value), static_cast<float>(value), 0));
            PlayerInfo::setStatBase(Attributes::WALK_SPEED, value);
            PlayerInfo::setStatMod(Attributes::WALK_SPEED, 0);
        break;
        case 0x0004:
            break;  // manner
        case 0x0005:
            PlayerInfo::setAttribute(Attributes::HP, value);
            if (localPlayer->isInParty() && Party::getParty(1))
            {
                PartyMember *const m = Party::getParty(1)
                    ->getMember(localPlayer->getId());
                if (m)
                {
                    m->setHp(value);
                    m->setMaxHp(PlayerInfo::getAttribute(Attributes::MAX_HP));
                }
            }
            break;
        case 0x0006:
            PlayerInfo::setAttribute(Attributes::MAX_HP, value);

            if (localPlayer->isInParty() && Party::getParty(1))
            {
                PartyMember *const m = Party::getParty(1)->getMember(
                    localPlayer->getId());
                if (m)
                {
                    m->setHp(PlayerInfo::getAttribute(Attributes::HP));
                    m->setMaxHp(value);
                }
            }
            break;
        case 0x0007:
            PlayerInfo::setAttribute(Attributes::MP, value);
            break;
        case 0x0008:
            PlayerInfo::setAttribute(Attributes::MAX_MP, value);
            break;
        case 0x0009:
            PlayerInfo::setAttribute(Attributes::CHAR_POINTS, value);
            break;
        case 0x000b:
            PlayerInfo::setAttribute(Attributes::LEVEL, value);
            if (localPlayer)
            {
                localPlayer->setLevel(value);
                localPlayer->updateName();
            }
            break;
        case 0x000c:
            PlayerInfo::setAttribute(Attributes::SKILL_POINTS, value);
            if (skillDialog)
                skillDialog->update();
            break;
        case 0x0018:
            if (!weightNotice && config.getBoolValue("weightMsg"))
            {
                const int max = PlayerInfo::getAttribute(
                    Attributes::MAX_WEIGHT) / 2;
                const int total = PlayerInfo::getAttribute(
                    Attributes::TOTAL_WEIGHT);
                if (value >= max && total < max)
                {
                    weightNoticeTime = cur_time + 5;
                    // TRANSLATORS: message header
                    weightNotice = new OkDialog(_("Message"),
                        // TRANSLATORS: weight message
                        _("You are carrying more than "
                        "half your weight. You are "
                        "unable to regain health."), DialogType::OK,
                        false, true, nullptr, 260);
                    weightNotice->addActionListener(
                        &weightListener);
                }
                else if (value < max && total >= max)
                {
                    weightNoticeTime = cur_time + 5;
                    // TRANSLATORS: message header
                    weightNotice = new OkDialog(_("Message"),
                        // TRANSLATORS: weight message
                        _("You are carrying less than "
                        "half your weight. You "
                        "can regain health."), DialogType::OK,
                        false, true, nullptr, 260);
                    weightNotice->addActionListener(
                        &weightListener);
                }
            }
            PlayerInfo::setAttribute(Attributes::TOTAL_WEIGHT, value);
            break;
        case 0x0019:
            PlayerInfo::setAttribute(Attributes::MAX_WEIGHT, value);
            break;

        case 0x0029:
            PlayerInfo::setStatBase(EA_ATK, value);
            PlayerInfo::updateAttrs();
            break;
        case 0x002a:
            PlayerInfo::setStatMod(EA_ATK, value);
            PlayerInfo::updateAttrs();
            break;

        case 0x002b:
            PlayerInfo::setStatBase(EA_MATK, value);
            break;
        case 0x002c:
            PlayerInfo::setStatMod(EA_MATK, value);
            break;

        case 0x002d:
            PlayerInfo::setStatBase(EA_DEF, value);
            break;
        case 0x002e:
            PlayerInfo::setStatMod(EA_DEF, value);
            break;

        case 0x002f:
            PlayerInfo::setStatBase(EA_MDEF, value);
            break;
        case 0x0030:
            PlayerInfo::setStatMod(EA_MDEF, value);
            break;

        case 0x0031:
            PlayerInfo::setStatBase(EA_HIT, value);
            break;

        case 0x0032:
            PlayerInfo::setStatBase(EA_FLEE, value);
            break;
        case 0x0033:
            PlayerInfo::setStatMod(EA_FLEE, value);
            break;

        case 0x0034:
            PlayerInfo::setStatBase(EA_CRIT, value);
            break;

        case 0x0035:
            localPlayer->setAttackSpeed(value);
            PlayerInfo::setStatBase(Attributes::ATTACK_DELAY, value);
            PlayerInfo::setStatMod(Attributes::ATTACK_DELAY, 0);
            PlayerInfo::updateAttrs();
            break;

        case 0x0037:
            PlayerInfo::setStatBase(EA_JOB, value);
            break;

        case 500:
            localPlayer->setGMLevel(value);
            break;

        default:
            logger->log("QQQQ PLAYER_STAT_UPDATE_1 "
                        + toString(type) + "," + toString(value));
            break;
    }

    if (PlayerInfo::getAttribute(Attributes::HP) == 0 && !deathNotice)
    {
        // TRANSLATORS: message header
        deathNotice = new OkDialog(_("Message"),
            DeadDB::getRandomString(), DialogType::OK,
            false, true, nullptr, 260);
        deathNotice->addActionListener(&deathListener);
        if (localPlayer->getCurrentAction() != BeingAction::DEAD)
        {
            localPlayer->setAction(BeingAction::DEAD, 0);
            localPlayer->recalcSpritesOrder();
        }
    }
    BLOCK_END("PlayerHandler::processPlayerStatUpdate1")
}

void PlayerHandler::processPlayerStatUpdate2(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate2")
    const int type = msg.readInt16();
    switch (type)
    {
        case 0x0001:
            PlayerInfo::setAttribute(Attributes::EXP, msg.readInt32());
            break;
        case 0x0002:
            PlayerInfo::setStatExperience(EA_JOB, msg.readInt32(),
            PlayerInfo::getStatExperience(EA_JOB).second);
            break;
        case 0x0014:
        {
            const int oldMoney = PlayerInfo::getAttribute(Attributes::MONEY);
            const int newMoney = msg.readInt32();
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
        case 0x0016:
            PlayerInfo::setAttribute(Attributes::EXP_NEEDED, msg.readInt32());
            break;
        case 0x0017:
            PlayerInfo::setStatExperience(EA_JOB,
                PlayerInfo::getStatExperience(EA_JOB).first, msg.readInt32());
            break;
        default:
            logger->log("QQQQ PLAYER_STAT_UPDATE_2 " + toString(type));
            break;
    }
    BLOCK_END("PlayerHandler::processPlayerStatUpdate2")
}

void PlayerHandler::processPlayerStatUpdate3(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate3")
    const int type = msg.readInt32();
    const int base = msg.readInt32();
    const int bonus = msg.readInt32();

    PlayerInfo::setStatBase(type, base, false);
    PlayerInfo::setStatMod(type, bonus);
    if (type == EA_ATK || type == Attributes::ATTACK_DELAY)
        PlayerInfo::updateAttrs();
    BLOCK_END("PlayerHandler::processPlayerStatUpdate3")
}

void PlayerHandler::processPlayerStatUpdate4(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate4")
    const int type = msg.readInt16();
    const uint8_t ok = msg.readUInt8();
    const int value = msg.readUInt8();

    if (ok != 1)
    {
        const int oldValue = PlayerInfo::getStatBase(type);
        const int points = PlayerInfo::getAttribute(Attributes::CHAR_POINTS)
            + oldValue - value;
        PlayerInfo::setAttribute(Attributes::CHAR_POINTS, points);
        NotifyManager::notify(NotifyTypes::SKILL_RAISE_ERROR);
    }

    PlayerInfo::setStatBase(type, value);
    BLOCK_END("PlayerHandler::processPlayerStatUpdate4")
}

void PlayerHandler::processPlayerStatUpdate5(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate5")
    PlayerInfo::setAttribute(Attributes::CHAR_POINTS, msg.readInt16());

    unsigned int val = msg.readUInt8();
    PlayerInfo::setStatBase(EA_STR, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(EA_STR, msg.readUInt8());
    else
        msg.readUInt8();

    val = msg.readUInt8();
    PlayerInfo::setStatBase(EA_AGI, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(EA_AGI, msg.readUInt8());
    else
        msg.readUInt8();

    val = msg.readUInt8();
    PlayerInfo::setStatBase(EA_VIT, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(EA_VIT, msg.readUInt8());
    else
        msg.readUInt8();

    val = msg.readUInt8();
    PlayerInfo::setStatBase(EA_INT, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(EA_INT, msg.readUInt8());
    else
        msg.readUInt8();

    val = msg.readUInt8();
    PlayerInfo::setStatBase(EA_DEX, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(EA_DEX, msg.readUInt8());
    else
        msg.readUInt8();

    val = msg.readUInt8();
    PlayerInfo::setStatBase(EA_LUK, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(EA_LUK, msg.readUInt8());
    else
        msg.readUInt8();

    PlayerInfo::setStatBase(EA_ATK, msg.readInt16(), false);
    PlayerInfo::setStatMod(EA_ATK, msg.readInt16());
    PlayerInfo::updateAttrs();

    val = msg.readInt16();
    PlayerInfo::setStatBase(EA_MATK, val, false);

    val = msg.readInt16();
    PlayerInfo::setStatMod(EA_MATK, val);

    PlayerInfo::setStatBase(EA_DEF, msg.readInt16(), false);
    PlayerInfo::setStatMod(EA_DEF, msg.readInt16());

    PlayerInfo::setStatBase(EA_MDEF, msg.readInt16(), false);
    PlayerInfo::setStatMod(EA_MDEF, msg.readInt16());

    PlayerInfo::setStatBase(EA_HIT, msg.readInt16());

    PlayerInfo::setStatBase(EA_FLEE, msg.readInt16(), false);
    PlayerInfo::setStatMod(EA_FLEE, msg.readInt16());

    PlayerInfo::setStatBase(EA_CRIT, msg.readInt16());

    msg.readInt16();  // manner
    BLOCK_END("PlayerHandler::processPlayerStatUpdate5")
}

void PlayerHandler::processPlayerStatUpdate6(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate6")
    const int type = msg.readInt16();
    if (statusWindow)
    {
        switch (type)
        {
            case 0x0020:
                statusWindow->setPointsNeeded(EA_STR, msg.readUInt8());
                break;
            case 0x0021:
                statusWindow->setPointsNeeded(EA_AGI, msg.readUInt8());
                break;
            case 0x0022:
                statusWindow->setPointsNeeded(EA_VIT, msg.readUInt8());
                break;
            case 0x0023:
                statusWindow->setPointsNeeded(EA_INT, msg.readUInt8());
                break;
            case 0x0024:
                statusWindow->setPointsNeeded(EA_DEX, msg.readUInt8());
                break;
            case 0x0025:
                statusWindow->setPointsNeeded(EA_LUK, msg.readUInt8());
                break;
            default:
                logger->log("QQQQ PLAYER_STAT_UPDATE_6 "
                            + toString(type));
                break;
        }
    }
    BLOCK_END("PlayerHandler::processPlayerStatUpdate6")
}

void PlayerHandler::processPlayerArrowMessage(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processPlayerArrowMessage")
    const int type = msg.readInt16();
    switch (type)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::ARROWS_EQUIP_NEEDED);
            break;
        case 3:
            // arrows equiped
            break;
        default:
            logger->log("QQQQ 0x013b: Unhandled message %i", type);
            break;
    }
    BLOCK_END("PlayerHandler::processPlayerArrowMessage")
}

bool PlayerHandler::canUseMagic() const
{
    return PlayerInfo::getStatEffective(EA_MATK) > 0;
}

int PlayerHandler::getJobLocation() const
{
    return EA_JOB;
}

int PlayerHandler::getAttackLocation() const
{
    return EA_ATK;
}

}  // namespace Ea
