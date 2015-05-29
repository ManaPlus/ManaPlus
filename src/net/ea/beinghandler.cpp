/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "net/ea/beinghandler.h"

#include "actormanager.h"
#include "game.h"
#include "party.h"

#include "being/localplayer.h"
#include "being/playerrelations.h"

#include "gui/viewport.h"

#include "gui/windows/socialwindow.h"

#include "resources/mapitemtype.h"

#include "resources/map/map.h"

#include "net/serverfeatures.h"

#include "debug.h"

namespace Ea
{
BeingId BeingHandler::mSpawnId = BeingId_zero;
bool BeingHandler::mSync = false;

BeingHandler::BeingHandler(const bool enableSync)
{
    mSync = enableSync;
    mSpawnId = BeingId_zero;
}

Being *BeingHandler::createBeing(const BeingId id,
                                 const int16_t job)
{
    if (!actorManager)
        return nullptr;

    ActorType::Type type = ActorType::Unknown;
    if (job <= 25 || (job >= 4001 && job <= 4049))
        type = ActorType::Player;
    else if (job >= 46 && job <= 1000)
        type = ActorType::Npc;
    else if (job > 1000 && job <= 2000)
        type = ActorType::Monster;
    else if (job == 45)
        type = ActorType::Portal;

    Being *const being = actorManager->createBeing(id, type, job);
    return being;
}

void BeingHandler::setSprite(Being *const being, const unsigned int slot,
                             const int id, const std::string &color,
                             const unsigned char colorId,
                             const bool isWeapon,
                             const bool isTempSprite)
{
    if (!being)
        return;
    being->updateSprite(slot, id, color, colorId, isWeapon, isTempSprite);
}

void BeingHandler::processBeingRemove(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processBeingRemove")
    if (!actorManager || !localPlayer)
    {
        BLOCK_END("BeingHandler::processBeingRemove")
        return;
    }

    // A being should be removed or has died

    const BeingId id = msg.readBeingId("being id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
    {
        msg.readUInt8("dead flag?");
        BLOCK_END("BeingHandler::processBeingRemove")
        return;
    }

    localPlayer->followMoveTo(dstBeing, localPlayer->getNextDestX(),
        localPlayer->getNextDestY());

    // If this is player's current target, clear it.
    if (dstBeing == localPlayer->getTarget())
        localPlayer->stopAttack(true);

    if (msg.readUInt8("dead flag?") == 1U)
    {
        if (dstBeing->getCurrentAction() != BeingAction::DEAD)
        {
            dstBeing->setAction(BeingAction::DEAD, 0);
            dstBeing->recalcSpritesOrder();
        }
    }
    else
    {
        if (dstBeing->getType() == ActorType::Player)
        {
            if (socialWindow)
                socialWindow->updateActiveList();
        }
        actorManager->destroy(dstBeing);
    }
    BLOCK_END("BeingHandler::processBeingRemove")
}

void BeingHandler::processSkillDamage(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processSkillDamage")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processSkillDamage")
        return;
    }

    const int id = msg.readInt16("skill id");
    Being *const srcBeing = actorManager->findBeing(
        msg.readBeingId("src being id"));
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("dst being id"));
    msg.readInt32("tick");
    msg.readInt32("src speed");
    msg.readInt32("dst speed");
    const int param1 = msg.readInt32("damage");
    const int level = msg.readInt16("skill level");
    msg.readInt16("div");
    msg.readUInt8("skill hit/type?");
    if (srcBeing)
        srcBeing->handleSkill(dstBeing, param1, id, level);
    if (dstBeing)
        dstBeing->takeDamage(srcBeing, param1, AttackType::SKILL, id, level);
    BLOCK_END("BeingHandler::processSkillDamage")
}

