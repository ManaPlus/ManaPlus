/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "net/ea/beingrecv.h"

#include "actormanager.h"
#include "configuration.h"
#include "game.h"
#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"
#include "being/playerrelation.h"
#include "being/playerrelations.h"

#include "enums/resources/notifytypes.h"

#include "enums/resources/map/mapitemtype.h"

#include "gui/viewport.h"

#include "gui/windows/socialwindow.h"

#include "resources/map/map.h"

#include "net/messagein.h"
#include "net/serverfeatures.h"

#include "debug.h"

namespace Ea
{

namespace BeingRecv
{
    bool mSync = false;
    BeingId mSpawnId = BeingId_zero;
}  // namespace BeingRecv

void BeingRecv::processBeingRemove(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingRemove")
    if ((actorManager == nullptr) || (localPlayer == nullptr))
    {
        BLOCK_END("BeingRecv::processBeingRemove")
        return;
    }

    // A being should be removed or has died

    const BeingId id = msg.readBeingId("being id");
    const uint8_t type = msg.readUInt8("remove flag");
    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing == nullptr)
    {
        BLOCK_END("BeingRecv::processBeingRemove")
        return;
    }

    localPlayer->followMoveTo(dstBeing, localPlayer->getNextDestX(),
        localPlayer->getNextDestY());

    // If this is player's current target, clear it.
    if (dstBeing == localPlayer->getTarget())
        localPlayer->stopAttack(true);

    if (type == 1U)
    {
        if (dstBeing->getCurrentAction() != BeingAction::DEAD)
        {
            dstBeing->setAction(BeingAction::DEAD, 0);
            dstBeing->recalcSpritesOrder();
        }
    }
    else if (type == 0U && dstBeing->getType() == ActorType::Npc)
    {
        const BeingInfo *const info = dstBeing->getInfo();
        if ((info == nullptr) || (info->getAllowDelete() != 0))
            actorManager->destroy(dstBeing);
    }
    else
    {
        if (dstBeing->getType() == ActorType::Player)
        {
            if (socialWindow != nullptr)
                socialWindow->updateActiveList();
            const std::string name = dstBeing->getName();
            if (!name.empty() && config.getBoolValue("logPlayerActions"))
            {
                switch (type)
                {
                    case 0:
                        dstBeing->serverRemove();
                        break;
                    case 1:
                        NotifyManager::notify(
                            NotifyTypes::BEING_REMOVE_DIED,
                            name);
                        break;
                    case 2:
                        NotifyManager::notify(
                            NotifyTypes::BEING_REMOVE_LOGGED_OUT,
                            name);
                        break;
                    case 3:
                        NotifyManager::notify(
                            NotifyTypes::BEING_REMOVE_WARPED,
                            name);
                        break;
                    case 4:
                        NotifyManager::notify(
                            NotifyTypes::BEING_REMOVE_TRICK_DEAD,
                            name);
                        break;
                    default:
                        NotifyManager::notify(
                            NotifyTypes::BEING_REMOVE_UNKNOWN,
                            name);
                        break;
                }
            }
        }
        actorManager->destroy(dstBeing);
    }
    BLOCK_END("BeingRecv::processBeingRemove")
}

