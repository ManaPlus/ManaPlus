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

#include "net/tmwa/charserverrecv.h"

#include "client.h"
#include "configuration.h"
#include "settings.h"

#include "gui/windows/charcreatedialog.h"
#include "gui/windows/okdialog.h"

#include "gui/widgets/createwidget.h"

#include "net/character.h"
#include "net/charserverhandler.h"
#include "net/messagein.h"
#include "net/serverfeatures.h"

#include "net/ea/token.h"

#include "net/tmwa/gamehandler.h"
#include "net/tmwa/loginhandler.h"
#include "net/tmwa/network.h"
#include "net/tmwa/sprite.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

namespace TmwAthena
{

extern ServerInfo mapServer;

extern ServerInfo charServer;

void CharServerRecv::readPlayerData(Net::MessageIn &msg,
                                    Net::Character *const character)
{
    if (character == nullptr)
        return;

    const Token &token =
        static_cast<LoginHandler*>(loginHandler)->getToken();

    LocalPlayer *const tempPlayer = new LocalPlayer(
        msg.readBeingId("account id"), BeingTypeId_zero);

    PlayerInfoBackend &data = character->data;
    data.mAttributes[Attributes::PLAYER_EXP] = msg.readInt32("exp");
    data.mAttributes[Attributes::MONEY] = msg.readInt32("money");
    Stat &jobStat = data.mStats[Attributes::PLAYER_JOB];
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

    data.mAttributes[Attributes::PLAYER_HP] = msg.readInt16("hp");
    data.mAttributes[Attributes::PLAYER_MAX_HP] = msg.readInt16("max hp");
    data.mAttributes[Attributes::PLAYER_MP] = msg.readInt16("mp");
    data.mAttributes[Attributes::PLAYER_MAX_MP] = msg.readInt16("max mp");

    msg.readInt16("speed");
    const uint16_t race = msg.readInt16("class");
    const uint8_t hairStyle = msg.readUInt8("hair style");
    const uint16_t look = msg.readUInt8("look");
    tempPlayer->setSubtype(fromInt(race, BeingTypeId), look);
    const uint16_t weapon = msg.readInt16("weapon");
    tempPlayer->setSpriteId(SPRITE_BODY,
        weapon);
    tempPlayer->setWeaponId(weapon);

    data.mAttributes[Attributes::PLAYER_LEVEL] = msg.readInt16("level");

    msg.readInt16("skill point");
    const int bottomClothes = msg.readInt16("bottom clothes");
    const int shield = msg.readInt16("shield");

    const int hat = msg.readInt16("hat");
    const int topClothes = msg.readInt16("top clothes");

    const ItemColor hairColor = fromInt(
        msg.readUInt8("hair color"), ItemColor);
    msg.readUInt8("unused");
    if (hairStyle == 0)
    {
        tempPlayer->unSetSprite(SPRITE_HAIR_COLOR);
    }
    else
    {
        tempPlayer->setSpriteColor(SPRITE_HAIR_COLOR,
            hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
    }
    tempPlayer->setHairColor(hairColor);

    const int misc2 = msg.readInt16("misc2");
    tempPlayer->setName(msg.readString(24, "name"));

    character->dummy = tempPlayer;

    character->data.mStats[Attributes::PLAYER_STR].base = msg.readUInt8("str");
    character->data.mStats[Attributes::PLAYER_AGI].base = msg.readUInt8("agi");
    character->data.mStats[Attributes::PLAYER_VIT].base = msg.readUInt8("vit");
    character->data.mStats[Attributes::PLAYER_INT].base = msg.readUInt8("int");
    character->data.mStats[Attributes::PLAYER_DEX].base = msg.readUInt8("dex");
    character->data.mStats[Attributes::PLAYER_LUK].base = msg.readUInt8("luk");

    tempPlayer->setSpriteId(SPRITE_HAIR,
        shoes);
    tempPlayer->setSpriteId(SPRITE_SHOES,
        gloves);
    tempPlayer->setSpriteId(SPRITE_SHIELD,
        cape);
    tempPlayer->setSpriteId(SPRITE_HEAD_TOP,
        misc1);
    tempPlayer->setSpriteId(SPRITE_WEAPON,
        bottomClothes);
    tempPlayer->setSpriteId(SPRITE_FLOOR,
        shield);
    tempPlayer->setSpriteId(SPRITE_CLOTHES_COLOR,
        hat);
    tempPlayer->setSpriteId(SPRITE_HEAD_BOTTOM,
        topClothes);
    tempPlayer->setSpriteId(SPRITE_HEAD_MID,
        misc2);

    character->slot = msg.readUInt8("slot");
    const uint8_t sex = CAST_U8(msg.readUInt8("gender"));
    if (serverFeatures->haveCreateCharGender())
        tempPlayer->setGender(Being::intToGender(sex));
    else
        tempPlayer->setGender(token.sex);
}

void CharServerRecv::processCharLogin(Net::MessageIn &msg)
{
    BLOCK_START("CharServerRecv::processCharLogin")

    msg.readInt16("len");
    const int slots = msg.readInt16("slots");
    if (slots > 0 && slots < 30)
        loginData.characterSlots = CAST_U16(slots);

    msg.skip(18, "unused");

    delete_all(charServerHandler->mCharacters);
    charServerHandler->mCharacters.clear();

    // Derive number of characters from message length
    const int count = (msg.getLength() - 24) / 106;

    for (int i = 0; i < count; ++i)
    {
        Net::Character *const character = new Net::Character;
        readPlayerData(msg, character);
        charServerHandler->mCharacters.push_back(character);
        if (character->dummy != nullptr)
        {
            logger->log("CharServer: Player: %s (%d)",
                character->dummy->getName().c_str(), character->slot);
        }
    }

    client->setState(State::CHAR_SELECT);
    BLOCK_END("CharServerRecv::processCharLogin")
}

void CharServerRecv::processCharMapInfo(Net::MessageIn &restrict msg)
{
    Network *const network = Network::mInstance;
    ServerInfo &server = mapServer;
    BLOCK_START("CharServerRecv::processCharMapInfo")
    PlayerInfo::setCharId(msg.readInt32("char id?"));
    const GameHandler *const gh = static_cast<GameHandler*>(gameHandler);
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
    localPlayer = charServerHandler->mSelectedCharacter->dummy;
    PlayerInfo::setBackend(charServerHandler->mSelectedCharacter->data);

    charServerHandler->mSelectedCharacter->dummy = nullptr;

    charServerHandler->clear();
    charServerHandler->updateCharSelectDialog();

    if (network != nullptr)
        network->disconnect();
    client->setState(State::CONNECT_GAME);
    BLOCK_END("CharServerRecv::processCharMapInfo")
}

void CharServerRecv::processChangeMapServer(Net::MessageIn &msg)
{
    Network *const network = Network::mInstance;
    ServerInfo &server = mapServer;
    BLOCK_START("CharServerRecv::processChangeMapServer")
    GameHandler *const gh = static_cast<GameHandler*>(gameHandler);
    if ((gh == nullptr) || (network == nullptr))
    {
        BLOCK_END("CharServerRecv::processChangeMapServer")
        return;
    }
    gh->setMap(msg.readString(16, "map name"));
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
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

    network->disconnect();
    client->setState(State::CHANGE_MAP);
    if (localPlayer != nullptr)
    {
        localPlayer->setTileCoords(x, y);
        localPlayer->setMap(nullptr);
    }
    BLOCK_END("CharServerRecv::processChangeMapServer")
}

void CharServerRecv::processCharCreate(Net::MessageIn &msg)
{
    BLOCK_START("CharServerRecv::processCharCreate")
    Net::Character *const character = new Net::Character;
    readPlayerData(msg, character);
    charServerHandler->mCharacters.push_back(character);

    charServerHandler->updateCharSelectDialog();

    // Close the character create dialog
    if (charServerHandler->mCharCreateDialog != nullptr)
    {
        charServerHandler->mCharCreateDialog->scheduleDelete();
        charServerHandler->mCharCreateDialog = nullptr;
    }
    BLOCK_END("CharServerRecv::processCharCreate")
}

void CharServerRecv::processCharDeleteFailed(Net::MessageIn &msg)
{
    BLOCK_START("CharServerRecv::processCharDeleteFailed")
    charServerHandler->unlockCharSelectDialog();
    msg.readUInt8("error");
    CREATEWIDGET(OkDialog,
        // TRANSLATORS: error header
        _("Error"),
        // TRANSLATORS: error message
        _("Failed to delete character."),
        // TRANSLATORS: ok dialog button
        _("OK"),
        DialogType::ERROR,
        Modal_true,
        ShowCenter_true,
        nullptr,
        260);
    BLOCK_END("CharServerRecv::processCharDeleteFailed")
}

}  // namespace TmwAthena
