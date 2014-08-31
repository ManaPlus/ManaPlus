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

#include "net/eathena/charserverhandler.h"

#include "client.h"
#include "configuration.h"
#include "settings.h"

#include "being/attributes.h"

#include "net/character.h"
#include "net/logindata.h"
#include "net/net.h"

#include "net/eathena/attrs.h"
#include "net/eathena/gamehandler.h"
#include "net/eathena/loginhandler.h"
#include "net/eathena/messageout.h"
#include "net/eathena/network.h"
#include "net/eathena/protocol.h"
#include "net/eathena/sprite.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/dtor.h"

#include "debug.h"

extern Net::CharServerHandler *charServerHandler;

namespace EAthena
{

extern ServerInfo charServer;
extern ServerInfo mapServer;

CharServerHandler::CharServerHandler() :
    MessageHandler(),
    Ea::CharServerHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_CHAR_LOGIN,
        SMSG_CHAR_LOGIN_ERROR,
        SMSG_CHAR_CREATE_SUCCEEDED,
        SMSG_CHAR_CREATE_SUCCEEDED2,
        SMSG_CHAR_CREATE_FAILED,
        SMSG_CHAR_DELETE_SUCCEEDED,
        SMSG_CHAR_DELETE_FAILED,
        SMSG_CHAR_MAP_INFO,
        SMSG_CHANGE_MAP_SERVER,
        0
    };
    handledMessages = _messages;
    charServerHandler = this;
}

void CharServerHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_CHAR_LOGIN:
            processCharLogin(msg);
            break;

        case SMSG_CHAR_LOGIN_ERROR:
            processCharLoginError(msg);
            break;

        case SMSG_CHAR_CREATE_SUCCEEDED:
        case SMSG_CHAR_CREATE_SUCCEEDED2:
            processCharCreate(msg);
            break;

        case SMSG_CHAR_CREATE_FAILED:
            processCharCreateFailed(msg);
            break;

        case SMSG_CHAR_DELETE_SUCCEEDED:
            processCharDelete(msg);
            break;

        case SMSG_CHAR_DELETE_FAILED:
            processCharDeleteFailed(msg);
            break;

        case SMSG_CHAR_MAP_INFO:
            processCharMapInfo(msg);
            break;

        case SMSG_CHANGE_MAP_SERVER:
            processChangeMapServer(msg);
            break;

        default:
            break;
    }
}