void BeingRecv::processBeingAction(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingAction")
    if (actorManager == nullptr)
    {
        BLOCK_END("BeingRecv::processBeingAction")
        return;
    }

    Being *const srcBeing = actorManager->findBeing(
        msg.readBeingId("src being id"));
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("dst being id"));

    msg.readInt32("tick");
    const int srcSpeed = msg.readInt32("src speed");
    msg.readInt32("dst speed");
    const int param1 = msg.readInt16("param1");
    msg.readInt16("param 2");
    const AttackTypeT type = static_cast<AttackTypeT>(
        msg.readUInt8("type"));
    msg.readInt16("param 3");

    switch (type)
    {
        case AttackType::HIT:  // Damage
        case AttackType::CRITICAL:  // Critical Damage
        case AttackType::MULTI:  // Critical Damage
        case AttackType::REFLECT:  // Reflected Damage
        case AttackType::FLEE:  // Lucky Dodge
            if (srcBeing != nullptr)
            {
                if (srcSpeed != 0 && srcBeing->getType() == ActorType::Player)
                    srcBeing->setAttackDelay(srcSpeed);
                // attackid=1, type
                srcBeing->handleAttack(dstBeing, param1, 1);
                if (srcBeing->getType() == ActorType::Player)
                    srcBeing->setAttackTime();
            }
            if (dstBeing != nullptr)
            {
                // level not present, using 1
                dstBeing->takeDamage(srcBeing, param1,
                    static_cast<AttackTypeT>(type), 1);
            }
            break;

        case AttackType::PICKUP:
            break;
            // tmw server can send here garbage?
//            if (srcBeing)
//                srcBeing->setAction(BeingAction::DEAD, 0);

        case AttackType::SIT:
            if (srcBeing != nullptr)
            {
                srcBeing->setAction(BeingAction::SIT, 0);
                if (srcBeing->getType() == ActorType::Player)
                {
                    srcBeing->setMoveTime();
                    if (localPlayer != nullptr)
                        localPlayer->imitateAction(srcBeing, BeingAction::SIT);
                }
            }
            break;

        case AttackType::STAND:
            if (srcBeing != nullptr)
            {
                srcBeing->setAction(BeingAction::STAND, 0);
                if (srcBeing->getType() == ActorType::Player)
                {
                    srcBeing->setMoveTime();
                    if (localPlayer != nullptr)
                    {
                        localPlayer->imitateAction(srcBeing,
                            BeingAction::STAND);
                    }
                }
            }
            break;
        default:
        case AttackType::SPLASH:
        case AttackType::SKILL:
        case AttackType::REPEATE:
        case AttackType::MULTI_REFLECT:
        case AttackType::TOUCH_SKILL:
        case AttackType::MISS:
        case AttackType::SKILLMISS:
            UNIMPLEMENTEDPACKETFIELD(CAST_S32(type));
            break;
    }
    BLOCK_END("BeingRecv::processBeingAction")
}

void BeingRecv::processBeingEmotion(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingEmotion")
    if ((localPlayer == nullptr) || (actorManager == nullptr))
    {
        BLOCK_END("BeingRecv::processBeingEmotion")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));
    if (dstBeing == nullptr)
    {
        DEBUGLOGSTR("invisible player?");
        msg.readUInt8("emote");
        BLOCK_END("BeingRecv::processBeingEmotion")
        return;
    }

    const uint8_t emote = msg.readUInt8("emote");
    if ((emote != 0u) &&
        player_relations.hasPermission(dstBeing, PlayerRelation::EMOTE))
    {
        dstBeing->setEmote(emote, 0);
        localPlayer->imitateEmote(dstBeing, emote);
    }
    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setOtherTime();
    BLOCK_END("BeingRecv::processBeingEmotion")
}

void BeingRecv::processNameResponse(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processNameResponse")
    if ((localPlayer == nullptr) || (actorManager == nullptr))
    {
        BLOCK_END("BeingRecv::processNameResponse")
        return;
    }

    const BeingId beingId = msg.readBeingId("being id");
    const std::string name = msg.readString(24, "name");
    Being *const dstBeing = actorManager->findBeing(beingId);

    actorManager->updateNameId(name, beingId);

    if (dstBeing != nullptr)
    {
        if (beingId == localPlayer->getId())
        {
            localPlayer->pingResponse();
        }
        else
        {
            if (dstBeing->getType() != ActorType::Portal)
            {
                dstBeing->setName(name);
            }
            else if (viewport != nullptr)
            {
                Map *const map = viewport->getMap();
                if (map != nullptr)
                {
                    map->addPortalTile(name, MapItemType::PORTAL,
                        dstBeing->getTileX(), dstBeing->getTileY());
                }
            }
            dstBeing->updateGuild();
            dstBeing->addToCache();

            if (dstBeing->getType() == ActorType::Player)
                dstBeing->updateColors();

            if (localPlayer != nullptr)
            {
                const Party *const party = localPlayer->getParty();
                if (party != nullptr && party->isMember(dstBeing->getId()))
                {
                    PartyMember *const member = party->getMember(
                        dstBeing->getId());

                    if (member != nullptr)
                        member->setName(dstBeing->getName());
                }
                localPlayer->checkNewName(dstBeing);
            }
            BLOCK_END("BeingRecv::processNameResponse")
            return;
        }
    }
    BLOCK_END("BeingRecv::processNameResponse")
}

void BeingRecv::processPlayerStop(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processPlayerStop")
    if ((actorManager == nullptr) || (localPlayer == nullptr))
    {
        BLOCK_END("BeingRecv::processPlayerStop")
        return;
    }

    const BeingId id = msg.readBeingId("account id");

    if (mSync || id != localPlayer->getId())
    {
        Being *const dstBeing = actorManager->findBeing(id);
        if (dstBeing != nullptr)
        {
            const uint16_t x = msg.readInt16("x");
            const uint16_t y = msg.readInt16("y");
            dstBeing->setTileCoords(x, y);
            if (dstBeing->getCurrentAction() == BeingAction::MOVE)
                dstBeing->setAction(BeingAction::STAND, 0);
            BLOCK_END("BeingRecv::processPlayerStop")
            return;
        }
    }
    msg.readInt16("x");
    msg.readInt16("y");
    BLOCK_END("BeingRecv::processPlayerStop")
}

