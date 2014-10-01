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

#include "gui/dialogtype.h"
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

void PlayerHandler::processPlayerStatUpdate1(Net::MessageIn &msg) const
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate1")
    const int type = msg.readInt16("type");
    const int value = msg.readInt32("value");
    if (!localPlayer)
    {
        BLOCK_END("PlayerHandler::processPlayerStatUpdate1")
        return;
    }

    setStat(type, value, -1, true);

    if (PlayerInfo::getAttribute(Attributes::HP) == 0 && !deathNotice)
    {
        // TRANSLATORS: message header
        deathNotice = new OkDialog(_("Message"),
            DeadDB::getRandomString(),
            // TRANSLATORS: ok dialog button
            _("Revive"),
            DialogType::OK,
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

void PlayerHandler::processPlayerStatUpdate2(Net::MessageIn &msg) const
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate2")
    const int type = msg.readInt16("type");
    const int value = msg.readInt32("value");
    setStat(type, value, -1, true);
    BLOCK_END("PlayerHandler::processPlayerStatUpdate2")
}

void PlayerHandler::processPlayerStatUpdate3(Net::MessageIn &msg) const
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate3")
    const int type = msg.readInt32("type");
    const int base = msg.readInt32("base");
    const int bonus = msg.readInt32("bonus");

    setStat(type, base, bonus, false);
    BLOCK_END("PlayerHandler::processPlayerStatUpdate3")
}

void PlayerHandler::processPlayerStatUpdate4(Net::MessageIn &msg) const
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

    setStat(type, value, -1, true);
    BLOCK_END("PlayerHandler::processPlayerStatUpdate4")
}

void PlayerHandler::processPlayerStatUpdate6(Net::MessageIn &msg) const
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate6")
    const int type = msg.readInt16("type");
    const int value = msg.readUInt8("value");
    if (statusWindow)
        setStat(type, value, -1, true);
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
    return PlayerInfo::getStatEffective(Attributes::MATK) > 0;
}

int PlayerHandler::getJobLocation() const
{
    return EA_JOB;
}

#define setStatComplex(stat) \
    PlayerInfo::setStatBase(stat, base, notify); \
    if (mod != -1) \
        PlayerInfo::setStatMod(stat, mod)

