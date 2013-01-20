/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "net/tmwa/charserverhandler.h"

#include "client.h"
#include "configuration.h"

#include "gui/charcreatedialog.h"

#include "net/logindata.h"

#include "net/tmwa/gamehandler.h"
#include "net/tmwa/loginhandler.h"
#include "net/tmwa/network.h"
#include "net/tmwa/protocol.h"

#include "resources/colordb.h"
#include "resources/itemdb.h"
#include "resources/iteminfo.h"

#include "utils/dtor.h"

#include "debug.h"

extern Net::CharHandler *charHandler;

namespace TmwAthena
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
    charHandler = this;
}

void CharServerHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("CharServerHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_CHAR_LOGIN:
            processCharLogin(msg);
            break;

        case SMSG_CHAR_LOGIN_ERROR:
            processCharLoginError(msg);
            break;

        case SMSG_CHAR_CREATE_SUCCEEDED:
            processCharCreate(msg, false);
            break;

        case SMSG_CHAR_CREATE_SUCCEEDED2:
            processCharCreate(msg, true);
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
        {
//            msg.skip(4); // CharID, must be the same as player_node->charID
            PlayerInfo::setCharId(msg.readInt32());
            GameHandler *const gh = static_cast<GameHandler*>(
                Net::getGameHandler());
            gh->setMap(msg.readString(16));
            if (config.getBoolValue("usePersistentIP"))
            {
                msg.readInt32();
                mapServer.hostname = Client::getServerName();
            }
            else
            {
                mapServer.hostname = ipToString(msg.readInt32());
            }
            mapServer.port = msg.readInt16();

            // Prevent the selected local player from being deleted
            player_node = mSelectedCharacter->dummy;
            PlayerInfo::setBackend(mSelectedCharacter->data);

            mSelectedCharacter->dummy = nullptr;

            delete_all(mCharacters);
            mCharacters.clear();
            updateCharSelectDialog();

            if (mNetwork)
                mNetwork->disconnect();
            Client::setState(STATE_CONNECT_GAME);
        }
        break;

        case SMSG_CHANGE_MAP_SERVER:
        {
            GameHandler *const gh = static_cast<GameHandler*>(
                Net::getGameHandler());
            if (!gh || !mNetwork)
                return;
            gh->setMap(msg.readString(16));
            const int x = msg.readInt16();
            const int y = msg.readInt16();
            mapServer.hostname = ipToString(msg.readInt32());
            mapServer.port = msg.readInt16();

            mNetwork->disconnect();
            Client::setState(STATE_CHANGE_MAP);
            if (player_node)
            {
                player_node->setTileCoords(x, y);
                player_node->setMap(nullptr);
            }
        }
        break;

        default:
            break;
    }
    BLOCK_END("CharServerHandler::handleMessage")
}

