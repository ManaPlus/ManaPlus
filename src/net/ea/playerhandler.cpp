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

#include "net/ea/playerhandler.h"

#include "localplayer.h"
#include "logger.h"
#include "party.h"
#include "playerinfo.h"
#include "units.h"

#include "gui/ministatuswindow.h"
#include "gui/okdialog.h"
#include "gui/skilldialog.h"
#include "gui/statuswindow.h"
#include "gui/viewport.h"

#include "net/messagein.h"
#include "net/npchandler.h"

#include "net/ea/eaprotocol.h"

#include "utils/gettext.h"

#include "debug.h"

extern OkDialog *weightNotice;
extern OkDialog *deathNotice;
extern int weightNoticeTime;

// Max. distance we are willing to scroll after a teleport;
// everything beyond will reset the port hard.
static const int MAP_TELEPORT_SCROLL_DISTANCE = 8;

// TODO Move somewhere else
namespace
{
    /**
     * Listener used for handling the overweigth message.
     */
    struct WeightListener : public gcn::ActionListener
    {
        void action(const gcn::ActionEvent &event A_UNUSED)
        {
            weightNotice = nullptr;
        }
    } weightListener;

    /**
     * Listener used for handling death message.
     */
    struct DeathListener : public gcn::ActionListener
    {
        void action(const gcn::ActionEvent &event A_UNUSED)
        {
            if (Net::getPlayerHandler())
                Net::getPlayerHandler()->respawn();
            deathNotice = nullptr;

            Game::closeDialogs();

            if (viewport)
                viewport->closePopupMenu();

            Net::NpcHandler *handler = Net::getNpcHandler();
            if (handler)
                handler->clearDialogs();
            if (player_node)
                player_node->respawn();
        }
    } deathListener;

} // anonymous namespace

static const char *randomDeathMessage()
{
    static char const *const deadMsg[] =
    {
        N_("You are dead."),
        N_("We regret to inform you that your character was killed in "
           "battle."),
        N_("You are not that alive anymore."),
        N_("The cold hands of the grim reaper are grabbing for your soul."),
        N_("Game Over!"),
        N_("Insert coin to continue."),
        N_("No, kids. Your character did not really die. It... "
           "err... went to a better place."),
        N_("Your plan of breaking your enemies weapon by "
           "bashing it with your throat failed."),
        N_("I guess this did not run too well."),
        // NetHack reference:
        N_("Do you want your possessions identified?"),
        // Secret of Mana reference:
        N_("Sadly, no trace of you was ever found..."),
        // Final Fantasy VI reference:
        N_("Annihilated."),
        // Earthbound reference:
        N_("Looks like you got your head handed to you."),
        // Leisure Suit Larry 1 reference:
        N_("You screwed up again, dump your body down the tubes "
           "and get you another one."),
        // Monty Python references (Dead Parrot sketch mostly):
        N_("You're not dead yet. You're just resting."),
        N_("You are no more."),
        N_("You have ceased to be."),
        N_("You've expired and gone to meet your maker."),
        N_("You're a stiff."),
        N_("Bereft of life, you rest in peace."),
        N_("If you weren't so animated, you'd be pushing up the daisies."),
        N_("Your metabolic processes are now history."),
        N_("You're off the twig."),
        N_("You've kicked the bucket."),
        N_("You've shuffled off your mortal coil, run down the "
           "curtain and joined the bleedin' choir invisibile."),
        N_("You are an ex-player."),
        N_("You're pining for the fjords.")
    };

    const int random = static_cast<int>(rand() % (sizeof(deadMsg)
        / sizeof(deadMsg[0])));
    return gettext(deadMsg[random]);
}

namespace Ea
{

PlayerHandler::PlayerHandler()
{
}

void PlayerHandler::decreaseAttribute(int attr A_UNUSED)
{
    // Supported by eA?
}

void PlayerHandler::ignorePlayer(const std::string &player A_UNUSED,
                                 bool ignore A_UNUSED)
{
    // TODO
}

void PlayerHandler::ignoreAll(bool ignore A_UNUSED)
{
    // TODO
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
    /*
      * This client assumes that all walk messages succeed,
      * and that the server will send a correction notice
      * otherwise.
      */
    uint16_t srcX, srcY, dstX, dstY;
    msg.readInt32();    //tick
    msg.readCoordinatePair(srcX, srcY, dstX, dstY);
    if (player_node)
        player_node->setRealPos(dstX, dstY);
}

void PlayerHandler::processPlayerWarp(Net::MessageIn &msg)
{
    std::string mapPath = msg.readString(16);
    int x = msg.readInt16();
    int y = msg.readInt16();

    logger->log("Warping to %s (%d, %d)", mapPath.c_str(), x, y);

    if (!player_node)
        logger->log1("SMSG_PLAYER_WARP player_node null");

    /*
      * We must clear the local player's target *before* the call
      * to changeMap, as it deletes all beings.
      */
    if (player_node)
        player_node->stopAttack();

    Game *game = Game::instance();

    const std::string &currentMapName = game->getCurrentMapName();
    bool sameMap = (currentMapName == mapPath);

    // Switch the actual map, deleting the previous one if necessary
    mapPath = mapPath.substr(0, mapPath.rfind("."));
    game->changeMap(mapPath);

    int scrollOffsetX = 0;
    int scrollOffsetY = 0;

    if (player_node)
    {
        Map *map = game->getCurrentMap();
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
                || (abs(x - player_node->getTileX())
                > MAP_TELEPORT_SCROLL_DISTANCE)
                || (abs(y - player_node->getTileY())
                > MAP_TELEPORT_SCROLL_DISTANCE))
            {
                scrollOffsetX = (x - player_node->getTileX())
                    * map->getTileWidth();
                scrollOffsetY = (y - player_node->getTileY())
                    * map->getTileHeight();
            }
        }

