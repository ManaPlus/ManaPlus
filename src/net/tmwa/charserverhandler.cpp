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

#include "net/tmwa/charserverhandler.h"

#include "client.h"
#include "configuration.h"
#include "settings.h"

#include "enums/being/attributes.h"

#include "enums/gui/dialogtype.h"

#include "gui/windows/charcreatedialog.h"
#include "gui/windows/okdialog.h"

#include "net/character.h"
#include "net/serverfeatures.h"

#include "net/tmwa/gamehandler.h"
#include "net/tmwa/loginhandler.h"
#include "net/tmwa/messageout.h"
#include "net/tmwa/network.h"
#include "net/tmwa/protocol.h"
#include "net/tmwa/sprite.h"

#include "resources/iteminfo.h"

#include "resources/db/chardb.h"
#include "resources/db/itemdb.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

extern Net::CharServerHandler *charServerHandler;

namespace TmwAthena
{

extern ServerInfo mapServer;

extern ServerInfo charServer;

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
            processCharCreate(msg);
            break;

        case SMSG_CHAR_CREATE_SUCCEEDED2:
            processCharCreate2(msg);
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
    BLOCK_END("CharServerHandler::handleMessage")
}

void CharServerHandler::readPlayerData(Net::MessageIn &msg,
                                       Net::Character *const character,
                                       const bool withColors) const
{
    if (!character)
        return;

    const Token &token =
        static_cast<LoginHandler*>(loginHandler)->getToken();

    LocalPlayer *const tempPlayer = new LocalPlayer(
        msg.readInt32("account id"), 0);
    tempPlayer->setGender(token.sex);

    PlayerInfoBackend &data = character->data;
    data.mAttributes[Attributes::EXP] = msg.readInt32("exp");
    data.mAttributes[Attributes::MONEY] = msg.readInt32("money");
    Stat &jobStat = data.mStats[static_cast<size_t>(Attributes::JOB)];
    jobStat.exp = msg.readInt32("job");

    const int temp = msg.readInt32("job level");
    jobStat.base = temp;
    jobStat.mod = temp;

    const int shoes = msg.readInt16("shoes");
    const int gloves = msg.readInt16("gloves");
    const int cape = msg.readInt16("cape");
    const int misc1 = msg.readInt16("misc1");

    msg.readInt32("option");
    tempPlayer->setKarma(msg.readInt32("karma"));
    tempPlayer->setManner(msg.readInt32("manner"));
    msg.readInt16("character points left");

    data.mAttributes[Attributes::HP] = msg.readInt16("hp");
    data.mAttributes[Attributes::MAX_HP] = msg.readInt16("max hp");
    data.mAttributes[Attributes::MP] = msg.readInt16("mp");
    data.mAttributes[Attributes::MAX_MP] = msg.readInt16("max mp");

    msg.readInt16("speed");
    const uint16_t race = msg.readInt16("class");
    const uint8_t hairStyle = msg.readUInt8("hair style");
    const uint16_t look = msg.readUInt8("look");
    tempPlayer->setSubtype(race, look);
    const uint16_t weapon = msg.readInt16("weapon");
    tempPlayer->setSprite(SPRITE_BODY, weapon, "", 1, true);

    data.mAttributes[Attributes::LEVEL] = msg.readInt16("level");

    msg.readInt16("skill point");
    const int bottomClothes = msg.readInt16("bottom clothes");
    const int shield = msg.readInt16("shield");

    const int hat = msg.readInt16("hat");
    const int topClothes = msg.readInt16("top clothes");

    const uint8_t hairColor = msg.readUInt8("hair color");
    msg.readUInt8("unused");
    tempPlayer->setSprite(SPRITE_HAIR_COLOR, hairStyle * -1,
        ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
    tempPlayer->setHairColor(static_cast<unsigned char>(hairColor));

    const int misc2 = msg.readInt16("misc2");
    tempPlayer->setName(msg.readString(24, "name"));

    character->dummy = tempPlayer;

    for (int i = 0; i < 6; i++)
    {
        character->data.mStats[i + Attributes::STR].base
            = msg.readUInt8("stat");
    }

    if (withColors)
    {
        tempPlayer->setSprite(SPRITE_HAIR, shoes, "",
            msg.readUInt8("shoes color"));
        tempPlayer->setSprite(SPRITE_SHOES, gloves, "",
            msg.readUInt8("gloves color"));
        tempPlayer->setSprite(SPRITE_SHIELD, cape, "",
            msg.readUInt8("cape color"));
        tempPlayer->setSprite(SPRITE_HEAD_TOP, misc1, "",
            msg.readUInt8("misc1 color"));
        tempPlayer->setSprite(SPRITE_WEAPON, bottomClothes,
            "", msg.readUInt8("bottom clothes color"));
        tempPlayer->setSprite(SPRITE_FLOOR, shield, "",
            msg.readUInt8("shield color"));
        tempPlayer->setSprite(SPRITE_CLOTHES_COLOR, hat, "",
            msg.readUInt8("head option top color"));
        tempPlayer->setSprite(SPRITE_HEAD_BOTTOM, topClothes, "",
            msg.readUInt8("top clothes color"));
        tempPlayer->setSprite(SPRITE_HEAD_MID, misc2, "",
            msg.readUInt8("misc2 color"));
        msg.skip(5, "unused");
    }
    else
    {
        tempPlayer->setSprite(SPRITE_HAIR, shoes);
        tempPlayer->setSprite(SPRITE_SHOES, gloves);
        tempPlayer->setSprite(SPRITE_SHIELD, cape);
        tempPlayer->setSprite(SPRITE_HEAD_TOP, misc1);
        tempPlayer->setSprite(SPRITE_WEAPON, bottomClothes);
        tempPlayer->setSprite(SPRITE_FLOOR, shield);
        tempPlayer->setSprite(SPRITE_CLOTHES_COLOR, hat);  // head option top
        tempPlayer->setSprite(SPRITE_HEAD_BOTTOM, topClothes);
        tempPlayer->setSprite(SPRITE_HEAD_MID, misc2);
    }
    character->slot = msg.readUInt8("slot");
    msg.readUInt8("unused");
}

void CharServerHandler::chooseCharacter(Net::Character *const character)
{
    if (!character)
        return;

    mSelectedCharacter = character;
    mCharSelectDialog = nullptr;

    createOutPacket(CMSG_CHAR_SELECT);
    outMsg.writeInt8(static_cast<unsigned char>(mSelectedCharacter->slot),
        "slot");
}

void CharServerHandler::newCharacter(const std::string &name, const int slot,
                                     const Gender::Type gender A_UNUSED,
                                     const int hairstyle,
                                     const int hairColor,
                                     const unsigned char race,
                                     const uint16_t look,
                                     const std::vector<int> &stats) const
{
    createOutPacket(CMSG_CHAR_CREATE);
    outMsg.writeString(name, 24, "name");
    for (int i = 0; i < 6; i++)
        outMsg.writeInt8(static_cast<unsigned char>(stats[i]), "stat");

    outMsg.writeInt8(static_cast<unsigned char>(slot), "slot");
    outMsg.writeInt8(static_cast<int8_t>(hairColor), "hair color");
    outMsg.writeInt8(0, "unused");
    outMsg.writeInt8(static_cast<int8_t>(hairstyle), "hair style");
    if (serverFeatures->haveLookSelection())
        outMsg.writeInt8(static_cast<int8_t>(look), "look");
    else
        outMsg.writeInt8(0, "unused");
    if (serverFeatures->haveRaceSelection())
        outMsg.writeInt8(race, "class");
}

void CharServerHandler::deleteCharacter(Net::Character *const character,
                                        const std::string &email A_UNUSED)
{
    if (!character)
        return;

    mSelectedCharacter = character;

    createOutPacket(CMSG_CHAR_DELETE);
    outMsg.writeInt32(mSelectedCharacter->dummy->getId(), "id?");
    outMsg.writeString("a@a.com", 40, "email");
}

void CharServerHandler::switchCharacter() const
{
    // This is really a map-server packet
    createOutPacket(CMSG_PLAYER_RESTART);
    outMsg.writeInt8(1, "flag");
}

void CharServerHandler::connect()
{
    const Token &token =
        static_cast<LoginHandler*>(loginHandler)->getToken();

    if (!mNetwork)
        return;

    mNetwork->disconnect();
    mNetwork->connect(charServer);
    createOutPacket(CMSG_CHAR_SERVER_CONNECT);
    outMsg.writeInt32(token.account_ID, "account id");
    outMsg.writeInt32(token.session_ID1, "session id1");
    outMsg.writeInt32(token.session_ID2, "session id2");
    // [Fate] The next word is unused by the old char server, so we squeeze in
    //        mana client version information
    if (serverVersion > 0)
    {
        outMsg.writeInt16(CLIENT_PROTOCOL_VERSION, "client protocol version");
    }
    else
    {
        outMsg.writeInt16(CLIENT_TMW_PROTOCOL_VERSION,
            "client protocol version");
    }
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
    // TRANSLATORS: playe stat
    attributes.push_back(_("Strength:"));
    // TRANSLATORS: playe stat
    attributes.push_back(_("Agility:"));
    // TRANSLATORS: playe stat
    attributes.push_back(_("Vitality:"));
    // TRANSLATORS: playe stat
    attributes.push_back(_("Intelligence:"));
    // TRANSLATORS: playe stat
    attributes.push_back(_("Dexterity:"));
    // TRANSLATORS: playe stat
    attributes.push_back(_("Luck:"));

    const Token &token = static_cast<LoginHandler*>(loginHandler)->getToken();

    int minStat = CharDB::getMinStat();
    if (!minStat)
        minStat = 1;
    int maxStat = CharDB::getMaxStat();
    if (!maxStat)
        maxStat = 9;
    int sumStat = CharDB::getSumStat();
    if (!sumStat)
        sumStat = 30;

    mCharCreateDialog->setAttributes(attributes, sumStat, minStat, maxStat);
    mCharCreateDialog->setDefaultGender(token.sex);
}

void CharServerHandler::processCharLogin(Net::MessageIn &msg)
{
    BLOCK_START("CharServerHandler::processCharLogin")

    msg.readInt16("len");
    const int slots = msg.readInt16("slots");
    if (slots > 0 && slots < 30)
        loginData.characterSlots = static_cast<uint16_t>(slots);

    const bool version = msg.readUInt8("version") == 1 && serverVersion > 0;
    msg.skip(17, "unused");

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
        if (character->dummy)
        {
            logger->log("CharServer: Player: %s (%d)",
                character->dummy->getName().c_str(), character->slot);
        }
    }

    client->setState(STATE_CHAR_SELECT);
    BLOCK_END("CharServerHandler::processCharLogin")
}