void CharServerHandler::readPlayerData(Net::MessageIn &msg,
                                       Net::Character *character,
                                       bool withColors)
{
    if (!character)
        return;

    const Token &token =
        static_cast<LoginHandler*>(Net::getLoginHandler())->getToken();

    LocalPlayer *const tempPlayer = new LocalPlayer(msg.readInt32(), 0);
    tempPlayer->setGender(token.sex);

    PlayerInfoBackend &data = character->data;
    data.mAttributes[PlayerInfo::EXP] = msg.readInt32();
    data.mAttributes[PlayerInfo::MONEY] = msg.readInt32();
    Stat &jobStat = data.mStats[static_cast<size_t>(JOB)];
    jobStat.exp = msg.readInt32();

    const int temp = msg.readInt32();
    jobStat.base = temp;
    jobStat.mod = temp;

    const int shoes = msg.readInt16();
    const int gloves = msg.readInt16();
    const int cape = msg.readInt16();
    const int misc1 = msg.readInt16();

    msg.readInt32();                       // option
    msg.readInt32();                       // karma
    msg.readInt32();                       // manner
    msg.readInt16();                       // character points left

    data.mAttributes[PlayerInfo::HP] = msg.readInt16();
    data.mAttributes[PlayerInfo::MAX_HP] = msg.readInt16();
    data.mAttributes[PlayerInfo::MP] = msg.readInt16();
    data.mAttributes[PlayerInfo::MAX_MP] = msg.readInt16();

    msg.readInt16();                       // speed
    tempPlayer->setSubtype(msg.readInt16()); // class (used for race)
    const int hairStyle = msg.readInt16();
    const uint16_t weapon = msg.readInt16();  // unused on server. need use?
    tempPlayer->setSprite(SPRITE_WEAPON, weapon, "", 1, true);

    data.mAttributes[PlayerInfo::LEVEL] = msg.readInt16();

    msg.readInt16();                       // skill point
    const int bottomClothes = msg.readInt16();
    const int shield = msg.readInt16();

    const int hat = msg.readInt16(); // head option top
    const int topClothes = msg.readInt16();

    int hairColor = msg.readInt16();
    if (hairColor > 255)
        hairColor = 255;
    tempPlayer->setSprite(SPRITE_HAIR, hairStyle * -1,
        ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
    tempPlayer->setHairColor(static_cast<unsigned char>(hairColor));

    const int misc2 = msg.readInt16();
    tempPlayer->setName(msg.readString(24));

    character->dummy = tempPlayer;

    for (int i = 0; i < 6; i++)
        character->data.mStats[i + STR].base = msg.readInt8();

    if (withColors)
    {
        tempPlayer->setSprite(SPRITE_SHOE, shoes, "", msg.readInt8());
        tempPlayer->setSprite(SPRITE_GLOVES, gloves, "", msg.readInt8());
        tempPlayer->setSprite(SPRITE_CAPE, cape, "", msg.readInt8());
        tempPlayer->setSprite(SPRITE_MISC1, misc1, "", msg.readInt8());
        tempPlayer->setSprite(SPRITE_BOTTOMCLOTHES, bottomClothes,
            "", msg.readInt8());
        //to avoid show error (error.xml) need remove this sprite
        if (!config.getBoolValue("hideShield"))
            tempPlayer->setSprite(SPRITE_SHIELD, shield, "", msg.readInt8());
        else
            msg.readInt8();

        tempPlayer->setSprite(SPRITE_HAT, hat, "",
            msg.readInt8()); // head option top
        tempPlayer->setSprite(SPRITE_TOPCLOTHES, topClothes, "",
            msg.readInt8());
        tempPlayer->setSprite(SPRITE_MISC2, misc2, "", msg.readInt8());
        msg.skip(5);
        character->slot = msg.readInt8(); // character slot
    }
    else
    {
        tempPlayer->setSprite(SPRITE_SHOE, shoes);
        tempPlayer->setSprite(SPRITE_GLOVES, gloves);
        tempPlayer->setSprite(SPRITE_CAPE, cape);
        tempPlayer->setSprite(SPRITE_MISC1, misc1);
        tempPlayer->setSprite(SPRITE_BOTTOMCLOTHES, bottomClothes);
        //to avoid show error (error.xml) need remove this sprite
        if (!config.getBoolValue("hideShield"))
            tempPlayer->setSprite(SPRITE_SHIELD, shield);

        tempPlayer->setSprite(SPRITE_HAT, hat); // head option top
        tempPlayer->setSprite(SPRITE_TOPCLOTHES, topClothes);
        tempPlayer->setSprite(SPRITE_MISC2, misc2);
        character->slot = msg.readInt8(); // character slot
    }

    msg.readInt8();                        // unknown
}

void CharServerHandler::chooseCharacter(Net::Character *character)
{
    if (!character)
        return;

    mSelectedCharacter = character;
    mCharSelectDialog = nullptr;

    MessageOut outMsg(CMSG_CHAR_SELECT);
    outMsg.writeInt8(static_cast<unsigned char>(mSelectedCharacter->slot));
}

void CharServerHandler::newCharacter(const std::string &name, int slot,
                                     bool gender A_UNUSED, int hairstyle,
                                     int hairColor, unsigned char race,
                                     const std::vector<int> &stats)
{
    MessageOut outMsg(CMSG_CHAR_CREATE);
    outMsg.writeString(name, 24);
    for (int i = 0; i < 6; i++)
        outMsg.writeInt8(static_cast<unsigned char>(stats[i]));

    outMsg.writeInt8(static_cast<unsigned char>(slot));
    outMsg.writeInt16(static_cast<short>(hairColor));
    outMsg.writeInt16(static_cast<short>(hairstyle));
    if (serverVersion >= 2)
        outMsg.writeInt8(race);
}

void CharServerHandler::deleteCharacter(Net::Character *character)
{
    if (!character)
        return;

    mSelectedCharacter = character;

    MessageOut outMsg(CMSG_CHAR_DELETE);
    outMsg.writeInt32(mSelectedCharacter->dummy->getId());
    outMsg.writeString("a@a.com", 40);
}

void CharServerHandler::switchCharacter()
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
    // [Fate] The next word is unused by the old char server, so we squeeze in
    //        mana client version information
    if (serverVersion > 0)
        outMsg.writeInt16(CLIENT_PROTOCOL_VERSION);
    else
        outMsg.writeInt16(CLIENT_TMW_PROTOCOL_VERSION);
    outMsg.writeInt8(Being::genderToInt(token.sex));

    // We get 4 useless bytes before the real answer comes in (what are these?)
    mNetwork->skip(4);
}

void CharServerHandler::processCharLogin(Net::MessageIn &msg)
{
    msg.skip(2);  // Length word
    const int slots = msg.readInt16();
    if (slots > 0 && slots < 30)
        loginData.characterSlots = static_cast<short unsigned int>(slots);

    const bool version = msg.readInt8() == 1 && serverVersion > 0;
    msg.skip(17); // 0 Unused

    delete_all(mCharacters);
    mCharacters.clear();

    // Derive number of characters from message length
    int count = (msg.getLength() - 24);
    if (version)
        count /= 120;
    else
        count /= 106;

    for (int i = 0; i < count; ++i)
    {
        Net::Character *const character = new Net::Character;
        readPlayerData(msg, character, version);
        mCharacters.push_back(character);
        if (character && character->dummy)
        {
            logger->log("CharServer: Player: %s (%d)",
                character->dummy->getName().c_str(), character->slot);
        }
    }

    Client::setState(STATE_CHAR_SELECT);
}

} // namespace TmwAthena