        player_node->setAction(Being::STAND);
        player_node->setTileCoords(x, y);
        player_node->navigateClean();
    }

    logger->log("Adjust scrolling by %d:%d", scrollOffsetX, scrollOffsetY);

    if (viewport)
    {
        viewport->returnCamera();
        if (miniStatusWindow)
            miniStatusWindow->updateStatus();
        viewport->scrollBy(scrollOffsetX, scrollOffsetY);
    }
}

void PlayerHandler::processPlayerStatUpdate1(Net::MessageIn &msg)
{
    int type = msg.readInt16();
    int value = msg.readInt32();
    if (!player_node)
        return;

    switch (type)
    {
        case 0x0000:
            player_node->setWalkSpeed(Vector(static_cast<float>(
                value), static_cast<float>(value), 0));
            PlayerInfo::setStatBase(PlayerInfo::WALK_SPEED, value);
            PlayerInfo::setStatMod(PlayerInfo::WALK_SPEED, 0);
        break;
        case 0x0004: break; // manner
        case 0x0005:
            PlayerInfo::setAttribute(PlayerInfo::HP, value);
            if (player_node->isInParty() && Party::getParty(1))
            {
                PartyMember *m = Party::getParty(1)
                    ->getMember(player_node->getId());
                if (m)
                {
                    m->setHp(value);
                    m->setMaxHp(PlayerInfo::getAttribute(PlayerInfo::MAX_HP));
                }
            }
            break;
        case 0x0006:
            PlayerInfo::setAttribute(PlayerInfo::MAX_HP, value);

            if (player_node->isInParty() && Party::getParty(1))
            {
                PartyMember *m = Party::getParty(1)->getMember(
                    player_node->getId());
                if (m)
                {
                    m->setHp(PlayerInfo::getAttribute(PlayerInfo::HP));
                    m->setMaxHp(value);
                }
            }
            break;
        case 0x0007:
            PlayerInfo::setAttribute(PlayerInfo::MP, value);
            break;
        case 0x0008:
            PlayerInfo::setAttribute(PlayerInfo::MAX_MP, value);
            break;
        case 0x0009:
            PlayerInfo::setAttribute(PlayerInfo::CHAR_POINTS, value);
            break;
        case 0x000b:
            PlayerInfo::setAttribute(PlayerInfo::LEVEL, value);
            if (player_node)
            {
                player_node->setLevel(value);
                player_node->updateName();
            }
            break;
        case 0x000c:
            PlayerInfo::setAttribute(PlayerInfo::SKILL_POINTS, value);
            if (skillDialog)
                skillDialog->update();
            break;
        case 0x0018:
            if (!weightNotice)
            {
                const int max = PlayerInfo::getAttribute(
                    PlayerInfo::MAX_WEIGHT) / 2;
                const int total = PlayerInfo::getAttribute(
                    PlayerInfo::TOTAL_WEIGHT);
                if (value >= max && total < max)
                {
                    weightNoticeTime = cur_time + 5;
                    weightNotice = new OkDialog(_("Message"),
                        _("You are carrying more than "
                        "half your weight. You are "
                        "unable to regain health."), DIALOG_OK, false);
                    weightNotice->addActionListener(
                        &weightListener);
                }
                else if (value < max && total >= max)
                {
                    weightNoticeTime = cur_time + 5;
                    weightNotice = new OkDialog(_("Message"),
                        _("You are carrying less than "
                        "half your weight. You "
                        "can regain health."), DIALOG_OK, false);
                    weightNotice->addActionListener(
                        &weightListener);
                }
            }
            PlayerInfo::setAttribute(PlayerInfo::TOTAL_WEIGHT, value);
            break;
        case 0x0019:
            PlayerInfo::setAttribute(PlayerInfo::MAX_WEIGHT, value);
            break;

        case 0x0029:
            PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
                EA_ATK), value);
            PlayerInfo::updateAttrs();
            break;
        case 0x002a:
            PlayerInfo::setStatMod(static_cast<PlayerInfo::Attribute>(
                EA_ATK), value);
            PlayerInfo::updateAttrs();
            break;

        case 0x002b:
            PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
                EA_MATK), value);
            break;
        case 0x002c:
            PlayerInfo::setStatMod(static_cast<PlayerInfo::Attribute>(
                EA_MATK), value);
            break;

        case 0x002d:
            PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
                EA_DEF), value);
            break;
        case 0x002e:
            PlayerInfo::setStatMod(static_cast<PlayerInfo::Attribute>(
                EA_DEF), value);
            break;

        case 0x002f:
            PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
                EA_MDEF), value);
            break;
        case 0x0030:
            PlayerInfo::setStatMod(static_cast<PlayerInfo::Attribute>(
                EA_MDEF), value);
            break;

        case 0x0031:
            PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
                EA_HIT), value);
            break;

        case 0x0032:
            PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
                EA_FLEE), value);
            break;
        case 0x0033:
            PlayerInfo::setStatMod(static_cast<PlayerInfo::Attribute>(
                EA_FLEE), value);
            break;

        case 0x0034:
            PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
                EA_CRIT), value);
            break;

        case 0x0035:
            player_node->setAttackSpeed(value);
            PlayerInfo::setStatBase(PlayerInfo::ATTACK_DELAY, value);
            PlayerInfo::setStatMod(PlayerInfo::ATTACK_DELAY, 0);
            PlayerInfo::updateAttrs();
            break;

        case 0x0037:
            PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
                EA_JOB), value);
            break;

        case 500:
            player_node->setGMLevel(value);
            break;

        default:
            logger->log("QQQQ PLAYER_STAT_UPDATE_1 "
                        + toString(type) + "," + toString(value));
            break;
    }

    if (PlayerInfo::getAttribute(PlayerInfo::HP) == 0 && !deathNotice)
    {
        deathNotice = new OkDialog(_("Message"),
            randomDeathMessage(), DIALOG_OK, false);
        deathNotice->addActionListener(&deathListener);
        player_node->setAction(Being::DEAD);
    }
}