void CharServerHandler::readPlayerData(Net::MessageIn &msg,
                                       Net::Character *const character,
                                       const bool) const
{
    if (!character)
        return;

    const Token &token =
        static_cast<LoginHandler*>(Net::getLoginHandler())->getToken();

    LocalPlayer *const tempPlayer = new LocalPlayer(msg.readInt32(), 0);
    tempPlayer->setGender(token.sex);

    PlayerInfoBackend &data = character->data;
    data.mAttributes[Attributes::EXP] = msg.readInt32();
    data.mAttributes[Attributes::MONEY] = msg.readInt32();
    Stat &jobStat = data.mStats[JOB];
    jobStat.exp = msg.readInt32();

    const int temp = msg.readInt32();   // job_level
    jobStat.base = temp;
    jobStat.mod = temp;

    const int shoes = msg.readInt16();     // look like unused
    const int gloves = msg.readInt16();    // look like unused
    const int cape = msg.readInt16();      // look like unused
    const int misc1 = msg.readInt16();     // look like unused

    msg.readInt32();                       // option
    msg.readInt32();                       // karma
    msg.readInt32();                       // manner
    msg.readInt16();                       // character points left

    data.mAttributes[Attributes::HP] = msg.readInt16();
    data.mAttributes[Attributes::MAX_HP] = msg.readInt16();

    msg.skip(4);    // unused

    data.mAttributes[Attributes::MP] = msg.readInt16();     // sp
    data.mAttributes[Attributes::MAX_MP] = msg.readInt16(); // max sp

    msg.readInt16();                             // speed
    tempPlayer->setSubtype(msg.readInt16(), 0);  // class (used for race)
    const int hairStyle = msg.readInt16();
    const uint16_t weapon = msg.readInt16();     // weapon or non (for riding?)

    tempPlayer->setSprite(SPRITE_WEAPON, weapon, "", 1, true);

    data.mAttributes[Attributes::LEVEL] = msg.readInt16();

    msg.readInt16();  // skill point
    const int bottomClothes = msg.readInt16();  // head_bottom
    const int shield = msg.readInt16();         // shield

    const int hat = msg.readInt16();            // head_top
    const int topClothes = msg.readInt16();     // head_mid

    tempPlayer->setSprite(SPRITE_HAIR, hairStyle * -1,  // hair_color
        ItemDB::get(-hairStyle).getDyeColorsString(msg.readInt16()));

    const int misc2 = msg.readInt16();          // clothes_color
    tempPlayer->setName(msg.readString(24));

    character->dummy = tempPlayer;

    for (int i = 0; i < 6; i++)
        character->data.mStats[i + STR].base = msg.readUInt8();

    tempPlayer->setSprite(SPRITE_SHOE, shoes);
    tempPlayer->setSprite(SPRITE_GLOVES, gloves);
    tempPlayer->setSprite(SPRITE_CAPE, cape);
    tempPlayer->setSprite(SPRITE_MISC1, misc1);
    tempPlayer->setSprite(SPRITE_BOTTOMCLOTHES, bottomClothes);
    // to avoid show error (error.xml) need remove this sprite
    if (!config.getBoolValue("hideShield"))
        tempPlayer->setSprite(SPRITE_SHIELD, shield);

    tempPlayer->setSprite(SPRITE_HAT, hat);  // head option top
    tempPlayer->setSprite(SPRITE_TOPCLOTHES, topClothes);
    tempPlayer->setSprite(SPRITE_MISC2, misc2);
    character->slot = msg.readInt16();  // character slot
    msg.readInt16();     // rename (1, 0) addons bar?
    msg.readString(16);  // map name
    msg.readInt32();     // delete_date
    msg.readInt32();     // robe
    msg.readInt32();     // slotchange (1, 0)
    msg.readInt32();     // rename (0, 1) addons bar?
}

void CharServerHandler::chooseCharacter(Net::Character *const character)
{
    if (!character)
        return;

    mSelectedCharacter = character;
    mCharSelectDialog = nullptr;

    MessageOut outMsg(CMSG_CHAR_SELECT);
    outMsg.writeInt8(static_cast<unsigned char>(mSelectedCharacter->slot));
}

void CharServerHandler::newCharacter(const std::string &name, const int slot,
                                     const bool gender A_UNUSED,
                                     const int hairstyle, const int hairColor,
                                     const unsigned char race A_UNUSED,
                                     const unsigned char look A_UNUSED,
                                     const std::vector<int> &stats) const
{
    MessageOut outMsg(CMSG_CHAR_CREATE);
    outMsg.writeString(name, 24);
    for (int i = 0; i < 6; i++)
        outMsg.writeInt8(static_cast<unsigned char>(stats[i]));

    outMsg.writeInt8(static_cast<unsigned char>(slot));
    outMsg.writeInt16(static_cast<int16_t>(hairColor));
    outMsg.writeInt16(static_cast<int16_t>(hairstyle));
}

void CharServerHandler::deleteCharacter(Net::Character *const character)
{
    if (!character)
        return;

    mSelectedCharacter = character;

    MessageOut outMsg(CMSG_CHAR_DELETE);
    outMsg.writeInt32(mSelectedCharacter->dummy->getId());
    outMsg.writeString("a@a.com", 40);
}

void CharServerHandler::switchCharacter() const
{
    // This is really a map-server packet
    MessageOut outMsg(CMSG_PLAYER_RESTART);
    outMsg.writeInt8(1);
}