void BeingRecv::processPlayerMoveToAttack(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processPlayerStop")
    msg.readInt32("target id");
    msg.readInt16("target x");
    msg.readInt16("target y");
    msg.readInt16("x");
    msg.readInt16("y");
    msg.readInt16("attack range");

    if (localPlayer != nullptr)
        localPlayer->fixAttackTarget();
    BLOCK_END("BeingRecv::processPlayerStop")
}

void BeingRecv::processSkillNoDamage(Net::MessageIn &msg)
{
    const int id = msg.readInt16("skill id");
    int heal;
    if (msg.getVersion() >= 20131223)
        heal = msg.readInt32("heal");
    else
        heal = msg.readInt16("heal");
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("dst being id"));
    Being *const srcBeing = actorManager->findBeing(
        msg.readBeingId("src being id"));
    msg.readUInt8("fail");

    if (srcBeing != nullptr)
        srcBeing->handleSkill(dstBeing, heal, id, 1);
}

void BeingRecv::processPvpMapMode(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processPvpMapMode")
    const Game *const game = Game::instance();
    if (game == nullptr)
    {
        BLOCK_END("BeingRecv::processPvpMapMode")
        return;
    }

    Map *const map = game->getCurrentMap();
    if (map != nullptr)
        map->setPvpMode(msg.readInt16("pvp mode"));
    BLOCK_END("BeingRecv::processPvpMapMode")
}

void BeingRecv::processBeingMove3(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingMove3")
    if ((actorManager == nullptr) || !serverFeatures->haveMove3())
    {
        BLOCK_END("BeingRecv::processBeingMove3")
        return;
    }

    static const int16_t dirx[8] = {0, -1, -1, -1,  0,  1, 1, 1};
    static const int16_t diry[8] = {1,  1,  0, -1, -1, -1, 0, 1};

    const int len = msg.readInt16("len") - 14;
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));
    if ((dstBeing == nullptr) || dstBeing == localPlayer)
    {
        DEBUGLOGSTR("invisible player?");
        msg.readInt16("speed");
        msg.readInt16("x");
        msg.readInt16("y");
        msg.readBytes(len, "moving path");
        BLOCK_END("BeingRecv::processBeingMove3")
        return;
    }
    const int16_t speed = msg.readInt16("speed");
    dstBeing->setWalkSpeed(speed);
    const int16_t x = msg.readInt16("x");
    const int16_t y = msg.readInt16("y");
    const unsigned char *moves = msg.readBytes(len, "moving path");

    Path path;
    if (moves != nullptr)
    {
        int x2 = dstBeing->getCachedX();
        int y2 = dstBeing->getCachedY();
        Path path2;
        path2.push_back(Position(x2, y2));
        for (int f = len - 1; f >= 0; f --)
        {
            const unsigned char dir = moves[f];
            if (dir <= 7)
            {
                x2 -= dirx[dir];
                y2 -= diry[dir];
                path2.push_back(Position(x2, y2));
                if (x2 == x && y2 == y)
                    break;
            }
            else
            {
                logger->log("bad move packet: %d", dir);
            }
        }

        if (!path2.empty())
        {
            const Position &pos = path2.back();
            if (x != pos.x ||
                y != pos.y)
            {
                dstBeing->setTileCoords(pos.x, pos.y);
            }
        }

        path2.pop_back();
        FOR_EACHR (PathRIterator, it, path2)
        {
            path.push_back(*it);
        }
        delete [] moves;
    }

    if (path.empty())
        return;

    dstBeing->setAction(BeingAction::STAND, 0);
    dstBeing->setTileCoords(x, y);
    dstBeing->setPath(path);
    BLOCK_END("BeingRecv::processBeingMove3")
}

Being *BeingRecv::createBeing(const BeingId id,
                              const int job)
{
    if (actorManager == nullptr)
        return nullptr;

    ActorTypeT type = ActorType::Unknown;
    if (job <= 25 || (job >= 4001 && job <= 4049))
        type = ActorType::Player;
    else if (job >= 46 && job <= 1000)
        type = ActorType::Npc;
    else if (job > 1000 && job <= 2000)
        type = ActorType::Monster;
    else if (job == 45)
        type = ActorType::Portal;

    Being *const being = actorManager->createBeing(
        id, type, fromInt(job, BeingTypeId));
    return being;
}

}  // namespace Ea