void BeingHandler::processBeingAction(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processBeingAction")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processBeingAction")
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
    const uint8_t type = msg.readUInt8("type");
    msg.readInt16("param 3");

    switch (type)
    {
        case AttackType::HIT:  // Damage
        case AttackType::CRITICAL:  // Critical Damage
        case AttackType::MULTI:  // Critical Damage
        case AttackType::REFLECT:  // Reflected Damage
        case AttackType::FLEE:  // Lucky Dodge
            if (srcBeing)
            {
                if (srcSpeed && srcBeing->getType() == ActorType::Player)
                    srcBeing->setAttackDelay(srcSpeed);
                // attackid=1, type
                srcBeing->handleAttack(dstBeing, param1, 1);
                if (srcBeing->getType() == ActorType::Player)
                    srcBeing->setAttackTime();
            }
            if (dstBeing)
            {
                // level not present, using 1
                dstBeing->takeDamage(srcBeing, param1,
                    static_cast<AttackType::Type>(type), 1);
            }
            break;

        case 0x01:  // dead or pickup?
            break;
            // tmw server can send here garbage?
//            if (srcBeing)
//                srcBeing->setAction(BeingAction::DEAD, 0);

        case 0x02:  // Sit
            if (srcBeing)
            {
                srcBeing->setAction(BeingAction::SIT, 0);
                if (srcBeing->getType() == ActorType::Player)
                {
                    srcBeing->setMoveTime();
                    if (localPlayer)
                        localPlayer->imitateAction(srcBeing, BeingAction::SIT);
                }
            }
            break;

        case 0x03:  // Stand up
            if (srcBeing)
            {
                srcBeing->setAction(BeingAction::STAND, 0);
                if (srcBeing->getType() == ActorType::Player)
                {
                    srcBeing->setMoveTime();
                    if (localPlayer)
                    {
                        localPlayer->imitateAction(srcBeing,
                            BeingAction::STAND);
                    }
                }
            }
            break;
        default:
            UNIMPLIMENTEDPACKET;
            break;
    }
    BLOCK_END("BeingHandler::processBeingAction")
}

void BeingHandler::processBeingEmotion(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processBeingEmotion")
    if (!localPlayer || !actorManager)
    {
        BLOCK_END("BeingHandler::processBeingEmotion")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));
    if (!dstBeing)
    {
        BLOCK_END("BeingHandler::processBeingEmotion")
        return;
    }

    if (player_relations.hasPermission(dstBeing, PlayerRelation::EMOTE))
    {
        const uint8_t emote = msg.readUInt8("emote");
        if (emote)
        {
            dstBeing->setEmote(emote, 0);
            localPlayer->imitateEmote(dstBeing, emote);
        }
    }
    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setOtherTime();
    BLOCK_END("BeingHandler::processBeingEmotion")
}

void BeingHandler::processNameResponse(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processNameResponse")
    if (!localPlayer || !actorManager)
    {
        BLOCK_END("BeingHandler::processNameResponse")
        return;
    }

    const BeingId beingId = msg.readBeingId("being id");
    Being *const dstBeing = actorManager->findBeing(beingId);

    if (dstBeing)
    {
        if (beingId == localPlayer->getId())
        {
            localPlayer->pingResponse();
        }
        else
        {
            const std::string name = msg.readString(24, "name");
            if (dstBeing->getType() != ActorType::Portal)
            {
                dstBeing->setName(name);
            }
            else if (viewport)
            {
                Map *const map = viewport->getMap();
                if (map)
                {
                    map->addPortalTile(name, MapItemType::PORTAL,
                        dstBeing->getTileX(), dstBeing->getTileY());
                }
            }
            dstBeing->updateGuild();
            dstBeing->addToCache();

            if (dstBeing->getType() == ActorType::Player)
                dstBeing->updateColors();

            if (localPlayer)
            {
                const Party *const party = localPlayer->getParty();
                if (party && party->isMember(dstBeing->getId()))
                {
                    PartyMember *const member = party->getMember(
                        dstBeing->getId());

                    if (member)
                        member->setName(dstBeing->getName());
                }
                localPlayer->checkNewName(dstBeing);
            }
            BLOCK_END("BeingHandler::processNameResponse")
            return;
        }
    }
    msg.readString(24, "name");
    BLOCK_END("BeingHandler::processNameResponse")
}

void BeingHandler::processPlayerStop(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processPlayerStop")
    if (!actorManager || !localPlayer)
    {
        BLOCK_END("BeingHandler::processPlayerStop")
        return;
    }

    const BeingId id = msg.readBeingId("account id");

    if (mSync || id != localPlayer->getId())
    {
        Being *const dstBeing = actorManager->findBeing(id);
        if (dstBeing)
        {
            const uint16_t x = msg.readInt16("x");
            const uint16_t y = msg.readInt16("y");
            dstBeing->setTileCoords(x, y);
            if (dstBeing->getCurrentAction() == BeingAction::MOVE)
                dstBeing->setAction(BeingAction::STAND, 0);
            BLOCK_END("BeingHandler::processPlayerStop")
            return;
        }
    }
    msg.readInt16("x");
    msg.readInt16("y");
    BLOCK_END("BeingHandler::processPlayerStop")
}

