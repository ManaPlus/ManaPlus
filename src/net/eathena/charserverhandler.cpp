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

#include "gui/windows/charcreatedialog.h"

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

#include "resources/db/chardb.h"
#include "resources/db/itemdb.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

extern Net::CharServerHandler *charServerHandler;

namespace EAthena
{

extern ServerInfo charServer;
extern ServerInfo mapServer;

CharServerHandler::CharServerHandler() :
    MessageHandler(),
    Ea::CharServerHandler(),
    mPinSeed(0),
    mPinAccountId(0),
    mNeedCreatePin(false)
{
    static const uint16_t _messages[] =
    {
        SMSG_CHAR_LOGIN,
        SMSG_CHAR_LOGIN2,
        SMSG_CHAR_LOGIN_ERROR,
        SMSG_CHAR_CREATE_SUCCEEDED,
        SMSG_CHAR_CREATE_FAILED,
        SMSG_CHAR_DELETE_SUCCEEDED,
        SMSG_CHAR_DELETE_FAILED,
        SMSG_CHAR_MAP_INFO,
        SMSG_CHANGE_MAP_SERVER,
        SMSG_CHAR_PINCODE_STATUS,
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

        case SMSG_CHAR_LOGIN2:
            processCharLogin2(msg);
            break;

        case SMSG_CHAR_LOGIN_ERROR:
            processCharLoginError(msg);
            break;

        case SMSG_CHAR_CREATE_SUCCEEDED:
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

        case SMSG_CHAR_PINCODE_STATUS:
            processPincodeStatus(msg);
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

    LocalPlayer *const tempPlayer = new LocalPlayer(
        msg.readInt32("player id"), 0);
    tempPlayer->setGender(token.sex);

    PlayerInfoBackend &data = character->data;
    data.mAttributes[Attributes::EXP] = msg.readInt32("exp");
    data.mAttributes[Attributes::MONEY] = msg.readInt32("money");
    Stat &jobStat = data.mStats[JOB];
    jobStat.exp = msg.readInt32("job");

    const int temp = msg.readInt32("job level");
    jobStat.base = temp;
    jobStat.mod = temp;

    const int shoes = msg.readInt16();     // look like unused
    const int gloves = msg.readInt16();    // look like unused
    const int cape = msg.readInt16();      // look like unused
    const int misc1 = msg.readInt16();     // look like unused

    msg.readInt32("option");
    msg.readInt32("karma");
    msg.readInt32("manner");
    msg.readInt16("left points");

    data.mAttributes[Attributes::HP] = msg.readInt16("hp");
    data.mAttributes[Attributes::MAX_HP] = msg.readInt16("max hp");

    msg.skip(4, "unused");

    data.mAttributes[Attributes::MP] = msg.readInt16("mp/sp");
    data.mAttributes[Attributes::MAX_MP] = msg.readInt16("max mp/sp");

    msg.readInt16("speed");
    tempPlayer->setSubtype(msg.readInt16("class"), 0);
    const int hairStyle = msg.readInt16("hair style");
    const uint16_t weapon = msg.readInt16("weapon");

    tempPlayer->setSprite(SPRITE_WEAPON, weapon, "", 1, true);

    data.mAttributes[Attributes::LEVEL] = msg.readInt16("level");

    msg.readInt16("skill points");
    const int bottomClothes = msg.readInt16("head bottom");
    const int shield = msg.readInt16("shild");
    const int hat = msg.readInt16("head top");
    const int topClothes = msg.readInt16("head mid");

    tempPlayer->setSprite(SPRITE_HAIR, hairStyle * -1,
        ItemDB::get(-hairStyle).getDyeColorsString(
        msg.readInt16("hair color")));

    const int misc2 = msg.readInt16("clothes color");
    tempPlayer->setName(msg.readString(24));

    character->dummy = tempPlayer;

    for (int i = 0; i < 6; i++)
        character->data.mStats[i + STR].base = msg.readUInt8("stat");

    tempPlayer->setSprite(SPRITE_SHOE, shoes);
    tempPlayer->setSprite(SPRITE_GLOVES, gloves);
    tempPlayer->setSprite(SPRITE_CAPE, cape);
    tempPlayer->setSprite(SPRITE_MISC1, misc1);
    tempPlayer->setSprite(SPRITE_BOTTOMCLOTHES, bottomClothes);
    // to avoid show error (error.xml) need remove this sprite
    if (!config.getBoolValue("hideShield"))
        tempPlayer->setSprite(SPRITE_SHIELD, shield);

    tempPlayer->setSprite(SPRITE_HAT, hat);
    tempPlayer->setSprite(SPRITE_TOPCLOTHES, topClothes);
    tempPlayer->setSprite(SPRITE_MISC2, misc2);
    character->slot = msg.readInt16("character slot id");
    msg.readInt16("rename");
    msg.readString(16, "map name");
    msg.readInt32("delete date");
    msg.readInt32("robe");
    msg.readInt32("slot change");
    msg.readInt32("rename (inverse)");
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
                                     const std::vector<int> &stats A_UNUSED)
                                     const
{
    MessageOut outMsg(CMSG_CHAR_CREATE);
    outMsg.writeString(name, 24, "login");

    outMsg.writeInt8(static_cast<unsigned char>(slot), "slot");
    outMsg.writeInt16(static_cast<int16_t>(hairColor), "hair color");
    outMsg.writeInt16(static_cast<int16_t>(hairstyle), "hair style");
}

void CharServerHandler::deleteCharacter(Net::Character *const character)
{
    if (!character)
        return;

    mSelectedCharacter = character;

    MessageOut outMsg(CMSG_CHAR_DELETE);
    outMsg.writeInt32(mSelectedCharacter->dummy->getId(), "id?");
    outMsg.writeString("a@a.com", 40, "email");
}

void CharServerHandler::switchCharacter() const
{
    // This is really a map-server packet
    MessageOut outMsg(CMSG_PLAYER_RESTART);
    outMsg.writeInt8(1, "flag");
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
    outMsg.writeInt32(token.account_ID, "account id");
    outMsg.writeInt32(token.session_ID1, "session id1");
    outMsg.writeInt32(token.session_ID2, "session id2");
    outMsg.writeInt16(CLIENT_PROTOCOL_VERSION, "client protocol version");
    outMsg.writeInt8(Being::genderToInt(token.sex), "gender");

    // We get 4 useless bytes before the real answer comes in (what are these?)
    mNetwork->skip(4);
}

void CharServerHandler::setCharCreateDialog(CharCreateDialog *const window)
{
    mCharCreateDialog = window;

    if (!mCharCreateDialog)
        return;

    StringVect attributes;

    const Token &token =
        static_cast<LoginHandler*>(Net::getLoginHandler())->getToken();

    mCharCreateDialog->setAttributes(attributes, 0, 0, 0);
    mCharCreateDialog->setFixedGender(true, token.sex);
}

void CharServerHandler::processCharLogin(Net::MessageIn &msg)
{
    msg.skip(2, "packet len");
    const int slots = msg.readInt8("MAX_CHARS");
    msg.readInt8("sd->char_slots");
    msg.readInt8("MAX_CHARS");
    loginData.characterSlots = static_cast<uint16_t>(slots);

    msg.skip(20, "unused 0");

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

void CharServerHandler::processCharLogin2(Net::MessageIn &msg)
{
    msg.readInt16("len");
    msg.readUInt8("char slots");
    msg.readUInt8("left slots");
    msg.readUInt8("left slots");
    msg.readUInt8("char slots");
    msg.readUInt8("char slots");
    msg.skip(20, "unused");
}

void CharServerHandler::processCharMapInfo(Net::MessageIn &restrict msg)
{
    Network *const network = mNetwork;
    ServerInfo &server = mapServer;
    BLOCK_START("CharServerHandler::processCharMapInfo")
    PlayerInfo::setCharId(msg.readInt32("char id"));
    GameHandler *const gh = static_cast<GameHandler*>(Net::getGameHandler());
    gh->setMap(msg.readString(16, "map name"));
    if (config.getBoolValue("usePersistentIP") || settings.persistentIp)
    {
        msg.readInt32("map ip address");
        server.hostname = settings.serverName;
    }
    else
    {
        server.hostname = ipToString(msg.readInt32("map ip address"));
    }
    server.port = msg.readInt16("map ip port");

    // Prevent the selected local player from being deleted
    localPlayer = mSelectedCharacter->dummy;
    PlayerInfo::setBackend(mSelectedCharacter->data);

    mSelectedCharacter->dummy = nullptr;

    charServerHandler->clear();
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
    gh->setMap(msg.readString(16, "map name"));
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    server.hostname = ipToString(msg.readInt32("host"));
    server.port = msg.readInt16("port");

    network->disconnect();
    client->setState(STATE_CHANGE_MAP);
    if (localPlayer)
    {
        localPlayer->setTileCoords(x, y);
        localPlayer->setMap(nullptr);
    }
    BLOCK_END("CharServerHandler::processChangeMapServer")
}

void CharServerHandler::processPincodeStatus(Net::MessageIn &msg)
{
    mPinSeed = msg.readInt32("pincode seed");
    mPinAccountId = msg.readInt32("account id");
    const uint16_t state = static_cast<uint16_t>(msg.readInt16("state"));
    switch (state)
    {
        case 0:  // pin ok
            break;
        case 1:  // ask for pin
            break;
        case 2:  // create new pin
        case 4:  // create new pin?
        {
            mNeedCreatePin = true;
            break;
        }
        case 3:  // pin must be changed
            break;
        case 5:  // client show error?
            break;
        case 6:  // Unable to use your KSSN number
            break;
        case 7:  // char select window shows a button
            break;
        case 8:  // pincode was incorrect
            break;
        default:
            logger->log("processPincodeStatus: unknown pin state: %d",
                static_cast<int>(state));
            break;
    }
}

void CharServerHandler::setNewPincode(const std::string &pin A_UNUSED)
{
//  here need ecript pin with mPinSeed and pin values.

//    MessageOut outMsg(CMSG_CHAR_CREATE_PIN);
//    outMsg.writeInt32(mPinAccountId, "account id");
//    outMsg.writeString(pin, 4, "encrypted pin");
}

void CharServerHandler::processCharCreate(Net::MessageIn &msg)
{
    BLOCK_START("CharServerHandler::processCharCreate")
    Net::Character *const character = new Net::Character;
    readPlayerData(msg, character, false);
    mCharacters.push_back(character);

    updateCharSelectDialog();

    // Close the character create dialog
    if (mCharCreateDialog)
    {
        mCharCreateDialog->scheduleDelete();
        mCharCreateDialog = nullptr;
    }
    BLOCK_END("CharServerHandler::processCharCreate")
}

}  // namespace EAthena
