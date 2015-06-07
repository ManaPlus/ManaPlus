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

#include "net/eathena/charserverhandler.h"

#include "client.h"
#include "configuration.h"
#include "settings.h"

#include "gui/windows/charcreatedialog.h"
#include "gui/windows/charselectdialog.h"
#include "gui/windows/okdialog.h"

#include "net/character.h"
#include "net/serverfeatures.h"

#include "net/eathena/gamehandler.h"
#include "net/eathena/loginhandler.h"
#include "net/eathena/messageout.h"
#include "net/eathena/network.h"
#include "net/eathena/playerhandler.h"
#include "net/eathena/protocol.h"
#include "net/eathena/sprite.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

extern Net::CharServerHandler *charServerHandler;

namespace EAthena
{

extern ServerInfo charServer;
extern ServerInfo mapServer;

std::string CharServerHandler::mNewName;
uint32_t CharServerHandler::mPinSeed = 0;
BeingId CharServerHandler::mPinAccountId = BeingId_zero;
BeingId CharServerHandler::mRenameId = BeingId_zero;
bool CharServerHandler::mNeedCreatePin = false;

CharServerHandler::CharServerHandler() :
    MessageHandler(),
    Ea::CharServerHandler()
{
    mNewName.clear();
    mPinSeed = 0;
    mPinAccountId = BeingId_zero;
    mRenameId = BeingId_zero;
    mNeedCreatePin = false;

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
        SMSG_CHAR_CHECK_RENAME,
        SMSG_CHAR_RENAME,
        SMSG_CHAR_CHANGE_SLOT,
        SMSG_CHAR_CAPTCHA_NOT_SUPPORTED,
        SMSG_CHAR_DELETE2_ACK,
        SMSG_CHAR_DELETE2_ACCEPT_ACTUAL_ACK,
        SMSG_CHAR_DELETE2_CANCEL_ACK,
        SMSG_CHAR_CHARACTERS,
        SMSG_CHAR_BAN_CHAR_LIST,
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

        case SMSG_CHAR_CHECK_RENAME:
            processCharCheckRename(msg);
            break;

        case SMSG_CHAR_RENAME:
            processCharRename(msg);
            break;

        case SMSG_CHAR_CHANGE_SLOT:
            processCharChangeSlot(msg);
            break;

        case SMSG_CHAR_CAPTCHA_NOT_SUPPORTED:
            processCharCaptchaNotSupported(msg);
            break;

        case SMSG_CHAR_DELETE2_ACK:
            processCharDelete2Ack(msg);
            break;

        case SMSG_CHAR_DELETE2_ACCEPT_ACTUAL_ACK:
            processCharDelete2AcceptActual(msg);
            break;

        case SMSG_CHAR_DELETE2_CANCEL_ACK:
            processCharDelete2CancelAck(msg);
            break;

        case SMSG_CHAR_CHARACTERS:
            processCharCharacters(msg);
            break;

        case SMSG_CHAR_BAN_CHAR_LIST:
            processCharBanCharList(msg);
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
        static_cast<LoginHandler*>(loginHandler)->getToken();

    LocalPlayer *const tempPlayer = new LocalPlayer(
        msg.readBeingId("player id"), BeingTypeId_zero);
    tempPlayer->setGender(token.sex);

    PlayerInfoBackend &data = character->data;
    data.mAttributes[Attributes::EXP] = msg.readInt32("exp");
    data.mAttributes[Attributes::MONEY] = msg.readInt32("money");
    Stat &jobStat = data.mStats[Attributes::JOB];
    jobStat.exp = msg.readInt32("job");

    const int temp = msg.readInt32("job level");
    jobStat.base = temp;
    jobStat.mod = temp;

    msg.readInt16("shoes?");
    const int gloves = msg.readInt16("gloves");
    const int cape = msg.readInt16("cape");
    const int misc1 = msg.readInt16("misc1");

    msg.readInt32("option");
    tempPlayer->setKarma(msg.readInt32("karma"));
    tempPlayer->setManner(msg.readInt32("manner"));
    msg.readInt16("left points");

    data.mAttributes[Attributes::HP] = msg.readInt32("hp");
    data.mAttributes[Attributes::MAX_HP] = msg.readInt32("max hp");
    data.mAttributes[Attributes::MP] = msg.readInt16("mp/sp");
    data.mAttributes[Attributes::MAX_MP] = msg.readInt16("max mp/sp");

    msg.readInt16("speed");
    const uint16_t race = msg.readInt16("class");
//    tempPlayer->setSubtype(race, 0);
    const int hairStyle = msg.readInt16("hair style");
    const int weapon = msg.readInt32("weapon");

    tempPlayer->setSprite(SPRITE_BODY, weapon, "", 1, true);

    data.mAttributes[Attributes::LEVEL] = msg.readInt16("level");

    msg.readInt16("skill points");
    const int bottomClothes = msg.readInt16("head bottom");
    const int shield = msg.readInt16("shild");
    const int hat = msg.readInt16("head top");
    const int topClothes = msg.readInt16("head mid");

    const uint16_t color = msg.readInt16("hair color");
    tempPlayer->setHairColor(static_cast<unsigned char>(color));
    tempPlayer->setSprite(SPRITE_HAIR_COLOR, hairStyle * -1,
        ItemDB::get(-hairStyle).getDyeColorsString(
        color));

    const uint16_t look = msg.readInt16("clothes color");
    tempPlayer->setSubtype(fromInt(race, BeingTypeId), look);
    tempPlayer->setName(msg.readString(24, "name"));

    character->dummy = tempPlayer;

    character->data.mStats[Attributes::STR].base = msg.readUInt8("str");
    character->data.mStats[Attributes::AGI].base = msg.readUInt8("agi");
    character->data.mStats[Attributes::VIT].base = msg.readUInt8("vit");
    character->data.mStats[Attributes::INT].base = msg.readUInt8("int");
    character->data.mStats[Attributes::DEX].base = msg.readUInt8("dex");
    character->data.mStats[Attributes::LUK].base = msg.readUInt8("luk");

    character->slot = msg.readInt16("character slot id");
    msg.readInt16("rename");
    msg.readString(16, "map name");
    msg.readInt32("delete date");
    const int shoes = msg.readInt32("robe");
    tempPlayer->setSprite(SPRITE_HAIR, shoes);
    tempPlayer->setSprite(SPRITE_SHOES, gloves);
    tempPlayer->setSprite(SPRITE_SHIELD, cape);
    tempPlayer->setSprite(SPRITE_HEAD_TOP, misc1);
    tempPlayer->setSprite(SPRITE_WEAPON, bottomClothes);
    tempPlayer->setSprite(SPRITE_FLOOR, shield);

    tempPlayer->setSprite(SPRITE_CLOTHES_COLOR, hat);
    tempPlayer->setSprite(SPRITE_HEAD_BOTTOM, topClothes);
//    tempPlayer->setSprite(SPRITE_HEAD_MID, misc2);
    msg.readInt32("slot change");
    tempPlayer->setRename(msg.readInt32("rename (inverse)"));

    const uint8_t gender = static_cast<uint8_t>(msg.readUInt8("gender"));
    if (gender != 99)
        tempPlayer->setGender(Being::intToGender(gender));
}

void CharServerHandler::chooseCharacter(Net::Character *const character)
{
    if (!character)
        return;

    mSelectedCharacter = character;
    mCharSelectDialog = nullptr;

    createOutPacket(CMSG_CHAR_SELECT);
    outMsg.writeInt8(static_cast<unsigned char>(
        mSelectedCharacter->slot), "slot");
}

void CharServerHandler::newCharacter(const std::string &name, const int slot,
                                     const GenderT gender,
                                     const int hairstyle, const int hairColor,
                                     const unsigned char race,
                                     const uint16_t look,
                                     const std::vector<int> &stats A_UNUSED)
                                     const
{
    createOutPacket(CMSG_CHAR_CREATE);
    outMsg.writeString(name, 24, "login");

    outMsg.writeInt8(static_cast<unsigned char>(slot), "slot");
    outMsg.writeInt16(static_cast<int16_t>(hairColor), "hair color");
    outMsg.writeInt16(static_cast<int16_t>(hairstyle), "hair style");
    if (serverFeatures->haveRaceSelection())
        outMsg.writeInt16(static_cast<int16_t>(race), "race");
    if (serverFeatures->haveCreateCharGender())
    {
        uint8_t sex = 0;
        if (gender == Gender::UNSPECIFIED)
            sex = 99;
        else
            sex = Being::genderToInt(gender);
        outMsg.writeInt8(sex, "gender");
    }
    if (serverFeatures->haveLookSelection())
        outMsg.writeInt16(static_cast<int16_t>(look), "look");
}

void CharServerHandler::deleteCharacter(Net::Character *const character,
                                        const std::string &email)
{
    if (!character)
        return;

    mSelectedCharacter = character;

    createOutPacket(CMSG_CHAR_DELETE);
    outMsg.writeBeingId(mSelectedCharacter->dummy->getId(), "id?");
    if (email.empty())
        outMsg.writeString("a@a.com", 40, "email");
    else
        outMsg.writeString(email, 40, "email");
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
    outMsg.writeBeingId(token.account_ID, "account id");
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

    const Token &token = static_cast<LoginHandler*>(loginHandler)->getToken();

    mCharCreateDialog->setAttributes(attributes, 0, 0, 0);
    mCharCreateDialog->setDefaultGender(token.sex);
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
    // ignored
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
    GameHandler *const gh = static_cast<GameHandler*>(gameHandler);
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
    PlayerInfo::setStatBase(Attributes::WALK_SPEED,
        playerHandler->getDefaultWalkSpeed().x);

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
    if (config.getBoolValue("usePersistentIP") || settings.persistentIp)
    {
        msg.readInt32("host");
        server.hostname = settings.serverName;
    }
    else
    {
        server.hostname = ipToString(msg.readInt32("host"));
    }
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
    mPinAccountId = msg.readBeingId("account id");
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
            UNIMPLIMENTEDPACKET;
            break;
    }
}