void CharServerHandler::connect()
{
    const Token &token =
        static_cast<LoginHandler*>(Net::getLoginHandler())->getToken();

    if (!mNetwork)
        return;

    mNetwork->disconnect();
    mNetwork->connect(charServer);
    MessageOut outMsg(CMSG_CHAR_SERVER_CONNECT);
    outMsg.writeInt32(token.account_ID);
    outMsg.writeInt32(token.session_ID1);
    outMsg.writeInt32(token.session_ID2);
    outMsg.writeInt16(CLIENT_PROTOCOL_VERSION);
    outMsg.writeInt8(Being::genderToInt(token.sex));

    // We get 4 useless bytes before the real answer comes in (what are these?)
    mNetwork->skip(4);
}

void CharServerHandler::processCharLogin(Net::MessageIn &msg)
{
    msg.skip(2);  // Length word
    const int slots = msg.readInt8();  // MAX_CHARS
    msg.readInt8();  // sd->char_slots
    msg.readInt8();  // MAX_CHARS

    if (slots > 0 && slots < 30)
        loginData.characterSlots = static_cast<uint16_t>(slots);

    msg.skip(20);  // 0 Unused

    delete_all(mCharacters);
    mCharacters.clear();

    // Derive number of characters from message length
    const int count = (msg.getLength() - 27)
        / (106 + 4 + 2 + 16 + 4 + 4 + 4 + 4);

    for (int i = 0; i < count; ++i)
    {
        Net::Character *const character = new Net::Character;
        readPlayerData(msg, character, false);
        mCharacters.push_back(character);
        if (character->dummy)
        {
            logger->log("CharServer: Player: %s (%d)",
                character->dummy->getName().c_str(), character->slot);
        }
    }

    client->setState(STATE_CHAR_SELECT);
}

void CharServerHandler::processCharMapInfo(Net::MessageIn &restrict msg)
{
    Network *const network = mNetwork;
    ServerInfo &server = mapServer;
    BLOCK_START("CharServerHandler::processCharMapInfo")
//    msg.skip(4); // CharID, must be the same as localPlayer->charID
    PlayerInfo::setCharId(msg.readInt32());
    GameHandler *const gh = static_cast<GameHandler*>(Net::getGameHandler());
    gh->setMap(msg.readString(16));
    if (config.getBoolValue("usePersistentIP") || settings.persistentIp)
    {
        msg.readInt32();
        server.hostname = settings.serverName;
    }
    else
    {
        server.hostname = ipToString(msg.readInt32());
    }
    server.port = msg.readInt16();

    // Prevent the selected local player from being deleted
    localPlayer = mSelectedCharacter->dummy;
    PlayerInfo::setBackend(mSelectedCharacter->data);

    mSelectedCharacter->dummy = nullptr;

    Net::getCharServerHandler()->clear();
    updateCharSelectDialog();

    if (network)
        network->disconnect();
    client->setState(STATE_CONNECT_GAME);
    BLOCK_END("CharServerHandler::processCharMapInfo")
}

void CharServerHandler::processChangeMapServer(Net::MessageIn &msg)
{
    Network *const network = mNetwork;
    ServerInfo &server = mapServer;
    BLOCK_START("CharServerHandler::processChangeMapServer")
    GameHandler *const gh = static_cast<GameHandler*>(Net::getGameHandler());
    if (!gh || !network)
    {
        BLOCK_END("CharServerHandler::processChangeMapServer")
        return;
    }
    gh->setMap(msg.readString(16));
    const int x = msg.readInt16();
    const int y = msg.readInt16();
    server.hostname = ipToString(msg.readInt32());
    server.port = msg.readInt16();

    network->disconnect();
    client->setState(STATE_CHANGE_MAP);
    if (localPlayer)
    {
        localPlayer->setTileCoords(x, y);
        localPlayer->setMap(nullptr);
    }
    BLOCK_END("CharServerHandler::processChangeMapServer")
}

}  // namespace EAthena