void BeingHandler::processPlayerMoveToAttack(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processPlayerStop")
    msg.readInt32("target id");
    msg.readInt16("target x");
    msg.readInt16("target y");
    msg.readInt16("x");
    msg.readInt16("y");
    msg.readInt16("attack range");

    if (localPlayer)
        localPlayer->fixAttackTarget();
    BLOCK_END("BeingHandler::processPlayerStop")
}

void BeingHandler::processSkillNoDamage(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("skill id");
    msg.readInt16("heal");
    msg.readInt32("dst id");
    msg.readInt32("src id");
    msg.readUInt8("fail");
}

void BeingHandler::processPvpMapMode(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processPvpMapMode")
    const Game *const game = Game::instance();
    if (!game)
    {
        BLOCK_END("BeingHandler::processPvpMapMode")
        return;
    }

    Map *const map = game->getCurrentMap();
    if (map)
        map->setPvpMode(msg.readInt16("pvp mode"));
    BLOCK_END("BeingHandler::processPvpMapMode")
}

void BeingHandler::processPvpSet(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processPvpSet")
    const BeingId id = msg.readBeingId("being id");
    const int rank = msg.readInt32("rank");
    msg.readInt32("num");
    if (actorManager)
    {
        Being *const dstBeing = actorManager->findBeing(id);
        if (dstBeing)
            dstBeing->setPvpRank(rank);
    }
    BLOCK_END("BeingHandler::processPvpSet")
}

void BeingHandler::processNameResponse2(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processNameResponse2")
    if (!actorManager || !localPlayer)
    {
        BLOCK_END("BeingHandler::processNameResponse2")
        return;
    }

    const int len = msg.readInt16("len");
    const BeingId beingId = msg.readBeingId("account ic");
    const std::string str = msg.readString(len - 8, "name");
    Being *const dstBeing = actorManager->findBeing(beingId);
    if (dstBeing)
    {
        if (beingId == localPlayer->getId())
        {
            localPlayer->pingResponse();
        }
        else
        {
            dstBeing->setName(str);
            dstBeing->updateGuild();
            dstBeing->addToCache();

            if (dstBeing->getType() == ActorType::Player)
                dstBeing->updateColors();

            if (localPlayer)
            {
                const Party *const party = localPlayer->getParty();
                if (party && party->isMember(dstBeing->getId()))
                {
                    PartyMember *const member = party->getMember(
                        dstBeing->getId());

                    if (member)
                        member->setName(dstBeing->getName());
                }
                localPlayer->checkNewName(dstBeing);
            }
        }
    }
    BLOCK_END("BeingHandler::processNameResponse2")
}

void BeingHandler::processBeingMove3(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processBeingMove3")
    if (!actorManager || !serverFeatures->haveMove3())
    {
        BLOCK_END("BeingHandler::processBeingMove3")
        return;
    }

    static const int16_t dirx[8] = {0, -1, -1, -1,  0,  1, 1, 1};
    static const int16_t diry[8] = {1,  1,  0, -1, -1, -1, 0, 1};

    const int len = msg.readInt16("len") - 14;
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));
    if (!dstBeing)
    {
        BLOCK_END("BeingHandler::processBeingMove3")
        return;
    }
    const int16_t speed = msg.readInt16("speed");
    dstBeing->setWalkSpeed(Vector(speed, speed, 0));
    int16_t x = msg.readInt16("x");
    int16_t y = msg.readInt16("y");
    const unsigned char *moves = msg.readBytes(len, "moving path");
    Path path;
    if (moves)
    {
        for (int f = 0; f < len; f ++)
        {
            const unsigned char dir = moves[f];
            if (dir <= 7)
            {
                x += dirx[dir];
                y += diry[dir];
                path.push_back(Position(x, y));
            }
            else
            {
                logger->log("bad move packet: %d", dir);
            }
        }
        delete [] moves;
    }
    dstBeing->setPath(path);
    BLOCK_END("BeingHandler::processBeingMove3")
}

}  // namespace Ea