void CharServerHandler::processCharMapInfo(Net::MessageIn &restrict msg)
{
    Network *const network = mNetwork;
    ServerInfo &server = mapServer;
    BLOCK_START("CharServerHandler::processCharMapInfo")
    PlayerInfo::setCharId(msg.readInt32("char id?"));
    GameHandler *const gh = static_cast<GameHandler*>(gameHandler);
    gh->setMap(msg.readString(16, "map name"));
    if (config.getBoolValue("usePersistentIP") || settings.persistentIp)
    {
        msg.readInt32("ip address");
        server.hostname = settings.serverName;
    }
    else
    {
        server.hostname = ipToString(msg.readInt32("ip address"));
    }
    server.port = msg.readInt16("port");

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
    GameHandler *const gh = static_cast<GameHandler*>(gameHandler);
    if (!gh || !network)
    {
        BLOCK_END("CharServerHandler::processChangeMapServer")
        return;
    }
    gh->setMap(msg.readString(16, "map name"));
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    server.hostname = ipToString(msg.readInt32("ip address"));
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

void CharServerHandler::processCharCreate(Net::MessageIn &msg)
{
    BLOCK_START("CharServerHandler::processCharCreate")
    Net::Character *const character = new Net::Character;
    charServerHandler->readPlayerData(msg, character, false);
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

void CharServerHandler::processCharCreate2(Net::MessageIn &msg)
{
    BLOCK_START("CharServerHandler::processCharCreate2")
    Net::Character *const character = new Net::Character;
    charServerHandler->readPlayerData(msg, character, true);
    mCharacters.push_back(character);

    updateCharSelectDialog();

    // Close the character create dialog
    if (mCharCreateDialog)
    {
        mCharCreateDialog->scheduleDelete();
        mCharCreateDialog = nullptr;
    }
    BLOCK_END("CharServerHandler::processCharCreate2")
}

void CharServerHandler::renameCharacter(const int id A_UNUSED,
                                        const std::string &newName A_UNUSED)
{
}

void CharServerHandler::changeSlot(const int oldSlot A_UNUSED,
                                   const int newSlot A_UNUSED)
{
}

void CharServerHandler::processCharDeleteFailed(Net::MessageIn &msg)
{
    BLOCK_START("CharServerHandler::processCharDeleteFailed")
    unlockCharSelectDialog();
    msg.readUInt8("error");
    // TRANSLATORS: error message
    new OkDialog(_("Error"), _("Failed to delete character."),
        // TRANSLATORS: ok dialog button
        _("OK"),
        DialogType::ERROR,
        true, true, nullptr, 260);
    BLOCK_END("CharServerHandler::processCharDeleteFailed")
}

void CharServerHandler::ping() const
{
}

unsigned int CharServerHandler::hatSprite() const
{
    return 7;
}

}  // namespace TmwAthena