void PlayerHandler::processPlayerStatUpdate2(Net::MessageIn &msg)
{
    int type = msg.readInt16();
    switch (type)
    {
        case 0x0001:
            PlayerInfo::setAttribute(PlayerInfo::EXP, msg.readInt32());
            break;
        case 0x0002:
            PlayerInfo::setStatExperience(static_cast<PlayerInfo::Attribute>(
                EA_JOB), msg.readInt32(),
            PlayerInfo::getStatExperience(static_cast<PlayerInfo::Attribute>(
                EA_JOB)).second);
            break;
        case 0x0014:
        {
            int oldMoney = PlayerInfo::getAttribute(PlayerInfo::MONEY);
            int newMoney = msg.readInt32();
            if (newMoney > oldMoney)
            {
                SERVER_NOTICE(strprintf(_("You picked up %s."),
                    Units::formatCurrency(newMoney - oldMoney).c_str()))
            }
            else if (newMoney < oldMoney)
            {
                SERVER_NOTICE(strprintf(_("You spent %s."),
                    Units::formatCurrency(oldMoney - newMoney).c_str()))
            }

            PlayerInfo::setAttribute(PlayerInfo::MONEY, newMoney);
            break;
        }
        case 0x0016:
            PlayerInfo::setAttribute(PlayerInfo::EXP_NEEDED, msg.readInt32());
            break;
        case 0x0017:
            PlayerInfo::setStatExperience(static_cast<PlayerInfo::Attribute>(
                EA_JOB), PlayerInfo::getStatExperience(
                static_cast<PlayerInfo::Attribute>(EA_JOB)).first,
                msg.readInt32());
            break;
        default:
            logger->log("QQQQ PLAYER_STAT_UPDATE_2 " + toString(type));
            break;
    }
}

void PlayerHandler::processPlayerStatUpdate3(Net::MessageIn &msg)
{
    int type = msg.readInt32();
    int base = msg.readInt32();
    int bonus = msg.readInt32();

    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(
        type), base, false);
    PlayerInfo::setStatMod(static_cast<PlayerInfo::Attribute>(
        type), bonus);
    if (type == EA_ATK || type == PlayerInfo::ATTACK_DELAY)
        PlayerInfo::updateAttrs();
}