void PlayerHandler::setStat(const int type,
                            const int base,
                            const int mod,
                            const bool notify) const
{
    switch (type)
    {
        case 0x0000:
            localPlayer->setWalkSpeed(Vector(static_cast<float>(
                base), static_cast<float>(base), 0));
            PlayerInfo::setStatBase(Attributes::WALK_SPEED, base);
            PlayerInfo::setStatMod(Attributes::WALK_SPEED, 0);
            break;
        case 0x0001:
            PlayerInfo::setAttribute(Attributes::EXP, base);
            break;
        case 0x0002:
            PlayerInfo::setStatExperience(EA_JOB, base,
                PlayerInfo::getStatExperience(EA_JOB).second);
            break;
        case 0x0003:
            PlayerInfo::setStatBase(Attributes::KARMA, base);
            PlayerInfo::setStatMod(Attributes::KARMA, 0);
            break;
        case 0x0004:
            PlayerInfo::setStatBase(Attributes::MANNER, base);
            PlayerInfo::setStatMod(Attributes::MANNER, 0);
            break;
        case 0x0005:
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
        case 0x0006:
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
        case 0x0007:
            PlayerInfo::setAttribute(Attributes::MP, base);
            break;
        case 0x0008:
            PlayerInfo::setAttribute(Attributes::MAX_MP, base);
            break;
        case 0x0009:
            PlayerInfo::setAttribute(Attributes::CHAR_POINTS, base);
            break;
        case 0x000b:
            PlayerInfo::setAttribute(Attributes::LEVEL, base);
            if (localPlayer)
            {
                localPlayer->setLevel(base);
                localPlayer->updateName();
            }
            break;
        case 0x000c:
            PlayerInfo::setAttribute(Attributes::SKILL_POINTS, base);
            if (skillDialog)
                skillDialog->update();
            break;
        case 0x000d:
            setStatComplex(EA_STR);
            break;
        case 0x000e:
            setStatComplex(Attributes::AGI);
            break;
        case 0x000f:
            setStatComplex(Attributes::VIT);
            break;
        case 0x0010:
            setStatComplex(Attributes::INT);
            break;
        case 0x0011:
            setStatComplex(Attributes::DEX);
            break;
        case 0x0012:
            setStatComplex(Attributes::LUK);
            break;
        case 0x0014:
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
        case 0x0016:
            PlayerInfo::setAttribute(Attributes::EXP_NEEDED, base);
            break;
        case 0x0017:
            PlayerInfo::setStatExperience(EA_JOB,
                PlayerInfo::getStatExperience(EA_JOB).first, base);
            break;
        case 0x0018:
            if (!weightNotice && config.getBoolValue("weightMsg"))
            {
                const int max = PlayerInfo::getAttribute(
                    Attributes::MAX_WEIGHT) / 2;
                const int total = PlayerInfo::getAttribute(
                    Attributes::TOTAL_WEIGHT);
                if (base >= max && total < max)
                {
                    weightNoticeTime = cur_time + 5;
                    // TRANSLATORS: message header
                    weightNotice = new OkDialog(_("Message"),
                        // TRANSLATORS: weight message
                        _("You are carrying more than "
                        "half your weight. You are "
                        "unable to regain health."),
                        // TRANSLATORS: ok dialog button
                        _("OK"),
                        DialogType::OK,
                        false, true, nullptr, 260);
                    weightNotice->addActionListener(
                        &weightListener);
                }
                else if (base < max && total >= max)
                {
                    weightNoticeTime = cur_time + 5;
                    // TRANSLATORS: message header
                    weightNotice = new OkDialog(_("Message"),
                        // TRANSLATORS: weight message
                        _("You are carrying less than "
                        "half your weight. You "
                        "can regain health."),
                        // TRANSLATORS: ok dialog button
                        _("OK"),
                        DialogType::OK,
                        false, true, nullptr, 260);
                    weightNotice->addActionListener(
                        &weightListener);
                }
            }
            PlayerInfo::setAttribute(Attributes::TOTAL_WEIGHT, base);
            break;
        case 0x0019:
            PlayerInfo::setAttribute(Attributes::MAX_WEIGHT, base);
            break;
        case 0x0020:
            statusWindow->setPointsNeeded(EA_STR, base);
            break;
        case 0x0021:
            statusWindow->setPointsNeeded(Attributes::AGI, base);
            break;
        case 0x0022:
            statusWindow->setPointsNeeded(Attributes::VIT, base);
            break;
        case 0x0023:
            statusWindow->setPointsNeeded(Attributes::INT, base);
            break;
        case 0x0024:
            statusWindow->setPointsNeeded(Attributes::DEX, base);
            break;
        case 0x0025:
            statusWindow->setPointsNeeded(Attributes::LUK, base);
            break;
        case 0x0029:
            PlayerInfo::setStatBase(Attributes::ATK, base);
            PlayerInfo::updateAttrs();
            break;
        case 0x002a:
            PlayerInfo::setStatMod(Attributes::ATK, base);
            PlayerInfo::updateAttrs();
            break;
        case 0x002b:
            PlayerInfo::setStatBase(Attributes::MATK, base);
            break;
        case 0x002c:
            PlayerInfo::setStatMod(Attributes::MATK, base);
            break;
        case 0x002d:
            PlayerInfo::setStatBase(Attributes::DEF, base);
            break;
        case 0x002e:
            PlayerInfo::setStatMod(Attributes::DEF, base);
            break;
        case 0x002f:
            PlayerInfo::setStatBase(Attributes::MDEF, base);
            break;
        case 0x0030:
            PlayerInfo::setStatMod(Attributes::MDEF, base);
            break;
        case 0x0031:
            PlayerInfo::setStatBase(Attributes::HIT, base);
            break;
        case 0x0032:
            PlayerInfo::setStatBase(Attributes::FLEE, base);
            break;
        case 0x0033:
            PlayerInfo::setStatMod(Attributes::FLEE, base);
            break;
        case 0x0034:
            PlayerInfo::setStatBase(Attributes::CRIT, base);
            break;
        case 0x0035:
            localPlayer->setAttackSpeed(base);
            PlayerInfo::setStatBase(Attributes::ATTACK_DELAY, base);
            PlayerInfo::setStatMod(Attributes::ATTACK_DELAY, 0);
            PlayerInfo::updateAttrs();
            break;
        case 0x0037:
            PlayerInfo::setStatBase(EA_JOB, base);
            break;

        default:
            logger->log("Error: Unknown stat set: %d, values: %d, %d",
                type, base, mod);
            break;
    }
}

}  // namespace Ea