void CharServerHandler::setNewPincode(const std::string &pin A_UNUSED)
{
//  here need ecript pin with mPinSeed and pin values.

//    createOutPacket(CMSG_CHAR_CREATE_PIN);
//    outMsg.writeBeingId(mPinAccountId, "account id");
//    outMsg.writeString(pin, 4, "encrypted pin");
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

void CharServerHandler::renameCharacter(const BeingId id,
                                        const std::string &newName)
{
    createOutPacket(CMSG_CHAR_CHECK_RENAME);
    mRenameId = id;
    mNewName = newName;
    outMsg.writeBeingId(id, "char id");
    outMsg.writeString(newName, 24, "name");
}

void CharServerHandler::processCharCheckRename(Net::MessageIn &msg)
{
    if (msg.readInt16("flag"))
    {
        createOutPacket(CMSG_CHAR_RENAME);
        outMsg.writeBeingId(mRenameId, "char id");
    }
    else
    {
        // TRANSLATORS: info message
        (new OkDialog(_("Error"), _("Character rename error."),
            // TRANSLATORS: ok dialog button
            _("Error"),
            DialogType::ERROR,
            Modal_true,
            ShowCenter_true,
            nullptr,
            260))->postInit();
    }
}

void CharServerHandler::processCharRename(Net::MessageIn &msg)
{
    const int flag = msg.readInt16("flag");
    if (!flag)
    {
        mCharSelectDialog->setName(mRenameId, mNewName);
        // TRANSLATORS: info message
        (new OkDialog(_("Info"), _("Character renamed."),
            // TRANSLATORS: ok dialog button
            _("OK"),
            DialogType::OK,
            Modal_true,
            ShowCenter_true,
            nullptr,
            260))->postInit();
    }
    else
    {
        std::string message;
        switch (flag)
        {
            case 1:
                // TRANSLATORS: char rename error
                message = _("Rename not allowed.");
                break;
            case 2:
                // TRANSLATORS: char rename error
                message = _("New name is not set.");
                break;
            case 3:
            default:
                // TRANSLATORS: char rename error
                message = _("Character rename error.");
                break;
            case 4:
                // TRANSLATORS: char rename error
                message = _("Character not found.");
                break;
        }
        // TRANSLATORS: info message
        (new OkDialog(_("Info"), message,
            // TRANSLATORS: ok dialog button
            _("OK"),
            DialogType::OK,
            Modal_true,
            ShowCenter_true,
            nullptr,
            260))->postInit();
    }
}

void CharServerHandler::changeSlot(const int oldSlot, const int newSlot)
{
    createOutPacket(CMSG_CHAR_CHANGE_SLOT);
    outMsg.writeInt16(static_cast<int16_t>(oldSlot), "old slot");
    outMsg.writeInt16(static_cast<int16_t>(newSlot), "new slot");
    outMsg.writeInt16(0, "unused");
}

void CharServerHandler::processCharChangeSlot(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("len");
    msg.readInt16("flag");  // 0 - ok, 1 - error
    msg.readInt16("unused");
}

void CharServerHandler::processCharDeleteFailed(Net::MessageIn &msg)
{
    BLOCK_START("CharServerHandler::processCharDeleteFailed")
    unlockCharSelectDialog();
    msg.readUInt8("error");
    // TRANSLATORS: error message
    (new OkDialog(_("Error"), _("Failed to delete character."),
        // TRANSLATORS: ok dialog button
        _("OK"),
        DialogType::ERROR,
        Modal_true,
        ShowCenter_true,
        nullptr,
        260))->postInit();
    BLOCK_END("CharServerHandler::processCharDeleteFailed")
}

void CharServerHandler::processCharCaptchaNotSupported(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("5");
    msg.readUInt8("1");
}

void CharServerHandler::processCharDelete2Ack(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("char id");
    msg.readInt32("result");
    msg.readInt32("time");
}

void CharServerHandler::processCharDelete2AcceptActual(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("char id");
    msg.readInt32("result");
}

void CharServerHandler::processCharDelete2CancelAck(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("char id");
    msg.readInt32("result");
}

void CharServerHandler::processCharCharacters(Net::MessageIn &msg)
{
    msg.skip(2, "packet len");

    delete_all(mCharacters);
    mCharacters.clear();

    // Derive number of characters from message length
    const int count = (msg.getLength() - 4)
        / (106 + 4 + 2 + 16 + 4 + 4 + 4 + 4);

    for (int i = 0; i < count; ++i)
    {
        Net::Character *const character = new Net::Character;
        charServerHandler->readPlayerData(msg, character, false);
        mCharacters.push_back(character);
        if (character->dummy)
        {
            logger->log("CharServer: Player: %s (%d)",
                character->dummy->getName().c_str(), character->slot);
        }
    }

    client->setState(STATE_CHAR_SELECT);
}

void CharServerHandler::ping() const
{
    createOutPacket(CMSG_CHAR_PING);
    outMsg.writeInt32(0, "unused");
}

unsigned int CharServerHandler::hatSprite() const
{
    return 7;
}

void CharServerHandler::processCharBanCharList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    const int count = (msg.readInt16("len") - 4) / 24;
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("char id");
        msg.readString(20, "unbun time");
    }
}

}  // namespace EAthena