void PlayerHandler::processPlayerStatUpdate4(Net::MessageIn &msg)
{
    int type = msg.readInt16();
    int ok = msg.readInt8();
    int value = msg.readInt8();

    if (ok != 1)
    {
        int oldValue = PlayerInfo::getStatBase(
            static_cast<PlayerInfo::Attribute>(type));
        int points = PlayerInfo::getAttribute(PlayerInfo::CHAR_POINTS);
        points += oldValue - value;
        PlayerInfo::setAttribute(PlayerInfo::CHAR_POINTS, points);
        SERVER_NOTICE(_("Cannot raise skill!"))
    }

    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(type), value);
}

void PlayerHandler::processPlayerStatUpdate5(Net::MessageIn &msg)
{
    PlayerInfo::setAttribute(PlayerInfo::CHAR_POINTS, msg.readInt16());

    int val = msg.readInt8();
    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_STR), val);
    if (statusWindow)
        statusWindow->setPointsNeeded(EA_STR, msg.readInt8());
    else
        msg.readInt8();

    val = msg.readInt8();
    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_AGI), val);
    if (statusWindow)
        statusWindow->setPointsNeeded(EA_AGI, msg.readInt8());
    else
        msg.readInt8();

    val = msg.readInt8();
    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_VIT), val);
    if (statusWindow)
        statusWindow->setPointsNeeded(EA_VIT, msg.readInt8());
    else
        msg.readInt8();

    val = msg.readInt8();
    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_INT), val);
    if (statusWindow)
        statusWindow->setPointsNeeded(EA_INT, msg.readInt8());
    else
        msg.readInt8();

    val = msg.readInt8();
    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_DEX), val);
    if (statusWindow)
        statusWindow->setPointsNeeded(EA_DEX, msg.readInt8());
    else
        msg.readInt8();

    val = msg.readInt8();
    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_LUK), val);
    if (statusWindow)
        statusWindow->setPointsNeeded(EA_LUK, msg.readInt8());
    else
        msg.readInt8();

    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_ATK),
        msg.readInt16(), false);
    PlayerInfo::setStatMod(static_cast<PlayerInfo::Attribute>(EA_ATK),
        msg.readInt16());
    PlayerInfo::updateAttrs();

    val = msg.readInt16();
    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_MATK),
        val, false);

    val = msg.readInt16();
    PlayerInfo::setStatMod(static_cast<PlayerInfo::Attribute>(EA_MATK), val);

    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_DEF),
        msg.readInt16(), false);
    PlayerInfo::setStatMod(static_cast<PlayerInfo::Attribute>(EA_DEF),
        msg.readInt16());

    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_MDEF),
        msg.readInt16(), false);
    PlayerInfo::setStatMod(static_cast<PlayerInfo::Attribute>(EA_MDEF),
        msg.readInt16());

    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_HIT),
        msg.readInt16());

    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_FLEE),
        msg.readInt16(), false);
    PlayerInfo::setStatMod(static_cast<PlayerInfo::Attribute>(EA_FLEE),
        msg.readInt16());

    PlayerInfo::setStatBase(static_cast<PlayerInfo::Attribute>(EA_CRIT),
        msg.readInt16());

    msg.readInt16();  // manner
}

void PlayerHandler::processPlayerStatUpdate6(Net::MessageIn &msg)
{
    int type = msg.readInt16();
    if (statusWindow)
    {
        switch (type)
        {
            case 0x0020:
                statusWindow->setPointsNeeded(EA_STR, msg.readInt8());
                break;
            case 0x0021:
                statusWindow->setPointsNeeded(EA_AGI, msg.readInt8());
                break;
            case 0x0022:
                statusWindow->setPointsNeeded(EA_VIT, msg.readInt8());
                break;
            case 0x0023:
                statusWindow->setPointsNeeded(EA_INT, msg.readInt8());
                break;
            case 0x0024:
                statusWindow->setPointsNeeded(EA_DEX, msg.readInt8());
                break;
            case 0x0025:
                statusWindow->setPointsNeeded(EA_LUK, msg.readInt8());
                break;
            default:
                logger->log("QQQQ PLAYER_STAT_UPDATE_6 "
                            + toString(type));
                break;
        }
    }
}

void PlayerHandler::processPlayerArrowMessage(Net::MessageIn &msg)
{
    int type = msg.readInt16();

    switch (type)
    {
        case 0:
            SERVER_NOTICE(_("Equip arrows first."))
            break;
        case 3:
            // arrows equiped
            break;
        default:
            logger->log("QQQQ 0x013b: Unhandled message %i", type);
            break;
    }
}

bool PlayerHandler::canUseMagic() const
{
    return PlayerInfo::getStatEffective(static_cast<PlayerInfo::Attribute>(
        EA_MATK)) > 0;
}

PlayerInfo::Attribute PlayerHandler::getJobLocation() const
{
    return static_cast<PlayerInfo::Attribute>(EA_JOB);
}

PlayerInfo::Attribute PlayerHandler::getAttackLocation() const
{
    return static_cast<PlayerInfo::Attribute>(EA_ATK);
}

} // namespace Ea
